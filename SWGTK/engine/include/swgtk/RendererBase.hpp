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
        static constexpr float defaultAlphaFloat = 1.0f;

    public:
        RendererBase() = default;
        RendererBase(const RendererBase &) = default;
        RendererBase(RendererBase &&) = delete;
        RendererBase &operator=(const RendererBase &) = default;
        RendererBase &operator=(RendererBase &&) = delete;
        virtual ~RendererBase() = default;

        /**
         * @brief Clears the rendering backend and prepares it for accepting draw calls.
         * 
         */
        constexpr virtual void BufferClear() = 0;

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
        [[nodiscard]] virtual bool PrepareDevice(SDL_Window* window, SDL_FColor bgColor = SDL_FColor{.r=0.0f, .g=0.0f, .b=0.0f, .a=defaultAlphaFloat}) = 0;

        [[nodiscard]] constexpr virtual bool IsDeviceInitialized() const = 0;
        constexpr virtual void SetBackgroundColor(const SDL_FColor&) = 0;

        /**
         * @brief Destroy all the resources allocated by the rendering backend.
         * 
         * Typically, this would occur in the derived class destructor.
         * 
         */
        virtual void DestroyDevice() = 0;

        template<std::derived_from<RendererBase> T>
        [[nodiscard]] T* GetImpl() { return static_cast<T*>(this); }
    };

    template<std::derived_from<RendererBase> T, typename... Args>
    [[nodiscard]] constexpr std::shared_ptr<T> CreateRenderer(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#endif