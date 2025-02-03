#ifndef SWGTK_RENDERERBASE_HPP
#define SWGTK_RENDERERBASE_HPP

#include <memory>
#include <concepts>
#include <SDL3/SDL_pixels.h>

extern "C" {
    struct SDL_Window;
}

namespace swgtk {

    /**
     * @brief class RendererBase
     * 
     * A base class for the different renderers SWGTK will support.
     * 
     */
    class RendererBase {
    protected:
        static constexpr auto defaultAlphaFloat = 1.0f;
        static constexpr auto defaultAlphaInt = 255u;

    public:
        RendererBase() = default;
        RendererBase(const RendererBase &) = default;
        RendererBase(RendererBase &&) = delete;
        RendererBase &operator=(const RendererBase &) = default;
        RendererBase &operator=(RendererBase &&) = delete;
        virtual ~RendererBase() = default;

        /**
         * @brief Clears the rendering backend and prepares it for accepting draw calls. Draws to the current
         *          render buffer if the bufferID is not valid.
         * 
         * @param bufferID - The layer number to clear. Default is 0.
         * @param color    - Optional color to clear the layer to. Default is Black.
         * 
         */
        constexpr virtual void BufferClear(SDL_FColor color = SDL_FColor{ .r=0.0f, .g=0.0f, .b=0.0f, .a=1.0f}) = 0;

        /**
         * @brief After finishing your draw calls, call this function to present the new frame
         * to the screen.
         * 
         */
        constexpr virtual void BufferPresent() = 0;

        /**
         * @brief Binds your SDL_Window object to the rendering backend and prepares the TTF_Text implementation.
         * 
         * @param window 
         * @return true on sucess
         * @return false failure
         */
        [[nodiscard]] virtual bool PrepareDevice(SDL_Window* window) = 0;

        [[nodiscard]] constexpr virtual bool IsDeviceInitialized() const = 0;
        constexpr virtual void SetBackgroundColor(const SDL_FColor&) = 0;

        /**
         * @brief Destroy all the resources allocated by the rendering backend.
         * 
         * Typically, this would occur in the derived class destructor.
         * 
         */
        virtual void DestroyDevice() = 0;

        constexpr virtual std::shared_ptr<RendererBase> GetRef() = 0;
    };

    template<std::derived_from<RendererBase> T>
    [[nodiscard]] constexpr T* RenderImpl(std::shared_ptr<RendererBase> ptr) { return dynamic_cast<T*>(ptr.get()); }    
}

#endif