#include "scenes/Scene.hpp"

#include "SDLApp.hpp"

namespace swgtk
{
	Scene::Scene(SDLApp* app)
		: _parent(app)
	{

	}

	SceneFactory Scene::GetNextScene()
	{
		return nextScene;
	}

	float Scene::GetScroll() const
	{
		return _scroll;
	}

	bool Scene::IsKeyPressed(LayoutCode code) const
	{
		return (_keyEvent.first == code && _keyEvent.second == true);
	}

	bool Scene::IsKeyReleased(LayoutCode code) const
	{
		return (_keyEvent.first == code && _keyEvent.second == false);
	}

	bool Scene::IsKeyHeld(LayoutCode code) const
	{
		return _keyboardState[uint64_t(code)] == 1;
	}

	KeyMod Scene::GetKeyMods() const
	{
		return _modifiers;
	}

	bool Scene::IsButtonPressed(MButton button) const
	{
		return _mouseEvents[uint64_t(button) - 1u] == MButtonState::Pressed;
	}

	bool Scene::IsButtonReleased(MButton button) const
	{
		return _mouseEvents[uint64_t(button) - 1u] == MButtonState::Released;
	}

	bool Scene::IsButtonHeld(MButton button) const
	{
		return static_cast<uint32_t>(_mouseState.buttons) & static_cast<uint32_t>(button);
	}

	int Scene::GetMouseX() const
	{
		return _mouseState.x;
	}

	int Scene::GetMouseY() const
	{
		return _mouseState.y;
	}

	void Scene::SetMouseState(const MouseState& event)
	{
		_mouseState = event;
	}

	void Scene::SetModState(const SDL_Keymod& state)
	{
		_modifiers = static_cast<KeyMod>(state);
	}

	void Scene::SetKeyboardState(const uint8_t* state)
	{
		_keyboardState = state;
	}

	void Scene::ResetScroll()
	{
		_scroll = 0.0f;
	}

	void Scene::AddScroll(float amount)
	{
		_scroll = amount;
	}

	void Scene::SetMouseEvent(MButton button, MButtonState state)
	{
		_mouseEvents[uint64_t(button) - 1u] = state;
	}

	void Scene::ResetMouseEvents()
	{
		for (auto& s : _mouseEvents)
		{
			s = MButtonState::None;
		}
	}

	void Scene::ResetKeyEvent()
	{
		_keyEvent.first = LayoutCode::Unknown;
		_keyEvent.second = false;
	}

	void Scene::SetKeyEvent(LayoutCode code, bool pressed)
	{
		_keyEvent.first = code;
		_keyEvent.second = pressed;
	}
	
	SDLApp* Scene::Parent()
	{
		return _parent;
	}
}