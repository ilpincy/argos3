#
# Eye-bot headers
#
# argos3/plugins/robots/eye-bot/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_EYEBOT_CONTROLINTERFACE
  control_interface/ci_eyebot_light_sensor.h
  control_interface/ci_eyebot_proximity_sensor.h
)
# argos3/plugins/robots/eye-bot/simulator
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_EYEBOT_SIMULATOR
    simulator/eyebot_entity.h
    simulator/eyebot_light_rotzonly_sensor.h
    simulator/eyebot_proximity_default_sensor.h
    simulator/pointmass3d_eyebot_model.h)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Eye-bot sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_EYEBOT
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EYEBOT_CONTROLINTERFACE}
  control_interface/ci_eyebot_light_sensor.cpp
  control_interface/ci_eyebot_proximity_sensor.cpp
)

if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_EYEBOT
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_EYEBOT}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EYEBOT_SIMULATOR}
    simulator/eyebot_entity.cpp
    simulator/eyebot_light_rotzonly_sensor.cpp
    simulator/eyebot_proximity_default_sensor.cpp
    simulator/pointmass3d_eyebot_model.cpp)
  # Compile the graphical visualization only if the necessary libraries have been found
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_EYEBOT_SIMULATOR
      ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EYEBOT_SIMULATOR}
      simulator/qtopengl_eyebot.h)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_EYEBOT
      ${ARGOS3_SOURCES_PLUGINS_ROBOTS_EYEBOT}
      simulator/qtopengl_eyebot.h
      simulator/qtopengl_eyebot.cpp)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create eye-bot plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_eyebot SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_EYEBOT})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_eyebot
  argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
  argos3plugin_${ARGOS_BUILD_FOR}_pointmass3d)
if(ARGOS_COMPILE_QTOPENGL)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_eyebot argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
endif(ARGOS_COMPILE_QTOPENGL)

install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EYEBOT_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/eye-bot/control_interface)

if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EYEBOT_SIMULATOR}      DESTINATION include/argos3/plugins/robots/eye-bot/simulator)
endif(ARGOS_BUILD_FOR_SIMULATOR)

install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_eyebot
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
