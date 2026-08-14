#ifndef SWGTK_ENGINE_INCLUDE_SWGTK_GPU_HPP_
#define SWGTK_ENGINE_INCLUDE_SWGTK_GPU_HPP_

#include <swgtk/RenderingDevice.hpp>
#include <swgtk/Texture.hpp>
#include <utility>
#include <SDL3_ttf/SDL_textengine.h>

namespace swgtk {

  enum class shader_format : std::uint32_t {
    invalid = SDL_GPU_SHADERFORMAT_INVALID,
    priv = SDL_GPU_SHADERFORMAT_PRIVATE,
    spirv = SDL_GPU_SHADERFORMAT_SPIRV,
    dxbc = SDL_GPU_SHADERFORMAT_DXBC,
    dxil = SDL_GPU_SHADERFORMAT_DXIL,
    msl = SDL_GPU_SHADERFORMAT_MSL,
    metallib = SDL_GPU_SHADERFORMAT_METALLIB,
  };

  enum class gpu_api : std::uint32_t {
    none = 0u,
    vulkan = 1u,
    d3d12 = 2u,
    metal = 3u,
  };

  struct gpu_params {
    shader_format format{shader_format::spirv};
    bool debug_mode{false};
    gpu_api api_name;
  };

  class gpu : public rendering_device, public std::enable_shared_from_this<gpu> {
    public:
    constexpr gpu() = default;
    explicit constexpr gpu(const gpu_params params) : _params(params) {}

    constexpr gpu(const gpu&) = delete;
    constexpr auto operator=(const gpu&) -> gpu& = delete;
    constexpr gpu(gpu&&) = delete;
    constexpr auto operator=(gpu&&) -> gpu& = delete;
    ~gpu() override { gpu::destroy_device(); }

    auto buffer_clear(const color_t& color = color_t{}) -> void override;
    auto buffer_present() -> void override;

    auto prepare_device(SDL_Window* window) -> bool override;
    auto destroy_device() -> void override;
    auto is_device_initialized() const -> bool override;
    [[nodiscard]] auto get_ref() -> std::weak_ptr<rendering_device> override { return shared_from_this(); }

    auto set_background_color(const color_t& color) -> void override;
    auto set_font(TTF_Font* font) -> void; // TODO: Should this be taken out of the class, like with Basic2D?
    auto set_vsync(vsync value) -> void override;
    [[nodiscard]] auto get_vsync() const -> vsync override;

    template <typename... Args>
    [[nodiscard]] static auto create(Args&&... args) -> std::shared_ptr<gpu> { return std::shared_ptr<gpu>(std::forward<Args>(args)...); }

    private:
    SDL_GPUDevice* _device = nullptr;
    SDL_Window* _window = nullptr;
    TTF_TextEngine* _textEngine = nullptr; // TODO: Should this be taken out of the class, like with Basic2D?
    gpu_params _params{};
  };

}

#endif
