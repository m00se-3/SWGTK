add_executable(SWGTK_TestSuite)
add_library(SWGTK STATIC)

add_executable(SWGTK::TestSuite ALIAS SWGTK_TestSuite)

if(${CMAKE_CXX_COMPILER_ID} MATCHES MSVC)
	include("../cmake/MSVCOptions.cmake")
else()
	include("../cmake/GNUOptions.cmake")
endif()

target_compile_options(SWGTK PRIVATE ${CompilerFlags})
target_link_options(SWGTK PRIVATE ${LinkerFlags})

target_compile_options(SWGTK_TestSuite PRIVATE ${CompilerFlags})
target_link_options(SWGTK_TestSuite PRIVATE ${LinkerFlags})

if(CCACHE_PROGRAM)

	target_compile_definitions(SWGTK PRIVATE USE_CCACHE=YES)
	target_compile_definitions(SWGTK_TestSuite PRIVATE USE_CCACHE=YES)

endif()

find_program(CLANG_TIDY_LOC clang-tidy)

if(CLANG_TIDY_LOC)
  
  message("-- Enabling clang-tidy.\n")
  set_property(TARGET SWGTK PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_LOC})
  set_property(TARGET SWGTK_TestSuite PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_LOC})

endif()

set(Added_Sources "")
set(Added_Headers "")

if(SWGTK_USE_NUKLEAR)
	list(
		APPEND 
		Added_Headers

		${CMAKE_CURRENT_LIST_DIR}/engine/include/NKFont.hpp
		${CMAKE_CURRENT_LIST_DIR}/engine/include/NKUI.hpp
	)

	list(
		APPEND
		Added_Sources

		${CMAKE_CURRENT_LIST_DIR}/engine/src/nuklear.cpp
		${CMAKE_CURRENT_LIST_DIR}/engine/src/NKFont.cpp
		${CMAKE_CURRENT_LIST_DIR}/engine/src/NKUI.cpp
	)
endif()

target_sources(
	SWGTK	

	PUBLIC 

	${CMAKE_CURRENT_LIST_DIR}/engine/include/ErrCodes.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/SDLApp.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/Texture.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/Input.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/Scene.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/Component.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/RenderWrap.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/TTFFont.hpp
	${CMAKE_CURRENT_LIST_DIR}/engine/include/Project.hpp

	${Added_Headers}

	PRIVATE 

	${CMAKE_CURRENT_LIST_DIR}/engine/src/SDLApp.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Input.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Component.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Scene.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/TTFFont.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/RenderWrap.cpp

	${Added_Sources}
)

target_compile_definitions(
	SWGTK_TestSuite	

	PRIVATE 

	SWGTK_ASSETS="$ENV{Assets_Dir}"
	SWGTK_CONFIG="$ENV{Config_Dir}"
)

target_include_directories(
	SWGTK_TestSuite

	PUBLIC 

	${CMAKE_CURRENT_LIST_DIR}/engine/include
	${CMAKE_CURRENT_LIST_DIR}/tests/include
	${lua_SOURCE_DIR}
	${SDL2_SOURCE_DIR}/include
)

target_link_libraries(
	SWGTK_TestSuite

	PRIVATE 

	SDL2::SDL2main 
	SDL2::SDL2
	SDL2_image::SDL2_image
	SDL2_mixer::SDL2_mixer
	SDL2_ttf::SDL2_ttf
	gsl::gsl-lite gsl::gsl-lite-v0 gsl::gsl-lite-v1
	lua
	sol2
	SWGTK
)

target_include_directories(
	SWGTK

	PUBLIC 

	${CMAKE_CURRENT_LIST_DIR}/engine/include
	${lua_SOURCE_DIR}
	${SDL2_SOURCE_DIR}/include
	${SDL2_mixer_SOURCE_DIR}/include
	${SDL2_image_SOURCE_DIR}/include
	${SDL2_ttf_SOURCE_DIR}
	${Nuklear_SOURCE_DIR}
)

target_link_libraries(
	SWGTK	

	PRIVATE 

	SDL2::SDL2main
	SDL2::SDL2
	SDL2_image::SDL2_image
	SDL2_mixer::SDL2_mixer
	SDL2_ttf::SDL2_ttf
	gsl::gsl-lite gsl::gsl-lite-v0 gsl::gsl-lite-v1
	lua
	sol2
)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	target_compile_options(SWGTK_TestSuite PRIVATE "-g")
	target_compile_options(SWGTK PRIVATE "-g")
endif()


target_sources(
	SWGTK_TestSuite

	PUBLIC 

	${CMAKE_CURRENT_LIST_DIR}/tests/include/Tests.hpp

	PRIVATE 

	${CMAKE_CURRENT_LIST_DIR}/tests/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/tests/src/Tests.cpp
)
