#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "gpio_setup.h"
#include "uart_setup.h"

#define LED_PIN 25
#define UART_TX_PIN 0
#define UART_RX_PIN 1

int main(void){
    stdio_init_all();
    printf("1");

    // initialize uart0 
    my_uart_init(my_uart0_hw, BAUDRATE);
    printf("2");

    // Configure GPIO Pins for UART0
    my_gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    my_gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    printf("3");

    // set up LED for blinking 
    my_gpio_set_function(LED_PIN, GPIO_FUNC_SIO);
    my_gpio_set_dir_out(LED_PIN);
    printf("4");

    // send message through uart
    my_uart_put_string(my_uart0_hw, "hi there, how are you?");
    my_uart_put_string(my_uart0_hw, "Send a response back!");
    printf("5");

    while(1){
        // get char from keyboard
        char c = my_uart_get_c(my_uart0_hw);
        my_uart_put_c(my_uart0_hw, c);
        printf("6");

        // toggle led pin
        my_gpio_toggle(LED_PIN);
        printf("6");

        // check for an enter (carriage return = CR)
        if(c == '\r'){
            // add a newline (LF)
            my_uart_put_c(my_uart0_hw, '\n');
            printf("7");
        }

        printf("Hi");
    }

    return 0;
}
