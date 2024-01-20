set(LuaRoot ${CMAKE_CURRENT_LIST_DIR}/../lua CACHE FILEPATH "Path to Lua source directory.")

if(LuaRoot)
message("-- Lua found!!!")

set(CMAKE_STATIC_LIBRARY_PREFIX "")

# Lua does not include it's own CMakeLists.txt file or targets.
# So we must provide them and compile/link ourselves.

# Because we are embedding Lua into a C++ application, we only need to compile the library API.

add_library(lua_library)

# Set binary directories for each target.

set_target_properties(
	lua_library

	PROPERTIES 

	LIBRARY_OUTPUT_DIRECTORY ${BINARY_DIR}/external/lua/lib/ 
	OUTPUT_NAME liblua
)

# Include directories for each library

target_include_directories(lua_library PUBLIC ${LuaRoot}/src)

# Set Lua Library sources.


target_sources(
	lua_library

	PRIVATE 

	${LuaRoot}/src/lapi.c
	${LuaRoot}/src/lcode.c
	${LuaRoot}/src/lctype.c
	${LuaRoot}/src/ldebug.c
	${LuaRoot}/src/ldo.c
	${LuaRoot}/src/ldump.c
	${LuaRoot}/src/lfunc.c
	${LuaRoot}/src/lgc.c
	${LuaRoot}/src/llex.c
	${LuaRoot}/src/lmem.c
	${LuaRoot}/src/lobject.c
	${LuaRoot}/src/lopcodes.c
	${LuaRoot}/src/lparser.c
	${LuaRoot}/src/lstate.c
	${LuaRoot}/src/lstring.c
	${LuaRoot}/src/ltable.c
	${LuaRoot}/src/ltm.c
	${LuaRoot}/src/lundump.c
	${LuaRoot}/src/lvm.c
	${LuaRoot}/src/lzio.c
	${LuaRoot}/src/lauxlib.c
	${LuaRoot}/src/lbaselib.c
	${LuaRoot}/src/lcorolib.c
	${LuaRoot}/src/ldblib.c
	${LuaRoot}/src/liolib.c
	${LuaRoot}/src/lmathlib.c
	${LuaRoot}/src/loadlib.c
	${LuaRoot}/src/loslib.c
	${LuaRoot}/src/lstrlib.c
	${LuaRoot}/src/ltablib.c
	${LuaRoot}/src/lutf8lib.c
	${LuaRoot}/src/linit.c
)

separate_arguments(MemFlag UNIX_COMMAND PROGRAM $ENV{MEM_PARAMS})

target_compile_options(lua_library PRIVATE ${MemFlag})


# Finally, set the Cmake variables for linking to the project

set(LUA_INCLUDE_DIR ${LuaRoot}/src)
list(APPEND LUA_LIBRARIES lua_library)

endif()

unset(LuaRoot)