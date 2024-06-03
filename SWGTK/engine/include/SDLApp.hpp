#ifndef SDLAPP_HPP
#define SDLAPP_HPP

#include <SDL_video.h>
#include <chrono>
#include <gsl/gsl-lite.hpp>
#include <memory>
#include <string>

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

#include "TTFFont.hpp"
#include "Scene.hpp"

#ifdef _DEBUG
#include "fmt/format.h"
#define DEBUG_PRINT(Debug_Format, Debug_Message) fmt::print(Debug_Format, Debug_Message);
#else
#define DEBUG_PRINT(Debug_Format, Debug_Message)
#endif

extern "C" {
	struct SDL_Window;
	struct SDL_Renderer;
}

namespace swgtk
{

	/*
		This class is designed to handle SDL windows and events.
	*/
	class SDLApp {
	public:
#ifdef __EMSCRIPTEN__
		SDLApp();
		SDLApp(const SDLApp&) = delete;
		SDLApp(SDLApp&&) = delete;
		SDLApp& operator=(const SDLApp&) = delete;
		SDLApp& operator=(SDLApp&&) = delete;
#else
		SDLApp(int argc, const char** argv);
		SDLApp(const SDLApp&) = delete;
		SDLApp(SDLApp&&) = delete;
		SDLApp& operator=(const SDLApp&) = delete;
		SDLApp& operator=(SDLApp&&) = delete;
#endif
		~SDLApp();
		
		void Run(gsl::owner<GameScene::Node*> logicNode);
		void InitHeadless();
		void InitGraphical();
		void EventsAndTimeStep();
		void CloseApp();

		void OpenMenu(const std::string& name);
		void CloseMenu(const std::string& name);

		// static nk_keys SDLKeytoNKKey(int key, uint16_t mods);
		//static int SDLButtontoNKButton(uint8_t button);

		[[nodiscard]] constexpr std::string AssetsDir(this SDLApp& self) { return self._assetsDir; }
		[[nodiscard]] constexpr std::string ConfigDir(this SDLApp& self) { return self._configDir; }
		[[nodiscard]] constexpr SSC GetSceneStatus(this SDLApp& self) { return self._currentSSC; }

		[[nodiscard]] constexpr TTF_Font* GetTTF(this SDLApp& self, sdl::FontStyle style, int size) { return self._fonts.GetTTF(style, size); }
		[[nodiscard]] constexpr SDL_Renderer* Renderer(this SDLApp& self) { return self._renderer; }
		[[nodiscard]] constexpr SDL_Window* Window(this SDLApp& self) { return self._window; }

		[[nodiscard]] constexpr std::pair<int, int> GetWindowSize(this SDLApp& self)
		{
			int width{}, height{};
			SDL_GetWindowSize(self._window, &width, &height);

			return std::make_pair(width, height);
		}

		void SetNewSceneNode(gsl::owner<GameScene::Node*> ptr);

		// The following functions are for emscripten.
#ifdef __EMSCRIPTEN__

		const timePoint& GetLastFrame();

		static void EmscriptenUpdate(void* ptr);

#endif // __EMSCRIPTEN__

	private:
		using timePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

		SDL_Window* _window = nullptr;
		SDL_Renderer* _renderer = nullptr;
		gsl::owner<GameScene::Node*> _nextSceneNode = nullptr;

		std::unique_ptr<GameScene> _currentScene;
		sdl::FontGroup _fonts;

		SSC _currentSSC = SSC::ok;

		std::string _assetsDir;
		std::string _configDir;

		timePoint _lastFrameTime, _currentFrameTime;
		bool _running = true;

		// A mostly unused flag, until we get 'headless server' mode.
		bool _headless = false;
	};
}
#endif // !SDLAPP_HPP
