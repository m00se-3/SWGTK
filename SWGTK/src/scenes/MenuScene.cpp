#include "scenes/MenuScene.hpp"

#include <string>

#include "SDLApp.hpp"

namespace swgtk
{
	MenuScene::MenuScene(SDLApp* app)
		: Scene(app)
	{
	}

	SSC MenuScene::Create()
	{
		auto* font = Parent()->GetTTF(FontStyle::Bold, 16);

		_background.Create(Parent()->Renderer(), Parent()->AssetsDir() + "/Card Assets/Backgrounds/background_2.png");

		SDL_Surface* surf = TTF_RenderUTF8_Blended_Wrapped(font, "Play\nGame!", SDL_Color{ 230, 0, 0, 255 }, 0u);

		if (surf)
		{
			auto& tex = _freeTextItems.emplace_back(Parent()->Renderer(), surf);

			SDL_FreeSurface(surf);
		}
		
		return statusCode;
	}

	SSC MenuScene::Update(float dt)
	{
		// Draw the background, the texture has visible edges that need trimmed.
		int w = 0, h = 0;
		auto rect = SDL_Rect{ 0, 0, 0, 0 };

		SDL_QueryTexture(_background.Get(), nullptr, nullptr, &w, &h);
		rect.w = w - 2;
		rect.h = h - 3;

		SDL_RenderCopy(Parent()->Renderer(), _background.Get(), &rect, nullptr);

		for (auto& texture : _freeTextItems)
		{
			// We are stretching the sample texture here, so it will look pixelated.

			auto rect = SDL_Rect{ 50, 200, 100, 100 };
			
			SDL_RenderCopy(Parent()->Renderer(), texture.Get(), nullptr, &rect);
		}
		
		return statusCode;
	}
}