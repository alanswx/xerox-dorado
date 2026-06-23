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

# AC comparison. ours' aacs = STK[1..4] = AC0,AC1,AC2,AC3 (Start.mc). CA
# prints acs=R[3],R[2],R[1],R[0]; in the Alto the ACs map to R in REVERSE
# (AC0=R[3], AC1=R[2], AC2=R[1], AC3=R[0]), so CA's printed list is ALREADY
# AC0,AC1,AC2,AC3 -- do NOT reverse it (an earlier version did, which made
# the AC check compare ours and CA in opposite orders and misfire). AC_PERM
# maps ours[0..3] -> index into CA's AC0..3 list; identity is correct.
#
# The AC check is ADVISORY and defaults to "skip" (IR-only). Exact AC
# equality is unreliable for two reasons that do NOT indicate a bug:
#   (1) Writeback lag -- ours snapshots at IFUJump dispatch (before the
#       previous opcode's apply_lc writeback), CA at IR<- time, so ours'
#       AC values lag CA's by ~1 opcode (handled here by matching ca[i] OR
#       ca[i-1], lag-tolerant).
#   (2) PC-namespace offset -- ours' PC is a byte cursor relative to BR[31];
#       CA's is absolute. ACs that hold PC-relative addresses therefore
#       differ by the boot-phase slip constant even when execution agrees.
# The IR (executed-instruction-word) stream is the trustworthy divergence
# signal: a real behavioral fault shows up as a control-flow (IR) divergence
# via a skip/branch. Set AC_PERM=0,1,2,3 to enable the advisory AC check.
AC_PERM="${AC_PERM:-skip}"

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
        # rtrap=1 marks an IFUJump diverted to AEmuReschedule: the held-back
        # opcode is re-dispatched on the NEXT record, not executed here. A
        # plain Alto (ContrAlto) has no such trap, so skip it — counting it
        # would falsely report a duplicate opcode in the stream.
        m_rt = re.search(r'rtrap=([01])', line)
        if m_rt and m_rt.group(1) == '1':
            continue
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
    """CATRACEPC <seq> <pc-oct> <bus-oct> cyc=.. acs=R3,R2,R1,R0
       -> (ir, [AC0..3], word_pc, rawline). bus (field 3) is the executed
       instruction word = IR. In the Alto the ACs map to R in reverse
       (AC0=R[3] .. AC3=R[0]); CA prints acs=R[3],R[2],R[1],R[0], which is
       therefore ALREADY AC0,AC1,AC2,AC3 -- take it as-is, do NOT reverse."""
    rows = []
    for line in open(path):
        p = line.split()
        if len(p) < 4 or p[0] != 'CATRACEPC':
            continue
        word_pc = int(p[2], 8)
        ir = int(p[3], 8)
        m_acs = re.search(r'acs=([0-7,]+)', line)
        acs = octs(m_acs.group(1)) if m_acs else []
        rows.append((ir, acs, word_pc, line.rstrip()))
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
    # AC check (advisory) is lag-tolerant: ours' aacs snapshot lags CA's by
    # ~1 opcode (IFUJump dispatch precedes apply_lc writeback), so accept a
    # match against CA at the same index OR one earlier. Only flag when ours
    # matches NEITHER -- and even then it may be a PC-namespace address
    # offset, not a bug (see header). The IR stream is the real signal.
    if ac_check and len(ours[i][1]) >= 4:
        o_ac = ours[i][1]
        cands = [ca[k][1] for k in (i, i - ac_lag) if k >= 0 and len(ca[k][1]) >= 4]
        if cands and all(any(o_ac[k] != c[pmap[k]] for k in range(4)) for c in cands):
            print(f"\n>>> AC MISMATCH at ours #{i} (same IR {oct(ours[i][0])}) "
                  f"-- ours' ACs match CA at neither lag 0 nor {ac_lag}; "
                  f"may be a real fault OR a PC-namespace address offset")
            print(f"ours: {ours[i][3]}")
            print(f"CA:   {ca[i][3]}")
            ctx(i, n)
            break
else:
    if len(ours) != len(ca):
        print(f"\nstreams match (IR{'+AC' if ac_check else ''}) for first {n}; "
              f"lengths differ ours={len(ours)} CA={len(ca)}")
    else:
        print(f"\nstreams match (IR{'+AC' if ac_check else ''}) for all {n} opcodes")
PY
