include(${CMAKE_CURRENT_LIST_DIR}/engine/SDL2DRenderer.cmake)

add_library(swgtk STATIC)
add_library(swgtk::SWGTK ALIAS swgtk)

if(SWGTK_LUA_BINDINGS)
  include(${CMAKE_CURRENT_LIST_DIR}/engine/LuaBindings.cmake)
endif()

target_compile_options(swgtk PRIVATE ${CompilerFlags})
target_link_options(swgtk PRIVATE ${LinkerFlags})

if(EMSCRIPTEN)
  file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/assets)

  add_custom_command(
    TARGET swgtk PRE_LINK
    COMMAND ${CMAKE_COMMAND} -E copy -t ${PROJECT_BINARY_DIR}/assets
    ${CMAKE_CURRENT_LIST_DIR}/default_font/Natural_Mono-Regular.ttf
    ${SWGTK_ENGINE_INTERNALS}/swgtk.lua
  )
endif()

target_compile_definitions(
  swgtk
  PRIVATE
  $<$<CONFIG:Debug>:_DEBUG>
  $<$<BOOL:${SWGTK_LUA_BINDINGS}>:SWGTK_BUILD_WITH_LUA="1">
  HAV_STRINGS_H="0" # Disable non-standard strings.
)

if(NOT EMSCRIPTEN)
  target_compile_definitions(
    swgtk PUBLIC
    SWGTK_DEFAULT_FONT_FILE="${CMAKE_CURRENT_LIST_DIR}/default_font/Natural_Mono-Regular.ttf"
    SWGTK_DEFAULT_FONT_ID="Natural_Mono-Regular"
  )
else()
  target_compile_definitions(
    swgtk PUBLIC

    SWGTK_DEFAULT_FONT_FILE="assets/Natural_Mono-Regular.ttf"
    SWGTK_DEFAULT_FONT_ID="Natural_Mono-Regular"
  )
endif()

if(SWGTK_CLANG_TIDY_FOUND)

  message("-- Enabling clang-tidy.\n")
  set_target_properties(swgtk PROPERTIES CXX_CLANG_TIDY ${SWGTK_CLANG_TIDY})

endif()

if(SWGTK_CPPCHECK_FOUND)

  message("-- Enabling cppcheck.\n")
  set_target_properties(swgtk PROPERTIES CXX_CPPCHECK "${SWGTK_CPPCHECK};--suppress=unknownMacro")

endif()

target_sources(
  swgtk

  PUBLIC

  FILE_SET HEADERS

  BASE_DIRS

  ${CMAKE_CURRENT_LIST_DIR}/engine/include
  ${lua_SOURCE_DIR}

  FILES

  ${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/App.hpp
        ${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Texture.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Surface.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Input.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Scene.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/RenderingDevice.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Font.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Utility.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Math.hpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Timer.hpp

  PRIVATE

  ${CMAKE_CURRENT_LIST_DIR}/engine/src/App.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/src/Scene.cpp
  ${CMAKE_CURRENT_LIST_DIR}/engine/src/Font.cpp
)

target_link_libraries(
  swgtk

  PUBLIC

  SDL3::SDL3
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
  $<$<TARGET_EXISTS:swgtk::Lua>:swgtk::Lua>
  $<$<TARGET_EXISTS:Freetype::Freetype>:Freetype::Freetype>
)
