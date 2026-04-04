#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "includes/main_init.h"
#include "math.h"


void Heartbeat_Task(void *pvParameters) {
    for(;;) {
        uint16_t raw = adc_read(); 
        printf("Raw Value: 0x%03x, voltage = %f V\n", raw, raw * ADC_CONV_FACTOR);
        float voltage_adc = raw * ADC_CONV_FACTOR;
        float threshold = voltage_adc / 3.3f;
        int leds_on = (int)roundf(threshold * 5);

        for(int i = LED1_PIN; i < LED1_PIN + 5; i++){
            if((i - LED1_PIN) < leds_on){gpio_put(i, 1);}
            else{gpio_put(i, 0);}
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main(void) {
    stdio_init_all();
    // Temporarily removed cyw43_arch_init() to eliminate driver conflicts
    main_init();
    
    xTaskCreate(Heartbeat_Task, "HB", 512, NULL, 1, NULL);
    vTaskStartScheduler();

    for(;;); // Should never get here
}