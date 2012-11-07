#
# Look for Lua and create the wrapper
#
find_package(Lua)
if(LUA_FOUND)
  include_directories(${LUA_INCLUDE_DIR})
  swig_add_module(argos3core_${ARGOS_BUILD_FOR}_lua lua ${ARGOS3_SOURCES})
  swig_link_libraries(argos3core_${ARGOS_BUILD_FOR}_lua ${LUA_LIBRARIES})
  set_target_properties(argos3core_${ARGOS_BUILD_FOR}_lua
    PROPERTIES
    LINKER_LANGUAGE CXX)
endif(LUA_FOUND)
