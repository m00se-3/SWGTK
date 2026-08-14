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
#ifndef SWGTK_TESTS_TEST_CPP_PARTICLES_HPP_
#define SWGTK_TESTS_TEST_CPP_PARTICLES_HPP_

#include <random>
#include <swgtk/Basic2D.hpp>
#include <swgtk/Scene.hpp>
#include <vector>

#include "swgtk/Font.hpp"
#include "swgtk/Math.hpp"

inline constexpr auto particle_count = 100uz;

namespace swgtk {

  struct mouse_cursor {
    SDL_FPoint pos{};
    texture_2d texture;
  };

  struct particle {
    SDL_FPoint pos{};
    SDL_FPoint velocity{};
    rads angle;
    float speed = 0.f;
    float lifetime = 0.f;
  };

  class particles_test final : public scene::node {
  public:
    explicit particles_test(const object_ref<scene> &scene) :
        node(scene), _particles(std::vector<particle>(particle_count)),
        _gen(_rd()), _app(scene->get_app()),
        _render(scene->app_renderer<basic_2d>()) {}

    auto create() -> bool  override;
    auto update(float deltaTime) -> bool  override;

    [[nodiscard]] auto draw() const { return _render; }
    [[nodiscard]] auto get_average_time() const { return _averageTime; }

  private:
    mouse_cursor _mouse;
    std::vector<particle> _particles;
    std::random_device _rd;
    std::mt19937_64 _gen;
    object_ref<app> _app;
    object_ref<basic_2d> _render{};
    font _font{font::load_default()};
    uint32_t _currentFrameCount = 0u;
    float _runningTime = 0.0f;
    float _averageTime = 0.0f;
    bool generate = true;
    bool showTime = false;
  };



} // namespace swgtk

#endif // SWGTK_TESTS_TEST_CPP_PARTICLES_HPP_
