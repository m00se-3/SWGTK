add_executable(Test_Particles)

target_compile_options(Test_Particles PRIVATE ${CompilerFlags})
target_link_options(Test_Particles PRIVATE ${LinkerFlags})

target_compile_features(Test_Particles PRIVATE cxx_std_23)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    target_compile_definitions(Test_Particles PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)

    set_property(TARGET Test_Particles PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})

endif()

if(CPPCHECK_PROGRAM)

    set_target_properties(Test_Particles PROPERTIES CXX_CPPCHECK ${CPPCHECK_PROGRAM})

endif()

if(CPPLINT_PROGRAM)
    set_target_properties(Test_Particles PROPERTIES CXX_CPPLINT ${CPPLINT_PROGRAM})
endif()

target_include_directories(
        Test_Particles

        PUBLIC

        ${SWGTK_ROOT_DIRECTORY}/engine/include
        ${CMAKE_CURRENT_LIST_DIR}
        ${lua_SOURCE_DIR}
)

target_link_libraries(
        Test_Particles

        PRIVATE

        swgtk
)

target_sources(
        Test_Particles

        PUBLIC

        ${CMAKE_CURRENT_LIST_DIR}/Particles.hpp

        PRIVATE

        ${CMAKE_CURRENT_LIST_DIR}/Particles.cpp
)