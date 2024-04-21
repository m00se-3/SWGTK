#include "MenuScene.hpp"

#include <cstdio>
#include <string>

#include "Font.hpp"
#include "SDLApp.hpp"
#include "SDL_rect.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "Scene.hpp"
#include "UI.hpp"
#include "sol/protected_function_result.hpp"
#include "sol/table.hpp"


namespace swgtk 
{
	MenuScene::MenuScene(SDLApp *app) : Scene(app) { InitLua(); }

	SSC MenuScene::Create() {
	auto *font = Parent()->GetTTF(FontStyle::Bold, normalFontSize);

	_background.Create(Parent()->Renderer(), Parent()->AssetsDir() + "/Card Assets/Backgrounds/background_2.png");

	const sol::protected_function_result welcome = Lua().script_file(Parent()->ConfigDir() + "/data/welcome_screen.lua");

	if (welcome.valid()) 
	{
		sol::table data = welcome;
		sol::table buttons = data["main_buttons"];

		const auto size = buttons.size();

		for (auto index = 1uz; index <= size; ++index) {
			const std::string &text = buttons[index]["text"].get<std::string>();
			const auto &bounds = buttons[index]["bounds"].get<sol::table>();

			SDL_Surface *surf = TTF_RenderUTF8_Blended(font, text.c_str(), SDL_Color{255, 0, 0, 255}); // NOLINT

			_freeTextItems.emplace_back(Parent()->Renderer(), surf);
			_freeTextBounds.emplace_back(SDL_Rect{bounds["x"].get<int>(), bounds["y"].get<int>(), bounds["w"].get<int>(), bounds["h"].get<int>()});

			SDL_FreeSurface(surf);
		}
	}

	return GetSceneState();
	}

	SSC MenuScene::Update(float dt) 
	{
		// Draw the background, the texture has visible edges that need trimmed.
		int w{}, h{};
		auto rect = SDL_Rect{0, 0, 0, 0};

		// We have to adjust the width and height variables because the background
		// image doesn't go to the edge of the image buffer.
		SDL_QueryTexture(_background.Get(), nullptr, nullptr, &w, &h);
		rect.w = w - 2;
		rect.h = h - 3;

		SDL_RenderCopy(Parent()->Renderer(), _background.Get(), &rect, nullptr);

		for (auto index = 0uz; index < _freeTextItems.size(); ++index) {
			SDL_RenderCopy(Parent()->Renderer(), _freeTextItems[index].Get(), nullptr,
						&_freeTextBounds[index]);
		}

	return GetSceneState();
	}
} // namespace swgtk
