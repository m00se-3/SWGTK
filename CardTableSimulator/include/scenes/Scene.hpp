#ifndef CTS_SCENE_HPP
#define CTS_SCENE_HPP

#include <string>

#include "sol/sol.hpp"

namespace cts
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

#endif // !CTS_SCENE_HPP
