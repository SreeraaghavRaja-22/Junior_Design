cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

set(PICO_BOARD pico2)

project(mic_basics C CXX ASM)


set(CMAKE_C_STANDARD 11)

set(CMAKE_CXX_STANDARD 17)

set(EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

add_executable(mic_basics
    src/main.c
)

pico_enable_stdio_usb(mic_basics 1)
pico_enable_stdio_uart(mic_basics 0)
pico_add_extra_outputs(mic_basics)
target_link_libraries(mic_basics 
    pico_stdlib
    hardware_uart
    hardware_gpio
    hardware_timer
    pico_time
    hardware_irq
)