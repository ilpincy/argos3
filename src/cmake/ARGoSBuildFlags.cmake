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
set(CMAKE_C_FLAGS                  "-Wall")
if(ARGOS_BUILD_NATIVE)
  set(CMAKE_C_FLAGS                "${CMAKE_C_FLAGS} -mtune=native -march=native")
endif(ARGOS_BUILD_NATIVE)
set(CMAKE_C_FLAGS_RELEASE          "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-Os -ggdb3 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG            "-ggdb3")
set(CMAKE_CXX_FLAGS                "-Wall")
if(ARGOS_BUILD_NATIVE)
  set(CMAKE_CXX_FLAGS              "${CMAKE_CXX_FLAGS} -mtune=native -march=native")
endif(ARGOS_BUILD_NATIVE)
if(APPLE)
  set(CMAKE_CXX_FLAGS              "${CMAKE_C_FLAGS}")
endif(APPLE)
set(CMAKE_CXX_FLAGS_RELEASE        "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -ggdb3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG          "-ggdb3")

# Get rid of annoying warnings
add_definitions(-Wno-unknown-pragmas)

if(APPLE)
  # MAC OSX
  # Allow for dynamic lookup of undefined symbols
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -undefined dynamic_lookup")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
  set(ARGOS_SHARED_LIBRARY_EXTENSION "dylib")
  set(ARGOS_MODULE_LIBRARY_EXTENSION "so")
  set(ARGOS_DYNAMIC_LIBRARY_VARIABLE "DYLD_LIBRARY_PATH")
else(APPLE)
  # Linux
  #
  # Align doubles for higher performance
  # Also: required by the PhysX engine
  #set(ARGOS_PC_CFLAGS -malign-double)
  add_definitions(${ARGOS_PC_CFLAGS})
  # Avoid discarding unused symbols to allow plugins to work
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-as-needed")
  set(ARGOS_SHARED_LIBRARY_EXTENSION "so")
  set(ARGOS_MODULE_LIBRARY_EXTENSION "so")
  set(ARGOS_DYNAMIC_LIBRARY_VARIABLE "LD_LIBRARY_PATH")
endif(APPLE)
