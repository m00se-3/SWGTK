add_library(sdl2drender STATIC)
target_compile_options(sdl2drender PRIVATE ${CompilerFlags})

target_link_options(sdl2drender PRIVATE ${LinkerFlags})

target_compile_features(sdl2drender PRIVATE cxx_std_23)

if(${SWGTK_LUA_BINDINGS} MATCHES ON)
  target_compile_definitions(sdl2drender PUBLIC SWGTK_BUILD_WITH_LUA="1")
endif()

target_compile_definitions(
  sdl2drender

  PUBLIC

  HAV_STRINGS_H="0" # Disable non-standard strings.
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  target_compile_definitions(sdl2drender PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)
  set_target_properties(sdl2drender PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})
endif()

if(CPPCHECK_PROGRAM)
  set_target_properties(sdl2drender PROPERTIES CXX_CPPCHECK "${CPPCHECK_PROGRAM};--suppress=unknownMacro")
endif()

target_sources(
  sdl2drender

  PUBLIC

  FILE_SET HEADERS

  BASE_DIRS

  ${CMAKE_CURRENT_LIST_DIR}/include
  ${lua_SOURCE_DIR}

  FILES

  PUBLIC

  ${CMAKE_CURRENT_LIST_DIR}/include/swgtk/Texture.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/swgtk/Surface.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/swgtk/SDLHW2D.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/swgtk/RenderingDevice.hpp

  PRIVATE

  ${CMAKE_CURRENT_LIST_DIR}/src/SDLHW2D.cpp
)

target_link_libraries(
  sdl2drender

  PUBLIC

  SDL3::SDL3
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
)

add_library(swgtk::SDLHW2D ALIAS sdl2drender)

if(${SWGTK_LUA_BINDINGS} MATCHES ON)
  target_link_libraries(
    sdl2drender

    PUBLIC
    swgtk_lua
  )
endif()
