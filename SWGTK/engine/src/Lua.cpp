#include <swgtk/App.hpp>
#include <swgtk/Font.hpp>
#include <swgtk/Input.hpp>
#include <swgtk/Lua.hpp>
#include <swgtk/Surface.hpp>
#include <swgtk/Timer.hpp>
#include <swgtk/Utility.hpp>

namespace swgtk {
  auto init_lua(swgtk::app *app_ptr, sol::state &lua, const lua_privledges priv) -> void {

    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::package);

    lua.safe_script_file(SWGTK_TABLE_LUA_FILE);

    auto swgtk_table = lua["swgtk"];

    // Define useful enums and types.

    swgtk_table["Vec2i"] = lua.new_usertype<SDL_Point>(
        "Vec2i", "x", &SDL_Point::x, "y", &SDL_Point::y);

    swgtk_table["Vec2i"]["new"] = [](const sol::optional<int> nx, const sol::optional<int> ny) -> SDL_Point {
      return SDL_Point{nx.value_or(0), ny.value_or(0)};
    };

    swgtk_table["Vec2f"] = lua.new_usertype<SDL_FPoint>(
        "Vec2f", std::make_pair("x", &SDL_FPoint::x), std::make_pair("y", &SDL_FPoint::y));

    swgtk_table["Vec2f"]["new"] = [](const sol::optional<float> nx, const sol::optional<float> ny) -> SDL_FPoint {
      return SDL_FPoint{nx.value_or(0.0f), ny.value_or(0.0f)};
    };

    swgtk_table["Recti"] = lua.new_usertype<SDL_Rect>(
        "Recti", "x", &SDL_Rect::x, "y", &SDL_Rect::y, "w", &SDL_Rect::w, "h", &SDL_Rect::h);

    swgtk_table["Recti"]["new"] = [](const sol::optional<int> nx, const sol::optional<int> ny, const sol::optional<int> nw, const sol::optional<int> nh) -> SDL_Rect {
      return SDL_Rect{nx.value_or(0), ny.value_or(0), nw.value_or(0), nh.value_or(0)};
    };

    swgtk_table["Rectf"] = lua.new_usertype<SDL_FRect>(
        "Rectf", "x", &SDL_FRect::x, "y", &SDL_FRect::y, "w", &SDL_FRect::w, "h", &SDL_FRect::h);

    swgtk_table["Rectf"]["new"] = [](const sol::optional<float> nx, const sol::optional<float> ny, const sol::optional<float> nw, const sol::optional<float> nh) -> SDL_FRect {
      return SDL_FRect{nx.value_or(0.0f), ny.value_or(0.0f), nw.value_or(0.0f), nh.value_or(0.0f)};
    };

    swgtk_table["GameTimer"] = lua.new_usertype<timer>("GameTimer", sol::constructors<timer()>());
    swgtk_table["GameTimer"]["GetSeconds"] = &timer::get_seconds;

    auto app_type = lua.new_usertype<app>("Host");
    app_type["DeltaTime"] = [app_ptr] -> float { return app_ptr->get_internal_clock()->get_seconds(); };
    swgtk_table["app_ptr"] = app_ptr;

    if ((priv & lua_privledges::fonts) == lua_privledges::fonts) {
      swgtk_table["Font"] = lua.new_usertype<font>("Font", sol::constructors<font(), font(TTF_Font*), font(const std::filesystem::path&, float)>());
      auto font_type = swgtk_table["Font"];
      lua.new_enum<font_style>("FontStyle",
                            {std::make_pair("Normal", font_style::normal),
                             std::make_pair("Bold", font_style::bold),
                             std::make_pair("Italic", font_style::italic),
                             std::make_pair("Underlined", font_style::underlined),
                             std::make_pair("Bold_Italic", font_style::bold_italic),
                             std::make_pair("Bold_Underlined", font_style::bold_underlined),
                             std::make_pair("Bold_Italic_Underlined", font_style::bold_italic_underlined),
                             std::make_pair("Italic_Underlined", font_style::italic_underlined),});

      swgtk_table["FontStyle"] = lua["FontStyle"];

      font_type["IsEmpty"] = &font::is_empty;
      font_type["SetStyle"] = &font::set_style;
      font_type["GetStyle"] = &font::get_style;
      font_type["LoadDefault"] = &font::load_default;
    }

    if ((priv & lua_privledges::user_input) == lua_privledges::user_input) {
      lua.new_enum<m_button>("MButton",
                            {
                                std::make_pair("None", m_button::none),
                                std::make_pair("Left", m_button::left),
                                std::make_pair("Middle", m_button::middle),
                                std::make_pair("Right", m_button::right),
                                std::make_pair("Ex1", m_button::ex1),
                                std::make_pair("Ex2", m_button::ex2),
                            });
      swgtk_table["MButton"] = lua["MButton"];

      lua.new_enum<key_mod>("KeyMod",
                           {
                               std::make_pair("None", key_mod::none),
                               std::make_pair("LShift", key_mod::l_shift),
                               std::make_pair("RShift", key_mod::r_shift),
                               std::make_pair("LCtrl", key_mod::l_ctrl),
                               std::make_pair("RCtrl", key_mod::r_ctrl),
                               std::make_pair("LAlt", key_mod::l_alt),
                               std::make_pair("RAlt", key_mod::r_alt),
                               std::make_pair("NumLock", key_mod::num_lock),
                               std::make_pair("Caps", key_mod::caps),
                               std::make_pair("Ctrl", key_mod::ctrl),
                               std::make_pair("Shift", key_mod::shift),
                               std::make_pair("Alt", key_mod::alt),
                           });
      swgtk_table["KeyMod"] = lua["KeyMod"];

      lua.new_enum<key_code>("KeyValue",
                            {
                                std::make_pair("Unknown", key_code::unknown),
                                std::make_pair("Back", key_code::back),
                                std::make_pair("Tab", key_code::tab),
                                std::make_pair("Enter", key_code::enter),
                                std::make_pair("Esc", key_code::esc),
                                std::make_pair("Space", key_code::space),
                                std::make_pair("Exlaim", key_code::exlaim),
                                std::make_pair("DblQuote", key_code::dbl_quote),
                                std::make_pair("Hash", key_code::hash),
                                std::make_pair("Dollar", key_code::dollar),
                                std::make_pair("Prct", key_code::prct),
                                std::make_pair("Amp", key_code::amp),
                                std::make_pair("Quote", key_code::quote),
                                std::make_pair("Ast", key_code::ast),
                                std::make_pair("Plus", key_code::plus),
                                std::make_pair("Comma", key_code::comma),
                                std::make_pair("Minus", key_code::minus),
                                std::make_pair("Period", key_code::period),
                                std::make_pair("FSlash", key_code::f_slash),
                                std::make_pair("R0", key_code::r0),
                                std::make_pair("R1", key_code::r1),
                                std::make_pair("R2", key_code::r2),
                                std::make_pair("R3", key_code::r3),
                                std::make_pair("R4", key_code::r4),
                                std::make_pair("R5", key_code::r5),
                                std::make_pair("R6", key_code::r6),
                                std::make_pair("R7", key_code::r7),
                                std::make_pair("R8", key_code::r8),
                                std::make_pair("R9", key_code::r9),
                                std::make_pair("Colon", key_code::colon),
                                std::make_pair("SemiColon", key_code::semi_colon),
                                std::make_pair("Less", key_code::less),
                                std::make_pair("Equals", key_code::equals),
                                std::make_pair("Greater", key_code::greater),
                                std::make_pair("Question", key_code::question),
                                std::make_pair("At", key_code::at),
                                std::make_pair("LBracket", key_code::l_bracket),
                                std::make_pair("BSlash", key_code::b_slash),
                                std::make_pair("RBracket", key_code::r_bracket),
                                std::make_pair("Caret", key_code::caret),
                                std::make_pair("UnderScore", key_code::under_score),
                                std::make_pair("BQuote", key_code::b_quote),
                                std::make_pair("A", key_code::a),
                                std::make_pair("B", key_code::b),
                                std::make_pair("C", key_code::c),
                                std::make_pair("D", key_code::d),
                                std::make_pair("E", key_code::e),
                                std::make_pair("F", key_code::f),
                                std::make_pair("G", key_code::g),
                                std::make_pair("H", key_code::h),
                                std::make_pair("I", key_code::i),
                                std::make_pair("J", key_code::j),
                                std::make_pair("K", key_code::k),
                                std::make_pair("L", key_code::l),
                                std::make_pair("M", key_code::m),
                                std::make_pair("N", key_code::n),
                                std::make_pair("O", key_code::o),
                                std::make_pair("P", key_code::p),
                                std::make_pair("Q", key_code::q),
                                std::make_pair("R", key_code::r),
                                std::make_pair("S", key_code::s),
                                std::make_pair("T", key_code::t),
                                std::make_pair("U", key_code::u),
                                std::make_pair("V", key_code::v),
                                std::make_pair("W", key_code::w),
                                std::make_pair("X", key_code::x),
                                std::make_pair("Y", key_code::y),
                                std::make_pair("Z", key_code::z),
                                std::make_pair("Delete", key_code::del),
                            });
      swgtk_table["KeyValue"] = lua["KeyValue"];

      lua.new_enum<layout_code>("KeyCode",
                               {
                                   std::make_pair("Unknown", layout_code::unknown),
                                   std::make_pair("A", layout_code::a),
                                   std::make_pair("B", layout_code::b),
                                   std::make_pair("c", layout_code::c),
                                   std::make_pair("D", layout_code::d),
                                   std::make_pair("E", layout_code::e),
                                   std::make_pair("F", layout_code::f),
                                   std::make_pair("G", layout_code::g),
                                   std::make_pair("H", layout_code::h),
                                   std::make_pair("I", layout_code::i),
                                   std::make_pair("J", layout_code::j),
                                   std::make_pair("K", layout_code::k),
                                   std::make_pair("L", layout_code::l),
                                   std::make_pair("M", layout_code::m),
                                   std::make_pair("N", layout_code::n),
                                   std::make_pair("O", layout_code::o),
                                   std::make_pair("P", layout_code::p),
                                   std::make_pair("Q", layout_code::q),
                                   std::make_pair("R", layout_code::r),
                                   std::make_pair("S", layout_code::s),
                                   std::make_pair("T", layout_code::t),
                                   std::make_pair("U", layout_code::u),
                                   std::make_pair("V", layout_code::v),
                                   std::make_pair("W", layout_code::w),
                                   std::make_pair("X", layout_code::x),
                                   std::make_pair("Y", layout_code::y),
                                   std::make_pair("Z", layout_code::z),
                                   std::make_pair("R1", layout_code::r1),
                                   std::make_pair("R2", layout_code::r2),
                                   std::make_pair("R3", layout_code::r3),
                                   std::make_pair("R4", layout_code::r4),
                                   std::make_pair("R5", layout_code::r5),
                                   std::make_pair("R6", layout_code::r6),
                                   std::make_pair("R7", layout_code::r7),
                                   std::make_pair("R8", layout_code::r8),
                                   std::make_pair("R9", layout_code::r9),
                                   std::make_pair("R0", layout_code::r0),
                                   std::make_pair("Enter", layout_code::enter),
                                   std::make_pair("Esc", layout_code::esc),
                                   std::make_pair("Back", layout_code::back),
                                   std::make_pair("Tab", layout_code::tab),
                                   std::make_pair("Space", layout_code::space),
                                   std::make_pair("Minus", layout_code::minus),
                                   std::make_pair("Equals", layout_code::equals),
                                   std::make_pair("LBracket", layout_code::l_bracket),
                                   std::make_pair("RBracket", layout_code::r_bracket),
                                   std::make_pair("BSlash", layout_code::b_slash),
                                   std::make_pair("SemiColon", layout_code::semi_colon),
                                   std::make_pair("Apostrophe", layout_code::apostrophe),
                                   std::make_pair("Comma", layout_code::comma),
                                   std::make_pair("Period", layout_code::period),
                                   std::make_pair("FSlash", layout_code::f_slash),
                                   std::make_pair("CapsLock", layout_code::caps_lock),
                                   std::make_pair("F1", layout_code::f1),
                                   std::make_pair("F2", layout_code::f2),
                                   std::make_pair("F3", layout_code::f3),
                                   std::make_pair("F4", layout_code::f4),
                                   std::make_pair("F5", layout_code::f5),
                                   std::make_pair("F6", layout_code::f6),
                                   std::make_pair("F7", layout_code::f7),
                                   std::make_pair("F8", layout_code::f8),
                                   std::make_pair("F9", layout_code::f9),
                                   std::make_pair("F10", layout_code::f10),
                                   std::make_pair("F11", layout_code::f11),
                                   std::make_pair("F12", layout_code::f12),
                                   std::make_pair("PRTSCRN", layout_code::prt_scrn),
                                   std::make_pair("ScrLock", layout_code::scr_lock),
                                   std::make_pair("Pause", layout_code::pause),
                                   std::make_pair("Insert", layout_code::insert),
                                   std::make_pair("Home", layout_code::home),
                                   std::make_pair("PgUp", layout_code::pg_up),
                                   std::make_pair("Delete", layout_code::del),
                                   std::make_pair("End", layout_code::end),
                                   std::make_pair("PgDown", layout_code::pg_down),
                                   std::make_pair("Right", layout_code::right),
                                   std::make_pair("Left", layout_code::left),
                                   std::make_pair("Down", layout_code::down),
                                   std::make_pair("Up", layout_code::up),
                                   std::make_pair("NumLock", layout_code::num_lock),
                                   std::make_pair("KPDivide", layout_code::kp_divide),
                                   std::make_pair("KPMultiply", layout_code::kp_multiply),
                                   std::make_pair("KPMinus", layout_code::kp_minus),
                                   std::make_pair("KPPlus", layout_code::kp_plus),
                                   std::make_pair("KPEnter", layout_code::kp_enter),
                                   std::make_pair("KP1", layout_code::kp1),
                                   std::make_pair("KP2", layout_code::kp2),
                                   std::make_pair("KP3", layout_code::kp3),
                                   std::make_pair("KP4", layout_code::kp4),
                                   std::make_pair("KP5", layout_code::kp5),
                                   std::make_pair("KP6", layout_code::kp6),
                                   std::make_pair("KP7", layout_code::kp7),
                                   std::make_pair("KP8", layout_code::kp8),
                                   std::make_pair("KP9", layout_code::kp9),
                                   std::make_pair("KP0", layout_code::kp0),
                                   std::make_pair("KPPeriod", layout_code::kp_period),
                                   std::make_pair("LCtrl", layout_code::l_ctrl),
                                   std::make_pair("LShift", layout_code::l_shift),
                                   std::make_pair("LAlt", layout_code::l_alt),
                                   std::make_pair("RCtrl", layout_code::r_ctrl),
                                   std::make_pair("RShift", layout_code::r_shift),
                                   std::make_pair("RAlt", layout_code::r_alt),
                               });
      swgtk_table["KeyCode"] = lua["KeyCode"];

      app_type["GetScrollX"] = &app::get_scroll_x;

      app_type["GetScrollY"] = &app::get_scroll_y;

      app_type["IsKeyPressed"] = &app::is_key_pressed;

      app_type["IsKeyReleased"] = &app::is_key_released;

      app_type["IsKeyHeld"] = &app::is_key_held;

      app_type["IsButtonPressed"] = &app::is_button_pressed;

      app_type["IsButtonReleased"] = &app::is_button_released;

      app_type["IsButtonHeld"] = &app::is_button_held;

      app_type["GetMouseClicks"] = &app::get_mouse_clicks;

      app_type["GetKeyMods"] = &app::get_key_mods;

      app_type["GetMouseX"] = &app::get_mouse_x;

      app_type["GetMouseY"] = &app::get_mouse_y;

      app_type["GetMousePos"] = &app::get_mouse_pos;
    }

    if ((priv & lua_privledges::window_control) == lua_privledges::window_control) {
      app_type["SetWindowSize"] = &app::set_window_size;

      app_type["SetTitle"] = &app::set_title;

      app_type["SetFullscreen"] = &app::set_fullscreen;

      app_type["ShowWindow"] = &app::show_window;

      app_type["HideWindow"] = &app::hide_window;

      app_type["RaiseWindow"] = &app::raise_window;

      app_type["RestoreWindow"] = &app::restore_window;

      app_type["MaximizeWindow"] = &app::maximize_window;

      app_type["MinimizeWindow"] = &app::minimize_window;

      app_type["IsFullscreenBorderless"] = &app::is_fullscreen_borderless;
    }

    // Define functions for the application.

    app_type["IsappRunning"] = &app::is_running;

    app_type["EventsAndTimeStep"] = &app::events_and_time_step;

    app_type["Closeapp"] = &app::close_app;

    app_type["GetWindowSize"] = &app::get_window_size;

    swgtk_table["Surface"] = lua.new_usertype<surface>("Surface",
                                                 sol::constructors<surface(), surface(SDL_Surface*), surface(const surface&),
                                                                   surface(const int, const int, const SDL_PixelFormat),
                                                                   surface(const int, const int, const SDL_PixelFormat, void*, const int)>());

    swgtk_table["Surface"]["Clear"] = &surface::clear;

    swgtk_table["Surface"]["ReadPixel"] = &surface::read_pixel;

    swgtk_table["Surface"]["DrawPixel"] = &surface::draw_pixel;

    swgtk_table["Surface"]["FillRect"] = &surface::fill_rect;

    swgtk_table["Surface"]["FillRects"] = &surface::fill_rects;
  }

} // namespace
