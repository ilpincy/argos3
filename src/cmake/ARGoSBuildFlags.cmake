#
# Set the compilation type
#
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ..." FORCE)
endif(CMAKE_BUILD_TYPE STREQUAL "")

#
# Get information about the current processor
#
execute_process(
  COMMAND uname -m
  COMMAND tr -d '\n'
  OUTPUT_VARIABLE ARGOS_PROCESSOR_ARCH)

#
# Select ISO C++17
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# General compilation flags
#
set(CMAKE_C_FLAGS                  "${CMAKE_C_FLAGS} -Wall -Wno-unknown-pragmas")
if(ARGOS_BUILD_NATIVE)
  set(CMAKE_C_FLAGS                "${CMAKE_C_FLAGS} -mtune=native -march=native")
endif(ARGOS_BUILD_NATIVE)
set(CMAKE_C_FLAGS_RELEASE          "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_C_FLAGS_DEBUG            "${CMAKE_C_FLAGS_DEBUG} -ggdb3")
set(CMAKE_C_FLAGS_RELWITHDEBINFO   "${CMAKE_C_FLAGS_RELWITHDEBINFO} ${CMAKE_C_FLAGS_DEBUG} ${CMAKE_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas")
if(ARGOS_BUILD_NATIVE)
  set(CMAKE_CXX_FLAGS              "${CMAKE_CXX_FLAGS} -mtune=native -march=native")
endif(ARGOS_BUILD_NATIVE)
set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_DEBUG}")

if(APPLE)
  # MAC OSX
  set(ARGOS_SHARED_LIBRARY_EXTENSION "dylib")
  set(ARGOS_MODULE_LIBRARY_EXTENSION "so")
  set(ARGOS_DYNAMIC_LIBRARY_VARIABLE "DYLD_LIBRARY_PATH")
  # Add address sanitizer support for CLang
  set(CMAKE_C_FLAGS_DEBUG            "${CMAKE_C_FLAGS_DEBUG} -fsanitize=address -fno-optimize-sibling-calls -fno-omit-frame-pointer")
  set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-optimize-sibling-calls -fno-omit-frame-pointer")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO   "${CMAKE_C_FLAGS_RELWITHDEBINFO} -fsanitize=address -fno-optimize-sibling-calls -fno-omit-frame-pointer")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -fsanitize=address -fno-optimize-sibling-calls -fno-omit-frame-pointer")
  # Allow for dynamic lookup of undefined symbols
  set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} -undefined dynamic_lookup")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
  # Add address sanitizer support for CLang
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG    "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
  set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -fsanitize=address")
  set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} -fsanitize=address")
else(APPLE)
  # Linux
  # Avoid discarding unused symbols to allow plugins to work
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-as-needed")
  set(ARGOS_SHARED_LIBRARY_EXTENSION "so")
  set(ARGOS_MODULE_LIBRARY_EXTENSION "so")
  set(ARGOS_DYNAMIC_LIBRARY_VARIABLE "LD_LIBRARY_PATH")
endif(APPLE)

# Hide flags
mark_as_advanced(CMAKE_C_FLAGS)
mark_as_advanced(CMAKE_C_FLAGS_DEBUG)
mark_as_advanced(CMAKE_C_FLAGS_RELEASE)
mark_as_advanced(CMAKE_C_FLAGS_RELWITHDEBINFO)
mark_as_advanced(CMAKE_CXX_FLAGS)
mark_as_advanced(CMAKE_CXX_FLAGS_DEBUG)
mark_as_advanced(CMAKE_CXX_FLAGS_RELEASE)
mark_as_advanced(CMAKE_CXX_FLAGS_RELWITHDEBINFO)
mark_as_advanced(CMAKE_EXE_LINKER_FLAGS)
mark_as_advanced(CMAKE_EXE_LINKER_FLAGS_DEBUG)
mark_as_advanced(CMAKE_EXE_LINKER_FLAGS_RELEASE)
mark_as_advanced(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO)
mark_as_advanced(CMAKE_SHARED_LINKER_FLAGS)
mark_as_advanced(CMAKE_SHARED_LINKER_FLAGS_DEBUG)
mark_as_advanced(CMAKE_SHARED_LINKER_FLAGS_RELEASE)
mark_as_advanced(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO)
mark_as_advanced(CMAKE_MODULE_LINKER_FLAGS)
mark_as_advanced(CMAKE_MODULE_LINKER_FLAGS_DEBUG)
mark_as_advanced(CMAKE_MODULE_LINKER_FLAGS_RELEASE)
mark_as_advanced(CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO)
