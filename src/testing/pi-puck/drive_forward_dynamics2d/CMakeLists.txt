# compile test loop functions
add_library(pipuck_drive_forward_dynamics2d_loop_functions MODULE
  loop_functions.h
  loop_functions.cpp)
target_link_libraries(pipuck_drive_forward_dynamics2d_loop_functions
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_pipuck)
# compile test controller
add_library(pipuck_drive_forward_dynamics2d_controller MODULE
  controller.h
  controller.cpp)
target_link_libraries(pipuck_drive_forward_dynamics2d_controller
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_pipuck)
# configure experiment
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/configuration.argos.in
  ${CMAKE_CURRENT_BINARY_DIR}/configuration.argos)
# define test
add_test(
   NAME pipuck_drive_forward_dynamics2d
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMAND argos3 -zc configuration.argos)
set_tests_properties(pipuck_drive_forward_dynamics2d
  PROPERTIES ENVIRONMENT "ARGOS_PLUGIN_PATH=${ARGOS_PLUGIN_PATH}")

