##
## This file is part of the libsigrokdecode project.
##
## Copyright (C) 2011-2014 Uwe Hermann <uwe@hermann-uwe.de>
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, see <http://www.gnu.org/licenses/>.
##

import sigrokdecode as srd
from common.srdhelper import bitpack
from math import floor, ceil

'''
'''

class Decoder(srd.Decoder):
    api_version = 3
    id = 'jskprrpls'
    name = 'jskprrpls'
    longname = 'jskprrpls'
    desc = 'jskprrpls'
    license = 'gplv2+'
    inputs = ['dman']
    outputs = ['jskprrpls']
    tags = ['Embedded/industrial']
    annotations = (
        ('commands', 'commands'),
    )
    annotation_rows = (
        ('commands', 'commands', (0,)),
    )
    cmds = ["All Off", "Clear", "Approach", "Stop", "SOP"]

    def __init__(self):
        self.reset()

    def reset(self):
        self.bits = []

    def start(self):
        self.out_python = self.register(srd.OUTPUT_PYTHON)
        self.out_binary = self.register(srd.OUTPUT_BINARY)
        self.out_ann = self.register(srd.OUTPUT_ANN)

    def metadata(self, key, value):
        if key == srd.SRD_CONF_SAMPLERATE:
            self.samplerate = value
            self.bit_width = float(self.samplerate) / float(self.options['baudrate'])

    def decode(self, startsample, endsample, v):
        self.bits.append((startsample, endsample, v))
        cmd = v
        ss = startsample
        found = True
        extra = False
        for i in range(1,5):
            if len(self.bits) < (i+1):
                found = (i == 4)
                break
            if ss != self.bits[-(i+1)][1]:
                found = (i == 4)
                break
            if i == 4:
                found = False
            else:
                ss = self.bits[-(i+1)][0]
                cmd <<= 1
                cmd |= self.bits[-(i+1)][2]

        if found:
            cmd_txt = f"Unknown 0x{cmd:x}"
            if cmd < len(self.cmds):
                cmd_txt = self.cmds[cmd]
            self.put(ss, endsample, self.out_ann, [0,[cmd_txt]])
