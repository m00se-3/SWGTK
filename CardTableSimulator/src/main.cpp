#include "SDLApp.hpp"


int main(int argc, char** argv)
{	
	auto app = cts::SDLApp(argc, argv);
	app.Run();

	return 0; // This is SDL, can't forget to return 0.
}
