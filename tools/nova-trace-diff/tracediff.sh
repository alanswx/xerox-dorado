#!/bin/bash
# Nova trace-diff: compare a boot file's write-value sequence to an Alto
# address between our Dorado emulator and ContrAlto.
#
# Usage:
#   tracediff.sh <octal-addr> [boot-file] [dorado-cycles] [extra dorado args...]
ADDR="$1"
DOR=/Users/alans/Documents/development/Dorado/dorado
CA=/Users/alans/Documents/development/Dorado/AltoInfo/contralto-headless
BF="${2:-/Users/alans/Documents/development/Dorado/chm/bootfiles/MissileCommand.boot!1}"
CYCLES="${3:-130000000}"
shift $(( $# >= 3 ? 3 : $# ))
# Ours: every write to ADDR by program code (br31 != 0 = real Alto code,
# not boot). Dorado's trace range parser (memory.c, sscanf "%lo,%lo")
# expects OCTAL text, so pass ADDR verbatim (do NOT decimal-convert).
cd "$DOR" && DORADO_STORE_TRACE_VA="$ADDR,$ADDR" ./build/dorado --eb worlds/aemu.eb \
  --eftp "$BF" --cycles "$CYCLES" --out /tmp/td.pgm "$@" 2>/tmp/td_ours.log >/dev/null
grep STORE_VA /tmp/td_ours.log | grep -vE "br31=0o0 " | sed -E 's/.*data=0*([0-7]+) pcx=0o([0-7]+) br31=0o([0-7]+).*/\1 pc=\2 br=\3/' > /tmp/td_ours.seq
# ContrAlto
( cd "$CA" && DOTNET_ROLL_FORWARD=Major CA_NETBOOT=1 \
  CA_BOOTSERVER="$BF" CA_TRACEW="$ADDR" \
  timeout 120 dotnet run -- dummy.dsk /tmp/td_ca.pgm 22 2>&1 | grep CATRACEW ) \
  | sed -E 's/CATRACEW ([0-9]+) ([0-7]+)/\2 caseq=\1/' > /tmp/td_ca.seq
python3 - "$ADDR" <<'PY'
import sys
addr=sys.argv[1]
ours=[l.split()[0] for l in open('/tmp/td_ours.seq') if l.strip()]
ca  =[l.split()[0] for l in open('/tmp/td_ca.seq')   if l.strip()]
oursf=[l.strip() for l in open('/tmp/td_ours.seq') if l.strip()]
caf  =[l.strip() for l in open('/tmp/td_ca.seq')   if l.strip()]
print(f"addr 0o{addr}: ours has {len(ours)} MC-writes, CA has {len(ca)} writes")
print(f"  ours first 6: {ours[:6]}")
print(f"  CA   first 6: {ca[:6]}")
n=min(len(ours),len(ca))
for i in range(n):
    if ours[i]!=ca[i]:
        print(f"  >>> FIRST DIVERGENCE at write #{i}: ours={ours[i]} ({oursf[i]})  CA={ca[i]} ({caf[i]})")
        break
else:
    print(f"  no divergence in first {n} writes (ours/CA lengths differ: {len(ours)}/{len(ca)})" if len(ours)!=len(ca) else f"  sequences match for all {n} writes")
PY
