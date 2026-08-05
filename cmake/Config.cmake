find_program(SWGTK_CCACHE_FOUND ${SWGTK_CCACHE})
find_program(SWGTK_CLANG_TIDY_FOUND ${SWGTK_CLANG_TIDY})
find_program(SWGTK_CPPCHECK_FOUND ${SWGTK_CPPCHECK})

# Sets the following options to defaults for library consumers.
# Devs use the defaults defined in CMakePresets.json

if(NOT DEFINED SWGTK_BUILD_EXAMPLES)
  set(SWGTK_BUILD_EXAMPLES OFF)
endif()

if(NOT DEFINED SWGTK_NO_CCACHE)
  set(SWGTK_NO_CCACHE ON)
endif()

if(NOT DEFINED SWGTK_INSTALL_FREETYPE)
  set(SWGTK_INSTALL_FREETYPE ON)
endif()

if(NOT DEFINED SWGTK_LUA_BINDINGS)
  set(SWGTK_LUA_BINDINGS OFF)
endif()

if(NOT DEFINED SWGTK_BUILD_TESTS)
  set(SWGTK_BUILD_TESTS OFF)
endif()

if(NOT DEFINED SWGTK_EXCEPTIONS)
  set(SWGTK_EXCEPTIONS OFF)
endif()

if($<CXX_COMPILER_ID:MSVC>)
  include(${CMAKE_CURRENT_LIST_DIR}/MSVCOptions.cmake)
else()
  include(${CMAKE_CURRENT_LIST_DIR}/GNUOptions.cmake)
endif()
