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
#include <swgtk/Utility.hpp>

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
		
		void Run(Scene::NodeProxy logicNode);
		void EventsAndTimeStep();
		void CloseApp();
		[[nodiscard]] bool InitGraphics(const char* appName, int width, int height, std::shared_ptr<RendererBase> renderPtr);

		constexpr bool InitHeadless() {
			_headless = true;
			return true;
		}

		[[nodiscard]] constexpr SSC GetSceneStatus(this auto&& self) { return self._currentSSC; }

		constexpr void AddFont(this auto&& self, const std::filesystem::path& path, FontStyle style) { self._fonts.AddFont(path, style); }
		[[nodiscard]] constexpr TTF_Font* GetFont(this auto&& self, FontStyle style) { return self._fonts.GetFont(style); }
		[[nodiscard]] constexpr std::shared_ptr<RendererBase> Renderer(this auto&& self) { return self._renderer; }
		[[nodiscard]] constexpr SDL_Window* Window(this auto&& self) { return self._window; }

		[[nodiscard]] constexpr std::pair<int, int> GetWindowSize(this auto&& self)	{
			int width{}, height{};
			SDL_GetWindowSize(self._window, &width, &height);

			return std::make_pair(width, height);
		}

		constexpr void SetNewSceneNode(Scene::NodeProxy ptr) { _nextSceneNode = ptr.ptr; }

		/*
			Input state and event polling for the client's logic.
		*/

		[[nodiscard]] constexpr auto GetScrollX() const { return _input.scroll.x; }
		[[nodiscard]] constexpr auto GetScrollY() const { return _input.scroll.y; }
		[[nodiscard]] constexpr bool IsKeyPressed(LayoutCode code) const { return (_input.keyEvent.first == code && _input.keyEvent.second); }
		[[nodiscard]] constexpr bool IsKeyReleased(LayoutCode code) const { return (_input.keyEvent.first == code && !_input.keyEvent.second); }
		[[nodiscard]] constexpr bool IsKeyHeld(LayoutCode code) const{ return _input.keyboardState[static_cast<size_t>(code)]; }
		[[nodiscard]] constexpr std::pair<LayoutCode, bool> GetCurrentKeyEvent() const { return _input.keyEvent; }
		[[nodiscard]] constexpr KeyMod GetKeyMods() const { return _input.modifiers; }
		[[nodiscard]] constexpr bool IsButtonPressed(MButton button) const { return _input.mouseEvents.at(static_cast<uint32_t>(button)) == MButtonState::Pressed; }
		[[nodiscard]] constexpr bool IsButtonReleased(MButton button) const { return _input.mouseEvents.at(static_cast<uint32_t>(button)) == MButtonState::Released; }
		[[nodiscard]] constexpr bool IsButtonHeld(MButton button) const { return static_cast<bool>(static_cast<uint32_t>(_input.mouseState.buttons) & static_cast<uint32_t>(button)); }
		[[nodiscard]] constexpr auto GetMouseX() const { return _input.mouseState.x; }
		[[nodiscard]] constexpr auto GetMouseY() const { return _input.mouseState.y; }
		[[nodiscard]] constexpr auto GetMousePos() const { return SDL_FPoint{ _input.mouseState.x, _input.mouseState.y }; }

		/*
			Input state and event management.
		*/

		constexpr void SetMouseState(const MouseState& event) { _input.mouseState = event; }
		constexpr void SetModState(const SDL_Keymod& state) { _input.modifiers = static_cast<KeyMod>(state); }
		constexpr void ResetScroll() { _input.scroll = { .x=0.f, .y=0.f }; }
		constexpr void AddScroll(float amountX, float amountY) { _input.scroll = { .x=amountX, .y=amountY }; }
		constexpr void SetMouseEvent(MButton button, MButtonState state) { _input.mouseEvents.at(static_cast<size_t>(button)) = state; }

		constexpr void ResetKeyEvent()
		{
			_input.keyEvent.first = LayoutCode::Unknown;
			_input.keyEvent.second = false;	
		}

		constexpr void SetKeyEvent(LayoutCode code, bool pressed)
		{	
			_input.keyEvent.first = code;
			_input.keyEvent.second = pressed;
		}

		constexpr void SetKeyboardState() 
		{
			int numKeys{};
			const bool* state = SDL_GetKeyboardState(&numKeys);
			_input.keyboardState = std::span<const bool>{ state, static_cast<size_t>(numKeys) };
		}
		
		constexpr void ResetMouseEvents() {
			for (auto& s : _input.mouseEvents)
			{
				s = MButtonState::None;
			}
		}

#ifdef __EMSCRIPTEN__

		const timePoint& GetLastFrame();

		static void EmscriptenUpdate(void* ptr);

#endif // __EMSCRIPTEN__

	private:
		using timePoint = std::chrono::time_point<std::chrono::steady_clock>;

		SDL_Window* _window = nullptr;
		std::shared_ptr<RendererBase> _renderer;
		InputSystem _input;

		Scene::Node* _nextSceneNode = nullptr;
		std::unique_ptr<Scene> _currentScene;
		FontGroup _fonts;

		SSC _currentSSC = SSC::Ok;
		bool _running = true;

		// A mostly unused flag, until we get 'headless server' mode.
		bool _headless = false;

		timePoint _lastFrameTime = std::chrono::steady_clock::now(), _currentFrameTime;
	};
}
#endif // !APP_HPP
