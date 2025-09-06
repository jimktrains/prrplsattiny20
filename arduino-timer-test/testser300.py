#!/usr/bin/env python3

import serial
from time import sleep

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=1200,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout = 0,
)

ser.isOpen()

while True:
    print(".", end="")
    ser.write(bytes([0xff]))
    cc = ser.read(1024)
    for c in cc:
        b = chr(c)
        if not b.isprintable():
            b = ""
        print(f"{c:02x}  {c:08b}   {b}")
    sleep(1)
