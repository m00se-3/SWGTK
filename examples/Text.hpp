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

#include <swgtk/SDLHW2D.hpp>
#include <swgtk/Scene.hpp>

namespace swgtk {

    struct MouseCursor {
        SDL_FPoint pos{};
        Texture texture;
        Rads angle{};
    };

    class TextTest final : public Scene::Node {
    public:
        explicit TextTest(const ObjectRef<Scene>& scene) : Node(scene) {}

        bool Create() override;
        bool Update(float dt) override;

    private:
        MouseCursor _mouse{};
        Texture _background;
        ObjectRef<App> _app;
        ObjectRef<SDLHW2D> _render;
    };

} // namespace swgtk

#endif // SWGTK_TESTS_TEST_CPP_TEXT_HPP_