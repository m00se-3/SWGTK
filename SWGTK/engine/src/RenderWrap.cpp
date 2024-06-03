#include "RenderWrap.hpp"

#include "SDL2/SDL_image.h"
#include <filesystem>

namespace swgtk
{
	gsl::owner<SDL_Texture*> RenderWrapper::LoadTextureImg(const std::string_view& img, SDL_BlendMode blendMode)
	{
	    if(std::filesystem::exists(img))
	    {
			auto* texture = IMG_LoadTexture(_render, img.data());
			SDL_SetTextureBlendMode(texture, blendMode);
		
			return texture;
	    }

	    return nullptr;
	}
}
