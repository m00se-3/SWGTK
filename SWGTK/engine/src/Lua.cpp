#include <swgtk/Lua.hpp>
#include <swgtk/Surface.hpp>
#include <swgtk/Input.hpp>
#include <swgtk/FontGroup.hpp>
#include <swgtk/Timer.hpp>
#include <swgtk/App.hpp>

namespace {
	void InitLuaFonts(swgtk::FontGroup* fonts, sol::state& lua);
}

namespace swgtk {
    void InitLua(App* app, sol::state& lua, const LuaPrivledges priv) {

		lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::package);

		lua.safe_script_file(SWGTK_TABLE_LUA_FILE);

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

		auto App_Type = lua.new_usertype<App>("Host");
    	App_Type["DeltaTime"] = [app] { return app->GetInternalClock()->GetSeconds(); };
		SWGTK["App"] = app;

		if((priv & LuaPrivledges::Fonts) == LuaPrivledges::Fonts) {
			InitLuaFonts(app->GetFontHandle(), lua);
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

		App_Type["IsAppRunning"] = &App::IsRunning;

		App_Type["EventsAndTimeStep"] = &App::EventsAndTimeStep;

		App_Type["CloseApp"] = &App::CloseApp;

		App_Type["GetWindowSize"] = &App::GetWindowSize;

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
}

namespace {
	void InitLuaFonts(swgtk::FontGroup* fonts, sol::state& lua) {
		using namespace swgtk;

		auto SWGTK = lua["swgtk"];

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

		SWGTK["FontStyle"] = lua["FontStyle"];

		auto FontGroup_Type = lua.new_usertype<FontGroup>("FontGroup", sol::constructors<FontGroup()>());
		SWGTK["Fonts"] = fonts;

		auto Font_Type = lua.new_usertype<Font>("FontHandle");

		FontGroup_Type["GetDefaultFont"] = [] (const FontGroup& self) { return self.GetDefaultFont(); };

		FontGroup_Type["SetDefaultFontSize"] = [] (FontGroup& self, const float size) { self.SetDefaultFontSize(size); };

		FontGroup_Type["SetAllFontSizes"] = [] (const FontGroup& self, const float size) { self.SetAllFontSizes(size); };

		FontGroup_Type["AddFont"] = [] (FontGroup& self, const std::filesystem::path& filename) { self.AddFont(filename); };

		FontGroup_Type["GetFont"] = [] (const FontGroup& self, const std::string& name) { return self.GetFont(name); };

		FontGroup_Type["SetFontStyle"] = [] (const Font font, const FontStyle style) { FontGroup::SetFontStyle(font, style); };

		FontGroup_Type["GetFontStyle"] = [] (const Font font) { return FontGroup::GetFontStyle(font); };

		FontGroup_Type["ClearFonts"] = [] (const FontGroup& self) { self.ClearFonts(); };
	}

}