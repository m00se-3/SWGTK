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
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_UTILITY_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_UTILITY_HPP_

#include <type_traits>
#include <utility>
#include <SDL3/SDL_pixels.h>

#ifdef _DEBUG
#include <format>
#define DEBUG_PRINT(Debug_Message, Debug_Error) std::puts(std::format(Debug_Message, Debug_Error).c_str());
#define DEBUG_PRINT2(Debug_Message, Debug_Error_1, Debug_error_2) std::puts(std::format(Debug_Message, Debug_Error_1, Debug_error_2).c_str());

#else
#define DEBUG_PRINT(Debug_Message, Debug_Error)
#define DEBUG_PRINT2(Debug_Message, Debug_Error_1, Debug_error_2)
#endif

namespace swgtk {

  /**
   * @brief A proxy class intended to deter users from erroneously deleting
   * framework pointers. You can access the underlying object with the '->'
   * operator.
   *
   * @tparam Ptr
   */
  template<typename Ptr>
  class ObjectRef {
  public:
    ObjectRef() = default;
    explicit ObjectRef(Ptr* ptr) : _ptr(ptr) {}
    auto operator->() const -> Ptr* { return _ptr; }
    explicit operator bool() const { return _ptr != nullptr; }

  private:
    Ptr* _ptr = nullptr;
  };

  // Items for internal use only.
  namespace detail {
    template<typename Ptr>
    class NonOwning {
    public:
      constexpr NonOwning() = default;
      constexpr explicit NonOwning(Ptr* ptr) : _ptr(ptr) {}
      [[nodiscard]] auto operator->() const -> Ptr* { return _ptr; }
      [[nodiscard]] auto operator->() -> Ptr* { return _ptr; }
      [[nodiscard]] explicit operator bool() const { return _ptr != nullptr; }
      [[nodiscard]] auto operator*() const -> Ptr* { return _ptr; }
      [[nodiscard]] auto operator*() -> Ptr* { return _ptr; }
      [[nodiscard]] explicit operator Ptr*() const { return _ptr; }
      [[nodiscard]] explicit operator Ptr*() { return _ptr; }

    private:
      Ptr* _ptr = nullptr;
    };
  }

  struct Red_t {
    constexpr Red_t() = default;
    constexpr explicit Red_t(float v) : value(v) {}
    constexpr explicit Red_t(uint8_t v) : value(v) {}

    std::common_type_t<uint8_t, float> value{};
  };

  struct Green_t {
    constexpr Green_t() = default;
    constexpr explicit Green_t(float v) : value(v) {}
    constexpr explicit Green_t(uint8_t v) : value(v) {}

    std::common_type_t<uint8_t, float> value{};
  };

  struct Blue_t {
    constexpr Blue_t() = default;
    constexpr explicit Blue_t(float v) : value(v) {}
    constexpr explicit Blue_t(uint8_t v) : value(v) {}

    std::common_type_t<uint8_t, float> value{};
  };

  struct Alpha_t {
    constexpr Alpha_t() = default;
    constexpr explicit Alpha_t(float v) : value(v) {}
    constexpr explicit Alpha_t(uint8_t v) : value(v) {}

    std::common_type_t<uint8_t, float> value = 1.0f;
  };

  struct Color {
    constexpr static auto floatMax = 255.0f;
    constexpr Color() = default;

    constexpr explicit Color(const SDL_Color& color)
    : red(static_cast<float>(color.r) / floatMax), green(static_cast<float>(color.g) / floatMax),
    blue(static_cast<float>(color.b) / floatMax), alpha(static_cast<float>(color.a) / floatMax) {}

    constexpr explicit Color(const SDL_FColor& color)
    : red(color.r), green(color.g), blue(color.b), alpha(color.a) {}

    constexpr Color(Red_t r, Green_t g, Blue_t b, Alpha_t a = Alpha_t{1.0f})
    : red(r.value), green(g.value), blue(b.value), alpha(a.value) {}

    constexpr explicit operator SDL_FColor() const {
      return SDL_FColor{ .r = red, .g = green, .b = blue, .a = alpha };
    }

    constexpr explicit operator SDL_Color() const {
      return SDL_Color{ 
        .r = static_cast<uint8_t>(red * floatMax),
        .g = static_cast<uint8_t>(green * floatMax),
        .b = static_cast<uint8_t>(blue * floatMax),
        .a = static_cast<uint8_t>(alpha* floatMax) 
      };
    }

    constexpr static auto White() -> Color {
      return Color{ Red_t{1.0f}, Green_t{1.0f}, Blue_t{1.0f}, Alpha_t{} };
    }

    constexpr static auto Black() -> Color {
      return Color{ Red_t{}, Green_t{}, Blue_t{}, Alpha_t{} };
    }

    constexpr static auto Red() -> Color {
      return Color{ Red_t{1.0f}, Green_t{}, Blue_t{}, Alpha_t{} };
    }

    constexpr static auto Green() -> Color {
      return Color{ Red_t{}, Green_t{1.0f}, Blue_t{}, Alpha_t{} };
    }

    constexpr static auto Blue() -> Color {
      return Color{ Red_t{}, Green_t{}, Blue_t{1.0f}, Alpha_t{} };
    }

    constexpr static auto Blank() -> Color {
      return Color{ Red_t{}, Green_t{}, Blue_t{}, Alpha_t{0.0f} };
    }

    std::common_type_t<uint8_t, float> red{}, green{}, blue{}, alpha = 1.0f;
  };

  enum class LuaPrivledges {
    None = 0,
    UserInput = 1,
    WindowControl = 2,
    Fonts = 4,
    All = 15,
  };

  // Enum class operators.

  template<typename T>
  concept ScopedEnum = std::is_scoped_enum_v<T>;

  template<ScopedEnum Enum>
  [[nodiscard]] constexpr auto operator bitand(const Enum& lhs, const Enum& rhs) -> Enum {
    return Enum{std::to_underlying(lhs) bitand std::to_underlying(rhs)};
  }

  template<ScopedEnum Enum>
  [[nodiscard]] constexpr auto operator bitor(const Enum& lhs, const Enum& rhs) -> Enum {
    return Enum{std::to_underlying(lhs) bitor std::to_underlying(rhs)};
  }

  [[nodiscard]] constexpr auto operator bitand(const LuaPrivledges& lhs, const LuaPrivledges& rhs) -> LuaPrivledges {
    return LuaPrivledges{std::to_underlying(lhs) bitand std::to_underlying(rhs)};
  }

  [[nodiscard]] constexpr auto operator bitor(const LuaPrivledges& lhs, const LuaPrivledges& rhs) -> LuaPrivledges {
    return LuaPrivledges{std::to_underlying(lhs) bitor std::to_underlying(rhs)};
  }

} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_UTILITY_HPP_
