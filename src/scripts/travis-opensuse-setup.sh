#!/bin/bash
set -x

# Setup environment
zypper ar -n openSUSE-13.2-Graphics http://download.opensuse.org/repositories/graphics/openSUSE_13.2/ graphics
zypper refresh
zypper --non-interactive install cmake $ARGOS_CC_PKG $ARGOS_CXX_PKG git freeimage-devel doxygen graphviz asciidoc lua-devel libqt5-qtbase freeglut-devel 
useradd -ms /bin/bash argos-user

exit
