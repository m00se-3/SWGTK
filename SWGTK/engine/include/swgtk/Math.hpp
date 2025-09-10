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
#include <concepts>
#include <algorithm>

namespace swgtk {

    // A collection of math functions and helper classes.

    /** @brief Strongly typed representation of an angle in radians.
     *
     * @tparam Type - Requires a floating-point type. Anything else is a compiler error.
     */
    template<std::floating_point Type>
    class Radians {
    public:
        using value_type = Type;
        static constexpr value_type pi2 = static_cast<value_type>(std::numbers::pi_v<Type> + std::numbers::pi_v<Type>);

        constexpr Radians() = default;
        constexpr explicit Radians(Type value) noexcept : _value(std::clamp(value, -pi2, pi2)) {}

        [[nodiscard]] constexpr value_type value() const noexcept { return _value; }
        [[nodiscard]] constexpr value_type operator *() const { return _value; }
        [[nodiscard]] constexpr auto operator <=>(const Radians& other) const = default;

        [[nodiscard]] constexpr Radians operator +(const Radians& other) const {
            return Radians{ ApplySaturation(_value + *other) };
        }

        [[nodiscard]] constexpr Radians operator -(const Radians& other) const {
            return Radians{ ApplySaturation(_value - *other) };
        }

        constexpr Radians& operator +=(const Radians& other) {
            _value = ApplySaturation(_value + *other);
            return *this;
        }

        constexpr Radians& operator -=(const Radians& other) {
            _value = ApplySaturation(_value - *other);
            return *this;
        }

        [[nodiscard]] constexpr Radians operator +(const value_type other) const {
            return *this + Radians{ other };
        }

        [[nodiscard]] constexpr Radians operator -(const value_type other) const {
            return *this - Radians{ other };
        }

        constexpr Radians& operator +=(const value_type other) {
            *this += Radians{ other };
            return *this;
        }

        constexpr Radians& operator -=(const value_type other) {
            *this -= Radians{ other };
            return *this;
        }

    private:
        [[nodiscard]] constexpr value_type ApplySaturation(value_type value) const {
            if (value > pi2) { value -= pi2; }
            else if (value < -pi2) { value += pi2; }
            return value;
        }

        Type _value;
    };

    /** @brief Strongly typed representation of an angle in degrees.
     *
     * @tparam Type - Requires a floating-point type. Anything else is a compiler error.
     */
    template<std::floating_point Type>
    class Degrees{
    public:
        using value_type = Type;
        static constexpr value_type threeSixty = static_cast<value_type>(360.0f);

        constexpr Degrees() = default;
        explicit constexpr Degrees(Type value) noexcept : _value(std::clamp(value, -threeSixty, threeSixty)) {}

        [[nodiscard]] constexpr value_type value() const noexcept { return _value; }
        [[nodiscard]] constexpr value_type operator *() const { return _value; }
        [[nodiscard]] constexpr auto operator <=>(const Degrees& other) const = default;

        [[nodiscard]] constexpr Degrees operator +(const Degrees& other) const {
            return Degrees{ ApplySaturation(_value + *other) };
        }

        [[nodiscard]] constexpr Degrees operator -(const Degrees& other) const {
            return Degrees{ ApplySaturation(_value - *other) };
        }

        constexpr Degrees& operator +=(const Degrees& other) {
            _value = ApplySaturation(_value + *other);
            return *this;
        }

        constexpr Degrees& operator -=(const Degrees& other) {
            _value = ApplySaturation(_value - *other);
            return *this;
        }

        [[nodiscard]] constexpr Degrees operator +(const value_type other) const {
            return *this + Degrees{ other };
        }

        [[nodiscard]] constexpr Degrees operator -(const value_type other) const {
            return *this - Degrees{ other };
        }

        constexpr Degrees& operator +=(const value_type other) {
			*this += Degrees{ other };
            return *this;
        }

        constexpr Degrees& operator -=(const value_type other) {
			*this -= Degrees{ other };
            return *this;
        }
    private:
        [[nodiscard]] constexpr value_type ApplySaturation(value_type value) const {
            if (value > threeSixty) { value -= threeSixty; }
            else if (value < -threeSixty) { value += threeSixty; }
            return value;
        }

        Type _value;
    };

    // Inline representations of the above types.

    using Rads = Radians<double>;
    using Radsf = Radians<float>;
    using Radsl = Radians<long double>;
    using Degs = Degrees<double>;
    using Degsf = Degrees<float>;
    using Degsl = Degrees<long double>;

    /** @brief Convert an angle from radians to degrees. You are required to pass the
     * angle in a Radians object.
     *
     * @tparam Type - Requires a floating-point type. Otherwise, it won't compile.
     * @param radians - The angle you wish to convert, passed as radians.
     * @return - The same angle now represented as degrees.
     */
    template<std::floating_point Type>
    [[nodiscard]] constexpr auto RadiansToDegrees(const Radians<Type> radians) {
        static constexpr auto oneEighty = static_cast<Type>(180.0f);
        static constexpr auto pi = std::numbers::pi_v<Type>;
        
        return  Degrees{ (*radians / pi) * oneEighty };
    }

    /** @brief Convert an angle from degrees to radians. You are required to pass the
     * angle in a Degrees object.
     *
     * @tparam Type - Requires a floating-point type. Otherwise, it won't compile.
     * @param degrees - The angle you wish to convert, passed as degrees.
     * @return - The same angle now represented as radians.
     */
    template<std::floating_point Type>
    [[nodiscard]] constexpr auto DegreesToRadians(const Degrees<Type> degrees) {
        static constexpr auto oneEighty = static_cast<Type>(180.0f);
        static constexpr auto pi = std::numbers::pi_v<Type>;

        return Radians{ (*degrees / oneEighty) * pi };
    }
}

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_MATH_HPP_