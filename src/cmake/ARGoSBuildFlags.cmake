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
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -undefined dynamic_lookup")
else(APPLE)
  # Linux
  # Avoid discarding unused symbols to allow plugins to work
  set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-as-needed")
endif(APPLE)

#
# This is required for TinyXML++ to work
#
add_definitions(-DTIXML_USE_TICPP)
