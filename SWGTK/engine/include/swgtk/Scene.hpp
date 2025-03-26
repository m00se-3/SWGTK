#ifndef SWGTK_SCENE_HPP
#define SWGTK_SCENE_HPP

#include "swgtk/RendererBase.hpp"
#include <concepts>
#include <optional>
#include <swgtk/Simple2DRenderer.hpp>

#include <sol/sol.hpp>
#include <functional>
#include <memory>

namespace swgtk
{

	class App;

	/*
		Describes the game simulation.

		A game scene has the following lifetime:

		- Create(Scene&) is called once after the scene is initialized. This allows the program
			to initialize the simulation and allocate any needed resources.

		- Update(Scene&, float) is called once each frame. This is where all the simulation logic is
			kept, including input handling.
		
		- Destroy(Scene&) is called once at the end of the scene's lifetime. This function is *optional*.
			You only need to use it if you are using non-RAII structures for your allocated
			resources. (This is not recommended!)
	*/
	class Scene
	{
	public:

		class Node {
		public:
			Node(const Node&) = default;
			Node(Node&&) = delete;
			Node& operator=(const Node&) = default;
			Node& operator=(Node&&) = delete;
			Node(
				const std::function<bool(Scene&)>& cr,
				const std::function<bool(Scene&, float)>& up,
				const std::optional<std::function<void(Scene&)>>& ds =
					std::nullopt)
				: _createFunc(cr), _updateFunc(up), _destroyFunc(ds) {}

			virtual constexpr ~Node() = default;

			[[nodiscard]] constexpr bool Create(this auto&& self, Scene& scene) { return self._createFunc(scene); }
			[[nodiscard]] constexpr bool Update(this auto&& self, Scene& scene, float dt) { return self._updateFunc(scene, dt); }
			constexpr void Destroy(this auto&& self, Scene& scene) { if(self._destroyFunc) { (self._destroyFunc.value())(scene); } }

		private:
			std::function<bool(Scene&)> _createFunc;
			std::function<bool(Scene&, float)> _updateFunc;
			std::optional<std::function<void(Scene&)>> _destroyFunc;
		};

		Scene(App* parent, const std::function<bool(Scene&)>& createFunc,
		      const std::function<bool(Scene&, float)>& updateFunc,
		      const std::optional<std::function<void(Scene&)>>& destroyFunc = std::nullopt);

		[[nodiscard]] bool Create();
		[[nodiscard]] bool Update(const float dt);
		void Destroy();
		void InitLuaInput(sol::state& lua) const;

		[[nodiscard]] constexpr App* AppRoot(this auto&& self) { return self._parent; }

		template<std::derived_from<RendererBase> T>	
		[[nodiscard]] constexpr auto AppRenderer(this auto&& self) { return RenderImpl<T>(self._renderer); }

	private:
		App* _parent = nullptr;
		std::shared_ptr<RendererBase> _renderer;
		std::unique_ptr<Node> _root;

	};

}

#endif // !SWGTK_SCENE_HPP
