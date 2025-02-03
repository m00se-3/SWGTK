#include "swgtk/Simple2DRenderer.hpp"

#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "swgtk/RendererBase.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <filesystem>
#include <sol/optional_implementation.hpp>
#include <string_view>

namespace swgtk
{
	bool Simple2DRenderer::PrepareDevice(SDL_Window* window) {
		_render = SDL_CreateRenderer(window, nullptr);

		return _render != nullptr;
	}

	void Simple2DRenderer::DestroyDevice() {
		SDL_DestroyRenderer(_render);
	}

	void Simple2DRenderer::BufferClear(SDL_FColor color) {
		auto tmpColor = GetDrawColor();
		
		SetDrawColor(color);
		SDL_RenderClear(_render);
		SetDrawColor(tmpColor);
	}

	void Simple2DRenderer::BufferPresent() {
		SDL_SetRenderTarget(_render, nullptr);
		SDL_RenderPresent(_render);
	}
	
	Texture Simple2DRenderer::LoadTextureImg(const std::filesystem::path& img, SDL_BlendMode blendMode) const {
	    if(std::filesystem::exists(img)) {
			auto* texture = IMG_LoadTexture(_render, img.string().c_str());
			
			if(texture != nullptr) {
				SDL_SetTextureBlendMode(texture, blendMode);
				return Texture{ texture };
			}

			DEBUG_PRINT2("Failed to load image {}: {}\n", img.c_str(), SDL_GetError())
	    }

	    return Texture{};
	}

	Texture Simple2DRenderer::CreateRenderableTexture(int width, int height, SDL_PixelFormat format, SDL_BlendMode blendMode) const {
		if( width < 0 || height < 0) {
			DEBUG_PRINT2("Invalid texture dimensions: {}, {}\n", width, height)
			return Texture{};
		}

		auto* texture = SDL_CreateTexture(_render, format, SDL_TEXTUREACCESS_TARGET, width, height);

		if(texture == nullptr) {
			DEBUG_PRINT("Error creating renderable texture: {}\n", SDL_GetError())
			return Texture{};
		}

		SDL_SetTextureBlendMode(texture, blendMode);
		return Texture{ texture };
	}

	Texture Simple2DRenderer::CreateTextureFromSurface(Surface surface) const {
		auto* texture = SDL_CreateTextureFromSurface(_render, *surface);

		if(texture == nullptr) {
			DEBUG_PRINT("Failed to create texture: {}\n", SDL_GetError())
			return Texture{};
		}

		return Texture{ texture };
	}

	void Simple2DRenderer::DrawTexture(SDL_Texture* texture, const std::optional<SDL_FRect>& src, const std::optional<SDL_FRect>& dest) const {
		const auto* source = src ? &src.value() : nullptr;
		const auto* destination = dest ? &dest.value() : nullptr;
		
		SDL_RenderTexture(_render, texture, source, destination);
	}

	void Simple2DRenderer::DrawTexture(SDL_Texture* texture, const std::optional<SDL_FRect>& src, const std::optional<SDL_FRect>& dest, double angle, const std::optional<SDL_FPoint>& center, SDL_FlipMode flip) const{
		const auto* source = src ? &src.value() : nullptr;
		const auto* destination = dest ? &dest.value() : nullptr;
		const auto* cen = center ? &center.value() : nullptr;
		
		SDL_RenderTextureRotated(_render, texture, source, destination, angle, cen, flip);
	}

	void Simple2DRenderer::DrawPlainText(std::string_view text, TTF_Font* font, SDL_FRect pos, SDL_Color color) const {		
		auto* ttf = TTF_RenderText_Solid(font, text.data(), text.size(), color);

		if(ttf == nullptr) {
			DEBUG_PRINT("{}", SDL_GetError())
			return;
		}

		auto* texture = SDL_CreateTextureFromSurface(_render, ttf);
		SDL_RenderTexture(_render, texture, nullptr, &pos);
		SDL_DestroySurface(ttf);
	}

	Texture Simple2DRenderer::LoadPlainText(std::string_view text, TTF_Font* font, SDL_Color color) const {
		auto* surf = TTF_RenderText_Solid(font, text.data(), text.size(), color);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadBlendedText(std::string_view text, TTF_Font* font, SDL_Color color) const {
		auto* surf = TTF_RenderText_Blended(font, text.data(), text.size(), color);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadShadedText(std::string_view text, TTF_Font* font, SDL_Color bg, SDL_Color fg) const {
		auto* surf = TTF_RenderText_Shaded(font, text.data(), text.size(), fg, bg);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadLCDText(std::string_view text, TTF_Font* font, SDL_Color bg, SDL_Color fg) const {
		auto* surf = TTF_RenderText_LCD(font, text.data(), text.size(), fg, bg);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadPlainWrapText(std::string_view text, TTF_Font* font, int wrapLen, SDL_Color color) const {
		auto* surf = TTF_RenderText_Solid_Wrapped(font, text.data(), text.size(), color, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadBlendedWrapText(std::string_view text, TTF_Font* font, int wrapLen, SDL_Color color) const {
		auto* surf = TTF_RenderText_Blended_Wrapped(font, text.data(), text.size(), color, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadShadedWrapText(std::string_view text, TTF_Font* font, int wrapLen, SDL_Color bg, SDL_Color fg) const {
		auto* surf = TTF_RenderText_Shaded_Wrapped(font, text.data(), text.size(), fg, bg, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadLCDWrapText(std::string_view text, TTF_Font* font, int wrapLen, SDL_Color bg, SDL_Color fg) const {
		auto* surf = TTF_RenderText_LCD_Wrapped(font, text.data(), text.size(), fg, bg, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	void Simple2DRenderer::InitLua(sol::state& lua) {
		
		auto vertex = lua.new_usertype<SDL_Vertex>(
		"Vertex2D", "position", &SDL_Vertex::position, "color", &SDL_Vertex::color, "tex_coord", &SDL_Vertex::tex_coord
		);

		vertex["new"] = [](sol::optional<SDL_FPoint> pos, sol::optional<SDL_FColor> col, sol::optional<SDL_FPoint> tex) -> SDL_Vertex {
				return SDL_Vertex{ 
					.position=pos.value_or(SDL_FPoint{}), 
					.color=col.value_or(SDL_FColor{}), 
					.tex_coord=tex.value_or(SDL_FPoint{}) };
			};

		lua["SetDrawColor"] = [this](sol::optional<float> r, sol::optional<float> g, sol::optional<float> b, sol::optional<float> a) {
				return SetDrawColor(
					r.value_or(RendererBase::defaultAlphaFloat), 
					g.value_or(RendererBase::defaultAlphaFloat), 
					b.value_or(RendererBase::defaultAlphaFloat), 
					a.value_or(RendererBase::defaultAlphaFloat)
					);
			};

		lua["DrawTexture"] = [this](SDL_Texture* texture, sol::optional<SDL_FRect> src, sol::optional<SDL_FRect> dest, sol::optional<double> angle, sol::optional<SDL_FPoint> center, sol::optional<SDL_FlipMode> flip) {
				DrawTexture(texture, *src, *dest, angle.value_or(0.0), *center, flip.value_or(SDL_FLIP_NONE));
			};
	}
}
