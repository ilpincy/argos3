#
# Spiri headers
#
# argos3/plugins/robots/spiri/simulator
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_SPIRI_SIMULATOR
    simulator/spiri_entity.h
    simulator/pointmass3d_spiri_model.h)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Spiri sources
#
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_SPIRI
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_SPIRI}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_SPIRI_SIMULATOR}
    simulator/spiri_entity.cpp
    simulator/pointmass3d_spiri_model.cpp)
  # Compile the graphical visualization only if the necessary libraries have been found
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_SPIRI_SIMULATOR
      ${ARGOS3_HEADERS_PLUGINS_ROBOTS_SPIRI_SIMULATOR}
      simulator/qtopengl_spiri.h)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_SPIRI
      ${ARGOS3_SOURCES_PLUGINS_ROBOTS_SPIRI}
      simulator/qtopengl_spiri.h
      simulator/qtopengl_spiri.cpp)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create Spiri plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_spiri SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_SPIRI})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_spiri
  argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
  argos3plugin_${ARGOS_BUILD_FOR}_pointmass3d)
if(ARGOS_COMPILE_QTOPENGL)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_spiri argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
endif(ARGOS_COMPILE_QTOPENGL)

install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_SPIRI_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/spiri/control_interface)

if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_SPIRI_SIMULATOR}      DESTINATION include/argos3/plugins/robots/spiri/simulator)
endif(ARGOS_BUILD_FOR_SIMULATOR)

install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_spiri
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
