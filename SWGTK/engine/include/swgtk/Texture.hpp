#ifndef SWGTK_TEXTURE_HPP
#define SWGTK_TEXTURE_HPP

#include <SDL3/SDL_render.h>
#include <memory>

namespace swgtk
{
	/**
	*	@brief A simple RAII container class for SDL_Texture. This will delete the underlying SDL_Texure upon calling
	*	the destructor.
	*
	*	The texture class is not responsible for creating textures, however it can be used to modify existing ones.
	*/
	class Texture {
		static constexpr void DestroyTexture(SDL_Texture* texture) { SDL_DestroyTexture(texture); };

	public:
		constexpr Texture() = default;
		constexpr explicit Texture(SDL_Texture* texture) : _texture(std::shared_ptr<SDL_Texture>{texture, Texture::DestroyTexture}) {}

		[[nodiscard]] constexpr auto Get() const { return _texture; }

		constexpr void SetBlend(SDL_BlendMode mode) { SDL_SetTextureBlendMode(_texture.get(), mode); }
		constexpr void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

		[[nodiscard]] constexpr SDL_BlendMode GetBlend() const
		{
			SDL_BlendMode blend{};

			SDL_GetTextureBlendMode(_texture.get(), &blend);
			return blend;
		}

		[[nodiscard]] constexpr SDL_Color GetColor() const 
		{
			SDL_Color color{};

			SDL_GetTextureColorMod(_texture.get(), &color.r, &color.g, &color.b);
			SDL_GetTextureAlphaMod(_texture.get(), &color.a);

			return color;
		}

	private:
		std::shared_ptr<SDL_Texture> _texture;
	};
}
#endif // !SWGTK_TEXTURE_HPP
