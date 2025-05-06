add_executable(swl2d)

target_compile_options(swl2d PRIVATE ${CompilerFlags})
target_link_options(swl2d PRIVATE ${LinkerFlags})

target_compile_features(swl2d PRIVATE cxx_std_23)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    target_compile_definitions(swl2d PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)

    set_property(TARGET swl2d PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})

endif()

if(CPPCHECK_PROGRAM)

    set_target_properties(swl2d PROPERTIES CXX_CPPCHECK ${CPPCHECK_PROGRAM})

endif()

if(CPPLINT_PROGRAM)

    set_target_properties(swl2d PROPERTIES CXX_CPPLINT ${CPPLINT_PROGRAM})

endif()

target_include_directories(
        swl2d

        PUBLIC

        ${SWGTK_ROOT_DIRECTORY}/engine/include
        ${CMAKE_CURRENT_LIST_DIR}
        ${lua_SOURCE_DIR}
)

target_link_libraries(
        swl2d

        PRIVATE

        swgtk
)

target_sources(
        swl2d

        PRIVATE

        ${CMAKE_CURRENT_LIST_DIR}/swl2d.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../engine/include/swgtk/internal/LuaGame.cpp
)
