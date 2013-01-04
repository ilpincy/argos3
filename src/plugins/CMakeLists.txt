#
# Compile default simulator plugins
#
if(ARGOS_BUILD_FOR_SIMULATOR)
  add_subdirectory(simulator)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Compile robot plugins
#
add_subdirectory(robots)
