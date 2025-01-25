#ifndef SWGTK_MACROS_HPP
#define SWGTK_MACROS_HPP

#ifdef _DEBUG
#include <fmt/format.h>
#define DEBUG_PRINT(Debug_Message, Debug_Error) fmt::println(Debug_Message, Debug_Error);
#define DEBUG_PRINT2(Debug_Message, Debug_Error_1, Debug_error_2) fmt::println(Debug_Message, Debug_Error_1, Debug_error_2);

#else
#define DEBUG_PRINT(Debug_Message, Debug_Error) 
#define DEBUG_PRINT2(Debug_Message, Debug_Error_1, Debug_error_2) 
#endif

#endif