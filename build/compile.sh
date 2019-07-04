#!/bin/bash 

cmake -DGeant4_DIR=$GEANT4_DIR/lib64 ../
make -j4
