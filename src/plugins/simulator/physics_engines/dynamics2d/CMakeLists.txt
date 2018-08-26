#
# Headers
#
# argos3/plugins/simulator/physics_engines/dynamics2d/chipmunk-physics/include/constraints
# Chipmunk headers for constraints
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS2D_CHIPMUNK_CONSTRAINTS
  chipmunk-physics/include/constraints/cpGearJoint.h
  chipmunk-physics/include/constraints/cpSlideJoint.h
  chipmunk-physics/include/constraints/cpPivotJoint.h
  chipmunk-physics/include/constraints/cpDampedRotarySpring.h
  chipmunk-physics/include/constraints/cpSimpleMotor.h
  chipmunk-physics/include/constraints/cpGrooveJoint.h
  chipmunk-physics/include/constraints/cpRotaryLimitJoint.h
  chipmunk-physics/include/constraints/cpRatchetJoint.h
  chipmunk-physics/include/constraints/cpConstraint.h
  chipmunk-physics/include/constraints/util.h
  chipmunk-physics/include/constraints/cpPinJoint.h
  chipmunk-physics/include/constraints/cpDampedSpring.h)
# argos3/plugins/simulator/physics_engines/dynamics2d/chipmunk-physics/include
# Chipmunk base headers
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS2D_CHIPMUNK
  chipmunk-physics/include/cpShape.h
  chipmunk-physics/include/cpBody.h
  chipmunk-physics/include/chipmunk_unsafe.h
  chipmunk-physics/include/cpSpace.h
  chipmunk-physics/include/cpVect.h
  chipmunk-physics/include/chipmunk_private.h
  chipmunk-physics/include/chipmunk_types.h
  chipmunk-physics/include/cpBB.h
  chipmunk-physics/include/cpArbiter.h
  chipmunk-physics/include/cpSpatialIndex.h
  chipmunk-physics/include/chipmunk_ffi.h
  chipmunk-physics/include/cpPolyShape.h
  chipmunk-physics/include/chipmunk.h)
# argos3/plugins/simulator/physics_engines/dynamics2d/chipmunk-physics/include
# Headers of the 2d dynamics physics engine
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS2D
  dynamics2d_box_model.h
  dynamics2d_cylinder_model.h
  dynamics2d_differentialsteering_control.h
  dynamics2d_model.h
  dynamics2d_engine.h
  dynamics2d_gripping.h
  dynamics2d_single_body_object_model.h
  dynamics2d_multi_body_object_model.h
  dynamics2d_stretchable_object_model.h
  dynamics2d_velocity_control.h)

#
# Source files
#
# Chipmunk source files
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS2D_CHIPMUNK
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS2D_CHIPMUNK_CONSTRAINTS}
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS2D_CHIPMUNK}
  chipmunk-physics/src/cpShape.c
  chipmunk-physics/src/cpBody.c
  chipmunk-physics/src/cpSpaceComponent.c
  chipmunk-physics/src/cpBBTree.c
  chipmunk-physics/src/cpSpace.c
  chipmunk-physics/src/cpSpaceStep.c
  chipmunk-physics/src/cpSweep1D.c
  chipmunk-physics/src/constraints/cpGearJoint.c
  chipmunk-physics/src/constraints/cpSlideJoint.c
  chipmunk-physics/src/constraints/cpPivotJoint.c
  chipmunk-physics/src/constraints/cpDampedRotarySpring.c
  chipmunk-physics/src/constraints/cpSimpleMotor.c
  chipmunk-physics/src/constraints/cpGrooveJoint.c
  chipmunk-physics/src/constraints/cpRotaryLimitJoint.c
  chipmunk-physics/src/constraints/cpRatchetJoint.c
  chipmunk-physics/src/constraints/cpConstraint.c
  chipmunk-physics/src/constraints/cpPinJoint.c
  chipmunk-physics/src/constraints/cpDampedSpring.c
  chipmunk-physics/src/cpVect.c
  chipmunk-physics/src/cpBB.c
  chipmunk-physics/src/cpSpaceQuery.c
  chipmunk-physics/src/cpArbiter.c
  chipmunk-physics/src/cpArray.c
  chipmunk-physics/src/cpHashSet.c
  chipmunk-physics/src/cpSpatialIndex.c
  chipmunk-physics/src/cpPolyShape.c
  chipmunk-physics/src/cpCollision.c
  chipmunk-physics/src/cpSpaceHash.c
  chipmunk-physics/src/chipmunk.c)
# Dynamics 2D source files
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS2D
  ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS2D_CHIPMUNK}
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS2D}
  dynamics2d_box_model.cpp
  dynamics2d_cylinder_model.cpp
  dynamics2d_differentialsteering_control.cpp
  dynamics2d_engine.cpp
  dynamics2d_gripping.cpp
  dynamics2d_multi_body_object_model.cpp
  dynamics2d_single_body_object_model.cpp
  dynamics2d_stretchable_object_model.cpp
  dynamics2d_velocity_control.cpp)

#
# Create dynamics2d engine plugin library
#
# Add Chipmunk include path
include_directories(${CMAKE_SOURCE_DIR}/plugins/simulator/physics_engines/dynamics2d/chipmunk-physics/include)
# Set the C99 standard for all C files
set_source_files_properties(${ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS2D_CHIPMUNK}
  PROPERTIES
  COMPILE_FLAGS "-std=gnu99")
# Create library
add_library(argos3plugin_${ARGOS_BUILD_FOR}_dynamics2d SHARED
  ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS2D})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_dynamics2d argos3core_${ARGOS_BUILD_FOR})

install(
  FILES
  ${CMAKE_SOURCE_DIR}/plugins/simulator/physics_engines/dynamics2d/chipmunk-physics/CHIPMUNK_LICENSE.txt
  DESTINATION
  doc/argos3/licenses)
install(
  FILES
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS2D_CHIPMUNK_CONSTRAINTS}
  DESTINATION
  include/argos3/plugins/simulator/physics_engines/dynamics2d/chipmunk-physics/include/constraints)
install(
  FILES
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS2D_CHIPMUNK}
  DESTINATION
  include/argos3/plugins/simulator/physics_engines/dynamics2d/chipmunk-physics/include)
install(FILES
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES_DYNAMICS2D}
  DESTINATION
  include/argos3/plugins/simulator/physics_engines/dynamics2d)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_dynamics2d
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)
