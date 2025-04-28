/*
    MIT License
    Copyright (c) 2023 Samuel Bridgham

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_SCENE_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_SCENE_HPP_

#include <swgtk/RendererBase.hpp>

#include <concepts>

#include <sol/sol.hpp>
#include <gsl/gsl-lite.hpp>
#include <memory>

namespace swgtk
{

	class App;

	/**
		@brief Describes the game simulation.

		This intermediary class may be a needless middles man right now. However, I am looking into incorporating
		trees of swgtk::Scene::Nodes as part of the library for complex applications.

		Users should inject their code into the engine by inheriting from swgtk::Scene::Node.
	*/
	class Scene
	{
	public:
		/**
			@brief This class helps SWGTK manage the lifetime of your code. The lifetime of a Node has a similar design
			to the Scene class.

			Users will publicly inherit from this class and override the following functions:

			- Create(Scene&) is called once when the scene is initialized. This allows the program
				to initialize the simulation and allocate any needed resources.

			- Update(Scene&, float) is called once each frame. This is where all the simulation logic is
				kept, including input handling.

			- *optional* Destroy(Scene&) is called once at the end of the scene's lifetime. You only need this if you are
				using non-RAII structures for your allocated resources. (This is not recommended!)
		 */
		class Node {
		public:
			constexpr Node() = default;
			constexpr Node(const Node&) = default;
			constexpr Node(Node&&) noexcept = default;
			constexpr Node& operator=(const Node&) = default;
			constexpr Node& operator=(Node&&) noexcept = default;

			explicit Node(const std::shared_ptr<Node>& parent) : _parent(parent) {}

			virtual constexpr ~Node() = default;

			[[nodiscard]] virtual constexpr bool Create(Scene& scene) = 0;
			[[nodiscard]] virtual constexpr bool Update(Scene& scene, float dt) = 0;
			virtual constexpr void Destroy([[maybe_unused]]Scene& scene) {}

		private:
			std::shared_ptr<Node> _parent;
		};

		Scene(const gsl::not_null<App*>& parent, std::shared_ptr<Node>&& node) noexcept;

		[[nodiscard]] bool Create();
		[[nodiscard]] bool Update(float dt);
		void Destroy();

		[[nodiscard]] constexpr App* AppRoot(this auto&& self) { return self._parent; }

		template<std::derived_from<RendererBase> T>	
		[[nodiscard]] constexpr auto AppRenderer(this auto&& self) { return RenderImpl<T>(self._renderer); }

	private:
		gsl::not_null<App*> _parent;
		std::shared_ptr<RendererBase> _renderer;
		std::shared_ptr<Node> _root;

	};

} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_SCENE_HPP_
