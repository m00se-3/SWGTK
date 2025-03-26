#include "swgtk/TTFFont.hpp"
#include <swgtk/Utility.hpp>

namespace swgtk
{    
    void FontGroup::AddFont(const std::filesystem::path& filename, const FontStyle styleMask) {
        if (!_ttfFonts.contains(styleMask))
        {
            const auto str = filename.string();

            if (TTF_Font* ttf = TTF_OpenFont(str.c_str(), _defaultFontSize); ttf == nullptr)
            {
                DEBUG_PRINT2("Error opening font file {}: {}\n", filename.filename().string(), SDL_GetError());
            } else {
                _ttfFonts.insert_or_assign(styleMask, ttf);
            }

        }
    }

    void FontGroup::ClearFonts() const {
        for (const auto&[style, font] : _ttfFonts)
        {
            TTF_CloseFont(font);
        }
    }

}
