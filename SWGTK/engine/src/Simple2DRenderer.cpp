#include "swgtk/Simple2DRenderer.hpp"

#include "SDL3_image/SDL_image.h"
#include "swgtk/RendererBase.hpp"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <filesystem>
#include <sol/optional_implementation.hpp>

namespace swgtk
{
	Simple2DRenderer::~Simple2DRenderer() { DestroyDevice(); }

	bool Simple2DRenderer::PrepareDevice(SDL_Window* window, SDL_FColor bgColor) {
		_render = SDL_CreateRenderer(window, nullptr);
		
		if(_render != nullptr) {
			_textEngine = TTF_CreateRendererTextEngine(_render);

			if(_textEngine != nullptr) {
				return SetDrawColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
			}
		}

		return false;
	}

	void Simple2DRenderer::DestroyDevice() {
		TTF_DestroyRendererTextEngine(_textEngine);
		SDL_DestroyRenderer(_render);
	}
	
	Texture Simple2DRenderer::LoadTextureImg(const std::filesystem::path& img, SDL_BlendMode blendMode)	{
	    if(std::filesystem::exists(img)) {
			auto* texture = IMG_LoadTexture(_render, img.string().c_str());
			SDL_SetTextureBlendMode(texture, blendMode);
		
			return Texture{ texture };
	    }

	    return Texture{};
	}

	void Simple2DRenderer::InitLua(sol::state& lua) {
		
		auto vertex = lua.new_usertype<SDL_Vertex>(
		"Vertex2D", "position", &SDL_Vertex::position, "color", &SDL_Vertex::color, "tex_coord", &SDL_Vertex::tex_coord
		);

		vertex["new"] = [](sol::optional<SDL_FPoint> pos, sol::optional<SDL_FColor> col, sol::optional<SDL_FPoint> tex) -> SDL_Vertex {
				return SDL_Vertex{ 
					.position=pos.value_or(SDL_FPoint{}), 
					.color=col.value_or(SDL_FColor{}), 
					.tex_coord=tex.value_or(SDL_FPoint{}) };
			};

		lua["SetDrawColor"] = [this](sol::optional<float> r, sol::optional<float> g, sol::optional<float> b, sol::optional<float> a) {
				return SetDrawColor(
					r.value_or(RendererBase::defaultAlphaFloat), 
					g.value_or(RendererBase::defaultAlphaFloat), 
					b.value_or(RendererBase::defaultAlphaFloat), 
					a.value_or(RendererBase::defaultAlphaFloat)
					);
			};

		lua["DrawTexture"] = [this](SDL_Texture* texture, sol::optional<SDL_FRect> src, sol::optional<SDL_FRect> dest, sol::optional<double> angle, sol::optional<SDL_FPoint> center, sol::optional<SDL_FlipMode> flip) {
				DrawTexture(texture, (src) ? &src.value() : nullptr, (dest) ? &dest.value() : nullptr,
									angle.value_or(0.0), (center) ? &center.value() : nullptr, flip.value_or(SDL_FLIP_NONE));
			};
	}
}
