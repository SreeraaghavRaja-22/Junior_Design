import machine
import time
import math

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
r1 = 32978
# linearization equation
lin_eq = 8913 * math.exp(2.57) * 10**(-6) 

while True:
    # read the voltage and convert to adc value
    voltage_read = voltage_adc.read_u16() * conversion_factor
    # find r2 value
    r2 = r1 * ((voltage_read) / (3.3 - voltage_read))
    # round to 3 decimal places
    # r2 = round(r2,3)
    
    ## convert the value using linearized equation (used exponential)
    # r2_out = 8913*e^(2.57) * 10^(-6)(r2)
    r2_out = -2500 + 0.0512 * (r2) + 5.02e-8 * (r2)**2
    r2_out = round(r2_out)
    
    # output the adc value to LCD display
    out_string = "R = " + str(r2) + chr(0xf4)
    
    # clear before printing
    lcd.clear()
    
    # print out to lcd
    # if(r2 < 1000 or r2 > 1e6):
        # lcd.print("Out of Range")
    # else:
    lcd.print(out_string)
        
    # print out resistance value
    print(out_string)
    
    sleep(1)
    
    
