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
  enum class font_style : uint32_t {
    none = 32u,
    normal = TTF_STYLE_NORMAL,
    bold = TTF_STYLE_BOLD,
    italic = TTF_STYLE_ITALIC,
    underlined = TTF_STYLE_UNDERLINE,
    strikethrough = TTF_STYLE_STRIKETHROUGH,
    bold_italic = bold | italic,
    bold_strike = bold | strikethrough,
    bold_underlined = bold | underlined,
    bold_italic_underlined = bold | italic | underlined,
    bold_italic_strike = bold | italic | strikethrough,
    italic_underlined = italic | underlined,
    italic_strike = italic | strikethrough,
  };

  namespace detail {
    constexpr inline float default_font_size = 16.0f;
  }

  /**
      @brief This is the primary container class for SDL_ttf fonts.
   */
  class font {
  public:
    using FontHandle = std::unique_ptr<TTF_Font, decltype([](TTF_Font* font) -> void { if (font != nullptr) { TTF_CloseFont(font); } })>;
    font() = default;
    explicit font(TTF_Font* font) : _ptr{font} {}
    explicit font(const std::filesystem::path& path, float fontSize = detail::default_font_size);

    [[nodiscard]] static auto load_default() -> font;
    [[nodiscard]] auto is_empty() const -> bool { return _ptr == nullptr; }
    [[nodiscard]] auto get() const noexcept -> TTF_Font* { return _ptr.get(); }
    [[nodiscard]] auto operator*() const noexcept -> TTF_Font* { return _ptr.get(); }

    [[nodiscard]] auto get_style() const noexcept -> font_style { return font_style{ TTF_GetFontStyle(_ptr.get()) }; }
    auto set_style(const font_style style) const -> void { TTF_SetFontStyle(_ptr.get(), std::to_underlying(style)); }

  private:
    FontHandle _ptr;
  };

} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_TTFFONT_HPP_
