# Lua-based tests are created here simply by invoking the swl executable with the
# appropriate lua startup file.
add_test(NAME text_test_lua COMMAND swl2d ${CMAKE_CURRENT_LIST_DIR}/TextTest.lua)
add_test(NAME multi_text_lua COMMAND swl2d ${CMAKE_CURRENT_LIST_DIR}/MultiTestRoot.lua WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
