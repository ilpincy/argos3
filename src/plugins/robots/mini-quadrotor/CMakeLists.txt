#
# Mini-quadrotor headers
#
# argos3/plugins/robots/mini-quadrotor/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_MINIQUADROTOR_CONTROLINTERFACE
  control_interface/ci_miniquadrotor_rotor_actuator.h
  control_interface/ci_miniquadrotor_trajectory_actuator.h)
# argos3/plugins/robots/mini-quadrotor/simulator
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_MINIQUADROTOR_SIMULATOR
    simulator/miniquadrotor_entity.h
    simulator/physx_miniquadrotor_model.h
    simulator/miniquadrotor_rotor_default_actuator.h
    )
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_MINIQUADROTOR_SIMULATOR
      ${ARGOS3_HEADERS_PLUGINS_ROBOTS_MINIQUADROTOR_SIMULATOR}
      simulator/qtopengl_miniquadrotor.h)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Mini-quadrotor sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_MINIQUADROTOR
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_MINIQUADROTOR_CONTROLINTERFACE}
  control_interface/ci_miniquadrotor_rotor_actuator.cpp
  control_interface/ci_miniquadrotor_trajectory_actuator.cpp)
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_MINIQUADROTOR
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_MINIQUADROTOR}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_MINIQUADROTOR_SIMULATOR}
    simulator/miniquadrotor_entity.cpp
    simulator/physx_miniquadrotor_model.cpp
    simulator/miniquadrotor_rotor_default_actuator.cpp)
  # Compile the graphical visualization only if the necessary libraries have been found
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_MINIQUADROTOR
      ${ARGOS3_SOURCES_PLUGINS_ROBOTS_MINIQUADROTOR}
      simulator/qtopengl_miniquadrotor.cpp)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create mini-quadrotor plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_miniquadrotor SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_MINIQUADROTOR})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_miniquadrotor
  argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
  argos3plugin_${ARGOS_BUILD_FOR}_actuators
  argos3plugin_${ARGOS_BUILD_FOR}_sensors)
if(ARGOS_BUILD_FOR_SIMULATOR)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_miniquadrotor argos3plugin_${ARGOS_BUILD_FOR}_physx)
  if(ARGOS_COMPILE_QTOPENGL)
    target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_miniquadrotor argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_MINIQUADROTOR_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/mini-quadrotor/control_interface)
if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_MINIQUADROTOR_SIMULATOR}      DESTINATION include/argos3/plugins/robots/mini-quadrotor/simulator)
endif(ARGOS_BUILD_FOR_SIMULATOR)

install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_miniquadrotor
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
