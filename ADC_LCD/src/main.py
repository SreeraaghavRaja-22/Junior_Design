import machine
import time

from time import sleep
from LCD import LCD

# LCD Instantiation

# GPIO 
lcd = LCD(enable_pin=18,           # Enable Pin, int
         reg_select_pin=16,        # Register Select, int
         data_pins=[19, 20, 21, 22]   # Data Pin numbers for the upper nibble. list[int]
         )

lcd.init()
lcd.clear()


# initialize ADC
voltage_adc = machine.ADC(26)
# 16-bits because micropython upscales the 12 bit value by padding the lower 4 bits with 0
conversion_factor = 3.3 / 65535
# resistor 1 value
r1 = 9780

while True:
    # read the voltage and convert to adc value
    voltage_read = voltage_adc.read_u16() * conversion_factor
    # find r2 value
    r2 = r1 * ((voltage_read) / (3.3 - voltage_read))
    # round to 3 decimal places
    r2 = round(r2,3)
    
    # output the adc value to LCD display
    out_string = "R = " + str(r2) + chr(0xf4)
    
    # clear before printing
    lcd.clear()
    
    # print out to lcd
    if(r2 < 1000 or r2 > 1e6):
        lcd.print("Out of Range")
    else:
        lcd.print(out_string)
        
    # print out resistance value
    print(out_string)
    
    sleep(1)
    
    
