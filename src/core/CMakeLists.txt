#
# Project name
#
project(argos3core)

#
# Configuration for CPack's tar.bz2 generator
#
set(CPACK_GENERATOR TBZ2)
set(CPACK_PACKAGE_VERSION_MAJOR "3")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ARGoS3 core package")
include(CPack)

#
# Include path is points to base source dir
#
include_directories(${CMAKE_SOURCE_DIR})

#
# Headers
#
# argos3/core/utility
set(ARGOS3_HEADERS_UTILITY
  utility/string_utilities.h)
# argos3/core/utility/configuration
set(ARGOS3_HEADERS_UTILITY_CONFIGURATION
  utility/configuration/argos_configuration.h
  utility/configuration/argos_exception.h
  utility/configuration/base_configurable_resource.h
  utility/configuration/command_line_arg_parser.h
  utility/configuration/memento.h)
# argos3/core/utility/configuration/tinyxml
set(ARGOS3_HEADERS_UTILITY_CONFIGURATION_TINYXML
  utility/configuration/tinyxml/ticpp.h
  utility/configuration/tinyxml/ticpprc.h
  utility/configuration/tinyxml/tinystr.h
  utility/configuration/tinyxml/tinyxml.h)
# argos3/core/utility/datatypes
set(ARGOS3_HEADERS_UTILITY_DATATYPES
  utility/datatypes/any.h
  utility/datatypes/byte_array.h
  utility/datatypes/color.h
  utility/datatypes/datatypes.h)
# argos3/core/utility/logging
set(ARGOS3_HEADERS_UTILITY_LOGGING
  utility/logging/argos_colored_text.h
  utility/logging/argos_log.h)
# argos3/core/utility/plugins
set(ARGOS3_HEADERS_UTILITY_PLUGINS
  utility/plugins/factory.h
  utility/plugins/dynamic_loading.h
  utility/plugins/factory_impl.h)
# argos3/core/utility/profiler
set(ARGOS3_HEADERS_UTILITY_PROFILER
  utility/profiler/profiler.h)
# argos3/core/utility/math
set(ARGOS3_HEADERS_UTILITY_MATH
  utility/math/angles.h
  utility/math/general.h
  utility/math/matrix.h
  utility/math/quaternion.h
  utility/math/range.h
  utility/math/ray.h
  utility/math/rotationmatrix2.h
  utility/math/rotationmatrix3.h
  utility/math/segment.h
  utility/math/squarematrix.h
  utility/math/vector2.h
  utility/math/vector3.h
  utility/math/rng.h)
# argos3/core/control_interface
set(ARGOS3_HEADERS_CONTROLINTERFACE
  control_interface/ci_actuator.h
  control_interface/ci_controller.h
  control_interface/ci_robot.h
  control_interface/ci_sensor.h)
# argos3/core/simulator
set(ARGOS3_HEADERS_SIMULATOR
  simulator/event.h
  simulator/actuator.h
  simulator/sensor.h
  simulator/argos_command_line_arg_parser.h
  simulator/loop_functions.h
  simulator/simulator.h)
# argos3/core/simulator/entity
set(ARGOS3_HEADERS_SIMULATOR_ENTITY
  simulator/entity/composable_entity.h
  simulator/entity/controllable_entity.h
  simulator/entity/embodied_entity.h
  simulator/entity/entity.h
  simulator/entity/floor_entity.h
  simulator/entity/medium_entity.h
  simulator/entity/positional_entity.h
  simulator/entity/wifi_medium_entity.h)
# argos3/core/simulator/physics_engine
set(ARGOS3_HEADERS_SIMULATOR_PHYSICSENGINE
  simulator/physics_engine/physics_engine.h
  simulator/physics_engine/physics_engine_entity.h)
# argos3/core/simulator/visualization
set(ARGOS3_HEADERS_SIMULATOR_VISUALIZATION
  simulator/visualization/visualization.h)
# argos3/core/simulator/space
set(ARGOS3_HEADERS_SIMULATOR_SPACE
  simulator/space/space.h
  simulator/space/space_hash.h
  simulator/space/space_hash_native.h
  simulator/space/space_multi_thread_h_dispatch.h
  simulator/space/space_multi_thread_scatter_gather.h
  simulator/space/space_no_threads.h)

#
# Source files
#
# Common for all builds
set(ARGOS3_SOURCES
  ${ARGOS3_HEADERS_UTILITY}
  utility/string_utilities.cpp
  ${ARGOS3_HEADERS_UTILITY_CONFIGURATION}
  utility/configuration/command_line_arg_parser.cpp
  ${ARGOS3_HEADERS_UTILITY_CONFIGURATION_TINYXML}
  utility/configuration/tinyxml/ticpp.cpp
  utility/configuration/tinyxml/tinystr.cpp
  utility/configuration/tinyxml/tinyxml.cpp
  utility/configuration/tinyxml/tinyxmlerror.cpp
  utility/configuration/tinyxml/tinyxmlparser.cpp
  ${ARGOS3_HEADERS_UTILITY_DATATYPES}
  utility/datatypes/byte_array.cpp
  utility/datatypes/color.cpp
  ${ARGOS3_HEADERS_UTILITY_LOGGING}
  utility/logging/argos_log.cpp
  ${ARGOS3_HEADERS_UTILITY_PLUGINS}
  ${ARGOS3_HEADERS_UTILITY_PROFILER}
  utility/profiler/profiler.cpp
  ${ARGOS3_HEADERS_UTILITY_MATH}
  utility/math/angles.cpp
  utility/math/rotationmatrix2.cpp
  utility/math/rotationmatrix3.cpp
  utility/math/segment.cpp
  utility/math/vector2.cpp
  utility/math/vector3.cpp
  utility/math/rng.cpp
  ${ARGOS3_HEADERS_CONTROLINTERFACE}
  control_interface/ci_robot.cpp)
# Compile dynamic library loading only if enabled
if(ARGOS3_DYNAMIC_LIBRARY_LOADING)
  set(ARGOS3_SOURCES ${ARGOS3_SOURCES} utility/plugins/dynamic_loading.cpp)
endif(ARGOS3_DYNAMIC_LIBRARY_LOADING)
# Compile simulator code only if requested
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES
    ${ARGOS3_SOURCES}
    ${ARGOS3_HEADERS_SIMULATOR}
    simulator/argos_command_line_arg_parser.cpp
    simulator/loop_functions.cpp
    simulator/simulator.cpp
    ${ARGOS3_HEADERS_SIMULATOR_ENTITY}
    simulator/entity/controllable_entity.cpp
    simulator/entity/embodied_entity.cpp
    simulator/entity/entity.cpp
    simulator/entity/floor_entity.cpp
    simulator/entity/positional_entity.cpp
    ${ARGOS3_HEADERS_SIMULATOR_PHYSICSENGINE}
    simulator/physics_engine/physics_engine.cpp
    ${ARGOS3_HEADERS_SIMULATOR_VISUALIZATION}
    simulator/visualization/visualization.cpp
    ${ARGOS3_HEADERS_SIMULATOR_SPACE}
    simulator/space/space.cpp
    simulator/space/space_multi_thread_h_dispatch.cpp
    simulator/space/space_multi_thread_scatter_gather.cpp
    simulator/space/space_no_threads.cpp)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create core ARGoS3 library
#
add_library(argos3core_${ARGOS_BUILD_FOR} SHARED ${ARGOS3_SOURCES})
set_target_properties(argos3core_${ARGOS_BUILD_FOR} PROPERTIES VERSION 3)
# Link DLFCN library if necessary
if(ARGOS3_DYNAMIC_LIBRARY_LOADING)
  target_link_libraries(argos3core_${ARGOS_BUILD_FOR} ${DLFCN_LIBRARY})
endif(ARGOS3_DYNAMIC_LIBRARY_LOADING)
# Link PThreads library if necessary
if(PTHREADS_FOUND)
  target_link_libraries(argos3core_${ARGOS_BUILD_FOR} ${PTHREADS_LIBRARY})
endif(PTHREADS_FOUND)
# Link GSL library if necessary
if(GSL_FOUND)
  target_link_libraries(argos3core_${ARGOS_BUILD_FOR} ${GSL_LIBRARIES})
endif(GSL_FOUND)
# Link FreeImage library if necessary
if(FREEIMAGE_FOUND)
  target_link_libraries(argos3core_${ARGOS_BUILD_FOR} ${FREEIMAGE_LIBRARIES})
endif(FREEIMAGE_FOUND)


#
# Core library installation
#
install(FILES ${ARGOS3_HEADERS_UTILITY}                       DESTINATION include/argos3/core/utility)
install(FILES ${ARGOS3_HEADERS_UTILITY_CONFIGURATION}         DESTINATION include/argos3/core/utility/configuration)
install(FILES ${ARGOS3_HEADERS_UTILITY_CONFIGURATION_TINYXML} DESTINATION include/argos3/core/utility/configuration/tinyxml)
install(FILES ${ARGOS3_HEADERS_UTILITY_DATATYPES}             DESTINATION include/argos3/core/utility/datatypes)
install(FILES ${ARGOS3_HEADERS_UTILITY_LOGGING}               DESTINATION include/argos3/core/utility/logging)
install(FILES ${ARGOS3_HEADERS_UTILITY_PLUGINS}               DESTINATION include/argos3/core/utility/plugins)
install(FILES ${ARGOS3_HEADERS_UTILITY_PROFILER}              DESTINATION include/argos3/core/utility/profiler)
install(FILES ${ARGOS3_HEADERS_UTILITY_MATH}                  DESTINATION include/argos3/core/utility/math)
install(FILES ${ARGOS3_HEADERS_CONTROLINTERFACE}              DESTINATION include/argos3/core/control_interface)
if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_SIMULATOR}                     DESTINATION include/argos3/core/simulator)
  install(FILES ${ARGOS3_HEADERS_SIMULATOR_ENTITY}              DESTINATION include/argos3/core/simulator/entity)
  install(FILES ${ARGOS3_HEADERS_SIMULATOR_PHYSICSENGINE}       DESTINATION include/argos3/core/simulator/physics_engine)
  install(FILES ${ARGOS3_HEADERS_SIMULATOR_VISUALIZATION}       DESTINATION include/argos3/core/simulator/visualization)
  install(FILES ${ARGOS3_HEADERS_SIMULATOR_SPACE}               DESTINATION include/argos3/core/simulator/space)
endif(ARGOS_BUILD_FOR_SIMULATOR)
install(TARGETS argos3core_${ARGOS_BUILD_FOR}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)

if(ARGOS_BUILD_FOR_SIMULATOR)
  #
  # Create core ARGoS3 executable
  #
  add_executable(argos3 simulator/main.cpp)
  target_link_libraries(argos3 argos3core_${ARGOS_BUILD_FOR})
  #
  # Core ARGoS3 installation
  #
  install(TARGETS argos3
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib/argos3
    ARCHIVE DESTINATION lib/argos3)
endif(ARGOS_BUILD_FOR_SIMULATOR)
