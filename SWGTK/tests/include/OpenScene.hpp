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

	std::string _img = "C:/CppProjects/SWGTK/assets/Card Assets/Backgrounds/background_1.png";
	std::unique_ptr<swgtk::UI> _ui;

};

#endif // !TEST_OPEN_SCENE_HPP
