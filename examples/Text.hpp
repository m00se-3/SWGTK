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

#include <swgtk/Basic2D.hpp>
#include <swgtk/Scene.hpp>
#include <swgtk/Font.hpp>
#include <swgtk/Math.hpp>

namespace swgtk {

  struct mouse_cursor {
    SDL_FPoint pos{};
    texture_2d texture;
    rads angle;
  };

  class text_test final : public scene::node {
  public:
    explicit text_test(const object_ref<scene> &scene) :
        node(scene), _font{font::load_default()} {}

    auto create() -> bool  override;
    auto update(float dt) -> bool  override;

  private:
    mouse_cursor _mouse{};
    texture_2d _background;
    font _font;
    object_ref<app> _app;
    object_ref<basic_2d> _render;
  };

} // namespace swgtk

#endif // SWGTK_TESTS_TEST_CPP_TEXT_HPP_
