#ifndef SWGTK_TESTS_TESTRENDERER_HPP_
#define SWGTK_TESTS_TESTRENDERER_HPP_

#include <functional>
#include <swgtk/RenderingDevice.hpp>

namespace swgtk::test {
  // Mock class for running tests which do not require a working renderer.
  class test_renderer final : public rendering_device {
  public:
    [[nodiscard]] auto prepare_device([[maybe_unused]] SDL_Window* dependency) -> bool override { return true; }
    [[nodiscard]] auto is_device_initialized() const -> bool override { return true; }
    constexpr void buffer_clear([[maybe_unused]] const color_t &color) override {}
    constexpr void buffer_present() override {}
    constexpr void set_background_color([[maybe_unused]] const color_t &color) override {}
    constexpr void destroy_device() override {}

    constexpr void assign_update_function(const std::function<void()> &_func) { _testFunction = _func; }

    constexpr auto get_ref() -> std::weak_ptr<rendering_device> override { return std::make_shared<test_renderer>(); }

  private:
    std::function<void()> _testFunction;
  };
} // namespace swgtk::test

#endif
