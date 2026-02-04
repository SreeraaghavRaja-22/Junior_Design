#include "gpio_setup.h"

void gpio_set_function(uint8_t gpio_pin, uint8_t function)
{
    gpio_hw->PIN[gpio_pin].CTRL = function & 0x1F; // ctrl register is 5 bits wide

    // set default values for this gpio pin
    pads_hw->PAD[gpio_pin] = (0x1 << PADS_BANK0_IE) 
                                | (0x1 << PADS_BANK0_PDE) 
                                | (0x01 << PADS_BANK0_DRIVE) 
                                | (0x1 << PADS_BANK0_SCHMITT);
}

// set the direction of a gpio pin as an output
void gpio_set_dir_out(uint8_t gpio_pin)
{
    if(gpio_pin < 32){
        sio_hw -> GPIO_OE_SET = (1 << gpio_pin);
    }
    else{
        sio_hw -> GPIO_HI_OE_SET = (1 << (gpio_pin - 32));
    }
}

// set the direction of a GPIO pin to an input
void gpio_set_dir_in(uint8_t gpio_pin){
    if(gpio_pin < 32){
        sio_hw -> GPIO_OE_CLR = (1 << gpio_pin);
    }
    else{
        sio_hw -> GPIO_HI_OE_CLR = (1 << (gpio_pin - 32));
    }
}

// read the value oof a GPIO pin
bool gpio_get_val(uint8_t gpio_pin){
    if(gpio_pin < 32){
        return (sio_hw -> GPIO_IN >> gpio_pin) & 0x1;
    }
    else{
        return (sio_hw -> GPIO_HI_IN >> (gpio_pin - 32)) & 0x1; 
    }
}

// set the value of the outputs for the pins based on if it's HI or LOW
void gpio_set_val(uint8_t gpio_pin, bool value){
    if(gpio_pin < 32){
        if(value){
            sio_hw->GPIO_OUT_SET = (1 << gpio_pin);
        }
        else{
            sio_hw->GPIO_OUT_CLR = (1 << gpio_pin);
        }
    }
    else{
        if(value){
            sio_hw->GPIO_HI_OUT_SET = (1 << (gpio_pin - 32));
        }
        else{
            sio_hw->GPIO_HI_OUT_CLR = (1 << (gpio_pin - 32));
        }
    }
}

void gpio_toggle(uint8_t gpio_pin)
{
    if(gpio_pin < 32){
        sio_hw -> GPIO_OUT_XOR = (1 << (gpio_pin));
    }
    else{
        sio_hw -> GPIO_HI_OUT_XOR = (1 << (gpio_pin - 32));
    }
}
