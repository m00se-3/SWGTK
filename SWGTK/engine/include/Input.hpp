#ifndef SWGTK_INPUT_HPP
#define SWGTK_INPUT_HPP

#include "SDL2/SDL_mouse.h"
#include "SDL2/SDL_keycode.h"

namespace swgtk
{
	/*
		The following are wrapper values for the corresponding keyboard and mouse button SDL values. These values are based on the
		following webpages:

		Key codes and Layout codes: https://wiki.libsdl.org/SDL_Scancode
		Key modifiers:				https://wiki.libsdl.org/SDL_Keymod
		Mouse buttons:				https://github.com/libsdl-org/SDL/blob/main/include/SDL_mouse.h
	*/

	// SDL KeyCode implementation.
	enum class KeyCode : int32_t
	{
		Unknown = 0, Back = 8, Tab = 9, Enter = 13, Esc = 27, Space = 32, Exlaim = 33, DblQuote = 34, Hash = 35, Dollar = 36, Prct = 37,
		Amp = 38, Quote = 39, Ast = 42, Plus = 43, Comma = 44, Minus = 45, Period = 46, FSlash = 47, R0 = 48, R1 = 49, R2 = 50, R3 = 51,
		R4 = 52, R5 = 53, R6 = 54, R7 = 55, R8 = 56, R9 = 57, Colon = 58, SemiColon = 59, Less = 60, Equals = 61, Greater = 62, Question = 63,
		At = 64, LBracket = 91, BSlash = 92, RBracket = 93, Caret = 94, UnderScore = 95, BQuote = 96, A = 97, B = 98, C = 99, D = 100, E = 101,
		F = 102, G = 103, H = 104, I = 105, J = 106, K = 107, L = 108, M = 109, N = 110, O = 111, P = 112, Q = 113, R = 114, S = 115, T = 116,
		U = 117, V = 118, W = 119, X = 120, Y = 121, Z = 122, Delete = 127
	};

	// SDL ScanCode implementation.
	enum class LayoutCode : int32_t
	{
		Unknown = 0, A = 4, B = 5, C = 6, D = 7, E = 8, F = 9, G = 10, H = 11, I = 12, J = 13, K = 14, L = 15,
		M = 16, N = 17, O = 18, P = 19, Q = 20, R = 21, S = 22, T = 23, U = 24, V = 25, W = 26, X = 27, Y = 28, Z = 29,
		R1 = 30, R2 = 31, R3 = 32, R4 = 33, R5 = 34, R6 = 35, R7 = 36, R8 = 37, R9 = 38, R0 = 39, Enter = 40, Esc = 41,
		Back = 42, Tab = 43, Space = 44, Rminus = 45, Equals = 46, LBracket = 47, RBracket = 48, BSlash = 49,
		SemiColon = 51, Apostrophe = 52, Comma = 54, Period = 55, FSlash = 56, CapsLock = 57, F1 = 58, F2 = 59, F3 = 60,
		F4 = 61, F5 = 62, F6 = 63, F7 = 64, F8 = 65, F9 = 66, F10 = 67, F11 = 68, F12 = 69, PRTSCRN = 70, ScrLock = 71,
		Pause = 72, Insert = 73, Home = 74, PgUp = 75, Delete = 76, End = 77, PgDown = 78, Right = 79, Left = 80, Down = 81,
		Up = 82, NumLock = 83, KPDivide = 84, KPMultiply = 85, KPMinus = 86, KPPlus = 87, KPEnter = 88, KP1 = 89, KP2 = 90,
		KP3 = 91, KP4 = 92, KP5 = 93, KP6 = 94, KP7 = 95, KP8 = 96, KP9 = 97, KP0 = 98, KPPeriod = 99, LCtrl = 224, LShift = 225,
		LAlt = 226, RCtrl = 228, RShift = 229, RAlt = 230
	};

	// SDL Key Modifiers.
	enum class KeyMod : uint16_t
	{
		None = KMOD_NONE, LShift = KMOD_LSHIFT, RShift = KMOD_RSHIFT, LCtrl = KMOD_LCTRL, RCtrl = KMOD_RCTRL, LAlt = KMOD_LALT,
		RAlt = KMOD_RALT, NumLock = KMOD_NUM, Caps = KMOD_CAPS, Ctrl = KMOD_CTRL, Shift = KMOD_SHIFT, Alt = KMOD_ALT
	};

	// SDL Mouse button values.
	enum class MButton : uint32_t
	{
		None = 0, Left = SDL_BUTTON_LEFT, Middle = SDL_BUTTON_MIDDLE, Right = SDL_BUTTON_RIGHT, Ex1 = SDL_BUTTON_X1, Ex2 = SDL_BUTTON_X2
	};

	// Mouse button event states.
	enum class MButtonState : uint8_t
	{
		None = 0u, Pressed = 1u, Released = 2u
	};

	/*
		A container struct storing the current state of the mouse
	*/
	struct MouseState
	{
		MButton buttons;
		int x, y;
	};
}

#endif // !SWGTK_INPUT_HPP
