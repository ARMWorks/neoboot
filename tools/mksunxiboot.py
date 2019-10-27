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
        sum &= 0xFFFFFFFF
    return sum

with open(sys.argv[1], 'rb') as f:
    stage1 = f.read(MAX_SIZE - HEADER_LEN)
    stage1 += b'\0' * (BLOCK_SIZE + HEADER_LEN - ((len(stage1) + HEADER_LEN) % BLOCK_SIZE))
    stage2 = f.read()

with open(sys.argv[1], 'wb') as f:
    instruction = 0xEA000000 | ((HEADER_LEN // 4 - 2) & 0x00FFFFFF)

    header = bytearray(HEADER_LEN)
    struct.pack_into('<I8sII3sB', header, 0, instruction, b'eGON.BT0', 0,
        len(header) + len(stage1), b'SPL', SPL_VERSION)

    checksum = gen_checksum(header + stage1)
    struct.pack_into('<I', header, 12, checksum)

    f.write(header)
    f.write(stage1)
    f.write(stage2)