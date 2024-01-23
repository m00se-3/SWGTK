#ifndef CTS_COMPONENT_HPP
#define CTS_COMPONENT_HPP

#include <vector>
#include <variant>

namespace cts
{
	
	struct vec2i
	{
		int x = 0, y = 0;

		vec2i() = default;
		vec2i(int x, int y);
	};

	struct vec2f
	{
		float x = 0.f, y = 0.f;

		vec2f() = default;
		vec2f(float x, float y);
	};

	struct vec4i
	{
		int x = 0, y = 0,
			w = 0, h = 0;

		vec4i() = default;
		vec4i(int x, int y, int w, int h);
	};

	struct vec4f
	{
		float x = 0.f, y = 0.f,
			w = 0.f, h = 0.f;

		vec4f() = default;
		vec4f(float x, float y, float w, float h);
	};

	class Component
	{
	public:
		using Type = std::variant<std::monostate, vec2i, vec2f, vec4i, vec4f>;

		Component(uint32_t size);


	private:
		std::vector<Type> _comps;
	};

}

#endif // !CTS_COMPONENT_HPP
