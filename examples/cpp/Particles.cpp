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
#include <swgtk/App.hpp>
#include <swgtk/Math.hpp>
#include <algorithm>
#include <cmath>

static constexpr auto particleSize = 5;
static constexpr auto particleRed = 0.7f;
static constexpr auto speedConstant = 100.f;
static constexpr auto canonicalBitSize = 16ull;
static constexpr auto maxLifetime = 5.0f;

namespace swgtk {

    bool ParticlesTest::Create() {

        _mouse.texture = _render->CreateRenderableTexture(particleSize, particleSize);

        if(!_render->SetDrawTarget(_mouse.texture)) { return false; }

        _render->BufferClear(SDL_FColor{ .r=particleRed, .g=0.0f, .b=0.0f, .a=1.0f });

        return _render->SetDrawTarget(Texture{});
    }

    bool ParticlesTest::Update(const float deltaTime) {

        _mouse.pos = _app->GetMousePos();

        if(generate) {
            std::ranges::generate(_particles, [&gen=ParticlesTest::_gen, &mouse=ParticlesTest::_mouse]() {
                const auto speed = std::generate_canonical<float, canonicalBitSize>(gen) * speedConstant;
                const auto angle = std::generate_canonical<double, canonicalBitSize>(gen) * math::pi2;
                const auto angleF = static_cast<float>(angle);

                return Particle{
                    .pos = SDL_FPoint{ .x=mouse.pos.x, .y=mouse.pos.y },
                    .velocity = SDL_FPoint{ .x=cosf(angleF), .y=sinf(angleF) },
                    .angle = angle,
                    .speed = speed,
                };
            });

            generate = false;
        } else {
            std::ranges::for_each(_particles, [&](Particle& particle) {
                const auto speedFrame = particle.speed * deltaTime;

                particle.pos.x += particle.velocity.x * speedFrame;
                particle.pos.y += particle.velocity.y * speedFrame;
                particle.angle += static_cast<double>(speedFrame);
                particle.lifetime += speedFrame;

                if(particle.angle > math::pi2) { particle.angle -= math::pi2; }
                if(particle.lifetime > maxLifetime) {
                    const auto angle = std::generate_canonical<double, canonicalBitSize>(_gen) * math::pi2;
                    const auto angleF = static_cast<float>(angle);

                    particle.lifetime = 0.0f;
                    particle.pos = SDL_FPoint{ .x=_mouse.pos.x, .y=_mouse.pos.y };
                    particle.speed = std::generate_canonical<float, canonicalBitSize>(_gen) * speedConstant;
                    particle.angle = angle;
                    particle.velocity.x = cosf(angleF);
                    particle.velocity.y = sinf(angleF);
                }
            });

            _render->BufferClear();

            for(auto& particle : _particles) {
                const auto rect = SDL_FRect {
                    .x = particle.pos.x - (particleSize / 2.0f),
                    .y = particle.pos.y - (particleSize / 2.0f),
                    .w = particleSize, .h = particleSize,
                };

                _render->DrawTexture(_mouse.texture, std::nullopt, rect, math::RadiansToDegrees(particle.angle));
            }
        }
        return true;
    }

} // namespace swgtk

int main([[maybe_unused]] int argc, [[maybe_unused]] const char** argv) {
    constexpr auto windowWidth = 800;
    constexpr auto windowHeight = 600;

    /*
		This try block is to temporarily satisfy a clang-tidy warning that says 'main' could possibly throw
		an exception.

		I'm pretty sure this is caused by std::filesystem, so I will need to redesign my file handling
		soon.
	*/

    try {
        if(auto app = swgtk::App{}; app.InitGraphics("Particle Test.", windowWidth, windowHeight, swgtk::Simple2DRenderer::Create())) {
            app.RunGame<swgtk::ParticlesTest>();
        }
    } catch (std::exception& except) {
		DEBUG_PRINT("Esception thrown: {}/n", except.what());
	}
}