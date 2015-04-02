#
# Foot-bot headers
#
# argos3/plugins/robots/foot-bot/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_CONTROLINTERFACE
  control_interface/ci_footbot_base_ground_sensor.h
  control_interface/ci_footbot_distance_scanner_actuator.h
  control_interface/ci_footbot_distance_scanner_sensor.h
  control_interface/ci_footbot_encoder_sensor.h
  control_interface/ci_footbot_gripper_actuator.h
  control_interface/ci_footbot_light_sensor.h
  control_interface/ci_footbot_motor_ground_sensor.h
  control_interface/ci_footbot_proximity_sensor.h
  control_interface/ci_footbot_turret_actuator.h
  control_interface/ci_footbot_turret_encoder_sensor.h)
# argos3/plugins/robots/foot-bot/simulator
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR
    simulator/dynamics2d_footbot_model.h
    # simulator/physx_footbot_model.h
    simulator/footbot_base_ground_rotzonly_sensor.h
    simulator/footbot_distance_scanner_default_actuator.h
    simulator/footbot_distance_scanner_equipped_entity.h
    simulator/footbot_distance_scanner_rotzonly_sensor.h
    simulator/footbot_entity.h
    simulator/footbot_gripper_default_actuator.h
    simulator/footbot_light_rotzonly_sensor.h
    simulator/footbot_motor_ground_rotzonly_sensor.h
    simulator/footbot_proximity_default_sensor.h
    simulator/footbot_turret_default_actuator.h
    simulator/footbot_turret_encoder_default_sensor.h
    simulator/footbot_turret_entity.h)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Foot-bot sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_CONTROLINTERFACE}
  control_interface/ci_footbot_base_ground_sensor.cpp
  control_interface/ci_footbot_distance_scanner_actuator.cpp
  control_interface/ci_footbot_distance_scanner_sensor.cpp
  control_interface/ci_footbot_encoder_sensor.cpp
  control_interface/ci_footbot_gripper_actuator.cpp
  control_interface/ci_footbot_light_sensor.cpp
  control_interface/ci_footbot_motor_ground_sensor.cpp
  control_interface/ci_footbot_proximity_sensor.cpp
  control_interface/ci_footbot_turret_actuator.cpp
  control_interface/ci_footbot_turret_encoder_sensor.cpp)

if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR}
    simulator/dynamics2d_footbot_model.cpp
    # simulator/physx_footbot_model.cpp
    simulator/footbot_base_ground_rotzonly_sensor.cpp
    simulator/footbot_distance_scanner_default_actuator.cpp
    simulator/footbot_distance_scanner_equipped_entity.cpp
    simulator/footbot_distance_scanner_rotzonly_sensor.cpp
    simulator/footbot_entity.cpp
    simulator/footbot_gripper_default_actuator.cpp
    simulator/footbot_light_rotzonly_sensor.cpp
    simulator/footbot_motor_ground_rotzonly_sensor.cpp
    simulator/footbot_proximity_default_sensor.cpp
    simulator/footbot_turret_default_actuator.cpp
    simulator/footbot_turret_encoder_default_sensor.cpp
    simulator/footbot_turret_entity.cpp)
  # Compile the graphical visualization only if the necessary libraries have been found
  include(ARGoSCheckQTOpenGL)
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR
      ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR}
      simulator/qtopengl_footbot.h)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT
      ${ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT}
      simulator/qtopengl_footbot.h
      simulator/qtopengl_footbot.cpp)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create foot-bot plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_footbot SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_footbot
  argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
  argos3plugin_${ARGOS_BUILD_FOR}_dynamics2d)
  # argos3plugin_${ARGOS_BUILD_FOR}_physx)
if(ARGOS_COMPILE_QTOPENGL)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_footbot argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
endif(ARGOS_COMPILE_QTOPENGL)

install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/foot-bot/control_interface)

if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR}      DESTINATION include/argos3/plugins/robots/foot-bot/simulator)
endif(ARGOS_BUILD_FOR_SIMULATOR)

install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_footbot
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
