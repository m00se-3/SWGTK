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
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_MATH_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_MATH_HPP_

#include <numbers>


namespace swgtk::math {

    // A collection of math functions and pre-calculated values.

    inline constexpr double pi2 = std::numbers::pi * 2.0;
    inline constexpr float pi2f = std::numbers::pi_v<float> * 2.0f;

    [[nodiscard]] constexpr double RadiansToDegrees(const double radians) {
        static constexpr auto oneEighty = 180.0;
        
        return  (radians / std::numbers::pi) * oneEighty;
    }

    [[nodiscard]] constexpr float RadiansToDegrees(const float radians) {
        static constexpr auto oneEighty = 180.0f;
        
        return  (radians / std::numbers::pi_v<float>) * oneEighty;
    }

    [[nodiscard]] constexpr double DegreesToRadians(const double degrees) {
        static constexpr auto oneEighty = 180.0;

        return (degrees / oneEighty) * std::numbers::pi;
    }

    [[nodiscard]] constexpr float DegreesToRadians(const float degrees) {
        static constexpr auto oneEighty = 180.0f;

        return (degrees / oneEighty) * std::numbers::pi_v<float>;
    }
}

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_MATH_HPP_