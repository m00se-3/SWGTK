add_executable(testsuite)
include(${Catch2_SOURCE_DIR}/extras/Catch.cmake)
target_compile_options(testsuite PRIVATE ${CompilerFlags})

target_link_options(testsuite PRIVATE ${LinkerFlags})

target_compile_features(testsuite PRIVATE cxx_std_23)

target_compile_definitions(
  testsuite

  PUBLIC

  HAV_STRINGS_H="0" # Disable non-standard strings.
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  target_compile_definitions(testsuite PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)
  set_target_properties(testsuite PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})
endif()

if(CPPCHECK_PROGRAM)
  set_target_properties(testsuite PROPERTIES CXX_CPPCHECK "${CPPCHECK_PROGRAM};--suppress=unknownMacro")
endif()

target_sources(
  testsuite

  PUBLIC

  FILE_SET HEADERS

  BASE_DIRS

  ${SWGTK_SOURCE_DIR}/SWGTK/engine/include
  ${CMAKE_CURRENT_LIST_DIR}/include

  FILES

  PUBLIC

  ${CMAKE_CURRENT_LIST_DIR}/include/TestRenderer.hpp

  PRIVATE

  ${CMAKE_CURRENT_LIST_DIR}/src/MathTests.cpp
)

target_link_libraries(testsuite PRIVATE swgtk swgtk::SDLHW2D Catch2::Catch2WithMain)
catch_discover_tests(testsuite)