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
#include <swgtk/RendererBase.hpp>
#include <swgtk/Utility.hpp>
#include <sol/sol.hpp>

#include <memory>
#include <filesystem>
#include <string_view>
#include <span>
#include <optional>

namespace swgtk {
    class Simple2DRenderer : public RendererBase, public std::enable_shared_from_this<Simple2DRenderer>{
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

		[[nodiscard]] bool PrepareDevice(SDL_Window* window) override;
		void DestroyDevice() override;

		[[nodiscard]] std::shared_ptr<RendererBase> GetRef() override { return shared_from_this(); }

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
			Used to draw arbitrary shapes from a list of vertices. This is helpful when drawing things like particles.

			It's okay to allow for nullptr in 'texture' because SDL handles it for us.
		*/ 
		void DrawGeometry(SDL_Texture* texture, const std::span<SDL_Vertex> vertices, const std::span<int> indices) const {
			SDL_RenderGeometry(_render, texture, vertices.data(), static_cast<int>(std::ssize(vertices)),
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

		bool SetDrawTarget(SDL_Texture* ptr = nullptr) const { return SDL_SetRenderTarget(_render, ptr); }

		[[nodiscard]] static auto Create() noexcept { return std::make_shared<Simple2DRenderer>(); }

		void InitLua(sol::state& lua);

	private:
		SDL_Renderer* _render = nullptr;
		TTF_Font* _currentFont = nullptr;
    };
} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_SIMPLE2DRENDERER_HPP_  
