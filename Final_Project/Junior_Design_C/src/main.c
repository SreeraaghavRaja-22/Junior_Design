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
#include "includes/chime.h"


int main(void) {
    stdio_init_all();
    sleep_ms(2000);
    main_init();
    generate_sine();
    lcd_set_cursor(0, 0);
    lcd_print("Luminous Library");

    // initialize semaphores
    xButtonSemaphore = xSemaphoreCreateBinary();
    xLedMutex        = xSemaphoreCreateMutex();
    xChimeSemaphore  = xSemaphoreCreateBinary();
    xADCMutex        = xSemaphoreCreateMutex();

    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_isr);

    // initialize threads
    xTaskCreate(Progress_Task,  "HB",   512, NULL, 1, &xProgressHandle);
    xTaskCreate(Button_Task,    "BUTT", 512, NULL, 2, &xButtonHandle);
    xTaskCreate(Switch_Task,    "SWI",  512, NULL, 2, &xSwitchHandle);
    xTaskCreate(Chime_Task,     "CHM",  512, NULL, 3, NULL);
    xTaskCreate(Photo_Task,     "PHO",  512, NULL, 1, NULL);
    xTaskCreate(Log_Task,       "LOG",  512, NULL, 4, NULL);

    // start RTOS
    vTaskStartScheduler();

    for(;;);
}
