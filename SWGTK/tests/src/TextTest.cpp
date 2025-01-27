#include <TextTest.hpp>
#include <Project.hpp>
#include <swgtk/Simple2DRenderer.hpp>
#include <numbers>

namespace swgtk {
    SSC TextTest::Create(Scene& scene) {
        constexpr auto colorDefault = 255u;

        auto* app = scene.AppRoot();
        const std::filesystem::path fontsDir = std::filesystem::path{project::AssetsDir()} / "fonts" / "roboto";

        app->AddFont(fontsDir / "Roboto-Medium.ttf", FontStyle::Normal);
        app->AddFont(fontsDir / "Roboto-Bold.ttf", FontStyle::Bold);

        auto* render = scene.AppRenderer<Simple2DRenderer>();
        _mouse.texture = render->LoadPlainWrapText("Hello\nWorld!", app->GetFont(FontStyle::Normal), 0, SDL_Color{colorDefault, 0u, 0u, colorDefault});
        
        _background = render->LoadLCDWrapText("EAT!\nSLEEP!\nCODE!", app->GetFont(FontStyle::Bold));

        return SSC::Ok;
    }

    SSC TextTest::Update(Scene& scene, float dt) {
        // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers) - Reason: It's pointless to create constants for this test.

        auto* app = scene.AppRoot(); 
        auto* render = scene.AppRenderer<Simple2DRenderer>();

        _mouse.pos = app->GetMousePos();
        _mouse.angle += dt * 2.0;

        if(_mouse.angle > std::numbers::pi * 2.0) { _mouse.angle -= (std::numbers::pi * 2.0); }

        auto rect = SDL_FRect{};

        auto s = _mouse.texture.GetSize();

        rect.w = s.first * 2.0f;
        rect.h = s.second * 2.0f;

        rect.x = _mouse.pos.x - (rect.w / 2.0f);
        rect.y = _mouse.pos.y - (rect.h / 2.0f);

        render->BufferClear();

        render->DrawTexture(*_background, nullptr, nullptr);

        // Rotating in SDL3 is in degrees...
        // This interface needs to be improved, it needs to not take things by pointer if possible, especially since the pointers 
        // are definitly going to out live the draw call no matter what.
        render->DrawTexture(*_mouse.texture, nullptr, &rect, (_mouse.angle / std::numbers::pi) * 180.0);


        // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)

        return SSC::Ok;
    }
}


int main([[maybe_unused]]int argc, [[maybe_unused]]const char** argv) {
    swgtk::App app;
    swgtk::TextTest test;

    if(app.InitGraphics("Text Test", swgtk::Simple2DRenderer::Create())) {
        app.Run(swgtk::Scene::CreateSceneNode(
            [&test](swgtk::Scene& sc){ return test.Create(sc); },
            [&test](swgtk::Scene& sc, float dt) { return test.Update(sc, dt); }
        ));
    }
}