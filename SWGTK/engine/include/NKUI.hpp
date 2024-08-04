#ifndef SWGTK_NUKLEARUI_SYSTEM_HPP
#define SWGTK_NUKLEARUI_SYSTEM_HPP

#include "RenderWrap.hpp"
#include <filesystem>
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_STANDARD_IO

#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_UINT_DRAW_INDEX
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_FIXED_TYPES

#undef NK_IMPLEMENTATION
#include "nuklear.h"

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
	class RenderWrapper;
}

namespace swgtk::nk
{
	class NuklearUI
	{
	public:
		NuklearUI(SDLApp* app);
		NuklearUI(const NuklearUI&) = delete;
		NuklearUI(NuklearUI&&) = delete;

		NuklearUI& operator=(const NuklearUI&) = delete;
		NuklearUI& operator=(NuklearUI&&) = delete;

		~NuklearUI();

		void Compile(uint64_t vertexBufferSize, nk_font* initFont);
		void Update();
		void Draw(RenderWrapper* ren);
		[[nodiscard]] FontGroup& GetFontHandle();

		void Open(const std::string& name);
		void Close(const std::string& name);

		void InitLua();
		[[nodiscard]] LuaError LoadScript(const std::string& file);
		[[nodiscard]] LuaError LoadScriptsFromDirectory(const std::string& dir, bool recursive = false);
		[[nodiscard]] nk_context* Context(this NuklearUI& self);

	private:

		[[nodiscard]] LuaError CheckLuaPath(const std::filesystem::directory_entry& entry);

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

#endif // !SWGTK_NUKLEARUI_SYSTEM_HPP
