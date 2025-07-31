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
#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_RENDERERBASE_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_RENDERERBASE_HPP_

#include <memory>
#include <concepts>
#include <SDL3/SDL_pixels.h>
#include <swgtk/Utility.hpp>

extern "C" {
    struct SDL_Window;
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
     * @brief class RendererBase
     * 
     * A base class for the different renderers SWGTK will support. You can also create your own
     * custom implementation through this interface.
     * 
     */
    class RendererBase {
    public:
        RendererBase() = default;
        RendererBase(const RendererBase &) = default;
        RendererBase(RendererBase &&)noexcept = delete;
        RendererBase &operator=(const RendererBase &) = default;
        RendererBase &operator=(RendererBase &&) noexcept = delete;
        virtual ~RendererBase() = default;

        /**
         * @brief Clears the rendering backend and prepares it for accepting draw calls. Draws to the current
         *          render buffer if the bufferID is not valid.
         * 
         * @param color Optional color to clear the layer to. Default is Black.
         * 
         */
        constexpr virtual void BufferClear(const SDL_FColor& color = SDL_FColor{ .r=0.0f, .g=0.0f, .b=0.0f, .a=1.0f}) = 0;

        /**
         * @brief After finishing your draw calls, call this function to present the new frame
         * to the screen.
         * 
         */
        virtual void BufferPresent() = 0;

        /**
         * @brief Binds your SDL_Window object to the rendering backend and prepares the TTF_Text implementation.
         * 
         * @param window 
         * @return true on success
         * @return false failure
         */
        [[nodiscard]] virtual bool PrepareDevice(SDL_Window* window) = 0;

        [[nodiscard]] virtual bool IsDeviceInitialized() const = 0;
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

        [[nodiscard]] virtual VSync GetVSync() const = 0;

#ifdef SWGTK_BUILD_WITH_LUA
        virtual void InitLua(sol::state*) = 0;
#endif

        /**
         * @brief Get a reference pointer to the rendering context.
         * 
         * @return constexpr std::shared_ptr<RendererBase> 
         */
        constexpr virtual std::weak_ptr<RendererBase> GetRef() = 0;
    };

    /**
     * @brief Used for getting a non-owning reference to the rendering system so you can use it in your code.
     * @tparam T The Rendering system you are currently using, a child of RendererBase.
     * @param ptr A proxy wrapper to the renderer, typically obtained by calling Scene::AppRenderer().
     * @return A non-owning pointer to the exact type of renderer your game is using.
     */
    template<std::derived_from<RendererBase> T>
    [[nodiscard]] constexpr auto RenderImpl(const std::shared_ptr<RendererBase>& ptr) { return ObjectRef<T>{std::static_pointer_cast<T>(ptr).get()}; }
} // namespace swgtk

#endif // SWGTK_ENGINE_INCLUDE_SWGTK_RENDERERBASE_HPP_