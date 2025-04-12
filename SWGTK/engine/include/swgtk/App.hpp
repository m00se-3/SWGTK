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

#include "swgtk/Input.hpp"
#include "swgtk/RendererBase.hpp"
#include <swgtk/Timer.hpp>
#include <SDL3/SDL_video.h>
#include <memory>
#include <string>
#include <utility>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "swgtk/TTFFont.hpp"
#include "swgtk/Scene.hpp"

extern "C" {
	struct SDL_Window;
	struct SDL_Renderer;
}

namespace swgtk {
	/**
	  @brief This class is the root manager of the SWGTK framework.

	  App is where the engine code lives. Your code is injected during initialization, and
	  App manages the lifetime of your game classes. (See swgtk::Scene::Node for more details)
	 */
	class App {
	public:
		App() = default;
		App(const App&) = delete;
		App(App&&) noexcept = delete;
		App& operator=(const App&) = delete;
		App& operator=(App&&) noexcept = delete;
		~App();

		bool InitializeGame();

		template<std::derived_from<Scene::Node> T, typename... Args>
		constexpr void RunGame(Args&&... args) noexcept {
			MakeScene<T>(std::forward<Args>(args)...);

			if(InitializeGame()) {
				Run();
			}
		}

		template<std::derived_from<Scene::Node> T, typename... Args>
		constexpr void MakeScene(Args&&... args) {
			_currentScene = std::make_unique<Scene>(gsl::make_not_null<App*>(this), std::make_shared<T>(std::forward<Args>(args)...));
		}

		void EventsAndTimeStep();
		[[nodiscard]] bool GameTick() const {

			const bool result = _currentScene->Update(_gameTimer.GetSeconds());

			_renderer->BufferPresent();

			return result;
		}

		void CloseApp();
		[[nodiscard]] bool InitGraphics(const char* appName, int width, int height, const std::shared_ptr<RendererBase>& renderPtr);

		void InitLua(sol::state& lua, bool acceptUserInput = false);

		[[nodiscard]] Font GetDefaultFont() const { return _fonts.GetDefaultFont(); }
		void AddFont(const std::filesystem::path& path) { _fonts.AddFont(path); }
		[[nodiscard]] Font GetFont(const std::string& path) const { return _fonts.GetFont(path); }
		static void SetFontStyle(const Font font, const FontStyle style) { FontGroup::SetFontStyle(font, style); }
		[[nodiscard]] static FontStyle GetFontStyle(const Font font) { return FontGroup::GetFontStyle(font); }

		[[nodiscard]] FontGroup& GetFontHandle() { return _fonts; }

		[[nodiscard]] std::shared_ptr<RendererBase> Renderer(this auto&& self) { return self._renderer; }
		[[nodiscard]] constexpr SDL_Window* Window(this auto&& self) { return self._window; }

		[[nodiscard]] std::pair<int, int> GetWindowSize() const {
			int width{}, height{};
			SDL_GetWindowSize(_window, &width, &height);

			return std::make_pair(width, height);
		}

		/*
			Input state and event polling for the client's logic.
		*/

		[[nodiscard]] constexpr auto GetScrollX() const { return _input.scroll.x; }
		[[nodiscard]] constexpr auto GetScrollY() const { return _input.scroll.y; }
		[[nodiscard]] constexpr bool IsKeyPressed(const LayoutCode code) const { return (_input.keyEvent.first == code && _input.keyEvent.second); }
		[[nodiscard]] constexpr bool IsKeyReleased(const LayoutCode code) const { return (_input.keyEvent.first == code && !_input.keyEvent.second); }
		[[nodiscard]] constexpr bool IsKeyHeld(LayoutCode code) const{ return _input.keyboardState[static_cast<size_t>(code)]; }
		[[nodiscard]] constexpr std::pair<LayoutCode, bool> GetCurrentKeyEvent() const { return _input.keyEvent; }
		[[nodiscard]] constexpr KeyMod GetKeyMods() const { return _input.modifiers; }
		[[nodiscard]] constexpr bool IsButtonPressed(MButton button) const { return _input.mouseEvents.at(static_cast<uint32_t>(button)).state == MButtonState::Pressed; }
		[[nodiscard]] constexpr bool IsButtonReleased(MButton button) const { return _input.mouseEvents.at(static_cast<uint32_t>(button)).state == MButtonState::Released; }
		[[nodiscard]] constexpr bool IsButtonHeld(MButton button) const { return static_cast<bool>(static_cast<uint32_t>(_input.mouseState.buttons) & static_cast<uint32_t>(button)); }
		[[nodiscard]] constexpr uint8_t GetMouseClicks(MButton button) const { return _input.mouseEvents.at(static_cast<uint32_t>(button)).clicks; }
		[[nodiscard]] constexpr auto GetMouseX() const { return _input.mouseState.x; }
		[[nodiscard]] constexpr auto GetMouseY() const { return _input.mouseState.y; }
		[[nodiscard]] constexpr auto GetMousePos() const { return SDL_FPoint{ _input.mouseState.x, _input.mouseState.y }; }

		/*
			Input state and event management.
		*/

		constexpr void SetMouseState(const MouseState& state) { _input.mouseState = state; }
		constexpr void SetModState(const SDL_Keymod& state) { _input.modifiers = static_cast<KeyMod>(state); }
		constexpr void ResetScroll() { _input.scroll = { .x=0.f, .y=0.f }; }
		constexpr void AddScroll(const float amountX, const float amountY) { _input.scroll = { .x=amountX, .y=amountY }; }
		constexpr void SetMouseEvent(MButton button, const MButtonData data) { _input.mouseEvents.at(static_cast<size_t>(button)) = data; }

		constexpr void ResetKeyEvent()
		{
			_input.keyEvent.first = LayoutCode::Unknown;
			_input.keyEvent.second = false;	
		}

		constexpr void SetKeyEvent(const LayoutCode code, const bool pressed)
		{	
			_input.keyEvent.first = code;
			_input.keyEvent.second = pressed;
		}

		void SetKeyboardState() 
		{
			int numKeys{};
			const bool* state = SDL_GetKeyboardState(&numKeys);
			_input.keyboardState = std::span<const bool>{ state, static_cast<size_t>(numKeys) };
		}
		
		constexpr void ResetMouseEvents() {
			for (auto&[state, clicks] : _input.mouseEvents)
			{
				state = MButtonState::None;
				clicks = 0u;
			}
		}

#ifdef __EMSCRIPTEN__

		const timePoint& GetLastFrame();

		static void EmscriptenUpdate(void* ptr);

#endif // __EMSCRIPTEN__

	private:
		void Run();

		SDL_Window* _window = nullptr;
		std::shared_ptr<RendererBase> _renderer;
		InputSystem _input;

		std::unique_ptr<Scene> _currentScene;
		FontGroup _fonts;
		Timer _gameTimer;

		bool _running = true;
	};
} // namespace swgtk
#endif // SWGTK_ENGINE_INCLUDE_SWGTK_APP_HPP_
