# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.25)

project ("SWGTK")

include("CPM.cmake")

# Clang compiler warning flags go here.
list(APPEND WarningFlags "-Wall" "-Wextra" "-Wpedantic" "-Wno-newline-eof" "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic" "-Wconversion" "-Wshadow" "-Wnon-virtual-dtor")
set(CMAKE_CXX_CLANG_TIDY clang-tidy)

# If building for emscripten, we want to add the cmake folder for our local dependencies to the search path.
if(EMSCRIPTEN)
message("-- Emscripten build enabled!!!")

set(CMAKE_EXECUTABLE_SUFFIX ".html")

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
    PRIVATE
    ${lua_SOURCE_DIR}
  )

endif()

CPMAddPackage("gh:m00se-3/sol2#fix-clang_18")

# SDL and external dependencies
CPMAddPackage(
  NAME WavPack
  GITHUB_REPOSITORY "libsdl-org/WavPack"
  GIT_TAG "5.7.0"
  OPTIONS "BUILD_SHARED_LIBS true"
)

CPMAddPackage("gh:libsdl-org/freetype#VER-2-13-2")

# Add a custom target alias for Freetype, since it uses a legacy version of cmake
if (freetype_ADDED)
  add_library(Freetype::Freetype ALIAS freetype)
endif()

CPMAddPackage("gh:libsdl-org/SDL#release-2.30.4")
CPMAddPackage("gh:libsdl-org/SDL_image#release-2.8.2")
CPMAddPackage("gh:libsdl-org/SDL_ttf#release-2.22.0")

# If any of these external dependencies become necessary in the future, they should be added before any SDL.(For consistency)
CPMAddPackage(
    NAME "SDL_mixer"
    GITHUB_REPOSITORY "libsdl-org/SDL_mixer"
    GIT_TAG "release-2.8.0"
    OPTIONS 
    "SDL2MIXER_OPUS false"
    "SDL2MIXER_MOD false"
    "SDL2MIXER_MIDI_FLUIDSYNTH false"
)

#  Test_Suite specific
CPMAddPackage("gh:Immediate-Mode-UI/Nuklear#4.12.0")

# Misc
CPMAddPackage("gh:gsl-lite/gsl-lite@0.41.0")
CPMAddPackage("gh:fmtlib/fmt#10.2.1")

# Include sub-projects.
add_subdirectory ("SWGTK")
