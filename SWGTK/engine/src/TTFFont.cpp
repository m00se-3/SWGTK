#include "TTFFont.hpp"

namespace swgtk::sdl
{    
    void FontGroup::AddFont(FontStyle styleMask, float size, const std::filesystem::path& filename) {
        const auto hash = Hash(styleMask, size);
        
        if (!_ttfFonts.contains(hash))
        {
            auto str = filename.string();
            
            TTF_Font* ttf = TTF_OpenFont(str.c_str(), size);

            if (ttf != nullptr) 
            {
                _ttfFonts.insert_or_assign(hash, ttf);
            }
        }
    }

    void FontGroup::ClearTTFFonts() {
        for (auto& font : _ttfFonts)
        {
            TTF_CloseFont(font.second);
        }
    }

    TTF_Font* FontGroup::GetTTF(FontStyle mask, int size) {
        const auto key = Hash(mask, size);
        
        if (_ttfFonts.contains(key))
        {
            return _ttfFonts.at(key);
        }

        return nullptr;
    }

    const TTF_Font* FontGroup::GetTTF(FontStyle mask, int size) const {
        const auto key = Hash(mask, size);
        
        if (_ttfFonts.contains(key))
        {
            return _ttfFonts.at(key);
        }

        return nullptr;
    }

}
