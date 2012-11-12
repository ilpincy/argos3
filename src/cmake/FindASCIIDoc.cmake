# Find ASCIIDoc
#
# This module defines these variables:
#
#  ASCIIDOC_FOUND       - True if the asciidoc executable was found
#  ASCIIDOC_EXECUTABLE  - The location of the asciidoc executable
#
# AUTHOR: Carlo Pinciroli <cpinciro@ulb.ac.be>

#
# Find the header file
#
FIND_PROGRAM(ASCIIDOC_EXECUTABLE
  NAMES
  asciidoc
  PATHS
  /usr/bin
  /usr/local/bin
  DOC "asciidoc executable location"
  )

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ASCIIDoc DEFAULT_MSG ASCIIDOC_EXECUTABLE)

MARK_AS_ADVANCED(ASCIIDOC_EXECUTABLE)
