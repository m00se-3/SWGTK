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

#include <stdexcept>
#include <swgtk/App.hpp>
#include <LuaGame.hpp>
#include <swgtk/Simple2DRenderer.hpp>
#include <ranges>
#include <string>
#include <unordered_map>

static constexpr auto def_w = 800;
static constexpr auto def_h = 600;

static constexpr auto argumentPatterns = std::array {
    "--width",
    "--height",
    "--title"
};

namespace {
    inline void swlPanic(const sol::optional<std::string>& msg) {
        fmt::print(stderr, "Exception occurred: {}\n", msg.value_or("Unknown error."));
    }

    void processArguments(const auto& arguments, std::unordered_map<std::string, std::string>& options, std::string& startupFile) {
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
} // namespace

int main (const int argc, const char** argv) {

    try {
        if(const auto args = std::ranges::drop_view{std::span<const char*>{ argv, static_cast<size_t>(argc) }, 1u}; argc > 1) {

            std::unordered_map<std::string, std::string> options;
            std::string startupFile;

            processArguments(args, options, startupFile);

            const auto& title = (options.contains("--title")) ? options.at("--title") : "Lua Game";
            const auto& w = (options.contains("--width")) ? std::stoi(options.at("--width")) : def_w;
            const auto& h = (options.contains("--height")) ? std::stoi(options.at("--height")) : def_h;

            if(auto swl = swgtk::App{}; swl.InitGraphics(title.c_str(), w, h, swgtk::Simple2DRenderer::Create())) {
                sol::state lua{sol::c_call<decltype(&swlPanic), &swlPanic>};

                if(const auto mfile = lua.safe_script_file(SWGTK_TABLE_LUA_FILE); !mfile.valid()) {
                    throw std::runtime_error("Swgtk could not initialize correctly.");
                }

                swl.InitLua(lua, swgtk::LuaPrivledges::All);

                if(startupFile.empty() || !startupFile.ends_with(".lua")) { throw std::runtime_error("No startup file provided."); }

                swl.MakeScene<swgtk::LuaGame>(startupFile, lua);

                if(const auto mfile = lua.safe_script_file(SWGTK_ENGINE_LUA_FILE); !mfile.valid()) {
                    throw std::runtime_error("Swl could not initialize correctly.");
                }

                if(!swl.InitializeGame()) {
                    throw std::runtime_error("main() failed to execute successfully.");
                }
            }
        } else {
            throw std::runtime_error("No startup file provided.");
        }
    } catch (const std::exception& e) {
        fmt::print(stderr, "Exception occurred: {}\n", e.what());
    }
}