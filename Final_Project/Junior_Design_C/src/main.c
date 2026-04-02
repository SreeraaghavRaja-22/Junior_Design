#include <stdio.h>
#include <stdbool.h>

#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/cyw43_arch.h"

#include "hardware/adc.h"
#include "components.h"

int main(void){
    stdio_init_all(); 

    // ensure that wifi can be initialized
    if(cyw43_arch_init()) {
        printf("WIFI FAILED!");
        return -1;
    }

    // initialize LEDS from the component library
    initialize_leds();
    
    for(;;){
        // toggle all the leds
        toggle_leds();
        sleep_ms(1000);
    }
}