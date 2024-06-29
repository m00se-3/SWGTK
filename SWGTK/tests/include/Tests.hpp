#ifndef SWGTK_TEST_SCENES_HPP
#define SWGTK_TEST_SCENES_HPP

#include "Scene.hpp"
#include "UI.hpp"
#include "SDLApp.hpp"

#include <memory>
#include <sol/forward.hpp>
#include <string>

namespace swgtk::tests 
{

	class Test_Suite
	{
	public:
		void Config(SDLApp& app);
		void UpdateUI(swgtk::GameScene& app);
		void DrawUI(swgtk::GameScene& app);
		
		static nk_keys SDLKeytoNKKey(int key, uint16_t mods);
        static int SDLButtontoNKButton(uint8_t button);

	private:

		std::string _config = SWGTK_CONFIG;
		std::string _assets = SWGTK_ASSETS;
		std::unique_ptr<UI> _ui;

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
