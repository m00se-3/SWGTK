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
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_INPUT_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_INPUT_HPP_

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_rect.h>
#include <array>
#include <span>
#include <utility>

namespace swgtk {
  /*
    The following are wrapper values for the corresponding keyboard and mouse button SDL values. These values are based on the
    following webpages:

    Key codes and Layout codes: https://wiki.libsdl.org/SDL3/SDL_Scancode
    Key modifiers:				https://wiki.libsdl.org/SDL3/SDL_Keymod
    Mouse buttons:				https://github.com/libsdl-org/SDL/blob/main/include/SDL_mouse.h
  */

  // SDL KeyCode implementation.
  enum class key_code : int32_t {
    unknown = 0,
    back = 8,
    tab = 9,
    enter = 13,
    esc = 27,
    space = 32,
    exlaim = 33,
    dbl_quote = 34,
    hash = 35,
    dollar = 36,
    prct = 37,
    amp = 38,
    quote = 39,
    ast = 42,
    plus = 43,
    comma = 44,
    minus = 45,
    period = 46,
    f_slash = 47,
    r0 = 48,
    r1 = 49,
    r2 = 50,
    r3 = 51,
    r4 = 52,
    r5 = 53,
    r6 = 54,
    r7 = 55,
    r8 = 56,
    r9 = 57,
    colon = 58,
    semi_colon = 59,
    less = 60,
    equals = 61,
    greater = 62,
    question = 63,
    at = 64,
    l_bracket = 91,
    b_slash = 92,
    r_bracket = 93,
    caret = 94,
    under_score = 95,
    b_quote = 96,
    a = 97,
    b = 98,
    c = 99,
    d = 100,
    e = 101,
    f = 102,
    g = 103,
    h = 104,
    i = 105,
    j = 106,
    k = 107,
    l = 108,
    m = 109,
    n = 110,
    o = 111,
    p = 112,
    q = 113,
    r = 114,
    s = 115,
    t = 116,
    u = 117,
    v = 118,
    w = 119,
    x = 120,
    y = 121,
    z = 122,
    del = 127
  };

  // SDL ScanCode implementation.
  enum class layout_code : int32_t {
    unknown = 0,
    a = 4,
    b = 5,
    c = 6,
    d = 7,
    e = 8,
    f = 9,
    g = 10,
    h = 11,
    i = 12,
    j = 13,
    k = 14,
    l = 15,
    m = 16,
    n = 17,
    o = 18,
    p = 19,
    q = 20,
    r = 21,
    s = 22,
    t = 23,
    u = 24,
    v = 25,
    w = 26,
    x = 27,
    y = 28,
    z = 29,
    r1 = 30,
    r2 = 31,
    r3 = 32,
    r4 = 33,
    r5 = 34,
    r6 = 35,
    r7 = 36,
    r8 = 37,
    r9 = 38,
    r0 = 39,
    enter = 40,
    esc = 41,
    back = 42,
    tab = 43,
    space = 44,
    minus = 45,
    equals = 46,
    l_bracket = 47,
    r_bracket = 48,
    b_slash = 49,
    semi_colon = 51,
    apostrophe = 52,
    comma = 54,
    period = 55,
    f_slash = 56,
    caps_lock = 57,
    f1 = 58,
    f2 = 59,
    f3 = 60,
    f4 = 61,
    f5 = 62,
    f6 = 63,
    f7 = 64,
    f8 = 65,
    f9 = 66,
    f10 = 67,
    f11 = 68,
    f12 = 69,
    prt_scrn = 70,
    scr_lock = 71,
    pause = 72,
    insert = 73,
    home = 74,
    pg_up = 75,
    del = 76,
    end = 77,
    pg_down = 78,
    right = 79,
    left = 80,
    down = 81,
    up = 82,
    num_lock = 83,
    kp_divide = 84,
    kp_multiply = 85,
    kp_minus = 86,
    kp_plus = 87,
    kp_enter = 88,
    kp1 = 89,
    kp2 = 90,
    kp3 = 91,
    kp4 = 92,
    kp5 = 93,
    kp6 = 94,
    kp7 = 95,
    kp8 = 96,
    kp9 = 97,
    kp0 = 98,
    kp_period = 99,
    l_ctrl = 224,
    l_shift = 225,
    l_alt = 226,
    r_ctrl = 228,
    r_shift = 229,
    r_alt = 230,
  };

  // SDL Key Modifiers.
  enum class key_mod : uint16_t {
    none = SDL_KMOD_NONE,
    l_shift = SDL_KMOD_LSHIFT,
    r_shift = SDL_KMOD_RSHIFT,
    l_ctrl = SDL_KMOD_LCTRL,
    r_ctrl = SDL_KMOD_RCTRL,
    l_alt = SDL_KMOD_LALT,
    r_alt = SDL_KMOD_RALT,
    num_lock = SDL_KMOD_NUM,
    caps = SDL_KMOD_CAPS,
    ctrl = SDL_KMOD_CTRL,
    shift = SDL_KMOD_SHIFT,
    alt = SDL_KMOD_ALT,
  };

  // SDL Mouse button values.
  enum class m_button : uint32_t {
    none = 0,
    left = SDL_BUTTON_LEFT,
    middle = SDL_BUTTON_MIDDLE,
    right = SDL_BUTTON_RIGHT,
    ex1 = SDL_BUTTON_X1,
    ex2 = SDL_BUTTON_X2,
  };

  // Mouse button event states.
  enum class m_button_state : uint8_t {
    none = 0u,
    pressed = 1u,
    released = 2u,
  };

  struct m_button_data {
    m_button_state state{};
    uint8_t clicks{};
  };

  /*
    A container struct storing the current state of the mouse
  */
  struct mouse_state {
    m_button buttons;
    float x, y;
  };

  struct input_system {
    static constexpr auto number_of_mouse_buttons = 6u;

    /*
      State management variables for input polling.
    */

    mouse_state mouseState{};
    key_mod modifiers = key_mod::none;
    std::span<const bool> keyboardState;

    /*
      Variables for processing input events.
    */

    std::array<m_button_data, number_of_mouse_buttons> mouseEvents = {m_button_data{.state = m_button_state::none, .clicks = 0u}};
    std::pair<layout_code, bool> keyEvent = std::make_pair(layout_code::unknown, false);
    SDL_FPoint scroll{};
  };
} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_INPUT_HPP_
