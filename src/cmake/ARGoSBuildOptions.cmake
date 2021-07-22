#
# Install ld.so.conf?
#
if(NOT DEFINED ARGOS_INSTALL_LDSOCONF)
  if(APPLE)
    option(ARGOS_INSTALL_LDSOCONF "ON -> install argos3.conf in /etc/ld.so.conf/, OFF -> don't" OFF)
  else(APPLE)
    option(ARGOS_INSTALL_LDSOCONF "ON -> install argos3.conf in /etc/ld.so.conf/, OFF -> don't" ON)
  endif(APPLE)
  mark_as_advanced(ARGOS_INSTALL_LDSOCONF)
endif(NOT DEFINED ARGOS_INSTALL_LDSOCONF)

#
# Optimize code for current platform?
#
if(NOT DEFINED ARGOS_BUILD_NATIVE)
  option(ARGOS_BUILD_NATIVE "ON -> compile with platform-specific optimizations, OFF -> compile to portable binary" OFF)
endif(NOT DEFINED ARGOS_BUILD_NATIVE)

#
# Compile thread-safe or thread-unsafe log
# By default, use the thread-safe version
# When compiling for the simulator, the thread-safe version is mandatory
#
if(NOT DEFINED ARGOS_THREADSAFE_LOG)
  option(ARGOS_THREADSAFE_LOG "ON -> compile thread-safe version of log, OFF -> compile thread-unsafe version of log" ON)
endif(NOT DEFINED ARGOS_THREADSAFE_LOG)
if((NOT ARGOS_THREADSAFE_LOG) AND (ARGOS_BUILD_FOR STREQUAL "SIMULATOR"))
  message(FATAL_ERROR "When compiling for the simulator, ARGOS_THREADSAFE_LOG must be ON")
endif((NOT ARGOS_THREADSAFE_LOG) AND (ARGOS_BUILD_FOR STREQUAL "SIMULATOR"))

#
# Compile support for dynamic library loading or not
# By default, support for dynamic library loading is on
# When compiling for the simulator, support for dynamic library loading must be on
#
if(NOT DEFINED ARGOS_DYNAMIC_LIBRARY_LOADING)
  option(ARGOS_DYNAMIC_LIBRARY_LOADING "ON -> compile support for dynamic library loading, OFF -> no support for dynamic library loading" ON)
  mark_as_advanced(ARGOS_DYNAMIC_LIBRARY_LOADING)
endif(NOT DEFINED ARGOS_DYNAMIC_LIBRARY_LOADING)
if((NOT ARGOS_DYNAMIC_LIBRARY_LOADING) AND (ARGOS_BUILD_FOR STREQUAL "simulator"))
  message(FATAL_ERROR "When compiling for the simulator, ARGOS_DYNAMIC_LIBRARY_LOADING must be ON")
endif((NOT ARGOS_DYNAMIC_LIBRARY_LOADING) AND (ARGOS_BUILD_FOR STREQUAL "simulator"))

#
# Whether to use double or float for the Real type
#
if(NOT DEFINED ARGOS_USE_DOUBLE)
  option(ARGOS_USE_DOUBLE "ON -> use double for Real, OFF -> use float for Real" ON)
endif(NOT DEFINED ARGOS_USE_DOUBLE)

#
# Compile documentation
#
if(NOT DEFINED ARGOS_DOCUMENTATION)
  option(ARGOS_DOCUMENTATION "ON -> compile documentation, OFF -> dont'compile documentation" ON)
endif(NOT DEFINED ARGOS_DOCUMENTATION)
