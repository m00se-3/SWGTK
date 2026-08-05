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
#include <swgtk/Timer.hpp>
#include <swgtk/Utility.hpp>
#include <swgtk/Props.hpp>
#include <utility>
#include <swgtk/Input.hpp>
#include <swgtk/RenderingDevice.hpp>

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

  enum class SystemInit : uint32_t {
    None = 0u,
    Audio = SDL_INIT_AUDIO,
    Video = SDL_INIT_VIDEO,
    Joystick = SDL_INIT_JOYSTICK,
    Haptic = SDL_INIT_HAPTIC,
    Gamepad = SDL_INIT_GAMEPAD,
    Events = SDL_INIT_EVENTS,
    Sensor = SDL_INIT_SENSOR,
    Camera = SDL_INIT_CAMERA,
  };

  enum class WindowFlags : uint64_t {
    Fullscreen         =  SDL_UINT64_C(0x0000000000000001),    /**< window is in fullscreen mode */
    Opengl             =  SDL_UINT64_C(0x0000000000000002),    /**< window usable with OpenGL context */
    Occluded           =  SDL_UINT64_C(0x0000000000000004),    /**< window is occluded */
    Hidden             =  SDL_UINT64_C(0x0000000000000008),    /**< window is neither mapped onto the desktop nor shown in the taskbar/dock/window list; SDL_ShowWindow() is required for it to become visible */
    Borderless         =  SDL_UINT64_C(0x0000000000000010),    /**< no window decoration */
    Resizable          =  SDL_UINT64_C(0x0000000000000020),    /**< window can be resized */
    Minimized          =  SDL_UINT64_C(0x0000000000000040),    /**< window is minimized */
    Maximized          =  SDL_UINT64_C(0x0000000000000080),    /**< window is maximized */
    MouseGrabbed       =  SDL_UINT64_C(0x0000000000000100),   /**< window has grabbed mouse input */
    InputFocus         =  SDL_UINT64_C(0x0000000000000200),    /**< window has input focus */
    MouseFocus         =  SDL_UINT64_C(0x0000000000000400),    /**< window has mouse focus */
    External           =  SDL_UINT64_C(0x0000000000000800),    /**< window not created by SDL */
    Modal              =  SDL_UINT64_C(0x0000000000001000),    /**< window is modal */
    HighPixelDensity   =  SDL_UINT64_C(0x0000000000002000),    /**< window uses high pixel density back buffer if possible */
    MouseCapture       =  SDL_UINT64_C(0x0000000000004000),    /**< window has mouse captured (unrelated to MOUSE_GRABBED) */
    MouseRelative_mode =  SDL_UINT64_C(0x0000000000008000),    /**< window has relative mode enabled */
    AlwaysOnTop        =  SDL_UINT64_C(0x0000000000010000),    /**< window should always be above others */
    Utility            =  SDL_UINT64_C(0x0000000000020000),    /**< window should be treated as a utility window, not showing in the task bar and window list */
    Tooltip            =  SDL_UINT64_C(0x0000000000040000),    /**< window should be treated as a tooltip and does not get mouse or keyboard focus, requires a parent window */
    PopupMenu          =  SDL_UINT64_C(0x0000000000080000),   /**< window should be treated as a popup menu, requires a parent window */
    KeyboardGrabbed    =  SDL_UINT64_C(0x0000000000100000),    /**< window has grabbed keyboard input */
    FillDocument       =  SDL_UINT64_C(0x0000000000200000),    /**< window is in fill-document mode (Emscripten only), since SDL 3.4.0 */
    Vulkan             =  SDL_UINT64_C(0x0000000010000000),    /**< window usable for Vulkan surface */
    Metal              =  SDL_UINT64_C(0x0000000020000000),    /**< window usable for Metal view */
    Transparent        =  SDL_UINT64_C(0x0000000040000000),    /**< window with transparent buffer */
    NotFocusable       =  SDL_UINT64_C(0x0000000080000000),    /**< window should not be focusable */
  };

  struct WindowConfig {
    std::string windowTitle;
    int width{};
    int height{};
    int posX = SDL_WINDOWPOS_CENTERED;
    int posY = SDL_WINDOWPOS_CENTERED;
    float opacity = 1.0f;
    Surface icon;
    WindowFlags flags = WindowFlags::Hidden;
  };

  /**
    @brief This class is the root manager of the SWGTK framework.

    App is where the engine code lives. Your code is injected during initialization, and
    App manages the lifetime of your game classes. (See swgtk::Scene::Node for more details)
   */
  class App {
  public:
    App() = default;
    App(const std::string& appName, int winWidth, int winHeight, std::shared_ptr<RenderingDevice>&& renderPtr, SystemInit sysFlags = SystemInit::Video);
    // App(std::initializer_list<std::pair<std::string_view, props::PropValue>>&&);
    App(const App&) = delete;
    App(App&&) noexcept = delete;
    auto operator=(const App&) -> App& = delete;
    auto operator=(App&&) noexcept -> App& = delete;
    ~App();

    [[nodiscard]] auto AppName(const std::string& _appName) -> App&; // Real name of your application, can be different from window title.
    [[nodiscard]] auto WindowTitle(const std::string& title) -> App&; // App window title, can be different from name.
    [[nodiscard]] auto AppSize(std::pair<int, int> dimensions) -> App&;
    [[nodiscard]] auto AppPos(std::pair<int, int> position) -> App&;
    [[nodiscard]] auto AppRenderer(std::shared_ptr<RenderingDevice>&& renderPtr) -> App&;
    [[nodiscard]] auto SubSystems(SystemInit sysFlags) -> App&;
    [[nodiscard]] auto AppIcon(const std::string &iconPath) -> App&;
    [[nodiscard]] auto AppOpacity(float opacity) -> App&;
    [[nodiscard]] auto Fullscreen() -> App&;

    [[nodiscard]] auto Build() -> bool;

    /**
     * @brief Starts up the application's framework. There is no need to call this function yourself
     * unless you require an advanced setup.
     *
     * @return true on successful initialization
     * @return false on failure
     */
    [[nodiscard]] auto InitializeGame() const -> bool;

    /**
     * @brief After confirming your app is initialized, call this function to start the main loop.
     *
     * @tparam T Your application's startup Scene::Node 
     * @param args Arguments your application needs
     */
    template<std::derived_from<Scene::Node> T>
    constexpr void RunGame(auto&&... args) noexcept {
      if (MakeScene<T>(std::forward<decltype(args)>(args)...)) {
        Run();
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
    std::derived_from<Scene> Manager,
    std::derived_from<Scene::Node> Root,
    typename...ArgsMan,
    typename... ArgsRoot>
    constexpr void RunGameExt(std::tuple<ArgsMan&&...> argsMan, std::tuple<ArgsRoot&&...> argsRoot) noexcept {
      if (MakeSceneExt<Manager, Root>(std::forward_as_tuple(argsMan), std::forward_as_tuple(argsRoot))) {
        Run();
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
    std::derived_from<Scene> Manager,
    std::derived_from<Scene::Node> Root,
    typename...ArgsMan,
    typename... ArgsRoot>
    constexpr auto MakeSceneExt(std::tuple<ArgsMan&&...> argsMan, std::tuple<ArgsRoot&&...> argsRoot) -> bool {
      _currentScene = std::make_unique<Manager>(ObjectRef<App>{this}, std::forward<ArgsMan>(argsMan)...);
      _currentScene->AddRootNode<Root>(std::forward<ArgsRoot>(argsRoot)...);
      return _currentScene->Create();
    }

    /**
     * @brief Create the game scene along with the root Scene::Node You defined in the template arguments.
     */
    template<std::derived_from<Scene::Node> T>
    constexpr auto MakeScene(auto&&... args) -> bool {
      _currentScene = std::make_unique<Scene>(ObjectRef<App>{this});
      _currentScene->AddRootNode<T>(std::forward<decltype(args)>(args)...);
      return _currentScene->Create();
    }

    void EventsAndTimeStep();

#ifdef __EMSCRIPTEN__
    void GameTick() const {
#else
    [[nodiscard]] auto GameTick() const -> bool {
#endif

      [[maybe_unused]] const bool result = _currentScene->Update(_gameTimer.GetSeconds());

      _renderer->BufferPresent();
#ifndef __EMSCRIPTEN__
      return result;
#endif
    }

    void CloseApp();
    [[nodiscard]] auto IsRunning() const -> bool { return _running; }

    void SetWindowSize(const int w, const int h) const { SDL_SetWindowSize(_window, w, h); }
    void SetTitle(const std::string& value) const { SDL_SetWindowTitle(_window, value.c_str()); }
    void SetFullscreen(const bool value) const { SDL_SetWindowFullscreen(_window, value); }
    void ShowWindow() const { SDL_ShowWindow(_window); }
    void HideWindow() const { SDL_HideWindow(_window); }
    void RaiseWindow() const { SDL_RaiseWindow(_window); }
    void RestoreWindow() const { SDL_RestoreWindow(_window); }
    void MaximizeWindow() const { SDL_MaximizeWindow(_window); }
    void MinimizeWindow() const { SDL_MinimizeWindow(_window); }

    [[nodiscard]] auto GetInternalClock() -> Timer* { return &_gameTimer; }

    [[nodiscard]] auto Renderer(this auto&& self) -> std::weak_ptr<RenderingDevice> { return self._renderer; }
    [[nodiscard]] constexpr auto Window(this auto&& self) -> SDL_Window* { return self._window; }

    [[nodiscard]] auto GetWindowSize() const -> std::pair<int, int> {
      int width{}, height{};
      SDL_GetWindowSize(_window, &width, &height);

      return std::make_pair(width, height);
    }

    [[nodiscard]] auto IsFullscreenBorderless() const -> bool { return SDL_GetWindowFullscreenMode(_window) == nullptr; }

    /*
    Input state and event polling for the client's logic.
    */

    [[nodiscard]] constexpr auto GetScrollX() const { return _input.scroll.x; }
    [[nodiscard]] constexpr auto GetScrollY() const { return _input.scroll.y; }
    [[nodiscard]] constexpr auto IsKeyPressed(const LayoutCode code) const -> bool { return (_input.keyEvent.first == code && _input.keyEvent.second); }
    [[nodiscard]] constexpr auto IsKeyReleased(const LayoutCode code) const -> bool { return (_input.keyEvent.first == code && !_input.keyEvent.second); }
    [[nodiscard]] constexpr auto IsKeyHeld(LayoutCode code) const -> bool { return _input.keyboardState[static_cast<size_t>(code)]; }
    [[nodiscard]] constexpr auto GetCurrentKeyEvent() const -> std::pair<LayoutCode, bool> { return _input.keyEvent; }
    [[nodiscard]] constexpr auto GetKeyMods() const -> KeyMod { return _input.modifiers; }
    [[nodiscard]] constexpr auto IsButtonPressed(MButton button) const -> bool { return _input.mouseEvents.at(static_cast<uint32_t>(button)).state == MButtonState::Pressed; }
    [[nodiscard]] constexpr auto IsButtonReleased(MButton button) const -> bool { return _input.mouseEvents.at(static_cast<uint32_t>(button)).state == MButtonState::Released; }
    [[nodiscard]] constexpr auto IsButtonHeld(MButton button) const -> bool { return static_cast<bool>(static_cast<uint32_t>(_input.mouseState.buttons) & static_cast<uint32_t>(button)); }
    [[nodiscard]] constexpr auto GetMouseClicks(MButton button) const -> uint8_t { return _input.mouseEvents.at(static_cast<uint32_t>(button)).clicks; }
    [[nodiscard]] constexpr auto GetMouseX() const { return _input.mouseState.x; }
    [[nodiscard]] constexpr auto GetMouseY() const { return _input.mouseState.y; }
    [[nodiscard]] constexpr auto GetMousePos() const { return SDL_FPoint{.x=_input.mouseState.x, .y=_input.mouseState.y}; }

    /*
      Input state and event management. For internal use only.
    */
  private:
    void UpdateMouseState() { _input.mouseState.buttons = MButton{SDL_GetMouseState(&_input.mouseState.x, &_input.mouseState.y)}; }
    constexpr void SetModState(const SDL_Keymod& state) { _input.modifiers = static_cast<KeyMod>(state); }
    constexpr void ResetScroll() { _input.scroll = {.x = 0.f, .y = 0.f}; }
    constexpr void AddScroll(const float amountX, const float amountY) { _input.scroll = {.x = amountX, .y = amountY}; }
    constexpr void SetMouseEvent(MButton button, const MButtonData data) { _input.mouseEvents.at(static_cast<size_t>(button)) = data; }

    constexpr void ResetKeyEvent() {
      _input.keyEvent.first = LayoutCode::Unknown;
      _input.keyEvent.second = false;
    }

    constexpr void SetKeyEvent(const LayoutCode code, const bool pressed) {
      _input.keyEvent.first = code;
      _input.keyEvent.second = pressed;
    }

    void SetKeyboardState() {
      int numKeys{};
      const bool* state = SDL_GetKeyboardState(&numKeys);
      _input.keyboardState = std::span<const bool>{state, static_cast<size_t>(numKeys)};
    }

    constexpr void ResetMouseEvents() {
      for (auto& [state, clicks]: _input.mouseEvents) {
        state = MButtonState::None;
        clicks = 0u;
      }
    }


#ifdef __EMSCRIPTEN__

    static void EmscriptenUpdate(void* ptr);

#endif // __EMSCRIPTEN__

    void Run();

    WindowConfig _winConfig{};
    std::string appName;
    SystemInit _sysFlags = SystemInit::Video;
    SDL_Window* _window = nullptr;
    std::shared_ptr<RenderingDevice> _renderer;
    std::unique_ptr<Scene> _currentScene;

    InputSystem _input;
    Timer _gameTimer;

    bool _running = true;
  };
} // namespace swgtk
#endif // SWGTK_ENGINE_INCLUDE_SWGTK_APP_HPP_
