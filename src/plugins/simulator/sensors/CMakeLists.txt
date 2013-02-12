#
# Headers
#
# argos3/plugins/simulator/sensors
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_SENSORS
  proximity_default_sensor.h
  ring_proximity_default_sensor.h)

#
# Source files
#
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_SENSORS
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_SENSORS}
  proximity_default_sensor.cpp
  ring_proximity_default_sensor.cpp)

#
# Create entity plugin library
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_sensors SHARED ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_SENSORS})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_sensors
  argos3plugin_${ARGOS_BUILD_FOR}_entities
  argos3plugin_${ARGOS_BUILD_FOR}_genericrobot)

#
# Core library installation
#
install(FILES ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_SENSORS} DESTINATION include/argos3/plugins/simulator/sensors/)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_sensors
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
