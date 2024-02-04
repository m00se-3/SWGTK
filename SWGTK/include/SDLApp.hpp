#ifndef SDLAPP_HPP
#define SDLAPP_HPP

#include <chrono>
#include <memory>
#include <string>

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO

#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_UINT_DRAW_INDEX
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_FIXED_TYPES

#undef NK_IMPLEMENTATION
#include "nuklear/nuklear.h"

#include "scenes/Scene.hpp"
#include "systems/UI.hpp"
#include "Font.hpp"

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
#else
		SDLApp(int argc, char** argv);
#endif
		~SDLApp();

		void Run(std::unique_ptr<Scene> opener);
		void InitHeadless();
		void InitGraphical();
		void EventsAndTimeStep();
		void CloseApp();

		void OpenMenu(const std::string& name);
		void CloseMenu(const std::string& name);

		nk_keys SDLKeytoNKKey(int key, uint16_t mods);
		int SDLButtontoNKButton(uint8_t button);
		const std::string& AssetsDir() const;
		const std::string& ConfigDir() const;
		const SSC GetSceneStatus() const;

		[[nodiscard]] FontGroup& GetFontGroup();
		[[nodiscard]]nk_font* GetNKFont(FontStyle style, int size);
		[[nodiscard]]TTF_Font* GetTTF(FontStyle style, int size);

		[[nodiscard]] nk_context* GetNKContext();
		[[nodiscard]] SDL_Renderer* Renderer();
		[[nodiscard]] SDL_Window* Window();

		std::pair<int, int> GetWindowSize();

		// The following functions are for emscripten.
#ifdef __EMSCRIPTEN__

		const timePoint& GetLastFrame();

		static void EmscriptenUpdate(void* ptr);

#endif // __EMSCRIPTEN__

	private:
		using timePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

		SDL_Window* _window = nullptr;
		SDL_Renderer* _renderer = nullptr;

		nk_context _ctx;
		FontGroup _fonts;

		std::unique_ptr<Scene> _currentScene;
		std::unique_ptr<UI> _ui;

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
