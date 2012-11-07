# Find the lua scripting language
#
# This module defines these variables:
#
#  LUA_FOUND       - True if the GooglePerfTools library was found
#  LUA_LIBRARY     - The location of the GooglePerfTools library
#  LUA_INCLUDE_DIR - The include directory of the GooglePerfTools library
#
# AUTHOR: Carlo Pinciroli <cpinciro@ulb.ac.be>

FIND_PATH(LUA_EXECUTABLE
  NAMES
  lua
  PATHS
  /usr/bin/
  /usr/local/bin/
  DOC "lua executable location")

FIND_PATH(LUAC_EXECUTABLE
  NAMES
  luac
  PATHS
  /usr/bin/
  /usr/local/bin/
  DOC "luac executable location")

FIND_PATH(LUA_INCLUDE_DIR
  NAMES
  lua.h
  PATHS
  /usr/include
  /usr/local/include
  DOC "Lua header location")

FIND_LIBRARY(LUA_LIBRARY
  NAMES
  lua
  PATHS
  /usr/lib
  /usr/local/lib
  DOC "Lua library executable location")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Lua DEFAULT_MSG
	LUA_LIBRARY LUA_INCLUDE_DIR)

IF(LUA_INCLUDE_DIR AND LUA_LIBRARY)
	SET(LUA_INCLUDE_DIRS ${LUA_INCLUDE_DIR})
	SET(LUA_LIBRARIES	  ${LUA_LIBRARY})
ENDIF(LUA_INCLUDE_DIR AND LUA_LIBRARY)

MARK_AS_ADVANCED(LUA_INCLUDE_DIR)
MARK_AS_ADVANCED(LUA_LIBRARY)
