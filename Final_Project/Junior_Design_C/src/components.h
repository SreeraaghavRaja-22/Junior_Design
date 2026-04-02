#ifndef _COMPONENTS_H
#define _COMPONENTS_H

#include <pico/stdlib.h>

#define SWITCH_PIN1 0
#define SWITCH_PIN2 1
#define SWITCH_PIN3 2
#define SWITCH_PIN4 3

#define LED1_PIN    9
#define LED2_PIN    10
#define LED3_PIN    11
#define LED4_PIN    12
#define LED5_PIN    13

#define BUTTON_PIN  15


void initialize_leds(void){
    gpio_init(LED1_PIN);
    gpio_init(LED2_PIN);
    gpio_init(LED3_PIN);
    gpio_init(LED4_PIN);
    gpio_init(LED5_PIN);

    gpio_set_dir(LED1_PIN, GPIO_OUT);
    gpio_set_dir(LED2_PIN, GPIO_OUT);
    gpio_set_dir(LED3_PIN, GPIO_OUT);
    gpio_set_dir(LED4_PIN, GPIO_OUT);
    gpio_set_dir(LED5_PIN, GPIO_OUT);

    gpio_pull_up(LED1_PIN);
    gpio_pull_up(LED2_PIN);
    gpio_pull_up(LED3_PIN);
    gpio_pull_up(LED4_PIN);
    gpio_pull_up(LED5_PIN);
}

void toggle_leds(void){
    gpio_xor_mask64(1u << LED1_PIN);
    gpio_xor_mask64(1u << LED2_PIN);
    gpio_xor_mask64(1u << LED3_PIN);
    gpio_xor_mask64(1u << LED4_PIN);
    gpio_xor_mask64(1u << LED5_PIN);
}


#endif /*_COMPONENTS_H*/