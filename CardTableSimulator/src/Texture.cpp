#include "Texture.hpp"

#include "SDL2/SDL_image.h"

namespace cts
{

	Texture::Texture(SDL_Renderer* ren, const std::string& filepath)
		: _texture(IMG_LoadTexture(ren, filepath.c_str()))
	{
		SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
	}

	Texture::Texture(SDL_Renderer* ren, SDL_Surface* surface)
		: _texture(SDL_CreateTextureFromSurface(ren, surface))
	{
		SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
	}

	Texture::Texture(Texture&& tex) noexcept
		: _texture(tex.Release())
	{
	}

	Texture& Texture::operator=(SDL_Texture* tex)
	{
		if (_texture) SDL_DestroyTexture(_texture);
		_texture = tex;
		return *this;
	}

	Texture::~Texture()
	{
		if(_texture) SDL_DestroyTexture(_texture);
	}

	SDL_Texture* Texture::Get() const { return _texture; }

	void Texture::SetBlend(const SDL_BlendMode& mode)
	{
		SDL_SetTextureBlendMode(_texture, mode);
	}

	void Texture::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		SDL_SetTextureColorMod(_texture, r, g, b);
		SDL_SetTextureAlphaMod(_texture, a);
	}

	void Texture::Create(SDL_Renderer* ren, const std::string& filepath)
	{
		if (_texture) SDL_DestroyTexture(_texture);
		_texture = IMG_LoadTexture(ren, filepath.c_str());
		SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
	}

	void Texture::Create(SDL_Renderer* ren, SDL_Surface* surface)
	{
		if (_texture) SDL_DestroyTexture(_texture);
		_texture = SDL_CreateTextureFromSurface(ren, surface);
		SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
	}

	SDL_BlendMode Texture::GetBlend() const
	{
		SDL_BlendMode m;
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

	SDL_Texture* Texture::Release()
	{
		SDL_Texture* temp = _texture;
		_texture = nullptr;
		return temp;
	}

}