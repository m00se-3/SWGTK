#ifndef SWGTK_TEXTURE_HPP
#define SWGTK_TEXTURE_HPP

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
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

		[[nodiscard]] constexpr auto operator*() const { return _texture.get(); }
		[[nodiscard]] constexpr auto Get() const { return _texture; }

		constexpr void SetBlendMode(SDL_BlendMode mode) { SDL_SetTextureBlendMode(_texture.get(), mode); }
		constexpr void SetTint(float r, float g, float b, float a) {
			SDL_SetTextureColorModFloat(_texture.get(), r, g, b);
			SDL_SetTextureAlphaModFloat(_texture.get(), a);
		}

		constexpr void SetScaleMode(SDL_ScaleMode mode) { SDL_SetTextureScaleMode(_texture.get(), mode); }

		[[nodiscard]] constexpr SDL_BlendMode GetBlendMode() const
		{
			SDL_BlendMode blend{};

			SDL_GetTextureBlendMode(_texture.get(), &blend);
			return blend;
		}

		[[nodiscard]] constexpr SDL_FColor GetTint() const 
		{
			SDL_FColor color{};

			SDL_GetTextureColorModFloat(_texture.get(), &color.r, &color.g, &color.b);
			SDL_GetTextureAlphaModFloat(_texture.get(), &color.a);

			return color;
		}

		[[nodiscard]] constexpr SDL_ScaleMode GetScaleMode() const {
			SDL_ScaleMode mode{};

			SDL_GetTextureScaleMode(_texture.get(), &mode);

			return mode;
		}

		[[nodiscard]] constexpr std::pair<float, float> GetSize() const {
			float w{}, y{};

			SDL_GetTextureSize(_texture.get(), &w, &y);
			return {w,y};
		}

	private:
		std::shared_ptr<SDL_Texture> _texture;
	};
}
#endif // !SWGTK_TEXTURE_HPP
