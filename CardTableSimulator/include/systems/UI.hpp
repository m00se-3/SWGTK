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

namespace cts
{
	class SDLApp;
	
	class UI
	{
	public:
		UI(SDLApp* app, const std::string& fontsDir);

		void Compile();
		void Draw();
		void InitLua();

	private:
		sol::state _lua;
		sol::table _dataTable;
		Texture _fontTexture, _whiteTexture;
		SDLApp* _parent = nullptr;

		nk_context* _ctx = nullptr;
		struct nk_convert_config _configurator;
		struct nk_buffer _cmds, _verts, _inds;
		struct nk_draw_null_texture _nullTexture;

		std::vector<SDL_Vertex> _buffer;
		std::vector<int> _elements;
	};
}

#endif // !CTS_UI_SYSTEM_HPP
