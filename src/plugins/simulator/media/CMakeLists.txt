#
# Headers
#
# argos3/plugins/simulator/media/
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_MEDIA
  directional_led_medium.h
  led_medium.h
  rab_medium.h
  simple_radio_medium.h
  tag_medium.h)

#
# Source files
#
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_MEDIA
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_MEDIA}
  directional_led_medium.cpp
  led_medium.cpp
  rab_medium.cpp
  simple_radio_medium.cpp
  tag_medium.cpp)

#
# Create entity plugin library
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_media SHARED ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_MEDIA})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_media argos3core_${ARGOS_BUILD_FOR})

#
# Add plugin to ARGOS_PLUGIN_PATH
#
set(ARGOS_PLUGIN_PATH "${ARGOS_PLUGIN_PATH}:${CMAKE_CURRENT_BINARY_DIR}" CACHE INTERNAL "ARGoS plugin path")

#
# Core library installation
#
install(FILES ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_MEDIA} DESTINATION include/argos3/plugins/simulator/media/)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_media
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
