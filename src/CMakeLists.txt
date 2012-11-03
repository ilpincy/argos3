#
# Set minimum required version
#
cmake_minimum_required(VERSION 2.8)

#
# Set the path additional cmake files must be searched for
#
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

#
# Make sure we are under Unix
#
if(NOT UNIX)
  message(FATAL_ERROR "ARGoS compiles only under UNIX, sorry!")
endif(NOT UNIX)

#
# Check for GCC version >= 4.2
#
execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
if (GCC_VERSION VERSION_GREATER 4.2 OR GCC_VERSION VERSION_EQUAL 4.2)
  message(STATUS "GCC/G++ version >= 4.2")
else(GCC_VERSION VERSION_GREATER 4.2 OR GCC_VERSION VERSION_EQUAL 4.2)
  message(FATAL_ERROR "You need to have at least GCC/G++ version 4.2!")
endif(GCC_VERSION VERSION_GREATER 4.2 OR GCC_VERSION VERSION_EQUAL 4.2)

#
# Set compiler flags
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildFlags.cmake)

#
# Set build options
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildOptions.cmake)

#
# Check for libraries
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildChecks.cmake)

#
# Compile stuff
#
add_subdirectory(core)
#add_subdirectory(plugins)
