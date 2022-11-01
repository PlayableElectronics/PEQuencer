cmake_minimum_required(VERSION 3.12)

set(PICO_EXTRAS_PATH "/Users/danielgoorny/Repos/pico-extras/external")

include(pico_sdk_import.cmake)
#include(pico_extras_import.cmake)

project(PEQuencer)

pico_sdk_init()

add_subdirectory(freertos)
add_subdirectory(src)
