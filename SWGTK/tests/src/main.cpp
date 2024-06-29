#define SDL_MAIN_HANDLED
#include "SDLApp.hpp"
#include "Tests.hpp"

int main(int argc, const char** argv)
{
	auto app = swgtk::SDLApp(argc, argv);
	auto suite = swgtk::tests::Test_Suite{};
	auto none = swgtk::tests::TestNone{&suite};

	suite.Config(app);

	app.Run(new swgtk::GameScene::Node {
		._createFunc = [&none](swgtk::GameScene& app) -> auto
		{ 
			return none.Create(app);
		},

		._updateFunc = [&none](swgtk::GameScene& app, float dt) -> auto 
		{
			return none.Update(app, dt);
		},
	});

	return 0; // This is SDL, can't forget to return 0.
}
