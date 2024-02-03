#include "SDLApp.hpp"

#include "scenes/MenuScene.hpp"


int main(int argc, char** argv)
{	
	auto app = swgtk::SDLApp(argc, argv);
	app.Run(std::make_unique<swgtk::MenuScene>(&app));

	return 0; // This is SDL, can't forget to return 0.
}
