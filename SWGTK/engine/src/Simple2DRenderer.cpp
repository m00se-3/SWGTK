#include "swgtk/Simple2DRenderer.hpp"

#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "swgtk/RendererBase.hpp"
#include <SDL3/SDL_error.h>
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

	void Simple2DRenderer::BufferClear(const SDL_FColor& color) {
		const auto tmpColor = GetDrawColor();
		
		SetDrawColor(color);
		SDL_RenderClear(_render);
		SetDrawColor(tmpColor);
	}

	void Simple2DRenderer::BufferPresent() {
		SDL_SetRenderTarget(_render, nullptr);
		SDL_RenderPresent(_render);
	}
	
	Texture Simple2DRenderer::LoadTextureImg(const std::filesystem::path& img, const SDL_BlendMode blendMode) const {
	    if(std::filesystem::exists(img)) {

			if(auto* texture = IMG_LoadTexture(_render, img.string().c_str()); texture != nullptr) {
				SDL_SetTextureBlendMode(texture, blendMode);
				return Texture{ texture };
			}

			DEBUG_PRINT2("Failed to load image {}: {}\n", img.string().c_str(), SDL_GetError())
	    }

	    return Texture{};
	}

	Texture Simple2DRenderer::CreateRenderableTexture(int width, int height, const SDL_PixelFormat format, const SDL_BlendMode blendMode) const {
		if( width < 1 || height < 1) {
			DEBUG_PRINT2("Invalid texture dimensions: {}, {}\n", width, height)
			return Texture{};
		}

		if(auto* texture = SDL_CreateTexture(_render, format, SDL_TEXTUREACCESS_TARGET, width, height); texture != nullptr) {
			SDL_SetTextureBlendMode(texture, blendMode);
			return Texture{ texture };
		}

		DEBUG_PRINT("Error creating renderable texture: {}\n", SDL_GetError())
		return Texture{};
	}

	Texture Simple2DRenderer::CreateTextureFromSurface(const Surface &surface) const {
		if(auto* texture = SDL_CreateTextureFromSurface(_render, *surface)) {
			return Texture{ texture };
		}

		DEBUG_PRINT("Failed to create texture: {}\n", SDL_GetError())
		return Texture{};
	}

	void Simple2DRenderer::DrawTexture(SDL_Texture* texture, const std::optional<SDL_FRect>& src, const std::optional<SDL_FRect>& dest) const {
		const auto* source = src ? &src.value() : nullptr;
		const auto* destination = dest ? &dest.value() : nullptr;
		
		SDL_RenderTexture(_render, texture, source, destination);
	}

	auto Simple2DRenderer::DrawTexture(SDL_Texture *texture, const std::optional<SDL_FRect> &src,
	                                   const std::optional<SDL_FRect> &dest, const double angle,
	                                   const std::optional<SDL_FPoint> &center, const SDL_FlipMode flip) const -> void {
		const auto* source = src ? &src.value() : nullptr;
		const auto* destination = dest ? &dest.value() : nullptr;
		const auto* cen = center ? &center.value() : nullptr;
		
		SDL_RenderTextureRotated(_render, texture, source, destination, angle, cen, flip);
	}

	void Simple2DRenderer::DrawPlainText(const std::string_view text, TTF_Font* font, const SDL_FRect& pos, const SDL_Color& color) const {

		if(auto* ttf = TTF_RenderText_Solid(font, text.data(), text.size(), color); ttf != nullptr) {
			auto* texture = SDL_CreateTextureFromSurface(_render, ttf);
			SDL_RenderTexture(_render, texture, nullptr, &pos);
			SDL_DestroySurface(ttf);
		} else {
			DEBUG_PRINT("{}", SDL_GetError())
		}
	}

	Texture Simple2DRenderer::LoadPlainText(const std::string_view text, TTF_Font* font, const SDL_Color& color) const {
		auto* surf = TTF_RenderText_Solid(font, text.data(), text.size(), color);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadBlendedText(const std::string_view text, TTF_Font* font, const SDL_Color& color) const {
		auto* surf = TTF_RenderText_Blended(font, text.data(), text.size(), color);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadShadedText(const std::string_view text, TTF_Font* font, const SDL_Color& bg, const SDL_Color& fg) const {
		auto* surf = TTF_RenderText_Shaded(font, text.data(), text.size(), fg, bg);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadLCDText(const std::string_view text, TTF_Font* font, const SDL_Color& bg, const SDL_Color& fg) const {
		auto* surf = TTF_RenderText_LCD(font, text.data(), text.size(), fg, bg);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadPlainWrapText(const std::string_view text, TTF_Font* font, const int wrapLen, const SDL_Color& color) const {
		auto* surf = TTF_RenderText_Solid_Wrapped(font, text.data(), text.size(), color, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadBlendedWrapText(const std::string_view text, TTF_Font* font, const int wrapLen, const SDL_Color& color) const {
		auto* surf = TTF_RenderText_Blended_Wrapped(font, text.data(), text.size(), color, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadShadedWrapText(const std::string_view text, TTF_Font* font, const int wrapLen, const SDL_Color bg, const SDL_Color fg) const {
		auto* surf = TTF_RenderText_Shaded_Wrapped(font, text.data(), text.size(), fg, bg, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadLCDWrapText(const std::string_view text, TTF_Font* font, const int wrapLen, const SDL_Color bg, const SDL_Color fg) const {
		auto* surf = TTF_RenderText_LCD_Wrapped(font, text.data(), text.size(), fg, bg, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	void Simple2DRenderer::InitLua(sol::state& lua) {
		
		// Type definitions

		// SDL_Vertex

		auto vertex = lua.new_usertype<SDL_Vertex>(
		"Vertex2D", "position", &SDL_Vertex::position, "color", &SDL_Vertex::color, "tex_coord", &SDL_Vertex::tex_coord
		);

		vertex["new"] = [](const sol::optional<SDL_FPoint> pos, const sol::optional<SDL_FColor> &col, const sol::optional<SDL_FPoint> tex) -> SDL_Vertex {
				return SDL_Vertex{ 
					.position=pos.value_or(SDL_FPoint{}), 
					.color=col.value_or(SDL_FColor{}), 
					.tex_coord=tex.value_or(SDL_FPoint{}) };
			};

		// swgtk::Texture

		auto texture = lua.new_usertype<Texture>("Texture", sol::constructors<Texture(), Texture(SDL_Texture*)>());

		texture["get"] = [](Texture& self) -> SDL_Texture* { return *self; };

		texture["SetBlendMode"] = &Texture::SetBlendMode;

		texture["SetTint"] = [](Texture& self, const float r, const float g, const float b, const sol::optional<float> a) {
				self.SetTint(r, g, b, a.value_or(1.0f));
			};

		texture["SetScaleMode"] = &Texture::SetScaleMode;

		texture["GetBlendMode"] = &Texture::GetBlendMode;

		texture["GetTint"] = &Texture::GetTint;

		texture["GetScaleMode"] = &Texture::GetScaleMode;

		texture["GetSize"] = &Texture::GetSize;

		// Renderer function definitions

		lua["SetDrawColor"] = [this](const sol::optional<float> r, const sol::optional<float> g,
		                             const sol::optional<float> b, const sol::optional<float> a) {
				SetDrawColor(
					r.value_or(RendererBase::defaultAlphaFloat), 
					g.value_or(RendererBase::defaultAlphaFloat), 
					b.value_or(RendererBase::defaultAlphaFloat), 
					a.value_or(RendererBase::defaultAlphaFloat)
					);
			};

		lua["SetDrawTarget"] = [this](const sol::optional<SDL_Texture*> target) { SetDrawTarget(target.value_or(nullptr)); };

		lua["DrawTexture"] = [this](SDL_Texture *texture, const sol::optional<SDL_FRect> &src,
		                            const sol::optional<SDL_FRect> &dest) {
			DrawTexture(texture, std::optional<SDL_FRect>{std::in_place_t{}, *src}, std::optional<SDL_FRect>{std::in_place_t{}, *dest});
		};

		lua["DrawTextureRotated"] = [this](SDL_Texture *texture, sol::optional<SDL_FRect> src, sol::optional<SDL_FRect> dest,
		                            const sol::optional<double> angle, sol::optional<SDL_FPoint> center,
		                            const sol::optional<SDL_FlipMode> flip) {
				DrawTexture(texture, std::optional<SDL_FRect>{std::in_place_t{}, *src}, std::optional<SDL_FRect>{std::in_place_t{}, *dest},
					angle.value_or(0.0), std::optional<SDL_FPoint>{std::in_place_t{}, *center}, flip.value_or(SDL_FLIP_NONE));
			};

		lua["LoadTextureImg"] = [this] (const std::filesystem::path& img, const sol::optional<SDL_BlendMode> blendMode) -> Texture {
			return LoadTextureImg(img, blendMode.value_or(SDL_BLENDMODE_BLEND));
		};

		lua["CreateRenderableTexture"] = [this] (const int width, const int height, const sol::optional<SDL_PixelFormat> format,
		                             const sol::optional<SDL_BlendMode> blendMode) -> Texture {
			return CreateRenderableTexture(width, height, format.value_or(SDL_PIXELFORMAT_RGBA32), blendMode.value_or(SDL_BLENDMODE_BLEND));
		};

		lua["CreateTextureFromSurface"] = [this] (const Surface &surface) -> Texture {
			return CreateTextureFromSurface(surface);
		};

		lua["GetDrawColor"] = [this] () -> SDL_FColor {
			return GetDrawColor();
		};

		lua["DrawGeometry"] = [this] (SDL_Texture* texture, const std::span<SDL_Vertex> vertices, const std::span<int> indices) {
			DrawGeometry(texture, vertices, indices);
		};
	}
}
