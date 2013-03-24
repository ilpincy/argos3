#
# General compilation flags
#
set(CMAKE_C_FLAGS                "-Wall")
set(CMAKE_C_FLAGS_RELEASE        "-Os -DNDEBUG")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-Os -ggdb3 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG          "-ggdb3")

set(CMAKE_CXX_FLAGS                "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE        "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -ggdb3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG          "-ggdb3")

if(APPLE)
  # MAC OSX
  # Allow for dynamic lookup of undefined symbols
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -undefined dynamic_lookup")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
  set(ARGOS_DYNAMIC_LIBRARY_EXTENSION "dylib")
else(APPLE)
  # Linux
  # Avoid discarding unused symbols to allow plugins to work
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-as-needed")
  set(ARGOS_DYNAMIC_LIBRARY_EXTENSION "so")
endif(APPLE)
