#ifndef SWGTK_PLATFORMER_SRC_PLATFORMER_H
#define SWGTK_PLATFORMER_SRC_PLATFORMER_H

/*
MIT License
    Copyright (c) 2026 Samuel Bridgham

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#include <swgtk/SDLHW2D.hpp>
#include <swgtk/Scene.hpp>
#include <filesystem>

namespace swgtk {

  namespace assets {

    // Sprites

    constexpr auto* coinSprite = "sprites/coind.png";
    constexpr auto* fruitSprite = "sprites/fruit.png";
    constexpr auto* knightSprite = "sprites/knight.png";
    constexpr auto* platformsSprite = "sprites/platforms.png";
    constexpr auto* greenSlimeSprite = "sprites/slime_green.png";
    constexpr auto* purpleSlimeSprite = "sprites/slime_purple.png";
    constexpr auto* worldSprite = "sprites/world-tileset.png";

    // Fonts

    constexpr auto* normalFont = "fonts/PixelOperator8.ttf";
    constexpr auto* boldFont = "fonts/PixelOperator8-Bold.ttf";

    [[nodiscard]] constexpr auto GetAssetFile(const char* name) -> std::filesystem::path {
      return std::filesystem::path{ ASSETS_DIR } / name;
    }
  }

  class Platformer final : public Scene::Node {
    public:
    explicit Platformer(const ObjectRef<Scene>& scene) :
      Node{scene}, _app{scene->GetApp()}, _render{scene->AppRenderer<SDLHW2D>()} {}

    auto Create() -> bool override;
    auto Update(float dt) -> bool override;

    private:
    SDL_FRect _camera{};
    ObjectRef<App> _app;
    ObjectRef<SDLHW2D> _render;

  };

}

#endif
