# Process

## Background

- DAC Equation: DAC_OUT = Vref(DAC_VAL / 2^N - 1)

## Design Steps

- Designed the Circuit and used SPI pins GP(10 - 13)
- SPI1 Module
- Frequency Potentiometer (GP27) = ADC1
- Voltage Potentiometer connects to REF
- MSB Switch 1 = GP28
- LSB Switch 0 = GP22

- To-Do:
  - Figure out how to use the rotary switches

## Code
