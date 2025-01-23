#include "swgtk/TTFFont.hpp"

namespace swgtk::sdl
{    
    void FontGroup::AddFont(FontStyle styleMask, const std::filesystem::path& filename) {
        if (!_ttfFonts.contains(styleMask))
        {
            auto str = filename.string();
            
            TTF_Font* ttf = TTF_OpenFont(str.c_str(), _defaultFontSize);

            if (ttf != nullptr) 
            {
                _ttfFonts.insert_or_assign(styleMask, ttf);
            }
        }
    }

    void FontGroup::ClearTTFFonts() {
        for (auto& font : _ttfFonts)
        {
            TTF_CloseFont(font.second);
        }
    }

    TTF_Font* FontGroup::GetTTF(FontStyle mask) {
        if (_ttfFonts.contains(mask))
        {
            return _ttfFonts.at(mask);
        }

        return nullptr;
    }

    const TTF_Font* FontGroup::GetTTF(FontStyle mask) const {
        if (_ttfFonts.contains(mask))
        {
            return _ttfFonts.at(mask);
        }

        return nullptr;
    }

}
