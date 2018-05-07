add_executable(test-range unit/test-range.cpp)
target_link_libraries(test-range argos3core_${ARGOS_BUILD_FOR})

add_executable(test-list unit/test-list.cpp)

add_executable(test-box
  unit/test-box.cpp)
target_link_libraries(test-box
  argos3core_${ARGOS_BUILD_FOR})

add_executable(test-cylinder
  unit/test-cylinder.cpp)
target_link_libraries(test-cylinder
  argos3core_${ARGOS_BUILD_FOR})

add_executable(test-grid
  unit/test-grid.cpp)
target_link_libraries(test-grid
  argos3core_${ARGOS_BUILD_FOR})

add_executable(test-server
  unit/test-server.cpp)
target_link_libraries(test-server
  argos3core_${ARGOS_BUILD_FOR})

add_executable(test-client
  unit/test-client.cpp)
target_link_libraries(test-client
  argos3core_${ARGOS_BUILD_FOR})

add_executable(test-rate
  unit/test-rate.cpp)
target_link_libraries(test-rate
  argos3core_${ARGOS_BUILD_FOR})

add_executable(test-rng
  unit/test-rng.cpp)
target_link_libraries(test-rng
  argos3core_${ARGOS_BUILD_FOR})

# add_executable(test-reset unit/test-reset.cpp)
# target_link_libraries(test-reset argos3core_${ARGOS_BUILD_FOR})

if(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR STREQUAL "foot-bot")
  add_library(test_media_enabledisable MODULE
    experiment/test_media_enabledisable_controller.h
    experiment/test_media_enabledisable_controller.cpp
    experiment/test_media_enabledisable_loopfunctions.h
    experiment/test_media_enabledisable_loopfunctions.cpp)
  target_link_libraries(test_media_enabledisable
    argos3plugin_${ARGOS_BUILD_FOR}_footbot)
  add_library(test_footbot_controller MODULE
    experiment/test_footbot_controller.h
    experiment/test_footbot_controller.cpp)
  target_link_libraries(test_footbot_controller
    argos3plugin_${ARGOS_BUILD_FOR}_footbot)
  add_library(test_eyebot_controller MODULE
    experiment/test_eyebot_controller.h
    experiment/test_eyebot_controller.cpp)
  target_link_libraries(test_eyebot_controller
    argos3plugin_${ARGOS_BUILD_FOR}_eyebot)
  if(ARGOS_COMPILE_QTOPENGL)
    add_library(test_loop_functions MODULE
      experiment/test_loop_functions.h
      experiment/test_loop_functions.cpp
      experiment/test_user_functions.h
      experiment/test_user_functions.cpp)
  else(ARGOS_COMPILE_QTOPENGL)
    add_library(test_loop_functions MODULE
      experiment/test_loop_functions.h
      experiment/test_loop_functions.cpp)
  endif(ARGOS_COMPILE_QTOPENGL)
  target_link_libraries(test_loop_functions
    argos3core_${ARGOS_BUILD_FOR}
    argos3plugin_${ARGOS_BUILD_FOR}_footbot)
endif(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR STREQUAL "foot-bot")

if(ARGOS_BUILD_FOR_SIMULATOR AND GOOGLEPERFTOOLS_FOUND)
  add_executable(argos3_prof
    ${CMAKE_SOURCE_DIR}/core/simulator/query_plugins.cpp
    ${CMAKE_SOURCE_DIR}/core/simulator/main.cpp)
  target_link_libraries(argos3_prof
    tcmalloc profiler
    argos3core_simulator
    argos3plugin_simulator_epuck
    argos3plugin_simulator_footbot
    argos3plugin_simulator_genericrobot
    argos3plugin_simulator_media
    argos3plugin_simulator_qtopengl
    argos3plugin_simulator_entities
    argos3plugin_simulator_dynamics2d
    argos3plugin_simulator_pointmass3d)
#    argos3plugin_simulator_physx
endif(ARGOS_BUILD_FOR_SIMULATOR AND GOOGLEPERFTOOLS_FOUND)
