#ifndef _CHIME_H
#define _CHIME_H

#include <pico/stdio.h>
#include "hardware/spi.h"
#define SAMPLE_COUNT 64
#define SPI_PORT spi0
#define SPI_CS      17
#define SPI_SCK     18
#define SPI_MOSI    19

// sample_rate_hz = desired_frequency * SAMPLE_COUNT
// e.g. 440 Hz tone -> 440 * 256 = 112640
#define CHIME_SAMPLE_RATE (440 * SAMPLE_COUNT)

extern uint8_t sine_table[SAMPLE_COUNT * 2];

void generate_sine(void);
void spi_start(void);
void chime_start(int sample_rate_hz);
void chime_stop(void);

#endif /*_CHIME_H*/