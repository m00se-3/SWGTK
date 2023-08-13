#include "SDLApp.hpp"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

#include <cstring>


SDLApp::SDLApp(int argc, char** argv)
{
	// Because I plan on eventually having a headless version, I'm guarding
	// window creation with the --headless cmd flag.
	if (argc > 1 && strcmp(argv[1], "--headless") != 0)
	{
		const int InitFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
		const int ImageFlags = IMG_INIT_PNG;
		const int MixFlags = 0;

		if (SDL_Init(InitFlags) == 0 && IMG_Init(ImageFlags) == ImageFlags && TTF_Init() == 0 && Mix_Init(MixFlags) == MixFlags)
		{

		}
		else
		{
			SDL_Log("An SDL Library failed to initialize. - %s\n", SDL_GetError());
			return;
		}
	}
	else
	{
		_headless = true;
	}

}

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
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(SDLApp::EmscriptenUpdate, this, -1, true);

#else

	while (_running)
	{

	}

#endif // __EMSCRIPTEN__

}

#ifdef __EMSCRIPTEN__

const SDLApp::timePoint& SDLApp::GetLastFrame() { return _lastFrameTime; }

void SDLApp::EmscriptenUpdate(void* ptr)
{

}

#endif
