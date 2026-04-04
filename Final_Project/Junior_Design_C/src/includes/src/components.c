#include "pico/stdlib.h"
#include "../components.h"

void set_switch(uint8_t switch_pin){
    // default direction is input
    gpio_init(switch_pin);

    // set pin to have internal pullup - unnecessary
    gpio_pull_up(switch_pin);
}

void initialize_leds(uint8_t led_pin){
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
}

void toggle_leds(uint8_t led_pin){
    gpio_xor_mask64(1u << led_pin);
}
