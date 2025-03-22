#include "swgtk/Utility.hpp"
#include <TextTest.hpp>
#include <Project.hpp>
#include <swgtk/Simple2DRenderer.hpp>

namespace swgtk {
	SSC TextTest::Create(Scene& scene) {
		constexpr auto colorDefault = 255u;

		_app = scene.AppRoot();
		_render = scene.AppRenderer<Simple2DRenderer>();


		const std::filesystem::path fontsDir = std::filesystem::path{project::AssetsDir()} / "fonts" / "roboto";

		_app->AddFont(fontsDir / "Roboto-Medium.ttf", FontStyle::Normal);
		_app->AddFont(fontsDir / "Roboto-Bold.ttf", FontStyle::Bold);

		auto* render = scene.AppRenderer<Simple2DRenderer>();
		_mouse.texture = render->LoadPlainWrapText("Hello\nWorld!", _app->GetFont(FontStyle::Normal), 0, SDL_Color{colorDefault, 0u, 0u, colorDefault});
		
		_background = render->LoadLCDWrapText("EAT!\nSLEEP!\nCODE!", _app->GetFont(FontStyle::Bold));

		return SSC::Ok;
	}

	SSC TextTest::Update(float dt) {
		// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers) - Reason: It's pointless to create constants for this test.

		_mouse.pos = _app->GetMousePos();
		_mouse.angle += dt * 2.0;

		if(_mouse.angle > math::pi2) { _mouse.angle -= (math::pi2); }

		auto s = _mouse.texture.GetSize();

		_rect.w = s.first * 2.0f;
		_rect.h = s.second * 2.0f;

		_rect.x = _mouse.pos.x - (_rect.w / 2.0f);
		_rect.y = _mouse.pos.y - (_rect.h / 2.0f);

		return SSC::Ok;
	}

	SSC TextTest::Render([[maybe_unused]]float dt) {
		
		_render->BufferClear();

		_render->DrawTexture(*_background);

		// Rotating in SDL3 is in degrees...
		_render->DrawTexture(*_mouse.texture, std::nullopt, _rect, math::radiansToDegrees(_mouse.angle));


		// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

		return SSC::Ok;
	}
}


int main([[maybe_unused]]int argc, [[maybe_unused]]const char** argv) {
	constexpr auto w = 800;
	constexpr auto h = 600;

	swgtk::App app;
	swgtk::TextTest test;

	if(app.InitGraphics("Text Test", w, h, swgtk::Simple2DRenderer::Create())) {
		app.Run(swgtk::Scene::CreateSceneNode(
			[&test](swgtk::Scene& sc){ return test.Create(sc); },
			[&test](swgtk::Scene&, float dt) { return test.Update(dt); },
			[&test](swgtk::Scene&, float dt) { return test.Render(dt); }
		));
	}
}