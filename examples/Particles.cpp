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
#include <Particles.hpp>
#include <algorithm>
#include <cmath>
#include <format>
#include <swgtk/App.hpp>
#include <swgtk/Math.hpp>

static constexpr auto particleSize = 5;
static constexpr auto particleRed = 0.7f;
static constexpr auto speedConstant = 100.f;
static constexpr auto canonicalBitSize = 16ull;
static constexpr auto maxLifetime = 2.0f;
static constexpr auto numberOfFramesToAverage = 60u;

namespace swgtk {

  auto ParticlesTest::Create() -> bool {

    _child = std::make_shared<TimeToFramesScene>(_scene->GetRootNode<ParticlesTest>());

    _mouse.texture = _render->CreateRenderableTexture(particleSize, particleSize);

    if (!_render->SetDrawTarget(_mouse.texture)) {
      return false;
    }

    _render->BufferClear(SDL_FColor{.r = particleRed, .g = 0.0f, .b = 0.0f, .a = 1.0f});

    return _render->SetDrawTarget(Texture{});
  }

  auto ParticlesTest::Update(const float deltaTime) -> bool {

    _mouse.pos = _app->GetMousePos();

    if (_app->IsKeyReleased(LayoutCode::Space)) {
      showTime = !showTime;
    }

    if (generate) {
      std::ranges::generate(_particles, [&gen = ParticlesTest::_gen, mouse = ParticlesTest::_mouse.pos]() {
        const auto speed = std::generate_canonical<float, canonicalBitSize>(gen) * speedConstant;
        const auto angle = std::generate_canonical<double, canonicalBitSize>(gen) * Rads::pi2;
        const auto angleF = static_cast<float>(angle);
        const auto life = std::generate_canonical<float, canonicalBitSize>(gen) * maxLifetime;

        return Particle{
            .pos = SDL_FPoint{.x = mouse.x, .y = mouse.y},
            .velocity = SDL_FPoint{.x = cosf(angleF), .y = sinf(angleF)},
            .angle = Radians{angle},
            .speed = speed,
            .lifetime = life};
      });

      generate = false;
    } else {
      _currentFrameCount++;
      _runningTime += deltaTime;

      if (_currentFrameCount == numberOfFramesToAverage) {
        _currentFrameCount = 0u;

        _averageTime = _runningTime / numberOfFramesToAverage;
        _runningTime = 0.0f;
      }

      std::ranges::for_each(_particles, [&](Particle &particle) {
        const auto speedFrame = particle.speed * deltaTime;

        particle.pos.x += particle.velocity.x * speedFrame;
        particle.pos.y += particle.velocity.y * speedFrame;
        particle.angle += static_cast<double>(speedFrame);
        particle.lifetime += deltaTime;

        if (particle.lifetime > maxLifetime) {
          const auto angle = std::generate_canonical<double, canonicalBitSize>(_gen) * Rads::pi2;
          const auto angleF = static_cast<float>(angle);

          particle.lifetime = 0.0f;
          particle.pos = SDL_FPoint{.x = _mouse.pos.x, .y = _mouse.pos.y};
          particle.speed = std::generate_canonical<float, canonicalBitSize>(_gen) * speedConstant;
          particle.angle = Radians{angle};
          particle.velocity.x = cosf(angleF);
          particle.velocity.y = sinf(angleF);
        }
      });

      _render->BufferClear();

      for (const auto &particle: _particles) {
        const auto rect = SDL_FRect{
            .x = particle.pos.x - (particleSize / 2.0f),
            .y = particle.pos.y - (particleSize / 2.0f),
            .w = particleSize,
            .h = particleSize,
        };

        _render->DrawTexture(_mouse.texture, std::nullopt, rect, RadiansToDegrees(particle.angle).value());
      }

      if (showTime) {
        _child->Update(deltaTime);
      }
    }
    return true;
  }

  auto TimeToFramesScene::Update([[maybe_unused]] const float deltaTime) -> bool {
    const auto p = GetParent<ParticlesTest>().lock();
    p->Draw()->DrawPlainText(std::format("Time between frames: {}", p->GetAverageTime()),
                             SDL_FRect{.x = 5.f, .y = 10.f, .w = 400.f, .h = 40.f}); // NOLINT

    return true;
  }

} // namespace swgtk

auto main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) -> int {
  constexpr auto windowWidth = 800;
  constexpr auto windowHeight = 600;

  if (auto app = swgtk::App{}; app.InitGraphics("Particle Test.", windowWidth, windowHeight, swgtk::SDLHW2D::Create())) {
    app.RunGame<swgtk::ParticlesTest>();
  }
}
