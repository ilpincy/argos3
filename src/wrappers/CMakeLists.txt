#
# Project name
#
project(argos3_wrappers)

#
# Create SWIG-based wrappers
# For this, SWIG is required
#
find_package(SWIG 1.3)
if(NOT SWIG_FOUND)
  message(FATAL_ERROR "SWIG >= 1.3 not found, and it is required for the creation of wrappers")
else(NOT SWIG_FOUND)
  # Include the necessary definitions
  include(${SWIG_USE_FILE})
  set(CMAKE_SWIG_FLAGS "-c++" "-Wall" "-includeall" "-I${CMAKE_SOURCE_DIR}")
  # Get list of source files
  file(GLOB_RECURSE ARGOS3_SOURCES ${CMAKE_SOURCE_DIR}/core/*.h)
  # Create the lua wrapper library
  add_subdirectory(lua)
endif(NOT SWIG_FOUND)
