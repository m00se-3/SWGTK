add_library(swgtk_lua STATIC)
add_library(swgtk::Lua ALIAS swgtk_lua)

target_compile_options(swgtk_lua PRIVATE ${CompilerFlags})
target_link_options(swgtk_lua PRIVATE ${LinkerFlags})

if(NOT EMSCRIPTEN)
  target_compile_definitions(
    swgtk_lua PUBLIC
    SWGTK_TABLE_LUA_FILE="${SWGTK_ENGINE_INTERNALS}/swgtk.lua"
    SWGTK_DEFAULT_FONT_ID="Natural_Mono-Regular"
  )
else()
  target_compile_definitions(
    swgtk_lua PUBLIC

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

target_sources(
  swgtk_lua

  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS 

  ${SWGTK_SOURCE_DIR}/SWGTK/engine/include
  ${lua_SOURCE_DIR}

  FILES 

  ${SWGTK_SOURCE_DIR}/SWGTK/engine/include/swgtk/Lua.hpp

  PRIVATE

  ${SWGTK_SOURCE_DIR}/SWGTK/engine/src/Lua.cpp
  ${lua_SOURCES}
)

target_link_libraries(
  swgtk_lua

  PUBLIC
  SDL3::SDL3
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
  sol2
)
