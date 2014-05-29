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
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  link_directories(${CMAKE_SOURCE_DIR}/plugins/simulator/physics_engines/physx/physx-dist/lib/linux32)
else(CMAKE_SIZEOF_VOID_P EQUAL 4)
  link_directories(${CMAKE_SOURCE_DIR}/plugins/simulator/physics_engines/physx/physx-dist/lib/linux64)
endif(CMAKE_SIZEOF_VOID_P EQUAL 4)

#
# Add necessary definitions
#
add_definitions(-DNDEBUG)

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
  PhysX3Common
  PxTask
  PhysXVisualDebuggerSDK
  PhysXProfileSDK
  RepXUpgrader3
  RepX3
  PhysX3CharacterKinematic
  PhysX3Extensions
  PhysX3Cooking
  PhysX3Vehicle
  PhysX3
  LowLevelCloth
  LowLevel
  SceneQuery
  SimulationController
  PvdRuntime
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
