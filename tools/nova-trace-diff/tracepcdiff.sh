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

limit = int(sys.argv[1])
perm  = sys.argv[2]
skip_ours = int(sys.argv[3])
skip_ca   = int(sys.argv[4])

def octs(s):
    """Comma-separated octal -> list of ints."""
    return [int(x, 8) for x in s.split(',') if x != '']

def load_ours(path):
    """IFUDISP pc=0o.. pcf=0o.. ... op=NNN ... acs=o,o,o,o
       -> (word_pc, [STK order acs], op, rawline)."""
    rows = []
    for line in open(path):
        m_pcf = re.search(r'pcf=0o([0-7]+)', line)
        m_acs = re.search(r'acs=([0-7,]+)', line)
        m_op  = re.search(r'\bop=([0-7]+)', line)
        if not (m_pcf and m_acs):
            continue
        word_pc = int(m_pcf.group(1), 8) >> 1        # byte cursor -> word PC
        acs = octs(m_acs.group(1))
        op  = int(m_op.group(1), 8) if m_op else -1
        rows.append((word_pc, acs, op, line.rstrip()))
    return rows

def load_ca(path):
    """CATRACEPC <seq> <pc-oct> <bus-oct> cyc=.. acs=r3,r2,r1,r0
       -> (word_pc, [AC0..3], rawline). CA acs is r3,r2,r1,r0 so reverse
       it to AC0..3."""
    rows = []
    for line in open(path):
        p = line.split()
        if len(p) < 3 or p[0] != 'CATRACEPC':
            continue
        word_pc = int(p[2], 8)
        m_acs = re.search(r'acs=([0-7,]+)', line)
        acs_rev = octs(m_acs.group(1))[::-1] if m_acs else []   # -> AC0..3
        rows.append((word_pc, acs_rev, line.rstrip()))
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

print(f"  ours first 6 PCs: {[oct(r[0]) for r in ours[:6]]}")
print(f"  CA   first 6 PCs: {[oct(r[0]) for r in ca[:6]]}")

# Auto-align the boot-phase offset when no skip was given: slide ours by
# 0..127 and pick the shift whose PC stream best matches CA's over the
# first 50 opcodes. Printed, never silent, so it can't mislead.
if skip_ours == 0 and skip_ca == 0:
    best_d, best_score = 0, -1
    for d in range(0, 128):
        w = min(50, len(ours) - d, len(ca))
        if w <= 0:
            break
        score = sum(1 for k in range(w) if ours[d + k][0] == ca[k][0])
        if score > best_score:
            best_score, best_d = score, d
    if best_d:
        print(f"  auto-align: SKIP_OURS={best_d} (PC match {best_score}/50 over "
              f"the boot-phase slip; set SKIP_OURS=0 explicitly to disable)")
        ours = ours[best_d:]

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
        opc = f" op={oct(ours[j][2])}" if ours[j][2] >= 0 else ""
        print(f"{mark} {j}: ours pc={oct(ours[j][0])}{opc} acs={[oct(a) for a in ours[j][1]]}"
              f"  |  CA pc={oct(ca[j][0])} acs={[oct(a) for a in ca[j][1]]}")

n = min(len(ours), len(ca))
for i in range(n):
    if ours[i][0] != ca[i][0]:
        print(f"\n>>> FIRST PC DIVERGENCE at opcode #{i}")
        print(f"ours: {ours[i][3]}")
        print(f"CA:   {ca[i][2]}")
        ctx(i, n)
        break
    if ac_check and len(ours[i][1]) >= 4 and len(ca[i][1]) >= 4:
        o_ac = ours[i][1]
        c_ac = ca[i][1]
        if any(o_ac[k] != c_ac[pmap[k]] for k in range(4)):
            print(f"\n>>> FIRST AC DIVERGENCE at opcode #{i} (PC matches: {oct(ca[i][0])})")
            print(f"ours: {ours[i][3]}")
            print(f"CA:   {ca[i][2]}")
            ctx(i, n)
            break
else:
    if len(ours) != len(ca):
        print(f"\nstreams match (PC{'+AC' if ac_check else ''}) for first {n}; "
              f"lengths differ ours={len(ours)} CA={len(ca)}")
    else:
        print(f"\nstreams match (PC{'+AC' if ac_check else ''}) for all {n} opcodes")
PY
