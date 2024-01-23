#ifndef CTS_SCENE_HPP
#define CTS_SCENE_HPP

#include <string>

#include "sol/sol.hpp"

#include "Entity.hpp"

extern "C"
{
	struct SDL_Renderer;
}

namespace cts
{

	class Scene
	{
	public:
		Scene(SDL_Renderer* ren);
		~Scene() = default;

	protected:
		virtual bool Init(const std::string& gameDir) = 0;
		virtual bool Update(float dt) = 0;


	private:
		SDL_Renderer* _renderer = nullptr;	// Does not own the pointer.
		sol::state _lua;
		Registry _registry;
	};

}

#endif // !CTS_SCENE_HPP
