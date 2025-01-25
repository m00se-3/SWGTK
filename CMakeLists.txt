cmake_minimum_required (VERSION 3.25)

project ("SWGTK")

include("CPM.cmake")
include("cmake/Config.cmake")
include(CTest)

# Add CCache, if available

if(CCACHE_PROGRAM)

  message("-- Enabling ccache.\n")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
  set(CPM_SOURCE_CACHE ${CMAKE_CURRENT_LIST_DIR}/.cache/CPM)
  
endif()

# If building for emscripten
if(EMSCRIPTEN)
  message("-- Emscripten build enabled!!!")

  set(CMAKE_EXECUTABLE_SUFFIX ".html")

endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SWGTK)

# Lua and sol2
CPMAddPackage(
  NAME lua
  GITHUB_REPOSITORY lua/lua
  VERSION 5.4.6
  DOWNLOAD_ONLY Yes
)

if(lua_ADDED)

  FILE(GLOB lua_sources ${lua_SOURCE_DIR}/*.c)
  list(REMOVE_ITEM lua_sources ${lua_SOURCE_DIR}/lua.c ${lua_SOURCE_DIR}/luac.c)

  add_library(lua STATIC ${lua_sources})

  target_include_directories(
    lua
    PRIVATE
    ${lua_SOURCE_DIR}
  )

endif()

CPMAddPackage("gh:m00se-3/sol2#fix-clang_18")

# SDL and external dependencies

CPMAddPackage("gh:libsdl-org/freetype#VER-2-13-2")

# Add a custom target alias for Freetype, since it uses a legacy version of cmake
if (freetype_ADDED)
  add_library(Freetype::Freetype ALIAS freetype)
endif()

CPMAddPackage("gh:libsdl-org/SDL#release-3.2.0")
CPMAddPackage("gh:libsdl-org/SDL_image#prerelease-3.1.1")

# If any more external dependencies become necessary in the future, they should be added before any SDL.(For consistency)

CPMAddPackage(
  NAME "SDL3_ttf"
  GITHUB_REPOSITORY "libsdl-org/SDL_ttf"
  GIT_TAG "main"
  OPTIONS 
  "FREETYPE_LIBRARY ${freetype_BINARY_DIR}"
  "FREETYPE_INCLUDE_DIRS ${freetype_SOURCE_DIR}"
)

CPMAddPackage(
    NAME "SDL3_mixer"
    GITHUB_REPOSITORY "libsdl-org/SDL_mixer"
    GIT_TAG "main"
    OPTIONS 
    "SDL3MIXER_OPUS false"
    "SDL3MIXER_MOD false"
)

CPMAddPackage("gh:fmtlib/fmt#11.1.2")

# Include sub-projects.
add_subdirectory ("SWGTK")
