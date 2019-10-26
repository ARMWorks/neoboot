#!/usr/bin/env python

import os
import struct
import sys

HEADER_LEN = 96
BLOCK_SIZE = 512
MAX_SIZE = 32768
SPL_VERSION = 2

def gen_checksum(code):
    sum = 0
    for i in range(0, len(code), 4):
        sum += struct.unpack_from('<I', code, i)[0]
    return sum & 0xFFFFFFFF

with open(sys.argv[1], 'r+b') as f:
    header = bytearray(HEADER_LEN)
    header[:4] = f.read(4)
    f.seek(HEADER_LEN)
    code = f.read(MAX_SIZE - HEADER_LEN)
    code += b'\0' * (BLOCK_SIZE - (len(code) + HEADER_LEN) % BLOCK_SIZE)
    checksum = gen_checksum(header + code)
    struct.pack_into('<8sII3sB', header, 4, b'eGON.BT0', checksum,
        len(header + code), b'SPL', SPL_VERSION)
    f.seek(0)
    f.write(header)