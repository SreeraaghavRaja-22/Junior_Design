#ifndef _MAIN_INIT_H
#define _MAIN_INIT_H

#include "adc_init.h"
#include "components.h"

void main_init(void){
    for(int i = LED1_PIN; i < LED1_PIN + 5; i++){initialize_leds(i);}
    adc_start();
}

#endif /*_MAIN_INIT_H*/