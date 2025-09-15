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

#include <cstdint>
#include <filesystem>
#include <map>
#include <ranges>
#include <string>
#include <utility>

#include "SDL3_ttf/SDL_ttf.h"

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

  constexpr inline float defaultFontSize = 16.0f;

  /**
      @brief A light wrapper for TTF_Font pointers.

      Because of the ambiguous nature of the TTF_Font type, sol3 is unable
      to determine, at compile-time, what the full definition is. As a result,
      binding TTF_Font pointers directly to Lua does not work.

      Having this light wrapper satisfies sol3's requirement for complete types.
   */
  struct Font {
    TTF_Font* ptr = nullptr;
  };

  /**
      @brief This is the primary container class for SDL_ttf fonts.
   */
  class FontGroup {
  public:
    auto LoadDefaultFont() -> bool;

    // Load font from a .ttf file.
    auto AddFont(const std::filesystem::path& filename) -> bool;

    // Used internally, do not call.
    void ClearFonts() const;

    [[nodiscard]] auto GetDefaultFont() const -> Font {
      return _ttfFonts.at(SWGTK_DEFAULT_FONT_ID);
    }

    static void SetFontStyle(const Font font, const FontStyle style) {
      TTF_SetFontStyle(font.ptr, std::to_underlying(style));
    }

    [[nodiscard]] static auto GetFontStyle(const Font font) -> FontStyle { return FontStyle{TTF_GetFontStyle(font.ptr)}; }

    [[nodiscard]] auto SetFontSize(const std::string& name, const float size) const -> bool {
      if (_ttfFonts.contains(name)) {
        return TTF_SetFontSize(_ttfFonts.at(name).ptr, size);
      }

      return false;
    }

    void SetAllFontSizes(const float size) const {
      for (const auto [ptr]: _ttfFonts | std::views::values) {
        TTF_SetFontSize(ptr, size);
      }
    }

    constexpr void SetDefaultFontSize(const float size) { _defaultFontSize = size; }

    [[nodiscard]] auto GetFont(const std::string& name) const -> Font {
      if (_ttfFonts.contains(name)) {
        return _ttfFonts.at(name);
      }

      return Font{};
    }

  private:
    std::map<std::string, Font> _ttfFonts;
    float _defaultFontSize = defaultFontSize;
  };
} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_TTFFONT_HPP_
