#include "SDLApp.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

#include <cstring>

namespace cts
{

#ifdef __EMSCRIPTEN__
	SDLApp::SDLApp()
		: _lastFrameTime(std::chrono::high_resolution_clock::now()),
		_currentFrameTime()
	{
		InitGraphical();
	}
#else
	SDLApp::SDLApp(int argc, char** argv)
		: _lastFrameTime(std::chrono::high_resolution_clock::now()),
		_currentFrameTime()
	{
		// Because I plan on eventually having a headless version, I'm guarding
		// window creation with the --headless cmd flag.
		if (argc > 1 && strcmp(argv[1], "--headless") == 0)
		{
			InitHeadless();
		}
		else
		{
			InitGraphical();
		}

	}
#endif // __EMSCRIPTEN__

	SDLApp::~SDLApp()
	{
		if (!_headless)
		{
			SDL_DestroyRenderer(_renderer);
			SDL_DestroyWindow(_window);

			Mix_Quit();
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
		}
	}

	void SDLApp::Run()
	{
		if (!_headless)
		{
			SDL_ShowWindow(_window);
		}

#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop_arg(SDLApp::EmscriptenUpdate, this, -1, true);

#else

		while (_running)
		{
			EventsAndTimeStep();
		}

#endif // __EMSCRIPTEN__

	}

	void SDLApp::InitHeadless()
	{
		_headless = true;
	}

	void SDLApp::InitGraphical()
	{
		const int InitFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
		const int ImageFlags = IMG_INIT_PNG;
		const int MixFlags = 0;

		if (SDL_Init(InitFlags) == 0 && IMG_Init(ImageFlags) == ImageFlags && TTF_Init() == 0 && Mix_Init(MixFlags) == MixFlags)
		{
			if (SDL_CreateWindowAndRenderer(1200, 900, SDL_WINDOW_HIDDEN, &_window, &_renderer) != 0)
			{
				SDL_Log("Failed to create window. - %s\n", SDL_GetError());
				return;
			}

			SDL_SetWindowTitle(_window, "Card Table");
			SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		}
		else
		{
			SDL_Log("An SDL Library failed to initialize. - %s\n", SDL_GetError());
			return;
		}
	}

	void SDLApp::EventsAndTimeStep()
	{
		SDL_Event e;

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{

			case SDL_MOUSEMOTION:
			{
				// TODO: handle outside of nuklear


				nk_input_motion(&_ctx, e.motion.x, e.motion.y);
				break;
			}

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			{
				// TODO: handle outside of nuklear
				
				nk_input_button(&_ctx, SDLButtontoNKButton(e.button.button), e.button.x, e.button.y, (e.type == SDL_MOUSEBUTTONDOWN));
				break;
			}

			case SDL_KEYDOWN:
			case SDL_KEYUP:
			{
				// TODO: handle outside of nuklear
								
				nk_input_key(&_ctx, SDLKeytoNKKey(e.key.keysym.sym, e.key.keysym.mod), (e.type == SDL_KEYDOWN));
				break;
			}

			case SDL_MOUSEWHEEL:
			{
				nk_input_scroll(&_ctx, nk_vec2(static_cast<float>(e.wheel.x), static_cast<float>(e.wheel.y)));
				break;
			}

			case SDL_QUIT:
			{
				CloseApp();
				break;
			}
			}
		}

		_currentFrameTime = std::chrono::high_resolution_clock::now();

		auto timeDiff = std::chrono::duration_cast<std::chrono::microseconds>(_currentFrameTime - _lastFrameTime);
		_lastFrameTime = _currentFrameTime;

	}

	void SDLApp::CloseApp()
	{
#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop();
#else
		_running = false;

#endif
	}

#ifdef __EMSCRIPTEN__

	const SDLApp::timePoint& SDLApp::GetLastFrame() { return _lastFrameTime; }

	void SDLApp::EmscriptenUpdate(void* ptr)
	{
		static_cast<SDLApp*>(ptr)->EventsAndTimeStep();
	}
}
#endif

nk_keys SDLApp::SDLKeytoNKKey(int key, uint16_t mods)
{
	switch (key)
	{
	case SDLK_LSHIFT:			
	case SDLK_RSHIFT:					return NK_KEY_SHIFT;
	case SDLK_LCTRL:
	case SDLK_RCTRL:					return NK_KEY_CTRL;
	}

	return NK_KEY_NONE;			// Not a nk_key.
}

nk_buttons SDLApp::SDLButtontoNKButton(uint8_t button)
{
	switch (button)
	{
	case SDL_BUTTON_LEFT:			return NK_BUTTON_LEFT;
	case SDL_BUTTON_RIGHT:			return NK_BUTTON_RIGHT;
	case SDL_BUTTON_MIDDLE:			return NK_BUTTON_MIDDLE;
	}
}

