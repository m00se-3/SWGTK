#ifndef CTS_COMPONENT_HPP
#define CTS_COMPONENT_HPP

namespace cts
{
	struct speed
	{
		float value = 0.f;

		speed() = default;
		speed(float v);
	};

	struct health
	{
		float value = 0.f;

		health() = default;
		health(float v);
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

}

#endif // !CTS_COMPONENT_HPP
