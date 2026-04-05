#ifndef _CHIME_H
#define _CHIME_H

#include <pico/stdio.h>
#include "hardware/spi.h"
#include "hardware/dma.h"


#define SAMPLE_COUNT 256
#define SPI_PORT spi0
#define SPI_CS      17
#define SPI_SCK     18
#define SPI_MOSI    19


extern uint8_t sine_table[SAMPLE_COUNT * 2];
extern int dma_channel;

void generate_sine(void);
void spi_start(void);
void dma_start(void);

#endif /*_CHIME_H*/