#ifndef _ADC_INIT_H
#define _ADC_INIT_H

#include "hardware/adc.h"

#define POT_PIN 27 // for ADC1
#define POT_ADC 1
#define PHO_PIN 26 // for ADC0
#define PHO_ADC 0
#define ADC_CONV_FACTOR (3.3f/((1 << 12) - 1))

// initialize the adc
void adc_start(void);

#endif /*_ADC_INIT_H*/