#ifndef _COMPONENTS_H
#define _COMPONENTS_H

#include <pico/stdlib.h>

#define DEBOUNCE_TIME 100

#define SWITCH1_PIN 0
#define SWITCH2_PIN 1
#define SWITCH3_PIN 2
#define SWITCH4_PIN 3

#define LED1_PIN    9
#define LED2_PIN    10
#define LED3_PIN    11
#define LED4_PIN    12
#define LED5_PIN    13

#define BUTTON_PIN  15

void set_switch(uint8_t switch_pin);
void initialize_leds(uint8_t led_pin);
void toggle_leds(uint8_t led_pin);

#endif /*_COMPONENTS_H*/