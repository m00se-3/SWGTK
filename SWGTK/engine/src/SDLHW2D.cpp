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
#include "swgtk/SDLHW2D.hpp"

#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "swgtk/RenderingDevice.hpp"
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <filesystem>
#include <string>
#include <string_view>
#include <utility>
#include <sol/optional_implementation.hpp>
#include <memory>

namespace swgtk {
	bool SDLHW2D::PrepareDevice(const std::any& window_ptr) {
		if (auto* window = std::any_cast<SDL_Window*>(window_ptr); window != nullptr) {
			_render = SDL_CreateRenderer(window, nullptr);
			return IsDeviceInitialized();
		}

		return false;
	}

	void SDLHW2D::DestroyDevice() {
		SDL_DestroyRenderer(_render);
	}

	void SDLHW2D::BufferClear(const SDL_FColor& color) {
		const auto tmpColor = GetDrawColor();

		SetDrawColor(color);
		SDL_RenderClear(_render);
		SetDrawColor(tmpColor);
	}

	void SDLHW2D::BufferPresent() {
		SDL_SetRenderTarget(_render, nullptr);
		SDL_RenderPresent(_render);
	}
	
	Texture SDLHW2D::LoadTextureImg(const std::filesystem::path& img, const SDL_BlendMode blendMode) const {
	    if(std::filesystem::exists(img)) {
	    	const auto imgStr = img.string();

			if(auto* texture = IMG_LoadTexture(_render, imgStr.c_str()); texture != nullptr) {
				SDL_SetTextureBlendMode(texture, blendMode);
				return Texture{ texture };
			}

			DEBUG_PRINT2("Failed to load image {}: {}\n", imgStr, SDL_GetError())
	    }

	    return Texture{};
	}

	Texture SDLHW2D::CreateRenderableTexture(int width, int height, const SDL_PixelFormat format, const SDL_BlendMode blendMode) const {
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

	Texture SDLHW2D::CreateTextureFromSurface(const Surface &surface) const {
		if(auto* texture = SDL_CreateTextureFromSurface(_render, *surface)) {
			return Texture{ texture };
		}

		DEBUG_PRINT("Failed to create texture: {}\n", SDL_GetError())
		return Texture{};
	}

	void SDLHW2D::DrawTexture(Texture texture, const std::optional<SDL_FRect>& src, const std::optional<SDL_FRect>& dest) const {
		const auto* source = src ? &src.value() : nullptr;
		const auto* destination = dest ? &dest.value() : nullptr;
		
		SDL_RenderTexture(_render, *texture, source, destination);
	}

	auto SDLHW2D::DrawTexture(Texture texture, const std::optional<SDL_FRect> &src,
	                                   const std::optional<SDL_FRect> &dest, const double angle,
	                                   const std::optional<SDL_FPoint> &center, const SDL_FlipMode flip) const -> void {
		const auto* source = src ? &src.value() : nullptr;
		const auto* destination = dest ? &dest.value() : nullptr;
		const auto* cen = center ? &center.value() : nullptr;
		
		SDL_RenderTextureRotated(_render, *texture, source, destination, angle, cen, flip);
	}

	void SDLHW2D::DrawPlainText(const std::string_view text, const SDL_FRect& pos, const SDL_Color& color) const {

		if(auto* ttf = TTF_RenderText_Solid(_currentFont, text.data(), text.size(), color); ttf != nullptr) {
			auto* texture = SDL_CreateTextureFromSurface(_render, ttf);
			SDL_RenderTexture(_render, texture, nullptr, &pos);
			SDL_DestroySurface(ttf);
		} else {
			DEBUG_PRINT("{}", SDL_GetError())
		}
	}

	void SDLHW2D::DrawPlainWrapText(const std::string_view text, const SDL_FRect& pos, const int wrapLen, const SDL_Color &color) const {
		if(auto* ttf = TTF_RenderText_Solid_Wrapped(_currentFont, text.data(), text.size(), color, wrapLen); ttf != nullptr) {
			auto* texture = SDL_CreateTextureFromSurface(_render, ttf);
			SDL_RenderTexture(_render, texture, nullptr, &pos);
			SDL_DestroySurface(ttf);
		} else {
			DEBUG_PRINT("{}", SDL_GetError())
		}
	}

	Texture SDLHW2D::LoadPlainText(const std::string_view text, const SDL_Color& color) const {
		auto* surf = TTF_RenderText_Solid(_currentFont, text.data(), text.size(), color);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture SDLHW2D::LoadBlendedText(const std::string_view text, const SDL_Color& color) const {
		auto* surf = TTF_RenderText_Blended(_currentFont, text.data(), text.size(), color);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture SDLHW2D::LoadShadedText(const std::string_view text, const SDL_Color& bg, const SDL_Color& fg) const {
		auto* surf = TTF_RenderText_Shaded(_currentFont, text.data(), text.size(), fg, bg);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture SDLHW2D::LoadLCDText(const std::string_view text, const SDL_Color& bg, const SDL_Color& fg) const {
		auto* surf = TTF_RenderText_LCD(_currentFont, text.data(), text.size(), fg, bg);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture SDLHW2D::LoadPlainWrapText(const std::string_view text, const int wrapLen, const SDL_Color& color) const {
		auto* surf = TTF_RenderText_Solid_Wrapped(_currentFont, text.data(), text.size(), color, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture SDLHW2D::LoadBlendedWrapText(const std::string_view text, const int wrapLen, const SDL_Color& color) const {
		auto* surf = TTF_RenderText_Blended_Wrapped(_currentFont, text.data(), text.size(), color, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture SDLHW2D::LoadShadedWrapText(const std::string_view text, const int wrapLen, const SDL_Color bg, const SDL_Color fg) const {
		auto* surf = TTF_RenderText_Shaded_Wrapped(_currentFont, text.data(), text.size(), fg, bg, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

	Texture SDLHW2D::LoadLCDWrapText(const std::string_view text, const int wrapLen, const SDL_Color bg, const SDL_Color fg) const {
		auto* surf = TTF_RenderText_LCD_Wrapped(_currentFont, text.data(), text.size(), fg, bg, wrapLen);
		auto* texture = SDL_CreateTextureFromSurface(_render, surf);

		SDL_DestroySurface(surf);
		return Texture{texture};
	}

#ifdef SWGTK_BUILD_WITH_LUA
	void SDLHW2D::InitLua(sol::state* lua_) {

		auto& lua = *lua_;
		auto SWGTK = lua["swgtk"];

		// NOLINTBEGIN(*-easily-swappable-parameters)
		
		SWGTK["Vertex2D"] = lua.new_usertype<SDL_Vertex>(
		"Vertex2D", "position", &SDL_Vertex::position, "color", &SDL_Vertex::color, "tex_coord", &SDL_Vertex::tex_coord
		);

		SWGTK["Vertex2D"]["new"] = [](const sol::optional<SDL_FPoint> pos, const sol::optional<SDL_FColor> &col, const sol::optional<SDL_FPoint> tex) -> SDL_Vertex {
				return SDL_Vertex{ 
					.position=pos.value_or(SDL_FPoint{}), 
					.color=col.value_or(SDL_FColor{}), 
					.tex_coord=tex.value_or(SDL_FPoint{}) };
			};

		SWGTK["Colori"] = lua.new_usertype<SDL_Color>(
			"Colori", "r", &SDL_Color::r, "g", &SDL_Color::g, "b", &SDL_Color::b, "a", &SDL_Color::a
			);


		SWGTK["Colori"]["new"] = [] (const uint8_t r, const uint8_t g, const uint8_t b, const sol::optional<uint8_t> a) -> SDL_Color {
			static constexpr auto fullAplha = 255u;
			return SDL_Color{ .r=r, .g=g, .b=b, .a=a.value_or(fullAplha) };
		};

		SWGTK["Colorf"] = lua.new_usertype<SDL_FColor>(
			"Colorf", "r", &SDL_FColor::r, "g", &SDL_FColor::g, "b", &SDL_FColor::b, "a", &SDL_FColor::a
			);

		SWGTK["Colorf"]["new"] = [] (const float r, const float g, const float b, const sol::optional<float> a) -> SDL_FColor {
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
		SWGTK["BlendMode"] = lua["BlendMode"];

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
		SWGTK["PixelFormat"] = lua["PixelFormat"];

		SWGTK["Texture"] = lua.new_usertype<Texture>("Texture", sol::constructors<Texture(), Texture(SDL_Texture*), Texture(const Texture&)>());

		SWGTK["Texture"]["SetBlendMode"] = &Texture::SetBlendMode;

		SWGTK["Texture"]["SetTint"] = [](const Texture& self, const sol::optional<SDL_FColor>& color) {
				self.SetTint(color.value_or(SDL_FColor{ .r=1.0, .g=1.0f, .b=1.0f, .a=1.0f }));
			};

		SWGTK["Texture"]["SetScaleMode"] = &Texture::SetScaleMode;

		SWGTK["Texture"]["GetBlendMode"] = &Texture::GetBlendMode;

		SWGTK["Texture"]["GetTint"] = &Texture::GetTint;

		SWGTK["Texture"]["GetScaleMode"] = &Texture::GetScaleMode;

		SWGTK["Texture"]["GetSize"] = &Texture::GetSize;

		auto Simple2DRenderer_Type = lua.new_usertype<SDLHW2D>("RenderingContext", sol::no_constructor);
		SWGTK["Render"] = shared_from_this();


		Simple2DRenderer_Type["BufferClear"] = &SDLHW2D::BufferClear;

		Simple2DRenderer_Type["BufferPresent"] = &SDLHW2D::BufferPresent;

		Simple2DRenderer_Type["SetDrawColor"] = [](const std::shared_ptr<SDLHW2D>& context, const sol::optional<float> r, const sol::optional<float> g,
		                             const sol::optional<float> b, const sol::optional<float> a) {
				context->SetDrawColor(
					r.value_or(defaultAlphaFloat),
					g.value_or(defaultAlphaFloat),
					b.value_or(defaultAlphaFloat),
					a.value_or(defaultAlphaFloat)
					);
			};

		Simple2DRenderer_Type["SetDrawTarget"] = &SDLHW2D::SetDrawTarget;

		Simple2DRenderer_Type["DrawTexture"] = [](const std::shared_ptr<SDLHW2D>& context, const Texture& tex, const sol::optional<SDL_FRect> &src,
		                            const sol::optional<SDL_FRect> &dest) {
			context->DrawTexture(tex,
				(src) ? std::optional<SDL_FRect>{std::in_place_t{}, *src} : std::nullopt,
				(dest) ? std::optional<SDL_FRect>{std::in_place_t{}, *dest} : std::nullopt);
		};

		Simple2DRenderer_Type["DrawTextureRotated"] = [](const std::shared_ptr<SDLHW2D>& context, const Texture& tex, const sol::optional<SDL_FRect>& src, const sol::optional<SDL_FRect>& dest,
		                            const sol::optional<double> angle, sol::optional<SDL_FPoint> center,
		                            const sol::optional<SDL_FlipMode> flip) {
				context->DrawTexture(tex,
					(src) ? std::optional<SDL_FRect>{std::in_place_t{}, *src} : std::nullopt,
					(dest) ? std::optional<SDL_FRect>{std::in_place_t{}, *dest} : std::nullopt,
					angle.value_or(0.0),
					(center) ? std::optional<SDL_FPoint>{std::in_place_t{}, *center} : std::nullopt,
					flip.value_or(SDL_FLIP_NONE));
			};

		// NOLINTEND(*-easily-swappable-parameters)

		Simple2DRenderer_Type["SetVSync"] = &SDLHW2D::SetVSync;

		Simple2DRenderer_Type["GetVSync"] = &SDLHW2D::GetVSync;

		Simple2DRenderer_Type["LoadTextureImg"] = &SDLHW2D::LoadTextureImg;

		Simple2DRenderer_Type["CreateRenderableTexture"] = &SDLHW2D::CreateRenderableTexture;

		Simple2DRenderer_Type["CreateTextureFromSurface"] = &SDLHW2D::CreateTextureFromSurface;

		Simple2DRenderer_Type["GetDrawColor"] = &SDLHW2D::GetDrawColor;

		Simple2DRenderer_Type["DrawGeometry"] = &SDLHW2D::DrawGeometry;

		Simple2DRenderer_Type["DrawPlainText"] = &SDLHW2D::DrawPlainText;

		Simple2DRenderer_Type["DrawPlainWrapText"] = &SDLHW2D::DrawPlainWrapText;

		Simple2DRenderer_Type["LoadPlainText"] = &SDLHW2D::LoadPlainText;

		Simple2DRenderer_Type["LoadBlendedText"] = &SDLHW2D::LoadBlendedText;

		Simple2DRenderer_Type["LoadShadedText"] = &SDLHW2D::LoadShadedText;

		Simple2DRenderer_Type["LoadLCDText"] = &SDLHW2D::LoadLCDText;

		Simple2DRenderer_Type["LoadPlainWrapText"] = &SDLHW2D::LoadPlainWrapText;

		Simple2DRenderer_Type["LoadBlendedWrapText"] = &SDLHW2D::LoadBlendedWrapText;

		Simple2DRenderer_Type["LoadShadedWrapText"] = &SDLHW2D::LoadShadedWrapText;

		Simple2DRenderer_Type["LoadLCDWrapText"] = &SDLHW2D::LoadLCDWrapText;
	}

#endif // SWGTK_BUILD_WITH_LUA
} // namespace swgtk
