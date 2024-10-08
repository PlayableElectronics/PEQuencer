## 🏃Get Started
### Install the toolchain
 - for MacOS
```sh
$ brew install cmake
$ brew tap ArmMbed/homebrew-formulae
$ brew install --cask gcc-arm-embedded
```
- for Linux
```sh
$ sudo apt install cmake \
	gcc-arm-none-eabi \
	libnewlib-arm-none-eabi \ 
	libstdc++-arm-none-eabi-newlib
```
### Set-up repository
```sh
$ utils/init.sh
```
### Build from source
```sh
$ mkdir build
$ cd build
$ cmake ..
$ make
```
### Flash
After plugging the Pico board into the host computer, the board appears as a Flash drive. If it does not, then unplug the board from the USB port. Hold down the **BOOTSEL** switch found on top of the board and plug the board back into the USB port. After three seconds, release the **BOOTSEL** switch.
 - for MacOS
```sh
$ cp build/src/pequencer.uf2 /Volumes/<device name>
```
 - for Linux
```sh
$ cp build/src/pequencer.uf2 /media/<username>/<device name>
```
