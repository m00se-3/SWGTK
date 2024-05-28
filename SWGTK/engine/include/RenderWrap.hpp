#ifndef SWGTK_RENDER_WRAP_HPP
#define SWGTK_RENDER_WRAP_HPP

#include "TTFFont.hpp"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL.h"
#include <SDL_render.h>
#include <gsl/gsl-lite.hpp>
#include <unordered_map>
#include <string>

namespace swgtk
{
    class RenderWrapper
    {
    public:
	RenderWrapper(SDL_Renderer* ren) : _render(ren) {}

        void SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255u) { SDL_SetRenderDrawColor(_render, r, g, b, a); } // NOLINT

	void DrawTexture(gsl::not_null<SDL_Texture*> texture, SDL_Rect src, SDL_FRect dest);
	void DrawTextureEx(gsl::not_null<SDL_Texture*> texture, SDL_Rect src, SDL_FRect dest, double angle = 0.0, SDL_FPoint center = SDL_FPoint{0.f, 0.f}, SDL_RendererFlip flip = SDL_FLIP_NONE);

	void DrawText(const std::string& text, SDL_Rect spot, gsl::not_null<TTF_Font*> font);

	[[nodiscard]] gsl::owner<SDL_Texture*> RenderTextChunk(const std::string& text, gsl::not_null<TTF_Font*> font);

        [[nodiscard]] SDL_Color GetDrawColor() const 
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
