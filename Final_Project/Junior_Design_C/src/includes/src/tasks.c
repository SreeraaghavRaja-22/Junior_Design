#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "../main_init.h"
#include "../semaphores.h"
#include "math.h"

volatile bool progress_flag = 0;

void Heartbeat_Task(void *pvParameters) {
    for(;;) {
        // printf("Output Progress Val: %d\n", (int)progress_flag);
        if(progress_flag){
            uint16_t raw = adc_read(); 
            printf("Raw Value: 0x%03x, voltage = %f V\n", raw, raw * ADC_CONV_FACTOR);
            float voltage_adc = raw * ADC_CONV_FACTOR;
            float threshold = voltage_adc / 3.3f;
            int leds_on = (int)roundf(threshold * 5);

            xSemaphoreTake(xLedMutex, portMAX_DELAY);
            for(int i = LED1_PIN; i < LED1_PIN + 5; i++){
                if((i - LED1_PIN) < leds_on){gpio_put(i, 1);}
                else{gpio_put(i, 0);}
            }
            xSemaphoreGive(xLedMutex);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void button_isr(uint gpio, uint32_t events){
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // printf("Reached ISR!");
    xSemaphoreGiveFromISR(xButtonSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // triggers a context switch
}

void Button_Task(void *pvParameters){
    for(;;){
        if(xSemaphoreTake(xButtonSemaphore, portMAX_DELAY) == pdTRUE){
            vTaskDelay(pdMS_TO_TICKS(100));
            if(!gpio_get(BUTTON_PIN)){
                progress_flag ^= 1;
            }

            xSemaphoreTake(xLedMutex, portMAX_DELAY);
            for(int i = LED1_PIN; i < LED1_PIN + 5; i++) gpio_put(i, 0);
            xSemaphoreGive(xLedMutex);

            while(xSemaphoreTake(xButtonSemaphore, 0) == pdTRUE);
        }
    }
}

void Switch_Task(void){
    uint8_t last_state = 0; 
    for(;;){
        uint8_t switches = 0; 

        // constantly poll all the switches
        switches |= gpio_get(SWITCH1_PIN) << 0;
        switches |= gpio_get(SWITCH2_PIN) << 1;
        switches |= gpio_get(SWITCH3_PIN) << 2;
        switches |= gpio_get(SWITCH4_PIN) << 3;

        // if the switches are different from the previous states, then update the previous state
        if(switches != last_state){
            last_state = switches;
            switch (switches){
                case 0:
                    printf("Mystery\n");
                    break;
                case 1:
                    printf("Romance\n");
                    break;
                case 2:
                    printf("Horror\n");
                    break;
                case 3: 
                    printf("Fantasy\n");
                    break;
                case 4:
                    printf("Psychological\n");
                    break;
                case 5: 
                    printf("Sci-Fi\n");
                    break;
                case 6:
                    printf("Classics\n");
                    break;
                case 7: 
                    printf("Historical\n");
                    break;
                case 8: 
                    printf("Comedy\n");
                    break;
                case 9:
                    printf("Philosophy\n");
                    break;
                case 10: 
                    printf("Drama\n");
                    break;
                case 11:
                    printf("Biography\n");
                    break;
                case 12: 
                    printf("Adventure\n");
                    break;
                case 13:
                    printf("Manga/Graphic Novel\n");
                    break;
                case 14: 
                    printf("True Crime\n");
                    break;
                case 15: 
                    printf("LGBTQ+\n");
                    break;
                default:
                    printf("Choose!\n");
                    break;
            }
        }

    }
}
