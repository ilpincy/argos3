#
# Foot-bot headers
#
# argos3/plugins/robots/foot-bot/simulator
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR
  footbot_entity.h
  dynamics2d_footbot_entity.h
  qtopengl_footbot.h)

#
# Foot-bot sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR}
  footbot_entity.cpp
  dynamics2d_footbot_entity.cpp
  qtopengl_footbot.cpp)

#
# Create foot-bot simulator plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_footbot MODULE
  ${ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR})

install(
  FILES
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR}
  DESTINATION
  include/argos3/plugins/robots/foot-bot/simulator)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_footbot
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
