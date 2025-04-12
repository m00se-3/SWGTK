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
#include <LuaGame.hpp>
#include <fmt/format.h>

namespace swgtk {

    LuaGame::LuaGame(const std::filesystem::path& path, sol::state& lua) : _lua(gsl::make_not_null<sol::state*>(&lua)) {
        if(std::filesystem::exists(path)) {

            if(const sol::protected_function_result file = lua.safe_script_file(path.string()); file.valid())
            {

                if(const sol::optional cr = (*_lua)["OnCreate"]; !cr) {
                    throw std::runtime_error("OnCreate function could not be found.");
                }

                if(const sol::optional up = (*_lua)["OnUpdate"]; !up) {
                    throw std::runtime_error("OnUpdate function could not be found.");
                }

                if(const auto mfile = _lua->safe_script_file(SWGTK_ENGINE_LUA_FILE); !mfile.valid()) {
                    throw std::runtime_error(fmt::format("Could not load lua script: {}", SWGTK_ENGINE_LUA_FILE));
                }


            }
        } else {
            throw std::runtime_error(fmt::format("Could not load lua script: {}", path.string()));
        }
    }

    bool LuaGame::Create([[maybe_unused]]Scene& scene) {
        if(const sol::optional<sol::protected_function> func = (*_lua)["main"]; func) {
            const sol::protected_function_result result = (*func)();
            return result.valid();
        }

        return false;
    }

} // namespace swgtk