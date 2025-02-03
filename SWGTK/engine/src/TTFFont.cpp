#include "swgtk/TTFFont.hpp"
#include <swgtk/Utility.hpp>

namespace swgtk
{    
    void FontGroup::AddFont(const std::filesystem::path& filename, FontStyle styleMask) {
        if (!_ttfFonts.contains(styleMask))
        {
            auto str = filename.string();
            
            TTF_Font* ttf = TTF_OpenFont(str.c_str(), _defaultFontSize);

            if (ttf == nullptr) 
            {
                DEBUG_PRINT2("Error opening font file {}: {}\n", filename.filename().string(), SDL_GetError());
            } else {
                _ttfFonts.insert_or_assign(styleMask, ttf);
            }

        }
    }

    void FontGroup::ClearFonts() {
        for (auto& font : _ttfFonts)
        {
            TTF_CloseFont(font.second);
        }
    }

}
