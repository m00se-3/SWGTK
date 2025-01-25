add_library(SWGTK STATIC)

target_compile_options(SWGTK PRIVATE ${CompilerFlags})
target_link_options(SWGTK PRIVATE ${LinkerFlags})

target_compile_features(SWGTK PRIVATE cxx_std_23)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	target_compile_definitions(SWGTK PRIVATE _DEBUG)
endif()

if(CLANG_TIDY_PROGRAM)
  
  message("-- Enabling clang-tidy.\n")
  set_property(TARGET SWGTK PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_PROGRAM})

endif()

set(Added_Sources "")
set(Added_Headers "")

target_sources(
	SWGTK	

	PUBLIC 

	FILE_SET HEADERS 

	BASE_DIRS

	${CMAKE_CURRENT_LIST_DIR}/engine/include
	${lua_SOURCE_DIR}

	FILES

	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/ErrCodes.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/App.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Texture.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Input.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Scene.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Simple2DRenderer.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/RendererBase.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/TTFFont.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/swgtk/Macros.hpp

	${Added_Headers}

	PRIVATE 

	${CMAKE_CURRENT_LIST_DIR}/engine/src/App.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Input.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Scene.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/TTFFont.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Simple2DRenderer.cpp

	${Added_Sources}
)

target_link_libraries(
	SWGTK	

	PRIVATE 

	SDL3::SDL3
	SDL3_image::SDL3_image
	SDL3_mixer::SDL3_mixer
	SDL3_ttf::SDL3_ttf
	lua
	sol2
	fmt::fmt
)

add_subdirectory("tests")
