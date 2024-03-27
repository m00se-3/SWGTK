#include "SDLApp.hpp"

#include <format>
#include <print>

#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

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
	SDLApp::SDLApp(std::span<const char*> argv)
		: _assetsDir(SWGTK_ASSETS), _configDir(SWGTK_CONFIG),
		_lastFrameTime(std::chrono::high_resolution_clock::now()),
		_currentFrameTime()
	{
		// Because I plan on eventually having a headless version, I'm guarding
		// window creation with the --headless cmd flag.
		if (argv.size() > 1u && strcmp(argv[1], "--headless") == 0)
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
			
			nk_free(&_ctx);
			
			SDL_DestroyRenderer(_renderer);
			SDL_DestroyWindow(_window);

			Mix_Quit();
			TTF_Quit();
			IMG_Quit();
			SDL_Quit();
		}
	}

	void SDLApp::Run(std::unique_ptr<Scene> opener)
	{
		if (!_headless)
		{
			SDL_ShowWindow(_window);
			SDL_SetRenderDrawColor(_renderer, 64u, 64u, 64u, 255u); // NOLINT

			_currentScene.reset(opener.release());
			
			if (_currentScene->Create() != SSC::ok)
			{
				return;
			}

#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop_arg(SDLApp::EmscriptenUpdate, this, -1, true);

	#else

			while (_running)
			{
				if (_currentSSC == SSC::fail)
				{
					break;
				}

				if (_currentSSC == SSC::change_scene)
				{
					auto factory = _currentScene->GetNextScene();

					_currentScene = factory();
					_currentSSC = _currentScene->Create();

					continue;
				}
				
				EventsAndTimeStep();
			}

#endif // __EMSCRIPTEN__
		}
		else
		{
			
		}

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
			_window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1200, 900, SDL_WINDOW_HIDDEN); // NOLINT

			if (_window)
			{
				_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
			}
			else
			{
				std::print("Failed to create window. - {}\n", SDL_GetError());
				return;
			}
			
			if (!_renderer)
			{
				std::print("Failed to initialize renderer. - {}\n", SDL_GetError());
				return;
			}

		}
		else
		{
			std::print("An SDL Library failed to initialize. - {}\n", SDL_GetError());
			return;
		}

		_ui = std::make_unique<UI>(this, _assetsDir + "/fonts");
		_ui->LoadScriptsFromDirectory(_configDir + "/ui");
	}

	void SDLApp::EventsAndTimeStep()
	{
		nk_input_begin(&_ctx);
		
		SDL_Event e;

		_currentScene->ResetScroll();
		_currentScene->ResetMouseEvents();
		_currentScene->ResetKeyEvent();

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{

			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			{
				_currentScene->SetMouseEvent(MButton{ e.button.button }, (e.type == SDL_MOUSEBUTTONDOWN) ? MButtonState::Pressed : MButtonState::Released);

				auto button = SDLButtontoNKButton(e.button.button);

				if (e.type == SDL_MOUSEMOTION)
				{
					nk_input_motion(&_ctx, e.motion.x, e.motion.y);
				}

				if (button > -1)
				{
					nk_input_button(&_ctx, static_cast<nk_buttons>(button), e.button.x, e.button.y, (e.type == SDL_MOUSEBUTTONDOWN));
				}
				break;
			}

			case SDL_KEYDOWN:
			case SDL_KEYUP:
			{
				_currentScene->SetKeyEvent(LayoutCode{ e.key.keysym.scancode }, (e.type == SDL_KEYDOWN));
				
				auto key = SDLKeytoNKKey(e.key.keysym.sym, e.key.keysym.mod);

				if (key != NK_KEY_NONE)
				{
					nk_input_key(&_ctx, key, (e.type == SDL_KEYDOWN));
				}
				break;
			}

			case SDL_MOUSEWHEEL:
			{
				_currentScene->AddScroll(e.wheel.preciseY);
				
				nk_input_scroll(&_ctx, nk_vec2(e.wheel.preciseX, e.wheel.preciseY));
				break;
			}

			case SDL_QUIT:
			{
				CloseApp();
				break;
			}
			}
		}

		nk_input_end(&_ctx);

		_currentScene->SetKeyboardState();
		_currentScene->SetModState(SDL_GetModState());

		{
			MouseState mouse;
			mouse.buttons = MButton{ SDL_GetMouseState(&mouse.x, &mouse.y) };

			_currentScene->SetMouseState(mouse);
		}

		_currentFrameTime = std::chrono::high_resolution_clock::now();

		double timeDiff = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(_currentFrameTime - _lastFrameTime).count());
		_lastFrameTime = _currentFrameTime;

		SDL_RenderClear(_renderer);

		_currentScene->Update(static_cast<float>(timeDiff * 0.000001)); //NOLINT

		_ui->Update();
		_ui->Draw();

		SDL_RenderPresent(_renderer);

	}

	void SDLApp::CloseApp()
	{
#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop();
#else
		_running = false;

#endif
	}

	void SDLApp::OpenMenu(const std::string& name)
	{
		_ui->Open(name);
	}

	void SDLApp::CloseMenu(const std::string& name)
	{
		_ui->Close(name);
	}

	const std::string& SDLApp::AssetsDir() const
	{
		return _assetsDir;
	}

	const std::string& SDLApp::ConfigDir() const
	{
		return _configDir;
	}

#ifdef __EMSCRIPTEN__

	const SDLApp::timePoint& SDLApp::GetLastFrame() { return _lastFrameTime; }

	void SDLApp::EmscriptenUpdate(void* ptr)
	{
		static_cast<SDLApp*>(ptr)->EventsAndTimeStep();
	}
#endif

	nk_keys SDLApp::SDLKeytoNKKey(int key, uint16_t mods)
	{
		switch (key)
		{
		case SDLK_LSHIFT:
		case SDLK_RSHIFT:
		{
			return NK_KEY_SHIFT;
		}

		case SDLK_LCTRL:
		case SDLK_RCTRL:
		{
			return NK_KEY_CTRL;
		}

		case SDLK_RETURN:
		{
			return NK_KEY_ENTER;
		}

		case SDLK_BACKSPACE:
		{
			return NK_KEY_BACKSPACE;
		}

		case SDLK_TAB:
		{
			return NK_KEY_TAB;
		}

		case SDLK_DELETE:
		{
			return NK_KEY_DEL;
		}

		case SDLK_RIGHT:
		{
			return NK_KEY_RIGHT;
		}

		case SDLK_LEFT:
		{
			return NK_KEY_LEFT;
		}

		case SDLK_DOWN:
		{
			return NK_KEY_DOWN;
		}

		case SDLK_UP:
		{
			return NK_KEY_UP;
		}
		
		}

		return NK_KEY_NONE;			// Not a nk_key.
	}

	int SDLApp::SDLButtontoNKButton(uint8_t button)
	{
		switch (button)
		{
		case SDL_BUTTON_LEFT:			return NK_BUTTON_LEFT;
		case SDL_BUTTON_RIGHT:			return NK_BUTTON_RIGHT;
		case SDL_BUTTON_MIDDLE:			return NK_BUTTON_MIDDLE;
		}
		return -1;
	}

	const SSC SDLApp::GetSceneStatus() const
	{
		return _currentSSC;
	}

	FontGroup& SDLApp::GetFontGroup()
	{
		return _fonts;
	}

	nk_font* SDLApp::GetNKFont(FontStyle style, int size)
	{
		return _fonts.GetNK(style, size);
	}

	TTF_Font* SDLApp::GetTTF(FontStyle style, int size)
	{
		return _fonts.GetTTF(style, size);
	}

	nk_context* SDLApp::GetNKContext()
	{
		return &_ctx;
	}

	SDL_Renderer* SDLApp::Renderer()
	{
		return _renderer;
	}

	SDL_Window* SDLApp::Window()
	{
		return _window;
	}

	std::pair<int, int> SDLApp::GetWindowSize()
	{
		int w = 0, h = 0;
		
		SDL_GetWindowSize(_window, &w, &h);

		return std::make_pair(w, h);
	}

}
