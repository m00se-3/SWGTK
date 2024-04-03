#define SDL_MAIN_HANDLED
#include "SDLApp.hpp"

#include "MenuScene.hpp"


int main(int argc, const char** argv)
{	
	auto app = swgtk::SDLApp(std::span<const char*>{argv, static_cast<size_t>(argc)});
	app.Run(std::make_unique<swgtk::MenuScene>(&app));

	return 0; // This is SDL, can't forget to return 0.
}
