#ifndef CTS_FONT_HPP
#define CTS_FONT_HPP

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

extern "C"
{
    struct SDL_Texture;
}

namespace cts
{
    enum class FontStyle : uint32_t
    {
        None = 0u,
        Normal = 1u,
        Bold = 2u,
        Italic = 4u,
        Underlined = 8u,
        BoldItalic = Bold | Italic,
        BoldUnderlinded = Bold | Underlined,
        BolItalicUnderlined = Bold | Italic | Underlined,
        ItalicUnderlined = Italic | Underlined,
    };

    class FontGroup
    {
    public:
        FontGroup();
        ~FontGroup();

        void Create();
        void Finalize(SDL_Texture* texture);
        void AddFont(FontStyle styleMask, float size, const std::filesystem::path& filename);

        [[nodiscard]] nk_font* GetFont(FontStyle mask);
        [[nodiscard]] const nk_font* GetFont(FontStyle mask) const;
        [[nodiscard]] nk_font_atlas* GetAtlas();
        [[nodiscard]] const nk_font_atlas* GetAtlas() const;

    private:
        struct nk_font_atlas _atlas;
        std::unordered_map<FontStyle, struct nk_font*> _fonts;
    };
}

#endif