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

#include <algorithm>
#include <concepts>
#include <numbers>

namespace swgtk {

  // A collection of math functions and helper classes.

  /** @brief Strongly typed representation of an angle in radians.
   *
   * @tparam Type - Requires a floating-point type. Anything else is a compiler error.
   */
  template<std::floating_point Type>
  class radians {
  public:
    using value_type = Type;
    static constexpr value_type pi2 = static_cast<value_type>(std::numbers::pi_v<Type> + std::numbers::pi_v<Type>);

    constexpr radians() = default;
    constexpr explicit radians(Type value) noexcept :
        _value(std::clamp(value, -pi2, pi2)) {}

    [[nodiscard]] constexpr auto value() const noexcept -> value_type { return _value; }
    [[nodiscard]] constexpr auto operator*() const -> value_type { return _value; }
    [[nodiscard]] constexpr auto operator<=>(const radians& other) const = default;

    [[nodiscard]] constexpr auto operator+(const radians& other) const -> radians {
      return radians{apply_saturation(_value + *other)};
    }

    [[nodiscard]] constexpr auto operator-(const radians& other) const -> radians {
      return radians{apply_saturation(_value - *other)};
    }

    constexpr auto operator+=(const radians& other) -> radians& {
      _value = apply_saturation(_value + *other);
      return *this;
    }

    constexpr auto operator-=(const radians& other) -> radians& {
      _value = apply_saturation(_value - *other);
      return *this;
    }

    [[nodiscard]] constexpr auto operator+(const value_type other) const -> radians {
      return *this + radians{other};
    }

    [[nodiscard]] constexpr auto operator-(const value_type other) const -> radians {
      return *this - radians{other};
    }

    constexpr auto operator+=(const value_type other) -> radians& {
      *this += radians{other};
      return *this;
    }

    constexpr auto operator-=(const value_type other) -> radians& {
      *this -= radians{other};
      return *this;
    }

  private:
    [[nodiscard]] constexpr auto apply_saturation(value_type value) const -> value_type {
      if (value > pi2) {
        value -= pi2;
      } else if (value < -pi2) {
        value += pi2;
      }
      return value;
    }

    Type _value{};
  };

  /** @brief Strongly typed representation of an angle in degrees.
   *
   * @tparam Type - Requires a floating-point type. Anything else is a compiler error.
   */
  template<std::floating_point Type>
  class degrees {
  public:
    using value_type = Type;
    static constexpr value_type three_sixty = static_cast<value_type>(360.0f);

    constexpr degrees() = default;
    explicit constexpr degrees(Type value) noexcept :
        _value(std::clamp(value, -three_sixty, three_sixty)) {}

    [[nodiscard]] constexpr auto value() const noexcept -> value_type { return _value; }
    [[nodiscard]] constexpr auto operator*() const -> value_type { return _value; }
    [[nodiscard]] constexpr auto operator<=>(const degrees& other) const = default;

    [[nodiscard]] constexpr auto operator+(const degrees& other) const -> degrees {
      return degrees{apply_saturation(_value + *other)};
    }

    [[nodiscard]] constexpr auto operator-(const degrees& other) const -> degrees {
      return degrees{apply_saturation(_value - *other)};
    }

    constexpr auto operator+=(const degrees& other) -> degrees& {
      _value = apply_saturation(_value + *other);
      return *this;
    }

    constexpr auto operator-=(const degrees& other) -> degrees& {
      _value = apply_saturation(_value - *other);
      return *this;
    }

    [[nodiscard]] constexpr auto operator+(const value_type other) const -> degrees {
      return *this + degrees{other};
    }

    [[nodiscard]] constexpr auto operator-(const value_type other) const -> degrees {
      return *this - degrees{other};
    }

    constexpr auto operator+=(const value_type other) -> degrees& {
      *this += degrees{other};
      return *this;
    }

    constexpr auto operator-=(const value_type other) -> degrees& {
      *this -= degrees{other};
      return *this;
    }

  private:
    [[nodiscard]] constexpr auto apply_saturation(value_type value) const -> value_type {
      if (value > three_sixty) {
        value -= three_sixty;
      } else if (value < -three_sixty) {
        value += three_sixty;
      }
      return value;
    }

    Type _value{};
  };

  // Inline representations of the above types.

  using rads = radians<double>;
  using radsf = radians<float>;
  using radsl = radians<long double>;
  using degs = degrees<double>;
  using degsf = degrees<float>;
  using degsl = degrees<long double>;

  /** @brief Convert an angle from radians to degrees. You are required to pass the
   * angle in a Radians object.
   *
   * @tparam Type - Requires a floating-point type. Otherwise, it won't compile.
   * @param rad - The angle you wish to convert, passed as radians.
   * @return - The same angle now represented as degrees.
   */
  template<std::floating_point Type>
  [[nodiscard]] constexpr auto radians_to_degrees(const radians<Type> rad) {
    static constexpr auto one_eighty = static_cast<Type>(180.0f);
    static constexpr auto pi = std::numbers::pi_v<Type>;

    return degrees{(*rad / pi) * one_eighty};
  }

  /** @brief Convert an angle from degrees to radians. You are required to pass the
   * angle in a Degrees object.
   *
   * @tparam Type - Requires a floating-point type. Otherwise, it won't compile.
   * @param deg
   * @return - The same angle now represented as radians.
   */
  template<std::floating_point Type>
  [[nodiscard]] constexpr auto degrees_to_radians(const degrees<Type> deg) {
    static constexpr auto one_eighty = static_cast<Type>(180.0f);
    static constexpr auto pi = std::numbers::pi_v<Type>;

    return radians{(*deg / one_eighty) * pi};
  }
} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_MATH_HPP_
