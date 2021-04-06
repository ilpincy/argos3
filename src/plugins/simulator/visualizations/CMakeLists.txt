#
# Compile the graphical visualization only if the necessary libraries have been found
#
if(ARGOS_QTOPENGL_FOUND)
  add_subdirectory(qt-opengl)
endif(ARGOS_QTOPENGL_FOUND)
