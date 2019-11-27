#
# Set minimum required version
#
cmake_minimum_required(VERSION 3.5.1)

#
# Set the path additional cmake files must be searched for
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

#
# Set CMake policies to select wanted behaviors
#
# Use new policies introduced up to this version
cmake_policy(VERSION 2.8.12)
if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif(POLICY CMP0042)

#
# Include path points to the base source dir
#
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})

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
# Compile stuff
#
add_subdirectory(core)
add_subdirectory(plugins)
if(ARGOS_BUILD_FOR_SIMULATOR)
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
# Configuration file for pkg-config
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/argos3.pc.in
  ${CMAKE_BINARY_DIR}/argos3_${ARGOS_BUILD_FOR}.pc
  @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/argos3_${ARGOS_BUILD_FOR}.pc DESTINATION lib/pkgconfig)
# Bash completion file
install(FILES ${CMAKE_SOURCE_DIR}/scripts/argos3_bash_completion.sh DESTINATION share/argos3)
