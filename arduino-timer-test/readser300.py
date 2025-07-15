#!/usr/bin/env python3

import serial
from time import sleep

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=300,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    bytesize=serial.EIGHTBITS,
    timeout = 0,
)

ser.isOpen()

print("Reading")
while True:
    x = chr(0xaa)
    x = input(">") #print(f"> {x}")
    x = x.encode('LATIN1') #input('>').encode('ASCII')
    ser.write(x)

    for _ in range(2):
        cc = ser.read(1024)
        for c in cc:
            b = chr(c)
            if not b.isprintable():
                b = ""
            print(f"{c:02x}  {c:08b}   {b}")

        sleep(0.5)
