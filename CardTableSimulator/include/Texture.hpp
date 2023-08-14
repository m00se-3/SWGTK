#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <filesystem>

extern "C" {
	struct SDL_Texture;
}

/*
	A simple wrapper class for SDL_Texture
*/
class Texture {

	SDL_Texture* _texture = nullptr;

public:
	Texture() = default;
	Texture(const std::filesystem::path& filepath);
	~Texture();

};

#endif // !TEXTURE_HPP
