#
# Foot-bot headers
#
# argos3/plugins/robots/foot-bot/simulator
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR
  footbot_entity.h
  distance_scanner_equipped_entity.h
  dynamics2d_footbot_entity.h)

#
# Foot-bot sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR}
  footbot_entity.cpp
  distance_scanner_equipped_entity.cpp
  dynamics2d_footbot_entity.cpp)

#
# Compile the graphical visualization only if the necessary libraries have been found
#
include(ARGoSCheckQTOpenGL)
if(ARGOS_COMPILE_QTOPENGL)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR}
    qtopengl_footbot.h)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR}
    qtopengl_footbot.cpp)
endif(ARGOS_COMPILE_QTOPENGL)

#
# Create foot-bot simulator plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_footbot MODULE
  ${ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_footbot argos3plugin_${ARGOS_BUILD_FOR}_dynamics2d)
if(ARGOS_COMPILE_QTOPENGL)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_footbot argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
endif(ARGOS_COMPILE_QTOPENGL)

install(
  FILES
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR}
  DESTINATION
  include/argos3/plugins/robots/foot-bot/simulator)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_footbot
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
