#include "swgtk/TTFFont.hpp"
#include <swgtk/Utility.hpp>

namespace swgtk
{    
    static constexpr auto DefaultFontRootDir = std::string_view{ SWGTK_DEFAULT_FONT_DIR };

    void FontGroup::LoadDefaultFont() {
        if (const auto filePath = std::filesystem::path{ DefaultFontRootDir } / "Natural Mono-Regular.ttf"; std::filesystem::exists(filePath)) {
            const auto fileString = filePath.string();
            
            if (TTF_Font* ttf = TTF_OpenFont(fileString.c_str(), _defaultFontSize); ttf == nullptr)
            {
                DEBUG_PRINT2("Error opening font file {}: {}\n", fileString, SDL_GetError());
            }
            else {
                _ttfFonts.insert_or_assign(filePath.stem().string(), ttf);
            }
        }
    }
    
    void FontGroup::AddFont(const std::filesystem::path& filename) {
        if (const auto fileString = filename.string(); !_ttfFonts.contains(fileString))
        {
            if (TTF_Font* ttf = TTF_OpenFont(fileString.c_str(), _defaultFontSize); ttf == nullptr)
            {
                DEBUG_PRINT2("Error opening font file {}: {}\n", fileString, SDL_GetError());
            } else {
                _ttfFonts.insert_or_assign(filename.stem().string(), ttf);
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
