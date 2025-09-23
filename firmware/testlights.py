#!/usr/bin/env python3

import serial
import sys
import time
import curses
import os
from unicodedata import category
import random

# configure the serial connections (the parameters differs on the device you are connecting to)
ser = serial.Serial(
    port='/dev/ttyUSB0',
    baudrate=115200,
    bytesize=serial.EIGHTBITS,
    parity=serial.PARITY_NONE,
    stopbits=serial.STOPBITS_ONE,
    timeout = 0,
    write_timeout = 0
)

if not ser.isOpen():
    raise "Not open!"

ser.dtr = True
time.sleep(0.5)
ser.dtr = False

x= None
while x != '>':
    x = ser.read(1).decode('ascii')
    print(x, end="")
print("")

ser.write(b's')
x = None
while x != '\n':
    x = ser.read(1).decode('ascii')
    if len(x) != 0:
        print(x, end="", flush=True)

asp = ([b"S", b"A", b"C"] * 33)
# asp += [bytes(x) for x in range(129,256)]

random.shuffle(asp)

for i in range(1, len(asp)):
    x = asp[i]
    y = asp[i-1]
    ser.write(b"!")
    ser.write(x)
    ser.write(y)
    time.sleep(1)
