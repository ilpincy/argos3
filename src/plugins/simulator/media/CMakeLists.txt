#
# Headers
#
# argos3/plugins/simulator/media/
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_MEDIA
  rab_medium.h)

#
# Source files
#
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_MEDIA
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_MEDIA}
  rab_medium.cpp)

#
# Create entity plugin library
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_media SHARED ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_MEDIA})

#
# Core library installation
#
install(FILES ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_MEDIA} DESTINATION include/argos3/plugins/simulator/media/)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_media
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
