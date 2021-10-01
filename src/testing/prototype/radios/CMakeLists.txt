# compile test loop functions
add_library(prototype_radios_loop_functions MODULE
  loop_functions.h
  loop_functions.cpp)
target_link_libraries(prototype_radios_loop_functions
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_prototype)
# compile test controller
add_library(prototype_radios_controller MODULE
  controller.h
  controller.cpp)
target_link_libraries(prototype_radios_controller
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_prototype)
# configure experiment
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/configuration.argos.in
  ${CMAKE_CURRENT_BINARY_DIR}/configuration.argos)
# define test
add_test(
   NAME prototype_radios
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMAND argos3 -zc configuration.argos)
set_tests_properties(prototype_radios
  PROPERTIES ENVIRONMENT "ARGOS_PLUGIN_PATH=${ARGOS_PLUGIN_PATH}")

