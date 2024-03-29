add_subdirectory(../submodules/pico-arduino-compat/libs/adafruit-sh110x lib/adafruit-sh110x)
add_subdirectory(../submodules/pico-arduino-compat/libs/rotaryencoder lib/rotaryencoder)
add_subdirectory(../submodules/pico-arduino-compat/libs/easybutton lib/easybutton)
add_subdirectory(../submodules/diatonic lib/diatonic)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR} ".")

add_executable(pequencer
  #usb_descriptors.c
  main.cpp
  #Playtune.cpp
)

target_link_libraries(pequencer
  pico_stdlib
  pico_stdio_usb
  hardware_i2c
  hardware_pio
  #tinyusb_device
  #tinyusb_board
  freertos
  pac-adafruit-sh110x
  pac-rotaryencoder
  pac-easybutton
  diatonic
)

set(PICO_BOARD pequencer)

pico_set_program_name(pequencer "pequencer")
pico_set_program_version(pequencer "0.1")
pico_enable_stdio_usb(pequencer 1)
pico_enable_stdio_uart(pequencer 1)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

pico_generate_pio_header(pequencer ${CMAKE_CURRENT_LIST_DIR}/piosrc/ws2812.pio)

pico_add_extra_outputs(pequencer)
