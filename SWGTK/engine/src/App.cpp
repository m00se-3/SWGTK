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
#include "swgtk/ErrCodes.hpp"
#include <swgtk/App.hpp>
#include <swgtk/Utility.hpp>

#include <swgtk/Simple2DRenderer.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_video.h>
#include <gsl/gsl-lite.hpp>

#include <memory>
#include <utility>


namespace swgtk {
	App::~App() {
		_fonts.ClearFonts();

		SDL_DestroyWindow(_window);

		TTF_Quit();
		SDL_Quit();
	}

	bool App::InitGraphics(const char* appName, const int width, const int height, const std::shared_ptr<RendererBase> &renderPtr) {
		constexpr int InitFlags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;

		if (SDL_Init(InitFlags) && TTF_Init()) {

			if (_window = SDL_CreateWindow(appName, width, height, SDL_WINDOW_HIDDEN); _window != nullptr)	{
				SDL_SetWindowPosition(_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
				_renderer = renderPtr;
				return true;
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
		if(!_renderer->PrepareDevice(_window)) {
			DEBUG_PRINT("Failed to initialize rendering context. - {}\n", SDL_GetError())
			return false;
		}

		ShowWindow();
		_fonts.LoadDefaultFont();
		_renderer->SetFont(_fonts.GetDefaultFont().ptr);

		return _currentScene->Create();
	}

	void App::Run() {
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

	const App::timePoint& App::GetLastFrame() { return _lastFrameTime; }

	void App::EmscriptenUpdate(void* ptr) {
		dynamic_cast<App*>(ptr)->EventsAndTimeStep();
	}
#endif

	void App::InitLua(sol::state& lua, const LuaPrivledges priv) {
		
		lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);

		auto SWGTK = lua["swgtk"];

		// Define useful enums and types.

		SWGTK["Vec2i"] = lua.new_usertype<SDL_Point>(
		"Vec2i", "x", &SDL_Point::x, "y", &SDL_Point::y
		);

		SWGTK["Vec2i"]["new"] = [](const sol::optional<int> nx, const sol::optional<int> ny) -> SDL_Point {
				return SDL_Point{ nx.value_or(0), ny.value_or(0) };
			};

		SWGTK["Vec2f"] = lua.new_usertype<SDL_FPoint>(
		"Vec2f", "x", &SDL_FPoint::x, "y", &SDL_FPoint::y
		);

		SWGTK["Vec2f"]["new"] = [](const sol::optional<float> nx, const sol::optional<float> ny) -> SDL_FPoint {
			return SDL_FPoint{ nx.value_or(0.0f), ny.value_or(0.0f) };
		};

		SWGTK["Recti"] = lua.new_usertype<SDL_Rect>(
		"Recti",	"x", &SDL_Rect::x, "y", &SDL_Rect::y, "w", &SDL_Rect::w, "h", &SDL_Rect::h
		);

		SWGTK["Recti"]["new"] = [](const sol::optional<int> nx, const sol::optional<int> ny, const sol::optional<int> nw, const sol::optional<int> nh) -> SDL_Rect {
				return SDL_Rect{ nx.value_or(0), ny.value_or(0), nw.value_or(0), nh.value_or(0) };
			};

		SWGTK["Rectf"] = lua.new_usertype<SDL_FRect>(
		"Rectf", "x", &SDL_FRect::x, "y", &SDL_FRect::y, "w", &SDL_FRect::w, "h", &SDL_FRect::h
		);

		SWGTK["Rectf"]["new"] = [](const sol::optional<float> nx, const sol::optional<float> ny, const sol::optional<float> nw, const sol::optional<float> nh) -> SDL_FRect {
				return SDL_FRect{ nx.value_or(0.0f), ny.value_or(0.0f), nw.value_or(0.0f), nh.value_or(0.0f) };
			};
		
		SWGTK["GameTimer"] = lua.new_usertype<Timer>("GameTimer", sol::constructors<Timer()>());
		SWGTK["GameTimer"]["GetSeconds"] = &Timer::GetSeconds;

		auto App_Type = lua.new_usertype<App>("Host", "DeltaTime", &App::_gameTimer);
		SWGTK["App"] = this;

		if((priv & LuaPrivledges::Fonts) == LuaPrivledges::Fonts) {
			_fonts.InitLua(lua);
		}

		if((priv & LuaPrivledges::DrawCalls) == LuaPrivledges::DrawCalls) {
			_renderer->InitLua(&lua);
		}

		if((priv & LuaPrivledges::UserInput) == LuaPrivledges::UserInput) {
			lua.new_enum<MButton>("MButton",
				{
					std::make_pair("None", MButton::None),
					std::make_pair("Left", MButton::Left),
					std::make_pair("Middle", MButton::Middle),
					std::make_pair("Right", MButton::Right),
					std::make_pair("Ex1", MButton::Ex1),
					std::make_pair("Ex2", MButton::Ex2),
				}
			);
			SWGTK["MButton"] = lua["MButton"];

			lua.new_enum<KeyMod>("KeyMod",
				{
					std::make_pair("None", KeyMod::None),
					std::make_pair("LShift", KeyMod::LShift),
					std::make_pair("RShift", KeyMod::RShift),
					std::make_pair("LCtrl", KeyMod::LCtrl),
					std::make_pair("RCtrl", KeyMod::RCtrl),
					std::make_pair("LAlt", KeyMod::LAlt),
					std::make_pair("RAlt", KeyMod::RAlt),
					std::make_pair("NumLock", KeyMod::NumLock),
					std::make_pair("Caps", KeyMod::Caps),
					std::make_pair("Ctrl", KeyMod::Ctrl),
					std::make_pair("Shift", KeyMod::Shift),
					std::make_pair("Alt", KeyMod::Alt),
				}
			);
			SWGTK["KeyMod"] = lua["KeyMod"];

			lua.new_enum<KeyCode>("KeyValue",
				{
					std::make_pair("Unknown", KeyCode::Unknown), std::make_pair("Back", KeyCode::Back), std::make_pair("Tab", KeyCode::Tab), std::make_pair("Enter", KeyCode::Enter),
					std::make_pair("Esc", KeyCode::Esc), std::make_pair("Space", KeyCode::Space), std::make_pair("Exlaim", KeyCode::Exlaim), std::make_pair("DblQuote", KeyCode::DblQuote),
					std::make_pair("Hash", KeyCode::Hash), std::make_pair("Dollar", KeyCode::Dollar), std::make_pair("Prct", KeyCode::Prct), std::make_pair("Amp", KeyCode::Amp),
					std::make_pair("Quote", KeyCode::Quote), std::make_pair("Ast", KeyCode::Ast), std::make_pair("Plus", KeyCode::Plus), std::make_pair("Comma", KeyCode::Comma),
					std::make_pair("Minus", KeyCode::Minus), std::make_pair("Period", KeyCode::Period), std::make_pair("FSlash", KeyCode::FSlash), std::make_pair("R0", KeyCode::R0),
					std::make_pair("R1", KeyCode::R1), std::make_pair("R2", KeyCode::R2), std::make_pair("R3", KeyCode::R3), std::make_pair("R4", KeyCode::R4),
					std::make_pair("R5", KeyCode::R5), std::make_pair("R6", KeyCode::R6), std::make_pair("R7", KeyCode::R7), std::make_pair("R8", KeyCode::R8),
					std::make_pair("R9", KeyCode::R9), std::make_pair("Colon", KeyCode::Colon), std::make_pair("SemiColon", KeyCode::SemiColon), std::make_pair("Less", KeyCode::Less),
					std::make_pair("Equals", KeyCode::Equals), std::make_pair("Greater", KeyCode::Greater), std::make_pair("Question", KeyCode::Question), std::make_pair("At", KeyCode::At),
					std::make_pair("LBracket", KeyCode::LBracket), std::make_pair("BSlash", KeyCode::BSlash), std::make_pair("RBracket", KeyCode::RBracket), std::make_pair("Caret", KeyCode::Caret),
					std::make_pair("UnderScore", KeyCode::UnderScore), std::make_pair("BQuote", KeyCode::BQuote), std::make_pair("A", KeyCode::A), std::make_pair("B", KeyCode::B),
					std::make_pair("C", KeyCode::C), std::make_pair("D", KeyCode::D), std::make_pair("E", KeyCode::E), std::make_pair("F", KeyCode::F),
					std::make_pair("G", KeyCode::G), std::make_pair("H", KeyCode::H), std::make_pair("I", KeyCode::I), std::make_pair("J", KeyCode::J),
					std::make_pair("K", KeyCode::K), std::make_pair("L", KeyCode::L), std::make_pair("M", KeyCode::M), std::make_pair("N", KeyCode::N),
					std::make_pair("O", KeyCode::O), std::make_pair("P", KeyCode::P), std::make_pair("Q", KeyCode::Q), std::make_pair("R", KeyCode::R),
					std::make_pair("S", KeyCode::S), std::make_pair("T", KeyCode::T), std::make_pair("U", KeyCode::U), std::make_pair("V", KeyCode::V),
					std::make_pair("W", KeyCode::W), std::make_pair("X", KeyCode::X), std::make_pair("Y", KeyCode::Y), std::make_pair("Z", KeyCode::Z),
					std::make_pair("Delete", KeyCode::Delete),
				}
			);
			SWGTK["KeyValue"] = lua["KeyValue"];

			lua.new_enum<LayoutCode>("KeyCode",
				{
					std::make_pair("Unknown", LayoutCode::Unknown), std::make_pair("A", LayoutCode::A), std::make_pair("B", LayoutCode::B),
					std::make_pair("C", LayoutCode::C), std::make_pair("D", LayoutCode::D), std::make_pair("E", LayoutCode::E),
					std::make_pair("F", LayoutCode::F), std::make_pair("G", LayoutCode::G), std::make_pair("H", LayoutCode::H),
					std::make_pair("I", LayoutCode::I), std::make_pair("J", LayoutCode::J), std::make_pair("K", LayoutCode::K),
					std::make_pair("L", LayoutCode::L), std::make_pair("M", LayoutCode::M), std::make_pair("N", LayoutCode::N),
					std::make_pair("O", LayoutCode::O), std::make_pair("P", LayoutCode::P), std::make_pair("Q", LayoutCode::Q),
					std::make_pair("R", LayoutCode::R), std::make_pair("S", LayoutCode::S), std::make_pair("T", LayoutCode::T),
					std::make_pair("U", LayoutCode::U), std::make_pair("V", LayoutCode::V), std::make_pair("W", LayoutCode::W),
					std::make_pair("X", LayoutCode::X), std::make_pair("Y", LayoutCode::Y), std::make_pair("Z", LayoutCode::Z),
					std::make_pair("R1", LayoutCode::R1), std::make_pair("R2", LayoutCode::R2), std::make_pair("R3", LayoutCode::R3),
					std::make_pair("R4", LayoutCode::R4), std::make_pair("R5", LayoutCode::R5), std::make_pair("R6", LayoutCode::R6),
					std::make_pair("R7", LayoutCode::R7), std::make_pair("R8", LayoutCode::R8), std::make_pair("R9", LayoutCode::R9),
					std::make_pair("R0", LayoutCode::R0), std::make_pair("Enter", LayoutCode::Enter), std::make_pair("Esc", LayoutCode::Esc),
					std::make_pair("Back", LayoutCode::Back), std::make_pair("Tab", LayoutCode::Tab), std::make_pair("Space", LayoutCode::Space),
					std::make_pair("Minus", LayoutCode::Minus), std::make_pair("Equals", LayoutCode::Equals), std::make_pair("LBracket", LayoutCode::LBracket),
					std::make_pair("RBracket", LayoutCode::RBracket), std::make_pair("BSlash", LayoutCode::BSlash), std::make_pair("SemiColon", LayoutCode::SemiColon),
					std::make_pair("Apostrophe", LayoutCode::Apostrophe), std::make_pair("Comma", LayoutCode::Comma), std::make_pair("Period", LayoutCode::Period),
					std::make_pair("FSlash", LayoutCode::FSlash), std::make_pair("CapsLock", LayoutCode::CapsLock), std::make_pair("F1", LayoutCode::F1),
					std::make_pair("F2", LayoutCode::F2), std::make_pair("F3", LayoutCode::F3), std::make_pair("F4", LayoutCode::F4),
					std::make_pair("F5", LayoutCode::F5), std::make_pair("F6", LayoutCode::F6), std::make_pair("F7", LayoutCode::F7),
					std::make_pair("F8", LayoutCode::F8), std::make_pair("F9", LayoutCode::F9), std::make_pair("F10", LayoutCode::F10),
					std::make_pair("F11", LayoutCode::F11), std::make_pair("F12", LayoutCode::F12), std::make_pair("PRTSCRN", LayoutCode::PrtScrn),
					std::make_pair("ScrLock", LayoutCode::ScrLock), std::make_pair("Pause", LayoutCode::Pause), std::make_pair("Insert", LayoutCode::Insert),
					std::make_pair("Home", LayoutCode::Home), std::make_pair("PgUp", LayoutCode::PgUp), std::make_pair("Delete", LayoutCode::Delete),
					std::make_pair("End", LayoutCode::End), std::make_pair("PgDown", LayoutCode::PgDown), std::make_pair("Right", LayoutCode::Right),
					std::make_pair("Left", LayoutCode::Left), std::make_pair("Down", LayoutCode::Down), std::make_pair("Up", LayoutCode::Up),
					std::make_pair("NumLock", LayoutCode::NumLock), std::make_pair("KPDivide", LayoutCode::KPDivide), std::make_pair("KPMultiply", LayoutCode::KPMultiply),
					std::make_pair("KPMinus", LayoutCode::KPMinus), std::make_pair("KPPlus", LayoutCode::KPPlus), std::make_pair("KPEnter", LayoutCode::KPEnter),
					std::make_pair("KP1", LayoutCode::KP1), std::make_pair("KP2", LayoutCode::KP2), std::make_pair("KP3", LayoutCode::KP3),
					std::make_pair("KP4", LayoutCode::KP4), std::make_pair("KP5", LayoutCode::KP5), std::make_pair("KP6", LayoutCode::KP6),
					std::make_pair("KP7", LayoutCode::KP7), std::make_pair("KP8", LayoutCode::KP8), std::make_pair("KP9", LayoutCode::KP9),
					std::make_pair("KP0", LayoutCode::KP0), std::make_pair("KPPeriod", LayoutCode::KPPeriod), std::make_pair("LCtrl", LayoutCode::LCtrl),
					std::make_pair("LShift", LayoutCode::LShift), std::make_pair("LAlt", LayoutCode::LAlt), std::make_pair("RCtrl", LayoutCode::RCtrl),
					std::make_pair("RShift", LayoutCode::RShift), std::make_pair("RAlt", LayoutCode::RAlt),
				}
			);
			SWGTK["KeyCode"] = lua["KeyCode"];

			App_Type["GetScrollX"] = &App::GetScrollX;

			App_Type["GetScrollY"] = &App::GetScrollY;

			App_Type["IsKeyPressed"] = &App::IsKeyPressed;

			App_Type["IsKeyReleased"] = &App::IsKeyReleased;

			App_Type["IsKeyHeld"] = &App::IsKeyHeld;

			App_Type["IsButtonPressed"] = &App::IsButtonPressed;

			App_Type["IsButtonReleased"] = &App::IsButtonReleased;

			App_Type["IsButtonHeld"] = &App::IsButtonHeld;

			App_Type["GetMouseClicks"] = &App::GetMouseClicks;

			App_Type["GetKeyMods"] = &App::GetKeyMods;

			App_Type["GetMouseX"] = &App::GetKeyMods;

			App_Type["GetMouseY"] = &App::GetMouseY;

			App_Type["GetMousePos"] = &App::GetMousePos;
		}

		if((priv & LuaPrivledges::WindowControl) == LuaPrivledges::WindowControl) {
			App_Type["SetWindowSize"] = &App::SetWindowSize;

			App_Type["SetTitle"] = &App::SetTitle;

			App_Type["SetFullscreen"] = &App::SetFullscreen;

			App_Type["ShowWindow"] = &App::ShowWindow;

			App_Type["HideWindow"] = &App::HideWindow;

			App_Type["RaiseWindow"] = &App::RaiseWindow;

			App_Type["RestoreWindow"] = &App::RestoreWindow;

			App_Type["MaximizeWindow"] = &App::MaximizeWindow;

			App_Type["MinimizeWindow"] = &App::MinimizeWindow;

			App_Type["IsFullscreenBorderless"] = &App::IsFullscreenBorderless;
		}

		// Define functions for the application.

		App_Type["IsAppRunning"] = [this] () -> bool { return _running; };

		App_Type["EventsAndTimeStep"] = &App::EventsAndTimeStep;

		App_Type["CloseApp"] = &App::CloseApp;

		App_Type["GetWindowSize"] = &App::GetWindowSize;

		// We don't need float variations of these values and functions, because Lua uses the same number type for both.

		auto mathTable = SWGTK["math"];

		mathTable["pi"] = std::numbers::pi;
		mathTable["pi2"] = math::pi2;

		lua.set_function("RadiansToDegrees", [] (const double radians) { return math::RadiansToDegrees(radians); });
		mathTable["RadiansToDegrees"] = lua["RadiansToDegrees"];

		lua.set_function("DegreesToRadians", [] (const double degrees) { return math::DegreesToRadians(degrees); });
		mathTable["DegreesToRadians"] = lua["DegreesToRadians"];

		SWGTK["Surface"] = lua.new_usertype<Surface>("Surface",
			sol::constructors<Surface(), Surface(SDL_Surface*), Surface(const Surface&),
			Surface(const int, const int, const SDL_PixelFormat),
			Surface(const int, const int, const SDL_PixelFormat, void*, const int)>());

		SWGTK["Surface"]["Clear"] = &Surface::Clear;

		SWGTK["Surface"]["ReadPixel"] = &Surface::ReadPixel;

		SWGTK["Surface"]["DrawPixel"] = &Surface::DrawPixel;

		SWGTK["Surface"]["FillRect"] = &Surface::FillRect;

		SWGTK["Surface"]["FillRects"] = &Surface::FillRects;
	}

} // namespace swgtk
