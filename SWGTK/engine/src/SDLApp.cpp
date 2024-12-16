#include "SDLApp.hpp"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <chrono>
#include <memory>
#include <span>
#include <string>

#include "gsl/gsl-lite.hpp"
#include "SDL3/SDL.h"
#include "Input.hpp"
#include "SDL3_mixer/SDL_mixer.h"
#include "Scene.hpp"


namespace swgtk
{

#ifdef __EMSCRIPTEN__
	SDLApp::SDLApp()
		: _lastFrameTime(std::chrono::high_resolution_clock::now()),
		_currentFrameTime()
	{
		InitGraphical();
	}
#else
	SDLApp::SDLApp(int argc, const char** argv) {
		const std::span<const char*> args{argv, static_cast<size_t>(argc)};		

		// Because I plan on eventually having a headless version, I'm guarding
		// window creation with the --headless cmd flag.
		if (args.size() > 1u && strcmp(args[1], "--headless") == 0)	{
			InitHeadless();
		} else {
			InitGraphical();
		}

	}
#endif // __EMSCRIPTEN__

	SDLApp::~SDLApp() {
		if (!_headless)	{
			_fonts.ClearTTFFonts();
			
			
			SDL_DestroyRenderer(_renderer);
			SDL_DestroyWindow(_window);

			// Mix_Quit();
			TTF_Quit();
			SDL_Quit();
		}
	}

	void SDLApp::InitHeadless() {
		_headless = true;
	}

	void SDLApp::InitGraphical() {
		constexpr int InitFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
		constexpr int MixFlags = 0;

		if (SDL_Init(InitFlags) && TTF_Init() && Mix_Init(MixFlags) == MixFlags) {
			_window = SDL_CreateWindow("Game", 800, 600, SDL_WINDOW_HIDDEN); // NOLINT

			if (_window != nullptr)	{
				SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
				_renderer = SDL_CreateRenderer(_window, nullptr);
			} else {
				DEBUG_PRINT(std::format("Failed to create window. - {}\n", SDL_GetError()).c_str())
				return;
			}
			
			if (_renderer == nullptr) {
				DEBUG_PRINT(std::format("Failed to initialize renderer. - {}\n", SDL_GetError()).c_str())
				return;
			}

		} else {
			DEBUG_PRINT(std::format("SDL failed to initialize. - {}\n", SDL_GetError()).c_str())
			return;
		}

	}

	void SDLApp::EventsAndTimeStep() {
		// If a scene change occurs, that is all we want to do for this frame.
		if(_currentSSC == SSC::ChangeScene)
		{
			_currentSSC = _currentScene->Create();
			return;
		}
		
		SDL_Event e;

		_currentScene->ResetScroll();
		_currentScene->ResetMouseEvents();
		_currentScene->ResetKeyEvent();

		while (SDL_PollEvent(&e)) {
			switch (e.type)	{
			case SDL_EVENT_MOUSE_BUTTON_UP:
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
				{
					_currentScene->SetMouseEvent(MButton{ e.button.button }, (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) ? MButtonState::Pressed : MButtonState::Released);
					break;
				}

			case SDL_EVENT_KEY_DOWN:
			case SDL_EVENT_KEY_UP:
				{
					_currentScene->SetKeyEvent(LayoutCode(e.key.scancode), (e.type == SDL_EVENT_KEY_DOWN));
					break;
				}

			case SDL_EVENT_MOUSE_WHEEL:
				{
					_currentScene->AddScroll(e.wheel.x, e.wheel.y);
					break;
				}

			case SDL_EVENT_QUIT:
				{
					CloseApp();
					break;
				}
			}
		}


		_currentScene->SetKeyboardState();
		_currentScene->SetModState(SDL_GetModState());

		{
			MouseState mouse{};
			mouse.buttons = MButton{ SDL_GetMouseState(&mouse.x, &mouse.y) };

			_currentScene->SetMouseState(mouse);
		}

		_currentFrameTime = std::chrono::high_resolution_clock::now();

		const auto timeDiff = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(_currentFrameTime - _lastFrameTime).count());
		_lastFrameTime = _currentFrameTime;

		SDL_RenderClear(_renderer);

		_currentSSC = _currentScene->Update(static_cast<float>(timeDiff * 0.000001)); // NOLINT


		SDL_RenderPresent(_renderer);

	}

	void SDLApp::Run(gsl::owner<GameScene::Node*> logicNode) {
		if (!_headless)	{
			SDL_ShowWindow(_window);
			SDL_SetRenderDrawColor(_renderer, 64u, 64u, 64u, 255u); // NOLINT

			_currentScene = std::make_unique<GameScene>(gsl::make_not_null(this), logicNode);
			
			if (_currentScene->Create() != SSC::Ok)	{
				return;
			}

#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop_arg(SDLApp::EmscriptenUpdate, this, -1, true);

#else

			while (_running) {
				if (_currentSSC == SSC::Fail) {
					break;
				}

				if(_nextSceneNode != nullptr) {
					_currentScene->SetNewScene(_nextSceneNode);
					_nextSceneNode = nullptr;
					_currentSSC = SSC::ChangeScene;
				}
					
				EventsAndTimeStep();
			}

#endif // __EMSCRIPTEN__
		}
	}

	void SDLApp::CloseApp() {
#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop();
#else
		_running = false;

#endif
	}

#ifdef __EMSCRIPTEN__

	const SDLApp::timePoint& SDLApp::GetLastFrame() { return _lastFrameTime; }

	void SDLApp::EmscriptenUpdate(void* ptr) {
		static_cast<SDLApp*>(ptr)->EventsAndTimeStep();
	}
#endif


}
