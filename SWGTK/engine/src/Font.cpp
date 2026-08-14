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
#include "swgtk/Font.hpp"

namespace swgtk {

  auto font::load_default() -> font {
    if (const auto file_path = std::filesystem::path{SWGTK_DEFAULT_FONT_FILE}; std::filesystem::exists(file_path)) {
      const auto file_string = file_path.string();

      if (TTF_Font* ttf = TTF_OpenFont(file_string.c_str(), detail::default_font_size); ttf != nullptr) {
        return font{ttf};
      }
    }

    DEBUG_PRINT("Error finding or loading font file {}\n", SWGTK_DEFAULT_FONT_FILE);
    return font{};
  }

  font::font(const std::filesystem::path& path, const float fontSize) {
    const auto file_string = path.string();
    if ( _ptr = FontHandle{ TTF_OpenFont(file_string.c_str(), fontSize) }; _ptr == nullptr) {
        DEBUG_PRINT2("Error opening font file {}: {}\n", file_string, SDL_GetError());
    }
  }


} // namespace swgtk
