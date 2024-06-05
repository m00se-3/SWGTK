#include "OpenScene.hpp"
#include "SDLApp.hpp"

swgtk::SSC WelcomeScreen::Create(swgtk::GameScene& app)
{
	auto* host = app.AppRoot();

	_ui = std::make_unique<swgtk::UI>(host, host->ConfigDir() + "/ui");

	_texture = app.Renderer().LoadTextureImg(_img);
	return swgtk::SSC::ok;
}

swgtk::SSC WelcomeScreen::Update(swgtk::GameScene& app, [[maybe_unused]] float dt)
{
	//Forward mouse and keyboard events.
	
	// auto button = SDLButtontoNKButton(e.button.button);

	nk_input_motion(_ui->Context(), app.GetMouseX(), app.GetMouseY());
	// nk_input_button(_ui->Context(), static_cast<nk_buttons>(button), e.button.x, e.button.y, static_cast<nk_bool>(e.type == SDL_MOUSEBUTTONDOWN));
	// nk_input_key(&_ctx, key, static_cast<nk_bool>(e.type == SDL_KEYDOWN));
	// auto key = SDLKeytoNKKey(e.key.keysym.sym, e.key.keysym.mod);
	// nk_input_scroll(&_ctx, nk_vec2(e.wheel.preciseX, e.wheel.preciseY));

	app.Renderer().DrawTexture(gsl::make_not_null(_texture.Get()), nullptr, &_rect);
	return swgtk::SSC::ok;
}

int WelcomeScreen::SDLButtontoNKButton(uint8_t button)
{
	switch(button)
	{
		case SDL_BUTTON_LEFT: return NK_BUTTON_LEFT;
		case SDL_BUTTON_RIGHT: return NK_BUTTON_RIGHT;
		case SDL_BUTTON_MIDDLE: return NK_BUTTON_MIDDLE;
		default: return -1;
	}
}

nk_keys WelcomeScreen::SDLKeytoNKKey(int key, uint16_t mods) // NOLINT
{
	switch (key)
	{
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
	{
		return NK_KEY_SHIFT;
	}

	case SDLK_LCTRL:
	case SDLK_RCTRL:
	{
		return NK_KEY_CTRL;
	}

	case SDLK_RETURN:
	{
		return NK_KEY_ENTER;
	}

	case SDLK_BACKSPACE:
	{
		return NK_KEY_BACKSPACE;
	}

	case SDLK_TAB:
	{
		return NK_KEY_TAB;
	}

	case SDLK_DELETE:
	{
		return NK_KEY_DEL;
	}

	case SDLK_RIGHT:
	{
		return NK_KEY_RIGHT;
	}

	case SDLK_LEFT:
	{
		return NK_KEY_LEFT;
	}

	case SDLK_DOWN:
	{
		return NK_KEY_DOWN;
	}

	case SDLK_UP:
	{
		return NK_KEY_UP;
	}
	
	default: 
	{
		return NK_KEY_NONE; // Not an NK key
	}
	}
}
