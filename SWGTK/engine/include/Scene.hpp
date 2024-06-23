#ifndef SWGTK_SCENE_HPP
#define SWGTK_SCENE_HPP

#include <SDL_rect.h>
#include <functional>
#include <memory>
#include <span>

#include "sol/sol.hpp"
#include "gsl-lite/gsl-lite.hpp"

#include "Input.hpp"
#include "RenderWrap.hpp"

namespace swgtk
{

	class SDLApp;

	/*
		This signals the internal state machine what to do to the
		current scene next update loop.
	*/
	enum struct SceneStateCode : uint8_t
	{
		Fail = 0,
		Ok,
		ChangeScene
	};

	using SSC = SceneStateCode;

	/*
		Describes the game simulation.

		A game scene has the following lifetime:

		- Create() is called once after the scene is initialized. This allows the program
			to initialize the simulation and allocate any needed resources.

		- Update(float) is called once each frame. This is where all the simulation logic
			kept, including input handling.

		- Destroy is called once at the end of the scene's lifetime. This function is *optional*.
			You only need to use it if you are using non-RAII structures for your allocated
			resources. (This is not recommended!)
	*/
	class GameScene
	{
	public:

		struct Node
		{
			std::function<SSC(GameScene&)> _createFunc;
			std::function<SSC(GameScene&, float)> _updateFunc;
			std::optional<std::function<void(GameScene&)>> _destroyFunc;
		};
	
		GameScene(gsl::not_null<SDLApp*> parent, gsl::owner<Node*> node);

		[[nodiscard]] SSC Create();
		[[nodiscard]] SSC Update(float dt);
		void Destroy();
		void InitLua();
		void SetNewScene(gsl::owner<Node*> scene);
		void GenerateNewScene(gsl::owner<Node*> ptr);

		[[nodiscard]] constexpr SDLApp* AppRoot(this GameScene& self) { return self._parent; }
		[[nodiscard]] constexpr RenderWrapper& Renderer(this GameScene& self) { return self._renderer; }

		/*
			Input state and event polling for the derived scene class.
		*/

		[[nodiscard]] constexpr float GetScrollX() const { return _scroll.x; }
		[[nodiscard]] constexpr float GetScrollY() const { return _scroll.y; }
		[[nodiscard]] constexpr bool IsKeyPressed(LayoutCode code) const { return (_keyEvent.first == code && _keyEvent.second); }
		[[nodiscard]] constexpr bool IsKeyReleased(LayoutCode code) const { return (_keyEvent.first == code && !_keyEvent.second); }
		[[nodiscard]] constexpr bool IsKeyHeld(LayoutCode code) const{ return _keyboardState[static_cast<size_t>(code)] == 1; }
		[[nodiscard]] constexpr std::pair<LayoutCode, bool> GetCurrentKeyEvent() const { return _keyEvent; }
		[[nodiscard]] constexpr KeyMod GetKeyMods() const { return _modifiers; }
		[[nodiscard]] constexpr bool IsButtonPressed(MButton button) const { return _mouseEvents.at(static_cast<size_t>(button)) == MButtonState::Pressed; }
		[[nodiscard]] constexpr bool IsButtonReleased(MButton button) const { return _mouseEvents.at(static_cast<size_t>(button)) == MButtonState::Released; }
		[[nodiscard]] constexpr bool IsButtonHeld(MButton button) const { return static_cast<bool>(static_cast<uint32_t>(_mouseState.buttons) & static_cast<uint32_t>(button)); }
		[[nodiscard]] constexpr int GetMouseX() const { return _mouseState.x; }
		[[nodiscard]] constexpr int GetMouseY() const { return _mouseState.y; }
		[[nodiscard]] constexpr SDL_Point GetMousePos() const { return SDL_Point{ _mouseState.x, _mouseState.y }; }

		/*
			Input state and event management.
		*/

		constexpr void SetMouseState(const MouseState& event) { _mouseState = event; }
		constexpr void SetModState(const SDL_Keymod& state) { _modifiers = static_cast<KeyMod>(state); }
		constexpr void ResetScroll() { _scroll = { 0.f, 0.f }; }
		constexpr void AddScroll(float amountX, float amountY) { _scroll = { amountX, amountY }; }
		constexpr void SetMouseEvent(MButton button, MButtonState state) { _mouseEvents.at(static_cast<size_t>(button)) = state; }

		constexpr void ResetKeyEvent()
		{
			_keyEvent.first = LayoutCode::Unknown;
			_keyEvent.second = false;	
		}

		constexpr void SetKeyEvent(LayoutCode code, bool pressed)
		{	
			_keyEvent.first = code;
			_keyEvent.second = pressed;
		}

		void SetKeyboardState() 
		{
			int numKeys{};
			const uint8_t* state = SDL_GetKeyboardState(&numKeys);
			_keyboardState = std::span<const uint8_t>{ state, static_cast<size_t>(numKeys) };
		}
		
		void ResetMouseEvents();

	private:
		SDLApp* _parent = nullptr;
		RenderWrapper _renderer;
		std::unique_ptr<Node> _pimpl;
		sol::state _lua; 
		SSC _sceneState = SSC::Ok;

		/*
			State management variables for input polling.
		*/

		MouseState _mouseState{};
		KeyMod _modifiers = KeyMod::None;
		std::span<const uint8_t>_keyboardState;

		/*
			Variables for processing input events.
		*/

		std::array<MButtonState, 6u> _mouseEvents = { MButtonState::None }; //NOLINT
		std::pair<LayoutCode, bool> _keyEvent = std::make_pair(LayoutCode::Unknown, false);
		SDL_FPoint _scroll{};

	};

	[[nodiscard]] gsl::owner<GameScene::Node*> CreateLuaScene(sol::state& state, const std::string& luaFileName);

}

#endif // !SWGTK_SCENE_HPP
