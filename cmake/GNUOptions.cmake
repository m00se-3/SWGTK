# Enable compiler flags for certain build configurations
if($<CONFIG:Debug>)
  list(
    APPEND CompilerFlags
    "-g"
    "-D_GLIBCXX_ASSERTIONS"
  )
elseif($<CONFIG:RelWithDebInfo>)
  list(
    APPEND CompilerFlags
    "-g"
    "-O2"
    "-U_FORTIFY_SOURCE"
    "-D_FORTIFY_SOURCE=3"
    "-D_GLIBCXX_ASSERTIONS"
  )
else()
  list(
    APPEND CompilerFlags
    "-O3"
    "-U_FORTIFY_SOURCE"
    "-D_FORTIFY_SOURCE=2"
  )
endif()

if(${SWGTK_EXCEPTIONS} MATCHES ON)
  list(APPEND CompilerFlags "-fexceptions")
else()
  list(APPEND CompilerFlags "-fno-exceptions")
endif()

# Enable special flags when using GCC
if($<CXX_COMPILER_ID:GNU>)
  list(
    APPEND CompilerFlags

    "-fhardened"
    "-Wbidi-chars=any"
    "-Wtrampolines"
    "-Whardened"
  )
endif()

#Exclude unsupported flags on Windows

list(
  APPEND CompilerFlags

  "$<$<AND:$<NOT:$<PLATFORM_ID:Windows>>,$<CXX_COMPILER_ID:Clang>>:-mbranch-protection=standard>"
)

list(
  APPEND LinkerFlags
  "$<$<NOT:$<PLATFORM_ID:Windows>>:-fPIC;-fPIE>"
)


# Enable flags for all builds
list(
  APPEND CompilerFlags

  "-Wall"
  "-Wextra"
  "-Werror"
  "-Wformat"
  "-Wformat=2"
  "-Wimplicit-fallthrough"
  "-Wreturn-local-addr"
  "-Wpedantic"
  "-Wdouble-promotion"
  "-Wconversion"
  "-Wsign-conversion"
  "-Wsuggest-override"

  "-fvisibility=hidden"
  "-fstrict-flex-arrays=3"
  "-fno-strict-overflow"
  "-fno-strict-aliasing"
  "-ftrivial-auto-var-init=zero"
)

if(NOT DEFINED EMSCRIPTEN)
  list(APPEND CompilerFlags "-fcf-protection=full")

  list(
    APPEND LinkerFlags

    "-Wl,-z,nodlopen"
    "-Wl,-z,noexecstack"
    "-Wl,-z,relro"
    "-Wl,--as-needed"
    "-Wl,--no-copy-dt-needed-entries"
  )
endif()

list(
  APPEND LinkerFlags

  "-pie"
)
