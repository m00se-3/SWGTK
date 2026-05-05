#include <Platformer.hpp>
#include <swgtk/App.hpp>

auto swgtk::Platformer::Create() -> bool {

  // TODO: How to retrieve the font later is unclear. Need to rethink this API.
  _app->AddFont(assets::GetAssetFile(assets::normalFont));
  _app->AddFont(assets::GetAssetFile(assets::boldFont));

  return true;
}

auto swgtk::Platformer::Update([[maybe_unused]] float dt) -> bool {

  return true;
}