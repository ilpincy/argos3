add_subdirectory(generic)

if(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR STREQUAL "foot-bot")
  add_subdirectory(foot-bot)
endif(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR STREQUAL "foot-bot")

if(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR STREQUAL "mini-quadrotor")
  add_subdirectory(mini-quadrotor)
endif(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR STREQUAL "mini-quadrotor")

if(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR STREQUAL "e-puck")
  add_subdirectory(e-puck)
endif(ARGOS_BUILD_FOR_SIMULATOR OR ARGOS_BUILD_FOR STREQUAL "e-puck")
