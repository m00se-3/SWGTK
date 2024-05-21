#ifndef SWGTK_TTF_FONT_HPP
#define SWGTK_TTF_FONT_HPP

#include <filesystem>
#include <unordered_map>
#include <cstdint>
#include "SDL2/SDL_ttf.h"

namespace swgtk::sdl
{
    enum class FontStyle : int32_t
    {
        None = 32,
        Normal = TTF_STYLE_NORMAL,
        Bold = TTF_STYLE_BOLD,
        Italic = TTF_STYLE_ITALIC,
        Underlined = TTF_STYLE_UNDERLINE,
        Strikethrough = TTF_STYLE_STRIKETHROUGH,
        Bold_Italic = Bold | Italic,
        Bold_Strike = Bold | Strikethrough,
        Bold_Underlinded = Bold | Underlined,
        Bold_Italic_Underlined = Bold | Italic | Underlined,
        Bold_Italic_Strike = Bold | Italic | Strikethrough,
        Italic_Underlined = Italic | Underlined,
        Italic_Strike = Italic | Strikethrough,
    };

    class FontGroup
    {
    public:
        FontGroup() = default;
        ~FontGroup() = default;

        FontGroup(const FontGroup&) = delete;
        FontGroup(FontGroup&&) = delete;
        FontGroup& operator=(const FontGroup&) = delete;
        FontGroup& operator=(FontGroup&&) = delete;

        void AddFont(FontStyle styleMask, int size, const std::filesystem::path& filename);
        void ClearTTFFonts();

        [[nodiscard]] TTF_Font* GetTTF(FontStyle mask, int size);
        [[nodiscard]] const TTF_Font* GetTTF(FontStyle mask, int size) const;

    private:
        [[nodiscard]] constexpr static int64_t Hash(FontStyle style, int size)
        {
            constexpr const int styleOffset = 32;
            return (int64_t)style << styleOffset | (int64_t)size;
        }

        std::unordered_map<int64_t, TTF_Font*> _ttfFonts;
    };
}

#endif // !SWGTK_TTF_FONT_HPP
