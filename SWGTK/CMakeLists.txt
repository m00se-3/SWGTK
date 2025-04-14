add_library(SWGTK STATIC)

set(SWGTK_ENGINE_INTERNALS ${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/internal)

target_compile_options(SWGTK PRIVATE ${CompilerFlags})
target_link_options(SWGTK PRIVATE ${LinkerFlags})

target_compile_features(SWGTK PRIVATE cxx_std_23)

target_compile_definitions(
	SWGTK 
	
	PUBLIC 
	
	SWGTK_DEFAULT_FONT_DIR="${CMAKE_CURRENT_LIST_DIR}/default_font"
	SWGTK_ENGINE_LUA_FILE="${SWGTK_ENGINE_INTERNALS}/engine.lua"
	SWGTK_TABLE_LUA_FILE="${SWGTK_ENGINE_INTERNALS}/swgtk.lua"
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	target_compile_definitions(SWGTK PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)
  
  message("-- Enabling clang-tidy.\n")
  set_target_properties(SWGTK PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})

endif()

if(CPPCHECK_PROGRAM)

	message("--- Enabling cppcheck.\n")
	set_target_properties(SWGTK PROPERTIES CXX_CPPCHECK "${CPPCHECK_PROGRAM};--suppress=unknownMacro")

endif()

if(CPPLINT_PROGRAM)

	message("--- Enabling cpplint.\n")
	set_target_properties(SWGTK PROPERTIES CXX_CPPLINT "${CPPLINT_PROGRAM};--filter=-whitespace,-build/include_order,-runtime/indentation_namespace,-build/c++11,-build/c++17,-runtime/references,-readability/nolint")

endif()

target_include_directories(SWGTK PUBLIC ${SWGTK_ENGINE_INTERNALS})

target_sources(
	SWGTK	

	PUBLIC 

	FILE_SET HEADERS 

	BASE_DIRS

	${CMAKE_CURRENT_LIST_DIR}/engine/include
	${lua_SOURCE_DIR}

	FILES

	PUBLIC

	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/ErrCodes.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/App.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Texture.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Input.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Scene.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Simple2DRenderer.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/RendererBase.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/TTFFont.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Utility.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Timer.hpp

	PRIVATE 

	${CMAKE_CURRENT_LIST_DIR}/engine/src/App.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Scene.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/TTFFont.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Simple2DRenderer.cpp

)

target_link_libraries(
	SWGTK	

	PUBLIC

	SDL3::SDL3
	SDL3_image::SDL3_image
	SDL3_mixer::SDL3_mixer
	SDL3_ttf::SDL3_ttf
	lua
	sol2
	fmt::fmt
	gsl::gsl-lite-v1
)

add_subdirectory("tests")
add_subdirectory("lua_runner")
