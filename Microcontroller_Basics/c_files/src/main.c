#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/timer.h"
#include "pico/time.h"

#define DIODE_PIN 25
#define PIEZO_PIN 1
#define SWITCH_PIN 7

#define HZ_2_IN_MS -250 // timer has to toggle every 250 ms
#define KHZ_2_IN_MS -250 // timer has to toggle every 250 us

// #define LED_PIN 25
// #define UART_TX_PIN 0
// #define UART_RX_PIN 1
// #define BAUDRATE 115200

volatile bool led_state = false, piezo_state = false; 

bool toggle_diode(__unused struct repeating_timer *t){
    led_state ^= 1; 
    gpio_put(DIODE_PIN, led_state);
    return true; 
}

bool toggle_piezo(__unused struct repeating_timer *t){
    gpio_xor_mask64(1u << PIEZO_PIN);
    return true;
}

int main(void){
    stdio_init_all();   
    gpio_init(DIODE_PIN);
    gpio_set_dir(DIODE_PIN, GPIO_OUT);
    
    struct repeating_timer diode_timer, piezo_timer; 

    add_repeating_timer_ms(-250, toggle_diode, NULL, &diode_timer);
    add_repeating_timer_us(-250, toggle_piezo, NULL, &piezo_timer);
    
    for(;;){} 

    return 0;
}
