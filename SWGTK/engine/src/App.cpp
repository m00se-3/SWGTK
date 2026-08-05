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
#include <swgtk/Utility.hpp>
#include <swgtk/Props.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_properties.h>

#include <memory>
#include <string>
#include <utility>


namespace swgtk {
  App::~App() {
    _currentScene.reset();
    _renderer.reset();
    SDL_DestroyWindow(_window);

    TTF_Quit();
    SDL_Quit();
  }

  App::App(const std::string& appName, const int winWidth, const int winHeight, std::shared_ptr<RenderingDevice>&& renderPtr, const SystemInit sysFlags) 
  : _winConfig{.windowTitle = appName, .width = winWidth, .height = winHeight, .icon{}}, _sysFlags(sysFlags), _renderer(std::move(renderPtr)) {}

  // App::App(std::initializer_list<std::pair<std::string_view, props::PropValue>>&& properites) {
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

  auto App::AppName(const std::string& _appName) -> App& {
    appName = _appName;
    return *this;
  }

  auto App::WindowTitle(const std::string& title) -> App& {
    _winConfig.windowTitle = title;
    return *this;
  }

  auto App::AppSize(std::pair<int, int> dimensions) -> App& {
    _winConfig.width = dimensions.first;
    _winConfig.height = dimensions.second;
    return *this;
  }

  auto App::AppPos(std::pair<int, int> position) -> App& {
    _winConfig.posX = position.first;
    _winConfig.posY = position.second;
    return *this;
  }

  auto App::SubSystems(const SystemInit sysFlags) -> App& {
    _sysFlags = sysFlags;
    return *this;
  }

  auto App::AppRenderer(std::shared_ptr<RenderingDevice>&& renderPtr) -> App& {
    _renderer = std::move(renderPtr);
    return *this;
  }

  auto App::AppOpacity(const float opacity) -> App& {
    _winConfig.opacity = opacity;
    return *this;
  }

  auto App::AppIcon(const std::string& iconPath) -> App& {
    _winConfig.icon = Surface{IMG_Load(iconPath.c_str())};
    return *this;
  }

  auto App::Fullscreen() -> App& {
    _winConfig.flags = _winConfig.flags | WindowFlags::Fullscreen;
    return *this;
  }

  auto App::Build() -> bool {
    SDL_SetAppMetadata(appName.c_str(), nullptr, nullptr);
    if (SDL_Init(std::to_underlying(_sysFlags)) && TTF_Init()) {

      // false positive
      // cppcheck-suppress syntaxError
      if (_window = SDL_CreateWindow(_winConfig.windowTitle.c_str(), _winConfig.width, _winConfig.height, std::to_underlying(_winConfig.flags)); _window != nullptr) {
        SDL_SetWindowPosition(_window, _winConfig.posX, _winConfig.posY);

        if (_winConfig.opacity != 1.0f) {
          SDL_SetWindowOpacity(_window, _winConfig.opacity);
        }
        if (!_winConfig.icon.Empty()) {
          SDL_SetWindowIcon(_window, *_winConfig.icon);
        }

        return InitializeGame();
      }
    }

    DEBUG_PRINT("SDL failed to initialize. - {}\n", SDL_GetError())
    return false;
  }

  void App::EventsAndTimeStep() {
    SDL_Event e;

    ResetScroll();
    ResetMouseEvents();
    ResetKeyEvent();

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_EVENT_MOUSE_BUTTON_UP:
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
          SetMouseEvent(MButton{e.button.button}, MButtonData{
                                                      .state = (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) ? MButtonState::Pressed : MButtonState::Released,
                                                      .clicks = e.button.clicks});
          break;
        }

        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP: {
          SetKeyEvent(static_cast<LayoutCode>(e.key.scancode), (e.type == SDL_EVENT_KEY_DOWN));
          break;
        }

        case SDL_EVENT_MOUSE_WHEEL: {
          AddScroll(e.wheel.x, e.wheel.y);
          break;
        }

        case SDL_EVENT_QUIT: {
          CloseApp();
          break;
        }
        default: {
          // Unsupported event.
        }
      }
    }


    SetKeyboardState();
    SetModState(SDL_GetModState());

    UpdateMouseState();

    _gameTimer.UpdateTime();
  }

  auto App::InitializeGame() const -> bool {
    if (_renderer->PrepareDevice(_window)) {
      return true;
    }

    DEBUG_PRINT("Failed to initialize rendering context. - {}\n", SDL_GetError())
    return false;
  }

  void App::Run() {
    if ((SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO) == SDL_INIT_VIDEO) {
      ShowWindow();
      SDL_SyncWindow(_window); // Make sure window is ready before starting the simulation.
    }
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(App::EmscriptenUpdate, this, -1, true);

#else

    bool gameOk = true;

    while (_running && gameOk) {
      EventsAndTimeStep();
      gameOk = GameTick();
    }

#endif // __EMSCRIPTEN__
  }

  void App::CloseApp() {
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();
#else
    _running = false;

#endif
  }

#ifdef __EMSCRIPTEN__
  void App::EmscriptenUpdate(void* ptr) {
    auto* app = static_cast<App*>(ptr);
    app->EventsAndTimeStep();
    app->GameTick();
  }
#endif


} // namespace swgtk
