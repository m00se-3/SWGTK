if(${SWGTK_BUILD_EXAMPLES} MATCHES ON)
    include(${CMAKE_CURRENT_LIST_DIR}/text.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/particles.cmake)

    add_test(NAME text_cpp COMMAND TextSample)
    add_test(NAME particles_cpp COMMAND ParticlesSample)

endif()