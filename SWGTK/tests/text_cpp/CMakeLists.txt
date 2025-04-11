add_executable(Test_TextSample)

target_compile_options(Test_TextSample PRIVATE ${CompilerFlags})
target_link_options(Test_TextSample PRIVATE ${LinkerFlags})

target_compile_features(Test_TextSample PRIVATE cxx_std_23)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	target_compile_definitions(Test_TextSample PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)
  
  set_property(TARGET Test_TextSample PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})

endif()

if(CPPCHECK_PROGRAM)

	set_target_properties(Test_TextSample PROPERTIES CXX_CPPCHECK ${CPPCHECK_PROGRAM})

endif()

target_include_directories(
	Test_TextSample

	PUBLIC 

	${CMAKE_CURRENT_LIST_DIR}/../../../engine/include
	${CMAKE_CURRENT_LIST_DIR}
	${lua_SOURCE_DIR}
)

target_link_libraries(
	Test_TextSample	

	PRIVATE 

	SWGTK
)

target_sources(
    Test_TextSample 

	PUBLIC 

    ${CMAKE_CURRENT_LIST_DIR}/TextTest.hpp

    PRIVATE 

    ${CMAKE_CURRENT_LIST_DIR}/TextTest.cpp
)

add_test(NAME text_test COMMAND Test_TextSample WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
