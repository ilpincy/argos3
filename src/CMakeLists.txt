#
# Set minimum required version
#
cmake_minimum_required(VERSION 3.5.1)

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
mark_as_advanced(ARGOS_BUILD_FOR)
# Set a macro according to value set in ARGOS_BUILD_FOR
add_definitions(-DARGOS_${ARGOS_BUILD_FOR}_BUILD)
# Create a convenience variable for checks in the CMake files
if(ARGOS_BUILD_FOR STREQUAL "simulator")
  set(ARGOS_BUILD_FOR_SIMULATOR TRUE)
else(ARGOS_BUILD_FOR STREQUAL "simulator")
  set(ARGOS_BUILD_FOR_SIMULATOR FALSE)
endif(ARGOS_BUILD_FOR STREQUAL "simulator")

#
# Project name
#
project(argos3_${ARGOS_BUILD_FOR})

#
# Set build options
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSBuildOptions.cmake)

#
# Make sure we are under Unix
#
if(NOT UNIX)
  message(FATAL_ERROR "ARGoS compiles only under UNIX, sorry!")
endif(NOT UNIX)

#
# Set the path additional cmake files must be searched for
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_SOURCE_DIR}/cmake)

#
# Set CMake policies to select wanted behaviors
#
# Use new policies introduced up to this version
if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif(POLICY CMP0042)
set(OpenGL_GL_PREFERENCE "LEGACY")

#
# Include path points to the base source dir
#
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

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
# Create config file with settings that must be accessed from C++
#
configure_file(
  ${CMAKE_SOURCE_DIR}/core/config.h.in
  ${CMAKE_BINARY_DIR}/core/config.h
  @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/core/config.h DESTINATION include/argos3/core)

#
# Create a soft link 'argos3' pointing to the build directory
# It is necessary for the config.h file to be found by the compiler
#
if(NOT EXISTS ${CMAKE_BINARY_DIR}/argos3)
  execute_process(
    COMMAND ln -s . argos3
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif(NOT EXISTS ${CMAKE_BINARY_DIR}/argos3)

#
# This variable will accumulate the paths for all enabled plugins. It
# is used for testing and for creating the setup_env.sh and .gdbinit
# files
#
set(ARGOS_PLUGIN_PATH "${CMAKE_BINARY_DIR}/core" CACHE INTERNAL "ARGoS plugin path")

#
# Compile stuff
#
add_subdirectory(core)
add_subdirectory(plugins)

#
# Enable testing if we are building the simulator
#
if(ARGOS_BUILD_FOR_SIMULATOR)
  enable_testing()
  add_subdirectory(testing)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create documentation
#
include(${CMAKE_SOURCE_DIR}/cmake/ARGoSCreateDocs.cmake)

#
# Create .gdbinit file for GDB support
# It is placed in the same directory as the argos3 executable
#
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/gdbinit.in
  ${CMAKE_BINARY_DIR}/core/.gdbinit
  @ONLY)

#
# Create setup_envsh file for bash support
# It is placed in the base build directory
#
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/setup_env.sh.in
  ${CMAKE_BINARY_DIR}/setup_env.sh
  @ONLY)

#
# Create CMake configuration file
#
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/argos3_cmake_config.cmake.in
  ${CMAKE_BINARY_DIR}/scripts/ARGoSSettings.cmake
  @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/scripts/ARGoSSettings.cmake DESTINATION share/argos3/cmake)

  #
  # Installation of CMake scripts useful for the simulator
  #
  install(
    FILES
    ${CMAKE_SOURCE_DIR}/cmake/ARGoSConfig.cmake
    ${CMAKE_SOURCE_DIR}/cmake/FindARGoSQTOpenGL.cmake
    ${CMAKE_SOURCE_DIR}/cmake/FindDLFCN.cmake
    ${CMAKE_SOURCE_DIR}/cmake/FindFreeImage.cmake
    ${CMAKE_SOURCE_DIR}/cmake/FindGooglePerfTools.cmake
    ${CMAKE_SOURCE_DIR}/cmake/FindPthreads.cmake
    DESTINATION
    share/argos3/cmake)

#
# Install extra stuff
#
if(ARGOS_INSTALL_LDSOCONF)
  # Configuration file for /etc/ld.so.conf.d/
  configure_file(
    ${CMAKE_SOURCE_DIR}/scripts/argos3.conf.in
    ${CMAKE_BINARY_DIR}/argos3.conf
    @ONLY)
  install(FILES ${CMAKE_BINARY_DIR}/argos3.conf DESTINATION /etc/ld.so.conf.d)
endif(ARGOS_INSTALL_LDSOCONF)
# Bash completion file
install(FILES ${CMAKE_SOURCE_DIR}/scripts/argos3_bash_completion.sh DESTINATION share/argos3)
