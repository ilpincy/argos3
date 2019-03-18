#
# Prototype headers
#
# argos3/plugins/robots/prototype/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_PROTOTYPE_CONTROLINTERFACE
  control_interface/ci_prototype_joints_sensor.h
  control_interface/ci_prototype_joints_actuator.h)
# argos3/plugins/robots/prototype/simulator 
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_PROTOTYPE_SIMULATOR
    simulator/prototype_joints_default_actuator.h
    simulator/prototype_entity.h
    simulator/prototype_joint_entity.h
    simulator/prototype_joint_equipped_entity.h
    simulator/prototype_link_entity.h
    simulator/prototype_link_equipped_entity.h
    simulator/dynamics3d_prototype_model.h
    simulator/prototype_joints_default_sensor.h)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Prototype sources
#
# argos3/plugins/robots/prototype/control_interface
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_PROTOTYPE
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_PROTOTYPE_CONTROLINTERFACE}
  control_interface/ci_prototype_joints_actuator.cpp
  control_interface/ci_prototype_joints_sensor.cpp)
# argos3/plugins/robots/prototype/simulator 
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_PROTOTYPE
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_PROTOTYPE}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_PROTOTYPE_SIMULATOR}
    simulator/dynamics3d_prototype_model.cpp
    simulator/prototype_entity.cpp
    simulator/prototype_joint_entity.cpp
    simulator/prototype_joint_equipped_entity.cpp
    simulator/prototype_joints_default_actuator.cpp
    simulator/prototype_joints_default_sensor.cpp
    simulator/prototype_link_entity.cpp
    simulator/prototype_link_equipped_entity.cpp)
  # Compile the graphical visualization only if the necessary libraries have been found
  include(ARGoSCheckQTOpenGL)
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_PROTOTYPE_SIMULATOR
      ${ARGOS3_HEADERS_PLUGINS_ROBOTS_PROTOTYPE_SIMULATOR}
      simulator/qtopengl_prototype.h)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_PROTOTYPE
      ${ARGOS3_SOURCES_PLUGINS_ROBOTS_PROTOTYPE}
      simulator/qtopengl_prototype.cpp)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create the prototype plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_prototype SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_PROTOTYPE})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_prototype
  argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
  argos3plugin_${ARGOS_BUILD_FOR}_dynamics3d)
if(ARGOS_COMPILE_QTOPENGL)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_prototype argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
endif(ARGOS_COMPILE_QTOPENGL)

install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_PROTOTYPE_CONTROLINTERFACE}
        DESTINATION include/argos3/plugins/robots/prototype/control_interface)

if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_PROTOTYPE_SIMULATOR}
          DESTINATION include/argos3/plugins/robots/prototype/simulator)
endif(ARGOS_BUILD_FOR_SIMULATOR)

install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_prototype
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
