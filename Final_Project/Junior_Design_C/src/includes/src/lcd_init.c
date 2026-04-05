#include "pico/stdlib.h"
#include "../lcd_init.h"

// got this from claude, but I know how it works

static void lcd_write(uint8_t data){
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &data, 1, false);
}

static void lcd_pulse_enable(uint8_t data){
    lcd_write(data | 0x04);  // EN high
    sleep_us(1);
    lcd_write(data & ~0x04); // EN low
    sleep_us(50);
}

static void lcd_send_nibble(uint8_t nibble, uint8_t mode){
    uint8_t data = (nibble & 0xF0) | mode | 0x08; // 0x08 = backlight on
    lcd_pulse_enable(data);
}

static void lcd_send_byte(uint8_t data, uint8_t mode){
    lcd_send_nibble(data & 0xF0, mode);        // upper nibble
    lcd_send_nibble((data << 4) & 0xF0, mode); // lower nibble
}

void lcd_command(uint8_t cmd){
    lcd_send_byte(cmd, 0x00); // RS=0
}

void lcd_data(uint8_t data){
    lcd_send_byte(data, 0x01); // RS=1
}

void lcd_init(void){
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    sleep_ms(50);

    // force 8-bit mode 3 times
    lcd_send_nibble(0x30, 0x00);
    sleep_ms(5);
    lcd_send_nibble(0x30, 0x00);
    sleep_us(150);
    lcd_send_nibble(0x30, 0x00);

    // switch to 4-bit mode
    lcd_send_nibble(0x20, 0x00);

    // function set: 4-bit, 2 lines, 5x8 font
    lcd_command(0x28);
    // display on, cursor off, blink off
    lcd_command(0x0C);
    // clear display
    lcd_command(0x01);
    sleep_ms(2);
    // entry mode: increment, no shift
    lcd_command(0x06);
}

void lcd_clear(void){
    lcd_command(0x01);
    sleep_ms(2);
}

void lcd_set_cursor(uint8_t row, uint8_t col){
    uint8_t offsets[] = {0x00, 0x40};
    lcd_command(0x80 | (offsets[row] + col));
}

void lcd_print(const char *str){
    while(*str){
        lcd_data(*str++);
    }
}