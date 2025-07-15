#!/usr/bin/env python3

import serial

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyACM0',
    baudrate=115200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS
)

ser.isOpen()

print("Reading")
last_c = None
count = 0
while True:
    cc = ser.read(1)
    for c in cc:
        # if last_c != c:
        b = chr(c)
        # if count != 0:
        #     print(count, end="")
        count = 0
        print(f"\n{c:02x}  {c:08b}   {b}  ", end="")
        last_c = c
        # else:
        #     count += 1
