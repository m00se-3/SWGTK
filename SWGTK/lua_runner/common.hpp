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

#ifndef SWGTK_LUA_RUNNER_COMMON_HPP_
#define SWGTK_LUA_RUNNER_COMMON_HPP_

#include <stdexcept>
#include <swgtk/App.hpp>
#include <LuaGame.hpp>
#include <ranges>
#include <string>
#include <unordered_map>
#include <fmt/format.h>

static constexpr auto argumentPatterns = std::array {
    "--width",
    "--height",
    "--title"
};

namespace swl {
    static constexpr auto def_w = 800;
    static constexpr auto def_h = 600;

    inline void processArguments(const auto& arguments, std::unordered_map<std::string, std::string>& options, std::string& startupFile) {
        std::string currentArg;

        for(const std::string_view arg : arguments) {
            if(std::ranges::any_of(argumentPatterns, [arg] (auto pattern) { return arg == pattern; })) {
                if(!currentArg.empty()) { throw std::invalid_argument("Invalid argument passed."); }
                currentArg = std::string{arg};
            } else if(!currentArg.empty()) {
                options[currentArg] = arg;
                currentArg.clear();
            } else if(startupFile.empty()) {
                startupFile = std::string{arg};
            } else {
                throw std::invalid_argument("Invalid argument passed.");
            }
        }
    }
} // namespace swl

#endif // SWGTK_LUA_RUNNER_COMMON_HPP_