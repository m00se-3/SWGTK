add_library(swgtk_lua STATIC)
add_library(swgtk::Lua ALIAS swgtk_lua)

target_compile_options(swgtk_lua PUBLIC ${CompilerFlags})
target_link_options(swgtk_lua PUBLIC ${LinkerFlags})

if(NOT EMSCRIPTEN)
  target_compile_definitions(
    swgtk_lua PRIVATE 
    SWGTK_TABLE_LUA_FILE="${SWGTK_ENGINE_INTERNALS}/swgtk.lua"
    SWGTK_DEFAULT_FONT_ID="Natural_Mono-Regular"
  )
else()
  target_compile_definitions(
    swgtk_lua PRIVATE

    SWGTK_TABLE_LUA_FILE="assets/swgtk.lua"
    SWGTK_DEFAULT_FONT_ID="Natural_Mono-Regular"
  )
endif()

if($<CONFIG:Debug>)
  target_compile_definitions(swgtk_lua PRIVATE _DEBUG)
endif()

if(SWGTK_CLANG_TIDY_FOUND)
  set_target_properties(swgtk_lua PROPERTIES CXX_CLANG_TIDY ${SWGTK_CLANG_TIDY})

endif()

if(SWGTK_CPPCHECK_FOUND)
  set_target_properties(swgtk_lua PROPERTIES CXX_CPPCHECK "${SWGTK_CPPCHECK};--suppress=unknownMacro")

endif()

# Lua target

add_library(lua STATIC ${lua_sources})
target_include_directories(lua PUBLIC ${lua_SOURCE_DIR})

# end Lua target

target_sources(
  swgtk_lua

  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS 

  ${SWGTK_SOURCE_DIR}/SWGTK/engine/include

  FILES 

  ${SWGTK_SOURCE_DIR}/SWGTK/engine/include/swgtk/Lua.hpp

  PRIVATE

  ${SWGTK_SOURCE_DIR}/SWGTK/engine/src/Lua.cpp
)

target_link_libraries(
  swgtk_lua

  PUBLIC
  SDL3::SDL3
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
  sol2
)

target_link_libraries(
  swgtk_lua
  PRIVATE 
  lua
)
