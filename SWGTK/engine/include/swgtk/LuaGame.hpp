#ifndef SWGTK_LUAGAME_HPP
#define SWGTK_LUAGAME_HPP

#include <swgtk/Scene.hpp>
#include <gsl/gsl-lite.hpp>

namespace swgtk {
    /**
        @brief This is a specialized class for games whose *primary* coding language is Lua.

        Eventually, this class will not be exposed to library users. This class will be used to run games
        in a similar fashion to Love2D.
     */
    class LuaGame final : public Scene::Node {
    public:
        LuaGame(const std::filesystem::path& path, sol::state& lua);

        [[nodiscard]] bool Create([[maybe_unused]]Scene& scene) override;
        [[nodiscard]] bool Update([[maybe_unused]]Scene& scene, [[maybe_unused]]float dt) override { return true; }

    private:
        gsl::not_null<sol::state*> _lua;
    };

}

#endif //SWGTK_LUAGAME_HPP
