#ifndef _COMPONENTS_H
#define _COMPONENTS_H

#include <pico/stdlib.h>

#define DEBOUNCE_TIME 100

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

void set_switch(uint8_t switch_pin){
    // default direction is input
    gpio_init(switch_pin);

    // set pin to have internal pullup - unnecessary
    gpio_pull_up(switch_pin);

    // set the interrupts 
    gpio_set_irq_enabled(switch_pin, GPIO_IRQ_EDGE_FALL, true);
}

void initialize_leds(uint8_t led_pin){
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
}

void toggle_leds(uint8_t led_pin){
    gpio_xor_mask64(1u << led_pin);
}


#endif /*_COMPONENTS_H*/