# compile test loop functions
add_library(builderbot_nfc_lua_loop_functions MODULE
  loop_functions.h
  loop_functions.cpp)
target_link_libraries(builderbot_nfc_lua_loop_functions
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_builderbot
    argos3plugin_${ARGOS_BUILD_FOR}_block)
# configure controllers
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/builderbot.lua
  ${CMAKE_CURRENT_BINARY_DIR}/builderbot.lua
  COPYONLY)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/block.lua
  ${CMAKE_CURRENT_BINARY_DIR}/block.lua
  COPYONLY)
# configure experiment
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/configuration.argos.in
  ${CMAKE_CURRENT_BINARY_DIR}/configuration.argos)
# define test
add_test(
   NAME builderbot_nfc_lua
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMAND argos3 -c configuration.argos)
set_tests_properties(builderbot_nfc_lua
  PROPERTIES ENVIRONMENT "ARGOS_PLUGIN_PATH=${ARGOS_PLUGIN_PATH}")

