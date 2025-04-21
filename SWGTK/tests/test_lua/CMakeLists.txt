# Lua-based tests are created here simply by invoking the swl executable with the
# appropriate lua startup file.
add_test(NAME text_test_lua COMMAND swl2d ${CMAKE_CURRENT_LIST_DIR}/TextTest.lua WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
