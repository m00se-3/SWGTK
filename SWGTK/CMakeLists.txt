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

	${CMAKE_CURRENT_LIST_DIR}/engine/include/ErrCodes.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/SDLApp.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/Texture.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/Input.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/Scene.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/RenderWrap.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/TTFFont.hpp

	${Added_Headers}

	PRIVATE 

	${CMAKE_CURRENT_LIST_DIR}/engine/src/SDLApp.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Input.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Scene.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/TTFFont.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/RenderWrap.cpp

	${Added_Sources}
)

target_include_directories(
	SWGTK

	PUBLIC 

	${CMAKE_CURRENT_LIST_DIR}/engine/include
	${lua_SOURCE_DIR}
)

target_link_libraries(
	SWGTK	

	PRIVATE 

	SDL3::SDL3
	SDL3_image::SDL3_image
	SDL3_mixer::SDL3_mixer
	SDL3_ttf::SDL3_ttf
	gsl::gsl-lite gsl::gsl-lite-v0 gsl::gsl-lite-v1
	lua
	sol2
)

add_subdirectory("tests")
