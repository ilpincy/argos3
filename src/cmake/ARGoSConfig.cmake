# Find ARGoS
#
# This module looks for ARGoS and its components. It works as follows:
#
#  find_package(ARGOS [REQUIRED] [QUIET] [COMPONENTS <plugins...>] [OPTIONAL_COMPONENTS <plugins...>])
#
# where:
#
#  - REQUIRED generates an error if the ARGoS core was not found, or if one of the plugins listed after PLUGINS was not found
#  - QUIET suppresses the search messages
#  - COMPONENTS is followed by a list of plugins required to declare ARGoS as found, such as "core", "entities", etc.
#  - OPTIONAL_COMPONENTS is a list of plugins that, if absent, do not result in ARGoS being considered "not found"
#
# This module defines these general variables:
#
#  ARGOS_FOUND        - True if ARGoS was found
#  ARGOS_INCLUDE_DIRS - The base folder where the ARGoS headers are installed
#  ARGOS_LIBRARY_DIR  - The base folder where the ARGoS libraries are installed
#  ARGOS_LDFLAGS      - The linker flags to compile ARGoS code
#
# It also defines plugin-specific variables:
#
#  ARGOS_${PLUGIN}_FOUND       - True if ARGoS was found
#  ARGOS_${PLUGIN}_INCLUDE_DIR - The base folder where the ARGoS headers are installed
#  ARGOS_${PLUGIN}_LIBRARY     - The base folder where the ARGoS libraries are installed
#
# AUTHOR: Carlo Pinciroli <ilpincy@gmail.com>

#
# Set the ARGOS_BUILD_FOR variable
#
if(NOT DEFINED ARGOS_BUILD_FOR OR ARGOS_BUILD_FOR STREQUAL "")
  set(ARGOS_BUILD_FOR "simulator" CACHE STRING "Target for which ARGOS is built [\"simulator\" or robot name]")
endif(NOT DEFINED ARGOS_BUILD_FOR OR ARGOS_BUILD_FOR STREQUAL "")

#
# Make sure "core" is not in the list
# It will be checked by itself
#
list(REMOVE_ITEM ARGoS_FIND_COMPONENTS "core")
if(${ARGoS_FIND_REQUIRED})
  set(_required REQUIRED)
endif(${ARGoS_FIND_REQUIRED})

#
# Check the ARGoS core
#
set(_lib "argos3core_${ARGOS_BUILD_FOR}")
find_path(ARGOS_INCLUDE_DIRS argos3/core/config.h
  HINTS ${CMAKE_INSTALL_PREFIX}/include
  DOC "Path of the headers needed for ARGoS"
  ${_required})
find_library(ARGOS_LIBRARY ${_lib}
  HINTS ${CMAKE_INSTALL_PREFIX}/lib
  PATH_SUFFIXES argos3
  ${_required})
if(ARGOS_INCLUDE_DIRS AND ARGOS_LIBRARY)
  set(ARGOS_FOUND 1)
  get_filename_component(ARGOS_LIBRARY_DIR ${ARGOS_LIBRARY} DIRECTORY CACHE)
  if(NOT ARGoS_FIND_QUIETLY)
    message(STATUS "Found ARGoS: ${ARGOS_LIBRARY}")
  endif(NOT ARGoS_FIND_QUIETLY)
else(ARGOS_INCLUDE_DIRS AND ARGOS_LIBRARY)
  set(ARGOS_FOUND 0)
  unset(ARGOS_INCLUDE_DIRS CACHE)
  if(NOT ARGoS_FIND_QUIETLY)
    message(STATUS "ARGoS NOT found")
  endif(NOT ARGoS_FIND_QUIETLY)
endif(ARGOS_INCLUDE_DIRS AND ARGOS_LIBRARY)
unset(ARGOS_LIBRARY)

#
# Check all the plugins
#
if(ARGOS_FOUND AND (NOT ARGoS_FIND_COMPONENTS STREQUAL ""))
  foreach(_plugin ${ARGoS_FIND_COMPONENTS})
    string(TOUPPER "${_plugin}" _PLUGIN)
    set(_lib "argos3plugin_${ARGOS_BUILD_FOR}_${_plugin}")
    unset(_required)
    find_library(ARGOS_${_PLUGIN}_LIBRARY ${_lib}
      HINTS ${CMAKE_INSTALL_PREFIX}/lib
      PATH_SUFFIXES argos3
      DOC "Path of the ARGoS ${_plugin} plugin library"
      ${_required})
    if(ARGOS_${_PLUGIN}_LIBRARY)
      if(NOT ARGoS_FIND_QUIETLY)
        message(STATUS "ARGoS plugin ${_plugin} found: ${ARGOS_${_PLUGIN}_LIBRARY}")
      endif(NOT ARGoS_FIND_QUIETLY)
    else(ARGOS_${_PLUGIN}_LIBRARY)
      if(NOT ARGoS_FIND_QUIETLY)
        message(STATUS "ARGoS plugin ${_plugin} NOT found")
      endif(NOT ARGoS_FIND_QUIETLY)
      if(ARGoS_FIND_REQUIRED_${_PLUGIN})
        set(ARGOS_FOUND 0)
      endif(ARGoS_FIND_REQUIRED_${_PLUGIN})
    endif(ARGOS_${_PLUGIN}_LIBRARY)
  endforeach()
endif(ARGOS_FOUND AND (NOT ARGoS_FIND_COMPONENTS STREQUAL ""))

#
# Find the ARGoS CMake files
#
find_path(ARGOS_CMAKE_DIR FindARGoSQTOpenGL.cmake
  PATHS /usr/share/argos3/cmake /usr/local/share/argos3/cmake /opt/share/argos3/cmake /opt/local/share/argos3/cmake
  HINTS ${CMAKE_INSTALL_PREFIX}/share/argos3/cmake
  DOC "Path of the ARGoS CMake modules"
  ${_required})
list(APPEND CMAKE_MODULE_PATH "${ARGOS_CMAKE_DIR}")
include(ARGoSSettings)

#
# Look for components
#
if(ARGOS_DYNAMIC_LIBRARY_LOADING)
  find_package(DLFCN REQUIRED)
  list(APPEND ARGOS_INCLUDE_DIRS "${DLFCN_INCLUDE_DIR}")
  list(APPEND ARGOS_LDFLAGS "${DLFCN_LIBRARY}")
endif(ARGOS_DYNAMIC_LIBRARY_LOADING)

if(ARGOS_BUILD_FOR_SIMULATOR)
  find_package(Pthreads REQUIRED)
  list(APPEND ARGOS_INCLUDE_DIRS "${PTHREADS_INCLUDE_DIR}")
  list(APPEND ARGOS_LDFLAGS "${PTHREADS_LIBRARY}")
  
  if(ARGOS_WITH_FREEIMAGE)
    find_package(FreeImage REQUIRED)
    list(APPEND ARGOS_INCLUDE_DIRS "${FREEIMAGE_BASE_INCLUDE_DIR}" "${FREEIMAGE_PLUS_INCLUDE_DIR}")
    list(APPEND ARGOS_LDFLAGS "${FREEIMAGE_BASE_LIBRARY}" "${FREEIMAGE_PLUS_LIBRARY}")
  endif(ARGOS_WITH_FREEIMAGE)

  find_package(ARGoSQTOpenGL)
  list(APPEND ARGOS_INCLUDE_DIRS "${ARGOS_QTOPENGL_INCLUDE_DIRS}")
  list(APPEND ARGOS_LDFLAGS "${ARGOS_QTOPENGL_LIBRARIES}")
endif(ARGOS_BUILD_FOR_SIMULATOR)

if(ARGOS_WITH_LUA)
  find_package(Lua REQUIRED)
  if(${LUA_VERSION_STRING} VERSION_GREATER_EQUAL "5.3")
    list(APPEND ARGOS_INCLUDE_DIRS "${LUA_INCLUDE_DIR}")
    list(APPEND ARGOS_LDFLAGS "${LUA_LIBRARIES}")
  else(${LUA_VERSION_STRING} VERSION_GREATER_EQUAL "5.3")
    message(FATAL_ERROR "Lua >=5.3 not found")
  endif(${LUA_VERSION_STRING} VERSION_GREATER_EQUAL "5.3")
endif(ARGOS_WITH_LUA)

list(REMOVE_DUPLICATES ARGOS_INCLUDE_DIRS)
set(ARGOS_INCLUDE_DIRS "${ARGOS_INCLUDE_DIRS}" CACHE STRING "Path of the headers needed for ARGoS" FORCE)
set(ARGOS_LDFLAGS      "${ARGOS_LDFLAGS}"      CACHE STRING "The linker flags to compile ARGoS code" FORCE)
