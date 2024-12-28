#ifndef SWGTK_MACROS_HPP
#define SWGTK_MACROS_HPP

#ifdef _DEBUG
#include <cstdio>
#define DEBUG_PRINT(Debug_Message) [[maybe_unused]] auto _ = std::fputs(Debug_Message, stderr);

#else
#define DEBUG_PRINT(Debug_Message)
#endif

#endif