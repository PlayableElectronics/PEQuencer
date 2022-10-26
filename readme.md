### Install Pico SDK

```sh
git clone https://github.com/raspberrypi/pico-sdk
export PICO_SDK_PATH="$HOME/[repos dir]/pico-sdk"
```

### Install External libraries
```sh
git clone https://github.com/fhdm-dev/pico-arduino-compat.git
cd pico-arduino-compat
git submodule update --init arduino-compat/arduino-pico
```
### Download libraries
```sh
./libs/adafruit-sh110x/init.sh
./libs/rotaryencoder/init.sh
./libs/easybutton/init.sh
```
### Build
```sh
mkdir build
cd build
cmake ..
make
```
### Flash
```sh
cp flash.sh build
cd build
chmod +x flash.sh
./flash.sh
```