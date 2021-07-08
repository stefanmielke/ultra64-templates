#!/bin/bash
make cleaner -C src/
make clean -C src/

# building textures
./build-resources.sh

make -j -C src/
