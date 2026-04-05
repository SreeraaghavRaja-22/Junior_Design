#include "../components.h"
#include "../adc_init.h"
#include "../main_init.h"
#include "../lcd_init.h"
#include "../chime.h"

void main_init(void){
    for(int i = LED1_PIN; i < LED1_PIN + 5; i++){initialize_leds(i);}
    for(int i = SWITCH1_PIN; i < SWITCH1_PIN + 4; i++){set_switch(i);}
    set_switch(BUTTON_PIN);
    adc_start();
    lcd_init();
    spi_start();
    generate_sine();
}
    