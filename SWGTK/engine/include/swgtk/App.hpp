#ifndef SWGTK_APP_HPP
#define SWGTK_APP_HPP

#include "swgtk/RendererBase.hpp"
#include <SDL3/SDL_video.h>
#include <chrono>
#include <memory>

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

#include "swgtk/TTFFont.hpp"
#include "swgtk/Scene.hpp"
#include <swgtk/Macros.hpp>

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
#ifdef __EMSCRIPTEN__
		App() = default;
		App(const App&) = delete;
		App(App&&) = delete;
		App& operator=(const App&) = delete;
		App& operator=(App&&) = delete;
#else
		App() = default;
		App(const App&) = delete;
		App(App&&) = delete;
		App& operator=(const App&) = delete;
		App& operator=(App&&) = delete;
#endif
		~App();
		
		void Run(Scene::NodeProxy logicNode);
		void EventsAndTimeStep();
		void CloseApp();
		[[nodiscard]] bool InitGraphics(std::shared_ptr<RendererBase> renderPtr);
		constexpr bool InitHeadless() {
			_headless = true;
			return true;
		}

		[[nodiscard]] constexpr SSC GetSceneStatus(this auto&& self) { return self._currentSSC; }

		[[nodiscard]] constexpr TTF_Font* GetTTF(this auto&& self, sdl::FontStyle style) { return self._fonts.GetTTF(style); }
		[[nodiscard]] constexpr std::shared_ptr<RendererBase> Renderer(this auto&& self) { return self._renderer; }
		[[nodiscard]] constexpr SDL_Window* Window(this auto&& self) { return self._window; }

		[[nodiscard]] constexpr std::pair<int, int> GetWindowSize(this auto&& self)	{
			int width{}, height{};
			SDL_GetWindowSize(self._window, &width, &height);

			return std::make_pair(width, height);
		}

		constexpr void SetNewSceneNode(Scene::NodeProxy ptr) { _nextSceneNode = ptr.ptr; }
		constexpr void SetBackgroundColor(const SDL_FColor& color) { 
			_bgColor = color; 
			if(_renderer && _renderer->IsDeviceInitialized()) { _renderer->SetBackgroundColor(color); }
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
		sdl::FontGroup _fonts;

		SSC _currentSSC = SSC::Ok;
		bool _running = true;

		// A mostly unused flag, until we get 'headless server' mode.
		bool _headless = false;
		SDL_FColor _bgColor{ .r=0.0f, .g=0.0f, .b=0.0f, .a=1.0f };

		timePoint _lastFrameTime = std::chrono::steady_clock::now(), _currentFrameTime;
	};
}
#endif // !APP_HPP
