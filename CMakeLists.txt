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

include(${CMAKE_CURRENT_LIST_DIR}/SWGTK/external/cmake/BuildLua.cmake)

else()

include(FindLua)
find_package(Lua REQUIRED)

endif()

CPMAddPackage("gh:m00se-3/sol2#fix-clang_18")
CPMFindPackage(NAME "SDL2" DOWNLOAD_NAME "SDL" GIT_TAG "release-2.30.x" FIND_PACAKGE_ARGUMENTS "CONFIG REQUIRED")
CPMFindPackage(NAME "SDL2_image" DOWNLOAD_NAME "SDL_image" GIT_TAG "release-2.8.x" FIND_PACKAGE_ARGUMENTS "CONFIG REQUIRED")
CPMFindPackage(NAME "SDL2_ttf" DOWNLOAD_NAME "SDL_ttf" GIT_TAG "release-2.22.x" FIND_PACKAGE_ARGUMENTS "CONFIG REQUIRED")
CPMFindPackage(NAME "SDL2_mixer" DOWNLOAD_NAME "SDL_mixer" GIT_TAG "release-2.8.x" FIND_PACKAGE_ARGUMENTS "CONFIG REQUIRED")
CPMFindPackage(NAME "unofficial-nuklear" DOWNLOAD_NAME "Nuklear" FIND_PACKAGE_ARGUMENTS "CONFIG REQUIRED")
CPMFindPackage(NAME "gsl-lite" VERSION "0.41.0" FIND_PACKAGE_ARGUMENTS "CONFIG REQUIRED")

# Include sub-projects.
add_subdirectory ("SWGTK")
