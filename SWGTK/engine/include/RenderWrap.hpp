#ifndef SWGTK_RENDER_WRAP_HPP
#define SWGTK_RENDER_WRAP_HPP

#include "Texture.hpp"
#include "TTFFont.hpp"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include <SDL_render.h>
#include <gsl/gsl-lite.hpp>
#include <string_view>
#include <optional>

namespace swgtk
{
    class RenderWrapper
    {
    public:
	constexpr RenderWrapper(SDL_Renderer* ren) : _render(ren) {}

        constexpr void SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255u) { SDL_SetRenderDrawColor(_render, r, g, b, a); } // NOLINT

	constexpr void DrawTexture(gsl::not_null<SDL_Texture*> texture, std::optional<SDL_Rect> src, std::optional<SDL_FRect> dest)
	{
		SDL_RenderCopyF(_render, texture, (src) ? &src.value() : nullptr, (dest) ? &dest.value() : nullptr);
	}

	constexpr void DrawTextureEx(gsl::not_null<SDL_Texture*> texture, SDL_Rect src, SDL_FRect dest, double angle = 0.0, SDL_FPoint center = SDL_FPoint{0.f, 0.f}, SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
		SDL_RenderCopyExF(_render, texture, &src, &dest, angle, &center, flip);
	}

	constexpr void DrawText(const std::string_view& text, SDL_Rect spot, gsl::not_null<TTF_Font*> font)
	{		
		auto* surface = TTF_RenderUTF8_Solid(font, text.data(), GetDrawColor());
		
		// This texture will get cleaned up at end of function.
		Texture textTexture{SDL_CreateTextureFromSurface(_render, surface)};
		SDL_FreeSurface(surface);

		SDL_RenderCopy(_render, textTexture.Get(), nullptr, &spot);
	}						

	[[nodiscard]] constexpr gsl::owner<SDL_Texture*> RenderTextChunk(const std::string& text, gsl::not_null<TTF_Font*> font)
	{
		auto* surface = TTF_RenderUTF8_Solid(font, text.c_str(), GetDrawColor());

		auto* textTexture = SDL_CreateTextureFromSurface(_render, surface);
		SDL_FreeSurface(surface);

		return textTexture;
	}

        [[nodiscard]] constexpr SDL_Color GetDrawColor() const 
        {
	    SDL_Color res;
	    SDL_GetRenderDrawColor(_render, &res.r, &res.g, &res.b, &res.a);

	    return res;
	}

    private:
	SDL_Renderer* _render = nullptr;
    };
}

#endif // !SWGTK_RENDER_WRAP_HPP
