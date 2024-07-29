#include "Tests.hpp"
#include "Input.hpp"
#include "UI.hpp"
#include <memory>

namespace swgtk::tests 
{
	void Test_Suite::Config(SDLApp& app)
	{
		_ui = std::make_unique<UI>(&app, _assets + "/fonts");
		[[maybe_unused]] auto err = _ui->LoadScriptsFromDirectory(_config + "/ui");
	}

	void Test_Suite::UpdateUI(swgtk::GameScene& app)
	{
		//Forward mouse and keyboard events to nuklear.
		
		auto mouseX = app.GetMouseX();
		auto mouseY = app.GetMouseY();

		nk_input_begin(_ui->Context());

		nk_input_motion(_ui->Context(), mouseX, mouseY);

		if(app.IsButtonPressed(swgtk::MButton::Left) || app.IsButtonReleased(swgtk::MButton::Left))
		{
			auto button = SDLButtontoNKButton(static_cast<int>(swgtk::MButton::Left)); 	
			nk_input_button(_ui->Context(), static_cast<nk_buttons>(button), mouseX, mouseY, static_cast<nk_bool>(app.IsButtonPressed(swgtk::MButton::Left)));
		}

		if(app.IsButtonPressed(swgtk::MButton::Right) || app.IsButtonReleased(swgtk::MButton::Right))
		{
			auto button = SDLButtontoNKButton(static_cast<uint8_t>(swgtk::MButton::Right)); 	
			nk_input_button(_ui->Context(), static_cast<nk_buttons>(button), mouseX, mouseY, static_cast<nk_bool>(app.IsButtonPressed(swgtk::MButton::Right)));
		}

		auto keyEvent = app.GetCurrentKeyEvent();
		auto key = SDLKeytoNKKey(static_cast<int>(keyEvent.first), static_cast<uint16_t>(app.GetKeyMods()));
		nk_input_key(_ui->Context(), key, static_cast<nk_bool>(keyEvent.second));

		nk_input_scroll(_ui->Context(), nk_vec2(app.GetScrollX(), app.GetScrollY()));

		nk_input_end(_ui->Context());

		_ui->Update();

	}

	void Test_Suite::DrawUI(swgtk::GameScene& app)
	{
		_ui->Draw(&app.Renderer());
	}

	int Test_Suite::SDLButtontoNKButton(uint8_t button)
	{
		switch(button)
		{
			case SDL_BUTTON_LEFT: return NK_BUTTON_LEFT;
			case SDL_BUTTON_RIGHT: return NK_BUTTON_RIGHT;
			case SDL_BUTTON_MIDDLE: return NK_BUTTON_MIDDLE;
			default: return -1;
		}
	}

	nk_keys Test_Suite::SDLKeytoNKKey(int key, uint16_t mods) // NOLINT
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

	TestNone::TestNone(Test_Suite* suite) : _parent(suite) {}

	SSC TestNone::Create(GameScene& app)
	{
		
		return SSC::Ok;
	}

	SSC TestNone::Update(GameScene& app, [[maybe_unused]] float dt)
	{
		_parent->UpdateUI(app);
		

		_parent->DrawUI(app);

		return SSC::Ok;
	}

}
