#define SDL_MAIN_HANDLED
#include "SDLApp.hpp"
#include "OpenScene.hpp"

int main(int argc, const char** argv)
{
	using namespace std::placeholders;

	auto app = swgtk::SDLApp(argc, argv);
	auto client = OpenScene{};

	app.Run(new swgtk::GameScene::Node {
		._createFunc = [&client](swgtk::GameScene& app) -> auto
		{ 
			return client.Create(app);
		},

		._updateFunc = [&client](swgtk::GameScene& app, float dt) -> auto 
		{
			return client.Update(app, dt);
		},
	});

	return 0; // This is SDL, can't forget to return 0.
}
