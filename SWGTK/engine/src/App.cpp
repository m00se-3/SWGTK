#include <swgtk/App.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <SDL3_mixer/SDL_mixer.h>

#include <chrono>
#include <memory>


namespace swgtk
{
	App::~App() {
		if (!_headless)	{
			_fonts.ClearFonts();
			
			
			SDL_DestroyWindow(_window);

			Mix_Quit();
			TTF_Quit();
			SDL_Quit();
		}
	}

	bool App::InitGraphics(const char* appName, int width, int height, std::shared_ptr<RendererBase> renderPtr) {
		constexpr int InitFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
		constexpr int MixFlags = 0;

		if (SDL_Init(InitFlags) && TTF_Init() && Mix_Init(MixFlags) == MixFlags) {
			_window = SDL_CreateWindow(appName, width, height, SDL_WINDOW_HIDDEN);

			if (_window != nullptr)	{
				SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
				_renderer = renderPtr;
				return true;
			}
		} 

		DEBUG_PRINT("SDL failed to initialize. - {}\n", SDL_GetError())
		return false;
	}

	void App::EventsAndTimeStep() {
		// If a scene change occurs, that is all we want to do for this frame.
		if(_currentSSC == SSC::ChangeScene) {
			_currentSSC = _currentScene->Create();
			return;
		}
		
		SDL_Event e;

		ResetScroll();
		ResetMouseEvents();
		ResetKeyEvent();

		while (SDL_PollEvent(&e)) {
			switch (e.type)	{
			case SDL_EVENT_MOUSE_BUTTON_UP:
			case SDL_EVENT_MOUSE_BUTTON_DOWN: {
					SetMouseEvent(MButton{ e.button.button }, (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) ? MButtonState::Pressed : MButtonState::Released);
					break;
				}

			case SDL_EVENT_KEY_DOWN:
			case SDL_EVENT_KEY_UP: {
					SetKeyEvent(LayoutCode(e.key.scancode), (e.type == SDL_EVENT_KEY_DOWN));
					break;
				}

			case SDL_EVENT_MOUSE_WHEEL: {
					AddScroll(e.wheel.x, e.wheel.y);
					break;
				}

			case SDL_EVENT_QUIT: {
					CloseApp();
					break;
				}
			default: {
				// Unsupported event.
				}
			}
		}


		SetKeyboardState();
		SetModState(SDL_GetModState());

		{
			MouseState mouse{};
			mouse.buttons = MButton{ SDL_GetMouseState(&mouse.x, &mouse.y) };

			SetMouseState(mouse);
		}

		_currentFrameTime = std::chrono::steady_clock::now();

		const auto timeDiff = std::chrono::duration_cast<std::chrono::microseconds>(_currentFrameTime - _lastFrameTime);
		_lastFrameTime = _currentFrameTime;

		// std::ratio<1,1> converts the timeDiff duration into seconds.
		_currentSSC = _currentScene->Update(std::chrono::duration<float, std::ratio<1,1>>(timeDiff).count());

		_renderer->BufferPresent();

	}

	void App::Run(Scene::NodeProxy logicNode) {
		if (!_headless)	{
			if(!_renderer->PrepareDevice(_window)) {
				DEBUG_PRINT("Failed to initialize rendering context. - {}\n", SDL_GetError())
				return;
			}

			SDL_ShowWindow(_window);

			_currentScene = std::make_unique<Scene>(this, logicNode);
			
			if (_currentScene->Create() != SSC::Ok)	{
				return;
			}

#ifdef __EMSCRIPTEN__
			emscripten_set_main_loop_arg(App::EmscriptenUpdate, this, -1, true);

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

	void App::CloseApp() {
#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop();
#else
		_running = false;

#endif
	}

#ifdef __EMSCRIPTEN__

	const App::timePoint& App::GetLastFrame() { return _lastFrameTime; }

	void App::EmscriptenUpdate(void* ptr) {
		dynamic_cast<App*>(ptr)->EventsAndTimeStep();
	}
#endif


}
