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

# Reset the arduino so we start in a known state
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

asp = [b"S", b"A", b"C"]
# Differential manchester
# 0 = TT (transition transition)
# 1 = NN (no transition)
# UART start bit is a low transition
# so, to send (LSB first) 1000
# it would be: NNTTTTTT
# since that first N is a L, then LLHLHLHL
# and we need to send the bits (accounting for the first L from the start
# bit) 0101010, since that's 7, just tack on a 1 since the end bit is high.
# then we need 2 end bits plus the extra 1 we add to single the end of a
# transmission. Since we write these MSB first, we flip it to 10101010
#
# val -> lsb bin -> transitions -> line vals -> reversed and lopped
# 0x0 -> 0000    -> TTTT        -> 01010101  -> 01010101 -> 0x55
# 0x1 -> 1000    -> NTTT        -> 00101010  -> 10101010 -> 0xaa
# 0x2 -> 0100    -> TNTT        -> 01001010  -> 10101001 -> 0xa9
# 0x3 -> 1100    -> NNTT        -> 00110101  -> 01010110 -> 0x56
# 0x4 -> 0010    -> TTNT        -> 01010010  -> 10100101 -> 0xa5
# 0x5 -> 1010    -> NTNT        -> 00101101  -> 01011010 -> 0x5a
# 0x6 -> 0110    -> TNNT        -> 01001101  -> 01011001 -> 0x59
# 0x7 -> 1110    -> NNNT        -> 00110010  -> 10100110 -> 0xa6
# 0x8 -> 0001    -> TTTN        -> 01010100  -> 10010101 -> 0x95
# 0x9 -> 1001    -> NTTN        -> 00101011  -> 01101010 -> 0x6a
# 0xa -> 0101    -> TNTN        -> 01001011  -> 01101001 -> 0x69
# 0xb -> 1101    -> NNTN        -> 00110100  -> 10010110 -> 0x96
# 0xc -> 0011    -> TTNN        -> 01010011  -> 01100101 -> 0x65
# 0xd -> 1011    -> NTNN        -> 00101100  -> 10011010 -> 0x9a
# 0xe -> 0111    -> TNNN        -> 01001100  -> 10011001 -> 0x99
# 0xf -> 1111    -> NNNN        -> 00110011  -> 01100110 -> 0x66
inttodman = [
  bytes([0b01010101]),
  bytes([0b10101010]),
  bytes([0b10101001]),
  bytes([0b01010110]),
  bytes([0b10100101]),
  bytes([0b01011010]),
  bytes([0b01011001]),
  bytes([0b10100110]),
  bytes([0b10010101]),
  bytes([0b01101010]),
  bytes([0b01101001]),
  bytes([0b10010110]),
  bytes([0b01100101]),
  bytes([0b10011010]),
  bytes([0b10011001]),
  bytes([0b01100110])
]

# 0x00 -> 0000 -> all off
# 0x01 -> 0001 -> clear
# 0x02 -> 0010 -> approach
# 0x03 -> 0011 -> stop
# 0x04 -> 0100 -> sop
asp = [inttodman[0x3], inttodman[0x2], inttodman[0x1]]
#asp = [b"\x0f", b"\x0e", b"\x0d"]

asp = asp * 33
# asp += [bytes(x) for x in range(129,256)]

#random.shuffle(asp)

x = b"O"
x = b"\x00"

x = inttodman[0x0]
y = x

delay = (1.0 / 300.0) * 40
print(delay)

sop = b"!"
sop = inttodman[0x4]

def print_packet(packet):
    for p in packet:
        print(f"{p:02x}", end=" ")
    print("->", end=" ")
    for p in packet:
        if p == ord(sop):
            print("!", end="")
        elif p == ord(inttodman[0x1]):
            print("C", end="")
        elif p == ord(inttodman[0x2]):
            print("A", end="")
        elif p == ord(inttodman[0x3]):
            print("S", end="")
        elif p == ord(inttodman[0x3]):
            print("S", end="")
        elif p == ord(inttodman[0x0]):
            print("O", end="")
        else:
            print(f"\\x{p:02x}", end="")
    print("")



for i in range(len(asp)):
    z = y
    y = x
    x = asp[i]


    packet = sop + x + y + z
    print("> ", end ="")
    print_packet(packet)

    ser.write(sop)
    time.sleep(delay)
    ser.write(x)
    time.sleep(delay)
    ser.write(y)
    time.sleep(delay)
    ser.write(z)
    time.sleep(delay)

    c = time.time()
    print("< ", end="")
    r = None
    while r != '\n' and (time.time() - c) < 1:
        r = ser.read(1)
        if r >= b' ' and r <= b'~':
            r = r.decode('ascii')
        else:
            r = "".join([f"\\x{rr:02x}" for rr in r])

        if len(r) != 0:
            print(r, end="", flush=True)
    print("")

    #time.sleep(1)
