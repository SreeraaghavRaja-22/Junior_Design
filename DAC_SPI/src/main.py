from machine import SPI, Pin


spi = SPI.init(0, baudrate=400000)

cs = Pin(4, mode=Pin.OUT, value=1)

try:
    cs(0)
