# Find ARGoS Qt OpenGL components
#
# This module defines these variables:
#
#  ARGOS_QTOPENGL_FOUND        - True if all the ARGoS Qt OpenGL components were found
#  ARGOS_QTOPENGL_LIBRARIES    - The location of the Qt OpenGL libraries
#  ARGOS_QTOPENGL_INCLUDE_DIRS - The include directories of the ARGoS Qt OpenGL components
#
# AUTHOR: Carlo Pinciroli <cpinciro@ulb.ac.be>

#
# Force avoidance of ARGoS Qt-OpenGL compilation
#
if(NOT DEFINED ARGOS_FORCE_NO_QTOPENGL)
  option(ARGOS_FORCE_NO_QTOPENGL "ON -> avoid trying to compile Qt-OpenGL, OFF -> try to compile Qt-OpenGL" OFF)
endif(NOT DEFINED ARGOS_FORCE_NO_QTOPENGL)

# Check for Qt, OpenGL, and GLUT unless 
if(NOT ARGOS_FORCE_NO_QTOPENGL)
  # Look for OpenGL
  find_package(OpenGL)
  if(OPENGL_FOUND)
    set(ARGOS_QTOPENGL_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR}/Headers)
    set(ARGOS_QTOPENGL_LIBRARIES ${OPENGL_LIBRARIES})
  else(OPENGL_FOUND)
    message(STATUS "OpenGL not found.")
  endif(OPENGL_FOUND)

  # Look for GLUT
  find_package(GLUT)
  if(GLUT_FOUND)
    list(APPEND ARGOS_QTOPENGL_INCLUDE_DIRS ${GLUT_INCLUDE_DIRS})
    list(APPEND ARGOS_QTOPENGL_LIBRARIES ${GLUT_LIBRARIES})
  else(GLUT_FOUND)
    message(STATUS "GLUT not found.")
  endif(GLUT_FOUND)

  # The CMake files for Qt are not automatically installed
  # system-wide on Mac, so we use brew to search for them
  if(APPLE)
    if(NOT ARGOS_BREW_QT_CELLAR)
      # Get Qt cellar path
      execute_process(
        COMMAND brew --cellar qt
        OUTPUT_VARIABLE ARGOS_BREW_QT_CELLAR
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    endif(NOT ARGOS_BREW_QT_CELLAR)
    if(NOT ARGOS_BREW_QT_CELLAR STREQUAL "")
      set(ARGOS_BREW_QT_CELLAR ${ARGOS_BREW_QT_CELLAR} CACHE STRING "HomeBrew cellar for Qt")
      # Get latest Qt version installed
      execute_process(
        COMMAND ls "${ARGOS_BREW_QT_CELLAR}"
        COMMAND tail -n1
        OUTPUT_VARIABLE ARGOS_BREW_QT_VERSION
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE)
      # Add path to CMAKE_PREFIX_PATH
      set(CMAKE_PREFIX_PATH
        ${CMAKE_PREFIX_PATH}
        "${ARGOS_BREW_QT_CELLAR}/${ARGOS_BREW_QT_VERSION}/lib/cmake")
    else(NOT ARGOS_BREW_QT_CELLAR STREQUAL "")
      message(STATUS "Could not find HomeBrew cellar for Qt")
    endif(NOT ARGOS_BREW_QT_CELLAR STREQUAL "")
  endif(APPLE)
    
  # Now look for either Qt6 or Qt5
  find_package(Qt6 QUIET COMPONENTS Core Widgets Gui OpenGLWidgets)
  if(Qt6_FOUND)
    message(STATUS "Found Qt6: version ${Qt6Core_VERSION}")
    message(STATUS "Found Qt6Widgets: version ${Qt6Widgets_VERSION}")
    message(STATUS "Found Qt6Gui: version ${Qt6Gui_VERSION}")
    message(STATUS "Found Qt6OpenGLWidgets: version ${Qt6OpenGLWidgets_VERSION}")
    list(APPEND ARGOS_QTOPENGL_INCLUDE_DIRS ${Qt6Widgets_INCLUDE_DIRS} ${Qt6Gui_INCLUDE_DIRS} ${Qt6OpenGLWidgets_INCLUDE_DIRS})
    list(APPEND ARGOS_QTOPENGL_LIBRARIES Qt6::Widgets Qt6::Gui Qt6::OpenGLWidgets)
  else(Qt6_FOUND)
    find_package(Qt5 QUIET COMPONENTS Widgets Gui OPTIONAL_COMPONENTS OpenGLWidgets)
    if(Qt5_FOUND)
      message(STATUS "Found Qt5: version ${Qt5Core_VERSION_}")
      message(STATUS "Found Qt5Widgets: version ${Qt5Widgets_VERSION}")
      message(STATUS "Found Qt5Gui: version ${Qt5Gui_VERSION}")
      list(APPEND ARGOS_QTOPENGL_INCLUDE_DIRS ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS})
      list(APPEND ARGOS_QTOPENGL_LIBRARIES Qt5::Widgets Qt5::Gui)
      if(Qt5OpenGLWidgets_FOUND)
        message(STATUS "Found Qt5OpenGLWidgets: version ${Qt5OpenGLWidgets_VERSION}")
        list(APPEND ARGOS_QTOPENGL_INCLUDE_DIRS ${Qt5OpenGLWidgets_INCLUDE_DIRS})
        list(APPEND ARGOS_QTOPENGL_LIBRARIES Qt5::OpenGLWidgets)
      endif(Qt5OpenGLWidgets_FOUND)
    else(Qt5_FOUND)
      message(STATUS "Qt not found.")
    endif(Qt5_FOUND)
  endif(Qt6_FOUND)

  if(OPENGL_FOUND AND GLUT_FOUND AND (Qt6_FOUND OR Qt5_FOUND))
    add_definitions(-DGL_SILENCE_DEPRECATION)
    list(REMOVE_DUPLICATES ARGOS_QTOPENGL_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES ARGOS_QTOPENGL_LIBRARIES)
    set(ARGOS_QTOPENGL_FOUND ON CACHE BOOL "Qt-OpenGL components found?" FORCE)
    set(ARGOS_QTOPENGL_INCLUDE_DIRS "${ARGOS_QTOPENGL_INCLUDE_DIRS}" CACHE STRING "Include directories for Qt-OpenGL components" FORCE)
    set(ARGOS_QTOPENGL_LIBRARIES "${ARGOS_QTOPENGL_LIBRARIES}" CACHE STRING "Libraries for Qt-OpenGL components" FORCE)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
  else(OPENGL_FOUND AND GLUT_FOUND AND (Qt6_FOUND OR Qt5_FOUND))
    set(ARGOS_QTOPENGL_FOUND OFF CACHE BOOL "Qt-OpenGL components found?" FORCE)
    unset(ARGOS_QTOPENGL_INCLUDE_DIRS)
    unset(ARGOS_QTOPENGL_LIBRARIES)
    message(STATUS "Skipping compilation of Qt-OpenGL visualization.")
  endif(OPENGL_FOUND AND GLUT_FOUND AND (Qt6_FOUND OR Qt5_FOUND))
  
endif(NOT ARGOS_FORCE_NO_QTOPENGL)

mark_as_advanced(ARGOS_BREW_QT_CELLAR)
mark_as_advanced(ARGOS_QTOPENGL_FOUND)
mark_as_advanced(ARGOS_QTOPENGL_INCLUDE_DIRS)
mark_as_advanced(ARGOS_QTOPENGL_LIBRARIES)
