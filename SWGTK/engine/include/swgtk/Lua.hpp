#ifndef SWGTK_INCLUDE_SWGTK_LUA_HPP
#define SWGTK_INCLUDE_SWGTK_LUA_HPP

#include <sol/sol.hpp>
#include <swgtk/Utility.hpp>

namespace swgtk {
  class App;
  void InitLua(App* app, sol::state& lua, LuaPrivledges priv = LuaPrivledges::None);
} // namespace swgtk

#endif // SWGTK_INCLUDE_SWGTK_LUA_HPP
