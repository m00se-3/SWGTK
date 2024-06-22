#ifndef SWGTK_RENDER_WRAP_HPP
#define SWGTK_RENDER_WRAP_HPP

#include "Texture.hpp"

#include "SDL_ttf.h"
#include "SDL_blendmode.h"
#include "SDL_render.h"
#include "gsl/gsl-lite.hpp"

#include <string_view>
#include <span>

namespace swgtk
{
    class RenderWrapper
    {
    public:
	constexpr RenderWrapper(SDL_Renderer* ren) : _render(ren) {}

	constexpr void SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255u) { SDL_SetRenderDrawColor(_render, r, g, b, a); } // NOLINT

	/*
	    Texture source rectangles are taken with integers while the destination is set with floats.

	    It is okay to allow nullptr for the rectangles, since SDL handles nullptr for us.
	*/
	constexpr void DrawTexture(gsl::not_null<SDL_Texture*> texture, SDL_Rect* src, SDL_FRect* dest)
	{
	    SDL_RenderCopyF(_render, texture, src, dest);
	}

	/*
	    Texture source rectangles are taken with integers while the destination and center are set with floats.

	    It is okay to allow nullptr for the source, destination, and center, since SDL handles nullptr for us.
	*/
	constexpr void DrawTexture(gsl::not_null<SDL_Texture*> texture, SDL_Rect* src, SDL_FRect* dest, double angle, SDL_FPoint* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE)
	{
	    SDL_RenderCopyExF(_render, texture, src, dest, angle, center, flip);
	}
 
	/*
	    It is okay to allow nullptr for the destination, since SDL handles nullptr for us.
	*/
	constexpr void DrawText(const std::string_view& text, gsl::not_null<TTF_Font*> font, SDL_Rect* dest)
	{		
	    auto* surface = TTF_RenderUTF8_Solid(font, text.data(), GetDrawColor());
	    
	    // This texture will get cleaned up at end of function.
	    const Texture textTexture{SDL_CreateTextureFromSurface(_render, surface)};
	    SDL_FreeSurface(surface);

	    SDL_RenderCopy(_render, textTexture.Get(), nullptr, dest);
	}						

	/*
	    Used to draw arbitray shapes from a list of verticies. This is helpful when drawing things like particles.

	    It's okay to allow for nullptr in 'texture' because SDL handles it for us.
	*/ 
	constexpr void DrawGeomerty(SDL_Texture* texture, std::span<SDL_Vertex> verticies, std::span<int> indicies)
	{
	    [[maybe_unused]] auto result = SDL_RenderGeometry(_render, texture, verticies.data(), static_cast<int>(std::ssize(verticies)),
								indicies.data(), static_cast<int>(std::ssize(indicies)));
	}

	[[nodiscard]] constexpr gsl::owner<SDL_Texture*> RenderTextChunk(const std::string_view& text, gsl::not_null<TTF_Font*> font)
	{
	    auto* surface = TTF_RenderUTF8_Solid(font, text.data(), GetDrawColor());

	    auto* textTexture = SDL_CreateTextureFromSurface(_render, surface);
	    SDL_FreeSurface(surface);

	    return textTexture;
	}

	[[nodiscard]] Texture LoadTextureImg(const std::string_view& img, SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND);

	[[nodiscard]] constexpr SDL_Color GetDrawColor() const 
	{
	    SDL_Color res;
	    SDL_GetRenderDrawColor(_render, &res.r, &res.g, &res.b, &res.a);

	    return res;
	}

	constexpr void SetDrawTarget(SDL_Texture* ptr = nullptr) { [[maybe_unused]] auto _ = SDL_SetRenderTarget(_render, ptr); }

    private:
	SDL_Renderer* _render = nullptr;
    };
}

#endif // !SWGTK_RENDER_WRAP_HPP
