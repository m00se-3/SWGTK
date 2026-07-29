add_library(gpu2d STATIC)
target_compile_options(gpu2d PRIVATE ${CompilerFlags})

target_link_options(gpu2d PRIVATE ${LinkerFlags})

target_compile_definitions(
  gpu2d

  PUBLIC

  $<$<CONFIG:Debug>:_DEBUG>
  $<$<BOOL:${SWGTK_LUA_BINDINGS}>:SWGTK_BUILD_WITH_LUA="1">
  HAV_STRINGS_H="0" # Disable non-standard strings.
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  target_compile_definitions(gpu2d PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)
  set_target_properties(gpu2d PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})
endif()

if(CPPCHECK_PROGRAM)
  set_target_properties(gpu2d PROPERTIES CXX_CPPCHECK "${CPPCHECK_PROGRAM};--suppress=unknownMacro")
endif()

target_sources(
  gpu2d

  PUBLIC

  FILE_SET HEADERS

  BASE_DIRS

  ${CMAKE_CURRENT_LIST_DIR}/include
  ${lua_SOURCE_DIR}

  FILES

  PUBLIC

  ${CMAKE_CURRENT_LIST_DIR}/include/swgtk/Texture.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/swgtk/Surface.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/swgtk/GPU2D.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/swgtk/RenderingDevice.hpp

  PRIVATE

  ${CMAKE_CURRENT_LIST_DIR}/src/GPU2D.cpp
)

target_link_libraries(
  gpu2d

  PUBLIC

  SDL3::SDL3
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
)

add_library(swgtk::GPU2D ALIAS gpu2d)

if(${SWGTK_LUA_BINDINGS} MATCHES ON)
  target_link_libraries(
    gpu2d

    PUBLIC
    swgtk_lua
  )
endif()
