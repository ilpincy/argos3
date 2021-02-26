#!/bin/bash
set -x

cd $HOME # You start at /

# Build ARGoS
git clone https://github.com/ilpincy/argos3.git argos3
cd argos3
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=$ARGOS_CC -DCMAKE_CXX_COMPILER=$ARGOS_CXX -DARGOS_INSTALL_LDSOCONF=OFF -DCMAKE_INSTALL_PREFIX=$HOME/.local ../src
make

# Bubble up return code to travis
exit
