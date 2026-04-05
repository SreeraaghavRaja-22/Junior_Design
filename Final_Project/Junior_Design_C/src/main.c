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
#include "hardware/i2c.h"
#include "includes/lcd_init.h"

int main(void) {
    stdio_init_all();
    sleep_ms(2000);
    // Temporarily removed cyw43_arch_init() to eliminate driver conflicts
    main_init();
    lcd_set_cursor(0, 0);
    lcd_print("Luminous Library");
    

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
