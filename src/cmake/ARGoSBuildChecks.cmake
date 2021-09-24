#
# Check for correct GCC version
# On Linux it must be >= 4.3
#
macro(CHECK_GCC MIN_VERSION)
  execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
  if(GCC_VERSION VERSION_GREATER ${MIN_VERSION} OR GCC_VERSION VERSION_EQUAL ${MIN_VERSION})
    message(STATUS "GCC/G++ version >= ${MIN_VERSION}")
  else(GCC_VERSION VERSION_GREATER ${MIN_VERSION} OR GCC_VERSION VERSION_EQUAL ${MIN_VERSION})
    message(FATAL_ERROR "You need to have at least GCC/G++ version ${MIN_VERSION}!")
  endif(GCC_VERSION VERSION_GREATER ${MIN_VERSION} OR GCC_VERSION VERSION_EQUAL ${MIN_VERSION})
endmacro(CHECK_GCC)

#
# Set variables depending on current compiler
#
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # using Clang
  set(ARGOS_START_LIB_GROUP)
  set(ARGOS_END_LIB_GROUP)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  set(ARGOS_START_LIB_GROUP -Wl,--start-group)
  set(ARGOS_END_LIB_GROUP -Wl,--end-group)

  if(APPLE)
    # On Apple it must be >= 4.2.1
    check_gcc(4.2.1)
  else(APPLE)
    # On Linux it must be >= 4.3
    check_gcc(4.3)
  endif(APPLE)
endif()

#
# Check for dynamic library loading facility
#
if(ARGOS_DYNAMIC_LIBRARY_LOADING)
  if(NOT DLFCN_INCLUDE_DIR)
    find_package(DLFCN)
    if(NOT DLFCN_FOUND)
      message(FATAL_ERROR "Required library dl not found.")
    endif(NOT DLFCN_FOUND)
  endif(NOT DLFCN_INCLUDE_DIR)
  include_directories(AFTER ${DLFCN_INCLUDE_DIR})
endif(ARGOS_DYNAMIC_LIBRARY_LOADING)

#
# Check for PThreads
# It is required only when compiling the simulator
#
if(ARGOS_BUILD_FOR_SIMULATOR)
  if(NOT PTHREADS_INCLUDE_DIR)
    find_package(Pthreads)
    if(NOT PTHREADS_FOUND)
      message(FATAL_ERROR "Required library pthreads not found.")
    endif(NOT PTHREADS_FOUND)
  endif(NOT PTHREADS_INCLUDE_DIR)
  add_definitions(${PTHREADS_DEFINITIONS})
  include_directories(AFTER ${PTHREADS_INCLUDE_DIR})
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Check for FreeImage
# It is required only when compiling the simulator
#
if(ARGOS_BUILD_FOR_SIMULATOR)
  find_package(FreeImage)
  if(FREEIMAGE_FOUND)
    set(ARGOS_WITH_FREEIMAGE ON)
    include_directories(AFTER ${FREEIMAGE_BASE_INCLUDE_DIR} ${FREEIMAGE_PLUS_INCLUDE_DIR})
  else(FREEIMAGE_FOUND)
    message(STATUS "FreeImage not found, image source and OpenGL drawing for the floor entity won't be available")
  endif(FREEIMAGE_FOUND)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Check for Google Perftools
#
find_package(GooglePerfTools)

#
# Check for Doxygen
#
set(ARGOS_BUILD_API OFF)
if(ARGOS_DOCUMENTATION)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    if(DOXYGEN_DOT_FOUND)
      set(ARGOS_BUILD_API ON)
    else(DOXYGEN_DOT_FOUND)
      message(WARNING "GraphViz/Dot not found, API documentation won't be generated")
    endif(DOXYGEN_DOT_FOUND)
  else(DOXYGEN_FOUND)
    message(WARNING "Doxygen not found, API documentation won't be generated")
  endif(DOXYGEN_FOUND)
endif(ARGOS_DOCUMENTATION)

#
# Check for ASCIIDoc
#
find_package(ASCIIDoc)

#
# Check whether Qt and OpenGL are installed
#
if(ARGOS_BUILD_FOR_SIMULATOR)
  find_package(ARGoSQTOpenGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Check for Lua 5.3
#
find_package(Lua)
if(LUA_FOUND)
  if(${LUA_VERSION_STRING} VERSION_GREATER_EQUAL "5.3")
    set(ARGOS_WITH_LUA ON)
    include_directories(AFTER ${LUA_INCLUDE_DIR})
  else()
    message(STATUS "Lua >=5.3 not found")
  endif()
else(LUA_FOUND)
  message(STATUS "Lua >=5.3 not found")
endif(LUA_FOUND)

