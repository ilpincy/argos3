# Checks whether QT5, OpenGL and GLUT are present and correctly configured
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
# Force avoidance of ARGoS Qt-OpenGL compilation
#
if(NOT DEFINED ARGOS_FORCE_NO_QTOPENGL)
  option(ARGOS_FORCE_NO_QTOPENGL "ON -> avoid trying to compile Qt-OpenGL, OFF -> try to compile Qt-OpenGL" OFF)
endif(NOT DEFINED ARGOS_FORCE_NO_QTOPENGL)

if(NOT ARGOS_FORCE_NO_QTOPENGL)
  #
  # Qt 5.8 is not linked automatically by HomeBrew on MacOSX
  # So we must find where it is
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

  if((NOT APPLE) OR ARGOS_BREW_QT_CELLAR)
    # Check for QT5
    set(ARGOS_COMPILE_QTOPENGL false) 
    find_package(Qt5 COMPONENTS Widgets Gui)
    if(Qt5Widgets_FOUND AND Qt5Gui_FOUND)
      # QT5 found, is it the minimum required version?
      if(Qt5_VERSION VERSION_GREATER 5.4)
        if (POLICY CMP0072)
          cmake_policy (SET CMP0072 OLD)
        endif(POLICY CMP0072)
        # QT is OK, now check for OpenGL
        find_package(OpenGL)
        if(OPENGL_FOUND)
          # OpenGL is ok, now check for GLUT
          find_package(GLUT)
          if(GLUT_FOUND)
            # GLUT is ok
            
            # All the required libraries are OK
            set(ARGOS_COMPILE_QTOPENGL ON)
            if(APPLE)
              add_definitions(-DGL_SILENCE_DEPRECATION)
              include_directories(${OPENGL_INCLUDE_DIR}/Headers)
            endif(APPLE)
            # These are required by Qt5
            set(CMAKE_AUTOMOC ON)
            set(CMAKE_CXX_STANDARD 14)
            set(CMAKE_INCLUDE_CURRENT_DIR ON)
            # Paths
            add_definitions(${Qt5Widgets_DEFINITIONS} ${Qt5Gui_DEFINITIONS})
            include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS})
            set(ARGOS_QTOPENGL_LIBRARIES ${Qt5Widgets_LIBRARIES} ${Qt5Gui_LIBRARIES} ${GLUT_LIBRARY} ${OPENGL_LIBRARY})

          else(GLUT_FOUND)
            message(STATUS "GLUT not found. Skipping compilation of QT-OpenGL visualization.")
          endif(GLUT_FOUND)

        else(OPENGL_FOUND)
          message(STATUS "OpenGL not found. Skipping compilation of QT-OpenGL visualization.")
        endif(OPENGL_FOUND)

      else(Qt5_VERSION VERSION_GREATER 5.4)
        message(STATUS "Minimum required version for Qt (>= 5.5) not found. Skipping compilation of QT-OpenGL visualization.")
      endif(Qt5_VERSION VERSION_GREATER 5.4)
      
    else(Qt5Widgets_FOUND AND Qt5Gui_FOUND)
      if(NOT Qt5Widgets_FOUND)
        message(STATUS "Qt5Widgets not found, skipping compilation of QT-OpenGL visualization.")
      endif(NOT Qt5Widgets_FOUND)
      if(NOT Qt5Gui_FOUND)
        message(STATUS "Qt5Gui not found, skipping compilation of QT-OpenGL visualization.")
      endif(NOT Qt5Gui_FOUND)
    endif(Qt5Widgets_FOUND AND Qt5Gui_FOUND)
  endif((NOT APPLE) OR ARGOS_BREW_QT_CELLAR)
endif(NOT ARGOS_FORCE_NO_QTOPENGL)
