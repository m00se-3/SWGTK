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
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_SIMPLE2DRENDERER_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_SIMPLE2DRENDERER_HPP_

#include "swgtk/Texture.hpp"

#include "SDL3_ttf/SDL_ttf.h"
#include "SDL3/SDL_blendmode.h"
#include "SDL3/SDL_render.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <swgtk/RenderingDevice.hpp>
#include <swgtk/Utility.hpp>
#include <swgtk/Surface.hpp>
#include <sol/sol.hpp>

#include <memory>
#include <filesystem>
#include <string_view>
#include <span>
#include <optional>

namespace swgtk {

	/**
	 * @brief This is currently the class you want to use for basic hardware 2D rendering.
	   This is based on SDL3's hardware accelerated 2D rendering backend. It does not support shaders.
	 * 
	 */
    class Simple2DRenderer : public RenderingDevice, public std::enable_shared_from_this<Simple2DRenderer>{
	public:

		constexpr Simple2DRenderer() = default;
		Simple2DRenderer(const Simple2DRenderer &) = delete;
		Simple2DRenderer(Simple2DRenderer &&) noexcept = delete;
		Simple2DRenderer &operator=(const Simple2DRenderer &) = delete;
		Simple2DRenderer &operator=(Simple2DRenderer &&) noexcept = delete;
		~Simple2DRenderer() override { Simple2DRenderer::DestroyDevice(); }

		void BufferClear(const SDL_FColor& color = SDL_FColor{ .r=0.0f, .g=0.0f, .b=0.0f, .a=1.0f}) override;
		void BufferPresent() override;

		void SetBackgroundColor(const SDL_FColor& color) override { SetDrawColor(color); }
		[[nodiscard]] constexpr bool IsDeviceInitialized() const override { return _render != nullptr; }

		void SetFont(TTF_Font* font) override { _currentFont = font; }

		[[nodiscard]] bool PrepareDevice(const std::any& window_ptr) override;
		void DestroyDevice() override;

		/**
		 * @brief SDL3 has several vsync options that you can set. This function wraps that functionality.
		 * 
		 * @param value 
		 */
		void SetVSync(const VSync value) override { SDL_SetRenderVSync(_render, std::to_underlying(value)); }

		[[nodiscard]] VSync GetVSync() const override { 
			int ret{};
			SDL_GetRenderVSync(_render, &ret);
			return VSync{ret};
		}

		[[nodiscard]] std::weak_ptr<RenderingDevice> GetRef() override { return shared_from_this(); }

		void SetDrawColor(const float r, const float g, const float b, const float a = defaultAlphaFloat) const { SDL_SetRenderDrawColorFloat(_render, r, g, b, a); }
        void SetDrawColor(const SDL_FColor& color = SDL_FColor{.r=0.0f, .g=0.0f, .b=0.0f, .a=defaultAlphaFloat}) const {
			SDL_SetRenderDrawColorFloat(_render, color.r, color.g, color.b, color.a);
		}

		void DrawTexture(Texture texture, const std::optional<SDL_FRect>& src = std::nullopt, const std::optional<SDL_FRect>& dest = std::nullopt) const;
		void DrawTexture(Texture texture, const std::optional<SDL_FRect> &src,
		                 const std::optional<SDL_FRect> &dest, double angle,
		                 const std::optional<SDL_FPoint> &center = std::nullopt,
		                 SDL_FlipMode flip = SDL_FLIP_NONE) const;
		
		/**
		 * @brief Draw text at the specified location with the specified font. Uses SDL_ttf's fastest algorithm.
		 * 
		 * @param text 
		 * @param pos - Destination rectangle
		 * @param color
		 */
		void DrawPlainText(std::string_view text, const SDL_FRect& pos,
		                   const SDL_Color &color = SDL_Color{
			                   .r = defaultAlphaInt, .g = defaultAlphaInt, .b = defaultAlphaInt, .a = defaultAlphaInt
		                   }) const;
		
		/**
		* @brief Same as DrawPlainText() except it allows you to specify word wrapping support.
		* 
		* @param text 
		* @param pos 
		* @param wrapLen Length of text before wrapping, in bytes.
		* @param color 
		*/
    	void DrawPlainWrapText(std::string_view text, const SDL_FRect& pos, int wrapLen = 0,
												  const SDL_Color &color = SDL_Color{
													  .r = defaultAlphaInt, .g = defaultAlphaInt, .b = defaultAlphaInt, .a = defaultAlphaInt
												  }) const;

		/*
			Combines SDL_ttf's API with SDL_Textures to preload text renderables as Textures. These can be rotated and tinted as needed.
		*/

		[[nodiscard]] Texture LoadPlainText(std::string_view text,
		                                    const SDL_Color &color = SDL_Color{
			                                    .r = defaultAlphaInt, .g = defaultAlphaInt, .b = defaultAlphaInt, .a = defaultAlphaInt
		                                    }) const;

		[[nodiscard]] Texture LoadBlendedText(std::string_view text,
		                                      const SDL_Color &color = SDL_Color{
			                                      .r = defaultAlphaInt, .g = defaultAlphaInt, .b = defaultAlphaInt, .a = defaultAlphaInt
		                                      }) const;

		[[nodiscard]] Texture LoadShadedText(std::string_view text,
		                                     const SDL_Color &bg = SDL_Color{.r = 0u, .g = 0u, .b = 0u, .a = defaultAlphaInt},
		                                     const SDL_Color &fg = SDL_Color{.r = defaultAlphaInt, .g = defaultAlphaInt, .b = defaultAlphaInt, .a = defaultAlphaInt}) const;

		[[nodiscard]] Texture LoadLCDText(std::string_view text,
		                                  const SDL_Color &bg = SDL_Color{.r = 0u, .g = 0u, .b = 0u, .a = defaultAlphaInt},
		                                  const SDL_Color &fg = SDL_Color{.r = defaultAlphaInt, .g = defaultAlphaInt, .b = defaultAlphaInt, .a = defaultAlphaInt}) const;

		[[nodiscard]] Texture LoadPlainWrapText(std::string_view text, int wrapLen = 0,
		                                        const SDL_Color &color = SDL_Color{
			                                        .r = defaultAlphaInt, .g = defaultAlphaInt, .b = defaultAlphaInt, .a = defaultAlphaInt
		                                        }) const;

		[[nodiscard]] Texture LoadBlendedWrapText(std::string_view text, int wrapLen = 0,
		                                          const SDL_Color &color = SDL_Color{
			                                          .r = defaultAlphaInt, .g = defaultAlphaInt, .b = defaultAlphaInt, .a = defaultAlphaInt
		                                          }) const;

		[[nodiscard]] Texture LoadShadedWrapText(std::string_view text, int wrapLen = 0,
		 SDL_Color bg = SDL_Color{ .r=0u, .g=0u, .b=0u, .a=defaultAlphaInt },
		 SDL_Color fg = SDL_Color{ .r=defaultAlphaInt, .g=defaultAlphaInt, .b=defaultAlphaInt, .a=defaultAlphaInt }) const;

		[[nodiscard]] Texture LoadLCDWrapText(std::string_view text, int wrapLen = 0,
		 SDL_Color bg = SDL_Color{ .r=0u, .g=0u, .b=0u, .a=defaultAlphaInt },
		 SDL_Color fg = SDL_Color{ .r=defaultAlphaInt, .g=defaultAlphaInt, .b=defaultAlphaInt, .a=defaultAlphaInt }) const;

		/**
		 * @brief Used to draw arbitrary shapes with raw vertex information. Great for making draw calls from
		 *			external sources, like GUI libraries.
		 * 
		 * @param texture 
		 * @param vertices 
		 * @param indices 
		 */
		void DrawGeometry(Texture texture, const std::span<SDL_Vertex> vertices, const std::span<int> indices) const {
			SDL_RenderGeometry(_render, *texture, vertices.data(), static_cast<int>(std::ssize(vertices)),
									indices.data(), static_cast<int>(std::ssize(indices)));
		}

		[[nodiscard]] Texture LoadTextureImg(const std::filesystem::path& img, SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND) const;
		[[nodiscard]] Texture CreateRenderableTexture(int width, int height, SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA32, SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND) const;
		[[nodiscard]] Texture CreateTextureFromSurface(const Surface &surface) const;

		[[nodiscard]] SDL_FColor GetDrawColor() const {
			SDL_FColor res{};
			SDL_GetRenderDrawColorFloat(_render, &res.r, &res.g, &res.b, &res.a);

			return res;
		}

		bool SetDrawTarget(Texture texture) const { return SDL_SetRenderTarget(_render, *texture); }

		[[nodiscard]] static auto Create() noexcept { return std::make_shared<Simple2DRenderer>(); }

#ifdef SWGTK_BUILD_WITH_LUA
		void InitLua(sol::state* lua_) override;
#endif

	private:
		SDL_Renderer* _render = nullptr;
		TTF_Font* _currentFont = nullptr;
    };
} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_SIMPLE2DRENDERER_HPP_  
