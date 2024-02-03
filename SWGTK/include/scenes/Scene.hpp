#ifndef SWGTK_SCENE_HPP
#define SWGTK_SCENE_HPP

#include <string>

#include "sol/sol.hpp"

namespace swgtk
{

	class SDLApp;

	class Scene
	{
	public:
		Scene(SDLApp* app);
		virtual ~Scene() = default;

		virtual bool Create(const std::string& rootDir) = 0;
		virtual bool Update(float dt) = 0;

	protected:
		SDLApp* Parent();

		sol::state lua;

	private:
		SDLApp* _parent = nullptr;
	};

}

#endif // !SWGTK_SCENE_HPP
