#
# Project name
#
project(argos3_${ARGOS_BUILD_FOR})

#
# General CPack configuration
#
# Version information
set(CPACK_PACKAGE_VERSION_MAJOR "3")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_RELEASE "1cp")
# Other stuff
set(CPACK_PACKAGE_DESCRIPTION "ARGoS (multi-physics multi-robot simulator)
 ARGoS is a highly scalable multi-robot simulator. Among its distinctive
 features, there are: (i) modularity (robots, devices, physics engines,
 visualizations and controllers are plug-ins), (ii) tunable accuracy, and (iii)
 the possibility to run multiple physics engines at the same time.")
set(CPACK_PACKAGE_HOMEPAGE "http://iridia.ulb.ac.be/argos")
set(CPACK_PACKAGE_MAINTAINER "Carlo Pinciroli <ilpincy@gmail.com>")
execute_process(
  COMMAND uname -i
  COMMAND tr -d '\n'
  OUTPUT_VARIABLE CPACK_PACKAGE_ARCH)
set(CPACK_PACKAGE_NAME "argos3_${ARGOS_BUILD_FOR}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A fast, parallel, multi-physics, and multi-robot simulator")
set(CPACK_PACKAGE_VENDOR "IRIDIA-ULB")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/../doc/GPL_V3")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/../README.asciidoc")
set(CPACK_STRIP_FILES ON)
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_ARCH}-${CPACK_PACKAGE_RELEASE}")

#
# Configuration for the Debian generator
#
set(CPACK_DEBIAN_PACKAGE_DEPENDS "gcc (>= 4.2), g++ (>= 4.2), cmake (>= 2.6), libgsl0-dev (>= 1.15), freeglut3-dev (>= 2.6.0), libqt4-opengl-dev (>= 4.5), libxi-dev, libxmu-dev, libfreeimage-dev (>= 3.15)")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION})
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE ${CPACK_PACKAGE_HOMEPAGE})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${CPACK_PACKAGE_MAINTAINER})
set(CPACK_DEBIAN_PACKAGE_SECTION "contrib/science")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_SOURCE_DIR}/scripts/postinst;")

#
# Configuration for the RPM generator
#
set(CPACK_RPM_PACKAGE_DESCRIPTION ${CPACK_PACKAGE_DESCRIPTION})
set(CPACK_RPM_PACKAGE_URL ${CPACK_PACKAGE_HOMEPAGE})
set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")
set(CPACK_RPM_PACKAGE_LICENSE "GPL")
set(CPACK_RPM_PACKAGE_REQUIRES "gcc >= 4.2, gcc-c++ >= 4.2, cmake >= 2.6, gsl >= 1.15, gsl-devel >= 1.15, freeglut-devel >= 2.8.0, libqt4-devel >= 4.5, libfreeimage3 >= 3.15, libfreeimageplus3 >= 3.15, freeimage-devel >= 3.15")
set(CPACK_RPM_PACKAGE_URL ${CPACK_PACKAGE_HOMEPAGE})
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${CMAKE_SOURCE_DIR}/scripts/argos_post_install.sh")

#
# Time to call CPack
#
include(CPack)
