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
- **Loading Content into the DAC**
  - Control Code
  - Input Code
  - 2 bits Don't care
  - use code 0x09 for control of DAC
  - then load code + don't care bits
  - Pack data
- SPI for DAC
  - [SPI Mode 0](https://www.analog.com/en/resources/analog-dialogue/articles/introduction-to-spi-interface.html)
  - Copy timing diagram and explain why it's SPI Mode 0

- To-Do:
  - Figure out how to use the rotary switches

## Code
