#
# Foot-bot headers
#
# argos3/plugins/robots/foot-bot/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_CONTROLINTERFACE
  control_interface/ci_footbot_accelerometer_sensor.h
  control_interface/ci_footbot_base_ground_sensor.h
  control_interface/ci_footbot_base_leds_actuator.h
  control_interface/ci_footbot_beacon_actuator.h
  control_interface/ci_footbot_ceiling_camera_sensor.h
  control_interface/ci_footbot_distance_scanner_actuator.h
  control_interface/ci_footbot_distance_scanner_sensor.h
  control_interface/ci_footbot_encoder_sensor.h
  control_interface/ci_footbot_gripper_actuator.h
  control_interface/ci_footbot_gripper_sensor.h
  control_interface/ci_footbot_gyroscopic_sensor.h
  control_interface/ci_footbot_leds_actuator.h
  control_interface/ci_footbot_light_sensor.h
  control_interface/ci_footbot_motor_ground_sensor.h
  control_interface/ci_footbot_omnidirectional_camera_sensor.h
  control_interface/ci_footbot_proximity_sensor.h
  control_interface/ci_footbot_rfid_sensor.h
  control_interface/ci_footbot_turret_actuator.h
  control_interface/ci_footbot_turret_encoder_sensor.h
  control_interface/ci_footbot_turret_torque_sensor.h
  control_interface/ci_footbot_wheel_speed_sensor.h
  control_interface/ci_footbot_wheels_actuator.h
  control_interface/ci_range_and_bearing_actuator.h
  control_interface/ci_range_and_bearing_sensor.h)

# argos3/plugins/robots/foot-bot/simulator
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR
    simulator/footbot_entity.h
    simulator/distance_scanner_equipped_entity.h
    simulator/dynamics2d_footbot_entity.h)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Foot-bot sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_CONTROLINTERFACE}
  control_interface/ci_footbot_base_ground_sensor.cpp
  control_interface/ci_footbot_base_leds_actuator.cpp
  control_interface/ci_footbot_gripper_actuator.cpp
  control_interface/ci_footbot_leds_actuator.cpp
  control_interface/ci_footbot_light_sensor.cpp
  control_interface/ci_footbot_motor_ground_sensor.cpp
  control_interface/ci_footbot_proximity_sensor.cpp
  control_interface/ci_footbot_turret_actuator.cpp
  control_interface/ci_footbot_turret_encoder_sensor.cpp
  control_interface/ci_footbot_turret_torque_sensor.cpp
  control_interface/ci_footbot_wheels_actuator.cpp)

if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR}
    simulator/footbot_entity.cpp
    simulator/distance_scanner_equipped_entity.cpp
    simulator/dynamics2d_footbot_entity.cpp)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Compile the graphical visualization only if the necessary libraries have been found
#
if(ARGOS_BUILD_FOR_SIMULATOR)
  include(ARGoSCheckQTOpenGL)
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR
      ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR}
      qtopengl_footbot.h)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR
      ${ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR}
      qtopengl_footbot.cpp)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create foot-bot simulator plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_footbot MODULE ${ARGOS3_SOURCES_PLUGINS_ROBOTS_FOOTBOT})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_footbot argos3plugin_${ARGOS_BUILD_FOR}_dynamics2d)
if(ARGOS_COMPILE_QTOPENGL)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_footbot argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
endif(ARGOS_COMPILE_QTOPENGL)

install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/foot-bot/control_interface)

if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_FOOTBOT_SIMULATOR}        DESTINATION include/argos3/plugins/robots/foot-bot/simulator)
endif(ARGOS_BUILD_FOR_SIMULATOR)

install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_footbot
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
