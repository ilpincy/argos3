# Checks whether QT4, OpenGL and GLUT are present and correctly configured
#
# Usage:
#
#   INCLUDE(CheckQTOpenGL4ARGoS)
#   IF(ARGOS_COMPILE_QTOPENGL)
#
#      your qt-opengl-dependent code here
#
#   ENDIF(ARGOS_COMPILE_QTOPENGL)
#
# This module defines this variable:
#
#  ARGOS_COMPILE_QTOPENGL - True if QT4, OpenGL and GLUT are present and correctly configured
#
# AUTHOR: Carlo Pinciroli <ilpincy@gmail.com>

# Check for QT4
set(ARGOS_COMPILE_QTOPENGL false) 
set(QT_USE_QTOPENGL true)
find_package(Qt4)
if(QT4_FOUND)
  # QT4 found, is it the minimum required version?
  if(QT_VERSION_MAJOR GREATER 3 AND QT_VERSION_MINOR GREATER 5)
    # Is the QTOpenGL module present?
    if(QT_QTOPENGL_FOUND)
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
	    include_directories(${OPENGL_INCLUDE_DIR}/Headers)
	  endif(APPLE)
	  include(${QT_USE_FILE})
          
          # # Now check for SDL (optional)
          # set(SDL_BUILDING_LIBRARY true)
          # find_package(SDL)
          # if(SDL_FOUND)
          #   # SDL is ok
          #   set(QTOPENGL_WITH_SDL ON)
          #   include_directories(${SDL_INCLUDE_DIR})
          
          # else(SDL_FOUND)
          #   message(STATUS "SDL not found. QTOpenGL won't have joystick support.")
          # endif(SDL_FOUND)
          
        else(GLUT_FOUND)
          message(STATUS "GLUT not found. Skipping compilation of QT-OpenGL visualization.")
        endif(GLUT_FOUND)

      else(OPENGL_FOUND)
        message(STATUS "OpenGL not found. Skipping compilation of QT-OpenGL visualization.")
      endif(OPENGL_FOUND)

    else(QT_QTOPENGL_FOUND)
      message(STATUS "QtOpenGL module not found. Skipping compilation of QT-OpenGL visualization.")
    endif(QT_QTOPENGL_FOUND)

  else(QT_VERSION_MAJOR GREATER 3 AND QT_VERSION_MINOR GREATER 5)
    message(STATUS "Minimum required version for Qt (>= 4.6) not found. Skipping compilation of QT-OpenGL visualization.")
  endif(QT_VERSION_MAJOR GREATER 3 AND QT_VERSION_MINOR GREATER 5)
  
else(QT4_FOUND)
  message(STATUS "Qt4 not found, skipping compilation of QT-OpenGL visualization.")
endif(QT4_FOUND)
