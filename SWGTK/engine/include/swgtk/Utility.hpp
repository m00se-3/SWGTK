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
  class object_ref {
  public:
    object_ref() = default;
    explicit object_ref(Ptr* ptr) : _ptr(ptr) {}
    auto operator->() const -> Ptr* { return _ptr; }
    explicit operator bool() const { return _ptr != nullptr; }

  private:
    Ptr* _ptr = nullptr;
  };

  // Items for internal use only.
  namespace detail {
    template<typename Ptr>
    class non_owning {
    public:
      constexpr non_owning() = default;
      constexpr explicit non_owning(Ptr* ptr) : _ptr(ptr) {}
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

  struct red_v {
    constexpr red_v() = default;
    constexpr explicit red_v(const float v) : value(v) {}
    constexpr explicit red_v(const uint8_t v) : value(v) {}

    std::common_type_t<uint8_t, float> value{};
  };

  struct green_v {
    constexpr green_v() = default;
    constexpr explicit green_v(const float v) : value(v) {}
    constexpr explicit green_v(const uint8_t v) : value(v) {}

    std::common_type_t<uint8_t, float> value{};
  };

  struct blue_v {
    constexpr blue_v() = default;
    constexpr explicit blue_v(const float v) : value(v) {}
    constexpr explicit blue_v(const uint8_t v) : value(v) {}

    std::common_type_t<uint8_t, float> value{};
  };

  struct alpha_v {
    constexpr alpha_v() = default;
    constexpr explicit alpha_v(const float v) : value(v) {}
    constexpr explicit alpha_v(const uint8_t v) : value(v) {}

    std::common_type_t<uint8_t, float> value = 1.0f;
  };

  struct color_t {
    constexpr static auto float_max = 255.0f;
    constexpr color_t() = default;

    constexpr explicit color_t(const SDL_Color& color)
    : r(static_cast<float>(color.r) / float_max), g(static_cast<float>(color.g) / float_max),
    b(static_cast<float>(color.b) / float_max), a(static_cast<float>(color.a) / float_max) {}

    constexpr explicit color_t(const SDL_FColor& color)
    : r(color.r), g(color.g), b(color.b), a(color.a) {}

    constexpr color_t(const red_v r, const green_v g, const blue_v b, const alpha_v a = alpha_v{1.0f})
    : r(r.value), g(g.value), b(b.value), a(a.value) {}

    constexpr explicit operator SDL_FColor() const {
      return SDL_FColor{ .r = r, .g = g, .b = b, .a = a };
    }

    constexpr explicit operator SDL_Color() const {
      return SDL_Color{ 
        .r = static_cast<uint8_t>(r * float_max),
        .g = static_cast<uint8_t>(g * float_max),
        .b = static_cast<uint8_t>(b * float_max),
        .a = static_cast<uint8_t>(a* float_max)
      };
    }

    constexpr static auto white() -> color_t {
      return color_t{ red_v{1.0f}, green_v{1.0f}, blue_v{1.0f}, alpha_v{} };
    }

    constexpr static auto black() -> color_t {
      return color_t{ red_v{}, green_v{}, blue_v{}, alpha_v{} };
    }

    constexpr static auto red() -> color_t {
      return color_t{ red_v{1.0f}, green_v{}, blue_v{}, alpha_v{} };
    }

    constexpr static auto green() -> color_t {
      return color_t{ red_v{}, green_v{1.0f}, blue_v{}, alpha_v{} };
    }

    constexpr static auto blue() -> color_t {
      return color_t{ red_v{}, green_v{}, blue_v{1.0f}, alpha_v{} };
    }

    constexpr static auto blank() -> color_t {
      return color_t{ red_v{}, green_v{}, blue_v{}, alpha_v{0.0f} };
    }

    std::common_type_t<uint8_t, float> r{}, g{}, b{}, a = 1.0f;
  };

  enum class lua_privledges {
    none = 0,
    user_input = 1,
    window_control = 2,
    fonts = 4,
    all = 15,
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

  [[nodiscard]] constexpr auto operator bitand(const lua_privledges& lhs, const lua_privledges& rhs) -> lua_privledges {
    return lua_privledges{std::to_underlying(lhs) bitand std::to_underlying(rhs)};
  }

  [[nodiscard]] constexpr auto operator bitor(const lua_privledges& lhs, const lua_privledges& rhs) -> lua_privledges {
    return lua_privledges{std::to_underlying(lhs) bitor std::to_underlying(rhs)};
  }

} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_UTILITY_HPP_
