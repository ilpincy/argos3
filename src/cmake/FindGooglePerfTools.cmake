# Find Google PerfTools
#
# This module defines these variables:
#
#  GOOGLEPERFTOOLS_FOUND       - True if the GooglePerfTools library was found
#  GOOGLEPERFTOOLS_LIBRARY     - The location of the GooglePerfTools library
#  GOOGLEPERFTOOLS_INCLUDE_DIR - The include directory of the GooglePerfTools library
#
# AUTHOR: Carlo Pinciroli <cpinciro@ulb.ac.be>

#
# Define a default exception scheme to link against
# and validate user choice.
#

#
# Find the header file
#
FIND_PATH(GOOGLEPERFTOOLS_INCLUDE_DIR
  NAMES
  tcmalloc.h
  PATHS
  /usr/include/google
  /usr/local/include/google
  /opt/local/include/google
  DOC "Google PerfTools header location"
)

#
# Find the library
#
FIND_LIBRARY(GOOGLEPERFTOOLS_LIBRARY
  NAMES
  tcmalloc
  PATHS
  /usr/lib
  /usr/local/lib
  /opt/local/lib
  DOC "Google PerfTools library location"
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GooglePerfTools DEFAULT_MSG
	GOOGLEPERFTOOLS_LIBRARY GOOGLEPERFTOOLS_INCLUDE_DIR)

IF(GOOGLEPERFTOOLS_INCLUDE_DIR AND GOOGLEPERFTOOLS_LIBRARY)
	SET(GOOGLEPERFTOOLS_INCLUDE_DIRS ${GOOGLEPERFTOOLS_INCLUDE_DIR})
	SET(GOOGLEPERFTOOLS_LIBRARIES	  ${GOOGLEPERFTOOLS_LIBRARY})
ENDIF(GOOGLEPERFTOOLS_INCLUDE_DIR AND GOOGLEPERFTOOLS_LIBRARY)

MARK_AS_ADVANCED(GOOGLEPERFTOOLS_INCLUDE_DIR)
MARK_AS_ADVANCED(GOOGLEPERFTOOLS_LIBRARY)
