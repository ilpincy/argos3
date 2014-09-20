#
# Headers
#
# argos3/plugins/simulator/visualizations/qt-opengl/models
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL_MODELS
  models/qtopengl_box.h
  models/qtopengl_cylinder.h
  models/qtopengl_light.h)
# argos3/plugins/simulator/visualizations/qt-opengl
# moc-headers processed by Qt's MOC utility
set(ARGOS3_MOC_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL
  qtopengl_application.h
  qtopengl_camera.h
  qtopengl_main_window.h
  qtopengl_widget.h)
# if(SDL_FOUND)
#   set(ARGOS3_MOC_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL
#     ${ARGOS3_MOC_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL}
#     qtopengl_joystick.h)
# endif(SDL_FOUND)
if(ARGOS_WITH_LUA)
  set(ARGOS3_MOC_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL
    ${ARGOS3_MOC_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL}
    qtopengl_lua_editor.h
    qtopengl_lua_find_dialog.h
    qtopengl_lua_main_window.h
    qtopengl_lua_syntax_highlighter.h
    qtopengl_lua_statetree_model.h)
endif(ARGOS_WITH_LUA)
# all base headers
set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL
  ${ARGOS3_MOC_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL}
  qtopengl_render.h
  qtopengl_log_stream.h
  qtopengl_user_functions.h)
if(ARGOS_WITH_LUA)
  set(ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL
    ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL}
    qtopengl_lua_statetree_item.h)
endif(ARGOS_WITH_LUA)

#
# Run the moc utility
#
qt4_wrap_cpp(ARGOS3_MOC_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL
  ${ARGOS3_MOC_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL})

#
# Source files
#
# Common for all builds
set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL}
  ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL_MODELS}
  ${ARGOS3_MOC_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL}
  models/qtopengl_box.cpp
  models/qtopengl_cylinder.cpp
  models/qtopengl_light.cpp
  qtopengl_application.cpp
  qtopengl_camera.cpp
  qtopengl_main_window.cpp
  qtopengl_render.cpp
  qtopengl_user_functions.cpp
  qtopengl_widget.cpp)
# Compile joystick support if SDL was found
# if(SDL_FOUND)
#   set(ARGOS3_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL
#     ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL}
#     qtopengl_joystick.cpp)
# endif(SDL_FOUND)
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
# Add the current binary dir to the include path to allow the compiler to find
# the moc-generated files
#
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#
# Create Qt-OpenGL visualization
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_qtopengl SHARED
  ${ARGOS3_SOURCES_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_qtopengl argos3plugin_${ARGOS_BUILD_FOR}_entities ${QT_LIBRARIES} ${GLUT_LIBRARY} ${OPENGL_LIBRARY})
# Link SDL if necessary
# if(SDL_FOUND)
#  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_qtopengl ${SDL_LIBRARY})
# endif(SDL_FOUND)

install(DIRECTORY icons textures DESTINATION include/argos3/plugins/simulator/visualizations/qt-opengl)
install(FILES ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL_MODELS} DESTINATION include/argos3/plugins/simulator/visualizations/qt-opengl/models)
install(FILES ${ARGOS3_HEADERS_PLUGINS_SIMULATOR_VISUALIZATIONS_QTOPENGL}        DESTINATION include/argos3/plugins/simulator/visualizations/qt-opengl)
install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_qtopengl
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3
)
