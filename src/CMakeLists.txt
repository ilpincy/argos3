#
# Set minimum required version
#
cmake_minimum_required(VERSION 2.8)

#
# Set the path additional cmake files must be searched for
#
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

#
# Include path points to the base source dir
#
include_directories(${CMAKE_SOURCE_DIR})

#
# Make sure we are under Unix
#
if(NOT UNIX)
  message(FATAL_ERROR "ARGoS compiles only under UNIX, sorry!")
endif(NOT UNIX)

#
# Set build options
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildOptions.cmake)

#
# Set compiler flags
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildFlags.cmake)

#
# Check for libraries
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildChecks.cmake)

#
# Set up CPack for later use
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSPackaging.cmake)

#
# Compile stuff
#
add_subdirectory(core)
add_subdirectory(plugins)
if(ARGOS_WRAPPERS)
  add_subdirectory(wrappers)
endif(ARGOS_WRAPPERS)

#
# Create documentation
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSCreateDocs.cmake)
