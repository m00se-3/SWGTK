/*
    MIT License
    Copyright (c) 2023 Samuel Bridgham

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#include <swgtk/App.hpp>
#include <swgtk/Props.hpp>
#include <swgtk/Utility.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <memory>
#include <string>
#include <utility>


namespace swgtk {
  app::~app() {
    _currentScene.reset();
    _renderer.reset();
    SDL_DestroyWindow(_window);

    TTF_Quit();
    SDL_Quit();
  }

  app::app(const std::string& name, const int win_width, const int win_height, std::shared_ptr<rendering_device>&& render_ptr, const system_init sys_flags)
  : _win_config{.window_title = name, .width = win_width, .height = win_height, .icon{}}, _sys_flags(sys_flags), _renderer(std::move(render_ptr)) {}

  // app::app(std::initializer_list<std::pair<std::string_view, props::PropValue>>&& properites) {
  //   auto props = SDL_CreateProperties();
  //
  //   for(auto&& [ propName, propValue ] : std::move(properites)) {
  //   std::visit(
  //     props::PropOverload {
  //       [&props, propName] (int64_t num) -> void {
  //         SDL_SetNumberProperty(props, propName.data(), num);
  //       },
  //       [&props, propName] (float num) -> void {
  //         SDL_SetFloatProperty(props, propName.data(), num);
  //       },
  //       [&props, propName] (std::string_view str) -> void {
  //         SDL_SetStringProperty(props, propName.data(), str.data());
  //       },
  //       [&props, propName] (void* ptr) -> void {
  //         SDL_SetPointerProperty(props, propName.data(), ptr);
  //       },
  //       [&props, propName] (bool flag) -> void {
  //         SDL_SetBooleanProperty(props, propName.data(), flag);
  //       },
  //       [] (const std::monostate&) -> void {
  //
  //       }
  //     }
  //   , propValue);
  //   }
  // }

  auto app::app_name(const std::string& name) -> app& {
    _app_name = name;
    return *this;
  }

  auto app::window_title(const std::string& title) -> app& {
    _win_config.window_title = title;
    return *this;
  }

  auto app::app_size(const std::pair<int, int> &dimensions) -> app& {
    _win_config.width = dimensions.first;
    _win_config.height = dimensions.second;
    return *this;
  }

  auto app::app_pos(const std::pair<int, int> &position) -> app& {
    _win_config.pos_x = position.first;
    _win_config.pos_y = position.second;
    return *this;
  }

  auto app::sub_systems(const system_init sysFlags) -> app& {
    _sys_flags = sysFlags;
    return *this;
  }

  auto app::app_renderer(std::shared_ptr<rendering_device>&& render_ptr) -> app& {
    _renderer = std::move(render_ptr);
    return *this;
  }

  auto app::app_opacity(const float opacity) -> app& {
    _win_config.opacity = opacity;
    return *this;
  }

  auto app::app_icon(const std::string& icon_path) -> app& {
    _win_config.icon = surface{IMG_Load(icon_path.c_str())};
    return *this;
  }

  auto app::fullscreen() -> app& {
    _win_config.flags = _win_config.flags | window_flags::fullscreen;
    return *this;
  }

  auto app::build() -> bool {
    SDL_SetAppMetadata(_app_name.c_str(), nullptr, nullptr);
    if (SDL_Init(std::to_underlying(_sys_flags)) && TTF_Init()) {

      // false positive
      // cppcheck-suppress syntaxError
      if (_window = SDL_CreateWindow(_win_config.window_title.c_str(), _win_config.width, _win_config.height, std::to_underlying(_win_config.flags)); _window != nullptr) {
        SDL_SetWindowPosition(_window, _win_config.pos_x, _win_config.pos_y);

        if (_win_config.opacity != 1.0f) {
          SDL_SetWindowOpacity(_window, _win_config.opacity);
        }
        if (!_win_config.icon.empty()) {
          SDL_SetWindowIcon(_window, *_win_config.icon);
        }

        return initialize_game();
      }
    }

    DEBUG_PRINT("SDL failed to initialize. - {}\n", SDL_GetError())
    return false;
  }

  void app::events_and_time_step() {
    SDL_Event e;

    reset_scroll();
    reset_mouse_events();
    reset_key_event();

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_EVENT_MOUSE_BUTTON_UP:
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
          set_mouse_event(m_button{e.button.button}, m_button_data{
                                                      .state = (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) ? m_button_state::pressed : m_button_state::released,
                                                      .clicks = e.button.clicks,});
          break;
        }

        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP: {
          set_key_event(static_cast<layout_code>(e.key.scancode), (e.type == SDL_EVENT_KEY_DOWN));
          break;
        }

        case SDL_EVENT_MOUSE_WHEEL: {
          add_scroll(e.wheel.x, e.wheel.y);
          break;
        }

        case SDL_EVENT_QUIT: {
          close_app();
          break;
        }
        default: {
          // Unsupported event.
        }
      }
    }


    set_keyboard_state();
    set_mod_state(SDL_GetModState());

    update_mouse_state();

    _gameTimer.update_time();
  }

  auto app::initialize_game() const -> bool {
    if (_renderer->prepare_device(_window)) {
      return true;
    }

    DEBUG_PRINT("Failed to initialize rendering context. - {}\n", SDL_GetError())
    return false;
  }

  void app::run() {
    if ((SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO) == SDL_INIT_VIDEO) {
      show_window();
      SDL_SyncWindow(_window); // Make sure window is ready before starting the simulation.
    }
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(app::EmscriptenUpdate, this, -1, true);

#else

    bool game_ok = true;

    while (_running && game_ok) {
      events_and_time_step();
      game_ok = game_tick();
    }

#endif // __EMSCRIPTEN__
  }

  void app::close_app() {
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
#else
    _running = false;

#endif
  }

#ifdef __EMSCRIPTEN__
  void app::EmscriptenUpdate(void* ptr) {
    auto* app = static_cast<app*>(ptr);
    app->EventsAndTimeStep();
    app->GameTick();
  }
#endif


} // namespace swgtk
