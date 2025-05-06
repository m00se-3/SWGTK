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

if(CPPLINT_PROGRAM)
    set_target_properties(TextSample PROPERTIES CXX_CPPLINT ${CPPLINT_PROGRAM})
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
)

target_sources(
        TextSample

        PUBLIC

        ${CMAKE_CURRENT_LIST_DIR}/Text.hpp

        PRIVATE

        ${CMAKE_CURRENT_LIST_DIR}/Text.cpp
)