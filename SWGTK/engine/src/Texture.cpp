#include "Texture.hpp"
#include <gsl/gsl-lite.hpp>

namespace swgtk
{
	Texture& Texture::operator=(SDL_Texture* tex)
	{
		if (_texture != nullptr) 
		{
			SDL_DestroyTexture(_texture);
		}
		_texture = gsl::owner<SDL_Texture*>(tex);
		return *this;
	}

	Texture& Texture::operator=(Texture&& other) noexcept
	{
		if(_texture != nullptr)
		{
			SDL_DestroyTexture(_texture);
		}

		_texture = other.Release();
		return *this;
	}

	Texture::~Texture()
	{
		if(_texture != nullptr) { SDL_DestroyTexture(_texture); }
	}

	SDL_Texture* Texture::Get() const { return _texture; } // NOLINT

	void Texture::SetBlend(const SDL_BlendMode& mode)
	{
		SDL_SetTextureBlendMode(_texture, mode);
	}

	void Texture::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		SDL_SetTextureColorMod(_texture, r, g, b);
		SDL_SetTextureAlphaMod(_texture, a);
	}

	SDL_BlendMode Texture::GetBlend() const
	{
		SDL_BlendMode m{};
		SDL_GetTextureBlendMode(_texture, &m);
		return m;
	}

	SDL_Color Texture::GetColor() const
	{
		SDL_Color c{};
		SDL_GetTextureColorMod(_texture, &c.r, &c.b, &c.g);
		SDL_GetTextureAlphaMod(_texture, &c.a);
		return c;
	}

	gsl::owner<SDL_Texture*> Texture::Release()
	{
		SDL_Texture* temp = _texture;
		_texture = nullptr;
		return temp;
	}

}
