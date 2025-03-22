#ifndef SWGTK_SCENE_HPP
#define SWGTK_SCENE_HPP

#include "swgtk/RendererBase.hpp"
#include <concepts>
#include <optional>
#include <swgtk/Input.hpp>
#include <swgtk/Simple2DRenderer.hpp>

#include <SDL3/SDL_rect.h>
#include <sol/sol.hpp>
#include <functional>
#include <memory>

namespace swgtk
{

	class App;

	/*
		This signals the internal state machine what to do to the
		current scene next update loop.
	*/
	enum struct SceneStateCode : uint8_t
	{
		Fail = 0,
		Ok,
		ChangeScene
	};

	using SSC = SceneStateCode;

	/*
		Describes the game simulation.

		A game scene has the following lifetime:

		- Create(Scene&) is called once after the scene is initialized. This allows the program
			to initialize the simulation and allocate any needed resources.

		- Update(Scene&, float) is called once each frame. This is where all the simulation logic is
			kept, including input handling.
		
		- Render(Scene&, float) is called once each frame after Update(). Your drawing code goes here.

		- Destroy(Scene&) is called once at the end of the scene's lifetime. This function is *optional*.
			You only need to use it if you are using non-RAII structures for your allocated
			resources. (This is not recommended!)
	*/
	class Scene
	{
	public:

		class Node final {
		public:
			Node(const Node &) = default;
			Node(Node &&) = delete;
			Node &operator=(const Node &) = default;
			Node &operator=(Node &&) = delete;
			Node(
				const std::function<SSC(Scene &)> &cr,
				const std::function<SSC(Scene &, float)> &up,
				const std::optional<std::function<void(Scene &)>> &ds =
					std::nullopt)
				: _createFunc(cr), _updateFunc(up), _destroyFunc(ds) {}

			virtual constexpr ~Node() = default;

			[[nodiscard]] constexpr SSC Create(this auto &&self, Scene &scene) { return self._createFunc(scene); }
			[[nodiscard]] constexpr SSC Update(this auto&& self, Scene& scene, float dt) { return self._updateFunc(scene, dt); }
			constexpr void Destroy(this auto&& self, Scene& scene) { if(self._destroyFunc) { (self._destroyFunc.value())(scene); } }

		private:
			std::function<SSC(Scene&)> _createFunc;
			std::function<SSC(Scene&, float)> _updateFunc;
			std::optional<std::function<void(Scene&)>> _destroyFunc;
		};

		struct NodeProxy {
			constexpr NodeProxy() = default;
			constexpr explicit NodeProxy(Node* node) : ptr(node){}
			Node* ptr = nullptr;
		};
	
		Scene(App* parent, NodeProxy node);

		[[nodiscard]] static constexpr NodeProxy CreateSceneNode(
			const std::function<SSC(Scene&)>& createFunc,
			const std::function<SSC(Scene&, float)>& updateFunc,
			const std::optional<std::function<void(Scene&)>>& destroyFunc = std::nullopt
		) {
			return NodeProxy {
				new Node { createFunc, updateFunc, destroyFunc }
			};
		}

		[[nodiscard]] SSC Create();
		[[nodiscard]] SSC Update(float dt);
		void Destroy();
		static void InitLua(sol::state& lua);
		void InitLuaInput(sol::state& lua);
		void SetNewScene(Node* scene); // The App uses this function.
		void GenerateNewScene(NodeProxy ptr); // The user uses this function.

		[[nodiscard]] constexpr App* AppRoot(this auto&& self) { return self._parent; }

		template<std::derived_from<RendererBase> T>	
		[[nodiscard]] constexpr auto AppRenderer(this auto&& self) { return RenderImpl<T>(self._renderer); }

	private:
		App* _parent = nullptr;
		std::shared_ptr<RendererBase> _renderer;
		std::unique_ptr<Node> _root;
		SSC _sceneState = SSC::Ok;

	};

	[[nodiscard]] Scene::NodeProxy CreateLuaScene(sol::state& state, const std::string& luaFileName);

}

#endif // !SWGTK_SCENE_HPP
