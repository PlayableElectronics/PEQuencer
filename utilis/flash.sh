#!/bin/bash

RPI_VOLUME=/Volumes/RPI-RP2

ROOT_DIR=$(realpath "$(realpath $(dirname "$0"))/..")
PROGRAM_NAME=`grep 'pico_set_program_name' $ROOT_DIR/CMakeLists.txt | grep -o '\".*\"' | tr -d '\"'`

make -C $ROOT_DIR/build all
find $ROOT_DIR/build -type f -name "${PROGRAM_NAME}.uf2" -exec install -m 644 {} $RPI_VOLUME \;
