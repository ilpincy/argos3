#
# BuilderBot headers
#
# argos3/plugins/robots/builderbot/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_CONTROLINTERFACE
  control_interface/ci_builderbot_camera_system_sensor.h
  control_interface/ci_builderbot_differential_drive_actuator.h
  control_interface/ci_builderbot_differential_drive_sensor.h
  control_interface/ci_builderbot_electromagnet_system_actuator.h
  control_interface/ci_builderbot_electromagnet_system_sensor.h
  control_interface/ci_builderbot_leds_actuator.h
  control_interface/ci_builderbot_lift_system_actuator.h
  control_interface/ci_builderbot_lift_system_sensor.h
  control_interface/ci_builderbot_system_sensor.h
  control_interface/ci_builderbot_rangefinders_sensor.h)
if(ARGOS_BUILD_FOR_SIMULATOR)
  # argos3/plugins/robots/builderbot/simulator
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR
    simulator/builderbot_camera_system_default_sensor.h
    simulator/builderbot_differential_drive_default_actuator.h
    simulator/builderbot_differential_drive_default_sensor.h
    simulator/builderbot_differential_drive_entity.h
    simulator/builderbot_electromagnet_system_default_actuator.h
    simulator/builderbot_electromagnet_system_default_sensor.h
    simulator/builderbot_electromagnet_system_entity.h
    simulator/builderbot_entity.h
    simulator/builderbot_leds_default_actuator.h
    simulator/builderbot_lift_system_default_actuator.h
    simulator/builderbot_lift_system_default_sensor.h
    simulator/builderbot_lift_system_entity.h
    simulator/builderbot_system_default_sensor.h
    simulator/builderbot_rangefinders_default_sensor.h
    simulator/dynamics3d_builderbot_model.h)
  if(ARGOS_QTOPENGL_FOUND)
    include_directories(${ARGOS_QTOPENGL_INCLUDE_DIRS})
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR
      ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR}
      simulator/qtopengl_builderbot.h)
  endif(ARGOS_QTOPENGL_FOUND)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# BuilderBot sources
#
# argos3/plugins/robots/builderbot/control_interface
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_CONTROLINTERFACE}
  control_interface/ci_builderbot_camera_system_sensor.cpp
  control_interface/ci_builderbot_differential_drive_actuator.cpp
  control_interface/ci_builderbot_differential_drive_sensor.cpp
  control_interface/ci_builderbot_electromagnet_system_actuator.cpp
  control_interface/ci_builderbot_electromagnet_system_sensor.cpp
  control_interface/ci_builderbot_leds_actuator.cpp
  control_interface/ci_builderbot_lift_system_actuator.cpp
  control_interface/ci_builderbot_lift_system_sensor.cpp
  control_interface/ci_builderbot_system_sensor.cpp
  control_interface/ci_builderbot_rangefinders_sensor.cpp)
if(ARGOS_BUILD_FOR_SIMULATOR)
  # argos3/plugins/robots/builderbot/simulator
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR}
    simulator/builderbot_camera_system_default_sensor.cpp
    simulator/builderbot_differential_drive_default_actuator.cpp
    simulator/builderbot_differential_drive_default_sensor.cpp
    simulator/builderbot_differential_drive_entity.cpp
    simulator/builderbot_electromagnet_system_default_actuator.cpp
    simulator/builderbot_electromagnet_system_default_sensor.cpp
    simulator/builderbot_electromagnet_system_entity.cpp
    simulator/builderbot_entity.cpp
    simulator/builderbot_leds_default_actuator.cpp
    simulator/builderbot_lift_system_default_actuator.cpp
    simulator/builderbot_lift_system_default_sensor.cpp
    simulator/builderbot_lift_system_entity.cpp
    simulator/builderbot_system_default_sensor.cpp
    simulator/builderbot_rangefinders_default_sensor.cpp
    simulator/dynamics3d_builderbot_model.cpp)
  if(ARGOS_QTOPENGL_FOUND)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT
      ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT}
      simulator/qtopengl_builderbot.cpp)
  endif(ARGOS_QTOPENGL_FOUND)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_builderbot SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BUILDERBOT})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_builderbot
  argos3core_${ARGOS_BUILD_FOR}
  argos3plugin_${ARGOS_BUILD_FOR}_genericrobot)
if(ARGOS_BUILD_FOR_SIMULATOR)
  if(ARGOS_USE_DOUBLE)
    # Use double precision in Bullet, if ARGoS is using double precision
    target_compile_definitions(argos3plugin_${ARGOS_BUILD_FOR}_builderbot PUBLIC BT_USE_DOUBLE_PRECISION)
  endif(ARGOS_USE_DOUBLE)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_builderbot
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_dynamics3d
    argos3plugin_${ARGOS_BUILD_FOR}_entities)
  if(ARGOS_QTOPENGL_FOUND)
    target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_builderbot argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
  endif(ARGOS_QTOPENGL_FOUND)
endif(ARGOS_BUILD_FOR_SIMULATOR)
set(ARGOS_PLUGIN_PATH "${ARGOS_PLUGIN_PATH}:${CMAKE_CURRENT_BINARY_DIR}" CACHE INTERNAL "ARGoS plugin path")

#
# Install plugin
#
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_builderbot
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/builderbot/control_interface)
if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BUILDERBOT_SIMULATOR} DESTINATION include/argos3/plugins/robots/builderbot/simulator)
  if(ARGOS_QTOPENGL_FOUND)
    install(DIRECTORY simulator/models DESTINATION include/argos3/plugins/simulator/visualizations/qt-opengl)
  endif(ARGOS_QTOPENGL_FOUND)
endif(ARGOS_BUILD_FOR_SIMULATOR)
