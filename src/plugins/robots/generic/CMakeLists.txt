#
# Generic robot headers
#
# argos3/plugins/robots/generic/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE
  control_interface/ci_altitude_sensor.h
  control_interface/ci_battery_sensor.h
  control_interface/ci_camera_sensor.h
  control_interface/ci_camera_sensor_algorithm.h
  control_interface/ci_camera_sensor_algorithms/ci_camera_sensor_directional_led_detector_algorithm.h
  control_interface/ci_camera_sensor_algorithms/ci_camera_sensor_led_detector_algorithm.h
  control_interface/ci_camera_sensor_algorithms/ci_camera_sensor_tag_detector_algorithm.h
  control_interface/ci_colored_blob_omnidirectional_camera_sensor.h
  control_interface/ci_colored_blob_perspective_camera_sensor.h
  control_interface/ci_differential_steering_actuator.h
  control_interface/ci_differential_steering_sensor.h
  control_interface/ci_directional_leds_actuator.h
  control_interface/ci_gripper_actuator.h
  control_interface/ci_ground_sensor.h
  control_interface/ci_leds_actuator.h
  control_interface/ci_light_sensor.h
  control_interface/ci_magnets_actuator.h
  control_interface/ci_pan_tilt_camera_actuator.h
  control_interface/ci_positioning_sensor.h
  control_interface/ci_proximity_sensor.h
  control_interface/ci_quadrotor_position_actuator.h
  control_interface/ci_quadrotor_speed_actuator.h
  control_interface/ci_radios_actuator.h
  control_interface/ci_radios_sensor.h
  control_interface/ci_range_and_bearing_actuator.h
  control_interface/ci_range_and_bearing_sensor.h
  control_interface/ci_tags_actuator.h)

if(ARGOS_BUILD_FOR_SIMULATOR)
  # argos3/plugins/robots/generic/simulator
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_SIMULATOR
    simulator/battery_default_sensor.h
    simulator/camera_default_sensor.h
    simulator/camera_sensor_algorithm.h
    simulator/camera_sensor_algorithms/camera_sensor_directional_led_detector_algorithm.h
    simulator/camera_sensor_algorithms/camera_sensor_led_detector_algorithm.h
    simulator/camera_sensor_algorithms/camera_sensor_tag_detector_algorithm.h
    simulator/colored_blob_omnidirectional_camera_rotzonly_sensor.h
    simulator/colored_blob_perspective_camera_default_sensor.h
    simulator/differential_steering_default_actuator.h
    simulator/differential_steering_default_sensor.h
    simulator/directional_leds_default_actuator.h
    simulator/gripper_default_actuator.h
    simulator/ground_rotzonly_sensor.h
    simulator/leds_default_actuator.h
    simulator/light_default_sensor.h
    simulator/magnets_default_actuator.h
    simulator/positioning_default_sensor.h
    simulator/proximity_default_sensor.h
    simulator/quadrotor_position_default_actuator.h
    simulator/quadrotor_speed_default_actuator.h
    simulator/radios_default_actuator.h
    simulator/radios_default_sensor.h
    simulator/range_and_bearing_default_actuator.h
    simulator/range_and_bearing_medium_sensor.h
    simulator/tags_default_actuator.h)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Generic robot sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE}
  control_interface/ci_altitude_sensor.cpp
  control_interface/ci_battery_sensor.cpp
  control_interface/ci_camera_sensor.cpp
  control_interface/ci_camera_sensor_algorithms/ci_camera_sensor_directional_led_detector_algorithm.cpp
  control_interface/ci_camera_sensor_algorithms/ci_camera_sensor_led_detector_algorithm.cpp
  control_interface/ci_camera_sensor_algorithms/ci_camera_sensor_tag_detector_algorithm.cpp
  control_interface/ci_colored_blob_omnidirectional_camera_sensor.cpp
  control_interface/ci_colored_blob_perspective_camera_sensor.cpp
  control_interface/ci_differential_steering_actuator.cpp
  control_interface/ci_differential_steering_sensor.cpp
  control_interface/ci_directional_leds_actuator.cpp
  control_interface/ci_gripper_actuator.cpp
  control_interface/ci_ground_sensor.cpp
  control_interface/ci_leds_actuator.cpp
  control_interface/ci_light_sensor.cpp
  control_interface/ci_magnets_actuator.cpp
  control_interface/ci_pan_tilt_camera_actuator.cpp
  control_interface/ci_positioning_sensor.cpp
  control_interface/ci_proximity_sensor.cpp
  control_interface/ci_quadrotor_position_actuator.cpp
  control_interface/ci_quadrotor_speed_actuator.cpp
  control_interface/ci_radios_actuator.cpp
  control_interface/ci_radios_sensor.cpp
  control_interface/ci_range_and_bearing_actuator.cpp
  control_interface/ci_range_and_bearing_sensor.cpp
  control_interface/ci_tags_actuator.cpp)
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_SIMULATOR}
    simulator/battery_default_sensor.cpp
    simulator/camera_default_sensor.cpp
    simulator/camera_sensor_algorithms/camera_sensor_directional_led_detector_algorithm.cpp
    simulator/camera_sensor_algorithms/camera_sensor_led_detector_algorithm.cpp
    simulator/camera_sensor_algorithms/camera_sensor_tag_detector_algorithm.cpp
    simulator/colored_blob_omnidirectional_camera_rotzonly_sensor.cpp
    simulator/colored_blob_perspective_camera_default_sensor.cpp
    simulator/differential_steering_default_actuator.cpp
    simulator/differential_steering_default_sensor.cpp
    simulator/directional_leds_default_actuator.cpp
    simulator/gripper_default_actuator.cpp
    simulator/ground_rotzonly_sensor.cpp
    simulator/leds_default_actuator.cpp
    simulator/light_default_sensor.cpp
    simulator/magnets_default_actuator.cpp
    simulator/positioning_default_sensor.cpp
    simulator/proximity_default_sensor.cpp
    simulator/quadrotor_position_default_actuator.cpp
    simulator/quadrotor_speed_default_actuator.cpp
    simulator/radios_default_actuator.cpp
    simulator/radios_default_sensor.cpp
    simulator/range_and_bearing_default_actuator.cpp
    simulator/range_and_bearing_medium_sensor.cpp
    simulator/tags_default_actuator.cpp)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create generic robot plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_genericrobot SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_genericrobot argos3core_${ARGOS_BUILD_FOR})
if(ARGOS_BUILD_FOR_SIMULATOR)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
    argos3plugin_${ARGOS_BUILD_FOR}_entities
    argos3plugin_${ARGOS_BUILD_FOR}_media)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Install stuff
#
install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/generic/control_interface)
if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_SIMULATOR}      DESTINATION include/argos3/plugins/robots/generic/simulator)
endif(ARGOS_BUILD_FOR_SIMULATOR)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
