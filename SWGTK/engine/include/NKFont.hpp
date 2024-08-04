#ifndef SWGTK_FONT_HPP
#define SWGTK_FONT_HPP

#include <filesystem>
#include <unordered_map> 

#define NK_INCLUDE_DEFAULT_FONT
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO

#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_UINT_DRAW_INDEX
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_FIXED_TYPES

// We have to explicitly undefine NK_IMPLEMENTATION here to avoid linking errors.
#undef NK_IMPLEMENTATION
#include "nuklear.h"

#include "SDL_render.h"

namespace swgtk::nk
{
    enum class FontStyle : int32_t
    {
        None = 32,
        Normal = 1,
        Bold = 2,
        Italic = 4,
        Underlined = 8,
        Strikethrough = 16,
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

        FontGroup(const FontGroup&) = delete;
        FontGroup(FontGroup&&) = delete;
        FontGroup& operator=(const FontGroup&) = delete;
        FontGroup& operator=(FontGroup&&) = delete;

        void Create();
        void Finalize(SDL_Texture* texture);
        void AddFont(FontStyle styleMask, int size, const std::filesystem::path& filename);

        [[nodiscard]] nk_font* GetNK(FontStyle mask, int size);
        [[nodiscard]] const nk_font* GetNK(FontStyle mask, int size) const;
        [[nodiscard]] nk_font_atlas* GetAtlas();
        [[nodiscard]] const nk_font_atlas* GetAtlas() const;

    private:
        [[nodiscard]] constexpr static int64_t Hash(FontStyle style, int size)
        {
            constexpr const int styleOffset = 32;
            return (int64_t)style << styleOffset | (int64_t)size;
        }

        struct nk_font_atlas _atlas{};
        std::unordered_map<int64_t, struct nk_font*> _nkFonts;
    };
}

#endif
