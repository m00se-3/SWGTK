#ifndef SWGTK_TTF_FONT_HPP
#define SWGTK_TTF_FONT_HPP

#include <filesystem>
#include <map>
#include <cstdint>
#include "SDL3_ttf/SDL_ttf.h"

namespace swgtk
{
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

    constexpr static float defaultFontSize = 16.0f;

    
    class FontGroup {
    public:
        void LoadDefaultFont();
        void AddFont(const std::filesystem::path& filename);
        void ClearFonts() const;

        [[nodiscard]] TTF_Font* GetDefaultFont() const { return _ttfFonts.at("Natural Mono-Regular"); }

        static void SetFontStyle(TTF_Font* font, const FontStyle style) {
            TTF_SetFontStyle(font, std::to_underlying(style));
        }

        [[nodiscard]] static FontStyle GetFontStyle(TTF_Font* font) { return FontStyle{ TTF_GetFontStyle(font)}; }

        [[nodiscard]] bool SetFontSize(const std::string& name, const float size) const {
            if(_ttfFonts.contains(name)) {
                return TTF_SetFontSize(_ttfFonts.at(name), size);
            }

            return false;
         }

         void SetAllFontSizes(const float size) const {
            for(const auto&[fst, ptr] : _ttfFonts) {
                TTF_SetFontSize(ptr, size);
            }
         }

        constexpr void SetDefaultFontSize(const float size) { _defaultFontSize = size; };

        [[nodiscard]] constexpr TTF_Font* GetFont(this auto&& self, const std::string& name) {
            if (self._ttfFonts.contains(name)) {
                return self._ttfFonts.at(name);
            }

            return nullptr;
        }

    private:
        std::map<std::string, TTF_Font*> _ttfFonts;
        float _defaultFontSize = defaultFontSize;
    };
}

#endif // !SWGTK_TTF_FONT_HPP
