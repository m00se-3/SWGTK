#include "Component.hpp"


namespace cts
{
	vec2i::vec2i(int x, int y)
		: x(x), y(y)
	{
	}

	vec2f::vec2f(float x, float y)
		: x(x), y(y)
	{
	}

	vec4i::vec4i(int x, int y, int w, int h)
		: x(x), y(y), w(w), h(h)
	{
	}

	vec4f::vec4f(float x, float y, float w, float h)
		: x(x), y(y), w(w), h(h)
	{
	}

	Component::Component(uint32_t size)
	{
		_comps.reserve(size);
	}

}