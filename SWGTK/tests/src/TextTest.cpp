#include <TextTest.hpp>
#include <Project.hpp>
#include <swgtk/Simple2DRenderer.hpp>
#include <SDL3/SDL_main.h>

namespace swgtk {
    SSC TextTest::Create(Scene& scene) {
        auto* app = scene.AppRoot();
        auto fontsDir = project::AssetsDir() + "/fonts/roboto";

        app->AddTTF(fontsDir + "/Roboto-Medium.ttf", sdl::FontStyle::Normal);

        std::puts("Creating test...\n");

        auto* render = scene.AppRenderer<Simple2DRenderer>();
        _mouse.texture = render->LoadPlainWrapText("Hello\nWorld!", app->GetTTF(sdl::FontStyle::Normal), 0, SDL_Color{255u, 0u, 0u, 255u});
        
        return SSC::Ok;
    }

    SSC TextTest::Update(Scene& scene, float dt) {
        constexpr auto speed = 2.0;
        auto* input = scene.AppInput(); // Why did I do this again?
        auto* render = scene.AppRenderer<Simple2DRenderer>();

        _mouse.pos = input->GetMousePos();
        _mouse.angle += dt * speed;

        if(_mouse.angle > std::numbers::pi * 2.0) { _mouse.angle -= (std::numbers::pi * 2.0); }

        auto rect = SDL_FRect{};

        // TODO: create a funciton to tidy this up.
        SDL_GetTextureSize(_mouse.texture.Get().get(), &rect.w, &rect.h);
        rect.w *= 2.0f;
        rect.h *= 2.0f;

        rect.x = _mouse.pos.x - (rect.w / 2.0f);
        rect.y = _mouse.pos.y - (rect.h / 2.0f);

        // Rotating in SDL3 is in degrees...
        // This interface needs to be improved, it needs to not take things by pointer if possible, especially since the pointers 
        // are definitly going to out live the draw call no matter what.
        render->DrawTexture(_mouse.texture.Get().get(), nullptr, &rect, (_mouse.angle / std::numbers::pi) * 180.0);

        return SSC::Ok;
    }
}


int main([[maybe_unused]]int argc, [[maybe_unused]]const char** argv) {
    swgtk::App app;
    swgtk::TextTest test;

    if(app.InitGraphics(swgtk::Simple2DRenderer::Create())) {
        app.Run(swgtk::Scene::CreateSceneNode(
            [&test](swgtk::Scene& sc){ return test.Create(sc); },
            [&test](swgtk::Scene& sc, float dt) { return test.Update(sc, dt); }
        ));
    }
}