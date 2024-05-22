#ifndef SWGTK_RENDER_WRAP_HPP
#define SWGTK_RENDER_WRAP_HPP

#include "TTFFont.hpp"
#include "Texture.hpp"
#include "SDL2/SDL.h"

#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <SDL_render.h>
#include <gsl/gsl-lite.hpp>
#include <unordered_map>
#include <string>

namespace swgtk
{
/*
 * TODO: Write a function that returns a rendered text texture instead of drawing the text and deleting the texture.
*/
    class RenderWrapper
    {
    public:
	RenderWrapper(SDL_Renderer* ren) : _render(ren) {}

        void CreateTexture(const std::string& name, const std::string& file) { _textures.insert_or_assign(name, Texture{ gsl::make_not_null(_render), file }); }
        void SetDrawColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255u) { SDL_SetRenderDrawColor(_render, r, g, b, a); } // NOLINT

	void DrawTexture(gsl::not_null<SDL_Texture*> texture, SDL_Rect src, SDL_FRect dest);
	void DrawTextureEx(gsl::not_null<SDL_Texture*> texture, SDL_Rect src, SDL_FRect dest, double angle = 0.0, SDL_FPoint center = SDL_FPoint{0.f, 0.f});

	void DrawText(const std::string& text, SDL_Rect spot, gsl::not_null<TTF_Font*> font);

        [[nodiscard]] bool TextureExists(const std::string& name) const { return _textures.contains(name); }
	[[nodiscard]] SDL_Texture* GetTexture(const std::string& name) const 
	{
	    if(TextureExists(name)) 
	    { 
		return _textures.at(name).Get(); 
	    } 

	    return nullptr;
	}

        [[nodiscard]] SDL_Color GetDrawColor() const 
        {
	    SDL_Color res;
	    SDL_GetRenderDrawColor(_render, &res.r, &res.g, &res.b, &res.a);

	    return res;
	}

    private:
	SDL_Renderer* _render = nullptr;
	std::unordered_map<std::string, Texture> _textures;
    };
}

#endif // !SWGTK_RENDER_WRAP_HPP
