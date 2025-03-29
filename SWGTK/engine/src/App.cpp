#include <swgtk/App.hpp>
#include <swgtk/Utility.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <gsl/gsl-lite.hpp>

#include <chrono>
#include <memory>


namespace swgtk
{
	App::~App() {
		_fonts.ClearFonts();

		SDL_DestroyWindow(_window);

		Mix_Quit();
		TTF_Quit();
		SDL_Quit();
	}

	bool App::InitGraphics(const char* appName, const int width, const int height, const std::shared_ptr<RendererBase> &renderPtr) {
		constexpr int InitFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
		constexpr int MixFlags = 0;

		if (SDL_Init(InitFlags) && TTF_Init() && Mix_Init(MixFlags) == MixFlags) {

			if (_window = SDL_CreateWindow(appName, width, height, SDL_WINDOW_HIDDEN); _window != nullptr)	{
				SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
				_renderer = renderPtr;
				return true;
			}
		} 

		DEBUG_PRINT("SDL failed to initialize. - {}\n", SDL_GetError())
		return false;
	}

	bool App::EventsAndTimeStep() {
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

		_currentFrameTime = std::chrono::steady_clock::now();

		const auto timeDiff = std::chrono::duration_cast<std::chrono::microseconds>(_currentFrameTime - _lastFrameTime);
		_lastFrameTime = _currentFrameTime;
		
		// std::ratio<1,1> converts the timeDiff duration into seconds.
		const auto deltaTime = std::chrono::duration<float, std::ratio<1,1>>(timeDiff).count();

		const bool result = _currentScene->Update(deltaTime);

		_renderer->BufferPresent();

		return result;
	}

	void App::RunLuaGame(const std::filesystem::path& path) {
		RunGame<LuaGame>(path);
	}

	void App::Run() {
		if(!_renderer->PrepareDevice(_window)) {
			DEBUG_PRINT("Failed to initialize rendering context. - {}\n", SDL_GetError())
			return;
		}

		SDL_ShowWindow(_window);

		if (!_currentScene->Create())	{
			return;
		}

#ifdef __EMSCRIPTEN__
		emscripten_set_main_loop_arg(App::EmscriptenUpdate, this, -1, true);

#else

		while (_running && EventsAndTimeStep()) {

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

	const App::timePoint& App::GetLastFrame() { return _lastFrameTime; }

	void App::EmscriptenUpdate(void* ptr) {
		dynamic_cast<App*>(ptr)->EventsAndTimeStep();
	}
#endif

	void App::InitLua(sol::state& lua) {
		lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);

		// Define useful enums and types.

		{
			lua.new_enum<FontStyle>("FontStyle",
				{
					std::make_pair("Normal", FontStyle::Normal),
					std::make_pair("Bold", FontStyle::Bold),
					std::make_pair("Italic", FontStyle::Italic),
					std::make_pair("Underlined", FontStyle::Underlined),
					std::make_pair("Bold_Italic", FontStyle::Bold_Italic),
					std::make_pair("Bold_Underlined", FontStyle::Bold_Underlined),
					std::make_pair("Bold_Italic_Underlined", FontStyle::Bold_Italic_Underlined),
					std::make_pair("Italic_Underlined", FontStyle::Italic_Underlined)
				}
			);
		}

		auto point = lua.new_usertype<SDL_Point>(
		"Vec2i", "x", &SDL_Point::x, "y", &SDL_Point::y
		);

		point["new"] = [](sol::optional<int> nx, sol::optional<int> ny) -> SDL_Point {
				return SDL_Point{ nx.value_or(0), ny.value_or(0) };
			};

		auto pointf = lua.new_usertype<SDL_FPoint>(
		"Vec2f", "x", &SDL_FPoint::x, "y", &SDL_FPoint::y
		);

		pointf["new"] = [](sol::optional<float> nx, sol::optional<float> ny) -> SDL_FPoint {
			return SDL_FPoint{ nx.value_or(0.0f), ny.value_or(0.0f) };
		};

		auto rect = lua.new_usertype<SDL_Rect>(
		"Recti",	"x", &SDL_Rect::x, "y", &SDL_Rect::y, "w", &SDL_Rect::w, "h", &SDL_Rect::h
		);

		rect["new"] = [](sol::optional<int> nx, sol::optional<int> ny, sol::optional<int> nw, sol::optional<int> nh) -> SDL_Rect {
				return SDL_Rect{ nx.value_or(0), ny.value_or(0), nw.value_or(0), nh.value_or(0) };
			};

		auto rectf = lua.new_usertype<SDL_FRect>(
		"Rectf", "x", &SDL_FRect::x, "y", &SDL_FRect::y, "w", &SDL_FRect::w, "h", &SDL_FRect::h
		);

		rectf["new"] = [](sol::optional<float> nx, sol::optional<float> ny, sol::optional<float> nw, sol::optional<float> nh) -> SDL_FRect {
				return SDL_FRect{ nx.value_or(0.0f), ny.value_or(0.0f), nw.value_or(0.0f), nh.value_or(0.0f) };
			};
	}

}
