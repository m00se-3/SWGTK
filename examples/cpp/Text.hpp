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
#ifndef SWGTK_TESTS_TEST_CPP_TEXT_HPP_
#define SWGTK_TESTS_TEST_CPP_TEXT_HPP_

#include <swgtk/Simple2DRenderer.hpp>
#include <swgtk/Scene.hpp>

namespace swgtk {

    struct MouseCursor {
        SDL_FPoint pos{};
        Texture texture;
        double angle = 0.0;
    };

    class TextTest final : public Scene::Node {
    public:
        explicit TextTest(const gsl::not_null<Scene*>& scene) : Node(scene) {}

        bool Create() override;
        bool Update(float dt) override;

    private:
        MouseCursor _mouse{};
        Texture _background;
        App* _app = nullptr;
        Simple2DRenderer* _render = nullptr;
    };

} // namespace swgtk

#endif // SWGTK_TESTS_TEST_CPP_TEXT_HPP_