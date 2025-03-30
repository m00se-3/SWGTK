#ifndef SWGTK_APP_HPP
#define SWGTK_APP_HPP

#include "swgtk/Input.hpp"
#include "swgtk/RendererBase.hpp"
#include <SDL3/SDL_video.h>
#include <chrono>
#include <memory>

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

#include "swgtk/TTFFont.hpp"
#include "swgtk/Scene.hpp"

extern "C" {
	struct SDL_Window;
	struct SDL_Renderer;
}

namespace swgtk {

	/*
		This class is designed to handle SDL windows and events.
	*/
	class App {
	public:
		App() = default;
		App(const App&) = delete;
		App(App&&) = delete;
		App& operator=(const App&) = delete;
		App& operator=(App&&) = delete;
		~App();

		template<std::derived_from<Scene::Node> T, typename... Args>
		constexpr void RunGame(Args&&... args) {
			_currentScene = std::make_unique<Scene>(gsl::make_not_null<App*>(this), std::make_shared<T>(std::forward<Args>(args)...));
			Run();
		}

		void RunLuaGame(const std::filesystem::path& path);

		bool EventsAndTimeStep();
		void CloseApp();
		[[nodiscard]] bool InitGraphics(const char* appName, int width, int height, const std::shared_ptr<RendererBase>& renderPtr);

		static void InitLua(sol::state& lua);

		[[nodiscard]] TTF_Font* GetDefaultFont(this auto&& self) { return self._fonts.GetDefaultFont(); }
		void AddFont(this auto&& self, const std::filesystem::path& path, FontStyle style) { self._fonts.AddFont(path, style); }
		[[nodiscard]] constexpr TTF_Font* GetFont(this auto&& self, FontStyle style) { return self._fonts.GetFont(style); }
		static void SetFontStyle(TTF_Font* font, const FontStyle style) { FontGroup::SetFontStyle(font, style); }
		[[nodiscard]] static FontStyle GetFontStyle(TTF_Font* font) { return FontGroup::GetFontStyle(font); }

		[[nodiscard]] std::shared_ptr<RendererBase> Renderer(this auto&& self) { return self._renderer; }
		[[nodiscard]] constexpr SDL_Window* Window(this auto&& self) { return self._window; }

		[[nodiscard]] std::pair<int, int> GetWindowSize(this auto&& self)	{
			int width{}, height{};
			SDL_GetWindowSize(self._window, &width, &height);

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
		[[nodiscard]] constexpr uint8_t GetButtonClicks(MButton button) const { return _input.mouseEvents.at(static_cast<uint32_t>(button)).clicks; }
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

		using timePoint = std::chrono::time_point<std::chrono::steady_clock>;

		SDL_Window* _window = nullptr;
		std::shared_ptr<RendererBase> _renderer;
		InputSystem _input;

		std::unique_ptr<Scene> _currentScene;
		FontGroup _fonts;

		bool _running = true;

		timePoint _lastFrameTime = std::chrono::steady_clock::now(), _currentFrameTime;
	};
}
#endif // SWGTK_APP_HPP
