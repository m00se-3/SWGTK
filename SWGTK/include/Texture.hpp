#ifndef SWGTK_TEXTURE_HPP
#define SWGTK_TEXTURE_HPP

#include <string>

#include "SDL2/SDL_render.h"

namespace swgtk
{
	/*
		A simple wrapper class for SDL_Texture.

		The class is able to reassign itself and release ownership of the pointer to another object.
		The copy constructor and copy assignment operators have been deleted to avoid needed to use reference
		counting for the underlying texture.
	*/
	class Texture {
	public:
		Texture() = default;
		Texture(SDL_Renderer* ren, const std::string& filepath);
		Texture(SDL_Renderer* ren, SDL_Surface* surface);
		Texture(Texture&& tex) noexcept;
		~Texture();

		// This will clean up the previous SDL_Texture, if it holds one, before taking ownership of the new SDL_Texture.
		Texture& operator=(SDL_Texture* tex);

		// This class should never copy, this is so that we don't have to worry about reference counting.
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;
		Texture& operator=(Texture&&) = delete;

		void SetBlend(const SDL_BlendMode& mode);
		void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		void Create(SDL_Renderer* ren, const std::string& filepath);
		void Create(SDL_Renderer* ren, SDL_Surface* surface);

		[[nodiscard]] SDL_BlendMode GetBlend() const;
		[[nodiscard]] SDL_Color GetColor() const;

		[[nodiscard]] SDL_Texture* Get() const;

		// Releases control of the SDL_Texture pointer to the caller.
		[[nodiscard]]SDL_Texture* Release();

	private:
		SDL_Texture* _texture = nullptr;
	};
}
#endif // !SWGTK_TEXTURE_HPP
