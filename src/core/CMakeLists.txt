#
# Headers
#
# argos3/core/utility
set(ARGOS3_HEADERS_UTILITY
  utility/signal_processing.h
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
  utility/datatypes/datatypes.h
  utility/datatypes/set.h)
# argos3/core/utility/logging
set(ARGOS3_HEADERS_UTILITY_LOGGING
  utility/logging/argos_colored_text.h
  utility/logging/argos_log.h)
# argos3/core/utility/networking
set(ARGOS3_HEADERS_UTILITY_NETWORKING
  utility/networking/tcp_socket.h)
# argos3/core/utility/plugins
set(ARGOS3_HEADERS_UTILITY_PLUGINS
  utility/plugins/dynamic_loading.h
  utility/plugins/vtable.h
  utility/plugins/factory.h
  utility/plugins/factory_impl.h)
# argos3/core/utility/profiler
set(ARGOS3_HEADERS_UTILITY_PROFILER
  utility/profiler/profiler.h)
# argos3/core/utility/math
set(ARGOS3_HEADERS_UTILITY_MATH
  utility/math/angles.h
  utility/math/box.h
  utility/math/cylinder.h
  utility/math/general.h
  utility/math/plane.h
  utility/math/quaternion.h
  utility/math/range.h
  utility/math/ray2.h
  utility/math/ray3.h
  utility/math/rng.h
  utility/math/vector2.h
  utility/math/vector3.h)
# argos3/core/utility/math/matrix
set(ARGOS3_HEADERS_UTILITY_MATH_MATRIX
  utility/math/matrix/matrix.h
  utility/math/matrix/squarematrix.h
  utility/math/matrix/rotationmatrix2.h
  utility/math/matrix/rotationmatrix3.h
  utility/math/matrix/transformationmatrix2.h
  utility/math/matrix/transformationmatrix3.h)
# argos3/core/control_interface
set(ARGOS3_HEADERS_CONTROLINTERFACE
  control_interface/ci_actuator.h
  control_interface/ci_controller.h
  control_interface/ci_sensor.h)
# argos3/core/simulator
set(ARGOS3_HEADERS_SIMULATOR
  simulator/actuator.h
  simulator/sensor.h
  simulator/argos_command_line_arg_parser.h
  simulator/loop_functions.h
  simulator/query_plugins.h
  simulator/simulator.h)
# argos3/core/simulator/entity
set(ARGOS3_HEADERS_SIMULATOR_ENTITY
  simulator/entity/composable_entity.h
  simulator/entity/controllable_entity.h
  simulator/entity/embodied_entity.h
  simulator/entity/entity.h
  simulator/entity/floor_entity.h
  simulator/entity/positional_entity.h)
# argos3/core/simulator/medium
set(ARGOS3_HEADERS_SIMULATOR_MEDIUM
  simulator/medium/medium.h)
# argos3/core/simulator/physics_engine
set(ARGOS3_HEADERS_SIMULATOR_PHYSICSENGINE
  simulator/physics_engine/physics_engine.h
  simulator/physics_engine/physics_model.h)
# argos3/core/simulator/visualization
set(ARGOS3_HEADERS_SIMULATOR_VISUALIZATION
  simulator/visualization/default_visualization.h
  simulator/visualization/visualization.h)
# argos3/core/simulator/space
set(ARGOS3_HEADERS_SIMULATOR_SPACE_POSITIONAL_INDICES
  simulator/space/positional_indices/grid.h
  simulator/space/positional_indices/grid_impl.h
  simulator/space/positional_indices/positional_index.h
  simulator/space/positional_indices/space_hash.h
  simulator/space/positional_indices/space_hash_native.h)
set(ARGOS3_HEADERS_SIMULATOR_SPACE
  simulator/space/space.h
  simulator/space/space_multi_thread_balance_length.h
  simulator/space/space_multi_thread_balance_quantity.h
  simulator/space/space_no_threads.h)
# argos3/core/wrappers/lua
set(ARGOS3_HEADERS_WRAPPERS_LUA
  wrappers/lua/lua_controller.h
  wrappers/lua/lua_utility.h)

#
# Source files
#
# Common for all builds
set(ARGOS3_SOURCES_CORE
  ${ARGOS3_HEADERS_UTILITY}
  utility/signal_processing.cpp
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
  ${ARGOS3_HEADERS_UTILITY_NETWORKING}
  utility/networking/tcp_socket.cpp
  ${ARGOS3_HEADERS_UTILITY_PLUGINS}
  ${ARGOS3_HEADERS_UTILITY_PROFILER}
  utility/profiler/profiler.cpp
  ${ARGOS3_HEADERS_UTILITY_MATH}
  ${ARGOS3_HEADERS_UTILITY_MATH_MATRIX}
  utility/math/angles.cpp
  utility/math/box.cpp
  utility/math/cylinder.cpp
  utility/math/vector2.cpp
  utility/math/vector3.cpp
  utility/math/plane.cpp
  utility/math/ray3.cpp
  utility/math/rng.cpp
  utility/math/matrix/rotationmatrix3.cpp
  utility/math/matrix/rotationmatrix2.cpp
  utility/math/matrix/transformationmatrix3.cpp
  utility/math/matrix/transformationmatrix2.cpp
  ${ARGOS3_HEADERS_CONTROLINTERFACE}
  control_interface/ci_controller.cpp)
# Compile dynamic library loading only if enabled
if(ARGOS_DYNAMIC_LIBRARY_LOADING)
  set(ARGOS3_SOURCES_CORE ${ARGOS3_SOURCES_CORE} utility/plugins/dynamic_loading.cpp)
endif(ARGOS_DYNAMIC_LIBRARY_LOADING)
# Compile simulator code only if requested
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_CORE
    ${ARGOS3_SOURCES_CORE}
    ${ARGOS3_HEADERS_SIMULATOR}
    simulator/argos_command_line_arg_parser.cpp
    simulator/loop_functions.cpp
    simulator/simulator.cpp
    ${ARGOS3_HEADERS_SIMULATOR_ENTITY}
    simulator/entity/composable_entity.cpp
    simulator/entity/controllable_entity.cpp
    simulator/entity/embodied_entity.cpp
    simulator/entity/entity.cpp
    simulator/entity/floor_entity.cpp
    simulator/entity/positional_entity.cpp
    ${ARGOS3_HEADERS_SIMULATOR_MEDIUM}
    simulator/medium/medium.cpp
    ${ARGOS3_HEADERS_SIMULATOR_PHYSICSENGINE}
    simulator/physics_engine/physics_engine.cpp
    simulator/physics_engine/physics_model.cpp
    ${ARGOS3_HEADERS_SIMULATOR_VISUALIZATION}
    simulator/visualization/default_visualization.cpp
    ${ARGOS3_HEADERS_SIMULATOR_SPACE}
    simulator/space/space.cpp
    simulator/space/space_multi_thread_balance_length.cpp
    simulator/space/space_multi_thread_balance_quantity.cpp
    simulator/space/space_no_threads.cpp)
endif(ARGOS_BUILD_FOR_SIMULATOR)
# Compile Lua wrapper only if Lua was found
if(ARGOS_WITH_LUA)
  set(ARGOS3_SOURCES_CORE
    ${ARGOS3_SOURCES_CORE}
    ${ARGOS3_HEADERS_WRAPPERS_LUA}
    wrappers/lua/lua_controller.cpp
    wrappers/lua/lua_utility.cpp)
endif(ARGOS_WITH_LUA)


#
# Create core ARGoS3 library
#
add_library(argos3core_${ARGOS_BUILD_FOR} SHARED ${ARGOS3_SOURCES_CORE})
# Link DLFCN library if necessary
if(ARGOS_DYNAMIC_LIBRARY_LOADING)
  target_link_libraries(argos3core_${ARGOS_BUILD_FOR} ${DLFCN_LIBRARY})
endif(ARGOS_DYNAMIC_LIBRARY_LOADING)
# Link PThreads library if necessary
if(PTHREADS_FOUND)
  target_link_libraries(argos3core_${ARGOS_BUILD_FOR} ${PTHREADS_LIBRARY})
endif(PTHREADS_FOUND)
# Link FreeImage library if necessary
if(FREEIMAGE_FOUND)
  target_link_libraries(argos3core_${ARGOS_BUILD_FOR} ${FREEIMAGE_LIBRARIES})
endif(FREEIMAGE_FOUND)
# Link Lua library if necessary
if(ARGOS_WITH_LUA)
  target_link_libraries(argos3core_${ARGOS_BUILD_FOR} ${LUA_LIBRARIES})
endif(ARGOS_WITH_LUA)

#
# Core library installation
install(FILES ${ARGOS3_HEADERS_UTILITY}                       DESTINATION include/argos3/core/utility)
install(FILES ${ARGOS3_HEADERS_UTILITY_CONFIGURATION}         DESTINATION include/argos3/core/utility/configuration)
install(FILES ${ARGOS3_HEADERS_UTILITY_CONFIGURATION_TINYXML} DESTINATION include/argos3/core/utility/configuration/tinyxml)
install(FILES ${ARGOS3_HEADERS_UTILITY_DATATYPES}             DESTINATION include/argos3/core/utility/datatypes)
install(FILES ${ARGOS3_HEADERS_UTILITY_LOGGING}               DESTINATION include/argos3/core/utility/logging)
install(FILES ${ARGOS3_HEADERS_UTILITY_NETWORKING}            DESTINATION include/argos3/core/utility/networking)
install(FILES ${ARGOS3_HEADERS_UTILITY_PLUGINS}               DESTINATION include/argos3/core/utility/plugins)
install(FILES ${ARGOS3_HEADERS_UTILITY_PROFILER}              DESTINATION include/argos3/core/utility/profiler)
install(FILES ${ARGOS3_HEADERS_UTILITY_MATH}                  DESTINATION include/argos3/core/utility/math)
install(FILES ${ARGOS3_HEADERS_UTILITY_MATH_MATRIX}           DESTINATION include/argos3/core/utility/math/matrix)
install(FILES ${ARGOS3_HEADERS_CONTROLINTERFACE}              DESTINATION include/argos3/core/control_interface)
if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_SIMULATOR}                   DESTINATION include/argos3/core/simulator)
  install(FILES ${ARGOS3_HEADERS_SIMULATOR_ACTUATORS}         DESTINATION include/argos3/core/simulator/actuators)
  install(FILES ${ARGOS3_HEADERS_SIMULATOR_ENTITY}            DESTINATION include/argos3/core/simulator/entity)
  install(FILES ${ARGOS3_HEADERS_SIMULATOR_MEDIUM}            DESTINATION include/argos3/core/simulator/medium)
  install(FILES ${ARGOS3_HEADERS_SIMULATOR_PHYSICSENGINE}     DESTINATION include/argos3/core/simulator/physics_engine)
  install(FILES ${ARGOS3_HEADERS_SIMULATOR_VISUALIZATION}     DESTINATION include/argos3/core/simulator/visualization)
  install(FILES ${ARGOS3_HEADERS_SIMULATOR_SPACE_POSITIONAL_INDICES} DESTINATION include/argos3/core/simulator/space/positional_indices)
  install(FILES ${ARGOS3_HEADERS_SIMULATOR_SPACE}             DESTINATION include/argos3/core/simulator/space)
endif(ARGOS_BUILD_FOR_SIMULATOR)
if(ARGOS_WITH_LUA)
  install(FILES ${ARGOS3_HEADERS_WRAPPERS_LUA}                DESTINATION include/argos3/core/wrappers/lua)
endif(ARGOS_WITH_LUA)
install(TARGETS argos3core_${ARGOS_BUILD_FOR}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)

if(ARGOS_BUILD_FOR_SIMULATOR)
  #
  # Create core ARGoS3 executable
  #
  add_executable(argos3
    simulator/query_plugins.cpp
    simulator/main.cpp)
  target_link_libraries(argos3 argos3core_${ARGOS_BUILD_FOR})
  #
  # Core ARGoS3 installation
  #
  if(APPLE)
    install(TARGETS argos3
      RUNTIME DESTINATION libexec
      LIBRARY DESTINATION lib/argos3
      ARCHIVE DESTINATION lib/argos3)
  else(APPLE)
    install(TARGETS argos3
      RUNTIME DESTINATION bin
      LIBRARY DESTINATION lib/argos3
      ARCHIVE DESTINATION lib/argos3)
  endif(APPLE)
  #
  # Installation of CMake scripts useful for the simulator
  #
  install(
    FILES
    ${CMAKE_SOURCE_DIR}/cmake/ARGoSCheckQTOpenGL.cmake
    ${CMAKE_SOURCE_DIR}/cmake/FindLua52.cmake
    DESTINATION
    share/argos3/cmake)
  #
  # Creation and installation of desktop file and MIME entry
  #
  if(NOT APPLE)
    configure_file(
      ${CMAKE_SOURCE_DIR}/scripts/argos3.desktop.in
      ${CMAKE_BINARY_DIR}/argos3.desktop
      @ONLY)
    install(FILES ${CMAKE_BINARY_DIR}/argos3.desktop DESTINATION share/applications)
    install(FILES ${CMAKE_SOURCE_DIR}/scripts/x-argos-mime-info.xml DESTINATION share/mime/packages RENAME argos.xml)
  endif(NOT APPLE)
endif(ARGOS_BUILD_FOR_SIMULATOR)
