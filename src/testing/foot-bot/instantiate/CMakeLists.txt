add_library(footbot_instantiate_loop_functions MODULE
  loop_functions.h
  loop_functions.cpp)
add_library(footbot_instantiate_controller MODULE
  controller.h
  controller.cpp)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/configuration.argos.in
  ${CMAKE_CURRENT_BINARY_DIR}/configuration.argos)
add_test(
   NAME footbot_instantiate
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMAND argos3 -zc configuration.argos)
set_tests_properties(footbot_instantiate
  PROPERTIES ENVIRONMENT "ARGOS_PLUGIN_PATH=${ARGOS_PLUGIN_PATH}")

