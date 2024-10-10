cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(PEQuencer)

pico_sdk_init()

set(PICO_SDK_FREERTOS_SOURCE FreeRTOS-Kernel)

add_library(freertos
    ${PICO_SDK_FREERTOS_SOURCE}/event_groups.c
    ${PICO_SDK_FREERTOS_SOURCE}/list.c
    ${PICO_SDK_FREERTOS_SOURCE}/queue.c
    ${PICO_SDK_FREERTOS_SOURCE}/stream_buffer.c
    ${PICO_SDK_FREERTOS_SOURCE}/tasks.c
    ${PICO_SDK_FREERTOS_SOURCE}/timers.c
    ${PICO_SDK_FREERTOS_SOURCE}/portable/MemMang/heap_3.c
    ${PICO_SDK_FREERTOS_SOURCE}/portable/GCC/ARM_CM0/port.c
)

target_include_directories(freertos PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/config
    ${PICO_SDK_FREERTOS_SOURCE}/include
    ${PICO_SDK_FREERTOS_SOURCE}/portable/GCC/ARM_CM0
)

add_subdirectory(submodules/pico-arduino-compat/libs/adafruit-sh110x lib/adafruit-sh110x)
add_subdirectory(submodules/pico-arduino-compat/libs/rotaryencoder lib/rotaryencoder)
add_subdirectory(submodules/pico-arduino-compat/libs/easybutton lib/easybutton)
add_subdirectory(submodules/diatonic lib/diatonic)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR} ".")

add_executable(pequencer
  src/main.cpp
  src/tasks/task_display.cpp
  src/tasks/task_hardware.cpp
  src/tasks/task_sequencer.cpp
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
  diatonic
)

set(PICO_BOARD pequencer)

pico_set_program_name(pequencer "pequencer")
pico_set_program_version(pequencer "0.1.0")
pico_enable_stdio_usb(pequencer 1)
pico_enable_stdio_uart(pequencer 1)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

pico_generate_pio_header(pequencer ${CMAKE_CURRENT_LIST_DIR}/src/piosrc/ws2812.pio)

pico_add_extra_outputs(pequencer)
