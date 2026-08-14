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
#include <Text.hpp>
#include <swgtk/App.hpp>
#include <swgtk/Math.hpp>
#include <swgtk/Utility.hpp>

namespace swgtk {
  auto text_test::create() -> bool {
    _app = _scene->get_app();
    _render = _scene->app_renderer<basic_2d>();
    _font.set_style(font_style::underlined);

    _mouse.texture = _render->load_plain_wrap_text(*_font, "Hello\nWorld!", 0, color_t::red());

    _background = _render->load_lcd_wrap_text(*_font, "EAT!\nSLEEP!\nCODE!");

    return true;
  }

  auto text_test::update(const float dt) -> bool {
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers) - Reason: It's pointless to create constants for this test.

    _mouse.pos = _app->get_mouse_pos();
    _mouse.angle += static_cast<double>(dt) * 2.0;

    auto [width, height] = _mouse.texture.get_size();

    auto rect = SDL_FRect{};

    rect.w = width * 2.0f;
    rect.h = height * 2.0f;

    rect.x = _mouse.pos.x - (rect.w / 2.0f);
    rect.y = _mouse.pos.y - (rect.h / 2.0f);

    _render->buffer_clear();

    _render->draw_texture(_background);

    // Rotating in SDL3 is in degrees...
    _render->draw_texture(_mouse.texture, std::nullopt, rect, radians_to_degrees(_mouse.angle).value());


    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

    return true;
  }

} // namespace swgtk


auto main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) -> int {
  constexpr auto w = 800;
  constexpr auto h = 600;

  if (swgtk::app app("Text Test", w, h, swgtk::basic_2d::create()); app.build()) {
    app.run_game<swgtk::text_test>();
  }
}
