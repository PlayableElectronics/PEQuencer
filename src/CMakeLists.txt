add_subdirectory(../../pico-arduino-compat/libs/adafruit-sh110x lib/adafruit-sh110x)
add_subdirectory(../../pico-arduino-compat/libs/rotaryencoder lib/rotaryencoder)
add_subdirectory(../../pico-arduino-compat/libs/easybutton lib/easybutton)

add_executable(pequencer
  main.cpp
)

target_link_libraries(pequencer
  pico_stdlib
  pico_stdio_usb
  hardware_i2c
  hardware_pio
  freertos
  pac-adafruit-sh110x
  pac-rotaryencoder
  pac-easybutton
)

pico_enable_stdio_usb(pequencer 1)
pico_enable_stdio_uart(pequencer 1)

pico_generate_pio_header(pequencer ${CMAKE_CURRENT_LIST_DIR}/piosrc/ws2812.pio)

pico_add_extra_outputs(pequencer)
