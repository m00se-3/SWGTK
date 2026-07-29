#include <swgtk/GPU2D.hpp>
#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_gpu.h>

namespace {
  constexpr auto apis = std::array{(const char*) nullptr, "vulkan", "direct3d12", "metal"};
};

namespace swgtk {

  auto SDLGPU::PrepareDevice(SDL_Window* window) -> bool {
    SDL_assert_release(window != nullptr); // NOLINT(*-avoid-do-while, *-else-after-return)
    _device = SDL_CreateGPUDevice(std::to_underlying(_params.format), _params.debugMode, apis.at(std::to_underlying(_params.apiName)));
    _window = window;
    SDL_ClaimWindowForGPUDevice(_device, _window);
    
    //TODO: Shader initialization code.

    return true;
  }

  auto SDLGPU::DestroyDevice() -> void {
    SDL_ReleaseWindowFromGPUDevice(_device, _window);
    SDL_DestroyGPUDevice(_device);
  }
} // namespace swgtk
