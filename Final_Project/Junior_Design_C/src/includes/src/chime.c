#include <pico/stdio.h>
#include "../chime.h"
#include "hardware/gpio.h"
#include <pico/time.h>
#include "math.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

uint8_t sine_table[SAMPLE_COUNT * 2];

static volatile int sample_index = 0;
static struct repeating_timer dac_timer;

static bool dac_sample_irq(struct repeating_timer *t){
    gpio_put(SPI_CS, 0);
    spi_write_blocking(SPI_PORT, &sine_table[sample_index * 2], 2);
    gpio_put(SPI_CS, 1);
    sample_index = (sample_index + 1) % SAMPLE_COUNT;
    return true;
}

void chime_start(int sample_rate_hz){
    sample_index = 0;
    int period_us = 1000000 / sample_rate_hz;
    add_repeating_timer_us(-period_us, dac_sample_irq, NULL, &dac_timer);
}

void chime_stop(void){
    cancel_repeating_timer(&dac_timer);
    gpio_put(SPI_CS, 1);
}

void spi_start(void){
    // initialize SPI0 with a clock freq of 1.8M
    spi_init(SPI_PORT, 8000 * 1000);
    spi_set_format(SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);
    // set all the used pins
    gpio_set_function(SPI_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(SPI_MOSI, GPIO_FUNC_SPI);
    gpio_init(SPI_CS);

    // set the CS to be an output
    gpio_set_dir(SPI_CS, GPIO_OUT);

    // CS idles as high
    gpio_put(SPI_CS, 1);
}

void generate_sine(void){
    for(int i = 0; i < SAMPLE_COUNT; i++){
        float s = sinf(2.0f * M_PI * i / SAMPLE_COUNT);
        uint16_t sample = (uint16_t)((s + 1.0f) / 2.0f * 4095.0f);
        uint16_t word = 0x3000 | (sample & 0x0FFF);
        sine_table[i * 2]     = (word >> 8) & 0xFF;
        sine_table[i * 2 + 1] = word & 0xFF;
    }
}
