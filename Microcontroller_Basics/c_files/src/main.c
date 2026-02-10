#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/timer.h"
#include "pico/time.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"

#define DIODE_PIN 1
#define PIEZO_PIN 0
#define SWITCH_PIN 7

#define HZ_2_IN_MS -250 // timer has to toggle every 250 ms
#define KHZ_2_IN_MS -250 // timer has to toggle every 250 us
#define DEBOUNCE_TIME 100 // typical debounce time is 10 ms
// #define LED_PIN 25>
// #define UART_TX_PIN 0
// #define UART_RX_PIN 1
// #define BAUDRATE 115200

volatile bool led_state = false, timer_enable = true, choose_comp = false; 

bool toggle_diode(__unused struct repeating_timer *t){
    led_state ^= 1; 
    gpio_put(DIODE_PIN, led_state);
    return true; 
}

bool toggle_piezo(__unused struct repeating_timer *t){
    gpio_xor_mask64(1u << PIEZO_PIN);
    return true;
}

int64_t gpio_alarm_callback(alarm_id_t id, __unused void *user_data){
    // re-enable the IRQ after the alarm goes off
    gpio_set_irq_enabled(SWITCH_PIN, GPIO_IRQ_EDGE_FALL, true);
    return 0;
}

void gpio_irq_callback(uint gpio, uint32_t event_mask){
    // disable the interrupt 
    gpio_set_irq_enabled(SWITCH_PIN, GPIO_IRQ_EDGE_FALL, false);

    // make it possible to change timers
    timer_enable = true; 

    // choose the timer and make it different from the previous one
    choose_comp ^= 1; 

    // ignore the callback when the alarm isn't set (should test with true too)
    add_alarm_in_ms(DEBOUNCE_TIME, gpio_alarm_callback, NULL, false);
}

int main(void){
    stdio_init_all();   

    // initialize all three pins
    gpio_init(DIODE_PIN);
    gpio_init(PIEZO_PIN);
    gpio_init(SWITCH_PIN);

    // set pin directions
    gpio_set_dir(DIODE_PIN, GPIO_OUT);
    gpio_set_dir(PIEZO_PIN, GPIO_OUT);
    gpio_set_dir(SWITCH_PIN, GPIO_IN);

    // can internally define this pin as a pull-up switch config
    gpio_pull_up(SWITCH_PIN); 
    
    // initialize the struct for the diode and piezo timers
    struct repeating_timer active_timer; 

    // add a repeating timer with callback for the diode and piezo
    add_repeating_timer_ms(-250, toggle_diode, NULL, &active_timer);

    // enable the falling edge interrupt for the switch pin (enable the interrupt)
    gpio_set_irq_enabled(SWITCH_PIN, GPIO_IRQ_EDGE_FALL, true); 
    gpio_set_irq_callback(gpio_irq_callback);
    irq_set_enabled(IO_IRQ_BANK0, true);
    
    for(;;){
        if(timer_enable){

            cancel_repeating_timer(&active_timer);

            // turn off both components (for a very small unit of time)
            gpio_put(PIEZO_PIN, 0);
            gpio_put(DIODE_PIN, 0);

            if(!choose_comp){
                // add the diode timer
                add_repeating_timer_ms(-250, toggle_diode, NULL, &active_timer);
            }
            else{
                // add piezo timer 
                add_repeating_timer_us(-250, toggle_piezo, NULL, &active_timer);
            }

            timer_enable = false; 
        }
    } 

    return 0;
}
