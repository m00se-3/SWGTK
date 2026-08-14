#ifndef SWGTK_INCLUDE_SWGTK_LUA_HPP
#define SWGTK_INCLUDE_SWGTK_LUA_HPP

#include <sol/sol.hpp>
#include <swgtk/Utility.hpp>

namespace swgtk {
  class app;
  void init_lua(app* app, sol::state& lua, lua_privledges priv = lua_privledges::none);
} // namespace swgtk

#endif // SWGTK_INCLUDE_SWGTK_LUA_HPP
