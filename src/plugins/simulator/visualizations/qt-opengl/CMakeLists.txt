#
# Headers
#
# argos3/plugins/simulator/visualizations/qt-opengl
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL
  qtopengl_application.h
  qtopengl_box.h
  qtopengl_camera.h
  qtopengl_cylinder.h
  qtopengl_light.h
  qtopengl_log_stream.h
  qtopengl_main_window.h
  qtopengl_obj_model.h
  qtopengl_render.h
  qtopengl_user_functions.h
  qtopengl_widget.h)
if(ARGOS_WITH_LUA)
  set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL
    ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL}
    qtopengl_lua_editor.h
    qtopengl_lua_find_dialog.h
    qtopengl_lua_main_window.h
    qtopengl_lua_syntax_highlighter.h
    qtopengl_lua_statetree_item.h
    qtopengl_lua_statetree_model.h)
endif(ARGOS_WITH_LUA)

#
# Source files
#
# Common for all builds
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL}
  qtopengl_application.cpp
  qtopengl_box.cpp
  qtopengl_camera.cpp
  qtopengl_cylinder.cpp
  qtopengl_light.cpp
  qtopengl_main_window.cpp
  qtopengl_obj_model.cpp
  qtopengl_render.cpp
  qtopengl_user_functions.cpp
  qtopengl_widget.cpp)
if(ARGOS_WITH_LUA)
  set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL
    ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL}
    qtopengl_lua_editor.cpp
    qtopengl_lua_find_dialog.cpp
    qtopengl_lua_main_window.cpp
    qtopengl_lua_syntax_highlighter.cpp
    qtopengl_lua_statetree_item.cpp
    qtopengl_lua_statetree_model.cpp)
endif(ARGOS_WITH_LUA)

#
# Create Qt-OpenGL visualization
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_qtopengl SHARED
  ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_qtopengl argos3plugin_${ARGOS_BUILD_FOR}_entities ${ARGOS_QTOPENGL_LIBRARIES})
# Link SDL if necessary
# if(SDL_FOUND)
#  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_qtopengl ${SDL_LIBRARY})
# endif(SDL_FOUND)

install(DIRECTORY icons textures models DESTINATION include/argos3/plugins/simulator/visualizations/qt-opengl)
install(FILES ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL} DESTINATION include/argos3/plugins/simulator/visualizations/qt-opengl)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_qtopengl
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3
)
