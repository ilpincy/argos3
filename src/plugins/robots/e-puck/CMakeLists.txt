#
# E-puck headers
#
# argos3/plugins/robots/e-puck/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_CONTROLINTERFACE
  control_interface/ci_epuck_proximity_sensor.h)
# argos3/plugins/robots/e-puck/simulator
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_SIMULATOR
    simulator/dynamics2d_epuck_model.h
    simulator/dynamics3d_epuck_model.h
    # simulator/physx_epuck_model.h
    simulator/epuck_entity.h
    simulator/epuck_proximity_default_sensor.h)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# E-puck sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_CONTROLINTERFACE}
  control_interface/ci_epuck_proximity_sensor.cpp)
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_SIMULATOR}
    simulator/dynamics3d_epuck_model.cpp
    simulator/dynamics2d_epuck_model.cpp
    # simulator/physx_epuck_model.cpp
    simulator/epuck_entity.cpp
    simulator/epuck_proximity_default_sensor.cpp)
  # Compile the graphical visualization only if the necessary libraries have been found
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_SIMULATOR
      ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_SIMULATOR}
      simulator/qtopengl_epuck.h)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK
      ${ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK}
      simulator/qtopengl_epuck.h
      simulator/qtopengl_epuck.cpp)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create e-puck plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_epuck SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_EPUCK})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_epuck
  argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
  argos3plugin_${ARGOS_BUILD_FOR}_dynamics2d
  argos3plugin_${ARGOS_BUILD_FOR}_dynamics3d)
  # argos3plugin_${ARGOS_BUILD_FOR}_physx)
if(ARGOS_BUILD_FOR_SIMULATOR AND ARGOS_COMPILE_QTOPENGL)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_epuck argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
endif(ARGOS_BUILD_FOR_SIMULATOR AND ARGOS_COMPILE_QTOPENGL)

install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/e-puck/control_interface)

if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_EPUCK_SIMULATOR}      DESTINATION include/argos3/plugins/robots/e-puck/simulator)
endif(ARGOS_BUILD_FOR_SIMULATOR)

install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_epuck
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
