include(${CMAKE_CURRENT_LIST_DIR}/text.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/particles.cmake)

add_test(NAME text_test COMMAND Test_TextSample WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
add_test(NAME test_particles COMMAND Test_Particles WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
