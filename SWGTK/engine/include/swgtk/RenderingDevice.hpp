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
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_RENDERINGDEVICE_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_RENDERINGDEVICE_HPP_

#include <SDL3/SDL_pixels.h>
#include <concepts>
#include <memory>
#include <swgtk/Utility.hpp>

extern "C" {
struct TTF_Font;
struct SDL_Window;
struct SDL_GPUDevice;
}
#ifdef SWGTK_BUILD_WITH_LUA
namespace sol {
  class state;
}
#endif

namespace swgtk {

  inline constexpr auto default_alpha_float = 1.0f;
  inline constexpr auto default_alpha_int = 255u;

  // SDL supports a number of interesting values for vsync support.
  enum class vsync {
    adaptive = -1,
    disable = 0,
    enable = 1,
    enable_every_other = 2,
  };

  /**
   * @brief class RenderingDevice
   *
   * A base class for the different renderers SWGTK will support. You can also create your own
   * custom implementation through this interface.
   *
   */
  class rendering_device {
  public:
    rendering_device() = default;
    rendering_device(const rendering_device&) = delete;
    rendering_device(rendering_device&&) noexcept = delete;
    auto operator=(const rendering_device&) -> rendering_device& = delete;
    auto operator=(rendering_device&&) noexcept -> rendering_device& = delete;
    virtual ~rendering_device() = default;

    /**
     * @brief Clears the rendering backend and prepares it for accepting draw calls. Draws to the current
     *          render buffer if the bufferID is not valid.
     *
     * @param color Optional color to clear the layer to. Default is Black.
     *
     */
    constexpr virtual void buffer_clear(const color_t& color = color_t{}) = 0;

    /**
     * @brief After finishing your draw calls, call this function to present the new frame
     * to the screen.
     *
     */
    virtual void buffer_present() = 0;

    /** @brief Finishes initializing the device.
     * @return true if the device could be initialized, false otherwise
     */
    [[nodiscard]] virtual auto prepare_device(SDL_Window* window) -> bool = 0;

    [[nodiscard]] virtual auto is_device_initialized() const -> bool = 0;
    virtual void set_background_color(const color_t&) = 0;

    /**
     * @brief Destroy all the resources allocated by the rendering backend.
     *
     * Typically, this would occur in the derived class destructor.
     *
     */
    virtual void destroy_device() = 0;

#ifdef SWGTK_BUILD_WITH_LUA
    virtual void init_lua(sol::state*) = 0;
#endif

    /**
     * @brief Get a reference pointer to the rendering context.
     *
     * @return constexpr std::shared_ptr<RendererBase>
     */
    constexpr virtual auto get_ref() -> std::weak_ptr<rendering_device> = 0;
  };

  /**
   * @brief Used for getting a non-owning reference to the rendering system so you can use it in your code.
   * @tparam T The Rendering system you are currently using, a child of RendererBase.
   * @param ptr A proxy wrapper to the renderer, typically obtained by calling Scene::appRenderer().
   * @return A non-owning pointer to the exact type of renderer your game is using.
   */
  template<std::derived_from<rendering_device> T>
  [[nodiscard]] constexpr auto render_impl(const std::shared_ptr<rendering_device>& ptr) { return object_ref<T>{std::static_pointer_cast<T>(ptr).get()}; }
} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_RENDERINGDEVICE_HPP_
