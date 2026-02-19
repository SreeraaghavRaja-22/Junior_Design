# -----------------------------------------
#                 NOTES
# -----------------------------------------
"""
Dieter Steinhauser
9/2023
Parallel LCD class test.
"""
from time import sleep
from LCD import LCD

# -----------------------------------------
#           LCD Instantiation
# -----------------------------------------

# add this code to the beginning of your main file to instantiate the LCD

# GPIO 
lcd = LCD(enable_pin=18,           # Enable Pin, int
         reg_select_pin=16,        # Register Select, int
         data_pins=[19, 20, 21, 22]   # Data Pin numbers for the upper nibble. list[int]
         )

lcd.init()
lcd.clear()
#lcd.cursor_on()
#lcd.blink()

lcd.print(f'LCD Test          ')
lcd.go_to(0,1)
lcd.print(f'Dieter S.         ')
sleep(2)



while True:
    sleep(1)
    #print(lcd.enable_pin)
    #print(lcd.reg_select_pin)
    #lcd.blink()
    pass
    
