#ifndef SWGTK_UTILITY_HPP
#define SWGTK_UTILITY_HPP

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_surface.h>
#include <memory>
#include <span>
#include <numbers>

#ifdef _DEBUG
#include <fmt/format.h>
#define DEBUG_PRINT(Debug_Message, Debug_Error) fmt::println(Debug_Message, Debug_Error);
#define DEBUG_PRINT2(Debug_Message, Debug_Error_1, Debug_error_2) fmt::println(Debug_Message, Debug_Error_1, Debug_error_2);

#else
#define DEBUG_PRINT(Debug_Message, Debug_Error) 
#define DEBUG_PRINT2(Debug_Message, Debug_Error_1, Debug_error_2) 
#endif

namespace swgtk {
    class Surface {
        static constexpr void DestroySurface(SDL_Surface* surface) { SDL_DestroySurface(surface); };
        static constexpr uint8_t whiteColorValue = 255u;
    public:
        constexpr Surface() = default;
        constexpr explicit Surface(SDL_Surface* surface) : _surface(SDL_DuplicateSurface(surface), Surface::DestroySurface) {}
        constexpr Surface(int width, int height, SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA32) 
        : _surface(SDL_CreateSurface(width, height, format), Surface::DestroySurface) {
            if(!_surface) {
                DEBUG_PRINT("Failed to create surface: {}\n", SDL_GetError())
            }
        }

        constexpr Surface(int width, int height, SDL_PixelFormat format, void* pixels, int pitch)
        : _surface(SDL_CreateSurfaceFrom(width, height, format, pixels, pitch), Surface::DestroySurface) {
            if(!_surface) {
                DEBUG_PRINT("Failed to create surface: {}\n", SDL_GetError())
            }
        }

        [[nodiscard]] constexpr SDL_Surface* operator*() const { return _surface.get(); }

        constexpr void Clear(SDL_FColor color = SDL_FColor { .r=0.0f, .g=0.0f, .b = 0.0f, .a=1.0f }) const { SDL_ClearSurface( _surface.get(), color.r, color.g, color.b, color.a); }
        
        [[nodiscard]] constexpr SDL_FColor ReadPixel(int x, int y) const {
            float r{}, g{}, b{}, a{};
            SDL_ReadSurfacePixelFloat(_surface.get(), x, y, &r, &g, &b, &a);
            return SDL_FColor{ .r=r, .g=g, .b=b, .a=a };
        }

        constexpr void DrawPixel(int x, int y, SDL_FColor color = SDL_FColor { .r=1.0f, .g=1.0f, .b = 1.0f, .a=1.0f }) const { SDL_WriteSurfacePixelFloat(_surface.get(), x, y, color.r, color.g, color.b, color.a); }

        constexpr void FillRect(SDL_Rect rect, SDL_Color color = SDL_Color { .r=whiteColorValue, .g=whiteColorValue, .b =whiteColorValue, .a=whiteColorValue }) const {
            SDL_FillSurfaceRect(_surface.get(), &rect, SDL_MapSurfaceRGBA(_surface.get(), color.r, color.g, color.b, color.a));
        }

        constexpr void FillRects(std::span<SDL_Rect> rects, SDL_Color color = SDL_Color { .r=whiteColorValue, .g=whiteColorValue, .b =whiteColorValue, .a=whiteColorValue }) const {
            SDL_FillSurfaceRects(_surface.get(), rects.data(), static_cast<int>(std::ssize(rects)),
             SDL_MapSurfaceRGBA(_surface.get(), color.r, color.g, color.b, color.a));
        }

    private:
        std::shared_ptr<SDL_Surface> _surface;
    };

    namespace math {
        inline constexpr double pi2 = std::numbers::pi * 2.0;
        inline constexpr float pi2f = std::numbers::pi_v<float> * 2.0f;

        [[nodiscard]] constexpr double radiansToDegrees(double radians) {
            static constexpr auto oneEighty = 180.0;
            
            return  (radians / std::numbers::pi) * oneEighty;
        }

        [[nodiscard]] constexpr float radiansToDegrees(float radians) {
            static constexpr auto oneEighty = 180.0f;
            
            return  (radians / std::numbers::pi_v<float>) * oneEighty;
        }

        [[nodiscard]] constexpr double degreesToRadians(double degrees) {
            static constexpr auto oneEighty = 180.0;

            return (degrees / oneEighty) * std::numbers::pi;
        }

        [[nodiscard]] constexpr float degreesToRadians(float degrees) {
            static constexpr auto oneEighty = 180.0f;

            return (degrees / oneEighty) * std::numbers::pi_v<float>;
        }
    }
}

#endif