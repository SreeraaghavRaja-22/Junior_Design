#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "includes/main_init.h"
#include "includes/semaphores.h"
#include "math.h"
#include "includes/tasks.h"

int main(void) {
    stdio_init_all();
    // Temporarily removed cyw43_arch_init() to eliminate driver conflicts
    main_init();

    // initialize semaphore
    xButtonSemaphore = xSemaphoreCreateBinary();
    xLedMutex = xSemaphoreCreateMutex();

    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_isr);

    // initialize threads
    xTaskCreate(Heartbeat_Task, "HB", 512, NULL, 1, NULL);
    xTaskCreate(Button_Task, "BUTT", 512, NULL, 2, NULL);
    xTaskCreate(Switch_Task, "SWI", 512, NULL, 2, NULL);

    // start RTOS
    vTaskStartScheduler(); // creates the idle thread, so no worries

    for(;;); // Should never get here
}
