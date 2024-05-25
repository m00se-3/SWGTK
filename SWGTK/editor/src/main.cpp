#include <SDL_render.h>
#define SDL_MAIN_HANDLED
#include "SDLApp.hpp"

#include "Scene.hpp"
#include "SDL_image.h"

class WelcomeScreen
{
public:
	void Create(swgtk::GameScene& app)
	{
		texture = IMG_LoadTexture(app.AppRoot()->Renderer(), img.c_str());
	}

	void Update(swgtk::GameScene& app)
	{
		SDL_RenderCopy(app.AppRoot()->Renderer(), texture, nullptr, &rect);
	}

private:

	SDL_Texture* texture = nullptr;
	SDL_Rect rect = SDL_Rect { 100, 100, 100, 100 };

	std::string img = "C:/CppProjects/SWGTK/assets/Card Assets/Backgrounds/background_1.png";

};

int main(int argc, const char** argv)
{
	auto app = swgtk::SDLApp(argc, argv);
	auto client = WelcomeScreen{};

	app.Run(new swgtk::GameScene::Node {
		._updateFunc = [client = &client]([[maybe_unused]] swgtk::GameScene& app, [[maybe_unused]] float dt) {
			client->Update(app);
			return swgtk::SSC::ok;
		},

		._createFunc = [client = &client](swgtk::GameScene& app){ 
			client->Create(app);
			return swgtk::SSC::ok;
		}
	});

	return 0; // This is SDL, can't forget to return 0.
}
