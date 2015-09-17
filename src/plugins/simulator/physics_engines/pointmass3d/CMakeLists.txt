#
# Headers
#
# argos3/plugins/simulator/physics_engines/pointmass3d/
# Headers of the 2d dynamics physics engine
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_POINTMASS3D
  pointmass3d_cylinder_model.h
  pointmass3d_box_model.h
  pointmass3d_engine.h
  pointmass3d_model.h
  pointmass3d_quadrotor_model.h)

#
# Source files
#
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_POINTMASS3D
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_POINTMASS3D}
  pointmass3d_cylinder_model.cpp
  pointmass3d_box_model.cpp
  pointmass3d_engine.cpp
  pointmass3d_model.cpp
  pointmass3d_quadrotor_model.cpp)

#
# Create pointmass3d engine plugin library
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_pointmass3d SHARED
  ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_POINTMASS3D})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_pointmass3d argos3core_${ARGOS_BUILD_FOR})

install(FILES
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_POINTMASS3D}
  DESTINATION
  include/argos3/plugins/simulator/physics_engines/pointmass3d)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_pointmass3d
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
