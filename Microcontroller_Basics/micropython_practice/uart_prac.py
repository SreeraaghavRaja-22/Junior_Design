from machine import UART, Pin
import time

uart1 = UART(1, baudrate = 9600, tx = Pin(4), rx = Pin(5))
uart0 = UART(0, baudrate = 9600, tx = Pin(0), rx = Pin(1))
         
tx_data = b'Hello, Pico2\n\r'

uart1.write(tx_data)
time.sleep(0.1)

rx_data = bytes()

while uart0.any() > 0:
    rx_data += uart0.read(1)
    
uart0.write(rx_data)
time.sleep(0.1)

tx_data2 = bytes()
while uart1.any() > 0:
    tx_data2 += uart1.read()
    
for i in range(10):
    print(tx_data2.decode("utf-8"))
        

        
                 
