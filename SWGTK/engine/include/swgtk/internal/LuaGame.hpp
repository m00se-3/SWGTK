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
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_LUAGAME_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_LUAGAME_HPP_

#include <swgtk/Scene.hpp>
#include <filesystem>
#include <sol/sol.hpp>
#include <gsl/gsl-lite.hpp>

namespace swgtk {
    /**
        @brief This is a specialized class for games whose *primary* coding language is Lua.
     */
    class LuaGame final : public Scene::Node {
    public:
        LuaGame(const gsl::not_null<Scene*>& scene, const std::filesystem::path& path, App* app);

        [[nodiscard]] bool Create() override;
        [[nodiscard]] bool Update([[maybe_unused]]float dt) override { return true; }

    private:
        sol::state _lua;
    };

} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_LUAGAME_HPP_
