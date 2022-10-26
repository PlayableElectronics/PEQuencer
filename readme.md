#path
git clone https://github.com/raspberrypi/pico-sdk
export PICO_SDK_PATH="$HOME/[repos dir]/pico-sdk"

#Pico arduino compat
git clone https://github.com/fhdm-dev/pico-arduino-compat.git

cd pico-arduino-compat
git submodule update --init arduino-compat/arduino-pico

./libs/adafruit-sh110x/init.sh
./libs/rotaryencoder/init.sh
./libs/easybutton/init.sh

#build
mkdir build
cd build
cmake ..
make

#build & flash
cp flash.sh build
cd build
chmod +x flash.sh
./flash.sh
