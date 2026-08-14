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

#include <swgtk/Texture.hpp>

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <swgtk/RenderingDevice.hpp>
#include <swgtk/Surface.hpp>
#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "SDL3/SDL_video.h"

#ifdef SWGTK_BUILD_WITH_LUA
#include <sol/sol.hpp>
#endif

#include <filesystem>
#include <memory>
#include <optional>
#include <span>
#include <string_view>

namespace swgtk {

  /**
   * @brief This is currently the class you want to use for basic hardware 2D rendering.
     This is based on SDL3's hardware accelerated 2D rendering backend. It does not support shaders.
   *
   */
  class basic_2d : public rendering_device, public std::enable_shared_from_this<basic_2d> {
  public:
    constexpr basic_2d() = default;
    basic_2d(const basic_2d&) = delete;
    basic_2d(basic_2d&&) noexcept = delete;
    auto operator=(const basic_2d&) -> basic_2d& = delete;
    auto operator=(basic_2d&&) noexcept -> basic_2d& = delete;
    ~basic_2d() override { basic_2d::destroy_device(); }

    void buffer_clear(const color_t& col = color_t{}) override;
    void buffer_present() override;

    void set_background_color(const color_t& color) override { set_draw_color(color); }
    [[nodiscard]] constexpr auto is_device_initialized() const -> bool override { return _render != nullptr; }

    [[nodiscard]] auto prepare_device(SDL_Window* window_ptr) -> bool override;

    void destroy_device() override;

    /**
     * @brief SDL3 has several vsync options that you can set. This function wraps that functionality.
     *
     * @param value
     */
    void set_vsync(const vsync value) { SDL_SetRenderVSync(_render, std::to_underlying(value)); }

    [[nodiscard]] auto get_vsync() const -> vsync {
      int ret{};
      SDL_GetRenderVSync(_render, &ret);
      return vsync{ret};
    }

    [[nodiscard]] auto get_ref() -> std::weak_ptr<rendering_device> override { return shared_from_this(); }

    void set_draw_color(const red_v r, const green_v g, const blue_v b, const alpha_v a = alpha_v{}) const { SDL_SetRenderDrawColorFloat(_render, r.value, g.value, b.value, a.value); }
    void set_draw_color(const color_t& color = color_t{}) const {
      SDL_SetRenderDrawColorFloat(_render, color.r, color.g, color.b, color.a);
    }

    void draw_texture(texture_2d texture, const std::optional<SDL_FRect>& src = std::nullopt, const std::optional<SDL_FRect>& dest = std::nullopt) const;
    void draw_texture(texture_2d texture, const std::optional<SDL_FRect>& src,
                     const std::optional<SDL_FRect>& dest, double angle,
                     const std::optional<SDL_FPoint>& center = std::nullopt,
                     SDL_FlipMode flip = SDL_FLIP_NONE) const;

    /**
     * @brief Draw text at the specified location with the specified font. Uses SDL_ttf's fastest algorithm.
     *
     * @param font
     * @param text
     * @param pos - Destination rectangle
     * @param color
     */
    void draw_plain_text(TTF_Font* font, std::string_view text,
                       const SDL_FRect& pos, const color_t& color = color_t{ SDL_Color{.r = default_alpha_int, .g = default_alpha_int, .b = default_alpha_int, .a = default_alpha_int} }) const;

    /**
     * @brief Same as DrawPlainText() except it allows you to specify word wrapping support.
     *
     * @param font
     * @param text
     * @param pos
     * @param wrapLen Length of text before wrapping, in bytes.
     * @param color
     */
    void draw_plain_wrap_text(TTF_Font* font, std::string_view text, const SDL_FRect& pos,
                           int wrapLen = 0, const color_t& color = color_t{ SDL_Color{.r = default_alpha_int, .g = default_alpha_int, .b = default_alpha_int, .a = default_alpha_int} }) const;

    /*
      Combines SDL_ttf's API with SDL_Textures to preload text renderables as Textures. These can be rotated and tinted as needed.
    */

    [[nodiscard]] auto load_plain_text(TTF_Font* font,
                                     std::string_view text, const color_t& color = color_t{ SDL_Color{.r = default_alpha_int, .g = default_alpha_int, .b = default_alpha_int, .a = default_alpha_int} }) const -> texture_2d;

    [[nodiscard]] auto load_blended_text(TTF_Font* font,
                                       std::string_view text, const color_t& color = color_t{ SDL_Color{.r = default_alpha_int, .g = default_alpha_int, .b = default_alpha_int, .a = default_alpha_int} }) const -> texture_2d;

    [[nodiscard]] auto load_shaded_text(TTF_Font* font,
                                      std::string_view text,
                                      const color_t& bg = color_t{},
                                      const color_t& fg = color_t{ SDL_Color{.r = default_alpha_int, .g = default_alpha_int, .b = default_alpha_int, .a = default_alpha_int} }) const -> texture_2d;

    [[nodiscard]] auto load_lcd_text(TTF_Font* font,
                                   std::string_view text,
                                   const color_t& bg = color_t{},
                                   const color_t& fg = color_t{ SDL_Color{.r = default_alpha_int, .g = default_alpha_int, .b = default_alpha_int, .a = default_alpha_int} }) const -> texture_2d;

    [[nodiscard]] auto load_plain_wrap_text(TTF_Font* font, std::string_view text,
                                         int wrapLen = 0, const color_t& color = color_t{ SDL_Color{.r = default_alpha_int, .g = default_alpha_int, .b = default_alpha_int, .a = default_alpha_int} }) const -> texture_2d;

    [[nodiscard]] auto load_blended_wrap_text(TTF_Font* font, std::string_view text,
                                           int wrapLen = 0, const color_t& color = color_t{ SDL_Color{.r = default_alpha_int, .g = default_alpha_int, .b = default_alpha_int, .a = default_alpha_int} }) const -> texture_2d;

    [[nodiscard]] auto load_shaded_wrap_text(TTF_Font* font, std::string_view text,
                                          int wrapLen = 0,
                                          const color_t& bg = color_t{},
                                          const color_t& fg = color_t{ SDL_Color{.r = default_alpha_int, .g = default_alpha_int, .b = default_alpha_int, .a = default_alpha_int} }) const -> texture_2d;

    [[nodiscard]] auto load_lcd_wrap_text(TTF_Font* font, std::string_view text,
                                       int wrapLen = 0,
                                       const color_t& bg = color_t{},
                                       const color_t& fg = color_t{ SDL_Color{.r = default_alpha_int, .g = default_alpha_int, .b = default_alpha_int, .a = default_alpha_int} }) const -> texture_2d;

    /**
     * @brief Used to draw arbitrary shapes with raw vertex information. Great for making draw calls from
     *			external sources, like GUI libraries.
     *
     * @param texture
     * @param vertices
     * @param indices
     */
    void draw_geometry(texture_2d texture, const std::span<SDL_Vertex> vertices, const std::span<int> indices) const {
      SDL_RenderGeometry(_render, *texture, vertices.data(), static_cast<int>(std::ssize(vertices)),
                         indices.data(), static_cast<int>(std::ssize(indices)));
    }

    [[nodiscard]] auto load_texture_img(const std::filesystem::path& img, SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND) const -> texture_2d;
    [[nodiscard]] auto create_renderable_texture(int width, int height, SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA32, SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND) const -> texture_2d;
    [[nodiscard]] auto create_texture_from_surface(const surface& surface) const -> texture_2d;

    [[nodiscard]] auto get_draw_color() const -> color_t {
      SDL_FColor res{};
      SDL_GetRenderDrawColorFloat(_render, &res.r, &res.g, &res.b, &res.a);

      return color_t{ res };
    }

    auto set_draw_target(texture_2d texture) const -> bool { return SDL_SetRenderTarget(_render, *texture); }

    template <typename... Args>
    [[nodiscard]] static auto create(Args&&... args) noexcept { return std::make_shared<basic_2d>(std::forward<Args>(args)...); }

#ifdef SWGTK_BUILD_WITH_LUA
    void init_lua(sol::state* lua_) override;
#endif

  private:
    SDL_Renderer* _render = nullptr;
  };
} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_SIMPLE2DRENDERER_HPP_
