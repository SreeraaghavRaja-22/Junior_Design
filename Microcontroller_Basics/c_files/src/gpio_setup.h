#ifndef _GPIO_SETUP_H_
#define _GPIO_SETUP_H_

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

// Bank 0 controls the GPIO Pins from the user side
#define IO_BANK0_BASE _u(0x40028000)
#define PADS_BANK0_BASE _u(0x40038000)
#define SIO_BASE _u(0xd0000000)

#define GPIO_LOW 0
#define GPIO_HIGH 1

#define DIODE_PIN 0
#define PIEZO_PIN 1
#define SWITCH_PIN 7


// define the registers for each pin
typedef struct my_gpio_pin_t{
    volatile uint32_t STATUS;
    volatile uint32_t CTRL;
} my_gpio_pin_t;

// struct to define the total number of pins = 48
typedef struct my_gpio_hw_t{
    my_gpio_pin_t PIN[48];
} my_gpio_hw_t;

// struct to define the register for the PADs
typedef struct my_gpio_pad_t{
    volatile uint32_t VOLTAGE_SELECT;
    volatile uint32_t PAD[48]; 
} my_gpio_pad_t;

typedef struct my_sio_hw_t{
    volatile uint32_t CPUID;            // 0x000
    volatile uint32_t GPIO_IN;          // 0x004
    volatile uint32_t GPIO_HI_IN;       // 0x008
    volatile uint32_t _reserved0;       // 0x00C
    volatile uint32_t GPIO_OUT;         // 0x010
    volatile uint32_t GPIO_HI_OUT;      // 0x014
    volatile uint32_t GPIO_OUT_SET;     // 0x018
    volatile uint32_t GPIO_HI_OUT_SET;  // 0x01C
    volatile uint32_t GPIO_OUT_CLR;     // 0x020
    volatile uint32_t GPIO_HI_OUT_CLR;  // 0x024
    volatile uint32_t GPIO_OUT_XOR;     // 0x028
    volatile uint32_t GPIO_HI_OUT_XOR;  // 0x02C
    volatile uint32_t GPIO_OE;          // 0x030
    volatile uint32_t GPIO_HI_OE;       // 0x034
    volatile uint32_t GPIO_OE_SET;      // 0x038
    volatile uint32_t GPIO_HI_OE_SET;   // 0x03C
    volatile uint32_t GPIO_OE_CLR;      // 0x040
    volatile uint32_t GPIO_HI_OE_CLR;   // 0x044
    volatile uint32_t GPIO_OE_XOR;      // 0x048
    volatile uint32_t GPIO_HI_OE_XOR;   // 0x04C
} my_sio_hw_t;

#define my_gpio_hw ((my_gpio_hw_t*) IO_BANK0_BASE)
#define my_pads_hw ((my_gpio_pad_t*) PADS_BANK0_BASE)
#define my_sio_hw  ((my_sio_hw_t*) SIO_BASE)

#define PADS_BANK0_OD      7
#define PADS_BANK0_IE      6
#define PADS_BANK0_DRIVE   4
#define PADS_BANK0_PUE     3
#define PADS_BANK0_PDE     2
#define PADS_BANK0_SCHMITT 1
#define PADS_BANK0_SLEWR   0

void my_gpio_set_function(uint8_t gpio_pin, uint8_t function);
void my_gpio_set_dir_out(uint8_t gpio_pin);
void my_gpio_set_dir_in(uint8_t gpio_pin);
bool my_gpio_get_val(uint8_t gpio_pin);
void my_gpio_set_val(uint8_t gpio_pin, bool value);
void my_gpio_toggle(uint8_t gpio_pin);


#endif /*_GPIO_SETUP_H_*/