#ifndef _ADC_INIT_H
#define _ADC_INIT_H

#include "hardware/adc.h"

#define POT_PIN 27 // for ADC0
#define POT_ADC 1
#define ADC_CONV_FACTOR (3.3f/((1 << 12) - 1))

// initialize the adc
void adc_start(void){
    adc_init(); 
    adc_gpio_init(POT_PIN);
    adc_select_input(POT_ADC);
}


#endif /*_ADC_INIT_H*/