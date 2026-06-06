add_executable(Platformer)

target_compile_options(Platformer PRIVATE ${CompilerFlags})
target_link_options(Platformer PRIVATE ${LinkerFlags})

target_compile_definitions(
  Platformer
  PRIVATE
  ASSETS_DIR="${CMAKE_CURRENT_LIST_DIR}/assets/"
  $<$<CONFIG:Debug>:_DEBUG>
)

if(SWGTK_CLANG_TIDY_FOUND)

    set_property(TARGET Platformer PROPERTY CXX_CLANG_TIDY ${SWGTK_CLANG_TIDY})

endif()

if(SWGTK_CPPCHECK_FOUND)

    set_target_properties(Platformer PROPERTIES CXX_CPPCHECK ${SWGTK_CPPCHECK})

endif()

target_link_libraries(
        Platformer

        PRIVATE

        swgtk::SWGTK
        swgtk::SDLHW2D
        swgtk::Lua
)

target_sources(
        Platformer

        PUBLIC
        FILE_SET HEADERS

        BASE_DIRS
        ${SWGTK_ROOT_DIRECTORY}/engine/include
        ${CMAKE_CURRENT_LIST_DIR}/src
        ${lua_SOURCE_DIR}

        FILES

        PUBLIC

        ${CMAKE_CURRENT_LIST_DIR}/src/Platformer.hpp

        PRIVATE

        ${CMAKE_CURRENT_LIST_DIR}/src/Platformer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
)
if(NOT EMSCRIPTEN)
    if(WIN32) # Windows
        add_custom_command(
                TARGET Platformer POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:Platformer> $<TARGET_RUNTIME_DLLS:Platformer>
                COMMAND_EXPAND_LISTS
        )
    else() # Unix-based systems
        add_custom_command(
                TARGET Platformer POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:Platformer>
                ${SWGTK_SHARED_LIBARIES}
                COMMAND_EXPAND_LISTS
        )
    endif()
else()
    target_link_libraries(
            Platformer PRIVATE
            "--embed-file assets/Natural_Mono-Regular.ttf"
            "--embed-file assets/swgtk.lua"
    )

    target_link_options(Platformer PRIVATE "-s" "ALLOW_MEMORY_GROWTH=1" "-sASYNCIFY=1")
endif()
