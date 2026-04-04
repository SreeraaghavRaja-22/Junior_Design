# --- FreeRTOS_import.cmake ---
if (NOT DEFINED FREERTOS_KERNEL_PATH)
    set(FREERTOS_KERNEL_PATH "/home/sreer/pico/FreeRTOS-Kernel")
endif()
if (NOT EXISTS "${FREERTOS_KERNEL_PATH}/CMakeLists.txt")
    message(FATAL_ERROR "FreeRTOS Kernel not found at ${FREERTOS_KERNEL_PATH}")
endif()
message(STATUS "FreeRTOS kernel path: ${FREERTOS_KERNEL_PATH}")
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)