add_executable(swl)

target_compile_options(swl PRIVATE ${CompilerFlags})
target_link_options(swl PRIVATE ${LinkerFlags})

target_compile_features(swl PRIVATE cxx_std_23)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    target_compile_definitions(swl PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)

    set_property(TARGET swl PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})

endif()

if(CPPCHECK_PROGRAM)

    set_target_properties(swl PROPERTIES CXX_CPPCHECK ${CPPCHECK_PROGRAM})

endif()

if(CPPLINT_PROGRAM)

    message("--- Enabling cpplint.\n")
    set_target_properties(swl PROPERTIES CXX_CPPLINT "${CPPLINT_PROGRAM};--filter=-whitespace,-build/include_order,-runtime/indentation_namespace,-build/c++11,-build/c++17,-runtime/references,-readability/nolint")

endif()

target_include_directories(
        swl

        PUBLIC

        ${CMAKE_CURRENT_LIST_DIR}/../engine/include
        ${CMAKE_CURRENT_LIST_DIR}
        ${lua_SOURCE_DIR}
)

target_link_libraries(
        swl

        PRIVATE

        SWGTK
)

target_sources(
        swl

        PRIVATE

        ${CMAKE_CURRENT_LIST_DIR}/swl.cpp
        ${CMAKE_CURRENT_LIST_DIR}/../engine/include/swgtk/internal/LuaGame.cpp
)
