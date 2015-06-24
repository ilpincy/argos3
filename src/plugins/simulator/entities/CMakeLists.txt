#
# Headers
#
# argos3/plugins/simulator/entities/
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_ENTITIES
  box_entity.h
  cylinder_entity.h
  gripper_equipped_entity.h
  ground_sensor_equipped_entity.h
  led_entity.h
  led_equipped_entity.h
  light_entity.h
  light_sensor_equipped_entity.h
  omnidirectional_camera_equipped_entity.h
  perspective_camera_equipped_entity.h
  proximity_sensor_equipped_entity.h
  quadrotor_entity.h
  rab_equipped_entity.h
  rotor_equipped_entity.h
  wheeled_entity.h
  wifi_equipped_entity.h)

#
# Source files
#
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_ENTITIES
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_ENTITIES}
  box_entity.cpp
  cylinder_entity.cpp
  gripper_equipped_entity.cpp
  ground_sensor_equipped_entity.cpp
  led_entity.cpp
  led_equipped_entity.cpp
  light_entity.cpp
  light_sensor_equipped_entity.cpp
  omnidirectional_camera_equipped_entity.cpp
  perspective_camera_equipped_entity.cpp
  proximity_sensor_equipped_entity.cpp
  quadrotor_entity.cpp
  rab_equipped_entity.cpp
  rotor_equipped_entity.cpp
  wheeled_entity.cpp
  wifi_equipped_entity.cpp)

#
# Create entity plugin library
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_entities SHARED ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_ENTITIES})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_entities argos3plugin_${ARGOS_BUILD_FOR}_media)

#
# Core library installation
#
install(FILES ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_ENTITIES} DESTINATION include/argos3/plugins/simulator/entities/)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_entities
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
