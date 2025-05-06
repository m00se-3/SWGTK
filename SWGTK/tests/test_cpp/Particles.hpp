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

#include <swgtk/Simple2DRenderer.hpp>
#include <swgtk/Scene.hpp>
#include <vector>
#include <random>

inline constexpr auto particleCount = 100uz;

namespace swgtk {

    struct MouseCursor {
        SDL_FPoint pos{};
        Texture texture;
    };

    struct Particle {
        SDL_FPoint pos{};
        SDL_FPoint velocity{};
        double angle = 0.0;
        float speed = 0.f;
        float lifetime = 0.f;
    };

    class ParticlesTest final : public Scene::Node {
    public:
        explicit ParticlesTest(const gsl::not_null<Scene*>& scene)
        : Node(scene), _particles(std::vector<Particle>(particleCount)),
        _gen(_rd()), _app(gsl::make_not_null<App*>(scene->AppRoot())),
        _render(gsl::make_not_null<Simple2DRenderer*>(scene->AppRenderer<Simple2DRenderer>())) {}

        bool Create() override;
        bool Update(float deltaTime) override;

    private:
        MouseCursor _mouse;
        std::vector<Particle> _particles;
        std::random_device _rd;
        std::mt19937_64 _gen;
        gsl::not_null<App*> _app;
        gsl::not_null<Simple2DRenderer*> _render;
        bool generate = true;
    };

} // namespace swgtk

#endif // SWGTK_TESTS_TEST_CPP_PARTICLES_HPP_
