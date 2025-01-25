add_executable(text)

target_compile_options(text PRIVATE ${CompilerFlags})
target_link_options(text PRIVATE ${LinkerFlags})

target_compile_features(text PRIVATE cxx_std_23)

target_compile_definitions(
	text 

	PRIVATE 

	SWGTK_ASSETS="$ENV{Assets_Dir}"
	SWGTK_CONFIG="$ENV{Config_Dir}"
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	target_compile_definitions(text PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)
  
  set_property(TARGET text PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})

endif()

target_include_directories(
	text

	PUBLIC 

	${CMAKE_CURRENT_LIST_DIR}/../../engine/include
	${CMAKE_CURRENT_LIST_DIR}/src
	${lua_SOURCE_DIR}
)

target_link_libraries(
	text	

	PRIVATE 

	SWGTK
	SDL3::SDL3
	SDL3_image::SDL3_image
	SDL3_mixer::SDL3_mixer
	SDL3_ttf::SDL3_ttf
	lua
	sol2
	fmt::fmt
)

target_sources(
    text 

	PUBLIC 

    ${CMAKE_CURRENT_LIST_DIR}/src/Project.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TextTest.hpp

    PRIVATE 

    ${CMAKE_CURRENT_LIST_DIR}/src/TextTest.cpp
)

add_test(text_test text)
