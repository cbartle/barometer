cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)
project(custom_barometer C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(barometer barometer.c)

# pull in common dependencies
target_link_libraries(barometer pico_stdlib hardware_i2c)

# enable usb output, disable uart output
pico_enable_stdio_usb(barometer 1)
pico_enable_stdio_uart(barometer 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(barometer)

# add url via pico_set_program_url
#example_auto_set_url(barometer)