#include "scenes/Scene.hpp"

#include "SDLApp.hpp"

namespace swgtk
{
	Scene::Scene(SDLApp* app)
		: _parent(app)
	{

	}

	SceneFactory Scene::GetNextScene()
	{
		return nextScene;
	}

	SDLApp* Scene::Parent()
	{
		return _parent;
	}
}