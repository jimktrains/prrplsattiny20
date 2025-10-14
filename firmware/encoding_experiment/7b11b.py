#!/usr/bin/env python3

from itertools import product
from functools import reduce
import math

def has_two_ones(ii):
    try:
        ifirst = ii.index(1)
        isecond = ii.index(1, ifirst + 1)
        if isecond == (len(ii) - 1):
            return False
    except ValueError:
        return False
    return True

def all_less_than(maxval):
    def f(ii):
        return reduce(lambda v,e : v and e < maxval, ii, True)
    return f

def subcode(m):
    r = []
    if m == 0:
        r = [[]]
    for i in range(m):
        for j in subcode(m - i - 1):
            r.append([i + 1] + j)
    return r

def generate_bitstrings(s):
    mapping=[]
    for i in s:
        b = 0
        x = ""
        y = ""
        for j in i:
            x += str(b) * j
            b = 1-b
            y += str(b) * j
        mapping.append(x)
    return mapping

def check_cross(myii, g, fcns):
    good = []
    for i in g:
        ii = myii[i]
        conflict = False
        for j in g:
            jj = myii[j]
            for r in range(2):
                for rr in range(2):
                    if conflict:
                        continue
                    for f in fcns:
                        conflict |= f(i, ii, j, jj, r, rr)
        if not conflict:
            good.append(i)
    return good

def check_against_other(i,ii,j,jj,r,rr):
    kk = ii[r] + jj[rr]
    for n in range(1, len(ii[r])):
        nn = kk[n:n+len(ii[r])]
        # print("  ", n, len(ii), nn, end=" ")
        # print(ii == nn)
        if ii[r] == nn:
            print(f"nn {i}_{r}+{j}_{rr}@{n}")
            print("  ", ii[r], jj[rr])
            print("  ", kk)
            print("  ", kk[:n], nn, kk[n+len(ii[r]):])
            return True
    return False

def good_check_after_stop(ii):
    for n in range(1, len(ii)):
        ee = ii[n:] + ("1" * n)
        if ii == ee:
            print(f"ee {i}@{n}")
            return False
    return True

def good_check_start_stop_bits(ii):
    return ((not check_start_bit) or (ii[0] == '0')) and  ((not check_stop_bit) or ii[-1] == '1')

def good_check_parity(ii):
    if not check_parity:
        return True
    if check_stop_bit:
            ii = ii[:-1]
    return reduce(lambda v,e : v ^ (e == '1'), ii, even_parity)

def good_check(m, g, fcns):
    good = []
    for i in g:
        is_good = True
        for f in fcns:
            is_good &= f(m[i])
        if is_good:
            good.append(i)
    return good



m = 8
max_run = 4
even_parity = True
check_stop_bit = True
check_start_bit = True
check_parity = False
print_raw_codeword = False

if check_start_bit:
    m += 1
if check_stop_bit:
    m += 1
if check_parity:
    m += 1

parity = "odd"
if even_parity:
    parity = "even"
if not check_parity:
    parity = "None"

print("#pragma once")
print("// copyright AGPL3")
print("")
print("#include <stdint.h>")
print(f"// codeword_length={m}")
print(f"// {max_run=}")
print(f"// {check_stop_bit=}")
print(f"// {check_start_bit=}")
print(f"// {check_parity=}")
print(f"// {print_raw_codeword=}")
print(f"// {parity=}")
print("")
has_start_bit = 0
if check_start_bit:
    has_start_bit = 1

has_stop_bit = 0
if check_stop_bit:
    has_stop_bit = 1
# print(f"#define ENC7b11b_CODEWORD_LENGTH {m}")
# print(f"#define ENC7b11b_HAS_START_BIT {has_start_bit}")
# print(f"#define ENC7b11b_HAS_STOP_BIT {has_stop_bit}")
# print(f"#define ENC7b11b_MAX_RUN {max_run}")
# print("")



s = subcode(m)
s = filter(has_two_ones, s)
s = filter(all_less_than(max_run + 1), s)
s = list(s)

all_count = len(s)
# print(f"{all_count=}")
log_all_count = int(math.log2(len(s)))
# print(f"all {log_all_count}:{m}")

mapping = generate_bitstrings(s)
good = list(range(len(mapping)))
good = good_check(mapping, good, [good_check_after_stop, 
                                  good_check_start_stop_bits,
                                  good_check_parity])
good = check_cross(mapping, good, [check_against_other])

codeword_count = len(good)
print(f"// {codeword_count=}")
log_codeword_count = 0
if codeword_count != 0:
    log_codeword_count = int(math.log2(codeword_count))
print(f"// code_rate={log_codeword_count}:{m}")



mid_a = int(m / 2)
mid_b = int((m + 1) / 2)
mids = [mid_a]
if mid_a != mid_b:
    mids.append(mid_b)
balance = [0] * (m+1)
for mi, i in enumerate(good):
    fii = mapping[i]
    zs = fii.count('0')
    balance[zs] += 1

print("")
print("// balance stats")
print("// zeros count")
for i,b in enumerate(balance):
    stars = "*" * int((b / (codeword_count/2)) * 20)
    ismid = " "
    if i in mids:
        ismid = "#"
    l = f"// {ismid}{i: 4} {b: 5} {stars}"
    print(l.strip())
print("")



max_d_val = 1 << log_codeword_count 
print(f"#define ENC7b11b_CODEWORD_COUNT {codeword_count}")
print("uint8_t enc7b11b_codewords[ENC7b11b_CODEWORD_COUNT] = {")
for mi, i in enumerate(good):
    fii = mapping[i]
    ii = fii
    if not print_raw_codeword:
        if check_start_bit:
            ii = ii[1:]
        if check_stop_bit:
            ii = ii[:-1]
        if check_parity:
            ii = ii[:-1]
    # reversing because LSB is sent first, which
    # is the opposite of how I have been printing
    # and examining them.
    rii = "".join(reversed(ii))
    ii = int(rii, 2)

    dk = "D"
    dki = mi
    if mi >= max_d_val:
        dk = "K"
        dki -= max_d_val
    print(f"  0x{ii:02x},  // i:{mi:03}(0x{mi:02x}) {dk}.x{dki:02x} 0b{ii:08b} fullframe:{fii}")
print("};")
# print("")
# print("uint8_t enc7b11b_encode(uint8_t b, uint8_t i);")
# print("uint16_t enc7b11b_encode_full(uint8_t b);")
# print("void enc7b11b_start_decode();")
# print("void enc7b11b_continue_decode(uint8_t v);")
# print("uint8_t enc7b11b_finish_decode();")

