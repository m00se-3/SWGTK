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

namespace swgtk {
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
	    	const auto imgStr = img.string();

			if(auto* texture = IMG_LoadTexture(_render, imgStr.c_str()); texture != nullptr) {
				SDL_SetTextureBlendMode(texture, blendMode);
				return Texture{ texture };
			}

			DEBUG_PRINT2("Failed to load image {}: {}\n", imgStr.c_str(), SDL_GetError())
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

	void Simple2DRenderer::DrawTexture(Texture texture, const std::optional<SDL_FRect>& src, const std::optional<SDL_FRect>& dest) const {
		const auto* source = src ? &src.value() : nullptr;
		const auto* destination = dest ? &dest.value() : nullptr;
		
		SDL_RenderTexture(_render, *texture, source, destination);
	}

	auto Simple2DRenderer::DrawTexture(Texture texture, const std::optional<SDL_FRect> &src,
	                                   const std::optional<SDL_FRect> &dest, const double angle,
	                                   const std::optional<SDL_FPoint> &center, const SDL_FlipMode flip) const -> void {
		const auto* source = src ? &src.value() : nullptr;
		const auto* destination = dest ? &dest.value() : nullptr;
		const auto* cen = center ? &center.value() : nullptr;
		
		SDL_RenderTextureRotated(_render, *texture, source, destination, angle, cen, flip);
	}

	void Simple2DRenderer::DrawPlainText(const std::string_view text, const SDL_FRect& pos, const SDL_Color& color) const {

		if(auto* ttf = TTF_RenderText_Solid(_currentFont, text.data(), text.size(), color); ttf != nullptr) {
			auto* texture = SDL_CreateTextureFromSurface(_render, ttf);
			SDL_RenderTexture(_render, texture, nullptr, &pos);
			SDL_DestroySurface(ttf);
		} else {
			DEBUG_PRINT("{}", SDL_GetError())
		}
	}

	void Simple2DRenderer::DrawPlainWrapText(const std::string_view text, const SDL_FRect& pos, const int wrapLen, const SDL_Color &color) const {
		if(auto* ttf = TTF_RenderText_Solid_Wrapped(_currentFont, text.data(), text.size(), color, wrapLen); ttf != nullptr) {
			auto* texture = SDL_CreateTextureFromSurface(_render, ttf);
			SDL_RenderTexture(_render, texture, nullptr, &pos);
			SDL_DestroySurface(ttf);
		} else {
			DEBUG_PRINT("{}", SDL_GetError())
		}
	}

	Texture Simple2DRenderer::LoadPlainText(const std::string_view text, const SDL_Color& color) const {
		auto* surf = TTF_RenderText_Solid(_currentFont, text.data(), text.size(), color);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadBlendedText(const std::string_view text, const SDL_Color& color) const {
		auto* surf = TTF_RenderText_Blended(_currentFont, text.data(), text.size(), color);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadShadedText(const std::string_view text, const SDL_Color& bg, const SDL_Color& fg) const {
		auto* surf = TTF_RenderText_Shaded(_currentFont, text.data(), text.size(), fg, bg);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadLCDText(const std::string_view text, const SDL_Color& bg, const SDL_Color& fg) const {
		auto* surf = TTF_RenderText_LCD(_currentFont, text.data(), text.size(), fg, bg);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadPlainWrapText(const std::string_view text, const int wrapLen, const SDL_Color& color) const {
		auto* surf = TTF_RenderText_Solid_Wrapped(_currentFont, text.data(), text.size(), color, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadBlendedWrapText(const std::string_view text, const int wrapLen, const SDL_Color& color) const {
		auto* surf = TTF_RenderText_Blended_Wrapped(_currentFont, text.data(), text.size(), color, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadShadedWrapText(const std::string_view text, const int wrapLen, const SDL_Color bg, const SDL_Color fg) const {
		auto* surf = TTF_RenderText_Shaded_Wrapped(_currentFont, text.data(), text.size(), fg, bg, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture Simple2DRenderer::LoadLCDWrapText(const std::string_view text, const int wrapLen, const SDL_Color bg, const SDL_Color fg) const {
		auto* surf = TTF_RenderText_LCD_Wrapped(_currentFont, text.data(), text.size(), fg, bg, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	void Simple2DRenderer::InitLua(sol::state& lua) {
		
		auto SDL_Vertex_Type = lua.new_usertype<SDL_Vertex>(
		"Vertex2D", "position", &SDL_Vertex::position, "color", &SDL_Vertex::color, "tex_coord", &SDL_Vertex::tex_coord
		);

		SDL_Vertex_Type["new"] = [](const sol::optional<SDL_FPoint> pos, const sol::optional<SDL_FColor> &col, const sol::optional<SDL_FPoint> tex) -> SDL_Vertex {
				return SDL_Vertex{ 
					.position=pos.value_or(SDL_FPoint{}), 
					.color=col.value_or(SDL_FColor{}), 
					.tex_coord=tex.value_or(SDL_FPoint{}) };
			};

		auto SDL_Color_Type = lua.new_usertype<SDL_Color>(
			"Colori", "r", &SDL_Color::r, "g", &SDL_Color::g, "b", &SDL_Color::b, "a", &SDL_Color::a
			);

		// NOLINTBEGIN(bugprone-easily-swappable-parameters)

		SDL_Color_Type["new"] = [] (const uint8_t r, const uint8_t g, const uint8_t b, const sol::optional<uint8_t> a) -> SDL_Color {
			static constexpr auto fullAplha = 255u;
			return SDL_Color{ .r=r, .g=g, .b=b, .a=a.value_or(fullAplha) };
		};

		auto SDL_FColor_Type = lua.new_usertype<SDL_FColor>(
			"Colorf", "r", &SDL_FColor::r, "g", &SDL_FColor::g, "b", &SDL_FColor::b, "a", &SDL_FColor::a
			);

		SDL_FColor_Type["new"] = [] (const float r, const float g, const float b, const sol::optional<float> a) -> SDL_FColor {
			return SDL_FColor{ .r=r, .g=g, .b=b, .a=a.value_or(1.0f) };
		};

		lua.new_enum<SDL_BlendMode>("BlendMode",
			{
				std::make_pair("None", SDL_BLENDMODE_NONE),
				std::make_pair("Add", SDL_BLENDMODE_ADD),
				std::make_pair("Blend", SDL_BLENDMODE_BLEND),
				std::make_pair("PreBlend", SDL_BLENDMODE_BLEND_PREMULTIPLIED),
				std::make_pair("PreAdd", SDL_BLENDMODE_ADD_PREMULTIPLIED),
				std::make_pair("Mod", SDL_BLENDMODE_MOD),
				std::make_pair("Mul",	SDL_BLENDMODE_MUL),
				std::make_pair("Invalid", SDL_BLENDMODE_INVALID),
			});

		lua.new_enum<SDL_PixelFormat>("PixelFormat",
			{
				std::make_pair("RGBA32", SDL_PIXELFORMAT_RGBA32),
				std::make_pair("ARGB32", SDL_PIXELFORMAT_ARGB32),
				std::make_pair("BGRA32", SDL_PIXELFORMAT_BGRA32),
				std::make_pair("ABGR32", SDL_PIXELFORMAT_ABGR32),
				std::make_pair("RGBX32", SDL_PIXELFORMAT_RGBX32),
				std::make_pair("XRGB32", SDL_PIXELFORMAT_XRGB32),
				std::make_pair("BGRX32", SDL_PIXELFORMAT_BGRX32),
				std::make_pair("XBGR32", SDL_PIXELFORMAT_XBGR32),
			});

		auto Texture_Type = lua.new_usertype<Texture>("Texture", sol::constructors<Texture(), Texture(SDL_Texture*), Texture(const Texture&)>());

		Texture_Type["SetBlendMode"] = &Texture::SetBlendMode;

		Texture_Type["SetTint"] = [](const Texture& self, const sol::optional<SDL_FColor> color) {
				self.SetTint(color.value_or(SDL_FColor{ .r=1.0, .g=1.0f, .b=1.0f, .a=1.0f }));
			};

		Texture_Type["SetScaleMode"] = &Texture::SetScaleMode;

		Texture_Type["GetBlendMode"] = &Texture::GetBlendMode;

		Texture_Type["GetTint"] = &Texture::GetTint;

		Texture_Type["GetScaleMode"] = &Texture::GetScaleMode;

		Texture_Type["GetSize"] = &Texture::GetSize;

		auto Simple2DRenderer_Type = lua.new_usertype<Simple2DRenderer>("RenderingContext", sol::no_constructor);
		lua["Render"] = shared_from_this();


		Simple2DRenderer_Type["BufferClear"] = &Simple2DRenderer::BufferClear;

		Simple2DRenderer_Type["BufferPresent"] = &Simple2DRenderer::BufferPresent;

		Simple2DRenderer_Type["SetDrawColor"] = [](const std::shared_ptr<Simple2DRenderer>& context, const sol::optional<float> r, const sol::optional<float> g,
		                             const sol::optional<float> b, const sol::optional<float> a) {
				context->SetDrawColor(
					r.value_or(defaultAlphaFloat),
					g.value_or(defaultAlphaFloat),
					b.value_or(defaultAlphaFloat),
					a.value_or(defaultAlphaFloat)
					);
			};

		Simple2DRenderer_Type["SetDrawTarget"] = &Simple2DRenderer::SetDrawTarget;

		Simple2DRenderer_Type["DrawTexture"] = [](const std::shared_ptr<Simple2DRenderer>& context, const Texture& tex, const sol::optional<SDL_FRect> &src,
		                            const sol::optional<SDL_FRect> &dest) {
			context->DrawTexture(tex,
				(src) ? std::optional<SDL_FRect>{std::in_place_t{}, *src} : std::nullopt,
				(dest) ? std::optional<SDL_FRect>{std::in_place_t{}, *dest} : std::nullopt);
		};

		Simple2DRenderer_Type["DrawTextureRotated"] = [](const std::shared_ptr<Simple2DRenderer>& context, const Texture& tex, const sol::optional<SDL_FRect>& src, const sol::optional<SDL_FRect>& dest,
		                            const sol::optional<double> angle, sol::optional<SDL_FPoint> center,
		                            const sol::optional<SDL_FlipMode> flip) {
				context->DrawTexture(tex,
					(src) ? std::optional<SDL_FRect>{std::in_place_t{}, *src} : std::nullopt,
					(dest) ? std::optional<SDL_FRect>{std::in_place_t{}, *dest} : std::nullopt,
					angle.value_or(0.0),
					(center) ? std::optional<SDL_FPoint>{std::in_place_t{}, *center} : std::nullopt,
					flip.value_or(SDL_FLIP_NONE));
			};

		// NOLINTEND(bugprone-easily-swappable-parameters)

		Simple2DRenderer_Type["LoadTextureImg"] = &Simple2DRenderer::LoadTextureImg;

		Simple2DRenderer_Type["CreateRenderableTexture"] = &Simple2DRenderer::CreateRenderableTexture;

		Simple2DRenderer_Type["CreateTextureFromSurface"] = &Simple2DRenderer::CreateTextureFromSurface;

		Simple2DRenderer_Type["GetDrawColor"] = &Simple2DRenderer::GetDrawColor;

		Simple2DRenderer_Type["DrawGeometry"] = &Simple2DRenderer::DrawGeometry;

		Simple2DRenderer_Type["DrawPlainText"] = &Simple2DRenderer::DrawPlainText;

		Simple2DRenderer_Type["DrawPlainWrapText"] = &Simple2DRenderer::DrawPlainWrapText;

		Simple2DRenderer_Type["LoadPlainText"] = &Simple2DRenderer::LoadPlainText;

		Simple2DRenderer_Type["LoadBlendedText"] = &Simple2DRenderer::LoadBlendedText;

		Simple2DRenderer_Type["LoadShadedText"] = &Simple2DRenderer::LoadShadedText;

		Simple2DRenderer_Type["LoadLCDText"] = &Simple2DRenderer::LoadLCDText;

		Simple2DRenderer_Type["LoadPlainWrapText"] = &Simple2DRenderer::LoadPlainWrapText;

		Simple2DRenderer_Type["LoadBlendedWrapText"] = &Simple2DRenderer::LoadBlendedWrapText;

		Simple2DRenderer_Type["LoadShadedWrapText"] = &Simple2DRenderer::LoadShadedWrapText;

		Simple2DRenderer_Type["LoadLCDWrapText"] = &Simple2DRenderer::LoadLCDWrapText;
	}
} // namespace swgtk
