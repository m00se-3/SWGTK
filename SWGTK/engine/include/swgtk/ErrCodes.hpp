#ifndef SWGTK_ERRCODES_HPP
#define SWGTK_ERRCODES_HPP

namespace swgtk
{
	// Not sure if I'm keeping these yet.
	enum class LuaError
	{
		Ok,
		FileDir404,
		ParsingFailed,
		BadResult,
	};
}

#endif // !SWGTK_ERRCODES_HPP
