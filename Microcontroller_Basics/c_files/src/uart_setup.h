#ifndef UART_SETUP_H_
#define UART_SETUP_H_

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"


#define LED_PIN 0
#define PIEZO_PIN 1
#define SWITCH_PIN 7

#define UART_0_BASE _u(0x40070000)
#define UART_1_BASE _u(0x40078000)

#define BAUDRATE 115200


typedef struct my_uart_hw_t{
    volatile uint32_t DR;           // 0x000: data register
    volatile uint32_t RSR;          // 0x004: receive status register
    uint32_t _reserved0[4];         // 0x008 - 0x017: reserved block
    volatile uint32_t FR;           // 0x018: flags register
    uint32_t _reserved1;            // 0x01C: reserved space again
    volatile uint32_t ILPR;         // 0x020: low-power counter register
    volatile uint32_t IBRD;         // 0x024: integer baud rate divisor register
    volatile uint32_t FBRD;         // 0x028: fractional baud rate divisor register
    volatile uint32_t LCR_H;        // 0x02c: line control register high
    volatile uint32_t CR;           // 0x030: control register
    volatile uint32_t IFLS;         // 0x034: interrupt fifo level select register
    volatile uint32_t IMSC;         // 0x038: interrupt mask set/clear register
    volatile uint32_t RIS;          // 0x03c: raw interrupt status register
    volatile uint32_t MIS;          // 0x040: masked interrupt status register
    volatile uint32_t ICR;          // 0x044: interrupt clear register
    volatile uint32_t DMACR;        // 0x048: DMA control register
    // ignore rest of the blocks since they are read only   

} my_uart_hw_t;

#define uart0_hw (my_uart_hw_t *) UART_0_BASE
#define uart1_hw (my_uart_hw_t *) UART_1_BASE

uint8_t uart_init(my_uart_hw_t *uart, uint baudrate);
void uart_put_c(my_uart_hw_t *uart, char c);
char uart_get_c(my_uart_hw_t *uart);
void uart_put_string(my_uart_hw_t *uart, const char *string);

#endif /*UART_SETUP_H*/
