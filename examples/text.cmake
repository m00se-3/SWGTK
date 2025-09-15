add_executable(TextSample)

target_compile_options(TextSample PRIVATE ${CompilerFlags})
target_link_options(TextSample PRIVATE ${LinkerFlags})

target_compile_features(TextSample PRIVATE cxx_std_23)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  target_compile_definitions(TextSample PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)

  set_property(TARGET TextSample PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})

endif()

if(CPPCHECK_PROGRAM)

  set_target_properties(TextSample PROPERTIES CXX_CPPCHECK ${CPPCHECK_PROGRAM})

endif()

target_include_directories(
  TextSample

  PUBLIC

  ${SWGTK_ROOT_DIRECTORY}/engine/include
  ${CMAKE_CURRENT_LIST_DIR}
  ${lua_SOURCE_DIR}
)

target_link_libraries(
  TextSample

  PRIVATE

  swgtk
  swgtk::SDLHW2D
)

target_sources(
  TextSample

  PUBLIC

  ${CMAKE_CURRENT_LIST_DIR}/Text.hpp

  PRIVATE

  ${CMAKE_CURRENT_LIST_DIR}/Text.cpp
)

if(NOT EMSCRIPTEN)
  if(WIN32) # Windows
    add_custom_command(
      TARGET TextSample POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:TextSample> $<TARGET_RUNTIME_DLLS:TextSample>
      COMMAND_EXPAND_LISTS
    )
  else() # Unix-based systems
    add_custom_command(
      TARGET TextSample POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:TextSample>
      ${SWGTK_SHARED_LIBARIES}
      COMMAND_EXPAND_LISTS
    )
  endif()
else()
  target_link_libraries(
    TextSample PRIVATE
    "--embed-file assets/Natural_Mono-Regular.ttf"
    "--embed-file assets/swgtk.lua"
  )

  target_link_options(TextSample PRIVATE "-s" "ALLOW_MEMORY_GROWTH=1")
endif()