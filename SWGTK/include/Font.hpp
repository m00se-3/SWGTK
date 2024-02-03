#ifndef SWGTK_FONT_HPP
#define SWGTK_FONT_HPP

#include <filesystem>
#include <unordered_map> 

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO

#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_UINT_DRAW_INDEX
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_FIXED_TYPES

// We have to explicitly undefine NK_IMPLEMENTATION here to avoid linking errors.
#undef NK_IMPLEMENTATION
#include "nuklear/nuklear.h"
#include "SDL2/SDL_ttf.h"

namespace swgtk
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
        FontGroup();
        ~FontGroup();

        void Create();
        void Finalize(SDL_Texture* texture);
        void AddFont(FontStyle styleMask, float size, const std::filesystem::path& filename);
        void ClearTTFFonts();

        [[nodiscard]] nk_font* GetNK(FontStyle mask);
        [[nodiscard]] const nk_font* GetNK(FontStyle mask) const;
        [[nodiscard]] TTF_Font* GetTTF(FontStyle mask);
        [[nodiscard]] const TTF_Font* GetTTF(FontStyle mask) const;
        [[nodiscard]] nk_font_atlas* GetAtlas();
        [[nodiscard]] const nk_font_atlas* GetAtlas() const;

    private:
        struct nk_font_atlas _atlas;
        std::unordered_map<FontStyle, struct nk_font*> _nkFonts;
        std::unordered_map<FontStyle, TTF_Font*> _ttfFonts;
    };
}

#endif