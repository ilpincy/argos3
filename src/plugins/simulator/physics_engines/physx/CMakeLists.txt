#
# Headers
#
# argos3/plugins/simulator/physics_engines/physx
# Headers for the PhysX engine
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PHYSX
  physx_differential_drive.h
  physx_engine.h
  physx_model.h
  physx_multi_body_object_model.h
  physx_single_body_object_model.h
  physx_stretchable_object_model.h
  physx_box_model.h
  physx_cylinder_model.h)

#
# Source files
#
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PHYSX
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PHYSX}
  physx_differential_drive.cpp
  physx_engine.cpp
  physx_box_model.cpp
  physx_cylinder_model.cpp
  physx_multi_body_object_model.cpp
  physx_single_body_object_model.cpp)

#
# Create physx engine plugin library
#
# Create library
add_library(argos3plugin_${ARGOS_BUILD_FOR}_physx SHARED
  ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PHYSX})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_physx
  argos3core_${ARGOS_BUILD_FOR}
  argos3plugin_${ARGOS_BUILD_FOR}_entities
  ${ARGOS_START_LIB_GROUP}
  SimulationController
  SceneQuery
  PxTask
  PvdRuntime
  PhysXVisualDebuggerSDK
  PhysXProfileSDK
  PhysX3Vehicle
  PhysX3Extensions
  PhysX3Cooking
  PhysX3Common
  PhysX3CharacterKinematic
  PhysX3
  LowLevelCloth
  LowLevel
  ${ARGOS_END_LIB_GROUP})

#
# Install files
#
install(FILES
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PHYSX}
  DESTINATION
  include/argos3/plugins/simulator/physics_engines/physx)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_physx
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
