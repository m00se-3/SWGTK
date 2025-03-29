add_executable(Test_TextSample)

target_compile_options(Test_TextSample PRIVATE ${CompilerFlags})
target_link_options(Test_TextSample PRIVATE ${LinkerFlags})

target_compile_features(Test_TextSample PRIVATE cxx_std_23)

set_target_properties(Test_TextSample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/SWGTK/tests)

target_compile_definitions(
	Test_TextSample 

	PRIVATE 

	SWGTK_ASSETS="$ENV{Assets_Dir}"
	SWGTK_CONFIG="$ENV{Config_Dir}"
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	target_compile_definitions(Test_TextSample PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)
  
  set_property(TARGET Test_TextSample PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})

endif()

target_include_directories(
	Test_TextSample

	PUBLIC 

	${CMAKE_CURRENT_LIST_DIR}/../../engine/include
	${CMAKE_CURRENT_LIST_DIR}/src
	${lua_SOURCE_DIR}
)

target_link_libraries(
	Test_TextSample	

	PRIVATE 

	SWGTK
	SDL3::SDL3
	SDL3_image::SDL3_image
	SDL3_mixer::SDL3_mixer
	SDL3_ttf::SDL3_ttf
	lua
	sol2
	fmt::fmt
	gsl::gsl-lite-v1
)

target_sources(
    Test_TextSample 

	PUBLIC 

    ${CMAKE_CURRENT_LIST_DIR}/src/Project.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/TextTest.hpp

    PRIVATE 

    ${CMAKE_CURRENT_LIST_DIR}/src/TextTest.cpp
)

add_test(text_test Test_TextSample)
