cmake_minimum_required (VERSION 3.28)

project(
	"SWGTK"
	DESCRIPTION "Software and Games Tool Kit" 
	HOMEPAGE_URL "https://github.com/m00se-3/SWGTK"
    VERSION 0.2.0
)

include("CPM.cmake")
include("cmake/CMakeOptions.cmake")
include("cmake/Config.cmake")
include(CTest)

# If building for emscripten

if(EMSCRIPTEN)

  message("-- Emscripten build enabled.\n")
  set(SWGTK_NO_CCACHE ON)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")

endif()

# Add CCache, if available

if(CCACHE_PROGRAM AND ${SWGTK_NO_CCACHE} MATCHES OFF)

  message("-- Enabling ccache.\n")
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_PROGRAM})
  set(CMAKE_C_COMPILER_LAUNCHER ${CCACHE_PROGRAM})

  if(NOT CPM_SOURCE_CACHE)
    set(CPM_SOURCE_CACHE ${CMAKE_CURRENT_LIST_DIR}/.cache/CPM)
  endif()
  
endif()

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

# SDL and external SDL dependencies
# If any more external dependencies for SDL become necessary in the future, they should be added before any SDL.(For consistency)

if(${SWGTK_INSTALL_FREETYPE})
  CPMAddPackage("gh:libsdl-org/freetype#VER-2-13-2")

  # Add a custom target alias for Freetype, since it uses a legacy version of cmake
  if (freetype_ADDED)
    add_library(Freetype::Freetype ALIAS freetype)
  endif()
endif()

CPMAddPackage(
  NAME "SDL"
  GITHUB_REPOSITORY "libsdl-org/SDL"
  GIT_TAG "release-3.2.16"
  OPTIONS 
  "SDL_TEST_LIBRARY false"
)

CPMAddPackage(
  NAME "SDL_image"
  GITHUB_REPOSITORY "libsdl-org/SDL_image"
  GIT_TAG "release-3.2.4"
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

add_subdirectory ("SWGTK")
add_subdirectory("examples")

