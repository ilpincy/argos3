#
# Headers
#
# argos3/plugins/simulator/actuators
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_ACTUATORS
  differential_steering_default_actuator.h
  gripper_default_actuator.h
  leds_default_actuator.h)

#
# Source files
#
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_ACTUATORS
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_ACTUATORS}
  differential_steering_default_actuator.cpp
  gripper_default_actuator.cpp
  leds_default_actuator.cpp)

#
# Create entity plugin library
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_actuators SHARED ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_ACTUATORS})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_actuators
  argos3plugin_${ARGOS_BUILD_FOR}_entities
  argos3plugin_${ARGOS_BUILD_FOR}_genericrobot)

#
# Core library installation
#
install(FILES ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_ACTUATORS} DESTINATION include/argos3/plugins/simulator/actuators/)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_actuators
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
