from machine import Pin, Timer

# Global Variable Initializations
led_pin = Pin(1, Pin.OUT)
piezo_pin = Pin(0, Pin.OUT)
switch_pin = Pin("GP7", Pin.IN, Pin.PULL_UP)

# Timer Object Initializations
tim = Timer()
tim2 = Timer()
tim3 = Timer()

# initialize a global flag;
global_flag = 0; 

# function definitions
def led(timer):
    global led_pin
    led_pin.toggle()
    
def piezo(timer):
    global piezo_pin
    piezo_pin.toggle()
    
def switch_handler(Pin):
    tim3.init(mode = Timer.PERIODIC, period=10, callback = switch_timer_handler)

def switch_timer_handler(timer):
    # check if the pin is released
    if(switch_pin.value()):
        # deinitialize the timer
        tim3.deinit()
        
        # initialize global flag
        global global_flag
        # change flag value
        global_flag = 1 if global_flag == 0 else 0
        
        if(global_flag == 0):
            # if the button is pushed then do this thing
            piezo_pin.off()
            tim2.deinit()
            tim.init(freq = 2, mode = Timer.PERIODIC, callback = led)
            # print("Doll")
        else:
            # else do the piezo
            led_pin.off()
            tim.deinit()
            tim2.init(freq = 2000, mode = Timer.PERIODIC, callback = piezo)
            # print("Notes") 
    
def main():
        # 16 interrupt levels on RP2350 but automatically handled by the RP2350
        switch_pin.irq(switch_handler, trigger=Pin.IRQ_FALLING)
        tim.init(freq = 2, mode = Timer.PERIODIC, callback = led)
        
        while True:
           pass

if __name__ == "__main__":
    main()