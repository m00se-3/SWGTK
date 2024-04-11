#include "Font.hpp"

namespace swgtk
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

    void FontGroup::AddFont(FontStyle styleMask, int size, const std::filesystem::path& filename)
    {
        const auto hash = Hash(styleMask, size);
        
        if (!_nkFonts.contains(hash))
        {
            auto str = filename.string();
            
            struct nk_font* temp = nk_font_atlas_add_from_file(&_atlas, str.c_str(), static_cast<float>(size), nullptr);
            TTF_Font* ttf = TTF_OpenFont(str.c_str(), size);

            if (temp != nullptr) 
            {
                _nkFonts.insert_or_assign(hash, temp);
            }

            if (ttf != nullptr) 
            {
                _ttfFonts.insert_or_assign(hash, ttf);
            }
        }
    }

    void FontGroup::ClearTTFFonts()
    {
        for (auto& font : _ttfFonts)
        {
            TTF_CloseFont(font.second);
        }
    }

    nk_font* FontGroup::GetNK(FontStyle mask, int size)
    {
        const auto key = Hash(mask, size);
            
        if (_nkFonts.contains(key))
        {
            return _nkFonts.at(key);
        }

        return nullptr;
    }

    const nk_font* FontGroup::GetNK(FontStyle mask, int size) const
    {
        const auto key = Hash(mask, size);
        
        if (_nkFonts.contains(key))
        {
            return _nkFonts.at(key);
        }

        return nullptr;
    }

    TTF_Font* FontGroup::GetTTF(FontStyle mask, int size)
    {
        const auto key = Hash(mask, size);
        
        if (_ttfFonts.contains(key))
        {
            return _ttfFonts.at(key);
        }

        return nullptr;
    }

    const TTF_Font* FontGroup::GetTTF(FontStyle mask, int size) const
    {
        const auto key = Hash(mask, size);
        
        if (_ttfFonts.contains(key))
        {
            return _ttfFonts.at(key);
        }

        return nullptr;
    }

    [[nodiscard]] nk_font_atlas* FontGroup::GetAtlas() { return &_atlas; }

    [[nodiscard]] const nk_font_atlas* FontGroup::GetAtlas() const { return &_atlas; }
}