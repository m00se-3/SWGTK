#ifndef SWGTK_TESTS_TESTRENDERER_HPP_
#define SWGTK_TESTS_TESTRENDERER_HPP_

#include <functional>
#include <swgtk/RenderingDevice.hpp>

namespace swgtk::test {
  // Mock class for running tests which do not require a working renderer.
  class TestRenderer : public RenderingDevice {
  public:
    [[nodiscard]] auto PrepareDevice([[maybe_unused]] const std::any &dependency) -> bool override { return true; }
    [[nodiscard]] auto IsDeviceInitialized() const -> bool override { return true; }
    constexpr void BufferClear([[maybe_unused]] const SDL_FColor &color) override {}
    constexpr void BufferPresent() override {}
    constexpr void SetBackgroundColor([[maybe_unused]] const SDL_FColor &color) override {}
    constexpr void SetFont([[maybe_unused]] TTF_Font *font) override {}
    constexpr void SetVSync([[maybe_unused]] VSync value) override {}
    constexpr void DestroyDevice() override {}

    constexpr void AssignUpdateFunction(const std::function<void()> &_func) { _testFunction = _func; }

  private:
    std::function<void()> _testFunction;
  };
} // namespace swgtk::test

#endif
