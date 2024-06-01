#define SDL_MAIN_HANDLED
#include "SDLApp.hpp"

#include "Texture.hpp"
#include "Scene.hpp"
#include "SDL_image.h"

class WelcomeScreen
{
public:
	swgtk::SSC Create(swgtk::GameScene& app)
	{
		texture = IMG_LoadTexture(app.AppRoot()->Renderer(), img.c_str());
		return swgtk::SSC::ok;
	}

	swgtk::SSC Update(swgtk::GameScene& app, [[maybe_unused]] float dt)
	{
		app.Renderer().DrawTexture(gsl::make_not_null(texture.Get()), std::nullopt, rect);
		return swgtk::SSC::ok;
	}

private:

	swgtk::Texture texture;
	SDL_FRect rect = SDL_Rect { 100.f, 100.f, 100.f, 100.f };

	std::string img = "C:/CppProjects/SWGTK/assets/Card Assets/Backgrounds/background_1.png";

};

int main(int argc, const char** argv)
{
	auto app = swgtk::SDLApp(argc, argv);
	auto client = WelcomeScreen{};

	app.Run(new swgtk::GameScene::Node {
		._updateFunc = [client = &client]([[maybe_unused]] swgtk::GameScene& app, [[maybe_unused]] float dt) {
			return client->Update(app, dt);
		},

		._createFunc = [client = &client](swgtk::GameScene& app){ 
			return client->Create(app);
		}
	});

	return 0; // This is SDL, can't forget to return 0.
}
