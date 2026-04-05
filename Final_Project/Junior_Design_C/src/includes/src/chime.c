#include <pico/stdio.h>
#include "hardware/dma.h"
#include "../chime.h"
#include "hardware/gpio.h"
#include "math.h"

int dma_channel;
uint16_t sine_table[SAMPLE_COUNT];

void dma_start(void){
    // claim unused channel
    dma_channel = dma_claim_unused_channel(true);

    dma_channel_config c = dma_channel_get_default_config(dma_channel);

    // set read address to sine table, increment after each read
    channel_config_set_read_increment(&c, true);

    // set the write address to SPI FIFO, don't increment it
    channel_config_set_write_increment(&c, false);

    // 16-bit transfers
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);

    // set the ring size to 9 since 2^9 = 512
    channel_config_set_ring(&c, false, 9);

    // Request New Data when SPI0_TX is ready for new data
    channel_config_set_dreq(&c, DREQ_SPI0_TX);

    // configure the dma_channel
    dma_channel_configure(dma_channel, &c, &spi_get_hw(SPI_PORT)->dr, sine_table, SAMPLE_COUNT, true);
}

void spi_start(void){
    // initialize SPI0 with a clock freq of 1M
    spi_init(SPI_PORT, 1000 * 1000);
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
        // this makes the sample go from 0 to 1023
        uint16_t sample = (uint16_t)((sinf(2 * M_PI * i / SAMPLE_COUNT) + 1) * 511.5f);
        // shift the data left by 
        sine_table[i] = (sample << 2) & 0x3FFC;
    }
}