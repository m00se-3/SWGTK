#include "Font.hpp"

namespace cts
{
    FontGroup::FontGroup()
    {
        nk_font_atlas_init_default(&_atlas);
    }

    FontGroup::~FontGroup()
    {
        nk_font_atlas_cleanup(&_atlas);
        nk_font_atlas_clear(&_atlas);
    }

    void FontGroup::Create()
    {
        nk_font_atlas_begin(&_atlas);
    }

    void FontGroup::Finalize(SDL_Texture* texture)
    {
        nk_font_atlas_end(&_atlas, nk_handle_ptr(texture), nullptr);
    }

    void FontGroup::AddFont(FontStyle styleMask, float size, const std::filesystem::path& filename)
    {
        if (!_fonts.contains(styleMask))
        {
            struct nk_font* temp = nk_font_atlas_add_from_file(&_atlas, filename.string().c_str(), size, nullptr);
            if (temp) _fonts.insert_or_assign(styleMask, temp);
        }
    }

    nk_font* FontGroup::GetFont(FontStyle mask)
    {
        if (_fonts.contains(mask))
        {
            return _fonts.at(mask);
        }

        return nullptr;
    }

    const nk_font* FontGroup::GetFont(FontStyle mask) const
    {
        if (_fonts.contains(mask))
        {
            return _fonts.at(mask);
        }

        return nullptr;
    }

    [[nodiscard]] nk_font_atlas* FontGroup::GetAtlas() { return &_atlas; }

    [[nodiscard]] const nk_font_atlas* FontGroup::GetAtlas() const { return &_atlas; }
}