#ifndef SDLAPP_HPP
#define SDLAPP_HPP

#include <chrono>
#include <concepts>
#include <gsl/gsl-lite.hpp>
#include <memory>
#include <string>
#include <span>

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

#include "TTFFont.hpp"
#include "Scene.hpp"

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

		[[nodiscard]] std::string AssetsDir() const;
		[[nodiscard]] std::string ConfigDir() const;
		[[nodiscard]] SSC GetSceneStatus() const;

		[[nodiscard]] TTF_Font* GetTTF(sdl::FontStyle style, int size);
		[[nodiscard]] SDL_Renderer* Renderer();
		[[nodiscard]] SDL_Window* Window();

		std::pair<int, int> GetWindowSize();
		void GetNewSceneNode(gsl::owner<GameScene::Node*> ptr);

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
