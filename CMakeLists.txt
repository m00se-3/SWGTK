# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.25)

project ("CardTableSimulator")

# Clang compiler warning flags go here.
list(APPEND WarningFlags "-Wall" "-Wextra" "-Wpedantic" "-Wno-newline-eof" "-Wno-c++98-compat" "-Wno-c++98-compat-pedantic")

# If building for emscripten, we want to add the cmake folder for our local dependencies to the search path.
if(EMSCRIPTEN)
message("-- Emscripten build enabled!!!")

set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

# Include sub-projects.
add_subdirectory ("CardTableSimulator")
