# compile test loop functions
add_library(drone_fly_pointmass3d_loop_functions MODULE
  loop_functions.h
  loop_functions.cpp)
target_link_libraries(drone_fly_pointmass3d_loop_functions
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_drone)
# compile test controller
add_library(drone_fly_pointmass3d_controller MODULE
  controller.h
  controller.cpp)
target_link_libraries(drone_fly_pointmass3d_controller
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_drone)
# configure experiment
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/configuration.argos.in
  ${CMAKE_CURRENT_BINARY_DIR}/configuration.argos)
# define test
add_test(
   NAME drone_fly_pointmass3d
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMAND argos3 -zc configuration.argos)
set_tests_properties(drone_fly_pointmass3d
  PROPERTIES ENVIRONMENT "ARGOS_PLUGIN_PATH=${ARGOS_PLUGIN_PATH}")

