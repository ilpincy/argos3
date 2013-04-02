add_executable(test-range unit/test-range.cpp)
target_link_libraries(test-range argos3core_${ARGOS_BUILD_FOR})

add_executable(test-list unit/test-list.cpp)

add_executable(test-grid
  unit/test-grid.cpp)
target_link_libraries(test-grid
  argos3core_${ARGOS_BUILD_FOR})

# add_executable(test-reset unit/test-reset.cpp)
# target_link_libraries(test-reset argos3core_${ARGOS_BUILD_FOR})

# add_library(test_footbot_controller MODULE
#   experiment/test_footbot_controller.h
#   experiment/test_footbot_controller.cpp)
# target_link_libraries(test_footbot_controller
#   argos3plugin_${ARGOS_BUILD_FOR}_footbot)

add_library(test_loop_functions MODULE
  experiment/test_loop_functions.h
  experiment/test_loop_functions.cpp)

# if(GOOGLEPERFTOOLS_FOUND)
#   add_executable(argos3_prof
#     ${CMAKE_SOURCE_DIR}/core/simulator/query_plugins.cpp
#     ${CMAKE_SOURCE_DIR}/core/simulator/main.cpp)
#   target_link_libraries(argos3_prof
#     tcmalloc profiler
#     argos3core_simulator
#     argos3plugin_simulator_footbot
#     argos3plugin_simulator_genericrobot
#     argos3plugin_simulator_qtopengl
#     argos3plugin_simulator_sensors
#     argos3plugin_simulator_entities
#     argos3plugin_simulator_dynamics2d
#     argos3plugin_simulator_actuators)
# endif(GOOGLEPERFTOOLS_FOUND)
