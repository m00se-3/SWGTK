#include <swgtk/GPU2D.hpp>
#include <SDL3/SDL_assert.h>
#include <SDL3/SDL_gpu.h>

namespace {
  constexpr auto apis = std::array{static_cast<const char *>(nullptr), "vulkan", "direct3d12", "metal"};
};

namespace swgtk {

  auto gpu::prepare_device(SDL_Window* window) -> bool {
    SDL_assert_release(window != nullptr); // NOLINT(*-avoid-do-while, *-else-after-return)
    _device = SDL_CreateGPUDevice(std::to_underlying(_params.format), _params.debug_mode, apis.at(std::to_underlying(_params.api_name)));
    _window = window;
    SDL_ClaimWindowForGPUDevice(_device, _window);
    
    //TODO: Shader initialization code.

    return true;
  }

  auto gpu::destroy_device() -> void {
    SDL_ReleaseWindowFromGPUDevice(_device, _window);
    SDL_DestroyGPUDevice(_device);
  }
} // namespace swgtk
