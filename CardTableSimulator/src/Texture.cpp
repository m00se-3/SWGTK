#include "Texture.hpp"

#include "SDL2/SDL_surface.h"
#include "SDL2/SDL_image.h"

Texture::Texture(SDL_Renderer* ren, const std::filesystem::path& filepath)
	: _texture(IMG_LoadTexture(ren, filepath.string().c_str()))
{
}

Texture::Texture(SDL_Renderer* ren, SDL_Surface* surface)
	: _texture(SDL_CreateTextureFromSurface(ren, surface))
{
}

Texture::~Texture()
{
	SDL_DestroyTexture(_texture);
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
