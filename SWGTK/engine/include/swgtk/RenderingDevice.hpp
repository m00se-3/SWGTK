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
#include <any>
#include <concepts>
#include <memory>
#include <swgtk/Utility.hpp>

extern "C" {
struct TTF_Font;
}
#ifdef SWGTK_BUILD_WITH_LUA
namespace sol {
  class state;
}
#endif

namespace swgtk {

  static constexpr auto defaultAlphaFloat = 1.0f;
  static constexpr auto defaultAlphaInt = 255u;

  // SDL supports a number of interesting values for vsync support.
  enum class VSync {
    Adaptive = -1,
    Disable = 0,
    Enable = 1,
    EnableEveryOther = 2,
  };

  /**
   * @brief class RenderingDevice
   *
   * A base class for the different renderers SWGTK will support. You can also create your own
   * custom implementation through this interface.
   *
   */
  class RenderingDevice {
  public:
    RenderingDevice() = default;
    RenderingDevice(const RenderingDevice&) = default;
    RenderingDevice(RenderingDevice&&) noexcept = delete;
    auto operator=(const RenderingDevice&) -> RenderingDevice& = default;
    auto operator=(RenderingDevice&&) noexcept -> RenderingDevice& = delete;
    virtual ~RenderingDevice() = default;

    /**
     * @brief Clears the rendering backend and prepares it for accepting draw calls. Draws to the current
     *          render buffer if the bufferID is not valid.
     *
     * @param color Optional color to clear the layer to. Default is Black.
     *
     */
    constexpr virtual void BufferClear(const SDL_FColor& color = SDL_FColor{.r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f}) = 0;

    /**
     * @brief After finishing your draw calls, call this function to present the new frame
     * to the screen.
     *
     */
    virtual void BufferPresent() = 0;

    /** @brief Finishes initializing the device.
     *
     * @param dependency An implementation dependency may be unique to a derived class. If more than one value
     * is needed, consider passing a struct. **We recommend using pointers to objects if you don't want exceptions.**
     * @return true if the device could be initialized, false otherwise
     */
    [[nodiscard]] virtual auto PrepareDevice(const std::any& dependency) -> bool = 0;

    [[nodiscard]] virtual auto IsDeviceInitialized() const -> bool = 0;
    virtual void SetBackgroundColor(const SDL_FColor&) = 0;
    virtual void SetFont(TTF_Font* font) = 0;

    /**
     * @brief Destroy all the resources allocated by the rendering backend.
     *
     * Typically, this would occur in the derived class destructor.
     *
     */
    virtual void DestroyDevice() = 0;

    virtual void SetVSync(VSync value) = 0;

    [[nodiscard]] virtual auto GetVSync() const -> VSync = 0;

#ifdef SWGTK_BUILD_WITH_LUA
    virtual void InitLua(sol::state*) = 0;
#endif

    /**
     * @brief Get a reference pointer to the rendering context.
     *
     * @return constexpr std::shared_ptr<RendererBase>
     */
    constexpr virtual auto GetRef() -> std::weak_ptr<RenderingDevice> = 0;
  };

  /**
   * @brief Used for getting a non-owning reference to the rendering system so you can use it in your code.
   * @tparam T The Rendering system you are currently using, a child of RendererBase.
   * @param ptr A proxy wrapper to the renderer, typically obtained by calling Scene::AppRenderer().
   * @return A non-owning pointer to the exact type of renderer your game is using.
   */
  template<std::derived_from<RenderingDevice> T>
  [[nodiscard]] constexpr auto RenderImpl(const std::shared_ptr<RenderingDevice>& ptr) { return ObjectRef<T>{std::static_pointer_cast<T>(ptr).get()}; }
} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_RENDERINGDEVICE_HPP_
