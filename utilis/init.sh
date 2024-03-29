#!/bin/bash

ROOT_DIR=$(realpath "$(realpath $(dirname "$0"))/..")
COMPAT_DIR=$ROOT_DIR/submodules/pico-arduino-compat

fetch_submodules (){
  cd $ROOT_DIR && git submodule init
  git submodule update --init $ROOT_DIR/submodules/pico-sdk
  git submodule update --init $ROOT_DIR/submodules/pico-arduino-compat/arduino-compat/arduino-pico
}

copy_cmake () {
  find $ROOT_DIR/submodules/pico-sdk -type f -name 'pico_sdk_import.cmake' -exec install -m 644 {} $ROOT_DIR \;
}

fetch_libraries () {
  cd $COMPAT_DIR/libs/$1
  ./init.sh
  cd -
}

fetch_submodules
copy_cmake

fetch_libraries "adafruit-sh110x"
fetch_libraries "rotaryencoder"
fetch_libraries "easybutton"

echo "Please type: export PICO_SDK_PATH="$ROOT_DIR/submodules/pico-sdk""
