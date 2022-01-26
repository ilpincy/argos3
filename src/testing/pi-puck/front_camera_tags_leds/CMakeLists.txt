# compile test loop functions
add_library(pipuck_front_camera_tags_leds_loop_functions MODULE
  loop_functions.h
  loop_functions.cpp)
target_link_libraries(pipuck_front_camera_tags_leds_loop_functions
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_pipuck)
# compile test controller
add_library(pipuck_front_camera_tags_leds_controller MODULE
  controller.h
  controller.cpp)
target_link_libraries(pipuck_front_camera_tags_leds_controller
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_pipuck)
# configure experiment
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/configuration.argos.in
  ${CMAKE_CURRENT_BINARY_DIR}/configuration.argos)
# define test
add_test(
   NAME pipuck_front_camera_tags_leds
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMAND argos3 -zc configuration.argos)
set_tests_properties(pipuck_front_camera_tags_leds
  PROPERTIES ENVIRONMENT "ARGOS_PLUGIN_PATH=${ARGOS_PLUGIN_PATH}")

