find_program(SWGTK_CCACHE_FOUND ${SWGTK_CCACHE})
find_program(SWGTK_CLANG_TIDY_FOUND ${SWGTK_CLANG_TIDY})
find_program(SWGTK_CPPCHECK_FOUND ${SWGTK_CPPCHECK})

if($ENV{MSVC_IDE} MATCHES "ON")
  include(${CMAKE_CURRENT_LIST_DIR}/MSVCOptions.cmake)
else()
  include(${CMAKE_CURRENT_LIST_DIR}/GNUOptions.cmake)
endif()
