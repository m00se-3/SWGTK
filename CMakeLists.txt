cmake_minimum_required(VERSION 3.28)

project(
  "SWGTK"
  DESCRIPTION "Software and Games Tool Kit"
  HOMEPAGE_URL "https://github.com/m00se-3/SWGTK"
  VERSION 0.3.0
)

include("CPM.cmake")
include("cmake/Config.cmake")

# If building for emscripten

if(EMSCRIPTEN)
  message("-- Emscripten build enabled.\n")
endif()

# Add CCache, if available

if(SWGTK_CCACHE_FOUND AND NOT SWGTK_NO_CCACHE)

  message("-- Enabling ccache.\n")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${SWGTK_CCACHE})
  set(CMAKE_C_COMPILER_LAUNCHER ${SWGTK_CCACHE})

  if(NOT CPM_SOURCE_CACHE)
    set(CPM_SOURCE_CACHE ${CMAKE_CURRENT_LIST_DIR}/.cache/CPM)
  endif()

  message("Caching libraries in location: ${CPM_SOURCE_CACHE}\n")

endif()

if(SWGTK_BUILD_TESTS)
  include(CTest)

  CPMAddPackage("gh:catchorg/Catch2@3.10.0")
  enable_testing()
endif()

if(SWGTK_LUA_BINDINGS)
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
      PUBLIC
      ${lua_SOURCE_DIR}
    )

endif()

  CPMAddPackage("gh:ThePhD/sol2@3.5.0")
endif()

# SDL and external SDL dependencies
# If any more external dependencies for SDL become necessary in the future, they should be added before any SDL.(For consistency)

if(SWGTK_INSTALL_FREETYPE)
  CPMAddPackage("gh:freetype/freetype#master")

  # Add a custom target alias for Freetype, since it uses a legacy version of cmake
  if(freetype_ADDED)
    add_library(Freetype::Freetype ALIAS freetype)
  endif()
else()
  find_package(freetype REQUIRED)
endif()

CPMAddPackage(
  NAME "SDL"
  GITHUB_REPOSITORY "libsdl-org/SDL"
  GIT_TAG "release-3.4.4"
  OPTIONS
  "SDL_TEST_LIBRARY false"
)

CPMAddPackage(
  NAME "SDL3_image"
  GITHUB_REPOSITORY "libsdl-org/SDL_image"
  GIT_TAG "release-3.4.2"
  OPTIONS
  "SDLIMAGE_VENDORED false"
)

CPMAddPackage(
  NAME "SDL3_ttf"
  GITHUB_REPOSITORY "libsdl-org/SDL_ttf"
  GIT_TAG "release-3.2.2"
  OPTIONS
  "FREETYPE_LIBRARY ${freetype_BINARY_DIR}"
  "FREETYPE_INCLUDE_DIRS ${freetype_SOURCE_DIR}"
)

add_subdirectory("SWGTK")

if(SWGTK_BUILD_EXAMPLES)
  add_subdirectory("examples")
endif()

if(SWGTK_BUILD_TESTS)
  add_subdirectory("tests")
endif()

