add_executable(ParticlesSample)

target_compile_options(ParticlesSample PRIVATE ${CompilerFlags})
target_link_options(ParticlesSample PRIVATE ${LinkerFlags})

target_compile_features(ParticlesSample PRIVATE cxx_std_23)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
    target_compile_definitions(ParticlesSample PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)

    set_property(TARGET ParticlesSample PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})

endif()

if(CPPCHECK_PROGRAM)

    set_target_properties(ParticlesSample PROPERTIES CXX_CPPCHECK ${CPPCHECK_PROGRAM})

endif()

target_include_directories(
        ParticlesSample

        PUBLIC

        ${SWGTK_ROOT_DIRECTORY}/engine/include
        ${CMAKE_CURRENT_LIST_DIR}
        ${lua_SOURCE_DIR}
)

target_link_libraries(
        ParticlesSample

        PRIVATE

        swgtk
)

target_sources(
        ParticlesSample

        PUBLIC

        ${CMAKE_CURRENT_LIST_DIR}/Particles.hpp

        PRIVATE

        ${CMAKE_CURRENT_LIST_DIR}/Particles.cpp
)