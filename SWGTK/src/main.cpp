#define SDL_MAIN_HANDLED
#include "SDLApp.hpp"

#include "MenuScene.hpp"


int main(int argc, const char** argv)
{	
	auto app = swgtk::SDLApp(argc, argv);
	app.Run<swgtk::MenuScene>();

	return 0; // This is SDL, can't forget to return 0.
}
