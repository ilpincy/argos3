#
# Project name
#
project(argos3_${ARGOS_BUILD_FOR})

#
# General CPack configuration
#
# Version information
if(NOT DEFINED CPACK_PACKAGE_VERSION_MAJOR)
  execute_process(
    COMMAND git describe --abbrev=0
    COMMAND cut -d. -f1
    COMMAND tr -d '\n'
    OUTPUT_VARIABLE CPACK_PACKAGE_VERSION_MAJOR)
endif(NOT DEFINED CPACK_PACKAGE_VERSION_MAJOR)
if(NOT DEFINED CPACK_PACKAGE_VERSION_MINOR)
  execute_process(
    COMMAND git describe --abbrev=0
    COMMAND cut -d. -f2
    COMMAND tr -d '\n'
    OUTPUT_VARIABLE CPACK_PACKAGE_VERSION_MINOR)
endif(NOT DEFINED CPACK_PACKAGE_VERSION_MINOR)
if(NOT DEFINED CPACK_PACKAGE_VERSION_PATCH)
  execute_process(
    COMMAND git describe --abbrev=0
    COMMAND cut -d. -f3
    COMMAND cut -d- -f1
    COMMAND tr -d '\n'
    OUTPUT_VARIABLE CPACK_PACKAGE_VERSION_PATCH)
endif(NOT DEFINED CPACK_PACKAGE_VERSION_PATCH)
if(NOT DEFINED CPACK_PACKAGE_RELEASE)
execute_process(
  COMMAND git describe --abbrev=0
  COMMAND cut -d- -f2
  COMMAND tr -d '\n'
  OUTPUT_VARIABLE CPACK_PACKAGE_RELEASE)
endif(NOT DEFINED CPACK_PACKAGE_RELEASE)
# Other stuff
set(CPACK_PACKAGE_DESCRIPTION "ARGoS (multi-physics multi-robot simulator)
 ARGoS is a highly scalable multi-robot simulator. Among its distinctive
 features, there are: (i) modularity (robots, devices, physics engines,
 visualizations and controllers are plug-ins), (ii) tunable accuracy, and (iii)
 the possibility to run multiple physics engines at the same time.")
set(CPACK_PACKAGE_HOMEPAGE "http://www.argos-sim.org/")
set(CPACK_PACKAGE_MAINTAINER "Carlo Pinciroli <ilpincy@gmail.com>")
set(CPACK_PACKAGE_NAME "argos3_${ARGOS_BUILD_FOR}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A fast, parallel, multi-physics, and multi-robot simulator")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/../doc/ARGoS_LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/../README.txt")
set(CPACK_STRIP_FILES ON)
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${ARGOS_PROCESSOR_ARCH}-${CPACK_PACKAGE_RELEASE}")

#
# Configuration for the Debian generator
#
set(CPACK_DEBIAN_PACKAGE_DEPENDS "gcc (>= 5.4), g++ (>= 5.4), cmake (>= 3.5), freeglut3-dev (>= 2.6.0), qt5-default (>= 5.5.1), libxi-dev, libxmu-dev, libfreeimage-dev (>= 3.15), libfreeimageplus-dev (>= 3.15), liblua5.3-dev, lua5.3")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION})
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${CPACK_PACKAGE_HOMEPAGE})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${CPACK_PACKAGE_MAINTAINER})
set(CPACK_DEBIAN_PACKAGE_SECTION "contrib/science")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_BINARY_DIR}/postinst;")

#
# Configuration for the RPM generator
#
set(CPACK_RPM_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION})
set(CPACK_RPM_PACKAGE_URL ${CPACK_PACKAGE_HOMEPAGE})
set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_REQUIRES "gcc >= 5.4, gcc-c++ >= 5.4, pkg-config, cmake >= 3.5, freeglut-devel >= 2.8.0, libXmu-devel >= 1.0, libXi-devel >= 1.0, libqt5-qtbase-devel >= 5.5, libfreeimage3 >= 3.15, libfreeimageplus3 >= 3.15, freeimage-devel >= 3.15, lua >= 5.3, lua-devel >= 5.3, liblua5_3 >= 5.3")
set(CPACK_RPM_PACKAGE_URL ${CPACK_PACKAGE_HOMEPAGE})
set(CPACK_RPM_PACKAGE_RELEASE ${CPACK_PACKAGE_RELEASE})
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${CMAKE_BINARY_DIR}/argos_post_install.sh")

#
# Configuration for OSX Package Manager
#
if(APPLE)
  configure_file(${CMAKE_SOURCE_DIR}/scripts/argos3_macos_wrapper.sh.in
    ${CMAKE_BINARY_DIR}/core/argos3_macos_wrapper.sh
    @ONLY)
  configure_file(${CMAKE_SOURCE_DIR}/scripts/macos_uninstall_argos3.sh.in
    ${CMAKE_BINARY_DIR}/macos_uninstall_argos3.sh
    @ONLY)
  install(FILES ${CMAKE_BINARY_DIR}/core/argos3_macos_wrapper.sh
    DESTINATION bin/
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    RENAME argos3)
  install(FILES ${CMAKE_BINARY_DIR}/macos_uninstall_argos3.sh
    DESTINATION share/argos3/
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    RENAME uninstall_argos3.sh)
endif(APPLE)

#
# Creation of SlackBuild script
#
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/slackware/argos.SlackBuild.in
  ${CMAKE_BINARY_DIR}/argos.SlackBuild
  @ONLY)
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/slackware/slack-desc.in
  ${CMAKE_BINARY_DIR}/slack-desc
  @ONLY)
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/slackware/doinst.sh.in
  ${CMAKE_BINARY_DIR}/doinst.sh
  @ONLY)
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/argos_post_install.sh.in
  ${CMAKE_BINARY_DIR}/argos_post_install.sh
  @ONLY)
configure_file(
  ${CMAKE_SOURCE_DIR}/scripts/argos_post_install.sh.in
  ${CMAKE_BINARY_DIR}/postinst
  @ONLY)

#
# Time to call CPack
#
include(CPack)
