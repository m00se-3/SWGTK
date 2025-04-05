/*
 * This test is to demonstrate how to create a dedicated Lua scripted game.
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