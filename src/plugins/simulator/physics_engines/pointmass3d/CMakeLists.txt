#
# Headers
#
# argos3/plugins/simulator/physics_engines/pm3d/
# Headers of the 2d dynamics physics engine
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PM3D
  pm3d_box_model.h
  pm3d_cylinder_model.h
  pm3d_differentialsteering_control.h
  pm3d_dynamicmodel.h
  pm3d_engine.h
  pm3d_kinematicmodel.h
  pm3d_model.h
  pm3d_quadrotor_control.h
  pm3d_quadrotor_model.h)

#
# Source files
#
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PM3D
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PM3D}
  pm3d_box_model.cpp
  pm3d_cylinder_model.cpp
  pm3d_differentialsteering_control.cpp
  pm3d_dynamicmodel.cpp
  pm3d_engine.cpp
  pm3d_kinematicmodel.cpp
  pm3d_model.cpp
  pm3d_quadrotor_model.cpp)

#
# Create pm3d engine plugin library
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_pm3d SHARED
  ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PM3D})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_pm3d argos3core_${ARGOS_BUILD_FOR})

install(FILES
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PM3D}
  DESTINATION
  include/argos3/plugins/simulator/physics_engines/pm3d)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_pm3d
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
