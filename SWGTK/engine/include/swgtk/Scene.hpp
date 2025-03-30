#ifndef SWGTK_SCENE_HPP
#define SWGTK_SCENE_HPP

#include <swgtk/RendererBase.hpp>

#include <concepts>

#include <sol/sol.hpp>
#include <gsl/gsl-lite.hpp>
#include <memory>
#include <filesystem>

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
			constexpr Node() = default;
			constexpr Node(const Node&) = default;
			constexpr Node(Node&&) = delete;
			constexpr Node& operator=(const Node&) = default;
			constexpr Node& operator=(Node&&) = delete;

			explicit Node(const std::shared_ptr<Node>& parent) : _parent(parent) {}

			virtual constexpr ~Node() = default;

			[[nodiscard]] virtual constexpr bool Create(Scene& scene) = 0;
			[[nodiscard]] virtual constexpr bool Update(Scene& scene, float dt) = 0;
			virtual constexpr void Destroy(Scene&) {}

		private:
			std::shared_ptr<Node> _parent;
		};

		Scene(const gsl::not_null<App*>& parent, std::shared_ptr<Node>&& node);

		[[nodiscard]] bool Create();
		[[nodiscard]] bool Update(const float dt);
		void Destroy();
		void InitLuaInput(sol::state& lua) const;

		[[nodiscard]] constexpr App* AppRoot(this auto&& self) { return self._parent; }

		template<std::derived_from<RendererBase> T>	
		[[nodiscard]] constexpr auto AppRenderer(this auto&& self) { return RenderImpl<T>(self._renderer); }

	private:
		gsl::not_null<App*> _parent;
		std::shared_ptr<RendererBase> _renderer;
		std::shared_ptr<Node> _root;

	};

	class LuaGame final : public Scene::Node {
	public:
		explicit LuaGame(const std::filesystem::path& path);

		[[nodiscard]] bool Create(Scene& scene) override;
		[[nodiscard]] bool Update(Scene& scene, float dt) override;

	private:
		sol::state _luaState;
		std::function<bool(Scene&)> _createFunc;
		std::function<bool(Scene&, float)> _updateFunc;
	};

}

#endif // !SWGTK_SCENE_HPP
