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
       swgtk::SDLHW2D
)

target_sources(
        ParticlesSample

        PUBLIC

        ${CMAKE_CURRENT_LIST_DIR}/Particles.hpp

        PRIVATE

        ${CMAKE_CURRENT_LIST_DIR}/Particles.cpp
)
if(NOT EMSCRIPTEN)
    if(WIN32) # Windows
        add_custom_command(
            TARGET ParticlesSample POST_BUILD 
            COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:ParticlesSample> $<TARGET_RUNTIME_DLLS:ParticlesSample>
            COMMAND_EXPAND_LISTS
        )
        else() # Unix-based systems
        add_custom_command(
            TARGET ParticlesSample POST_BUILD 
            COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:ParticlesSample>
                ${SWGTK_SHARED_LIBARIES}
            COMMAND_EXPAND_LISTS
        )
    endif()
else()
    target_link_libraries(
		ParticlesSample PRIVATE 
		"--embed-file assets/Natural_Mono-Regular.ttf"
		"--embed-file assets/swgtk.lua"
	)

    target_link_options(ParticlesSample PRIVATE "-s" "ALLOW_MEMORY_GROWTH=1")
endif()