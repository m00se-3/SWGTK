#ifndef CTS_SCENE_HPP
#define CTS_SCENE_HPP


extern "C"
{
	struct SDL_Renderer;
}

namespace cts
{

	class Scene
	{
	public:
		Scene(SDL_Renderer* ren);
		~Scene() = default;


	protected:


	private:
		SDL_Renderer* _renderer = nullptr;
	};

}

#endif // !CTS_SCENE_HPP
