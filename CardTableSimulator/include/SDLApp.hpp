#ifndef SDLAPP_HPP
#define SDLAPP_HPP

#include <chrono>

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

extern "C" {
	struct SDL_Window;
}

/*
	This class is designed to handle SDL windows and events.
*/
class SDLApp {

	using timePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;

	SDL_Window* _window = nullptr;


	timePoint _lastFrameTime;
	bool _running = true;

	// A mostly unused flag, until we get 'headless server' mode.
	bool _headless = false;

public:
#ifdef __EMSCRIPTEN__
	SDLApp();
#else
	SDLApp(int argc, char** argv);
#endif
	~SDLApp();

	void Run();
	void InitHeadless();
	void InitGraphical();

	// The following functions are for emscripten.
#ifdef __EMSCRIPTEN__

	const timePoint& GetLastFrame();

	static void EmscriptenUpdate(void* ptr);

#endif // __EMSCRIPTEN__
};

#endif // !SDLAPP_HPP
