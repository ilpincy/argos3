add_executable(test-range unit/test-range.cpp)
target_link_libraries(test-range argos3core_${ARGOS_BUILD_FOR})

# add_executable(test-reset unit/test-reset.cpp)
# target_link_libraries(test-reset argos3core_${ARGOS_BUILD_FOR})

add_library(test_footbot_controller MODULE
  experiment/test_footbot_controller.h
  experiment/test_footbot_controller.cpp)
target_link_libraries(test_footbot_controller
  argos3plugin_${ARGOS_BUILD_FOR}_footbot)

add_library(test_loop_functions MODULE
  experiment/test_loop_functions.h
  experiment/test_loop_functions.cpp)
target_link_libraries(test_footbot_controller
  argos3core_${ARGOS_BUILD_FOR})
