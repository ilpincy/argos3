#
# Headers
#
# argos3/plugins/simulator/physics_engines/physx
# Headers for the PhysX engine
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PHYSX
  physx_engine.h
  physx_model.h
  physx_stretchable_object_model.h
  physx_stretchable_object_model_impl.h
  physx_box_model.h
  physx_cylinder_model.h)

#
# Source files
#
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PHYSX
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PHYSX}
  physx_engine.cpp
  physx_box_model.cpp
  physx_cylinder_model.cpp)

#
# Set the include and link path
#
link_directories(${CMAKE_SOURCE_DIR}/plugins/simulator/physics_engines/physx/physx-dist/Lib/linux32)

#
# Create physx engine plugin library
#
# Create library
add_library(argos3plugin_${ARGOS_BUILD_FOR}_physx SHARED
  ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PHYSX})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_physx
  argos3core_${ARGOS_BUILD_FOR}
  argos3plugin_${ARGOS_BUILD_FOR}_entities
  -Wl,--start-group
  PhysX3CommonCHECKED
  PxTaskCHECKED
  PhysXVisualDebuggerSDKCHECKED
  PhysXProfileSDKCHECKED
  RepXUpgrader3CHECKED
  RepX3CHECKED
  PhysX3CharacterKinematicCHECKED
  PhysX3ExtensionsCHECKED
  PhysX3CookingCHECKED
  PhysX3VehicleCHECKED
  PhysX3CHECKED
  LowLevelClothCHECKED
  LowLevelCHECKED
  SceneQueryCHECKED
  SimulationControllerCHECKED
  PvdRuntimeCHECKED
  -Wl,--end-group)

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
