# Checks whether QT, OpenGL and GLUT are present and correctly configured
#
# Usage:
#
#   INCLUDE(ARGoSCheckQTOpenGL)
#   IF(ARGOS_COMPILE_QTOPENGL)
#
#      your qt-opengl-dependent code here
#
#   ENDIF(ARGOS_COMPILE_QTOPENGL)
#
# This module defines this variable:
#
#  ARGOS_COMPILE_QTOPENGL   - True if QT5, OpenGL and GLUT are present and correctly configured
#  ARGOS_QTOPENGL_LIBRARIES - The list of libraries to link
#
# AUTHOR: Carlo Pinciroli <ilpincy@gmail.com>

#
# Set CMake policies to select wanted behaviors
#
# Use new policies introduced up to this version
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif(POLICY CMP0072)

#
# Force avoidance of ARGoS Qt-OpenGL compilation
#
if(NOT DEFINED ARGOS_FORCE_NO_QTOPENGL)
  option(ARGOS_FORCE_NO_QTOPENGL "ON -> avoid trying to compile Qt-OpenGL, OFF -> try to compile Qt-OpenGL" OFF)
endif(NOT DEFINED ARGOS_FORCE_NO_QTOPENGL)

# By default, do not compile Qt
set(ARGOS_COMPILE_QTOPENGL OFF)
# Check for Qt, OpenGL, and GLUT unless 
if(NOT ARGOS_FORCE_NO_QTOPENGL)
  
  # Look for OpenGL
  find_package(OpenGL)
  if(NOT OPENGL_FOUND)
    message(STATUS "OpenGL not found.")
  endif(NOT OPENGL_FOUND)

  # Look for GLUT
  find_package(GLUT)
  if(NOT GLUT_FOUND)
    message(STATUS "GLUT not found.")
  endif(NOT GLUT_FOUND)

  # The CMake files for Qt are not automatically installed
  # system-wide on Mac, so we use brew to search for them
  if(APPLE AND (NOT ARGOS_BREW_QT_CELLAR))
    # Get Qt cellar path
    execute_process(
      COMMAND brew --cellar qt
      OUTPUT_VARIABLE ARGOS_BREW_QT_CELLAR
      ERROR_QUIET
      OUTPUT_STRIP_TRAILING_WHITESPACE)
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
  endif(APPLE AND (NOT ARGOS_BREW_QT_CELLAR))

  # Now look for either Qt6 or Qt5
  find_package(Qt6 COMPONENTS Widgets Gui OpenGLWidgets)
  if(NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Widgets Gui OpenGLWidgets)
    if(NOT Qt5_FOUND)
      message(STATUS "Qt not found.")
    endif(NOT Qt5_FOUND)
  endif(NOT Qt6_FOUND)

  if(OPENGL_FOUND AND GLUT_FOUND AND (Qt6_FOUND OR Qt5_FOUND))
    set(ARGOS_COMPILE_QTOPENGL ON)
    add_definitions(-DGL_SILENCE_DEPRECATION)
    set(ARGOS_QTOPENGL_INCLUDE_DIR ${OPENGL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR}/Headers ${GLUT_INCLUDE_DIR})
    set(ARGOS_QTOPENGL_LIBRARIES ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES} Qt::Widgets Qt::Gui Qt::OpenGLWidgets)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
  else(OPENGL_FOUND AND GLUT_FOUND AND (Qt6_FOUND OR Qt5_FOUND))
    message(STATUS "Skipping compilation of Qt-OpenGL visualization.")
  endif(OPENGL_FOUND AND GLUT_FOUND AND (Qt6_FOUND OR Qt5_FOUND))
  
endif(NOT ARGOS_FORCE_NO_QTOPENGL)
