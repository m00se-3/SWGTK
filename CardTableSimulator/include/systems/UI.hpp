#ifndef CTS_UI_SYSTEM_HPP
#define CTS_UI_SYSTEM_HPP

#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO

#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_UINT_DRAW_INDEX
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_FIXED_TYPES

#undef NK_IMPLEMENTATION
#include "nuklear/nuklear.h"

#include <string>
#include <vector>

#include "sol/sol.hpp"

#include "Texture.hpp"
#include "ErrCodes.hpp"
#include "Font.hpp"

namespace cts
{
	class SDLApp;
	
	class UI
	{
	public:
		UI(SDLApp* app, const std::string& fontsDir);
		~UI();

		void Update();
		void Compile();
		void Draw();
		void InitLua();

		LuaError LoadScriptsFromDirectory(const std::string& dir, bool recursive = false);

	private:

		LuaError LoadScript(const std::string& file);

		sol::state _lua;
		sol::table _dataTable;
		SDL_Texture* _fontTexture = nullptr, * _whiteTexture = nullptr;
		SDLApp* _parent = nullptr;

		nk_context* _ctx = nullptr;
		struct nk_convert_config _configurator;
		struct nk_buffer _cmds, _verts, _inds;
		struct nk_draw_null_texture _nullTexture;

		std::unique_ptr<SDL_Vertex[]> _buffer;
		std::unique_ptr<int[]> _elements;
	};
}

#endif // !CTS_UI_SYSTEM_HPP
