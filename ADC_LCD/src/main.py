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

# Rolling Average Filter
data = []
WIN_SIZE = 50
prev_filtered_resistance = 0
THRESHOLD = 0.05

while True:
    # read the voltage and convert to adc value
    voltage_read = voltage_adc.read_u16() * conversion_factor
    # find r2 value
    
    if(3.3 - voltage_read <= 0):
        r2 = 1e7
    else: 
        r2 = r1 * ((voltage_read) / (3.3 - voltage_read))
    
    ## convert the value using linearized equation (used exponential) for normal values and 
    if (r2 < 650e3):
       r2_out = -16.263 + 0.9866 * (r2) - 2e-7 * (r2)**2
    else:
       r2_out = 0.821 * r2 + 15.477e3
       
    if prev_filtered_resistance > 0:
        
        diff = abs(r2_out - prev_filtered_resistance) / prev_filtered_resistance
        
        if diff > THRESHOLD:
            data.clear()
            print("Resistance Changed!")
    
       
    data.append(r2_out)
    
    if len(data) > WIN_SIZE:
        # remove the oldest sample if the length of the array is greater than the window size
        data.pop(0) 
    
    # take the mean of the data values and the length
    r2_out_filter = sum(data) / len(data)
    r2_out_t = round(r2_out_filter, 3)
    prev_filtered_resistance = r2_out_filter
       
        
    # output the adc value to LCD display
    out_string = "R = " + str(r2_out_t) + chr(0xf4)
    
    # clear before printing
    lcd.clear()
    
    # print out to lcd
    # if(r2 < 1000 or r2 > 1e6):
        # lcd.print("Out of Range")
    # else:
    lcd.print(out_string)
        
    # print out resistance value
    print(out_string)
    
    sleep(0.5)
    
    
