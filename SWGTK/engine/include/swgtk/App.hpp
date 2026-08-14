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
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_APP_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_APP_HPP_

#include <SDL3/SDL_video.h>
#include <memory>
#include <string>
#include <swgtk/Input.hpp>
#include <swgtk/Props.hpp>
#include <swgtk/RenderingDevice.hpp>
#include <swgtk/Timer.hpp>
#include <swgtk/Utility.hpp>
#include <utility>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <SDL3/SDL_init.h>
#include <swgtk/Scene.hpp>
#include <swgtk/Surface.hpp>

extern "C" {
struct SDL_Window;
struct SDL_Renderer;
}

namespace swgtk {

  enum class system_init : uint32_t {
    none = 0u,
    audio = SDL_INIT_AUDIO,
    video = SDL_INIT_VIDEO,
    joystick = SDL_INIT_JOYSTICK,
    haptic = SDL_INIT_HAPTIC,
    gamepad = SDL_INIT_GAMEPAD,
    events = SDL_INIT_EVENTS,
    sensor = SDL_INIT_SENSOR,
    camera = SDL_INIT_CAMERA,
  };

  enum class window_flags : uint64_t {
    fullscreen         =  SDL_UINT64_C(0x0000000000000001),    /**< window is in fullscreen mode */
    opengl             =  SDL_UINT64_C(0x0000000000000002),    /**< window usable with OpenGL context */
    occluded           =  SDL_UINT64_C(0x0000000000000004),    /**< window is occluded */
    hidden             =  SDL_UINT64_C(0x0000000000000008),    /**< window is neither mapped onto the desktop nor shown in the taskbar/dock/window list; SDL_ShowWindow() is required for it to become visible */
    borderless         =  SDL_UINT64_C(0x0000000000000010),    /**< no window decoration */
    resizable          =  SDL_UINT64_C(0x0000000000000020),    /**< window can be resized */
    minimized          =  SDL_UINT64_C(0x0000000000000040),    /**< window is minimized */
    maximized          =  SDL_UINT64_C(0x0000000000000080),    /**< window is maximized */
    mouse_grabbed       =  SDL_UINT64_C(0x0000000000000100),   /**< window has grabbed mouse input */
    input_focus         =  SDL_UINT64_C(0x0000000000000200),    /**< window has input focus */
    mouse_focus         =  SDL_UINT64_C(0x0000000000000400),    /**< window has mouse focus */
    external           =  SDL_UINT64_C(0x0000000000000800),    /**< window not created by SDL */
    modal              =  SDL_UINT64_C(0x0000000000001000),    /**< window is modal */
    high_pixel_density   =  SDL_UINT64_C(0x0000000000002000),    /**< window uses high pixel density back buffer if possible */
    mouse_capture       =  SDL_UINT64_C(0x0000000000004000),    /**< window has mouse captured (unrelated to MOUSE_GRABBED) */
    mouse_relative_mode =  SDL_UINT64_C(0x0000000000008000),    /**< window has relative mode enabled */
    always_on_top        =  SDL_UINT64_C(0x0000000000010000),    /**< window should always be above others */
    utility            =  SDL_UINT64_C(0x0000000000020000),    /**< window should be treated as a utility window, not showing in the task bar and window list */
    tooltip            =  SDL_UINT64_C(0x0000000000040000),    /**< window should be treated as a tooltip and does not get mouse or keyboard focus, requires a parent window */
    popup_menu          =  SDL_UINT64_C(0x0000000000080000),   /**< window should be treated as a popup menu, requires a parent window */
    keyboard_grabbed    =  SDL_UINT64_C(0x0000000000100000),    /**< window has grabbed keyboard input */
    fill_document       =  SDL_UINT64_C(0x0000000000200000),    /**< window is in fill-document mode (Emscripten only), since SDL 3.4.0 */
    vulkan             =  SDL_UINT64_C(0x0000000010000000),    /**< window usable for Vulkan surface */
    metal              =  SDL_UINT64_C(0x0000000020000000),    /**< window usable for Metal view */
    transparent        =  SDL_UINT64_C(0x0000000040000000),    /**< window with transparent buffer */
    not_focusable       =  SDL_UINT64_C(0x0000000080000000),    /**< window should not be focusable */
  };

  struct window_config {
    std::string window_title;
    int width{};
    int height{};
    int pos_x = SDL_WINDOWPOS_CENTERED;
    int pos_y = SDL_WINDOWPOS_CENTERED;
    float opacity = 1.0f;
    surface icon;
    window_flags flags = window_flags::hidden;
  };

  /**
    @brief This class is the root manager of the SWGTK framework.

    app is where the engine code lives. Your code is injected during initialization, and
    app manages the lifetime of your game classes. (See swgtk::Scene::Node for more details)
   */
  class app {
  public:
    app() = default;
    app(const std::string& name, int win_width, int win_height, std::shared_ptr<rendering_device>&& render_ptr, system_init sys_flags = system_init::video);
    // app(std::initializer_list<std::pair<std::string_view, props::PropValue>>&&);
    app(const app&) = delete;
    app(app&&) noexcept = delete;
    auto operator=(const app&) -> app& = delete;
    auto operator=(app&&) noexcept -> app& = delete;
    ~app();

    [[nodiscard]] auto app_name(const std::string& name) -> app&; // Real name of your application, can be different from window title.
    [[nodiscard]] auto window_title(const std::string& title) -> app&; // app window title, can be different from name.
    [[nodiscard]] auto app_size(const std::pair<int, int> &dimensions) -> app&;
    [[nodiscard]] auto app_pos(const std::pair<int, int> &position) -> app&;
    [[nodiscard]] auto app_renderer(std::shared_ptr<rendering_device>&& render_ptr) -> app&;
    [[nodiscard]] auto sub_systems(system_init sysFlags) -> app&;
    [[nodiscard]] auto app_icon(const std::string &iconPath) -> app&;
    [[nodiscard]] auto app_opacity(float opacity) -> app&;
    [[nodiscard]] auto fullscreen() -> app&;

    [[nodiscard]] auto build() -> bool;

    /**
     * @brief Starts up the application's framework. There is no need to call this function yourself
     * unless you require an advanced setup.
     *
     * @return true on successful initialization
     * @return false on failure
     */
    [[nodiscard]] auto initialize_game() const -> bool;

    /**
     * @brief After confirming your app is initialized, call this function to start the main loop.
     *
     * @tparam T Your application's startup Scene::Node 
     * @param args Arguments your application needs
     */
    template<std::derived_from<scene::node> T>
    constexpr void run_game(auto&&... args) noexcept {
      if (make_scene<T>(std::forward<decltype(args)>(args)...)) {
        run();
      }
    }

    /**
     * @brief Works like RunGame() except you also pass in a custom Scene class to manage your game.
     *
     * @tparam Manager Your application's container class
     * @tparam Root Your application's startup Scene::Node
     *
     * @param argsMan Arguments to your Manager class
     * @param argsRoot Arguments to your Root Scene::Node
     */
    template<
    std::derived_from<scene> Manager,
    std::derived_from<scene::node> Root,
    typename...ArgsMan,
    typename... ArgsRoot>
    constexpr void run_game_ext(std::tuple<ArgsMan&&...> argsMan, std::tuple<ArgsRoot&&...> argsRoot) noexcept {
      if (make_scene_ext<Manager, Root>(std::forward_as_tuple(argsMan), std::forward_as_tuple(argsRoot))) {
        run();
      }
    }

    /**
     * @brief Works like MakeScene() except you also pass in a custom Scene class to manage your game.
     *
     * @tparam Manager Your application's container class
     * @tparam Root Your application's startup Scene::Node
     *
     * @param argsMan Arguments to your Manager class
     * @param argsRoot Arguments to your Root Scene::Node
     */
    template<
    std::derived_from<scene> Manager,
    std::derived_from<scene::node> Root,
    typename...ArgsMan,
    typename... ArgsRoot>
    constexpr auto make_scene_ext(std::tuple<ArgsMan&&...> argsMan, std::tuple<ArgsRoot&&...> argsRoot) -> bool {
      _currentScene = std::make_unique<Manager>(object_ref<app>{this}, std::forward<ArgsMan>(argsMan)...);
      _currentScene->add_root_node<Root>(std::forward<ArgsRoot>(argsRoot)...);
      return _currentScene->create();
    }

    /**
     * @brief Create the game scene along with the root Scene::Node You defined in the template arguments.
     */
    template<std::derived_from<scene::node> T>
    constexpr auto make_scene(auto&&... args) -> bool {
      _currentScene = std::make_unique<scene>(object_ref<app>{this});
      _currentScene->add_root_node<T>(std::forward<decltype(args)>(args)...);
      return _currentScene->create();
    }

    void events_and_time_step();

#ifdef __EMSCRIPTEN__
    void GameTick() const {
#else
    [[nodiscard]] auto game_tick() const -> bool {
#endif

      [[maybe_unused]] const bool result = _currentScene->update(_gameTimer.get_seconds());

      _renderer->buffer_present();
#ifndef __EMSCRIPTEN__
      return result;
#endif
    }

    void close_app();
    [[nodiscard]] auto is_running() const -> bool { return _running; }

    void set_window_size(const int w, const int h) const { SDL_SetWindowSize(_window, w, h); }
    void set_title(const std::string& value) const { SDL_SetWindowTitle(_window, value.c_str()); }
    void set_fullscreen(const bool value) const { SDL_SetWindowFullscreen(_window, value); }
    void show_window() const { SDL_ShowWindow(_window); }
    void hide_window() const { SDL_HideWindow(_window); }
    void raise_window() const { SDL_RaiseWindow(_window); }
    void restore_window() const { SDL_RestoreWindow(_window); }
    void maximize_window() const { SDL_MaximizeWindow(_window); }
    void minimize_window() const { SDL_MinimizeWindow(_window); }

    [[nodiscard]] auto get_internal_clock() -> timer* { return &_gameTimer; }

    [[nodiscard]] auto renderer(this auto&& self) -> std::weak_ptr<rendering_device> { return self._renderer; }
    [[nodiscard]] constexpr auto window(this auto&& self) -> SDL_Window* { return self._window; }

    [[nodiscard]] auto get_window_size() const -> std::pair<int, int> {
      int width{}, height{};
      SDL_GetWindowSize(_window, &width, &height);

      return std::make_pair(width, height);
    }

    [[nodiscard]] auto is_fullscreen_borderless() const -> bool { return SDL_GetWindowFullscreenMode(_window) == nullptr; }

    /*
    Input state and event polling for the client's logic.
    */

    [[nodiscard]] constexpr auto get_scroll_x() const { return _input.scroll.x; }
    [[nodiscard]] constexpr auto get_scroll_y() const { return _input.scroll.y; }
    [[nodiscard]] constexpr auto is_key_pressed(const layout_code code) const -> bool { return (_input.keyEvent.first == code && _input.keyEvent.second); }
    [[nodiscard]] constexpr auto is_key_released(const layout_code code) const -> bool { return (_input.keyEvent.first == code && !_input.keyEvent.second); }
    [[nodiscard]] constexpr auto is_key_held(layout_code code) const -> bool { return _input.keyboardState[static_cast<size_t>(code)]; }
    [[nodiscard]] constexpr auto get_current_key_event() const -> std::pair<layout_code, bool> { return _input.keyEvent; }
    [[nodiscard]] constexpr auto get_key_mods() const -> key_mod { return _input.modifiers; }
    [[nodiscard]] constexpr auto is_button_pressed(m_button button) const -> bool { return _input.mouseEvents.at(static_cast<uint32_t>(button)).state == m_button_state::pressed; }

    [[nodiscard]] constexpr auto is_button_released(m_button button) const -> bool { return _input.mouseEvents.at(static_cast<uint32_t>(button)).state == m_button_state::released; }
    [[nodiscard]] constexpr auto is_button_held(m_button button) const -> bool { return static_cast<bool>(static_cast<uint32_t>(_input.mouseState.buttons) & static_cast<uint32_t>(button)); }
    [[nodiscard]] constexpr auto get_mouse_clicks(m_button button) const -> uint8_t { return _input.mouseEvents.at(static_cast<uint32_t>(button)).clicks; }
    [[nodiscard]] constexpr auto get_mouse_x() const { return _input.mouseState.x; }
    [[nodiscard]] constexpr auto get_mouse_y() const { return _input.mouseState.y; }
    [[nodiscard]] constexpr auto get_mouse_pos() const { return SDL_FPoint{.x=_input.mouseState.x, .y=_input.mouseState.y}; }

    /*
      Input state and event management. For internal use only.
    */
  private:
    void update_mouse_state() { _input.mouseState.buttons = m_button{SDL_GetMouseState(&_input.mouseState.x, &_input.mouseState.y)}; }
    constexpr void set_mod_state(const SDL_Keymod& state) { _input.modifiers = static_cast<key_mod>(state); }
    constexpr void reset_scroll() { _input.scroll = {.x = 0.f, .y = 0.f}; }
    constexpr void add_scroll(const float amountX, const float amountY) { _input.scroll = {.x = amountX, .y = amountY}; }
    constexpr void set_mouse_event(m_button button, const m_button_data data) { _input.mouseEvents.at(static_cast<size_t>(button)) = data; }

    constexpr void reset_key_event() {
      _input.keyEvent.first = layout_code::unknown;
      _input.keyEvent.second = false;
    }

    constexpr void set_key_event(const layout_code code, const bool pressed) {
      _input.keyEvent.first = code;
      _input.keyEvent.second = pressed;
    }

    void set_keyboard_state() {
      int num_keys{};
      const bool* state = SDL_GetKeyboardState(&num_keys);
      _input.keyboardState = std::span<const bool>{state, static_cast<size_t>(num_keys)};
    }

    constexpr void reset_mouse_events() {
      for (auto& [state, clicks]: _input.mouseEvents) {
        state = m_button_state::none;
        clicks = 0u;
      }
    }


#ifdef __EMSCRIPTEN__

    static void EmscriptenUpdate(void* ptr);

#endif // __EMSCRIPTEN__

    void run();
    window_config _win_config{};
    std::string _app_name;
    system_init _sys_flags = system_init::video;
    SDL_Window* _window = nullptr;
    std::shared_ptr<rendering_device> _renderer;
    std::unique_ptr<scene> _currentScene;

    input_system _input;
    timer _gameTimer;

    bool _running = true;
  };
} // namespace swgtk
#endif // SWGTK_ENGINE_INCLUDE_SWGTK_APP_HPP_
