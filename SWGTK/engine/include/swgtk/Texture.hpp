/*
    MIT License
    Copyright (c) 2023 Samuel Bridgham

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_TEXTURE_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_TEXTURE_HPP_

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <memory>
#include <utility>

namespace swgtk {
	/**
	*	@brief A simple, reference-counted, RAII container class for SDL_Texture. This will delete the underlying SDL_Texture upon calling
	*	the destructor.
	*
	*	When creating a SDL_Texture, you are responsible for calling the appropriate SDL function for your use case. However,
	*	the Texture class will reference count your SDL_Texture and clean it up for you when all references are destroyed.
	*/
	class Texture {
		static void DestroyTexture(SDL_Texture* texture) { SDL_DestroyTexture(texture); }

	public:
		Texture() = default;
		explicit Texture(SDL_Texture* texture) : _texture(std::shared_ptr<SDL_Texture>{texture, Texture::DestroyTexture}) {}

		[[nodiscard]] auto operator*(this auto&& self) { return self._texture.get(); }
		[[nodiscard]] auto Get(this auto&& self) { return self._texture; }

		void SetBlendMode(const SDL_BlendMode mode) const { SDL_SetTextureBlendMode(_texture.get(), mode); }

		void SetTint(const SDL_FColor& color) const {
			SDL_SetTextureColorModFloat(_texture.get(), color.r, color.g, color.b);
			SDL_SetTextureAlphaModFloat(_texture.get(), color.a);
		}

		void SetScaleMode(const SDL_ScaleMode mode) const { SDL_SetTextureScaleMode(_texture.get(), mode); }

		[[nodiscard]] SDL_BlendMode GetBlendMode() const
		{
			SDL_BlendMode blend{};

			SDL_GetTextureBlendMode(_texture.get(), &blend);
			return blend;
		}

		[[nodiscard]] SDL_FColor GetTint() const 
		{
			SDL_FColor color{};

			SDL_GetTextureColorModFloat(_texture.get(), &color.r, &color.g, &color.b);
			SDL_GetTextureAlphaModFloat(_texture.get(), &color.a);

			return color;
		}

		[[nodiscard]] SDL_ScaleMode GetScaleMode() const {
			SDL_ScaleMode mode{};

			SDL_GetTextureScaleMode(_texture.get(), &mode);

			return mode;
		}

		[[nodiscard]] std::pair<float, float> GetSize() const {
			float w{}, y{};

			SDL_GetTextureSize(_texture.get(), &w, &y);
			return {w,y};
		}

	private:
		std::shared_ptr<SDL_Texture> _texture;
	};
} // namespace swgtk
#endif // SWGTK_ENGINE_INCLUDE_SWGTK_TEXTURE_HPP_
