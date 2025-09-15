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

#include <concepts>
#include <memory>
#include <swgtk/RenderingDevice.hpp>
#include <swgtk/Utility.hpp>

namespace swgtk {

  class App;

  /**
    @brief Describes the game simulation.

    This intermediary class may be a needless middle man right now. However, I am looking into incorporating
    trees of swgtk::Scene::Nodes as part of the library for complex applications.

    Users should inject their code into the engine by inheriting from swgtk::Scene::Node.
  */
  class Scene {
  public:
    /**
      @brief This class helps SWGTK manage the lifetime of your code. The lifetime of a Node has a similar design
      to the Scene class.

      Users will publicly inherit from this class and override the following functions:

      - Create() is called once when the scene is initialized. This allows the program
        to initialize the simulation and allocate any needed resources.

      - Update(float) is called once each frame. This is where all the simulation logic is
        kept, including input handling.

      - *optional* Destroy() is called once at the end of the scene's lifetime. You only need this if you are
        using non-RAII structures for your allocated resources. (This is not recommended!)
     */
    class Node {
    public:
      Node(const Node&) = default;
      Node(Node&&) noexcept = default;
      auto operator=(const Node&) -> Node& = default;
      auto operator=(Node&&) noexcept -> Node& = default;

      explicit Node(const ObjectRef<Scene>& scene) :
          _scene(scene) {}
      explicit Node(const std::shared_ptr<Node>& parent) :
          _scene(parent->GetScene()), _parent(parent) {}

      virtual ~Node() = default;

      [[nodiscard]] virtual constexpr auto Create() -> bool = 0;
      [[nodiscard]] virtual constexpr auto Update(float dt) -> bool = 0;
      virtual constexpr void Destroy() {}

      [[nodiscard]] constexpr auto GetScene() const -> ObjectRef<Scene> { return _scene; }

      template<std::derived_from<Node> T>
      [[nodiscard]] constexpr auto GetParent() const -> std::weak_ptr<T> { return std::static_pointer_cast<T>(_parent.lock()); }

    protected:
      ObjectRef<Scene> _scene;
      std::weak_ptr<Node> _parent;
    };

    explicit Scene(const ObjectRef<App>& parent);

    template<std::derived_from<Node> T>
    void AddRootNode(auto&&... args) {
      _root = std::make_shared<T>(GetScene(), std::forward<decltype(args)>(args)...);
    }

    [[nodiscard]] auto Create() const -> bool;
    [[nodiscard]] auto Update(float dt) const -> bool;
    void Destroy() const;

    template<std::derived_from<Node> T>
    [[nodiscard]] constexpr auto GetRootNode() -> std::shared_ptr<T> { return std::static_pointer_cast<T>(_root); }
    [[nodiscard]] constexpr auto GetApp() const -> ObjectRef<App> { return ObjectRef<App>{_parent}; }
    [[nodiscard]] auto GetScene() -> ObjectRef<Scene> { return ObjectRef<Scene>{this}; }

    template<std::derived_from<RenderingDevice> T>
    [[nodiscard]] constexpr auto AppRenderer() const { return RenderImpl<T>(_renderer); }

  private:
    ObjectRef<App> _parent;
    std::shared_ptr<RenderingDevice> _renderer;
    std::shared_ptr<Node> _root;
  };

} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_SCENE_HPP_
