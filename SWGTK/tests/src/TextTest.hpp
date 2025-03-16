#ifndef SWGTK_TEXTTEST_HPP
#define SWGTK_TEXTTEST_HPP

#include "swgtk/Simple2DRenderer.hpp"
#include <swgtk/App.hpp>
#include <swgtk/Scene.hpp>

namespace swgtk {

    struct MouseCursor {
        SDL_FPoint pos{};
        Texture texture;
        double angle = 0.0;
    };

    class TextTest {
    public:
        SSC Create(Scene& scene);
        SSC Update(float dt);

    private:
        MouseCursor _mouse{};
        Texture _background;
        App* _app = nullptr;
        Simple2DRenderer* _render = nullptr;
    };

}

#endif