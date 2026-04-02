cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

set(PICO_BOARD pico2_w)

project(final_project C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

add_executable(final_project
    src/main.c
)

pico_enable_stdio_usb(final_project 1)
pico_enable_stdio_usb(final_project 0)
pico_add_extra_outputs(final_project)

target_link_libraries(final_project
    pico_stdlib
    hardware_uart
    hardware_gpio
    pico_time
    hardware_irq
    hardware_i2c
    hardware_spi
    hardware_dma
    hardware_pwm
    hardware_adc
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_nosys
)

target_include_directories(final_project PRIVATE
    "/home/sreer/Junior_Design/Final_Project/Junior_Design_C/src"
    "${PICO_SDK_PATH}/src/rp2_common/pico_lwip/include"
    "${PICO_SDK_PATH}/lib/lwip/src/include"
)
