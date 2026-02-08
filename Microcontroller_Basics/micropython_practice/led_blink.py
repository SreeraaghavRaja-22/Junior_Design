from machine import Pin, Timer

led = Pin("LED", Pin.OUT) # can also use 25 instead of LED
tim = Timer()

def tick(timer):
    global led
    led.toggle()

tim.init(freq = 2, mode = Timer.PERIODIC, callback = tick)