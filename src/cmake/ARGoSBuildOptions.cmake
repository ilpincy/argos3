#
# What is ARGoS being built for?
# Accepted values: "SIMULATOR" or a robot name (uppercase)
#
if(NOT DEFINED ARGOS_BUILD_FOR)
  # Variable was not set, set to default value
  set(ARGOS_BUILD_FOR "SIMULATOR" CACHE STRING "What is ARGoS being built for? \"Simulator\" or a robot name (uppercase)")
endif(NOT DEFINED ARGOS_BUILD_FOR)
# Set the macro according to value set in ARGOS_BUILD_FOR
add_definitions(-DARGOS_${ARGOS_BUILD_FOR}_BUILD)

#
# Compile thread-safe or thread-unsafe log
# By default, use the thread-safe version
# When compiling for the simulator, the thread-safe version is mandatory
#
if(NOT DEFINED ARGOS_THREADSAFE_LOG)
  set(ARGOS_THREADSAFE_LOG TRUE CACHE BOOL "ON -> compile thread-safe version of log, OFF -> compile thread-unsafe version of log")
endif(NOT DEFINED ARGOS_THREADSAFE_LOG)
if((NOT ARGOS_THREADSAFE_LOG) AND (ARGOS_BUILD_FOR STREQUAL "SIMULATOR"))
  message(FATAL_ERROR "When compiling for the simulator, ARGOS_THREADSAFE_LOG must be ON")
endif((NOT ARGOS_THREADSAFE_LOG) AND (ARGOS_BUILD_FOR STREQUAL "SIMULATOR"))
if(ARGOS_THREADSAFE_LOG)
  add_definitions(-DARGOS_THREADSAFE_LOG)
endif(ARGOS_THREADSAFE_LOG)

#
# Compile support for dynamic library loading or not
# By default, support for dynamic library loading is on
# When compiling for the simulator, support for dynamic library loading must be on
#
if(NOT DEFINED ARGOS_DYNAMIC_LIBRARY_LOADING)
  set(ARGOS_DYNAMIC_LIBRARY_LOADING TRUE CACHE BOOL "ON -> compile support for dynamic library loading, OFF -> no support for dynamic library loading")
endif(NOT DEFINED ARGOS_DYNAMIC_LIBRARY_LOADING)
if((NOT ARGOS_DYNAMIC_LIBRARY_LOADING) AND (ARGOS_BUILD_FOR STREQUAL "SIMULATOR"))
  message(FATAL_ERROR "When compiling for the simulator, ARGOS_DYNAMIC_LIBRARY_LOADING must be ON")
endif((NOT ARGOS_DYNAMIC_LIBRARY_LOADING) AND (ARGOS_BUILD_FOR STREQUAL "SIMULATOR"))
if(ARGOS_DYNAMIC_LIBRARY_LOADING)
  add_definitions(-DARGOS_DYNAMIC_LIBRARY_LOADING)
endif(ARGOS_DYNAMIC_LIBRARY_LOADING)
