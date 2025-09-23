#!/usr/bin/env python3

import serial
import sys
import time
import curses
import os
from unicodedata import category

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

if False:
    print("** Getting ID")
    ser.write(b'i\n')
    x = None
    while x != '\n':
        x = ser.read(1).decode('ascii')
        if len(x) != 0:
            print(x, end="", flush=True)

if False:
    ser.write(b'u\n')
    x = None
    while x != '\n':
        x = ser.read(1).decode('ascii')
        print(x, end="", flush=True)

    with open(sys.argv[1], 'rb') as file:
        data = file.read()
        ser.write(data)

    x = None
    xx = ""
    while True:
        x = ser.read(1).decode('ascii')
        print(x, end="", flush=True)
        if x == '\n':
            xx = ""
        else:
            xx += x
        if xx == "Done":
            break

ser.write(b's')
x = None
while x != '\n':
    x = ser.read(1).decode('ascii')
    if len(x) != 0:
        print(x, end="", flush=True)
input("Go to console")

key = None
def console(win):
    x = None
    win.nodelay(True)
    win.clear()                
    while 1:          
        try:
            x = ser.read(1).decode('ascii')
        except e:
            x = "".join([f"\\x{xx:02x}" for xx in x])

        if len(x) != 0:
            if category(x[0]) == 'Cc':
                x = ord(x[0])
                x = f"\\x{x:02x}"
            win.addstr(x)
        try:                 
           key = win.getkey().encode('ascii')         
           ser.write(key)
           if key == 27:
              break           
        except Exception as e:
            if "no input" in str(e):
                pass
            else:
                raise e
curses.wrapper(console)
