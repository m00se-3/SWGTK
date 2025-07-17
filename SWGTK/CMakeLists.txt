add_library(swgtk STATIC)

set(SWGTK_ENGINE_INTERNALS ${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/internal)
set(SWGTK_ROOT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

target_compile_options(swgtk PRIVATE ${CompilerFlags})
target_link_options(swgtk PRIVATE ${LinkerFlags})

if(EMSCRIPTEN)
	file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/assets)

	add_custom_command(
		TARGET swgtk PRE_LINK
		COMMAND ${CMAKE_COMMAND} -E copy -t ${PROJECT_BINARY_DIR}/assets
		${CMAKE_CURRENT_LIST_DIR}/default_font/Natural_Mono-Regular.ttf
		${SWGTK_ENGINE_INTERNALS}/swgtk.lua
	)
endif()

if(DEFINED SWGTK_SANITIZE AND NOT ${SWGTK_SANITIZE} MATCHES "")
	target_compile_options(swgtk PRIVATE ${SWGTK_SANITIZE})
endif()

target_compile_features(swgtk PRIVATE cxx_std_23)

target_compile_definitions(
	swgtk 
	
	PUBLIC
	
	HAV_STRINGS_H="0" # Disable non-standard strings.
)

if(NOT EMSCRIPTEN)
	target_compile_definitions(
		swgtk PUBLIC 
		SWGTK_DEFAULT_FONT_FILE="${CMAKE_CURRENT_LIST_DIR}/default_font/Natural_Mono-Regular.ttf"
		SWGTK_TABLE_LUA_FILE="${SWGTK_ENGINE_INTERNALS}/swgtk.lua"
	)
else()
	target_compile_definitions(
		swgtk PUBLIC 

		SWGTK_DEFAULT_FONT_FILE="assets/Natural_Mono-Regular.ttf" 
		SWGTK_TABLE_LUA_FILE="assets/swgtk.lua")
endif()

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	target_compile_definitions(swgtk PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)
  
  message("-- Enabling clang-tidy.\n")
  set_target_properties(swgtk PROPERTIES CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})

endif()

if(CPPCHECK_PROGRAM)

	message("-- Enabling cppcheck.\n")
	set_target_properties(swgtk PROPERTIES CXX_CPPCHECK "${CPPCHECK_PROGRAM};--suppress=unknownMacro")

endif()

target_sources(
	swgtk	

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
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Surface.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Input.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Scene.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Simple2DRenderer.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/RendererBase.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/FontGroup.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Utility.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Math.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Timer.hpp

	PRIVATE 

	${CMAKE_CURRENT_LIST_DIR}/engine/src/App.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Scene.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/FontGroup.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Simple2DRenderer.cpp

)

target_link_libraries(
	swgtk	

	PUBLIC

	SDL3::SDL3
	SDL3_image::SDL3_image
	SDL3_ttf::SDL3_ttf
	lua
	sol2
	fmt::fmt
)

if(${SWGTK_INSTALL_FREETYPE})
	target_link_libraries(
		swgtk 

		PUBLIC 
		Freetype::Freetype
	)
endif()