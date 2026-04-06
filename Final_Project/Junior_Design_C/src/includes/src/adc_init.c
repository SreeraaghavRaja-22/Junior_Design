#include "hardware/adc.h"
#include "../adc_init.h"

void adc_start(void){
    adc_init(); 
    adc_gpio_init(POT_PIN);
    adc_gpio_init(PHO_PIN);
}