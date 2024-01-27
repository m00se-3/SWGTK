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
        if (!_nkFonts.contains(styleMask))
        {
            auto str = filename.string();
            
            struct nk_font* temp = nk_font_atlas_add_from_file(&_atlas, str.c_str(), size, nullptr);
            TTF_Font* ttf = TTF_OpenFont(str.c_str(), static_cast<int>(size));

            if (temp) _nkFonts.insert_or_assign(styleMask, temp);
            if (ttf) _ttfFonts.insert_or_assign(styleMask, ttf);
        }
    }

    void FontGroup::ClearTTFFonts()
    {
        for (auto& font : _ttfFonts)
        {
            TTF_CloseFont(font.second);
        }
    }

    nk_font* FontGroup::GetNK(FontStyle mask)
    {
        if (_nkFonts.contains(mask))
        {
            return _nkFonts.at(mask);
        }

        return nullptr;
    }

    const nk_font* FontGroup::GetNK(FontStyle mask) const
    {
        if (_nkFonts.contains(mask))
        {
            return _nkFonts.at(mask);
        }

        return nullptr;
    }

    TTF_Font* FontGroup::GetTTF(FontStyle mask)
    {
        if (_ttfFonts.contains(mask))
        {
            return _ttfFonts.at(mask);
        }

        return nullptr;
    }

    const TTF_Font* FontGroup::GetTTF(FontStyle mask) const
    {
        if (_ttfFonts.contains(mask))
        {
            return _ttfFonts.at(mask);
        }

        return nullptr;
    }

    [[nodiscard]] nk_font_atlas* FontGroup::GetAtlas() { return &_atlas; }

    [[nodiscard]] const nk_font_atlas* FontGroup::GetAtlas() const { return &_atlas; }
}