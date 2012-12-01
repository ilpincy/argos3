#
# Get information about the current processor
#
execute_process(
  COMMAND uname -m
  COMMAND tr -d '\n'
  OUTPUT_VARIABLE ARGOS_PROCESSOR_ARCH)

#
# General compilation flags
#
set(CMAKE_C_FLAGS                "-Wall")
set(CMAKE_C_FLAGS_RELEASE        "-Os -DNDEBUG -mtune=${ARGOS_PROCESSOR_ARCH}")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Os -ggdb3 -DNDEBUG -mtune=${ARGOS_PROCESSOR_ARCH}")
set(CMAKE_C_FLAGS_DEBUG          "-ggdb3")

set(CMAKE_CXX_FLAGS                "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE        "-Os -DNDEBUG -mtune=${ARGOS_PROCESSOR_ARCH}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -ggdb3 -DNDEBUG -mtune=${ARGOS_PROCESSOR_ARCH}")
set(CMAKE_CXX_FLAGS_DEBUG          "-ggdb3")

if(APPLE)
  # MAC OSX
  # Allow for dynamic lookup of undefined symbols
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -undefined dynamic_lookup")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
else(APPLE)
  # Linux
  # Avoid discarding unused symbols to allow plugins to work
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-as-needed")
endif(APPLE)

#
# Use double or float for Real?
#
if(ARGOS_USE_DOUBLE)
  add_definitions(-DARGOS_DOUBLE_PRECISION)
endif(ARGOS_USE_DOUBLE)

#
# Pass the install prefix to source
#
add_definitions(-DINSTALL_PREFIX="${CMAKE_INSTALL_PREFIX}")