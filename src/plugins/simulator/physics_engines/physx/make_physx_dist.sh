#!/bin/bash -e

#
# Version of the PhysX Core SDK
#
VERSION=3.3.0

#
# Output directory
#
DIST=$(pwd)/physx_dist

#
# ACTION!
#

# Fixes statements like ' #include " ' that can't be fixed
# with other methods
# $1 -> name of file to fix
function fix_flat_includes() {
    local FILE=$1
    # Go through the lines to fix one by one
    while grep -q '#include "' $FILE; do
        HEADER=$(grep '#include "' $FILE | head -n 1 | tr -d '"' | cut -d\  -f2)
        # The header file is located in the base directory or not?
        if [ -f $DIST/include/$HEADER ]; then
            # Yes, it's located in the base directory
            sed -i -e "0,/#include \"/ s|#include \"\([a-zA-Z0-9_./]*\)\"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/\1>|1" $FILE
        else
            # No, it's located in a deeper directory
            # Substitute the include with one that contains the directory where the file is located
            local FULLDIR=$(dirname $FILE)
            local ACTUALDIR=${FULLDIR#$DIST}
            sed -i -e "0,/#include \"/ s|#include \"\([a-zA-Z0-9_./]*\)\"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist${ACTUALDIR}/\1>|1" $FILE
        fi
    done


}

echo -n "(Re)Creating $DIST directory ... "
rm -rf $DIST
mkdir -p $DIST
echo "done"

echo -n "Unpacking ... "
unzip -qq \
    PhysX-${VERSION}_LINUX_SDK_Core.zip \
    Include/* Lib/* version-PhysX.txt \
    -x '*CHECKED.a' '*PROFILE.a' \
    -d $DIST
unzip -qq \
    PhysX-${VERSION}_OSX_SDK_Core.zip \
    Lib/* \
    -x '*CHECKED.a' '*PROFILE.a' \
    -d $DIST
echo "done"

echo -n "Renaming files and directories ... "
mv $DIST/Include $DIST/include
mv $DIST/Lib $DIST/lib
mv $DIST/version-PhysX.txt $DIST/VERSION
echo "done"

echo -n "Fixing permissions ... "
find $DIST -type d -exec chmod 755 {} \;
find $DIST -type f -exec chmod 644 {} \;
echo "done"

echo -n "Fixing #include statements ... "
# Fix non-flat includes
find $DIST/include -type f -print0 | \
    xargs -0 -n 1 \
    sed -i \
    -e 's|#include "foundation/\([a-zA-Z0-9_./]*\)"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/foundation/\1>|g' \
    -e 's|#include "common/\([a-zA-Z0-9_./]*\)"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/common/\1>|g' \
    -e 's|#include "physxprofilesdk/\([a-zA-Z0-9_./]*\)"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxprofilesdk/\1>|g' \
    -e 's|#include "physxvisualdebuggersdk/\([a-zA-Z0-9_./]*\)"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/physxvisualdebuggersdk/\1>|g' \
    -e 's|#include "pvd/\([a-zA-Z0-9_./]*\)"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/pvd/\1>|g' \
    -e 's|#include "pxtask/\([a-zA-Z0-9_./]*\)"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/pxtask/\1>|g' \
    -e 's|#include "geometry/\([a-zA-Z0-9_./]*\)"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/geometry/\1>|g' \
    -e 's|#include "characterkinematic/\([a-zA-Z0-9_./]*\)"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/characterkinematic/\1>|g' \
    -e 's|#include "cloth/\([a-zA-Z0-9_./]*\)"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/cloth/\1>|g' \
    -e 's|#include "cooking/\([a-zA-Z0-9_./]*\)"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/cooking/\1>|g' \
    -e 's|#include "extensions/\([a-zA-Z0-9_./]*\)"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/extensions/\1>|g' \
    -e 's|#include "particles/\([a-zA-Z0-9_./]*\)"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/particles/\1>|g' \
    -e 's|#include "vehicle/\([a-zA-Z0-9_./]*\)"|#include <argos3/plugins/simulator/physics_engines/physx/physx_dist/include/vehicle/\1>|g'
# Fix flat includes
for F in $(find $DIST/include/ -type f -print0 | xargs -0 grep -l '#include "')
do
    fix_flat_includes $F
done
echo "done"
