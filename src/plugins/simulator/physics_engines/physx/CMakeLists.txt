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
# Set the link path, depending on the architecture
#
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  # 32 bits
  if(APPLE)
    # Mac OSX
    link_directories(${CMAKE_SOURCE_DIR}/plugins/simulator/physics_engines/physx/physx_dist/lib/osx32)
  else(APPLE)
    # Linux
    link_directories(${CMAKE_SOURCE_DIR}/plugins/simulator/physics_engines/physx/physx_dist/lib/linux32)
  endif(APPLE)
else(CMAKE_SIZEOF_VOID_P EQUAL 4)
  # 64 bits
  if(APPLE)
    # Mac OSX
    link_directories(${CMAKE_SOURCE_DIR}/plugins/simulator/physics_engines/physx/physx_dist/lib/osx64)
  else(APPLE)
    # Linux
    link_directories(${CMAKE_SOURCE_DIR}/plugins/simulator/physics_engines/physx/physx_dist/lib/linux64)
  endif(APPLE)
endif(CMAKE_SIZEOF_VOID_P EQUAL 4)

#
# Create physx engine plugin library
#
# Create library
add_library(argos3plugin_${ARGOS_BUILD_FOR}_physx SHARED
  ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_PHYSX})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_physx
  argos3core_${ARGOS_BUILD_FOR}
  argos3plugin_${ARGOS_BUILD_FOR}_entities
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
  LowLevel)

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
