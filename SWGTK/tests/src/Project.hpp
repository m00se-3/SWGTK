#ifndef SWGTK_PROJECT_HPP
#define SWGTK_PROJECT_HPP

#include <string>

namespace swgtk::project
{
	[[nodiscard]] constexpr std::string AssetsDir() { return SWGTK_ASSETS; }

	[[nodiscard]] constexpr std::string ConfigDir() { return SWGTK_CONFIG; }

}

#endif // !SWGTK_PROJECT_HPP
