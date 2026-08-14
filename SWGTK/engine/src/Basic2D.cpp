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
#include <swgtk/Basic2D.hpp>

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "swgtk/RenderingDevice.hpp"

#ifdef SWGTK_BUILD_WITH_LUA
#include <sol/optional_implementation.hpp>
#endif

namespace swgtk {
  auto basic_2d::prepare_device(SDL_Window* window_ptr) -> bool {
    if (window_ptr != nullptr) {
      _render = SDL_CreateRenderer(window_ptr, nullptr);
      return is_device_initialized();
    }

    return false;
  }

  void basic_2d::destroy_device() {
    SDL_DestroyRenderer(_render);
  }

  void basic_2d::buffer_clear(const color_t& color) {
    const auto tmp_color = get_draw_color();

    set_draw_color(color);
    SDL_RenderClear(_render);
    set_draw_color(tmp_color);
  }

  void basic_2d::buffer_present() {
    SDL_SetRenderTarget(_render, nullptr);
    SDL_RenderPresent(_render);
  }

  auto basic_2d::load_texture_img(const std::filesystem::path& img, const SDL_BlendMode blendMode) const -> texture_2d {
    if (std::filesystem::exists(img)) {
      const auto img_str = img.string();

      if (auto* texture = IMG_LoadTexture(_render, img_str.c_str()); texture != nullptr) {
        SDL_SetTextureBlendMode(texture, blendMode);
        return texture_2d{texture};
      }

      DEBUG_PRINT2("Failed to load image {}: {}\n", img_str, SDL_GetError())
    }

    return texture_2d{};
  }

  auto basic_2d::create_renderable_texture(int width, int height, const SDL_PixelFormat format, const SDL_BlendMode blendMode) const -> texture_2d {
    if (width < 1 || height < 1) {
      DEBUG_PRINT2("Invalid texture dimensions: {}, {}\n", width, height)
      return texture_2d{};
    }

    if (auto* texture = SDL_CreateTexture(_render, format, SDL_TEXTUREACCESS_TARGET, width, height); texture != nullptr) {
      SDL_SetTextureBlendMode(texture, blendMode);
      return texture_2d{texture};
    }

    DEBUG_PRINT("Error creating renderable texture: {}\n", SDL_GetError())
    return texture_2d{};
  }

  auto basic_2d::create_texture_from_surface(const surface& surface) const -> texture_2d {
    if (auto* texture = SDL_CreateTextureFromSurface(_render, *surface)) {
      return texture_2d{texture};
    }

    DEBUG_PRINT("Failed to create texture: {}\n", SDL_GetError())
    return texture_2d{};
  }

  void basic_2d::draw_texture(texture_2d texture, const std::optional<SDL_FRect>& src, const std::optional<SDL_FRect>& dest) const {
    const auto* source = src ? &src.value() : nullptr;
    const auto* destination = dest ? &dest.value() : nullptr;

    SDL_RenderTexture(_render, *texture, source, destination);
  }

  auto basic_2d::draw_texture(texture_2d texture, const std::optional<SDL_FRect>& src,
                            const std::optional<SDL_FRect>& dest, const double angle,
                            const std::optional<SDL_FPoint>& center, const SDL_FlipMode flip) const -> void {
    const auto* source = src ? &src.value() : nullptr;
    const auto* destination = dest ? &dest.value() : nullptr;
    const auto* cen = center ? &center.value() : nullptr;

    SDL_RenderTextureRotated(_render, *texture, source, destination, angle, cen, flip);
  }

  void basic_2d::draw_plain_text(TTF_Font* font, const std::string_view text, const SDL_FRect& pos, const color_t& color) const {
    SDL_assert_release(font != nullptr); //NOLINT(*-avoid-do-while, *-else-after-return)

    if (auto* ttf = TTF_RenderText_Solid(font, text.data(), text.size(), SDL_Color(color)); ttf != nullptr) {
      auto* texture = SDL_CreateTextureFromSurface(_render, ttf);
      SDL_RenderTexture(_render, texture, nullptr, &pos);
      SDL_DestroySurface(ttf);
    } else {
      DEBUG_PRINT("{}", SDL_GetError())
    }
  }

  void basic_2d::draw_plain_wrap_text(TTF_Font* font, const std::string_view text, const SDL_FRect& pos, const int wrapLen, const color_t& color) const {
    SDL_assert_release(font != nullptr); //NOLINT(*-avoid-do-while, *-else-after-return)

    if (auto* ttf = TTF_RenderText_Solid_Wrapped(font, text.data(), text.size(), SDL_Color(color), wrapLen); ttf != nullptr) {
      auto* texture = SDL_CreateTextureFromSurface(_render, ttf);
      SDL_RenderTexture(_render, texture, nullptr, &pos);
      SDL_DestroySurface(ttf);
    } else {
      DEBUG_PRINT("{}", SDL_GetError())
    }
  }

  auto basic_2d::load_plain_text(TTF_Font* font, const std::string_view text, const color_t& color) const -> texture_2d {
    SDL_assert_release(font != nullptr); //NOLINT(*-avoid-do-while, *-else-after-return)

    auto* surf = TTF_RenderText_Solid(font, text.data(), text.size(), SDL_Color(color));
    auto* texture = SDL_CreateTextureFromSurface(_render, surf);

    SDL_DestroySurface(surf);
    return texture_2d{texture};
  }

  auto basic_2d::load_blended_text(TTF_Font* font, const std::string_view text, const color_t& color) const -> texture_2d {
    SDL_assert_release(font != nullptr); //NOLINT(*-avoid-do-while, *-else-after-return)

    auto* surf = TTF_RenderText_Blended(font, text.data(), text.size(), SDL_Color(color));
    auto* texture = SDL_CreateTextureFromSurface(_render, surf);

    SDL_DestroySurface(surf);
    return texture_2d{texture};
  }

  auto basic_2d::load_shaded_text(TTF_Font* font, const std::string_view text, const color_t& bg, const color_t& fg) const -> texture_2d {
    SDL_assert_release(font != nullptr); //NOLINT(*-avoid-do-while, *-else-after-return)

    auto* surf = TTF_RenderText_Shaded(font, text.data(), text.size(), SDL_Color(fg), SDL_Color(bg));
    auto* texture = SDL_CreateTextureFromSurface(_render, surf);

    SDL_DestroySurface(surf);
    return texture_2d{texture};
  }

  auto basic_2d::load_lcd_text(TTF_Font* font, const std::string_view text, const color_t& bg, const color_t& fg) const -> texture_2d {
    SDL_assert_release(font != nullptr); //NOLINT(*-avoid-do-while, *-else-after-return)

    auto* surf = TTF_RenderText_LCD(font, text.data(), text.size(), SDL_Color(fg), SDL_Color(bg));
    auto* texture = SDL_CreateTextureFromSurface(_render, surf);

    SDL_DestroySurface(surf);
    return texture_2d{texture};
  }

  auto basic_2d::load_plain_wrap_text(TTF_Font* font, const std::string_view text, const int wrapLen, const color_t& color) const -> texture_2d {
    SDL_assert_release(font != nullptr); //NOLINT(*-avoid-do-while, *-else-after-return)

    auto* surf = TTF_RenderText_Solid_Wrapped(font, text.data(), text.size(), SDL_Color(color), wrapLen);
    auto* texture = SDL_CreateTextureFromSurface(_render, surf);

    SDL_DestroySurface(surf);
    return texture_2d{texture};
  }

  auto basic_2d::load_blended_wrap_text(TTF_Font* font, const std::string_view text, const int wrapLen, const color_t& color) const -> texture_2d {
    SDL_assert_release(font != nullptr); //NOLINT(*-avoid-do-while, *-else-after-return)

    auto* surf = TTF_RenderText_Blended_Wrapped(font, text.data(), text.size(), SDL_Color(color), wrapLen);
    auto* texture = SDL_CreateTextureFromSurface(_render, surf);

    SDL_DestroySurface(surf);
    return texture_2d{texture};
  }

  auto basic_2d::load_shaded_wrap_text(TTF_Font* font, const std::string_view text, const int wrapLen, const color_t& bg, const color_t& fg) const -> texture_2d {
    SDL_assert_release(font != nullptr); //NOLINT(*-avoid-do-while, *-else-after-return)

    auto* surf = TTF_RenderText_Shaded_Wrapped(font, text.data(), text.size(), SDL_Color(fg), SDL_Color(bg), wrapLen);
    auto* texture = SDL_CreateTextureFromSurface(_render, surf);

    SDL_DestroySurface(surf);
    return texture_2d{texture};
  }

  auto basic_2d::load_lcd_wrap_text(TTF_Font* font, const std::string_view text, const int wrapLen, const color_t& bg, const color_t& fg) const -> texture_2d {
    SDL_assert_release(font != nullptr); //NOLINT(*-avoid-do-while, *-else-after-return)

    auto* surf = TTF_RenderText_LCD_Wrapped(font, text.data(), text.size(), SDL_Color(fg), SDL_Color(bg), wrapLen);
    auto* texture = SDL_CreateTextureFromSurface(_render, surf);

    SDL_DestroySurface(surf);
    return texture_2d{texture};
  }

#ifdef SWGTK_BUILD_WITH_LUA
  void basic_2d::init_lua(sol::state* lua_) {

    auto& lua = *lua_;
    auto swgtk_table = lua["swgtk"];

    // NOLINTBEGIN(*-easily-swappable-parameters)

    swgtk_table["Vertex2D"] = lua.new_usertype<SDL_Vertex>(
        "Vertex2D", "position", &SDL_Vertex::position, "color_t", &SDL_Vertex::color, "tex_coord", &SDL_Vertex::tex_coord);

    swgtk_table["Vertex2D"]["new"] = [](const sol::optional<SDL_FPoint> pos, const sol::optional<color_t>& col, const sol::optional<SDL_FPoint> tex) -> SDL_Vertex {
      return SDL_Vertex{
          .position = pos.value_or(SDL_FPoint{}),
          .color = SDL_FColor(col.value_or(color_t{})),
          .tex_coord = tex.value_or(SDL_FPoint{})};
    };

    swgtk_table["Color"] = lua.new_usertype<color_t>(
      "Color", std::make_pair("r", &color_t::r), std::make_pair("green", &color_t::g),
            std::make_pair("blue", &color_t::b), std::make_pair("alpha", &color_t::a)
    );

    lua.new_enum<SDL_BlendMode>("BlendMode",
                                {
                                    std::make_pair("None", SDL_BLENDMODE_NONE),
                                    std::make_pair("Add", SDL_BLENDMODE_ADD),
                                    std::make_pair("Blend", SDL_BLENDMODE_BLEND),
                                    std::make_pair("PreBlend", SDL_BLENDMODE_BLEND_PREMULTIPLIED),
                                    std::make_pair("PreAdd", SDL_BLENDMODE_ADD_PREMULTIPLIED),
                                    std::make_pair("Mod", SDL_BLENDMODE_MOD),
                                    std::make_pair("Mul", SDL_BLENDMODE_MUL),
                                    std::make_pair("Invalid", SDL_BLENDMODE_INVALID),
                                });
    swgtk_table["BlendMode"] = lua["BlendMode"];

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
    swgtk_table["PixelFormat"] = lua["PixelFormat"];

    swgtk_table["Texture2D"] = lua.new_usertype<texture_2d>("Texture2D", sol::constructors<texture_2d(), texture_2d(SDL_Texture*), texture_2d(const texture_2d&)>());

    swgtk_table["Texture2D"]["SetBlendMode"] = &texture_2d::set_blend_mode;

    swgtk_table["Texture"]["SetTint"] = [](const texture_2d& self, const sol::optional<SDL_FColor>& color) -> void {
      self.set_tint(color.value_or(SDL_FColor{.r = 1.0, .g = 1.0f, .b = 1.0f, .a = 1.0f}));    };

    swgtk_table["Texture2D"]["SetScaleMode"] = &texture_2d::set_scale_mode;

    swgtk_table["Texture2D"]["GetBlendMode"] = &texture_2d::get_blend_mode;

    swgtk_table["Texture2D"]["GetTint"] = &texture_2d::get_tint;

    swgtk_table["Texture2D"]["GetScaleMode"] = &texture_2d::get_scale_mode;

    swgtk_table["Texture2D"]["GetSize"] = &texture_2d::get_size;

    auto simple2_d_renderer_type = lua.new_usertype<basic_2d>("RenderingContext", sol::no_constructor);
    swgtk_table["Render"] = shared_from_this();


    simple2_d_renderer_type["BufferClear"] = &basic_2d::buffer_clear;

    simple2_d_renderer_type["BufferPresent"] = &basic_2d::buffer_present;

    simple2_d_renderer_type["SetDrawColor"] = [](const std::shared_ptr<basic_2d>& context, const sol::optional<float> r, const sol::optional<float> g,
                                               const sol::optional<float> b, const sol::optional<float> a) -> void {
      context->set_draw_color(
        color_t{
          red_v{ r.value_or(default_alpha_float) },
          green_v{ g.value_or(default_alpha_float) },
          blue_v{ b.value_or(default_alpha_float) },
          alpha_v{ a.value_or(default_alpha_float) }
        });
    };

    simple2_d_renderer_type["SetDrawTarget"] = &basic_2d::set_draw_target;

    simple2_d_renderer_type["DrawTexture"] = [](const std::shared_ptr<basic_2d>& context, const texture_2d& tex, const sol::optional<SDL_FRect>& src,
                                              const sol::optional<SDL_FRect>& dest) -> void {
      context->draw_texture(tex,
                           (src) ? std::optional<SDL_FRect>{std::in_place_t{}, *src} : std::nullopt,
                           (dest) ? std::optional<SDL_FRect>{std::in_place_t{}, *dest} : std::nullopt);
    };

    simple2_d_renderer_type["DrawTextureRotated"] = [](const std::shared_ptr<basic_2d>& context, const texture_2d& tex, const sol::optional<SDL_FRect>& src, const sol::optional<SDL_FRect>& dest,
                                                     const sol::optional<double> angle, sol::optional<SDL_FPoint> center,
                                                     const sol::optional<SDL_FlipMode> flip) -> void {
      context->draw_texture(tex,
                           src ? std::optional<SDL_FRect>{std::in_place_t{}, *src} : std::nullopt,
                           dest ? std::optional<SDL_FRect>{std::in_place_t{}, *dest} : std::nullopt,
                           angle.value_or(0.0),
                           center ? std::optional<SDL_FPoint>{std::in_place_t{}, *center} : std::nullopt,
                           flip.value_or(SDL_FLIP_NONE));
    };

    // NOLINTEND(*-easily-swappable-parameters)

    simple2_d_renderer_type["SetVSync"] = &basic_2d::set_vsync;

    simple2_d_renderer_type["GetVSync"] = &basic_2d::get_vsync;

    simple2_d_renderer_type["LoadTextureImg"] = &basic_2d::load_texture_img;

    simple2_d_renderer_type["CreateRenderableTexture"] = &basic_2d::create_renderable_texture;

    simple2_d_renderer_type["CreateTextureFromSurface"] = &basic_2d::create_texture_from_surface;

    simple2_d_renderer_type["GetDrawColor"] = &basic_2d::get_draw_color;

    simple2_d_renderer_type["DrawGeometry"] = &basic_2d::draw_geometry;

    simple2_d_renderer_type["DrawPlainText"] = &basic_2d::draw_plain_text;

    simple2_d_renderer_type["DrawPlainWrapText"] = &basic_2d::draw_plain_wrap_text;

    simple2_d_renderer_type["LoadPlainText"] = &basic_2d::load_plain_text;

    simple2_d_renderer_type["LoadBlendedText"] = &basic_2d::load_blended_text;

    simple2_d_renderer_type["LoadShadedText"] = &basic_2d::load_shaded_text;

    simple2_d_renderer_type["LoadLCDText"] = &basic_2d::load_lcd_text;

    simple2_d_renderer_type["LoadPlainWrapText"] = &basic_2d::load_plain_wrap_text;

    simple2_d_renderer_type["LoadBlendedWrapText"] = &basic_2d::load_blended_wrap_text;

    simple2_d_renderer_type["LoadShadedWrapText"] = &basic_2d::load_shaded_wrap_text;

    simple2_d_renderer_type["LoadLCDWrapText"] = &basic_2d::load_lcd_wrap_text;
  }

#endif // SWGTK_BUILD_WITH_LUA
} // namespace swgtk
