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
#include "swgtk/FontGroup.hpp"
#include <swgtk/Utility.hpp>
#include <utility>
#include <string>

namespace swgtk {

    bool FontGroup::LoadDefaultFont() {
        if (const auto filePath = std::filesystem::path{ SWGTK_DEFAULT_FONT_FILE }; std::filesystem::exists(filePath)) {
            const auto fileString = filePath.string();
            
            if (TTF_Font* ttf = TTF_OpenFont(fileString.c_str(), _defaultFontSize); ttf == nullptr)
            {
                DEBUG_PRINT2("Error opening font file {}: {}\n", fileString, SDL_GetError());
            } else {
                _ttfFonts.insert_or_assign(SWGTK_DEFAULT_FONT_ID, Font{ .ptr=ttf });
                return true;
            }
        }
        
        return false;
    }
    
    bool FontGroup::AddFont(const std::filesystem::path& filename) {
        if (const auto fileString = filename.string(); !_ttfFonts.contains(fileString))
        {
            if (TTF_Font* ttf = TTF_OpenFont(fileString.c_str(), _defaultFontSize); ttf == nullptr)
            {
                DEBUG_PRINT2("Error opening font file {}: {}\n", fileString, SDL_GetError());
            } else {
                _ttfFonts.insert_or_assign(filename.stem().string(), Font{ .ptr=ttf });
                return true;
            }
        }

        return false;
    }

    void FontGroup::ClearFonts() const {
        for (const auto [ptr] : _ttfFonts | std::views::values)
        {
            TTF_CloseFont(ptr);
        }
    }

} // namespace swgtk
