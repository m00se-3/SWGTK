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
#include <string>
#include <swgtk/Utility.hpp>
#include <utility>
#include "swgtk/Font.hpp"

namespace swgtk {

  auto Font::LoadDefault() -> Font {
    if (const auto filePath = std::filesystem::path{SWGTK_DEFAULT_FONT_FILE}; std::filesystem::exists(filePath)) {
      const auto fileString = filePath.string();

      if (TTF_Font* ttf = TTF_OpenFont(fileString.c_str(), detail::defaultFontSize); ttf != nullptr) {
        return Font{ttf};
      }
    }

    DEBUG_PRINT("Error finding or loading font file {}\n", SWGTK_DEFAULT_FONT_FILE);
    return Font{};
  }

  Font::Font(const std::filesystem::path& path, const float fontSize) {
    const auto fileString = path.string();
    if ( _ptr = detail::FontHandle{ TTF_OpenFont(fileString.c_str(), fontSize) }; _ptr == nullptr) {
        DEBUG_PRINT2("Error opening font file {}: {}\n", fileString, SDL_GetError());
    }
  }


} // namespace swgtk
