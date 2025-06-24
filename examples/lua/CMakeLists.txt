# Lua-based tests are created here simply by invoking the swl executable with the
# appropriate lua startup file.
add_test(NAME text_lua COMMAND swl2d --title "Lua Text Example" ${CMAKE_CURRENT_LIST_DIR}/Text.lua)
add_test(NAME multi_lua COMMAND swl2d --title "Lua Modules Example" ${CMAKE_CURRENT_LIST_DIR}/MultiRoot.lua WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
