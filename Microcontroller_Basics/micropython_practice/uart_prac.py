from machine import UART, Pin
import time


uart1 = UART(1, baudrate = 9600, tx = Pin(6), rx = Pin(7))
uart0 = UART(0, baudrate = 9600, tx = Pin(16), rx = Pin(17))
         
tx_data = b'Hello, Pico2\n\r'

print("sending data\n");

while True:
    uart1.write(tx_data)
# print("sleeping")
# time.sleep(0.1)
# 
# print("rec data")
# rx_data = bytes()
# 
# print("checking data")
# while uart0.any() > 0:
#     
#     print("check if data ready")
#     rx_data += uart0.read(1)
#     
# print(rx_data.decode('utf-8'))
        

        
                 
