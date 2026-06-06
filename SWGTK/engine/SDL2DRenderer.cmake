add_library(sdl2drender STATIC)
target_compile_options(sdl2drender PRIVATE ${CompilerFlags})
target_link_options(sdl2drender PRIVATE ${LinkerFlags})

target_compile_definitions(sdl2drender
  PRIVATE
  $<$<CONFIG:Debug>:_DEBUG>
  $<$<BOOL:${SWGTK_LUA_BINDINGS}>:SWGTK_BUILD_WITH_LUA="1">
  HAV_STRINGS="0"
)

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
  $<$<TARGET_EXISTS:swgtk::Lua>:swgtk::Lua>
)

add_library(swgtk::SDLHW2D ALIAS sdl2drender)

