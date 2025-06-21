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
     */
    class Surface {
        static void DestroySurface(SDL_Surface* surface) { SDL_DestroySurface(surface); }
        static constexpr uint8_t whiteColorValue = 255u;
    public:
        constexpr Surface() = default;
        explicit Surface(SDL_Surface* surface) : _surface(SDL_DuplicateSurface(surface), Surface::DestroySurface) {}
        Surface(const int width, const int height, const SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA32)
        : _surface(SDL_CreateSurface(width, height, format), Surface::DestroySurface) {
            if(!_surface) {
                DEBUG_PRINT("Failed to create surface: {}\n", SDL_GetError())
            }
        }

        Surface(const int width, const int height, const SDL_PixelFormat format, void* pixels, const int pitch)
        : _surface(SDL_CreateSurfaceFrom(width, height, format, pixels, pitch), Surface::DestroySurface) {
            if(!_surface) {
                DEBUG_PRINT("Failed to create surface: {}\n", SDL_GetError())
            }
        }

        [[nodiscard]] SDL_Surface* operator*() const { return _surface.get(); }

        void Clear(const SDL_FColor& color = SDL_FColor { .r=0.0f, .g=0.0f, .b = 0.0f, .a=1.0f }) const { SDL_ClearSurface( _surface.get(), color.r, color.g, color.b, color.a); }
        
        [[nodiscard]] SDL_FColor ReadPixel(const int x, const int y) const {
            float r{}, g{}, b{}, a{};
            SDL_ReadSurfacePixelFloat(_surface.get(), x, y, &r, &g, &b, &a);
            return SDL_FColor{ .r=r, .g=g, .b=b, .a=a };
        }

        void DrawPixel(const int x, const int y, const SDL_FColor& color = SDL_FColor { .r=1.0f, .g=1.0f, .b = 1.0f, .a=1.0f }) const { SDL_WriteSurfacePixelFloat(_surface.get(), x, y, color.r, color.g, color.b, color.a); }

        void FillRect(const SDL_Rect &rect, const SDL_Color& color = SDL_Color { .r=whiteColorValue, .g=whiteColorValue, .b =whiteColorValue, .a=whiteColorValue }) const {
            SDL_FillSurfaceRect(_surface.get(), &rect, SDL_MapSurfaceRGBA(_surface.get(), color.r, color.g, color.b, color.a));
        }

        void FillRects(const std::span<SDL_Rect> rects, const SDL_Color& color = SDL_Color { .r=whiteColorValue, .g=whiteColorValue, .b =whiteColorValue, .a=whiteColorValue }) const {
            SDL_FillSurfaceRects(_surface.get(), rects.data(), static_cast<int>(std::ssize(rects)),
             SDL_MapSurfaceRGBA(_surface.get(), color.r, color.g, color.b, color.a));
        }

    private:
        std::shared_ptr<SDL_Surface> _surface;
    };
} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_SURFACE_HPP_