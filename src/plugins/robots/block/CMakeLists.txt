#
# Block headers
#
if(ARGOS_BUILD_FOR_SIMULATOR)
  # argos3/plugins/robots/block/simulator
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BLOCK_SIMULATOR
    simulator/block_entity.h
    simulator/dynamics3d_block_model.h)
  if(ARGOS_QTOPENGL_FOUND)
    include_directories(${ARGOS_QTOPENGL_INCLUDE_DIRS})
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_BLOCK_SIMULATOR
      ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BLOCK_SIMULATOR}
      simulator/qtopengl_block.h)
  endif(ARGOS_QTOPENGL_FOUND)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Block sources
#
if(ARGOS_BUILD_FOR_SIMULATOR)
  # argos3/plugins/robots/block/simulator
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BLOCK
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BLOCK}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BLOCK_SIMULATOR}
    simulator/block_entity.cpp
    simulator/dynamics3d_block_model.cpp)
  if(ARGOS_QTOPENGL_FOUND)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_BLOCK
      ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BLOCK}
      simulator/qtopengl_block.cpp)
  endif(ARGOS_QTOPENGL_FOUND)
endif(ARGOS_BUILD_FOR_SIMULATOR)

if(ARGOS_BUILD_FOR_SIMULATOR)
  # Create a plug-in for the simulator
  add_library(argos3plugin_${ARGOS_BUILD_FOR}_block SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_BLOCK})
  # Use double precision in Bullet, if ARGoS is using double precision
  if(ARGOS_USE_DOUBLE)
    target_compile_definitions(argos3plugin_${ARGOS_BUILD_FOR}_block PUBLIC BT_USE_DOUBLE_PRECISION)
  endif(ARGOS_USE_DOUBLE)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_block
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_dynamics3d
    argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
    argos3plugin_${ARGOS_BUILD_FOR}_entities)
  if(ARGOS_QTOPENGL_FOUND)
    target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_block argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
  endif(ARGOS_QTOPENGL_FOUND)
  install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_block
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib/argos3
    ARCHIVE DESTINATION lib/argos3)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Add plugin to ARGOS_PLUGIN_PATH
#
set(ARGOS_PLUGIN_PATH "${ARGOS_PLUGIN_PATH}:${CMAKE_CURRENT_BINARY_DIR}" CACHE INTERNAL "ARGoS plugin path")

install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BLOCK_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/block/control_interface)

if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_BLOCK_SIMULATOR} DESTINATION include/argos3/plugins/robots/block/simulator)
  if(ARGOS_QTOPENGL_FOUND)
    install(DIRECTORY simulator/models DESTINATION include/argos3/plugins/simulator/visualizations/qt-opengl)
  endif(ARGOS_QTOPENGL_FOUND)
endif(ARGOS_BUILD_FOR_SIMULATOR)

