#define SDL_MAIN_HANDLED
#include "SDLApp.hpp"
#include "OpenScene.hpp"

int main(int argc, const char** argv)
{
	auto app = swgtk::SDLApp(argc, argv);
	auto client = OpenScene{};

	app.Run(new swgtk::GameScene::Node {
		._updateFunc = [&client](swgtk::GameScene& app, float dt) -> auto 
		{
			return client.Update(app, dt);
		},

		._createFunc = [&client](swgtk::GameScene& app) -> auto
		{ 
			return client.Create(app);
		}
	});

	return 0; // This is SDL, can't forget to return 0.
}
