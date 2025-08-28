/*
    MIT License
    Copyright (c) 2023 Samuel Bridgham

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#include "SDL3/SDL_init.h"
#include <swgtk/App.hpp>
#include <swgtk/Utility.hpp>
#include <swgtk/Math.hpp>
#include <swgtk/Surface.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>

#include <memory>
#include <utility>


namespace swgtk {
	App::~App() {
		_fonts.ClearFonts();
		if((SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO) == SDL_INIT_VIDEO) {
			_renderer.reset();
			SDL_DestroyWindow(_window);
		}

		TTF_Quit();
		SDL_Quit();
	}

	bool App::InitGraphics(const char* appName, const int width, const int height, std::shared_ptr<RenderingDevice>&& renderPtr, const SystemInit flags) {

		if (SDL_Init(std::to_underlying(flags)) && TTF_Init()) {

			// false positive
			// cppcheck-suppress syntaxError
			if(_window = SDL_CreateWindow(appName, width, height, SDL_WINDOW_HIDDEN); _window != nullptr) {
				SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
				_renderer = std::move(renderPtr);
				return InitializeGame();
			}
		} 

		DEBUG_PRINT("SDL failed to initialize. - {}\n", SDL_GetError())
		return false;
	}

	void App::EventsAndTimeStep() {
		SDL_Event e;

		ResetScroll();
		ResetMouseEvents();
		ResetKeyEvent();

		while (SDL_PollEvent(&e)) {
			switch (e.type)	{
			case SDL_EVENT_MOUSE_BUTTON_UP:
			case SDL_EVENT_MOUSE_BUTTON_DOWN: {
					SetMouseEvent(MButton{ e.button.button }, MButtonData {
						.state = (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) ? MButtonState::Pressed : MButtonState::Released,
						.clicks = e.button.clicks
					});
					break;
				}

			case SDL_EVENT_KEY_DOWN:
			case SDL_EVENT_KEY_UP: {
					SetKeyEvent(static_cast<LayoutCode>(e.key.scancode), (e.type == SDL_EVENT_KEY_DOWN));
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

		_gameTimer.UpdateTime();
	}

	bool App::InitializeGame() {
		if(_renderer->PrepareDevice(_window) && _fonts.LoadDefaultFont()) {
			_renderer->SetFont(_fonts.GetDefaultFont().ptr);
			return true;
		}

		DEBUG_PRINT("Failed to initialize rendering context. - {}\n", SDL_GetError())
		return false;
	}

	void App::Run() {
		if((SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO) == SDL_INIT_VIDEO) {
			ShowWindow();
		}
#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop_arg(App::EmscriptenUpdate, this, -1, true);

#else

		bool gameOk = true;

		while (_running && gameOk) {
			EventsAndTimeStep();
			gameOk = GameTick();
		}

#endif // __EMSCRIPTEN__
	}

	void App::CloseApp() {
#ifdef __EMSCRIPTEN__
		emscripten_cancel_main_loop();
#else
		_running = false;

#endif
	}

#ifdef __EMSCRIPTEN__
	void App::EmscriptenUpdate(void* ptr) {
		auto* app = static_cast<App*>(ptr);
		app->EventsAndTimeStep();
		app->GameTick();
	}
#endif



} // namespace swgtk
