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
    id = 'dman'
    name = 'DiffMan'
    longname = 'Differential Manchester'
    desc = 'Asynchronous, serial bus.'
    license = 'gplv2+'
    inputs = ['logic']
    outputs = ['dman']
    tags = ['Embedded/industrial']
    channels = (
        {'id': 'rx', 'name': 'RX', 'desc': 'UART receive line'},
    )
    options = (
        {'id': 'baudrate', 'desc': 'Baud rate', 'default': 115200},
    )
    annotations = (
        ('data', 'data'),
    )
    annotation_rows = (
        ('data-bits', 'bits', (0,)),
    )
    binary = (
        ('data', 'data'),
    )

    def __init__(self):
        self.reset()

    def reset(self):
        self.samplerate = None
        self.last_trans = None
        self.penult_trans = None
        self.last_val = None
        self.transitions = []
        self.last_was_zero = False
        self.bits = None

    def start(self):
        self.out_python = self.register(srd.OUTPUT_PYTHON)
        self.out_binary = self.register(srd.OUTPUT_BINARY)
        self.out_ann = self.register(srd.OUTPUT_ANN)

    def metadata(self, key, value):
        if key == srd.SRD_CONF_SAMPLERATE:
            self.samplerate = value
            self.bit_width = float(self.samplerate) / float(self.options['baudrate'])

    def handle_edge(self):
        if self.last_trans is not None:
            delta_samplenum = self.samplenum - self.last_trans
            dbits = delta_samplenum / self.bit_width

            pdbits = 0
            if self.penult_trans is not None:
                pdelta_samplenum = self.samplenum - self.penult_trans
                pdbits = pdelta_samplenum / self.bit_width
            if pdbits > 3:
                self.last_was_zero = False
            if self.last_was_zero:
                self.last_was_zero = False
                self.put(self.penult_trans, self.samplenum, self.out_ann, [0,["0"]])
                self.put(self.penult_trans, self.samplenum, self.out_python, 0)
            else:
                self.last_was_zero = True
                if dbits > 1.5 and dbits < 3:
                    self.last_was_zero = False
                    self.put(self.last_trans, self.samplenum, self.out_ann, [0,["1"]])
                    self.put(self.last_trans, self.samplenum, self.out_python, 1)
        self.penult_trans = self.last_trans
        self.last_trans = self.samplenum

    def decode(self):
        wait_cond = {0:'e'}
        while True:
            (d,) = self.wait(wait_cond)
            self.handle_edge()
