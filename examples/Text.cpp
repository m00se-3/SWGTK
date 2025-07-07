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
#include <SDL3/SDL_error.h>
#include <swgtk/Utility.hpp>
#include <swgtk/Math.hpp>
#include <Text.hpp>
#include <swgtk/App.hpp>

namespace swgtk {
	bool TextTest::Create() {
		constexpr auto colorDefault = 255u;

		_app = _scene->GetApp();
		_render = _scene->AppRenderer<Simple2DRenderer>();

		_mouse.texture = _render->LoadPlainWrapText("Hello\nWorld!", 0, SDL_Color{colorDefault, 0u, 0u, colorDefault});

		FontGroup::SetFontStyle(_app->GetDefaultFont(), FontStyle::Underlined);
		
		_background = _render->LoadLCDWrapText("EAT!\nSLEEP!\nCODE!");

		return true;
	}

	bool TextTest::Update(const float dt) {
		// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers) - Reason: It's pointless to create constants for this test.

		_mouse.pos = _app->GetMousePos();
		_mouse.angle += static_cast<double>(dt) * 2.0;

		if(_mouse.angle > math::pi2) { _mouse.angle -= (math::pi2); }

		auto [width, height] = _mouse.texture.GetSize();

		auto rect = SDL_FRect{};

		rect.w = width * 2.0f;
		rect.h = height * 2.0f;

		rect.x = _mouse.pos.x - (rect.w / 2.0f);
		rect.y = _mouse.pos.y - (rect.h / 2.0f);

		_render->BufferClear();

		_render->DrawTexture(_background);

		// Rotating in SDL3 is in degrees...
		_render->DrawTexture(_mouse.texture, std::nullopt, rect, math::RadiansToDegrees(_mouse.angle));


		// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

		return true;
	}

} // namespace swgtk


int main([[maybe_unused]]int argc, [[maybe_unused]]const char** argv) {
	constexpr auto w = 800;
	constexpr auto h = 600;

	/*
		This try block is to temporarily satisfy a clang-tidy warning that says 'main' could possibly throw
		an exception.

		I'm pretty sure this is caused by std::filesystem, so I will need to redesign my file handling
		soon.
	*/
	try{
		if(swgtk::App app; app.InitGraphics("Text Test", w, h, swgtk::Simple2DRenderer::Create())) {
		app.RunGame<swgtk::TextTest>();
		}
	} catch (std::exception& except) {
		DEBUG_PRINT("Esception thrown: {}/n", except.what());
	}
}