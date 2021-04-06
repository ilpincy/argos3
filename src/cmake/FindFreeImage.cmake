#
# Try to find the FreeImage library and include path.
# Once done this will define
#
# FREEIMAGE_FOUND
# FREEIMAGE_INCLUDE_DIRS
# FREEIMAGE_LIBRARIES
#

if(NOT FREEIMAGE_FOUND)

  find_path(FREEIMAGE_BASE_INCLUDE_DIR FreeImage.h
    /usr/include
    /usr/local/include
    /sw/include
    /opt/local/include
    DOC "Path to the base FreeImage.h header")
  find_path(FREEIMAGE_PLUS_INCLUDE_DIR FreeImagePlus.h
    /usr/include
    /usr/local/include
    /sw/include
    /opt/local/include
    DOC "Path to the base FreeImagePlus.h header")
  find_library(FREEIMAGE_BASE_LIBRARY
    NAMES FreeImage freeimage
    PATHS
    /usr/lib64
    /usr/lib/x86_64-linux-gnu
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    DOC "Path to the FreeImage library")
  find_library(FREEIMAGE_PLUS_LIBRARY
    NAMES FreeImagePlus freeimageplus
    PATHS
    /usr/lib64
    /usr/lib/x86_64-linux-gnu
    /usr/lib
    /usr/local/lib64
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    DOC "Path to FreeImagePlus library")

  if(FREEIMAGE_BASE_INCLUDE_DIR AND FREEIMAGE_PLUS_INCLUDE_DIR AND FREEIMAGE_BASE_LIBRARY AND FREEIMAGE_PLUS_LIBRARY)
    set(FREEIMAGE_FOUND TRUE CACHE BOOL "Set to TRUE if FreeImage is found, FALSE otherwise")
    message(STATUS "Found FreeImage: ${FREEIMAGE_LIBRARIES}")
  else(FREEIMAGE_BASE_INCLUDE_DIR AND FREEIMAGE_PLUS_INCLUDE_DIR AND FREEIMAGE_BASE_LIBRARY AND FREEIMAGE_PLUS_LIBRARY)
    set(FREEIMAGE_FOUND FALSE CACHE BOOL "Set to TRUE if FreeImage is found, FALSE otherwise")
    message(STATUS "FreeImage not found")
  endif(FREEIMAGE_BASE_INCLUDE_DIR AND FREEIMAGE_PLUS_INCLUDE_DIR AND FREEIMAGE_BASE_LIBRARY AND FREEIMAGE_PLUS_LIBRARY)

  mark_as_advanced(
    FREEIMAGE_FOUND
    FREEIMAGE_BASE_INCLUDE_DIR
    FREEIMAGE_PLUS_INCLUDE_DIR
    FREEIMAGE_BASE_LIBRARY
    FREEIMAGE_PLUS_LIBRARY)

endif(NOT FREEIMAGE_FOUND)
