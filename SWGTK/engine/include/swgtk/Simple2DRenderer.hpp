#ifndef SWGTK_SIMPLERENDERER_HPP
#define SWGTK_SIMPLERENDERER_HPP

#include "swgtk/Texture.hpp"

#include "SDL3_ttf/SDL_ttf.h"
#include "SDL3/SDL_blendmode.h"
#include "SDL3/SDL_render.h"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_oldnames.h>
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

namespace swgtk
{
    class Simple2DRenderer : public RendererBase, std::enable_shared_from_this<Simple2DRenderer>{
	public:

		constexpr Simple2DRenderer() = default;
		Simple2DRenderer(const Simple2DRenderer &) = delete;
		Simple2DRenderer(Simple2DRenderer &&) = delete;
		Simple2DRenderer &operator=(const Simple2DRenderer &) = delete;
		Simple2DRenderer &operator=(Simple2DRenderer &&) = delete;
		constexpr ~Simple2DRenderer() override { DestroyDevice(); }

		void BufferClear(SDL_FColor color = SDL_FColor{ .r=0.0f, .g=0.0f, .b=0.0f, .a=1.0f}) override;
		void BufferPresent() override;

		constexpr void SetBackgroundColor(const SDL_FColor& color) override { SetDrawColor(color); }
		[[nodiscard]] constexpr bool IsDeviceInitialized() const override { return _render != nullptr; }

		[[nodiscard]] bool PrepareDevice(SDL_Window* window) override;
		void DestroyDevice() override;

		[[nodiscard]] constexpr std::shared_ptr<RendererBase> GetRef() override { return shared_from_this(); }

		constexpr bool SetDrawColor(float r, float g, float b, float a = RendererBase::defaultAlphaFloat) const { return SDL_SetRenderDrawColorFloat(_render, r, g, b, a); }
        constexpr bool SetDrawColor(SDL_FColor color = SDL_FColor{.r=0.0f, .g=0.0f, .b=0.0f, .a=defaultAlphaFloat}) const { 
			return SDL_SetRenderDrawColorFloat(_render, color.r, color.g, color.b, color.a); 
		}

		void DrawTexture(SDL_Texture* texture, const std::optional<SDL_FRect>& src = std::nullopt, const std::optional<SDL_FRect>& dest = std::nullopt) const;		
		void DrawTexture(SDL_Texture* texture, const std::optional<SDL_FRect>& src, const std::optional<SDL_FRect>& dest, double angle, const std::optional<SDL_FPoint>& center = std::nullopt, SDL_FlipMode flip = SDL_FLIP_NONE) const;
		
		/**
		 * @brief Draw text at the specified location with the specified font. Uses SDL_ttf's fastest algorithm.
		 * 
		 * @param text 
		 * @param font 
		 * @param pos - Destination rectangle
		 * @param color
		 */
		void DrawPlainText(std::string_view text, TTF_Font* font, SDL_FRect pos,
		 SDL_Color color = SDL_Color{ .r=defaultAlphaInt, .g=defaultAlphaInt, .b=defaultAlphaInt, .a=defaultAlphaInt }) const;

		/*
			Combines SDL_ttf's API with SDL_Textures to preload text renderables as Textures. These can be rotated and tinted as needed.
		*/

		[[nodiscard]] Texture LoadPlainText(std::string_view text, TTF_Font* font,
		 SDL_Color color = SDL_Color{ .r=defaultAlphaInt, .g=defaultAlphaInt, .b=defaultAlphaInt, .a=defaultAlphaInt }) const;

		[[nodiscard]] Texture LoadBlendedText(std::string_view text, TTF_Font* font,
		 SDL_Color color = SDL_Color{ .r=defaultAlphaInt, .g=defaultAlphaInt, .b=defaultAlphaInt, .a=defaultAlphaInt }) const;

		[[nodiscard]] Texture LoadShadedText(std::string_view text, TTF_Font* font,
		 SDL_Color bg = SDL_Color{ .r=0u, .g=0u, .b=0u, .a=defaultAlphaInt },
		 SDL_Color fg = SDL_Color{ .r=defaultAlphaInt, .g=defaultAlphaInt, .b=defaultAlphaInt, .a=defaultAlphaInt }) const;

		[[nodiscard]] Texture LoadLCDText(std::string_view text, TTF_Font* font,
		 SDL_Color bg = SDL_Color{ .r=0u, .g=0u, .b=0u, .a=defaultAlphaInt },
		 SDL_Color fg = SDL_Color{ .r=defaultAlphaInt, .g=defaultAlphaInt, .b=defaultAlphaInt, .a=defaultAlphaInt }) const;

		[[nodiscard]] Texture LoadPlainWrapText(std::string_view text, TTF_Font* font, int wrapLen = 0,
		 SDL_Color color = SDL_Color{ .r=defaultAlphaInt, .g=defaultAlphaInt, .b=defaultAlphaInt, .a=defaultAlphaInt }) const;

		[[nodiscard]] Texture LoadBlendedWrapText(std::string_view text, TTF_Font* font, int wrapLen = 0,
		 SDL_Color color = SDL_Color{ .r=defaultAlphaInt, .g=defaultAlphaInt, .b=defaultAlphaInt, .a=defaultAlphaInt }) const;

		[[nodiscard]] Texture LoadShadedWrapText(std::string_view text, TTF_Font* font, int wrapLen = 0,
		 SDL_Color bg = SDL_Color{ .r=0u, .g=0u, .b=0u, .a=defaultAlphaInt },
		 SDL_Color fg = SDL_Color{ .r=defaultAlphaInt, .g=defaultAlphaInt, .b=defaultAlphaInt, .a=defaultAlphaInt }) const;

		[[nodiscard]] Texture LoadLCDWrapText(std::string_view text, TTF_Font* font, int wrapLen = 0,
		 SDL_Color bg = SDL_Color{ .r=0u, .g=0u, .b=0u, .a=defaultAlphaInt },
		 SDL_Color fg = SDL_Color{ .r=defaultAlphaInt, .g=defaultAlphaInt, .b=defaultAlphaInt, .a=defaultAlphaInt }) const;

		/*
			Used to draw arbitray shapes from a list of verticies. This is helpful when drawing things like particles.

			It's okay to allow for nullptr in 'texture' because SDL handles it for us.
		*/ 
		constexpr void DrawGeometry(SDL_Texture* texture, std::span<SDL_Vertex> verticies, std::span<int> indicies) const {
			SDL_RenderGeometry(_render, texture, verticies.data(), static_cast<int>(std::ssize(verticies)),
									indicies.data(), static_cast<int>(std::ssize(indicies)));
		}

		[[nodiscard]] Texture LoadTextureImg(const std::filesystem::path& img, SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND) const;
		[[nodiscard]] Texture CreateRenderableTexture(int width, int height, SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA32, SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND) const;
		[[nodiscard]] Texture CreateTextureFromSurface(Surface surface) const;

		[[nodiscard]] constexpr SDL_FColor GetDrawColor() const {
			SDL_FColor res;
			SDL_GetRenderDrawColorFloat(_render, &res.r, &res.g, &res.b, &res.a);

			return res;
		}

		constexpr bool SetDrawTarget(SDL_Texture* ptr = nullptr) const { return SDL_SetRenderTarget(_render, ptr); }

		[[nodiscard]] constexpr static auto Create() { return std::make_shared<Simple2DRenderer>(); }

		void InitLua(sol::state& lua);

	private:
		SDL_Renderer* _render = nullptr;
    };
}

#endif
