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
#include <swgtk/SDLHW2D.hpp>
#include <swgtk/Scene.hpp>
#include <vector>

#include "swgtk/Math.hpp"

inline constexpr auto particleCount = 100uz;

namespace swgtk {

  struct MouseCursor {
    SDL_FPoint pos{};
    Texture texture;
  };

  struct Particle {
    SDL_FPoint pos{};
    SDL_FPoint velocity{};
    Rads angle{};
    float speed = 0.f;
    float lifetime = 0.f;
  };

  class TimeToFramesScene;

  class ParticlesTest final : public Scene::Node {
  public:
    explicit ParticlesTest(const ObjectRef<Scene> &scene) :
        Node(scene), _particles(std::vector<Particle>(particleCount)),
        _gen(_rd()), _app(scene->GetApp()),
        _render(scene->AppRenderer<SDLHW2D>()) {}

    auto Create() -> bool override;
    auto Update(float deltaTime) -> bool override;

    [[nodiscard]] auto Draw() const { return _render; }
    [[nodiscard]] auto GetAverageTime() const { return _averageTime; }

  private:
    MouseCursor _mouse;
    std::vector<Particle> _particles;
    std::random_device _rd;
    std::mt19937_64 _gen;
    ObjectRef<App> _app;
    ObjectRef<SDLHW2D> _render;
    uint32_t _currentFrameCount = 0u;
    float _runningTime = 0.0f;
    float _averageTime = 0.0f;
    bool generate = true;
    bool showTime = false;
    std::shared_ptr<TimeToFramesScene> _child;
  };

  class TimeToFramesScene : public Scene::Node {
  public:
    explicit TimeToFramesScene(const std::shared_ptr<Node> &parent) :
        Node(parent) {}

    auto Create() -> bool override { return true; };
    auto Update(float deltaTime) -> bool override;
  };

} // namespace swgtk

#endif // SWGTK_TESTS_TEST_CPP_PARTICLES_HPP_
