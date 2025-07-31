#ifndef SWGTK_INCLUDE_SWGTK_LUA_HPP
#define SWGTK_INCLUDE_SWGTK_LUA_HPP

#include <swgtk/Utility.hpp>
#include <sol/sol.hpp>

namespace swgtk {
    class App;
    void InitLua(App* app, sol::state& lua, LuaPrivledges priv = LuaPrivledges::None);
}

#endif // SWGTK_INCLUDE_SWGTK_LUA_HPP