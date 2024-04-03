#ifndef SWGTK_MENUSCENE_HPP
#define SWGTK_MENUSCENE_HPP

#include <vector>

#include "Scene.hpp"
#include "Texture.hpp"

namespace swgtk
{

	class MenuScene : public Scene
	{
	public:
		MenuScene(SDLApp* app);
		
		SSC Create() override;
		SSC Update(float dt) override;


	private:

		void MainMenuScripts();	

		Texture _background;
		std::vector<Texture> _freeTextItems;
		std::vector<SDL_Rect> _freeTextBounds;
	};

}

#endif // !SWGTK_MENUSCENE_HPP
