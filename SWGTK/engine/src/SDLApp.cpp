#include "SDLApp.hpp"

#include <chrono>
#include <memory>
#include <span>
#include <string>

#include "gsl/gsl-lite.hpp"
#include "SDL.h"
#include "Input.hpp"
#include "SDL_image.h"
#include "SDL_mixer.h"
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
	SDLApp::SDLApp(int argc, const char** argv)
	{
		const std::span<const char*> args{argv, static_cast<size_t>(argc)};		

		// Because I plan on eventually having a headless version, I'm guarding
		// window creation with the --headless cmd flag.
		if (args.size() > 1u && strcmp(args[1], "--headless") == 0)
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
			_fonts.ClearTTFFonts();
			
			
			SDL_DestroyRenderer(_renderer);
			SDL_DestroyWindow(_window);

			Mix_Quit();
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
		}
	}

	void SDLApp::InitHeadless()
	{
		_headless = true;
	}

	void SDLApp::InitGraphical()
	{
		constexpr int InitFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
		constexpr int ImageFlags = IMG_INIT_PNG;
		constexpr int MixFlags = 0;

		if (SDL_Init(InitFlags) == 0 && IMG_Init(ImageFlags) == ImageFlags && TTF_Init() == 0 && Mix_Init(MixFlags) == MixFlags)
		{
			_window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_HIDDEN); // NOLINT

			if (_window != nullptr)
			{
				_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
			}
			else
			{
				DEBUG_PRINT("Failed to create window. - %s\n", SDL_GetError())
				return;
			}
			
			if (_renderer == nullptr)
			{
				DEBUG_PRINT("Failed to initialize renderer. - %s\n", SDL_GetError())
				return;
			}

		}
		else
		{
			DEBUG_PRINT("An SDL Library failed to initialize. - %s\n", SDL_GetError())
			return;
		}

	}

	void SDLApp::EventsAndTimeStep()
	{
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

		while (SDL_PollEvent(&e) == 1)
		{
			switch (e.type)
			{
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				{
					_currentScene->SetMouseEvent(MButton{ e.button.button }, (e.type == SDL_MOUSEBUTTONDOWN) ? MButtonState::Pressed : MButtonState::Released);
					break;
				}

			case SDL_KEYDOWN:
			case SDL_KEYUP:
				{
					_currentScene->SetKeyEvent(LayoutCode{ e.key.keysym.scancode }, (e.type == SDL_KEYDOWN));
					break;
				}

			case SDL_MOUSEWHEEL:
				{
					_currentScene->AddScroll(e.wheel.preciseX, e.wheel.preciseY);
					break;
				}

			case SDL_QUIT:
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

		const double timeDiff = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(_currentFrameTime - _lastFrameTime).count());
		_lastFrameTime = _currentFrameTime;

		SDL_RenderClear(_renderer);

		_currentSSC = _currentScene->Update(static_cast<float>(timeDiff * 0.000001)); // NOLINT


		SDL_RenderPresent(_renderer);

	}

	void SDLApp::Run(gsl::owner<GameScene::Node*> logicNode)
	{
		if (!_headless)
		{
			SDL_ShowWindow(_window);
			SDL_SetRenderDrawColor(_renderer, 64u, 64u, 64u, 255u); // NOLINT

			_currentScene = std::make_unique<GameScene>(gsl::make_not_null(this), logicNode);
			
			if (_currentScene->Create() != SSC::Ok)
			{
				return;
			}

#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop_arg(SDLApp::EmscriptenUpdate, this, -1, true);

#else

			while (_running)
			{
				if (_currentSSC == SSC::Fail)
				{
					break;
				}

				if(_nextSceneNode != nullptr)
				{
					_currentScene->SetNewScene(_nextSceneNode);
					_nextSceneNode = nullptr;
					_currentSSC = SSC::ChangeScene;
				}
					
				EventsAndTimeStep();
			}

#endif // __EMSCRIPTEN__
		}
	}

	void SDLApp::CloseApp()
	{
#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop();
#else
		_running = false;

#endif
	}

	void SDLApp::SetNewSceneNode(gsl::owner<GameScene::Node*> ptr)
	{
		_nextSceneNode = ptr;	
	}

#ifdef __EMSCRIPTEN__

	const SDLApp::timePoint& SDLApp::GetLastFrame() { return _lastFrameTime; }

	void SDLApp::EmscriptenUpdate(void* ptr)
	{
		static_cast<SDLApp*>(ptr)->EventsAndTimeStep();
	}
#endif


}
