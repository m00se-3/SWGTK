#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_GPU_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_GPU_HPP_

#include <swgtk/RenderingDevice.hpp>
#include <swgtk/Texture.hpp>
#include <utility>
#include <SDL3_ttf/SDL_textengine.h>

namespace swgtk {

  enum class ShaderFormat : std::uint32_t {
    Invalid = SDL_GPU_SHADERFORMAT_INVALID,
    Private = SDL_GPU_SHADERFORMAT_PRIVATE,
    Spirv = SDL_GPU_SHADERFORMAT_SPIRV,
    Dxbc = SDL_GPU_SHADERFORMAT_DXBC,
    Dxil = SDL_GPU_SHADERFORMAT_DXIL,
    Msl = SDL_GPU_SHADERFORMAT_MSL,
    Metallib = SDL_GPU_SHADERFORMAT_METALLIB,
  };

  enum class GPUAPI : std::uint32_t {
    None = 0u,
    Vulkan = 1u,
    D3D12 = 2u,
    Metal = 3u,
  };

  struct GPU_Params {
    ShaderFormat format{ShaderFormat::Spirv};
    bool debugMode{false};
    GPUAPI apiName;
  };

  class SDLGPU : public RenderingDevice, public std::enable_shared_from_this<SDLGPU> {
    public:
    constexpr SDLGPU() = default;
    explicit constexpr SDLGPU(GPU_Params params) : _params(params) {}

    constexpr SDLGPU(const SDLGPU&) = delete;
    constexpr auto operator=(const SDLGPU&) -> SDLGPU& = delete;
    constexpr SDLGPU(SDLGPU&&) = delete;
    constexpr auto operator=(SDLGPU&&) -> SDLGPU& = delete;
    ~SDLGPU() override { SDLGPU::DestroyDevice(); }

    auto BufferClear(const SDL_FColor& color = SDL_FColor{.r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f}) -> void override;
    auto BufferPresent() -> void override;

    auto PrepareDevice(SDL_Window* window) -> bool override;
    auto DestroyDevice() -> void override;
    auto IsDeviceInitialized() const -> bool override;
    [[nodiscard]] auto GetRef() -> std::weak_ptr<RenderingDevice> override { return shared_from_this(); }

    auto SetBackgroundColor(const SDL_FColor& color) -> void override;
    auto SetFont(TTF_Font* font) -> void; // TODO: Should this be taken out of the class, like with SDLHW2D?
    auto SetVSync(VSync value) -> void override;
    [[nodiscard]] auto GetVSync() const -> VSync override;

    template <typename... Args>
    [[nodiscard]] static auto Create(Args&&... args) -> std::shared_ptr<SDLGPU> { return std::shared_ptr<SDLGPU>(std::forward<Args>(args)...); }

    private:
    SDL_GPUDevice* _device = nullptr;
  SDL_Window* _window = nullptr;
    TTF_TextEngine* _textEngine = nullptr; // TODO: Should this be taken out of the class, like with SDLHW2D?
    GPU_Params _params{};
  };

}

#endif
