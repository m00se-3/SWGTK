#ifndef SWGTK_TEST_SCENES_HPP
#define SWGTK_TEST_SCENES_HPP

#include "Scene.hpp"
#include "NKUI.hpp"
#include "SDLApp.hpp"
#include "sol/sol.hpp"

#include <memory>
#include <sol/forward.hpp>
#include <string>

namespace swgtk::tests 
{
	/*
		NOTE: If you experience rendering issues while building the ui, consider increasing this buffer size.
	*/

	constexpr uint64_t MaxVertexBuffer = 32uz * 1024uz;

	constexpr const int normalFontSize = 16;
	constexpr const int largeFontSize = 40;

	class Test_Suite
	{
	public:
		void Config(SDLApp& app);
		void LoadUI();
		void UpdateUI(swgtk::GameScene& app);
		void DrawUI(swgtk::GameScene& app);
		
		static nk_keys SDLKeytoNKKey(int key, uint16_t mods);
		static int SDLButtontoNKButton(uint8_t button);

	private:

		std::unique_ptr<swgtk::nk::NuklearUI> _ui;

		sol::state _lua;
	};

	class TestNone
	{
	public:
		explicit TestNone(Test_Suite* suite);
		
		SSC Create(GameScene& app);
		SSC Update(GameScene& app, float dt);

	private:
		Test_Suite* _parent;
	};
}
#endif // !SWGTK_TEST_SCENES_HPP
