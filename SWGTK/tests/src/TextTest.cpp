#include "swgtk/Utility.hpp"
#include <TextTest.hpp>
#include <Project.hpp>
#include <swgtk/Simple2DRenderer.hpp>

namespace swgtk {
	bool TextTest::Create(Scene& scene) {
		constexpr auto colorDefault = 255u;

		_app = scene.AppRoot();
		_render = scene.AppRenderer<Simple2DRenderer>();

		const std::filesystem::path fontsDir = std::filesystem::path{project::AssetsDir()} / "fonts" / "roboto";

		_app->AddFont(fontsDir / "Roboto-Medium.ttf", FontStyle::Normal);
		_app->AddFont(fontsDir / "Roboto-Bold.ttf", FontStyle::Bold);

		_mouse.texture = _render->LoadPlainWrapText("Hello\nWorld!", _app->GetFont(FontStyle::Normal), 0, SDL_Color{colorDefault, 0u, 0u, colorDefault});
		
		_background = _render->LoadLCDWrapText("EAT!\nSLEEP!\nCODE!", _app->GetFont(FontStyle::Bold));

		return true;
	}

	bool TextTest::Update(const float dt) {
		// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers) - Reason: It's pointless to create constants for this test.

		_mouse.pos = _app->GetMousePos();
		_mouse.angle += dt * 2.0;

		if(_mouse.angle > math::pi2) { _mouse.angle -= (math::pi2); }

		auto [width, height] = _mouse.texture.GetSize();

		auto rect = SDL_FRect{};

		rect.w = width * 2.0f;
		rect.h = height * 2.0f;

		rect.x = _mouse.pos.x - (rect.w / 2.0f);
		rect.y = _mouse.pos.y - (rect.h / 2.0f);

		_render->BufferClear();

		_render->DrawTexture(*_background);

		// Rotating in SDL3 is in degrees...
		_render->DrawTexture(*_mouse.texture, std::nullopt, rect, math::radiansToDegrees(_mouse.angle));


		// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

		return true;
	}

}


int main([[maybe_unused]]int argc, [[maybe_unused]]const char** argv) {
	constexpr auto w = 800;
	constexpr auto h = 600;

	if(swgtk::App app; app.InitGraphics("Text Test", w, h, swgtk::Simple2DRenderer::Create())) {
		swgtk::TextTest test;

		app.Run(
			[&test](swgtk::Scene& sc){ return test.Create(sc); },
			[&test](swgtk::Scene&, const float dt) { return test.Update(dt); }
		);
	}
}