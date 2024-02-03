#include "Input.hpp"

namespace swgtk
{
	Key Key::FromValue(KeyCode value)
	{
		return Key{ LayoutCode(SDL_GetScancodeFromKey(uint8_t(value))), value, KeyMod(SDL_GetModState()) };
	}

	Key Key::FromCode(LayoutCode code)
	{
		return Key{ code, KeyCode(SDL_GetKeyFromScancode(SDL_Scancode(code))), KeyMod(SDL_GetModState()) };
	}
}