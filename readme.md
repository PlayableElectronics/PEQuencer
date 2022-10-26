#path
export PICO_SDK_PATH="$HOME/Repos/pico-sdk"

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
