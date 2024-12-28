#ifndef SWGTK_SIMPLERENDERER_HPP
#define SWGTK_SIMPLERENDERER_HPP

#include "swgtk/Texture.hpp"

#include "SDL3_ttf/SDL_ttf.h"
#include "SDL3/SDL_blendmode.h"
#include "SDL3/SDL_render.h"
#include <SDL3/SDL_error.h>
#include <swgtk/RendererBase.hpp>
#include <swgtk/Macros.hpp>
#include <sol/sol.hpp>

#include <filesystem>
#include <string_view>
#include <span>

namespace swgtk
{
    class Simple2DRenderer : public RendererBase {
	public:
		Simple2DRenderer() = default;
		Simple2DRenderer(const Simple2DRenderer &) = delete;
		Simple2DRenderer(Simple2DRenderer &&) = delete;
		Simple2DRenderer &operator=(const Simple2DRenderer &) = delete;
		Simple2DRenderer &operator=(Simple2DRenderer &&) = delete;
		~Simple2DRenderer();

		constexpr void BufferClear() override { SDL_RenderClear(_render); };
		constexpr void BufferPresent() override { SDL_RenderPresent(_render); };
		constexpr void SetBackgroundColor(const SDL_FColor& color) override { SetDrawColor(color); }
		[[nodiscard]] constexpr bool IsDeviceInitialized() const override { return _render != nullptr && _textEngine != nullptr; }

		[[nodiscard]] bool PrepareDevice(SDL_Window* window, SDL_FColor bgColor = SDL_FColor{.r=0.0f, .g=0.0f, .b=0.0f, .a=defaultAlphaFloat}) override;
		void DestroyDevice() override;

		constexpr bool SetDrawColor(float r, float g, float b, float a = RendererBase::defaultAlphaFloat) { return SDL_SetRenderDrawColorFloat(_render, r, g, b, a); }
        constexpr bool SetDrawColor(SDL_FColor color = SDL_FColor{.r=0.0f, .g=0.0f, .b=0.0f, .a=defaultAlphaFloat}) { 
			return SDL_SetRenderDrawColorFloat(_render, color.r, color.g, color.b, color.a); 
		}

		/*
			It is okay to allow nullptr for the rectangles, since SDL handles nullptr for us.
		*/
		constexpr void DrawTexture(SDL_Texture* texture, SDL_FRect* src, SDL_FRect* dest) {
			SDL_RenderTexture(_render, texture, src, dest);
		}

		/*
			It is okay to allow nullptr for the source, destination, and center, since SDL handles nullptr for us.
		*/
		constexpr void DrawTexture(SDL_Texture* texture, SDL_FRect* src, SDL_FRect* dest, double angle, SDL_FPoint* center = nullptr, SDL_FlipMode flip = SDL_FLIP_NONE) {
			SDL_RenderTextureRotated(_render, texture, src, dest, angle, center, flip);
		}
		
		/**
		 * @brief Draw unscaled text at the specified location with the specified font.
		 * 
		 * @param text 
		 * @param font 
		 * @param posX 
		 * @param posY 
		 */
		constexpr void DrawText(std::string_view text, TTF_Font* font, float posX, float posY) {		
			auto* ttf = TTF_CreateText(_textEngine, font, text.data(), text.size());

			if(ttf == nullptr) {
				DEBUG_PRINT(SDL_GetError());
				return;
			}

			auto color = GetDrawColor();
			TTF_SetTextColorFloat(ttf, color.r, color.g, color.b, color.a);

			TTF_DrawRendererText(ttf, posX, posY);
			TTF_DestroyText(ttf);
		}						

		/*
			Used to draw arbitray shapes from a list of verticies. This is helpful when drawing things like particles.

			It's okay to allow for nullptr in 'texture' because SDL handles it for us.
		*/ 
		constexpr void DrawGeometry(SDL_Texture* texture, std::span<SDL_Vertex> verticies, std::span<int> indicies) {
			SDL_RenderGeometry(_render, texture, verticies.data(), static_cast<int>(std::ssize(verticies)),
									indicies.data(), static_cast<int>(std::ssize(indicies)));
		}

		[[nodiscard]] Texture LoadTextureImg(const std::filesystem::path& img, SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND);

		[[nodiscard]] constexpr SDL_FColor GetDrawColor() const {
			SDL_FColor res;
			SDL_GetRenderDrawColorFloat(_render, &res.r, &res.g, &res.b, &res.a);

			return res;
		}

		constexpr bool SetDrawTarget(SDL_Texture* ptr = nullptr) { return SDL_SetRenderTarget(_render, ptr); }

		void InitLua(sol::state& lua);

	private:
		SDL_Renderer* _render = nullptr;
		TTF_TextEngine* _textEngine = nullptr;
    };
}

#endif
