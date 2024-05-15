#include "RenderWrap.hpp"
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_ttf.h>
#include <gsl/gsl-lite.hpp>

namespace swgtk
{
	void RenderWrapper::DrawTexture(gsl::not_null<SDL_Texture*> texture, SDL_Rect src, SDL_FRect dest)
	{
		SDL_RenderCopyF(_render, texture, &src, &dest);
	}

	void RenderWrapper::DrawTextureEx(gsl::not_null<SDL_Texture*> texture, SDL_Rect src, SDL_FRect dest, double angle, SDL_FPoint center)
	{
		SDL_RenderCopyExF(_render, texture, &src, &dest, angle, &center, SDL_FLIP_NONE);
	}

	void RenderWrapper::DrawText(const std::string& text, SDL_Rect spot, gsl::not_null<TTF_Font*> font)
	{		
		auto* surface = TTF_RenderUTF8_Solid(font, text.c_str(), GetDrawColor());
		
		// This texture will get cleaned up at end of function.
		Texture textTexture{gsl::make_not_null(_render), gsl::make_not_null(surface)};
		SDL_FreeSurface(surface);

		SDL_RenderCopy(_render, textTexture.Get(), nullptr, &spot);
	}						
}
