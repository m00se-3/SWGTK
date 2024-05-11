#ifndef SWGTK_RENDER_WRAP_HPP
#define SWGTK_RENDER_WRAP_HPP

#include "Texture.hpp"
#include "SDL2/SDL.h"

#include <SDL_pixels.h>
#include <SDL_render.h>
#include <unordered_map>
#include <string>

namespace swgtk
{
    class RenderWrapper
    {
    public:
	RenderWrapper(SDL_Renderer* ren) : _render(ren) {}

        void CreateTexture(const std::string& name, const std::string& file) { _textures.insert_or_assign(name, Texture{ _render, file }); }
        void SetDrawColor(uint8_t r, unint8_t g, uint8_t b, uint8_t a = 255u) { SDL_SetRenderDrawColor(_render, r, g, b, a); }

        [[nodiscard]] bool TextureExists(const std::string& name) { return _textures.contains(name); }

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
