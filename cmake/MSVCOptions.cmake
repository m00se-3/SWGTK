# Enable compiler flags for certain build configurations
if(NOT ${CMAKE_BUILD_TYPE} MATCHES Debug)
    list(
        APPEND CompilerFlags 
        "/O2"
    )
endif()

# Enable compiler flags for all builds
list(
	APPEND CompilerFlags 

	"/Wall"
	"/permissive-"
    "/sdl"    
    "/QSpectre"
    "/QSpectre-load"
)

list(
    APPEND LinkerFlags

    "/guard:cf"
    "/guard:ehcont"
    "/DYNAMICBASE"
    "/HIGHENTROPYVA"
)

# Enable sanitizers if chosen

if(${USE_ADDR_SANITIZER} MATCHES ON)
    list(
        APPEND CompilerFlags

        "/fsanitize=address"
    )
endif()