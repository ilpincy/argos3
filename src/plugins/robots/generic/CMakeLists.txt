#
# Generic robot headers
#
# argos3/plugins/robots/generic/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE
  control_interface/ci_differential_steering_actuator.h
  control_interface/ci_leds_actuator.h
  control_interface/ci_light_sensor.h
  control_interface/ci_proximity_sensor.h)

#
# Generic robot sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE}
  control_interface/ci_differential_steering_actuator.cpp
  control_interface/ci_leds_actuator.cpp
  control_interface/ci_light_sensor.cpp
  control_interface/ci_proximity_sensor.cpp)

#
# Create generic robot plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_genericrobot SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC})

#
# Install stuff
#
install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/generic/control_interface)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
