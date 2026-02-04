#include "uart_setup.h"
#include "hardware/clocks.h"


uint8_t uart_init(my_uart_hw_t *uart, uint baudrate)
{
    // set up baud rate for UART and include an error statement too
    // default clock speed is 150MHz (assumption because of the peripheral clock speed)
    uint32_t uart_clk = clock_get_hz(clk_peri);

    // baud_div = uart_clk / (16 * baudrate) 
    // multiply by 8 for better precision on dividers
    uint32_t baud_rate_div = (8 * uart_clk / baudrate);

    // get the integer and fractional baud rate dividers
    uint32_t int_div = (baud_rate_div >> 7); // 8 / 128 = 1/16
    uint32_t frac_div = ((baud_rate_div & 0x7F) + 1) >> 1;

    // Disable UART before setting everything up 
    uart->CR = 0; 

    // set up the parameters for the line control register high
    uart->LCR_H = (0b11 << 5) | (0b1 << 4); // set up word length to be 8-bits, no parity, and FIFO en
    
    // set the baud divs
    uart->IBRD = int_div; 
    uart->FBRD = frac_div;

    // Configure the Control Register
    // Enable UART and the RX and TX Pins
    uart->CR = (1 << 0) |   
               (1 << 8) |
               (1 << 9);

    return 0;
}

void uart_put_c(my_uart_hw_t *uart, char c)
{
    // while the transmit fifo is full don't send data
    while(uart->FR & (0x01 << 5));
    uart->DR = c;
}

char uart_get_c(my_uart_hw_t *uart)
{
    char c;
    // wait until the receive fifo is not empty to collect data
    while(uart->FR & (0x01 << 4));
    c = (char)uart->DR;
    return (c);
}

void uart_put_string(my_uart_hw_t *uart, const char *string)
{
    // i'm sorry but I straight up got this from claude
    while(*string){
        uart_put_c(uart, *string++);
    }
}
