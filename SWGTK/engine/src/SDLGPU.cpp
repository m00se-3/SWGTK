#include <swgtk/SDLGPU.hpp>

namespace swgtk {
  auto SDLGPU::PrepareDevice(const std::any& dependency) -> bool {
    _window = detail::NonOwning(std::any_cast<SDL_Window*>(dependency));
    _device = SDL_CreateGPUDevice(std::to_underlying(_params.format), _params.debugMode, !_params.apiName.empty() ? _params.apiName.c_str() : nullptr);

    return *_window != nullptr && _device != nullptr
      && SDL_ClaimWindowForGPUDevice(_device, *_window);
  }

  auto SDLGPU::DestroyDevice() -> void {
    SDL_ReleaseWindowFromGPUDevice(_device, *_window);
    SDL_DestroyGPUDevice(_device);
  }
} // namespace swgtk
