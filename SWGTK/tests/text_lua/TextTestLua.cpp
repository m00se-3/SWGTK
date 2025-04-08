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
#include <swgtk/Simple2DRenderer.hpp>
#include <swgtk/App.hpp>
#include <swgtk/LuaGame.hpp>

int main([[maybe_unused]]const int argc, [[maybe_unused]]const char** argv) {
    constexpr auto w = 800;
    constexpr auto h = 600;

    try {
        if(auto app = swgtk::App{}; app.InitGraphics("Text With Lua", w, h, swgtk::Simple2DRenderer::Create())) {
            sol::state lua;

            auto* render = swgtk::RenderImpl<swgtk::Simple2DRenderer>(app.Renderer());

            app.InitLua(lua, true);
            app.GetFontHandle().InitLua(lua);
            render->InitLua(lua);

            app.RunLuaGame(TEST_LUA_FILE, lua);
        }
    } catch(std::exception& e) {
        DEBUG_PRINT("{}", e.what());
    }
}