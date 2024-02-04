#ifndef SWGTK_SCENE_HPP
#define SWGTK_SCENE_HPP

#include <functional>
#include <memory>
#include <concepts>

#include "sol/sol.hpp"

namespace swgtk
{

	class SDLApp;
	class Scene;

	template <typename T>
	concept SceneObject = std::derived_from<T, Scene>;

	using SceneHandle = std::unique_ptr<Scene>;
	using SceneFactory = std::function<SceneHandle()>;

	/*
		This signals the internal state machine what to do to the
		current scene next update loop.
	*/
	enum struct SceneStateCode
	{
		fail = 0,
		ok,
		change_scene
	};

	using SSC = SceneStateCode;


	class Scene
	{
	public:
		Scene(SDLApp* app);
		virtual ~Scene() = default;

		virtual SSC Create() = 0;
		virtual SSC Update(float dt) = 0;

		SceneFactory GetNextScene();

	protected:
		/*
			To change scenes you simply:

			this->nextScene = SwitchToScene<[your scene class]>([pass optional arguments]);
			this->statusCode = SSC::CHANGE_SCENE;
		*/
		template<SceneObject T, typename... Args>
		SceneFactory SwitchToScene(Args&&... args)
		{
			return []() {
				return std::make_unique<T>(std::forward<Args>(args)...);
				};
		}

		SDLApp* Parent();

		sol::state lua;
		SceneFactory nextScene;
		SSC statusCode = SSC::ok;

	private:
		SDLApp* _parent = nullptr;
	};

}

#endif // !SWGTK_SCENE_HPP
