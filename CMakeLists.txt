cmake_minimum_required (VERSION 3.25)

project(
	"SWGTK"
	DESCRIPTION "Software and Games Tool Kit" 
	HOMEPAGE_URL "https://github.com/m00se-3/SWGTK"
    VERSION 0.1
)

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


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SWGTK_BINARY_DIR})

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

CPMAddPackage("gh:ThePhD/sol2@3.5.0")

# SDL and external SDL dependencies
# If any more external dependencies for SDL become necessary in the future, they should be added before any SDL.(For consistency)

if($ENV{SWGTK_INSTALL_FREETYPE})
  CPMAddPackage("gh:libsdl-org/freetype#VER-2-13-2")

  # Add a custom target alias for Freetype, since it uses a legacy version of cmake
  if (freetype_ADDED)
    add_library(Freetype::Freetype ALIAS freetype)
  endif()
endif()

CPMAddPackage("gh:libsdl-org/SDL#release-3.2.10")

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
  GIT_TAG "release-3.2.0"
  OPTIONS 
  "FREETYPE_LIBRARY ${freetype_BINARY_DIR}"
  "FREETYPE_INCLUDE_DIRS ${freetype_SOURCE_DIR}"
)

CPMAddPackage("gh:fmtlib/fmt#11.1.2")
CPMAddPackage("gh:gsl-lite/gsl-lite@0.42.0")

# Include sub-projects.
add_subdirectory ("SWGTK")
add_subdirectory("examples")
