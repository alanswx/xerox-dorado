#!/bin/bash
# Compare same-binary executed Alto/Nova instruction streams
# between Dorado AEmu and ContrAlto. Usage:
#   ./tracepcdiff.sh [max-instructions] [boot-file]
set -euo pipefail

MAX="${1:-200000}"
DOR=/Users/alans/Documents/development/Dorado/dorado
CA=/Users/alans/Documents/development/Dorado/AltoInfo/contralto-headless
BF="${2:-/Users/alans/Documents/development/Dorado/chm/bootfiles/MissileCommand.boot!1}"

cd "$DOR"
DORADO_TRACEPC="$MAX" ./build/dorado --eb worlds/aemu.eb \
  --eftp "$BF" --cycles 130000000 --out /tmp/td_pc_ours.pgm \
  >/dev/null 2>/tmp/td_pc_ours.log
grep DTRACEPC /tmp/td_pc_ours.log > /tmp/td_pc_ours.seq || true

cd "$CA"
DOTNET_ROLL_FORWARD=Major CA_NETBOOT=1 CA_BOOTSERVER="$BF" CA_TRACEPC="$MAX" \
  timeout 120 dotnet run -- dummy.dsk /tmp/td_pc_ca.pgm 22 \
  >/tmp/td_pc_ca.out 2>/tmp/td_pc_ca.log || true
grep CATRACEPC /tmp/td_pc_ca.log > /tmp/td_pc_ca.seq || true

python3 - "$MAX" <<'PY'
import sys

limit = int(sys.argv[1])

def load(path, tag):
    rows = []
    with open(path) as f:
        for line in f:
            parts = line.split()
            if len(parts) >= 4 and parts[0] == tag:
                rows.append((parts[1], parts[2], parts[3], line.strip()))
    return rows

def collapse_pcs(rows):
    out = []
    last_pc = None
    for row in rows:
        if row[1] != last_pc:
            out.append(row)
            last_pc = row[1]
    return out

ours_raw = load("/tmp/td_pc_ours.seq", "DTRACEPC")
ca_raw = load("/tmp/td_pc_ca.seq", "CATRACEPC")
ours = collapse_pcs(ours_raw)
ca = collapse_pcs(ca_raw)

print(f"requested {limit} instructions")
print(f"ours: {len(ours_raw)} raw, {len(ours)} after consecutive-PC collapse")
print(f"CA:   {len(ca_raw)} raw, {len(ca)} after consecutive-PC collapse")

n = min(len(ours), len(ca))
for i in range(n):
    if ours[i][1] != ca[i][1]:
        lo = max(0, i - 4)
        hi = min(n, i + 5)
        print(f">>> FIRST PC DIVERGENCE at instruction #{i}")
        print(f"ours: {ours[i][3]}")
        print(f"CA:   {ca[i][3]}")
        print("context:")
        for j in range(lo, hi):
            mark = "=>" if j == i else "  "
            print(f"{mark} {j}: ours pc={ours[j][1]} op={ours[j][2]} | CA pc={ca[j][1]} op={ca[j][2]}")
        break
else:
    if len(ours) != len(ca):
        print(f"streams match for first {n}; length differs")
    else:
        print(f"streams match for all {n}")
PY
