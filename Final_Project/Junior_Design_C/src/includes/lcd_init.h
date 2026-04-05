#ifndef _LCD_INIT_H
#define _LCD_INIT_H


#include "pico/stdio.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define I2C_SDA 4
#define I2C_SCL 5
#define LCD_ADDR 0x27 // Default Slave Address of the PCF8574T

void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(uint8_t row, uint8_t col);
void lcd_print(const char *str);
void lcd_command(uint8_t cmd);
void lcd_data(uint8_t data);void lcd_init(void);

#endif /* _LCD_INIT_H */