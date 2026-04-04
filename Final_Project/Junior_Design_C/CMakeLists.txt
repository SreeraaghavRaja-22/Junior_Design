cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico2_w)

set(FREERTOS_KERNEL_PATH /home/sreer/pico/FreeRTOS-Kernel)
set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/src)

include(pico_sdk_import.cmake)
include(FreeRTOS_import.cmake)

project(final_project C CXX ASM)
pico_sdk_init()

add_executable(final_project
    src/main.c
    src/includes/src/components.c
    src/includes/src/adc_init.c
    src/includes/src/main_init.c
    src/includes/src/semaphores.c
    src/includes/src/tasks.c
)

target_link_libraries(final_project
    pico_stdlib
    hardware_adc
    hardware_i2c
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

pico_enable_stdio_usb(final_project 1)
pico_enable_stdio_uart(final_project 0)
pico_add_extra_outputs(final_project)

target_link_options(final_project PRIVATE -u _printf_float)

target_include_directories(final_project PRIVATE
    src
)