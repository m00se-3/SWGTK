#include "SDLApp.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

#include <cstring>

#ifdef __EMSCRIPTEN__
SDLApp::SDLApp() { InitGraphical(); }
#else
SDLApp::SDLApp(int argc, char** argv)
	: _lastFrameTime(std::chrono::high_resolution_clock::now()),
	  _currentFrameTime()
{
	// Because I plan on eventually having a headless version, I'm guarding
	// window creation with the --headless cmd flag.
	if (argc > 1 && strcmp(argv[1], "--headless") != 0)
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
		SDL_DestroyWindow(_window);
		
		Mix_Quit();
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}
}

void SDLApp::Run()
{
	SDL_ShowWindow(_window);

	_currentFrameTime = std::chrono::high_resolution_clock::now();

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
		_window = SDL_CreateWindow("Card Table Simulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_HIDDEN);

		if (!_window)
		{
			SDL_Log("Failed to create window. - %s\n", SDL_GetError());
			return;
		}


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

		}
	}

	auto timeDiff = std::chrono::duration_cast<std::chrono::microseconds>(_currentFrameTime - _lastFrameTime);
	_lastFrameTime = _currentFrameTime;

}

#ifdef __EMSCRIPTEN__

const SDLApp::timePoint& SDLApp::GetLastFrame() { return _lastFrameTime; }

void SDLApp::EmscriptenUpdate(void* ptr)
{

}

#endif
