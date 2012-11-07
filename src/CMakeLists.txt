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
if(ARGOS_WRAPPERS)
  add_subdirectory(wrappers)
endif(ARGOS_WRAPPERS)

#
# Create doxygen documentation, if asked for
#
if(ARGOS_DOCUMENTATION)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    if(DOXYGEN_DOT_FOUND)
      configure_file(${CMAKE_SOURCE_DIR}/../doc/Doxyfile.in
        ${CMAKE_BINARY_DIR}/Doxyfile
        @ONLY)
      add_custom_target(doc
        COMMAND
        ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile)
      install(DIRECTORY ${CMAKE_SOURCE_DIR}/../doc/api DESTINATION doc/argos3)
    else(DOXYGEN_DOT_FOUND)
      message(FATAL_ERROR "GraphViz/Dot not found, and it is required to build the API documentation")
    endif(DOXYGEN_DOT_FOUND)
  else(DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen not found, and it is required to build the API documentation")
  endif(DOXYGEN_FOUND)
endif(ARGOS_DOCUMENTATION)
