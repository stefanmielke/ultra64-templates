#!/bin/bash

# make cleaner -C src/
make clean -C src/

# ./build-resources.sh

make -j -C src/
