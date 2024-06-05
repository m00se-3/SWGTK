#include "Scene.hpp"

#include <filesystem>
#include <utility>

#include "SDLApp.hpp"
#include "gsl/gsl-lite.hpp"

namespace swgtk
{
	GameScene::GameScene(gsl::not_null<SDLApp*> parent, gsl::owner<Node*> node)
	: _parent(parent), _renderer(parent->Renderer()), _pimpl(node)
	{
		InitLua();
	}

	void GameScene::SetNewScene(gsl::owner<Node*> scene)
	{
		if(_pimpl->_destroyFunc.has_value())
		{
			_pimpl->_destroyFunc.value()(*this);
		}

		_pimpl.reset(scene);
	}

	void GameScene::GenerateNewScene(gsl::owner<Node*> ptr)
	{
		_parent->SetNewSceneNode(ptr);
	}

	SSC GameScene::Create()
	{
		_sceneState = _pimpl->_createFunc(*this);
		return _sceneState;
	}

	SSC GameScene::Update(float dt)
	{
		auto result = _pimpl->_updateFunc(*this, dt);

		if(_sceneState != SSC::change_scene)
		{
			_sceneState = result;
		}

		return result;
	}

	void GameScene::Destroy()
	{
		if(_pimpl->_destroyFunc.has_value())
		{
			(_pimpl->_destroyFunc.value())(*this);
		}
	}

	void GameScene::ResetMouseEvents()
	{
		for (auto& s : _mouseEvents)
		{
			s = MButtonState::None;
		}
	}

	gsl::owner<GameScene::Node*> CreateLuaScene(const std::string& luaFileName)
	{
		sol::state lua{};

		if(std::filesystem::exists(luaFileName))
		{
			const sol::protected_function_result file = lua.script_file(luaFileName);

			if(file.valid())
			{
				sol::table functions = file;

				sol::optional<std::function<void(GameScene&)>> dest = functions["OnDestroy"];
				sol::optional<std::function<SSC(GameScene&, float)>> up = functions["OnUpdate"];
				sol::optional<std::function<SSC(GameScene&)>> cr = functions["OnCreate"];
				
				if(up && cr)
				{
					return new GameScene::Node{
						static_cast<std::function<SSC(GameScene&, float)>>(*up),
						static_cast<std::function<SSC(GameScene&)>>(*cr),
						std::optional<std::function<void(GameScene&)>>{static_cast<std::function<void(GameScene&)>>(*dest)}
					};
				}
			}
		}

		return nullptr;
	}

	void GameScene::InitLua()
	{
		_lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);

		// Define useful enums and types.

		{

			_lua.new_enum<MButton>("Btn",
				{
					std::make_pair("None", MButton::None),
					std::make_pair("Left", MButton::Left),
					std::make_pair("Middle", MButton::Middle),
					std::make_pair("Right", MButton::Right),
					std::make_pair("Ex1", MButton::Ex1),
					std::make_pair("Ex2", MButton::Ex2),
				}
			);

			_lua.new_enum<KeyMod>("KMod",
				{
					std::make_pair("None", KeyMod::None),
					std::make_pair("LShift", KeyMod::LShift),
					std::make_pair("RShift", KeyMod::RShift),
					std::make_pair("LCtrl", KeyMod::LCtrl),
					std::make_pair("RCtrl", KeyMod::RCtrl),
					std::make_pair("LAlt", KeyMod::LAlt),
					std::make_pair("RAlt", KeyMod::RAlt),
					std::make_pair("NumLock", KeyMod::NumLock),
					std::make_pair("Caps", KeyMod::Caps),
					std::make_pair("Ctrl", KeyMod::Ctrl),
					std::make_pair("Shift", KeyMod::Shift),
					std::make_pair("Alt", KeyMod::Alt),
				}
			);

			_lua.new_enum<KeyCode>("KValue",
				{
					std::make_pair("Unknown", KeyCode::Unknown), std::make_pair("Back", KeyCode::Back), std::make_pair("Tab", KeyCode::Tab), std::make_pair("Enter", KeyCode::Enter),
					std::make_pair("Esc", KeyCode::Esc), std::make_pair("Space", KeyCode::Space), std::make_pair("Exlaim", KeyCode::Exlaim), std::make_pair("DblQuote", KeyCode::DblQuote),
					std::make_pair("Hash", KeyCode::Hash), std::make_pair("Dollar", KeyCode::Dollar), std::make_pair("Prct", KeyCode::Prct), std::make_pair("Amp", KeyCode::Amp),
					std::make_pair("Quote", KeyCode::Quote), std::make_pair("Ast", KeyCode::Ast), std::make_pair("Plus", KeyCode::Plus), std::make_pair("Comma", KeyCode::Comma),
					std::make_pair("Minus", KeyCode::Minus), std::make_pair("Period", KeyCode::Period), std::make_pair("FSlash", KeyCode::FSlash), std::make_pair("R0", KeyCode::R0),
					std::make_pair("R1", KeyCode::R1), std::make_pair("R2", KeyCode::R2), std::make_pair("R3", KeyCode::R3), std::make_pair("R4", KeyCode::R4),
					std::make_pair("R5", KeyCode::R5), std::make_pair("R6", KeyCode::R6), std::make_pair("R7", KeyCode::R7), std::make_pair("R8", KeyCode::R8),
					std::make_pair("R9", KeyCode::R9), std::make_pair("Colon", KeyCode::Colon), std::make_pair("SemiColon", KeyCode::SemiColon), std::make_pair("Less", KeyCode::Less),
					std::make_pair("Equals", KeyCode::Equals), std::make_pair("Greater", KeyCode::Greater), std::make_pair("Question", KeyCode::Question), std::make_pair("At", KeyCode::At),
					std::make_pair("LBracket", KeyCode::LBracket), std::make_pair("BSlash", KeyCode::BSlash), std::make_pair("RBracket", KeyCode::RBracket), std::make_pair("Caret", KeyCode::Caret),
					std::make_pair("UnderScore", KeyCode::UnderScore), std::make_pair("BQuote", KeyCode::BQuote), std::make_pair("A", KeyCode::A), std::make_pair("B", KeyCode::B),
					std::make_pair("C", KeyCode::C), std::make_pair("D", KeyCode::D), std::make_pair("E", KeyCode::E), std::make_pair("F", KeyCode::F),
					std::make_pair("G", KeyCode::G), std::make_pair("H", KeyCode::H), std::make_pair("I", KeyCode::I), std::make_pair("J", KeyCode::J),
					std::make_pair("K", KeyCode::K), std::make_pair("L", KeyCode::L), std::make_pair("M", KeyCode::M), std::make_pair("N", KeyCode::N),
					std::make_pair("O", KeyCode::O), std::make_pair("P", KeyCode::P), std::make_pair("Q", KeyCode::Q), std::make_pair("R", KeyCode::R),
					std::make_pair("S", KeyCode::S), std::make_pair("T", KeyCode::T), std::make_pair("U", KeyCode::U), std::make_pair("V", KeyCode::V),
					std::make_pair("W", KeyCode::W), std::make_pair("X", KeyCode::X), std::make_pair("Y", KeyCode::Y), std::make_pair("Z", KeyCode::Z),
					std::make_pair("Delete", KeyCode::Delete),
				}
			);

			_lua.new_enum<LayoutCode>("KCode",
				{
					std::make_pair("Unknown", LayoutCode::Unknown), std::make_pair("A", LayoutCode::A), std::make_pair("B", LayoutCode::B),
					std::make_pair("C", LayoutCode::C), std::make_pair("D", LayoutCode::D), std::make_pair("E", LayoutCode::E),
					std::make_pair("F", LayoutCode::F), std::make_pair("G", LayoutCode::G), std::make_pair("H", LayoutCode::H),
					std::make_pair("I", LayoutCode::I), std::make_pair("J", LayoutCode::J), std::make_pair("K", LayoutCode::K),
					std::make_pair("L", LayoutCode::L), std::make_pair("M", LayoutCode::M), std::make_pair("N", LayoutCode::N),
					std::make_pair("O", LayoutCode::O), std::make_pair("P", LayoutCode::P), std::make_pair("Q", LayoutCode::Q),
					std::make_pair("R", LayoutCode::R), std::make_pair("S", LayoutCode::S), std::make_pair("T", LayoutCode::T),
					std::make_pair("U", LayoutCode::U), std::make_pair("V", LayoutCode::V), std::make_pair("W", LayoutCode::W),
					std::make_pair("X", LayoutCode::X), std::make_pair("Y", LayoutCode::Y), std::make_pair("Z", LayoutCode::Z),
					std::make_pair("R1", LayoutCode::R1), std::make_pair("R2", LayoutCode::R2), std::make_pair("R3", LayoutCode::R3),
					std::make_pair("R4", LayoutCode::R4), std::make_pair("R5", LayoutCode::R5), std::make_pair("R6", LayoutCode::R6),
					std::make_pair("R7", LayoutCode::R7), std::make_pair("R8", LayoutCode::R8), std::make_pair("R9", LayoutCode::R9),
					std::make_pair("R0", LayoutCode::R0), std::make_pair("Enter", LayoutCode::Enter), std::make_pair("Esc", LayoutCode::Esc),
					std::make_pair("Back", LayoutCode::Back), std::make_pair("Tab", LayoutCode::Tab), std::make_pair("Space", LayoutCode::Space),
					std::make_pair("Rminus", LayoutCode::Rminus), std::make_pair("Equals", LayoutCode::Equals), std::make_pair("LBracket", LayoutCode::LBracket),
					std::make_pair("RBracket", LayoutCode::RBracket), std::make_pair("BSlash", LayoutCode::BSlash), std::make_pair("SemiColon", LayoutCode::SemiColon),
					std::make_pair("Apostrophe", LayoutCode::Apostrophe), std::make_pair("Comma", LayoutCode::Comma), std::make_pair("Period", LayoutCode::Period),
					std::make_pair("FSlash", LayoutCode::FSlash), std::make_pair("CapsLock", LayoutCode::CapsLock), std::make_pair("F1", LayoutCode::F1),
					std::make_pair("F2", LayoutCode::F2), std::make_pair("F3", LayoutCode::F3), std::make_pair("F4", LayoutCode::F4),
					std::make_pair("F5", LayoutCode::F5), std::make_pair("F6", LayoutCode::F6), std::make_pair("F7", LayoutCode::F7),
					std::make_pair("F8", LayoutCode::F8), std::make_pair("F9", LayoutCode::F9), std::make_pair("F10", LayoutCode::F10),
					std::make_pair("F11", LayoutCode::F11), std::make_pair("F12", LayoutCode::F12), std::make_pair("PRTSCRN", LayoutCode::PRTSCRN),
					std::make_pair("ScrLock", LayoutCode::ScrLock), std::make_pair("Pause", LayoutCode::Pause), std::make_pair("Insert", LayoutCode::Insert),
					std::make_pair("Home", LayoutCode::Home), std::make_pair("PgUp", LayoutCode::PgUp), std::make_pair("Delete", LayoutCode::Delete),
					std::make_pair("End", LayoutCode::End), std::make_pair("PgDown", LayoutCode::PgDown), std::make_pair("Right", LayoutCode::Right),
					std::make_pair("Left", LayoutCode::Left), std::make_pair("Down", LayoutCode::Down), std::make_pair("Up", LayoutCode::Up),
					std::make_pair("NumLock", LayoutCode::NumLock), std::make_pair("KPDivide", LayoutCode::KPDivide), std::make_pair("KPMultiply", LayoutCode::KPMultiply),
					std::make_pair("KPMinus", LayoutCode::KPMinus), std::make_pair("KPPlus", LayoutCode::KPPlus), std::make_pair("KPEnter", LayoutCode::KPEnter),
					std::make_pair("KP1", LayoutCode::KP1), std::make_pair("KP2", LayoutCode::KP2), std::make_pair("KP3", LayoutCode::KP3),
					std::make_pair("KP4", LayoutCode::KP4), std::make_pair("KP5", LayoutCode::KP5), std::make_pair("KP6", LayoutCode::KP6),
					std::make_pair("KP7", LayoutCode::KP7), std::make_pair("KP8", LayoutCode::KP8), std::make_pair("KP9", LayoutCode::KP9),
					std::make_pair("KP0", LayoutCode::KP0), std::make_pair("KPPeriod", LayoutCode::KPPeriod), std::make_pair("LCtrl", LayoutCode::LCtrl),
					std::make_pair("LShift", LayoutCode::LShift), std::make_pair("LAlt", LayoutCode::LAlt), std::make_pair("RCtrl", LayoutCode::RCtrl),
					std::make_pair("RShift", LayoutCode::RShift), std::make_pair("RAlt", LayoutCode::RAlt),
				}
			);

			_lua.new_enum<SSC>("SSC", 
				{
					std::make_pair("ok", SSC::ok),
					std::make_pair("change_scene", SSC::change_scene),
					std::make_pair("fail", SSC::fail)
				}
			);

			_lua.new_enum<sdl::FontStyle>("FontStyle",
				{
					std::make_pair("Normal", sdl::FontStyle::Normal),
					std::make_pair("Bold", sdl::FontStyle::Bold),
					std::make_pair("Italic", sdl::FontStyle::Italic),
					std::make_pair("Underlined", sdl::FontStyle::Underlined),
					std::make_pair("Bold_Italic", sdl::FontStyle::Bold_Italic),
					std::make_pair("Bold_Underlinded", sdl::FontStyle::Bold_Underlinded),
					std::make_pair("Bold_Italic_Underlined", sdl::FontStyle::Bold_Italic_Underlined),
					std::make_pair("Italic_Underlined", sdl::FontStyle::Italic_Underlined)
				}
			);
		}

		auto point = _lua.new_usertype<SDL_FPoint>(
		"Vec2f", "x", &SDL_FPoint::x, "y", &SDL_FPoint::y
		);
		
		point["new"] = [](sol::optional<float> nx, sol::optional<float> ny) -> SDL_FPoint
			{
				return SDL_FPoint{ *nx, *ny };
			};

		auto rect = _lua.new_usertype<SDL_FRect>(
		"Rect",	"x", &SDL_FRect::x, "y", &SDL_FRect::y, "w", &SDL_FRect::w, "h", &SDL_FRect::h
		);

		rect["new"] = [](sol::optional<float> nx, sol::optional<float> ny, sol::optional<float> nw, sol::optional<float> nh) -> SDL_FRect
			{
				return SDL_FRect{ *nx, *ny, *nw, *nh };
			};

		// Define functions for Lua.

		_lua["GetScroll"] = [this]() -> float
			{
				return GetScroll();
			};

		_lua["IsKeyPressed"] = [this](sol::optional<LayoutCode> key) -> bool
			{
				return IsKeyPressed(*key);
			};

		_lua["IsKeyReleased"] = [this](sol::optional<LayoutCode> key) -> bool
			{
				return IsKeyReleased(*key);
			};

		_lua["IsKeyHeld"] = [this](sol::optional<LayoutCode> key) -> bool
			{
				return IsKeyHeld(*key);
			};

		_lua["IsButtonPressed"] = [this](sol::optional<MButton> btn) -> bool
			{
				return IsButtonPressed(*btn);
			};

		_lua["IsButtonReleased"] = [this](sol::optional<MButton> btn) -> bool
			{
				return IsButtonReleased(*btn);
			};

		_lua["IsButtonHeld"] = [this](sol::optional<MButton> btn) -> bool
			{
				return IsButtonHeld(*btn);
			};

		_lua["GetKeyMods"] = [this]() -> KeyMod
			{
				return GetKeyMods();
			};

		_lua["GetMouseX"] = [this]() -> int
			{
				return GetMouseX();
			};

		_lua["GetMouseY"] = [this]() -> int
			{
				return GetMouseY();
			};

	}
}
