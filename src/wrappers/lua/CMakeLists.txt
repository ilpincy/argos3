#
# Look for Lua and create the wrapper
#
find_package(Lua)
if(LUA_FOUND)
  include_directories(${LUA_INCLUDE_DIR})
  set_source_files_properties(
    control_interface.i
    simulator.i
    PROPERTIES CPLUSPLUS ON)
  swig_add_module(argos3core_${ARGOS_BUILD_FOR}_lua lua
    control_interface.i
    simulator.i)
  swig_link_libraries(argos3core_${ARGOS_BUILD_FOR}_lua ${LUA_LIBRARIES})
  set_target_properties(argos3core_${ARGOS_BUILD_FOR}_lua
    PROPERTIES
    LINKER_LANGUAGE CXX)
endif(LUA_FOUND)
