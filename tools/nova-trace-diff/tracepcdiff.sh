#!/bin/bash
# Compare the executed Alto opcode stream (PC + ACs) between our Dorado
# AEmu and ContrAlto, both net-booting the same boot file. Finds the first
# point at which the two emulators diverge — a PC mismatch (control-flow
# divergence) or, while PCs still agree, an AC mismatch (a precise
# behavioral fault at a matching instruction).
#
# Usage:
#   ./tracepcdiff.sh [max-opcodes] [boot-file] [dorado-cycles]
#
# Trace plumbing (cycle-accurate-timing-plan.md Phase 0.1-0.2):
#   ours: DORADO_IFUDISP_TRACE=1 emits one "IFUDISP ..." line per opcode
#         dispatch (cpu.c). pcf= is a BYTE cursor; word PC = pcf>>1.
#         acs= is STK[StkP+0..3] at dispatch time.
#   CA:   CA_TRACEPC=<n> emits "CATRACEPC <seq> <pc-oct> <bus-oct> cyc=..
#         acs=r3,r2,r1,r0" (EmulatorTask.cs). Field 2 is the word PC.
#
# The two emit one record per Alto opcode, so they align index-by-index
# from the first dispatched opcode (no consecutive-PC collapse).
set -euo pipefail

MAX="${1:-5000}"
DOR=/Users/alans/Documents/development/Dorado/dorado
CA=/Users/alans/Documents/development/Dorado/AltoInfo/contralto-headless
BF="${2:-/Users/alans/Documents/development/Dorado/chm/bootfiles/MissileCommand.boot!1}"
# AEmu's first Alto opcode dispatches long after the world loads (~32M);
# use a high ceiling and let `grep -m MAX` SIGPIPE Dorado once it has the
# opcodes it needs, so we don't trace all the way to the ceiling.
CYCLES="${3:-220000000}"

# AC permutation: ours prints STK[StkP+0..3]; CA prints r3,r2,r1,r0 (i.e.
# AC3,AC2,AC1,AC0). AC_PERM maps ours[0..3] -> index into CA's REVERSED
# list [r0,r1,r2,r3]=AC0..3. Default identity (ours[i] == AC i). Tune once
# the data shows the real Stack<->AC correspondence; "skip" disables the
# AC check and only diffs PCs.
AC_PERM="${AC_PERM:-0,1,2,3}"

echo "Dorado: IFUDISP trace (up to $MAX opcodes, $CYCLES-cycle ceiling), $BF" >&2
cd "$DOR"
# Stream stderr through grep -m MAX so Dorado is SIGPIPE'd once it has
# emitted MAX opcode records (bounds both runtime and log size). pipefail
# is off for this line because that early exit is expected.
set +o pipefail
DORADO_IFUDISP_TRACE=1 ./build/dorado --eb worlds/aemu.eb \
  --eftp "$BF" --cycles "$CYCLES" --out /tmp/td_pc_ours.pgm 2>&1 1>/dev/null \
  | grep -a -m "$MAX" '^IFUDISP' > /tmp/td_pc_ours.seq || true
set -o pipefail

echo "ContrAlto: CA_TRACEPC=$MAX, $BF" >&2
cd "$CA"
DOTNET_ROLL_FORWARD=Major CA_NETBOOT=1 CA_BOOTSERVER="$BF" CA_TRACEPC="$MAX" \
  timeout 180 dotnet run -- dummy.dsk /tmp/td_pc_ca.pgm 22 \
  >/tmp/td_pc_ca.out 2>/tmp/td_pc_ca.log || true
grep '^CATRACEPC' /tmp/td_pc_ca.log > /tmp/td_pc_ca.seq || true

# The two emulators boot the image differently (ContrAlto boots the Alto
# directly; Dorado's AEmu boots it through the microcode), so their opcode
# streams start misaligned — the "harmless phase slip" the plan warns about.
# SKIP_OURS / SKIP_CA drop leading records on each side to line the streams
# up on a common landmark before diffing (eyeball the first-PCs lists, then
# set the skips). Default 0 (raw, unaligned).
python3 - "$MAX" "$AC_PERM" "${SKIP_OURS:-0}" "${SKIP_CA:-0}" <<'PY'
import re, sys

import os
limit = int(sys.argv[1])
perm  = sys.argv[2]
skip_ours = int(sys.argv[3])
skip_ca   = int(sys.argv[4])
ac_lag    = int(os.environ.get('AC_LAG', '1'))

def octs(s):
    """Comma-separated octal -> list of ints."""
    return [int(x, 8) for x in s.split(',') if x != '']

def load_ours(path):
    """IFUDISP pc=0o.. pcf=0o.. op=NN alpha=NN ... aacs=o,o,o,o
       -> (ir, [AC0..3], word_pc, rawline). Align/compare on IR (the Alto
       instruction word = op<<8 | alpha), NOT the PC: ours' pcf>>1 is a
       byte cursor relative to BR[31], so its PC namespace differs from
       ContrAlto's absolute PC -- aligning by PC value is coincidental and
       wrong. Use aacs= (FIXED Alto AC window STK[1..4]=AC0..3); the older
       StkP-relative acs= is wrong once StkP moves off 1."""
    rows = []
    for line in open(path):
        m_pcf = re.search(r'pcf=0o([0-7]+)', line)
        m_aacs = re.search(r'aacs=([0-7,]+)', line)
        m_acs  = re.search(r'\bacs=([0-7,]+)', line)
        m_op  = re.search(r'\bop=([0-7]+)', line)
        m_al  = re.search(r'alpha=([0-7]+)', line)
        if not (m_pcf and (m_aacs or m_acs) and m_op):
            continue
        word_pc = int(m_pcf.group(1), 8) >> 1
        op = int(m_op.group(1), 8)
        alpha = int(m_al.group(1), 8) if m_al else 0
        ir = (op << 8) | alpha
        acs = octs((m_aacs or m_acs).group(1))
        rows.append((ir, acs, word_pc, line.rstrip()))
    return rows

def load_ca(path):
    """CATRACEPC <seq> <pc-oct> <bus-oct> cyc=.. acs=r3,r2,r1,r0
       -> (ir, [AC0..3], word_pc, rawline). bus (field 3) is the executed
       instruction word = IR; acs r3,r2,r1,r0 reversed to AC0..3."""
    rows = []
    for line in open(path):
        p = line.split()
        if len(p) < 4 or p[0] != 'CATRACEPC':
            continue
        word_pc = int(p[2], 8)
        ir = int(p[3], 8)
        m_acs = re.search(r'acs=([0-7,]+)', line)
        acs_rev = octs(m_acs.group(1))[::-1] if m_acs else []
        rows.append((ir, acs_rev, word_pc, line.rstrip()))
    return rows

ours = load_ours('/tmp/td_pc_ours.seq')
ca   = load_ca('/tmp/td_pc_ca.seq')

print(f"requested {limit} opcodes")
print(f"ours: {len(ours)} IFUDISP records (skip {skip_ours})")
print(f"CA:   {len(ca)} CATRACEPC records (skip {skip_ca})")
ours = ours[skip_ours:]
ca   = ca[skip_ca:]
if not ours or not ca:
    print("!! one side produced no records — check /tmp/td_pc_*.log")
    sys.exit(0)

print(f"  ours first 6 IRs: {[oct(r[0]) for r in ours[:6]]}")
print(f"  CA   first 6 IRs: {[oct(r[0]) for r in ca[:6]]}")

# Auto-align the boot-phase offset when no skip was given: slide ours by
# 0..127 and pick the shift with the longest INITIAL CONTIGUOUS IR match to
# CA. Contiguous (not total) so a mid-stream INSERTION -- ours executing an
# extra opcode -- is reported as the divergence rather than silently
# realigned away. IR is namespace-independent (unlike PC).
if skip_ours == 0 and skip_ca == 0:
    best_d, best_run = 0, -1
    for d in range(0, 128):
        if d >= len(ours):
            break
        run = 0
        while (d + run < len(ours) and run < len(ca)
               and ours[d + run][0] == ca[run][0]):
            run += 1
        if run > best_run:
            best_run, best_d = run, d
    if best_d:
        print(f"  auto-align: SKIP_OURS={best_d} (initial contiguous IR match "
              f"{best_run} after the boot-phase slip)")
        ours = ours[best_d:]
    else:
        print(f"  auto-align: SKIP_OURS=0 (initial contiguous IR match {best_run})")

ac_check = perm != 'skip'
pmap = None
if ac_check:
    try:
        pmap = [int(x) for x in perm.split(',')]
        assert len(pmap) == 4
    except Exception:
        print(f"!! bad AC_PERM '{perm}', disabling AC check")
        ac_check = False

def ctx(i, n):
    lo, hi = max(0, i - 4), min(n, i + 5)
    for j in range(lo, hi):
        mark = '=>' if j == i else '  '
        print(f"{mark} {j}: ours ir={oct(ours[j][0])} pc={oct(ours[j][2])} "
              f"acs={[oct(a) for a in ours[j][1]]}"
              f"  |  CA ir={oct(ca[j][0])} pc={oct(ca[j][2])} "
              f"acs={[oct(a) for a in ca[j][1]]}")

n = min(len(ours), len(ca))
for i in range(n):
    if ours[i][0] != ca[i][0]:
        print(f"\n>>> FIRST INSTRUCTION (IR) DIVERGENCE at opcode #{i} "
              f"-- ours executes a different Alto instruction word than CA "
              f"(memory-content or fetch divergence)")
        print(f"ours: {ours[i][3]}")
        print(f"CA:   {ca[i][3]}")
        ctx(i, n)
        break
    # AC check is lag-aware: ours' dispatch-time aacs snapshot is taken one
    # opcode before CA's (IFUJump dispatch precedes apply_lc writeback), so
    # ours[i] reflects the ACs CA reports at [i-AC_LAG]. AC_LAG default 1.
    j = i - ac_lag
    if ac_check and j >= 0 and len(ours[i][1]) >= 4 and len(ca[j][1]) >= 4:
        o_ac = ours[i][1]
        c_ac = ca[j][1]
        if any(o_ac[k] != c_ac[pmap[k]] for k in range(4)):
            print(f"\n>>> FIRST AC DIVERGENCE at ours #{i} / CA #{j} (same IR "
                  f"{oct(ours[i][0])}) -- same instruction, different ACs "
                  f"(emulation bug)")
            print(f"ours: {ours[i][3]}")
            print(f"CA:   {ca[j][3]}")
            ctx(i, n)
            break
else:
    if len(ours) != len(ca):
        print(f"\nstreams match (IR{'+AC' if ac_check else ''}) for first {n}; "
              f"lengths differ ours={len(ours)} CA={len(ca)}")
    else:
        print(f"\nstreams match (IR{'+AC' if ac_check else ''}) for all {n} opcodes")
PY
