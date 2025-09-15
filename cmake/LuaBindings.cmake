add_library(swgtk_lua STATIC)

target_compile_options(swgtk_lua PRIVATE ${CompilerFlags})
target_link_options(swgtk_lua PRIVATE ${LinkerFlags})

if(DEFINED SWGTK_SANITIZE AND NOT ${SWGTK_SANITIZE} MATCHES "")
  target_compile_options(swgtk_lua PRIVATE ${SWGTK_SANITIZE})
endif()

target_compile_features(swgtk_lua PRIVATE cxx_std_23)

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

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  target_compile_definitions(swgtk_lua PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)

  message("-- Enabling clang-tidy.\n")
  set_target_properties(swgtk_lua PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})

endif()

if(CPPCHECK_PROGRAM)

  message("-- Enabling cppcheck.\n")
  set_target_properties(swgtk_lua PROPERTIES CXX_CPPCHECK "${CPPCHECK_PROGRAM};--suppress=unknownMacro")

endif()

target_sources(
  swgtk_lua

  PUBLIC
  FILE_SET HEADERS
  BASE_DIRS ${SWGTK_SOURCE_DIR}/SWGTK/engine/include

  PUBLIC
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
  lua
  sol2
)