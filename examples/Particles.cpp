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

static constexpr auto particle_size = 5;
static constexpr auto particle_red = 0.7f;
static constexpr auto speed_constant = 100.f;
static constexpr auto canonical_bit_size = 16ull;
static constexpr auto max_lifetime = 2.0f;
static constexpr auto number_of_frames_to_average = 60u;

namespace swgtk {

  auto particles_test::create() -> bool {

    _mouse.texture = _render->create_renderable_texture(particle_size, particle_size);

    if (!_render->set_draw_target(_mouse.texture)) {
      return false;
    }

    _render->buffer_clear(color_t{ red_v{particle_red}, green_v{}, blue_v{} });

    return _render->set_draw_target(texture_2d{});
  }

  auto particles_test::update(const float deltaTime) -> bool {

    _mouse.pos = _app->get_mouse_pos();

    if (_app->is_key_released(layout_code::space)) {
      showTime = !showTime;
    }

    if (generate) {
      std::ranges::generate(_particles, [&gen = particles_test::_gen, mouse = particles_test::_mouse.pos] -> particle {
        const auto speed = std::generate_canonical<float, canonical_bit_size>(gen) * speed_constant;
        const auto angle = std::generate_canonical<double, canonical_bit_size>(gen) * rads::pi2;
        const auto anglef = static_cast<float>(angle);
        const auto life = std::generate_canonical<float, canonical_bit_size>(gen) * max_lifetime;

        return particle{
            .pos = SDL_FPoint{.x = mouse.x, .y = mouse.y},
            .velocity = SDL_FPoint{.x = cosf(anglef), .y = sinf(anglef)},
            .angle = radians{angle},
            .speed = speed,
            .lifetime = life,
        };
      });

      generate = false;
    } else {
      _currentFrameCount++;
      _runningTime += deltaTime;

      if (_currentFrameCount == number_of_frames_to_average) {
        _currentFrameCount = 0u;

        _averageTime = _runningTime / number_of_frames_to_average;
        _runningTime = 0.0f;
      }

      std::ranges::for_each(_particles, [&](particle &particle) -> void {
        const auto speed_frame = particle.speed * deltaTime;

        particle.pos.x += particle.velocity.x * speed_frame;
        particle.pos.y += particle.velocity.y * speed_frame;
        particle.angle += static_cast<double>(speed_frame);
        particle.lifetime += deltaTime;

        if (particle.lifetime > max_lifetime) {
          const auto angle = std::generate_canonical<double, canonical_bit_size>(_gen) * rads::pi2;
          const auto angle_f = static_cast<float>(angle);

          particle.lifetime = 0.0f;
          particle.pos = SDL_FPoint{.x = _mouse.pos.x, .y = _mouse.pos.y};
          particle.speed = std::generate_canonical<float, canonical_bit_size>(_gen) * speed_constant;
          particle.angle = radians{angle};
          particle.velocity.x = cosf(angle_f);
          particle.velocity.y = sinf(angle_f);
        }
      });

      _render->buffer_clear();

      for (const auto &particle: _particles) {
        const auto rect = SDL_FRect{
            .x = particle.pos.x - (particle_size / 2.0f),
            .y = particle.pos.y - (particle_size / 2.0f),
            .w = particle_size,
            .h = particle_size,
        };

        _render->draw_texture(_mouse.texture, std::nullopt, rect, radians_to_degrees(particle.angle).value());
      }

      if (showTime) {
        _render->draw_plain_text(*_font,
                               std::format("Time between frames: {}", get_average_time()), SDL_FRect{.x = 5.f, .y = 10.f, .w = 400.f, .h = 40.f}); // NOLINT
      }
    }
    return true;
  }

} // namespace swgtk

auto main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) -> int {
  constexpr auto window_width = 800;
  constexpr auto window_height = 600;

  if (auto app = swgtk::app{ "Particle Test.", window_width, window_height, swgtk::basic_2d::create() }; app.build()) {
    app.run_game<swgtk::particles_test>();
  }
}
