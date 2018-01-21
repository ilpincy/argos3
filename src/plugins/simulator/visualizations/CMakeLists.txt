#
# Compile the graphical visualization only if the necessary libraries have been found
#
if(ARGOS_COMPILE_QTOPENGL)
  add_subdirectory(qt-opengl)
endif(ARGOS_COMPILE_QTOPENGL)
