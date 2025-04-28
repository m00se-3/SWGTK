/*
    MIT License
    Copyright (c) 2023 Samuel Bridgham

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#include "swgtk/TTFFont.hpp"
#include <swgtk/Utility.hpp>
#include <utility>
#include <string>

namespace swgtk {    
    static constexpr auto DefaultFontRootDir = std::string_view{ SWGTK_DEFAULT_FONT_DIR };

    void FontGroup::LoadDefaultFont() {
        if (const auto filePath = std::filesystem::path{ DefaultFontRootDir } / "Natural Mono-Regular.ttf"; std::filesystem::exists(filePath)) {
            const auto fileString = filePath.string();
            
            if (TTF_Font* ttf = TTF_OpenFont(fileString.c_str(), _defaultFontSize); ttf == nullptr)
            {
                DEBUG_PRINT2("Error opening font file {}: {}\n", fileString, SDL_GetError());
            } else {
                _ttfFonts.insert_or_assign(filePath.stem().string(), Font{ .ptr=ttf });
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
                _ttfFonts.insert_or_assign(filename.stem().string(), Font{ .ptr=ttf });
            }

        }
    }

    void FontGroup::ClearFonts() const {
        for (const auto& font : _ttfFonts | std::views::values)
        {
            TTF_CloseFont(font.ptr);
        }
    }

    void FontGroup::InitLua(sol::state& lua) {
        auto SWGTK = lua["swgtk"];

        lua.new_enum<FontStyle>("FontStyle",
            {
                std::make_pair("Normal", FontStyle::Normal),
                std::make_pair("Bold", FontStyle::Bold),
                std::make_pair("Italic", FontStyle::Italic),
                std::make_pair("Underlined", FontStyle::Underlined),
                std::make_pair("Bold_Italic", FontStyle::Bold_Italic),
                std::make_pair("Bold_Underlined", FontStyle::Bold_Underlined),
                std::make_pair("Bold_Italic_Underlined", FontStyle::Bold_Italic_Underlined),
                std::make_pair("Italic_Underlined", FontStyle::Italic_Underlined)
            }
        );

        SWGTK["FontStyle"] = lua["FontStyle"];

        auto FontGroup_Type = lua.new_usertype<FontGroup>("FontGroup", sol::constructors<FontGroup()>());
        SWGTK["Fonts"] = this;

        auto Font_Type = lua.new_usertype<Font>("FontHandle");

        FontGroup_Type["GetDefaultFont"] = [] (const FontGroup& self) { return self.GetDefaultFont(); };

        FontGroup_Type["SetDefaultFontSize"] = [] (FontGroup& self, const float size) { self.SetDefaultFontSize(size); };

        FontGroup_Type["SetAllFontSizes"] = [] (const FontGroup& self, const float size) { self.SetAllFontSizes(size); };

        FontGroup_Type["AddFont"] = [] (FontGroup& self, const std::filesystem::path& filename) { self.AddFont(filename); };

        FontGroup_Type["GetFont"] = [] (const FontGroup& self, const std::string& name) { return self.GetFont(name); };

        FontGroup_Type["SetFontStyle"] = [] (const Font font, const FontStyle style) { SetFontStyle(font, style); };

        FontGroup_Type["GetFontStyle"] = [] (const Font font) { return GetFontStyle(font); };

        FontGroup_Type["ClearFonts"] = [] (const FontGroup& self) { self.ClearFonts(); };
    }

} // namespace swgtk
