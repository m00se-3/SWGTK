if(${SWGTK_BUILD_EXAMPLES} MATCHES ON)
    set(SWGTK_SHARED_LIBARIES)

    if($<TARGET_EXISTS:SDL3::SDL3-shared>)
        list(APPEND SWGTK_SHARED_LIBARIES $<TARGET_SONAME_FILE:SDL3::SDL3-shared>)
    endif()

    if($<TARGET_EXISTS:SDL3_image::SDL3_image-shared>)
        list(APPEND SWGTK_SHARED_LIBARIES $<TARGET_SONAME_FILE:SDL3_image::SDL3_image-shared>)
    endif()


    if($<TARGET_EXISTS:SDL3_ttf::SDL3_ttf-shared>)
        list(APPEND SWGTK_SHARED_LIBARIES $<TARGET_SONAME_FILE:SDL3_ttf::SDL3_ttf-shared>)
    endif()


    if($<TARGET_EXISTS:freetype-shared>)
        list(APPEND SWGTK_SHARED_LIBARIES $<TARGET_SONAME_FILE:freetype-shared>)
    endif()


    include(${CMAKE_CURRENT_LIST_DIR}/text.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/particles.cmake)

    add_test(NAME text_cpp COMMAND TextSample)
    add_test(NAME particles_cpp COMMAND ParticlesSample)

endif()