from machine import SPI, ADC, Pin, Timer
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

LUT_LENGTH = 64 
CONTROL_WORD = 0xF # this loads new data into input reg A and sends that to the 

freq_pot = ADC(27)        # connect to ADC1
conversion_factor = 3.3 / (65535)
conversion_factor_norm = conversion_factor / 3.3

count : int = 0
index : int = 0

# ----------------------- INITIALIZATION ----------------------- #
# initialize SPI module
spi = SPI(1, baudrate = 400000, polarity = 0, phase = 0, bits = 8, firstbit = SPI.MSB)
# set chip select separately to toggle and set it as an output pin with default value high (Active-Low CS)
cs = Pin(13, Pin.OUT, value = 1)
# initialize timer
tim = Timer()
# initialize 16-bit array to store data that we're sending - 2 bytes
send = bytearray(2)
# choose wave_type
wave = sine_lookup_table
# timer 2 for the freq pot
tim2 = Timer()
# initialize the dac update timer
samp_freq = 100
dac_update_freq = samp_freq * 64
tolerance = 1
# ----------------------- INITIALIZATION ----------------------- #

def pack_data(data):
    # pack data and bottom two bits are don't cares, so don't matter
    return ((CONTROL_WORD << 12) | ((data & 0x3FF) << 2)) 


def update_dac(timer):
    global index
    
    packet = pack_data(wave[index])
    
    send[0] = (packet >> 8) & 0xFF
    send[1] = packet & 0xFF
    
    # choose DAC as device
    cs.low()
    spi.write(send)
    utime.sleep_us(1)
    cs.high()
    
    index = (index + 1) % 64
    
def update_freq(timer):
    global dac_update_freq
    global samp_freq
    
    freq_reading = round(freq_pot.read_u16() * conversion_factor_norm * 90 + 10)
    if abs(samp_freq - freq_reading) > tolerance:
        samp_freq = round(freq_reading)
        dac_update_freq = samp_freq * 64
        tim.init(mode = Timer.PERIODIC, freq = dac_update_freq, callback = update_dac)
    

def main():
   
   
    # initialize callback timers
    tim.init(mode = Timer.PERIODIC, freq = dac_update_freq, callback = update_dac)
    tim2.init(mode = Timer.PERIODIC, freq = 20, callback = update_freq)
   
    while True:
        pass
        
        
    # print(freq_reading)
    # print(sine_lookup_table[count])
    # count = count + 1
    # if count > 63:
        # count = 0
    # sleep(0.5)

main()
    
        

