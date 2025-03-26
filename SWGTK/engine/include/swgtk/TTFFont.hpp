#ifndef SWGTK_TTF_FONT_HPP
#define SWGTK_TTF_FONT_HPP

#include <filesystem>
#include <unordered_map>
#include <cstdint>
#include "SDL3_ttf/SDL_ttf.h"

namespace swgtk
{
    enum class FontStyle : int32_t {
        None = 32,
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

    constexpr static float defaultFontSize = 16.0f;

    /**
     * @brief A FontGroup represents a single font family. (e.g. Sans, Roboto, etc.)
     * This class contains an unordered_map of styles to use when rendering.(See enum class FontStyle)
     * 
     */
    class FontGroup {
    public:
        void AddFont(const std::filesystem::path& filename, FontStyle styleMask);
        void ClearFonts() const;
        [[nodiscard]] bool SetFontSize(const FontStyle style, const float size) const {
            if(_ttfFonts.contains(style)) {
                return TTF_SetFontSize(_ttfFonts.at(style), size);
            }

            return false;
         }

         void SetAllFontSizes(const float size) const {
            for(const auto&[fst, snd] : _ttfFonts) {
                TTF_SetFontSize(snd, size);
            }
         }

        constexpr void SetDefaultFontSize(const float size) { _defaultFontSize = size; };

        [[nodiscard]] constexpr TTF_Font* GetFont(this auto&& self, FontStyle mask) {
            if (self._ttfFonts.contains(mask))
            {
                return self._ttfFonts.at(mask);
            }

            return nullptr;
        }

    private:
        std::unordered_map<FontStyle, TTF_Font*> _ttfFonts;
        float _defaultFontSize = defaultFontSize;
    };
}

#endif // !SWGTK_TTF_FONT_HPP
