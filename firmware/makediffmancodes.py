#!/usr/bin/env python3


bins = []
trns = []
for v in range(16):
    bs = []
    ts = []
    for x in range(4):
        b = v & (1 << (x)) != 0
        if b:
            ts.append("N")
            bs.append("1")
        else:
            ts.append("T")
            bs.append("0")
    bins.append("".join(bs))
    trns.append("".join(ts))

linevals = []
for i, ts in enumerate(trns):
    last_line_val = 1 # pre-startbit uart line value
    lv = []
    for t in ts:
        last_line_val = 1 - last_line_val
        if t == "N":
            lv.append(str(last_line_val))
            lv.append(str(last_line_val))
        elif t == "T":
            lv.append(str(last_line_val))
            last_line_val = 1 - last_line_val
            lv.append(str(last_line_val))
    linevals.append("".join(lv))

vals_to_send = []
for i,lv in enumerate(linevals):
    # lop off the initial 0 because that's the uart start bit
    # ensure transition at the end
    lv = lv[1:] + str(1 - int(lv[-1]))
    lv = lv[::-1]
    vals_to_send.append(lv)


joined = []
for i,lv in enumerate(vals_to_send):
    joined.append("0b" + ("".join([str(l) for l in lv])))

print("# val -> lsb bin -> transitions -> line vals -> reversed and lopped")
for i in range(len(joined)):
    h = hex(int(vals_to_send[i], 2))
    print(f"# 0x{i:x} -> {bins[i]}    -> {trns[i]}        -> {linevals[i]}  -> {vals_to_send[i]} -> {h}")

print("inttodman = [\n  bytes([" + ("]),\n  bytes([".join(joined)) + "])\n]")
