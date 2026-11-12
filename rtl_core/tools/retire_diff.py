#!/usr/bin/env python3
"""First difference between two HPS_RETIRE files (sim_hps), for one task: an
overlap run against an RTL_OV_HOLD one. The task switches fall at different
instructions in the two runs, so the comparison is of the instructions one task
retired: the address of each (the previous record's PC) and, where the task
kept the processor, the state it left.
usage: retire_diff.py A B [task=0] [context=12]"""
import sys, struct
N = 12
names = "pc task T Link StkP RBase MemBase Q Cnt ShC fl sv".split()
task = int(sys.argv[3]) if len(sys.argv) > 3 else 0
ctx = int(sys.argv[4]) if len(sys.argv) > 4 else 12
CMP = (0, 2, 3, 4, 5, 6)          # pc T Link StkP RBase MemBase
def stream(fn):
    f = open(fn, "rb"); prev = None; i = 0
    while True:
        r = f.read(2 * N)
        if len(r) < 2 * N: return
        u = struct.unpack("<12H", r)
        if prev is not None and prev[1] == task:
            yield (i, prev[0], u)          # record index, address, state after
        prev = u; i += 1
def fmt(e):
    i, addr, u = e
    return f"#{i} at {addr:o}: " + " ".join(f"{n}={v:o}" for n, v in zip(names, u))
hist = []; n = 0
sa, sb = stream(sys.argv[1]), stream(sys.argv[2])
for ea, eb in zip(sa, sb):
    bad = ea[1] != eb[1]
    if not bad and ea[2][1] == task and eb[2][1] == task:
        bad = any(ea[2][k] != eb[2][k] for k in CMP)
    if bad:
        for x, y in hist[-ctx:]:
            print("  A", fmt(x)); print("  B", fmt(y))
        print(f"--- first difference, task {task} instruction {n}")
        print("A:", fmt(ea)); print("B:", fmt(eb))
        for k, (x, y) in enumerate(zip(sa, sb)):
            if k >= ctx: break
            print("  A", fmt(x)); print("  B", fmt(y))
        sys.exit(1)
    hist.append((ea, eb)); n += 1
    if len(hist) > 64: hist = hist[-32:]
print(f"no difference in {n} task-{task} instructions")
