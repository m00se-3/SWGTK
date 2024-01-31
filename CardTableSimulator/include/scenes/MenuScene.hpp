#ifndef CTS_MENUSCENE_HPP
#define CTS_MENUSCENE_HPP

#include "Scene.hpp"

namespace cts
{

	class MenuScene : public Scene
	{
	public:
		MenuScene(SDLApp* app);
		
		bool Create(const std::string& rootDir) override;
		bool Update(float dt) override;


	private:

	};

}

#endif // !CTS_MENUSCENE_HPP
