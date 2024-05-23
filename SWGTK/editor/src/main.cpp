#define SDL_MAIN_HANDLED
#include "SDLApp.hpp"

#include "Scene.hpp"


int main(int argc, const char** argv)
{
	auto welcome = swgtk::GameScene::Node {
		._updateFunc = []([[maybe_unused]] swgtk::GameScene& app, [[maybe_unused]] float dt) {
			return swgtk::SSC::ok;
		},
		._createFunc = [](swgtk::GameScene&){ return swgtk::SSC::ok; }
	};

	auto app = swgtk::SDLApp(argc, argv);
	app.Run(gsl::owner<swgtk::GameScene::Node*>{&welcome});

	return 0; // This is SDL, can't forget to return 0.
}
