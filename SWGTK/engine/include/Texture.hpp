#ifndef SWGTK_TEXTURE_HPP
#define SWGTK_TEXTURE_HPP

#include <gsl/gsl-lite.hpp>
#include "SDL_render.h"

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

		constexpr Texture& operator=(gsl::owner<SDL_Texture*> tex)
		{
			if(_texture != nullptr)
			{
				SDL_DestroyTexture(_texture);
			}

			_texture = tex;
			return *this;
		}

		constexpr Texture& operator=(Texture&& other) noexcept
		{
			if(_texture != nullptr)
			{
				SDL_DestroyTexture(_texture);
			}

			_texture = other.Release();
			return *this;
		}

		// This class should never copy, this is so that we don't have to worry about reference counting.
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		constexpr void SetBlend(SDL_BlendMode mode) { SDL_SetTextureBlendMode(_texture, mode); }
		constexpr void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		[[nodiscard]] constexpr SDL_BlendMode GetBlend() const
		{
			SDL_BlendMode blend{};

			SDL_GetTextureBlendMode(_texture, &blend);
			return blend;
		}

		[[nodiscard]] constexpr SDL_Color GetColor() const 
		{
			SDL_Color color{};

			SDL_GetTextureColorMod(_texture, &color.r, &color.g, &color.b);
			SDL_GetTextureAlphaMod(_texture, &color.a);

			return color;
		}

		[[nodiscard]] constexpr SDL_Texture* Get() const { return _texture; } // NOLINT

		// Releases control of the SDL_Texture pointer to the caller.
		[[nodiscard]] constexpr gsl::owner<SDL_Texture*> Release()
		{
			auto* temp = _texture;
			
			_texture = nullptr;
			return temp;
		}

	private:
		gsl::owner<SDL_Texture*> _texture = nullptr;
	};
}
#endif // !SWGTK_TEXTURE_HPP
