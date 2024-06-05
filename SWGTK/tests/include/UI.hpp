#ifndef SWGTK_UI_SYSTEM_HPP
#define SWGTK_UI_SYSTEM_HPP

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO

#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_UINT_DRAW_INDEX
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_FIXED_TYPES

#undef NK_IMPLEMENTATION
#include "nuklear/nuklear.h"

#include <string>
#include <memory>
#include <map>
#include <set>
#include <vector>

#include "sol/sol.hpp"

#include "Texture.hpp"
#include "ErrCodes.hpp"
#include "NKFont.hpp"

namespace swgtk
{
	class SDLApp;

	constexpr const int normalFontSize = 16;
	constexpr const int largeFontSize = 40;
	
	class UI
	{
	public:
		UI(SDLApp* app, const std::string& fontsDir);
		UI(const UI&) = delete;
		UI(UI&&) = delete;

		UI& operator=(const UI&) = delete;
		UI& operator=(UI&&) = delete;

		~UI();

		void Update();
		void Compile();
		void Draw();
		void InitLua();

		void Open(const std::string& name);
		void Close(const std::string& name);

		[[nodiscard]] LuaError LoadScriptsFromDirectory(const std::string& dir, bool recursive = false);
		[[nodiscard]] nk_context* Context(this UI& self);

	private:

		LuaError LoadScript(const std::string& file);

		sol::state _lua;
		sol::table _dataTable;
		SDL_Texture* _fontTexture = nullptr, * _whiteTexture = nullptr;
		SDLApp* _parent = nullptr;

		nk_context _ctx;
		nk::FontGroup _fonts;
		struct nk_convert_config _configurator;
		struct nk_buffer _cmds, _verts, _inds;
		struct nk_draw_null_texture _nullTexture;

		std::vector<SDL_Vertex> _buffer;
		std::vector<int> _elements;

		std::map<std::string, std::string> _luaFunctions;
		std::set<std::string> _openMenus;
	};
}

#endif // !SWGTK_UI_SYSTEM_HPP
