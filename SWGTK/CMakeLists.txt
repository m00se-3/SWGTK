add_executable(Test_Suite)
add_library(SWGTK STATIC)

target_compile_definitions(SWGTK PRIVATE USE_CCACHE=YES)
target_compile_definitions(Test_Suite PRIVATE USE_CCACHE=YES)

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
	
	PRIVATE 

	${CMAKE_CURRENT_LIST_DIR}/engine/src/SDLApp.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Input.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Component.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/Scene.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/TTFFont.cpp
	${CMAKE_CURRENT_LIST_DIR}/engine/src/RenderWrap.cpp
)

target_compile_definitions(
	Test_Suite	

	PRIVATE 

	SWGTK_ASSETS="$ENV{Assets_Dir}"
	SWGTK_CONFIG="$ENV{Config_Dir}"
)

target_include_directories(
	Test_Suite

	PUBLIC 

	${CMAKE_CURRENT_LIST_DIR}/engine/include
	${CMAKE_CURRENT_LIST_DIR}/tests/include
	${lua_SOURCE_DIR}
	${SDL2_SOURCE_DIR}/include
	${Nuklear_SOURCE_DIR}
)

target_link_libraries(
	Test_Suite

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
	target_compile_options(Test_Suite PRIVATE "-g")
	target_compile_options(SWGTK PRIVATE "-g")
endif()


target_sources(
	Test_Suite

	PUBLIC 

	${CMAKE_CURRENT_LIST_DIR}/tests/include/NKFont.hpp
	${CMAKE_CURRENT_LIST_DIR}/tests/include/UI.hpp
	${CMAKE_CURRENT_LIST_DIR}/tests/include/Tests.hpp

	PRIVATE 

	${CMAKE_CURRENT_LIST_DIR}/tests/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/tests/src/nuklear.cpp
	${CMAKE_CURRENT_LIST_DIR}/tests/src/NKFont.cpp
	${CMAKE_CURRENT_LIST_DIR}/tests/src/UI.cpp
	${CMAKE_CURRENT_LIST_DIR}/tests/src/Tests.cpp
)
