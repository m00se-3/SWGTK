#ifndef SWGTK_COMPONENT_HPP
#define SWGTK_COMPONENT_HPP

extern "C"
{
	struct SDL_Texture;
}

namespace swgtk
{
	template<typename T>
	struct speed
	{
		T value{};

		speed() = default;
		speed(T v) : value(v) {}
	};

	template<typename T>
	struct health
	{
		T value{};

		health() = default;
		health(T v) : value(v) {}
	};

	template<typename T>
	struct score
	{
		T value{};
		
		score() = default;
		score(T v) : value(v) {}
	};

	template<typename T>
	struct basic_value
	{
		T value{};

		basic_value() = default;
		basic_value(T v) : value(v) {}
	};

	template<typename TFirst, typename TSecond>
	struct basic_pair
	{
		TFirst first{};
		TSecond second{};

		basic_pair() = default;
		basic_pair(TFirst f, TSecond s) : first(f), second(s) {}
	};

	struct angle
	{
		float value = 0.f;

		angle() = default;
		angle(float v);
	};
	
	struct position
	{
		float x = 0.f, y = 0.f;

		position() = default;
		position(float x, float y);
	};

	struct velocity
	{
		float x = 0.f, y = 0.f;

		velocity() = default;
		velocity(float x, float y);
	};

	struct acceleration
	{
		float x = 0.f, y = 0.f;

		acceleration() = default;
		acceleration(float x, float y);
	};

	struct sprite
	{
		float x = 0.f, y = 0.f,
			w = 0.f, h = 0.f;

		sprite() = default;
		sprite(float x, float y, float w, float h);
	};

	struct bounding_box
	{
		float x = 0.f, y = 0.f,
			w = 0.f, h = 0.f;

		bounding_box() = default;
		bounding_box(float x, float y, float w, float h);
	};

	struct bounding_circle
	{
		float x = 0.f, y = 0.f, radius = 0.f;

		bounding_circle() = default;
		bounding_circle(float x, float y, float r);
	};

	struct area_box
	{
		float x = 0.f, y = 0.f,
			w = 0.f, h = 0.f;

		area_box() = default;
		area_box(float x, float y, float w, float h);
	};

	struct area_circle
	{
		float x = 0.f, y = 0.f, radius = 0.f;

		area_circle() = default;
		area_circle(float x, float y, float r);
	};

	struct fov
	{
		float x = 0.f, y = 0.f, radius = 0.f;

		fov() = default;
		fov(float x, float y, float r);
	};

	struct texture
	{
		SDL_Texture* tex = nullptr;

		texture() = default;
		texture(SDL_Texture* in_tex);
	};

}

#endif // !SWGTK_COMPONENT_HPP
