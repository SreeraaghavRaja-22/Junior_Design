from machine import SPI, ADC, Pin
from time import sleep
import utime

# ----------------------- LUTs ----------------------- #
sine_lookup_table = (
    512, 562, 611, 660, 707, 753, 796, 836,
    873, 907, 937, 963, 984, 1001, 1013, 1021,
    1023, 1021, 1013, 1001, 984, 963, 937, 907,
    873, 836, 796, 753, 707, 660, 611, 562,
    512, 461, 412, 363, 316, 270, 227, 187,
    150, 116,  86,  60,  39,  22,  10,   2,
      0,   2,  10,  22,  39,  60,  86, 116,
    150, 187, 227, 270, 316, 363, 412, 461
)

square_lookup_table = 32 * (0,) + 32 * (1023,)

triangle_lookup_table = (
     32,  64,  96, 128, 160, 192, 224, 256,
    288, 320, 352, 384, 416, 448, 480, 512,
    543, 575, 607, 639, 671, 703, 735, 767,
    799, 831, 863, 895, 927, 959, 991, 1023,
    991, 959, 927, 895, 863, 831, 799, 767,
    735, 703, 671, 639, 607, 575, 543, 512,
    480, 448, 416, 384, 352, 320, 288, 256,
    224, 192, 160, 128,  96,  64,  32,   0,
)

sawtooth_lookup_table = (
    0, 16, 32, 48, 64, 81, 97, 113, 
    129, 146, 162, 178, 194, 211, 227, 243, 
    259, 276, 292, 308, 324, 341, 357, 373, 
    389, 406, 422, 438, 454, 471, 487, 503, 
    519, 536, 552, 568, 584, 601, 617, 633, 
    649, 666, 682, 698, 714, 731, 747, 763, 
    779, 796, 812, 828, 844, 861, 877, 893, 
    909, 926, 942, 958, 974, 991, 1007, 1023
)


# ----------------------- LUTs ----------------------- #


# spi = SPI.init(1, baudrate=400000)

# cs = Pin(4, mode=Pin.OUT, value=1)

# try:
#    cs(0)                        # select peripheral 
#    spi.write(b"12345678")       # Write 8 bytes, and dont' care about receiving data
# finally:
#    cs(1)                        # Deselect peripheral

freq_pot = ADC(27)        # connect to ADC1
conversion_factor = 3.3 / (65535)
count : int = 0

while True:
    freq_reading = freq_pot.read_u16() * conversion_factor    
    print(freq_reading)
    print(sine_lookup_table[count])
    count = count + 1
    if count >= 1024:
        count = 0
    sleep(0.5)

