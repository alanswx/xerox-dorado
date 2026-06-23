#!/bin/bash
# S1 metric harness for the cycle-accurate-timing project.
#
# Reports the LOCKSTEP DEPTH: how many executed Alto opcodes ours matches
# ContrAlto before the first control-flow (IR) divergence -- the North-Star
# metric of docs/timing-project-scope.md. Higher = ours stays cycle-aligned
# with a real Alto longer.
#
# ContrAlto's opcode stream for a given boot file is FIXED (it doesn't change
# as we tune ours), so we cache it once per (game, MAX) and on every later run
# only re-execute ours -- the fast S2 inner loop (change a cadence -> remeasure
# in seconds, not minutes).
#
# Usage:
#   lockstep.sh <game|bootpath> [MAX] [KEY=VAL ...]   # KEY=VAL = env for ours
#   lockstep.sh Invaders 5000 DORADO_ETH_WIRE=1
#   lockstep.sh Invaders                              # default MAX, no extra env
#   REFRESH=1 lockstep.sh Invaders                    # force re-run ContrAlto
#
# Env: CYCLES (dorado ceiling, default 260000000).
set -uo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
ROOT=/Users/alans/Documents/development/Dorado
DOR="$ROOT/dorado"
CA="$ROOT/AltoInfo/contralto-headless"
CACHE="$HERE/ca-cache"
mkdir -p "$CACHE"

GAME="${1:?usage: lockstep.sh <game|bootpath> [MAX] [KEY=VAL ...]}"
shift || true
MAX=5000
if [ "${1:-}" ] && [[ "${1:-}" =~ ^[0-9]+$ ]]; then MAX="$1"; shift; fi
# Remaining args are KEY=VAL env for the ours run.
OURS_ENV=("$@")
CYCLES="${CYCLES:-260000000}"

# Resolve the boot file.
if [ -f "$GAME" ]; then BF="$GAME"
else BF="$(ls "$ROOT"/chm/bootfiles/${GAME}.boot* 2>/dev/null | head -1)"; fi
[ -f "$BF" ] || { echo "no boot file for '$GAME'"; exit 1; }
base="$(basename "$BF")"
CA_SEQ="$CACHE/${base}.${MAX}.seq"

# --- ContrAlto side: cache once -------------------------------------------
if [ "${REFRESH:-0}" = 1 ] || [ ! -s "$CA_SEQ" ]; then
    echo "ContrAlto: generating cached seq for $base (MAX=$MAX)..." >&2
    ( cd "$CA" && DOTNET_ROLL_FORWARD=Major CA_NETBOOT=1 CA_BOOTSERVER="$BF" \
        CA_TRACEPC="$MAX" timeout 240 dotnet run -- dummy.dsk /tmp/ls_ca.pgm 22 \
        >/tmp/ls_ca.out 2>/tmp/ls_ca.log ) || true
    grep '^CATRACEPC' /tmp/ls_ca.log > "$CA_SEQ" || true
    [ -s "$CA_SEQ" ] || { echo "ContrAlto produced no trace; see /tmp/ls_ca.log"; exit 1; }
else
    echo "ContrAlto: using cached $CA_SEQ ($(wc -l < "$CA_SEQ") records)" >&2
fi

# --- ours side: always re-run ---------------------------------------------
echo "ours: ${OURS_ENV[*]:-(no extra env)} ..." >&2
# Safe empty-array expansion (macOS bash 3.2 errors on "${arr[@]}" + set -u).
( cd "$DOR" && env ${OURS_ENV[@]+"${OURS_ENV[@]}"} DORADO_IFUDISP_TRACE=1 \
    ./build/dorado --eb worlds/aemu.eb --eftp "$BF" --cycles "$CYCLES" \
    --out /tmp/ls_ours.pgm 2>&1 1>/dev/null \
    | grep -a -m "$MAX" '^IFUDISP' > /tmp/ls_ours.seq ) || true

# --- compare: contiguous IR-match depth -----------------------------------
python3 - /tmp/ls_ours.seq "$CA_SEQ" "$base" "${OURS_ENV[*]:-(none)}" <<'PY'
import re, sys
ours_path, ca_path, base, envs = sys.argv[1:5]
def lo(p):
    r=[]
    for l in open(p):
        if re.search(r'rtrap=1', l): continue         # skip AEmuReschedule trap
        m=re.search(r'\bop=([0-7]+)', l); a=re.search(r'alpha=([0-7]+)', l)
        if m: r.append((int(m.group(1),8)<<8)|(int(a.group(1),8) if a else 0))
    return r
def lc(p):
    r=[]
    for l in open(p):
        q=l.split()
        if len(q)>=4 and q[0]=='CATRACEPC': r.append(int(q[3],8))
    return r
o=lo(ours_path); ca=lc(ca_path)
# Best contiguous IR match over a small boot-phase skip on ours (0..127).
best_d, best_run = 0, -1
for d in range(128):
    if d>=len(o): break
    k=0
    while d+k<len(o) and k<len(ca) and o[d+k]==ca[k]: k+=1
    if k>best_run: best_run, best_d = k, d
print(f"LOCKSTEP {base}  env=[{envs}]  depth={best_run}  "
      f"(skip {best_d}; ours={len(o)} ca={len(ca)})")
PY
