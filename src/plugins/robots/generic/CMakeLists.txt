#
# Generic robot headers
#
# argos3/plugins/robots/generic/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE
  control_interface/ci_differential_steering_actuator.h
  control_interface/ci_leds_actuator.h
  control_interface/ci_proximity_sensor.h
  control_interface/ci_ring_proximity_sensor.h)
# argos3/plugins/robots/generic/wrappers/lua
if(LUA51_FOUND)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_WRAPPERS_LUA
    wrappers/lua/lua_differential_steering_actuator.h
    wrappers/lua/lua_leds_actuator.h
    wrappers/lua/lua_ring_proximity_sensor.h)
endif(LUA51_FOUND)

#
# Generic robot sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE}
  control_interface/ci_leds_actuator.cpp
  control_interface/ci_ring_proximity_sensor.cpp)

#
# Compile Lua 5.1 wrapping
#
if(LUA51_FOUND)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_WRAPPERS_LUA}
    wrappers/lua/lua_differential_steering_actuator.cpp
    wrappers/lua/lua_leds_actuator.cpp
    wrappers/lua/lua_ring_proximity_sensor.cpp)
endif(LUA51_FOUND)

#
# Create generic robot plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_genericrobot SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_GENERIC})

#
# Install stuff
#
install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/foot-bot/control_interface)
if(LUA51_FOUND)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_GENERIC_WRAPPERS_LUA}   DESTINATION include/argos3/plugins/robots/generic/wrappers/lua)
endif(LUA51_FOUND)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
