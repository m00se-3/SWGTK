#ifndef SWGTK_TEXTTEST_HPP
#define SWGTK_TEXTTEST_HPP

#include <swgtk/Simple2DRenderer.hpp>
#include <swgtk/Scene.hpp>

namespace swgtk {

    struct MouseCursor {
        SDL_FPoint pos{};
        Texture texture;
        double angle = 0.0;
    };

    class TextTest final : public swgtk::Scene::Node {
    public:
        bool Create(Scene& scene) override;
        bool Update(Scene& s, float dt) override;

    private:
        MouseCursor _mouse{};
        Texture _background;
        App* _app = nullptr;
        Simple2DRenderer* _render = nullptr;
    };

}

#endif