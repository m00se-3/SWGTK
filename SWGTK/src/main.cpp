#define SDL_MAIN_HANDLED
#include "SDLApp.hpp"

#include "Scene.hpp"


int main(int argc, const char** argv)
{	
	auto app = swgtk::SDLApp(argc, argv);
	app.Run(swgtk::CreateLuaScene("C:/CppProjects/SWGTK/config/data/welcome_screen.lua"));

	return 0; // This is SDL, can't forget to return 0.
}
