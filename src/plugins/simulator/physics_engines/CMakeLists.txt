set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES
  physics_box_model.h
  physics_cylinder_model.h)

add_subdirectory(dynamics2d)
add_subdirectory(dynamics3d)
add_subdirectory(pointmass3d)

install(FILES
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_PHYSICS_ENGINES}
  DESTINATION
  include/argos3/plugins/simulator/physics_engines)
