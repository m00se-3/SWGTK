include(${CMAKE_CURRENT_LIST_DIR}/text.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/particles.cmake)

add_test(NAME text_cpp COMMAND TextSample WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_test(NAME particles_cpp COMMAND ParticlesSample WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
