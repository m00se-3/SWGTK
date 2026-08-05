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
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_TTFFONT_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_TTFFONT_HPP_

#include <filesystem>
#include <utility>

#include <SDL3_ttf/SDL_ttf.h>

namespace swgtk {
  // Wrapper for TTF_STYLE_* enum.
  enum class FontStyle : uint32_t {
    None = 32u,
    Normal = TTF_STYLE_NORMAL,
    Bold = TTF_STYLE_BOLD,
    Italic = TTF_STYLE_ITALIC,
    Underlined = TTF_STYLE_UNDERLINE,
    Strikethrough = TTF_STYLE_STRIKETHROUGH,
    Bold_Italic = Bold | Italic,
    Bold_Strike = Bold | Strikethrough,
    Bold_Underlined = Bold | Underlined,
    Bold_Italic_Underlined = Bold | Italic | Underlined,
    Bold_Italic_Strike = Bold | Italic | Strikethrough,
    Italic_Underlined = Italic | Underlined,
    Italic_Strike = Italic | Strikethrough,
  };

  namespace detail {
    constexpr inline float defaultFontSize = 16.0f;
  }

  /**
      @brief This is the primary container class for SDL_ttf fonts.
   */
  class Font {
  public:
    using FontHandle = std::unique_ptr<TTF_Font, decltype([](TTF_Font* font) -> void { if (font != nullptr) { TTF_CloseFont(font); } })>;
    Font() = default;
    explicit Font(TTF_Font* font) : _ptr{font} {}
    explicit Font(const std::filesystem::path& path, float fontSize = detail::defaultFontSize);

    [[nodiscard]] static auto LoadDefault() -> Font;
    [[nodiscard]] auto IsEmpty() const -> bool { return _ptr == nullptr; }
    [[nodiscard]] auto Get() const noexcept -> TTF_Font* { return _ptr.get(); }
    [[nodiscard]] auto operator*() const noexcept -> TTF_Font* { return _ptr.get(); }

    [[nodiscard]] auto GetStyle() const noexcept -> FontStyle { return FontStyle{ TTF_GetFontStyle(_ptr.get()) }; }
    auto SetStyle(const FontStyle style) const -> void { TTF_SetFontStyle(_ptr.get(), std::to_underlying(style)); }

  private:
    FontHandle _ptr;
  };

} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_TTFFONT_HPP_
