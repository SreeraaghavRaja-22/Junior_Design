from machine import Pin, Timer

led_pin = Pin(6, Pin.OUT)

# initialize piezo pin
piezo_pin = Pin(7, Pin.OUT)

# initialize the switch pin

tim = Timer()

def led(timer):
    global led_pin
    led_pin.toggle()
    
def piezo(timer):
    global piezo_pin
    piezo_pin.toggle()
    
def main(timer):
    

# if the button is pushed then do this thing
tim.init(freq = 2, mode = Timer.PERIODIC, callback = led)

# else do the piezo
tim.init(freq = 2000, mode = Timer.PERIODIC, callback = piezo)