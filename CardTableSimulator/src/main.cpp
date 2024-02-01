#include "SDLApp.hpp"

#include "scenes/MenuScene.hpp"


int main(int argc, char** argv)
{	
	auto app = cts::SDLApp(argc, argv);
	app.Run(std::make_unique<cts::MenuScene>(&app));

	return 0; // This is SDL, can't forget to return 0.
}
