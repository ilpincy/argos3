#
# Headers
#
# argos3/plugins/simulator/sensors
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_SENSORS
  differential_steering_default_sensor.h
  ground_rotzonly_sensor.h
  light_default_sensor.h
  positioning_default_sensor.h
  proximity_default_sensor.h
  range_and_bearing_medium_sensor.h)

#
# Source files
#
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_SENSORS
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_SENSORS}
  differential_steering_default_sensor.cpp
  ground_rotzonly_sensor.cpp
  light_default_sensor.cpp
  positioning_default_sensor.cpp
  proximity_default_sensor.cpp
  range_and_bearing_medium_sensor.cpp)

#
# Create entity plugin library
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_sensors SHARED ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_SENSORS})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_sensors
  argos3plugin_${ARGOS_BUILD_FOR}_entities
  argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
  argos3plugin_${ARGOS_BUILD_FOR}_media)

#
# Core library installation
#
install(FILES ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_SENSORS} DESTINATION include/argos3/plugins/simulator/sensors/)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_sensors
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
