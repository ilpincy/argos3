add_executable(test-range unit/test-range.cpp)
target_link_libraries(test-range argos3core_${ARGOS_BUILD_FOR})

add_library(test_footbot_controller MODULE
  experiment/test_footbot_controller.h
  experiment/test_footbot_controller.cpp)
