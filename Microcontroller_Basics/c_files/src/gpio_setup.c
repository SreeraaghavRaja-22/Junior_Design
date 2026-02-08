#include "gpio_setup.h"

void my_gpio_set_function(uint8_t gpio_pin, uint8_t function)
{
    my_gpio_hw->PIN[gpio_pin].CTRL = function & 0x1F; // ctrl register is 5 bits wide

    // set default values for this gpio pin
    my_pads_hw->PAD[gpio_pin] = (0x1 << PADS_BANK0_IE) 
                                | (0x1 << PADS_BANK0_PDE) 
                                | (0x01 << PADS_BANK0_DRIVE) 
                                | (0x1 << PADS_BANK0_SCHMITT);
}

// set the direction of a gpio pin as an output
void my_gpio_set_dir_out(uint8_t gpio_pin)
{
    if(gpio_pin < 32){
        my_sio_hw -> GPIO_OE_SET = (1 << gpio_pin);
    }
    else{
        my_sio_hw -> GPIO_HI_OE_SET = (1 << (gpio_pin - 32));
    }
}

// set the direction of a GPIO pin to an input
void my_gpio_set_dir_in(uint8_t gpio_pin){
    if(gpio_pin < 32){
        my_sio_hw -> GPIO_OE_CLR = (1 << gpio_pin);
    }
    else{
        my_sio_hw -> GPIO_HI_OE_CLR = (1 << (gpio_pin - 32));
    }
}

// read the value oof a GPIO pin
bool my_gpio_get_val(uint8_t gpio_pin){
    if(gpio_pin < 32){
        return (my_sio_hw -> GPIO_IN >> gpio_pin) & 0x1;
    }
    else{
        return (my_sio_hw -> GPIO_HI_IN >> (gpio_pin - 32)) & 0x1; 
    }
}

// set the value of the outputs for the pins based on if it's HI or LOW
void my_gpio_set_val(uint8_t gpio_pin, bool value){
    if(gpio_pin < 32){
        if(value){
            my_sio_hw->GPIO_OUT_SET = (1 << gpio_pin);
        }
        else{
            my_sio_hw->GPIO_OUT_CLR = (1 << gpio_pin);
        }
    }
    else{
        if(value){
            my_sio_hw->GPIO_HI_OUT_SET = (1 << (gpio_pin - 32));
        }
        else{
            my_sio_hw->GPIO_HI_OUT_CLR = (1 << (gpio_pin - 32));
        }
    }
}

void my_gpio_toggle(uint8_t gpio_pin)
{
    if(gpio_pin < 32){
        my_sio_hw -> GPIO_OUT_XOR = (1 << (gpio_pin));
    }
    else{
        my_sio_hw -> GPIO_HI_OUT_XOR = (1 << (gpio_pin - 32));
    }
}
