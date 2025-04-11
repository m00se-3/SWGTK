add_executable(Test_TextSampleLua)

target_compile_options(Test_TextSampleLua PRIVATE ${CompilerFlags})
target_link_options(Test_TextSampleLua PRIVATE ${LinkerFlags})

target_compile_features(Test_TextSampleLua PRIVATE cxx_std_23)

target_compile_definitions(
		Test_TextSampleLua

	PRIVATE 

	TEST_LUA_FILE="${CMAKE_CURRENT_LIST_DIR}/TextTest.lua"
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	target_compile_definitions(Test_TextSampleLua PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)
  
  set_property(TARGET Test_TextSampleLua PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})

endif()

if(CPPCHECK_PROGRAM)

	set_target_properties(Test_TextSampleLua PROPERTIES CXX_CPPCHECK ${CPPCHECK_PROGRAM})

endif()

target_include_directories(
		Test_TextSampleLua

	PUBLIC 

	${CMAKE_CURRENT_LIST_DIR}/../../../engine/include
	${CMAKE_CURRENT_LIST_DIR}
	${lua_SOURCE_DIR}
)

target_link_libraries(
		Test_TextSampleLua

	PRIVATE 

	SWGTK
)

target_sources(
		Test_TextSampleLua

    PRIVATE 

    ${CMAKE_CURRENT_LIST_DIR}/TextTestLua.cpp
)

add_test(NAME text_test_lua COMMAND Test_TextSampleLua WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
