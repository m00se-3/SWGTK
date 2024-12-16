add_executable(sandbox)

target_compile_options(sandbox PRIVATE ${CompilerFlags})
target_link_options(sandbox PRIVATE ${LinkerFlags})

target_compile_features(sandbox PRIVATE cxx_std_23)

target_compile_definitions(
	sandbox 

	PRIVATE 

	SWGTK_ASSETS="$ENV{Assets_Dir}"
	SWGTK_CONFIG="$ENV{Config_Dir}"
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	target_compile_definitions(sandbox PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)
  
  set_property(TARGET sandbox PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})

endif()

target_include_directories(
	sandbox

	PUBLIC 

	${CMAKE_CURRENT_LIST_DIR}/../../engine/include
	${lua_SOURCE_DIR}
)

target_link_libraries(
	sandbox	

	PRIVATE 

	SWGTK
	SDL3::SDL3
	SDL3_image::SDL3_image
	SDL3_mixer::SDL3_mixer
	SDL3_ttf::SDL3_ttf
	gsl::gsl-lite gsl::gsl-lite-v0 gsl::gsl-lite-v1
	lua
	sol2
)

target_sources(
    sandbox 

	PUBLIC 

    ${CMAKE_CURRENT_LIST_DIR}/src/Project.hpp

    PRIVATE 

    ${CMAKE_CURRENT_LIST_DIR}/src/sandbox.cpp
)
