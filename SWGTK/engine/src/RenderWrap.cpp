#include "RenderWrap.hpp"

#include "SDL_image.h"
#include <filesystem>

namespace swgtk
{
	Texture RenderWrapper::LoadTextureImg(const std::string_view& img, SDL_BlendMode blendMode)
	{
	    if(std::filesystem::exists(img))
	    {
			auto* texture = IMG_LoadTexture(_render, img.data());
			SDL_SetTextureBlendMode(texture, blendMode);
		
			return Texture{ texture };
	    }

	    return Texture{};
	}

	void RenderWrapper::InitLua(sol::state& lua)
	{
		
		auto vertex = lua.new_usertype<SDL_Vertex>(
		"Vertex2D", "position", &SDL_Vertex::position, "color", &SDL_Vertex::color, "tex_coord", &SDL_Vertex::tex_coord
		);

		vertex["new"] = [](sol::optional<SDL_FPoint> pos, sol::optional<SDL_Color> col, sol::optional<SDL_FPoint> tex) -> SDL_Vertex
			{
				return SDL_Vertex{ *pos, *col, *tex };
			};

		/*
		   Begin binding  the rendering backend.
		*/

		lua["SetDrawColor"] = [this](uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255u)
			{
				SetDrawColor(r, g, b, a);
			};

		lua["DrawTexture"] = [this](SDL_Texture* texture, sol::optional<SDL_Rect> src, sol::optional<SDL_FRect> dest, sol::optional<double> angle, sol::optional<SDL_FPoint> center, sol::optional<SDL_RendererFlip> flip)
			{
				DrawTexture(gsl::make_not_null(texture), (src) ? &src.value() : nullptr, (dest) ? &dest.value() : nullptr,
									angle.value_or(0.0), (center) ? &center.value() : nullptr, flip.value_or(SDL_FLIP_NONE));
			};

		lua["DrawText"] = [this](std::string_view str, TTF_Font* font, sol::optional<SDL_Rect> dest)
			{
				DrawText(str, gsl::make_not_null(font), (dest) ? &dest.value() : nullptr);
			};
	}
}
