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

  class app;

  /**
    @brief Describes the game simulation.

    By default, this class acts as a gateway into the Scene::Node that your application logic is in.
    However, users have the option of inheriting from this class if they require more control over
    their application's global state.

    Note: This class has no functions to override.

    Users should inject their code into the engine by inheriting from swgtk::Scene::Node.
  */
  class scene {
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
    class node {
      public:
      node(const node&) = default;
      node(node&&) noexcept = default;
      auto operator=(const node&) -> node& = default;
      auto operator=(node&&) noexcept -> node& = default;

      explicit node(const object_ref<scene>& scene) :
          _scene(scene) {}
      explicit node(const std::shared_ptr<node>& parent) :
          _scene(parent->get_scene()), _parent(parent) {}

      virtual ~node() = default;

      [[nodiscard]] virtual constexpr auto create() -> bool = 0;
      [[nodiscard]] virtual constexpr auto update(float dt) -> bool = 0;
      virtual constexpr void destroy() {}

      // Get the Scene object managing the game.
      [[nodiscard]] auto get_scene() const -> object_ref<scene> { return _scene; }

      /// Get the parent of the current node.
      /// Returns a std::weak_ptr to the parent if it exists. Returns an empty std::weak_ptr if it doesn't.(i.e. root node)
      template<std::derived_from<node> T>
      [[nodiscard]] auto get_parent() const -> std::weak_ptr<T> { return std::static_pointer_cast<T>(_parent.lock()); }
      
      template<std::derived_from<node> T, typename... Args>
      [[nodiscard]] auto create_child(Args&&... args) -> std::shared_ptr<T> { return std::make_shared<T>(std::forward<Args>(args)...); }

      protected:
      object_ref<scene> _scene;
      std::weak_ptr<node> _parent;
    };

    scene(const scene&) = delete;
    scene(scene&&) = delete;
    auto operator=(const scene&) -> scene& = delete;
    auto operator=(scene&&) -> scene& = delete;
    explicit scene(const object_ref<app>& parent);
    virtual ~scene() = default;

    template<std::derived_from<node> T>
    void add_root_node(auto&&... args) {
      _root = std::make_shared<T>(get_scene(), std::forward<decltype(args)>(args)...);
    }

    [[nodiscard]] auto create() const -> bool;
    [[nodiscard]] auto update(float dt) const -> bool;
    void destroy() const;

    template<std::derived_from<node> T>
    [[nodiscard]] constexpr auto get_root_node() -> std::shared_ptr<T> { return std::static_pointer_cast<T>(_root); }
    [[nodiscard]] constexpr auto get_app() const -> object_ref<app> { return object_ref<app>{_parent}; }
    [[nodiscard]] auto get_scene() -> object_ref<scene> { return object_ref<scene>{this}; }

    template<std::derived_from<rendering_device> T>
    [[nodiscard]] constexpr auto app_renderer() const { return render_impl<T>(_renderer); }

    private:
    object_ref<app> _parent;
    std::shared_ptr<rendering_device> _renderer;
    std::shared_ptr<node> _root;
  };

} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_SCENE_HPP_
