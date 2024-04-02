#ifndef SWGTK_SCENE_HPP
#define SWGTK_SCENE_HPP

#include <functional>
#include <memory>
#include <concepts>
#include <span>

#include "sol/sol.hpp"

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
	enum struct SceneStateCode
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

		float GetScroll() const;
		bool IsKeyPressed(LayoutCode code) const;
		bool IsKeyReleased(LayoutCode code) const;
		bool IsKeyHeld(LayoutCode code) const;
		KeyMod GetKeyMods() const;
		bool IsButtonPressed(MButton button) const;
		bool IsButtonReleased(MButton button) const;
		bool IsButtonHeld(MButton button) const;
		int GetMouseX() const;
		int GetMouseY() const;
		SDL_Point GetMousePos() const;

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
		sol::state _lua{}; 
		SSC sceneState = SSC::ok;
		SceneFactory nextScene{};

		/*
			State management variables for inpuit polling.
		*/

		MouseState _mouseState{};
		KeyMod _modifiers = KeyMod::None;
		std::span<const uint8_t>_keyboardState{};

		/*
			Variables for processing input events.
		*/

		std::array<MButtonState, 6u> _mouseEvents = { MButtonState::None }; //NOLINT
		std::pair<LayoutCode, bool> _keyEvent = std::make_pair(LayoutCode::Unknown, false);
		float _scroll = 0.0f;
	};

}

#endif // !SWGTK_SCENE_HPP
