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
		
		bool Create(const std::string& rootDir) override;
		bool Update(float dt) override;


	private:
		Texture _background;
		std::vector<Texture> _freeTextItems;
	};

}

#endif // !SWGTK_MENUSCENE_HPP
