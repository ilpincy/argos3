#
# Pi-Puck headers
#
# argos3/plugins/robots/pi-puck/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_PIPUCK_CONTROLINTERFACE
  control_interface/ci_pipuck_front_camera_sensor.h
  control_interface/ci_pipuck_differential_drive_actuator.h
  control_interface/ci_pipuck_differential_drive_sensor.h
  control_interface/ci_pipuck_ground_sensor.h
  control_interface/ci_pipuck_imu_sensor.h
  control_interface/ci_pipuck_leds_actuator.h
  control_interface/ci_pipuck_system_sensor.h
  control_interface/ci_pipuck_rangefinders_sensor.h)
if(ARGOS_BUILD_FOR_SIMULATOR)
  # argos3/plugins/robots/pi-puck/simulator
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_PIPUCK_SIMULATOR
    simulator/dynamics2d_pipuck_model.h
    simulator/dynamics3d_pipuck_model.h
    simulator/pipuck_front_camera_default_sensor.h
    simulator/pipuck_differential_drive_default_actuator.h
    simulator/pipuck_differential_drive_default_sensor.h
    simulator/pipuck_differential_drive_entity.h
    simulator/pipuck_entity.h
    simulator/pipuck_ground_default_sensor.h
    simulator/pipuck_imu_default_sensor.h
    simulator/pipuck_leds_default_actuator.h
    simulator/pipuck_system_default_sensor.h
    simulator/pipuck_rangefinders_default_sensor.h)
  if(ARGOS_QTOPENGL_FOUND)
    include_directories(${ARGOS_QTOPENGL_INCLUDE_DIRS})
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_PIPUCK_SIMULATOR
      ${ARGOS3_HEADERS_PLUGINS_ROBOTS_PIPUCK_SIMULATOR}
      simulator/qtopengl_pipuck.h)
  endif(ARGOS_QTOPENGL_FOUND)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Pi-Puck sources
#
# argos3/plugins/robots/pi-puck/control_interface
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_PIPUCK
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_PIPUCK_CONTROLINTERFACE}
  control_interface/ci_pipuck_front_camera_sensor.cpp
  control_interface/ci_pipuck_differential_drive_actuator.cpp
  control_interface/ci_pipuck_differential_drive_sensor.cpp
  control_interface/ci_pipuck_ground_sensor.cpp
  control_interface/ci_pipuck_imu_sensor.cpp
  control_interface/ci_pipuck_leds_actuator.cpp  
  control_interface/ci_pipuck_system_sensor.cpp
  control_interface/ci_pipuck_rangefinders_sensor.cpp)
if(ARGOS_BUILD_FOR_SIMULATOR)
  # argos3/plugins/robots/pi-puck/simulator
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_PIPUCK
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_PIPUCK}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_PIPUCK_SIMULATOR}
    simulator/dynamics2d_pipuck_model.cpp
    simulator/dynamics3d_pipuck_model.cpp
    simulator/pipuck_front_camera_default_sensor.cpp
    simulator/pipuck_differential_drive_default_actuator.cpp
    simulator/pipuck_differential_drive_default_sensor.cpp
    simulator/pipuck_differential_drive_entity.cpp
    simulator/pipuck_entity.cpp
    simulator/pipuck_ground_default_sensor.cpp
    simulator/pipuck_imu_default_sensor.cpp
    simulator/pipuck_leds_default_actuator.cpp
    simulator/pipuck_system_default_sensor.cpp
    simulator/pipuck_rangefinders_default_sensor.cpp)
  if(ARGOS_QTOPENGL_FOUND)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_PIPUCK
      ${ARGOS3_SOURCES_PLUGINS_ROBOTS_PIPUCK}
      simulator/qtopengl_pipuck.cpp)
  endif(ARGOS_QTOPENGL_FOUND)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_pipuck SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_PIPUCK})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_pipuck
  argos3core_${ARGOS_BUILD_FOR}
  argos3plugin_${ARGOS_BUILD_FOR}_genericrobot)
if(ARGOS_BUILD_FOR_SIMULATOR)
  if(ARGOS_USE_DOUBLE)
    # Use double precision in Bullet, if ARGoS is using double precision
    target_compile_definitions(argos3plugin_${ARGOS_BUILD_FOR}_pipuck PUBLIC BT_USE_DOUBLE_PRECISION)
  endif(ARGOS_USE_DOUBLE)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_pipuck
    argos3plugin_${ARGOS_BUILD_FOR}_dynamics2d
    argos3plugin_${ARGOS_BUILD_FOR}_dynamics3d
    argos3plugin_${ARGOS_BUILD_FOR}_entities)
  if(ARGOS_QTOPENGL_FOUND)
    target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_pipuck argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
  endif(ARGOS_QTOPENGL_FOUND)
endif(ARGOS_BUILD_FOR_SIMULATOR)
set(ARGOS_PLUGIN_PATH "${ARGOS_PLUGIN_PATH}:${CMAKE_CURRENT_BINARY_DIR}" CACHE INTERNAL "ARGoS plugin path")

#
# Install plugin
#
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_pipuck
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_PIPUCK_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/pi-puck/control_interface)
if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_PIPUCK_SIMULATOR} DESTINATION include/argos3/plugins/robots/pi-puck/simulator)
  if(ARGOS_QTOPENGL_FOUND)
    install(DIRECTORY simulator/models DESTINATION include/argos3/plugins/simulator/visualizations/qt-opengl)
  endif(ARGOS_QTOPENGL_FOUND)
endif(ARGOS_BUILD_FOR_SIMULATOR)
