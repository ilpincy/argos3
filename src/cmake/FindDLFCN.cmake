# Find the POSIX dynamic library loading functions
#
# This module defines these variables:
#
#  DLFCN_FOUND       - True if the GooglePerfTools library was found
#  DLFCN_LIBRARY     - The location of the GooglePerfTools library
#  DLFCN_INCLUDE_DIR - The include directory of the GooglePerfTools library
#
# AUTHOR: Carlo Pinciroli <cpinciro@ulb.ac.be>

#
# Find the header file
#
FIND_PATH(DLFCN_INCLUDE_DIR
  NAMES
  dlfcn.h
  PATHS
  /usr/include/
  /usr/local/include/
  DOC "DLFCN header location"
)

#
# Find the library
#
FIND_LIBRARY(DLFCN_LIBRARY
  NAMES
  dl
  PATHS
  /usr/lib
  /usr/lib64
  /usr/local/lib
  /usr/local/lib64
  DOC "DLFCN library location"
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(DLFCN DEFAULT_MSG
	DLFCN_LIBRARY DLFCN_INCLUDE_DIR)

IF(DLFCN_INCLUDE_DIR AND DLFCN_LIBRARY)
	SET(DLFCN_INCLUDE_DIRS ${DLFCN_INCLUDE_DIR})
	SET(DLFCN_LIBRARIES	  ${DLFCN_LIBRARY})
ENDIF(DLFCN_INCLUDE_DIR AND DLFCN_LIBRARY)

MARK_AS_ADVANCED(DLFCN_INCLUDE_DIR)
MARK_AS_ADVANCED(DLFCN_LIBRARY)
