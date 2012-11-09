#
# What is ARGoS being built for?
# Accepted values: "simulator" or a robot name (lowercase)
#
if(NOT DEFINED ARGOS_BUILD_FOR)
  # Variable was not set, set to default value
  set(ARGOS_BUILD_FOR "simulator" CACHE STRING "What is ARGoS being built for? \"simulator\" or a robot name (lowercase)")
else(NOT DEFINED ARGOS_BUILD_FOR)
  # Variable was set, make it public
  set(ARGOS_BUILD_FOR ${ARGOS_BUILD_FOR} CACHE STRING "What is ARGoS being built for? \"simulator\" or a robot name (lowercase)")
endif(NOT DEFINED ARGOS_BUILD_FOR)
# Set a macro according to value set in ARGOS_BUILD_FOR
add_definitions(-DARGOS_${ARGOS_BUILD_FOR}_BUILD)
# Create a convenience variable for checks in the CMake files
if(ARGOS_BUILD_FOR STREQUAL "simulator")
  set(ARGOS_BUILD_FOR_SIMULATOR TRUE)
else(ARGOS_BUILD_FOR STREQUAL "simulator")
  set(ARGOS_BUILD_FOR_SIMULATOR FALSE)
endif(ARGOS_BUILD_FOR STREQUAL "simulator")

#
# Compile thread-safe or thread-unsafe log
# By default, use the thread-safe version
# When compiling for the simulator, the thread-safe version is mandatory
#
option(ARGOS_THREADSAFE_LOG "ON -> compile thread-safe version of log, OFF -> compile thread-unsafe version of log" ON)
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
option(ARGOS_DYNAMIC_LIBRARY_LOADING "ON -> compile support for dynamic library loading, OFF -> no support for dynamic library loading" ON)
if((NOT ARGOS_DYNAMIC_LIBRARY_LOADING) AND (ARGOS_BUILD_FOR STREQUAL "SIMULATOR"))
  message(FATAL_ERROR "When compiling for the simulator, ARGOS_DYNAMIC_LIBRARY_LOADING must be ON")
endif((NOT ARGOS_DYNAMIC_LIBRARY_LOADING) AND (ARGOS_BUILD_FOR STREQUAL "SIMULATOR"))

#
# Compile Doxygen-based documentation
# This requires SWIG to be installed
#
option(ARGOS_WRAPPERS "ON -> create wrappers, OFF -> don't create wrappers" OFF)

#
# Whether to use double or float for the Real type
#
option(ARGOS_USE_DOUBLE "ON -> use double for Real, OFF -> use float for Real" ON)

#
# Compile Doxygen-based documentation
# This requires GraphViz/Dot to be installed
#
option(ARGOS_DOCUMENTATION "ON -> compile documentation, OFF -> dont'compile documentation" ON)
