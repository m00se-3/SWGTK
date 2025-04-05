#include "swgtk/Scene.hpp"

#include <utility>
#include <swgtk/Utility.hpp>
#include <swgtk/App.hpp>

namespace swgtk
{
	Scene::Scene(const gsl::not_null<App*>& parent, std::shared_ptr<Node>&& node)
	: _parent(parent), _renderer(parent->Renderer()), _root(std::move(node))
	{
	}

	bool Scene::Create() {
		return _root->Create(*this);
	}

	bool Scene::Update(const float dt) {
		return _root->Update(*this, dt);
	}

	void Scene::Destroy() {
		_root->Destroy(*this);
	}

}
