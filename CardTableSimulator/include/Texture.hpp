#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "SDL2/SDL_render.h"

#include <filesystem>

extern "C" {
	struct SDL_Surface;
}

namespace cts
{
	/*
		A simple wrapper class for SDL_Texture
	*/
	class Texture {

		SDL_Texture* _texture = nullptr;

	public:
		Texture() = default;
		Texture(SDL_Renderer* ren, const std::filesystem::path& filepath);
		Texture(SDL_Renderer* ren, SDL_Surface* surface);
		~Texture();

		// For now, until our requirements are more defined.
		Texture(const Texture&) = delete;
		Texture(Texture&&) noexcept = delete;

		Texture& operator=(const Texture& other);

		void Assign(SDL_Texture* tex);
		void SetBlend(const SDL_BlendMode& mode);
		void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		SDL_BlendMode GetBlend() const;
		SDL_Color GetColor() const;

		SDL_Texture* Get() const;

	};
}
#endif // !TEXTURE_HPP
