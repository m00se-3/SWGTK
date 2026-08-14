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
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_SURFACE_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_SURFACE_HPP_

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>
#include <memory>
#include <span>

#include <swgtk/Utility.hpp>

namespace swgtk {

  /**
      @brief A reference-counted RAII wrapper for the SDL_Surface type.

      Unlike Texture2D, Surface provides a number of constructors that cover the majority of use cases.
   */
  class surface {
    static void destroy_surface(SDL_Surface* surface) { SDL_DestroySurface(surface); }
    static constexpr uint8_t white_color_value = 255u;

  public:
    constexpr surface() = default;
    explicit surface(SDL_Surface* surface) :
        _surface(SDL_DuplicateSurface(surface), surface::destroy_surface) {}

    surface(const int width, const int height, const SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA32) :
        _surface(SDL_CreateSurface(width, height, format), surface::destroy_surface) {
      if (!_surface) {
        DEBUG_PRINT("Failed to create surface: {}\n", SDL_GetError())
      }
    }

    surface(const int width, const int height, const SDL_PixelFormat format, void* pixels, const int pitch) :
        _surface(SDL_CreateSurfaceFrom(width, height, format, pixels, pitch), surface::destroy_surface) {
      if (!_surface) {
        DEBUG_PRINT("Failed to create surface: {}\n", SDL_GetError())
      }
    }

    [[nodiscard]] auto operator*() const -> SDL_Surface* { return _surface.get(); }

    [[nodiscard]] auto empty() const -> bool { return static_cast<bool>(_surface); }

    void clear(const SDL_FColor& color = SDL_FColor{.r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f}) const { SDL_ClearSurface(_surface.get(), color.r, color.g, color.b, color.a); }

    [[nodiscard]] auto read_pixel(const int x, const int y) const -> SDL_FColor {
      float r{}, g{}, b{}, a{};
      SDL_ReadSurfacePixelFloat(_surface.get(), x, y, &r, &g, &b, &a);
      return SDL_FColor{.r = r, .g = g, .b = b, .a = a};
    }

    void draw_pixel(const int x, const int y, const SDL_FColor& color = SDL_FColor{.r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f}) const { SDL_WriteSurfacePixelFloat(_surface.get(), x, y, color.r, color.g, color.b, color.a); }

    void fill_rect(const SDL_Rect& rect, const SDL_Color& color = SDL_Color{.r = white_color_value, .g = white_color_value, .b = white_color_value, .a = white_color_value}) const {
      SDL_FillSurfaceRect(_surface.get(), &rect, SDL_MapSurfaceRGBA(_surface.get(), color.r, color.g, color.b, color.a));
    }

    void fill_rects(const std::span<SDL_Rect> rects, const SDL_Color& color = SDL_Color{.r = white_color_value, .g = white_color_value, .b = white_color_value, .a = white_color_value}) const {
      SDL_FillSurfaceRects(_surface.get(), rects.data(), static_cast<int>(std::ssize(rects)),
                           SDL_MapSurfaceRGBA(_surface.get(), color.r, color.g, color.b, color.a));
    }

  private:
    std::shared_ptr<SDL_Surface> _surface;
  };
} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_SURFACE_HPP_
