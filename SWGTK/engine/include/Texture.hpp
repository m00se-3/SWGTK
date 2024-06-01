#ifndef SWGTK_TEXTURE_HPP
#define SWGTK_TEXTURE_HPP

#include <gsl/gsl-lite.hpp>
#include "SDL2/SDL_render.h"

namespace swgtk
{
	/*
		A simple RAII container class for SDL_Texture. This will delete the underlying SDL_Texure upon calling
		the destructor.

		The texture class is not responsible for creating any textures, however it can be used to modify existing
		ones.

		The class is able to reassign itself and release ownership of the pointer to another object.
		The copy constructor and copy assignment operators have been deleted to avoid the need for reference
		counting.
	*/
	class Texture {
	public:
		constexpr Texture() = default;
		explicit constexpr Texture(gsl::owner<SDL_Texture*> ptr) : _texture(ptr) {}
		constexpr Texture(Texture&& tex) noexcept : _texture(tex.Release()) {}
		constexpr ~Texture() 
		{ 
			if(_texture != nullptr)
			{	
				SDL_DestroyTexture(_texture);
			}
		}

		// This will clean up the previous SDL_Texture, if it holds one, before taking ownership of the new SDL_Texture.
		constexpr Texture& operator=(SDL_Texture* tex);
		constexpr Texture& operator=(Texture&& other) noexcept;

		// This class should never copy, this is so that we don't have to worry about reference counting.
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		constexpr void SetBlend(const SDL_BlendMode& mode);
		constexpr void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		[[nodiscard]] constexpr SDL_BlendMode GetBlend() const;
		[[nodiscard]] constexpr SDL_Color GetColor() const;

		[[nodiscard]] SDL_Texture* Get() const;

		// Releases control of the SDL_Texture pointer to the caller.
		[[nodiscard]] constexpr gsl::owner<SDL_Texture*> Release();

	private:
		gsl::owner<SDL_Texture*> _texture = nullptr;
	};
}
#endif // !SWGTK_TEXTURE_HPP
