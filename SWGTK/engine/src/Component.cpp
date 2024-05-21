#include "Component.hpp"


namespace swgtk
{	
	angle::angle(float v)
		: value(v)
	{
	}
	
	position::position(float x, float y)
		: x(x), y(y)
	{
	}
	
	velocity::velocity(float x, float y)
		: x(x), y(y)
	{
	}

	acceleration::acceleration(float x, float y)
		: x(x), y(y)
	{
	}

	sprite::sprite(float x, float y, float w, float h)
		: x(x), y(y), w(w), h(h)
	{
	}
	
	bounding_box::bounding_box(float x, float y, float w, float h)
		: x(x), y(y), w(w), h(h)
	{
	}
	
	bounding_circle::bounding_circle(float x, float y, float r)
		: x(x), y(y), radius(r)
	{
	}
	
	area_box::area_box(float x, float y, float w, float h)
		: x(x), y(y), w(w), h(h)
	{
	}

	area_circle::area_circle(float x, float y, float r)
		: x(x), y(y), radius(r)
	{
	}
	
	fov::fov(float x, float y, float r)
		: x(x), y(y), radius(r)
	{
	}

	texture::texture(SDL_Texture* in_tex)
		: tex(in_tex)
	{
	}

}