#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "../main_init.h"
#include "../semaphores.h"
#include "../lcd_init.h"
#include "math.h"

volatile bool start_flag = 0;
volatile bool progress_flag = 0;
volatile bool genre_flag = 0;

void Heartbeat_Task(void *pvParameters) {
    bool rst = 0; 
    int prev_leds_on = 0; 
    char buf[16];
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

           

            if(abs(prev_leds_on-leds_on) >= 1){
                int percent = (leds_on * 20);
                snprintf(buf, sizeof(buf), "Progress: %d%%", percent);
                lcd_clear();
                lcd_print(buf);
            }
            prev_leds_on = leds_on;
            rst = 1; 
        }
        else{
            if(rst){
                lcd_clear();
                lcd_print("Genre Selection");
                rst = 0;
            }
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
                if(start_flag){
                    if(!progress_flag && !genre_flag){
                    progress_flag = 1; 
                    }
                    else if(progress_flag && !genre_flag){
                        progress_flag = 0; 
                        genre_flag = 1; 
                    }
                    else{
                        start_flag = 0;
                        genre_flag = 0;
                    }
                }
                else{
                    start_flag = 1; 
                }
                
            }

            xSemaphoreTake(xLedMutex, portMAX_DELAY);
            for(int i = LED1_PIN; i < LED1_PIN + 5; i++) gpio_put(i, 0);
            xSemaphoreGive(xLedMutex);

            while(xSemaphoreTake(xButtonSemaphore, 0) == pdTRUE);
        }
    }
}

void Switch_Task(void *pvParameters){
    uint8_t last_state = 0; 
    bool rst = 0;
    for(;;){
        uint8_t switches = 0; 
        if(genre_flag){
            // default = 1
            rst = 1; 
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
                        lcd_clear();
                        lcd_print("Mystery");
                        // printf("Mystery\n");
                        break;
                    case 1:
                        lcd_clear();
                        lcd_print("Romance");
                        break;
                    case 2:
                        lcd_clear();
                        lcd_print("Horror");
                        break;
                    case 3: 
                        lcd_clear();
                        lcd_print("Fantasy");
                        break;
                    case 4:
                        lcd_clear();
                        lcd_print("Psychological");
                        break;
                    case 5: 
                        lcd_clear();
                        lcd_print("Sci-Fi");
                        break;
                    case 6:
                        lcd_clear();
                        lcd_print("Classics");
                        break;
                    case 7: 
                        lcd_clear();
                        lcd_print("Historical");
                        break;
                    case 8: 
                        lcd_clear();
                        lcd_print("Comedy");
                        break;
                    case 9:
                        lcd_clear();
                        lcd_print("Philosophy");
                        break;
                    case 10: 
                        lcd_clear();
                        lcd_print("Classics");
                        break;
                    case 11:
                        lcd_clear();
                        lcd_print("Biography");
                        break;
                    case 12: 
                        lcd_clear();
                        lcd_print("Adventure");
                        break;
                    case 13:
                        lcd_clear();
                        lcd_print("Manga/Comics");
                        break;
                    case 14: 
                        lcd_clear();
                        lcd_print("True Crime");
                        break;
                    case 15: 
                        lcd_clear();
                        lcd_print("LGBTQ+");
                        break;
                    default:
                        lcd_clear();
                        lcd_print("CHOOSE! Choose?");
                        break;
                }
            }
        }
        else{
            if(rst){
                lcd_clear();
                lcd_print("Luminous Library");
                rst = 0;
            }
        }
    }
}
