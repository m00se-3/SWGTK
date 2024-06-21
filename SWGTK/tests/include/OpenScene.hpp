#ifndef TEST_OPEN_SCENE_HPP
#define TEST_OPEN_SCENE_HPP

#include "Texture.hpp"
#include "Scene.hpp"
#include "UI.hpp"

class OpenScene
{
public:
	swgtk::SSC Create(swgtk::GameScene& app);
	swgtk::SSC Update(swgtk::GameScene& app, [[maybe_unused]] float dt);

	static nk_keys SDLKeytoNKKey(int key, uint16_t mods);
        static int SDLButtontoNKButton(uint8_t button);

private:

	swgtk::Texture _texture;
	SDL_FRect _rect = SDL_FRect { 100.f, 100.f, 100.f, 100.f };

	std::string _config = SWGTK_CONFIG;
	std::string _assets = SWGTK_ASSETS;
	std::unique_ptr<swgtk::UI> _ui;

};

#endif // !TEST_OPEN_SCENE_HPP
