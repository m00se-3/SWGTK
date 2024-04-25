#ifndef SWGTK_SCENE_HPP
#define SWGTK_SCENE_HPP

#include <functional>
#include <memory>
#include <concepts>
#include <span>

#include "sol/sol.hpp"
#include "gsl-lite/gsl-lite.hpp"

#include "Input.hpp"

namespace swgtk
{

	class SDLApp;
	class Scene;

	template <typename T>
	concept SceneObject = std::derived_from<T, Scene>;

	using SceneHandle = std::unique_ptr<Scene>;
	using SceneFactory = std::function<SceneHandle()>;

	/*
		This signals the internal state machine what to do to the
		current scene next update loop.
	*/
	enum struct SceneStateCode : uint8_t
	{
		fail = 0,
		ok,
		change_scene
	};

	using SSC = SceneStateCode;


	class Scene
	{
	public:
		Scene(SDLApp* app);
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;

		Scene& operator=(const Scene&) = delete;
		Scene& operator=(Scene&&) = delete;

		virtual ~Scene() = default;

		virtual SSC Create() = 0;
		virtual SSC Update(float dt) = 0;

		[[nodiscard]] SceneFactory GetNextScene();
		[[nodiscard]] SSC GetSceneState() const;

		/*
			Input state and event polling for the derived scene class.
		*/

		[[nodiscard]] float GetScroll() const;
		[[nodiscard]] bool IsKeyPressed(LayoutCode code) const;
		[[nodiscard]] bool IsKeyReleased(LayoutCode code) const;
		[[nodiscard]] bool IsKeyHeld(LayoutCode code) const;
		[[nodiscard]] KeyMod GetKeyMods() const;
		[[nodiscard]] bool IsButtonPressed(MButton button) const;
		[[nodiscard]] bool IsButtonReleased(MButton button) const;
		[[nodiscard]] bool IsButtonHeld(MButton button) const;
		[[nodiscard]] int GetMouseX() const;
		[[nodiscard]] int GetMouseY() const;
		[[nodiscard]] SDL_Point GetMousePos() const;

		/*
			Internal input state and event management.
		*/

		void SetMouseState(const MouseState& event);
		void SetModState(const SDL_Keymod& state);
		void SetKeyboardState();
		void ResetScroll();
		void AddScroll(float amount);
		void SetMouseEvent(MButton button, MButtonState state);
		void ResetMouseEvents();
		void ResetKeyEvent();
		void SetKeyEvent(LayoutCode code, bool pressed);

	protected:
		/*
			To change scenes you simply:

			SwitchToScene<[your scene class]>([pass optional arguments]);

			Note: The Scene change will take effect after returning from Update().
		*/
		template<SceneObject T, typename... Args>
		void SwitchToScene(Args&&... args) // NOLINT
		{
			sceneState = SSC::change_scene;
			nextScene = [&]() -> std::unique_ptr<T>{
				return std::make_unique<T>(std::forward<Args>(args)...);
				};
		}

		SDLApp* Parent();

		/*
			Initializes the Lua state by passing event handlers and basic SDL utilities common to all games.
		*/
		void InitLua();
		sol::state& Lua();

	private:
		SDLApp* _parent = nullptr;
		sol::state _lua; 
		SSC sceneState = SSC::ok;
		SceneFactory nextScene;

		/*
			State management variables for inpuit polling.
		*/

		MouseState _mouseState{};
		KeyMod _modifiers = KeyMod::None;
		std::span<const uint8_t>_keyboardState;

		/*
			Variables for processing input events.
		*/

		std::array<MButtonState, 6u> _mouseEvents = { MButtonState::None }; //NOLINT
		std::pair<LayoutCode, bool> _keyEvent = std::make_pair(LayoutCode::Unknown, false);
		float _scroll = 0.0f;
	};


	// Attempt to rewrite Scene class following the CPP Core Guidelines for classes.

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
		struct SceneNode;
	
		GameScene(gsl::not_null<SDLApp*> parent, gsl::owner<SceneNode*> node);

		[[nodiscard]] SSC Create();
		[[nodiscard]] SSC Update(float dt);
		void Destroy();
		void InitLua();

		[[nodiscard]] constexpr SDLApp* AppRoot() { return _parent; }

		/*
			Input state and event polling for the derived scene class.
		*/

		[[nodiscard]] constexpr float GetScroll() const { return _scroll; }
		[[nodiscard]] constexpr bool IsKeyPressed(LayoutCode code) const { return (_keyEvent.first == code && _keyEvent.second); }
		[[nodiscard]] constexpr bool IsKeyReleased(LayoutCode code) const { return (_keyEvent.first == code && !_keyEvent.second); }
		[[nodiscard]] constexpr bool IsKeyHeld(LayoutCode code) const{ return _keyboardState[size_t(code)] == 1; }
		[[nodiscard]] constexpr KeyMod GetKeyMods() const { return _modifiers; }
		[[nodiscard]] constexpr bool IsButtonPressed(MButton button) const { return _mouseEvents.at(size_t(button)) == MButtonState::Pressed; }
		[[nodiscard]] constexpr bool IsButtonReleased(MButton button) const { return _mouseEvents.at(size_t(button)) == MButtonState::Released; }
		[[nodiscard]] constexpr bool IsButtonHeld(MButton button) const { return static_cast<bool>(static_cast<uint32_t>(_mouseState.buttons) & static_cast<uint32_t>(button)); }
		[[nodiscard]] constexpr int GetMouseX() const { return _mouseState.x; }
		[[nodiscard]] constexpr int GetMouseY() const { return _mouseState.y; }
		[[nodiscard]] constexpr SDL_Point GetMousePos() const { return SDL_Point{ _mouseState.x, _mouseState.y }; }

		/*
			Input state and event management.
		*/

		constexpr void SetMouseState(const MouseState& event) { _mouseState = event; }
		constexpr void SetModState(const SDL_Keymod& state) { _modifiers = static_cast<KeyMod>(state); }
		constexpr void ResetScroll() { _scroll = 0.0f; }
		constexpr void AddScroll(float amount) { _scroll = amount; }
		constexpr void SetMouseEvent(MButton button, MButtonState state) { _mouseEvents.at(size_t(button)) = state; }

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

		inline void SetKeyboardState() 
		{
			int numKeys{};
			const uint8_t* state = SDL_GetKeyboardState(&numKeys);
			_keyboardState = std::span<const uint8_t>{ state, static_cast<size_t>(numKeys) };
		}
		
		void ResetMouseEvents();

		struct SceneNode
		{
			std::function<SSC(GameScene&, float)> _updateFunc;
			std::function<SSC(GameScene&)> _createFunc;
			std::optional<std::function<void(GameScene&)>> _destroyFunc;
		};

	private:
		SDLApp* _parent = nullptr;
		std::unique_ptr<SceneNode> _pimpl;
		sol::state _lua; 
		SSC sceneState = SSC::ok;

		/*
			State management variables for inpuit polling.
		*/

		MouseState _mouseState{};
		KeyMod _modifiers = KeyMod::None;
		std::span<const uint8_t>_keyboardState;

		/*
			Variables for processing input events.
		*/

		std::array<MButtonState, 6u> _mouseEvents = { MButtonState::None }; //NOLINT
		std::pair<LayoutCode, bool> _keyEvent = std::make_pair(LayoutCode::Unknown, false);
		float _scroll = 0.0f;

	};

	[[nodiscard]] gsl::owner<GameScene::SceneNode*> CreateLuaScene(const std::string& luaFileName);

}

#endif // !SWGTK_SCENE_HPP
