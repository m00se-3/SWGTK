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
#include <swgtk/internal/LuaGame.hpp>
#include <string>
#include <fmt/format.h>

#include "swgtk/App.hpp"

namespace swgtk {

    namespace {
        inline void panic(const sol::optional<std::string>& msg) {
            fmt::print(stderr, "Exception occurred: {}\n", msg.value_or("Unknown error."));
        }
    }

    LuaGame::LuaGame(const gsl::not_null<Scene*>& scene, const std::filesystem::path& path, App* app)
    : Node(scene), _lua(sol::c_call<decltype(&panic), &panic>) {
        app->InitLua(_lua, LuaPrivledges::All);

        if(std::filesystem::exists(path) && path.extension() == ".lua") {

            // false positive
            // cppcheck-suppress syntaxError
            if(const auto file = _lua.safe_script_file(path.string()); file.valid()) {
                auto swl = _lua["swgtk"];

                if(const auto cr = swl["OnCreate"]; !cr.valid()) {
                    throw std::runtime_error("No OnCreate function found in the swgtk namespace.");
                }

                if(const auto up = swl["OnUpdate"]; !up.valid()) {
                    throw std::runtime_error("No OnUpdate function could be found in the swgtk namespace.");
                }

            }

            if(const auto mfile = _lua.safe_script_file(SWGTK_ENGINE_LUA_FILE); !mfile.valid()) {
                throw std::runtime_error("Swl could not initialize correctly.");
            }

        } else {
            throw std::runtime_error(fmt::format("No startup file provided: {}", path.string()));
        }
    }

    bool LuaGame::Create() {
        if(const sol::protected_function func = _lua["main"]; func.valid()) {
            const sol::protected_function_result result = func();
            return result.valid();
        }

        return false;
    }

} // namespace swgtk