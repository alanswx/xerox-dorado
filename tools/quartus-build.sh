#!/usr/bin/env bash
# ---------------------------------------------------------------------------
# Synthesise the Dorado RTL for the DE10-Nano's Cyclone V, using Quartus
# 17.0.2 inside the raetro/quartus:mister container.
#
#   tools/quartus-build.sh map     # analysis & synthesis only -- the numbers
#   tools/quartus-build.sh all     # map + fit + sta, for a real placement
#   tools/quartus-build.sh clean   # remove Quartus output
#
# Adapted from the working script in RCAStudioII_Mister, and the reason it
# does NOT just run "quartus_sh --flow compile" is worth keeping:
#
#   The image is linux/amd64 and runs under emulation on Apple Silicon.
#   Quartus' parallel helper processes crash there -- they appear as
#   <defunct> alongside [crashreporter] -- and the parent then deadlocks
#   forever on their named pipes at ~4% CPU. That looks like a slow build and
#   never finishes. Passing --parallel=1 to each stage avoids spawning them.
#
#   SO: A HEALTHY BUILD SITS AT ~100% CPU. At ~4% it is deadlocked, not slow.
#
# Quartus REWRITES verilog/quartus/dorado.qsf on every run. Sources therefore
# live in files.qip, and `git checkout -- verilog/quartus/dorado.qsf` after a
# build keeps the rewrite out of commits and out of the way of git stash.
#
# Afterwards: output_files/dorado.map.rpt has the resource estimate and the
# inferred-RAM list; .fit.rpt has real ALM and block-RAM figures if you ran
# the fit.
# ---------------------------------------------------------------------------
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
IMAGE="${QUARTUS_IMAGE:-raetro/quartus:mister}"
PROJ=Dorado
QDIR=verilog
STAGE="${1:-map}"

docker image inspect "$IMAGE" >/dev/null 2>&1 || {
    echo "error: docker image $IMAGE not found -- is Docker Desktop running?" >&2
    exit 1; }

# NOTE: run from the REPO ROOT, not from the project directory. The PROM
# cells $readmemh their images from repo-relative paths
# (verilog/proms/packages/*.mem), so Quartus has to see the same working
# directory the simulator does. The project is named by path instead.
run() { docker run --rm --platform linux/amd64 -v "$ROOT":/build "$IMAGE" \
            bash -c "cd /build/$QDIR && $1"; }

case "$STAGE" in
    clean) rm -rf "$ROOT/$QDIR"/output_files "$ROOT/$QDIR"/db "$ROOT/$QDIR"/incremental_db
           echo "cleaned"; exit 0 ;;
    map)   run "quartus_map --parallel=1 $PROJ" ;;
    all)   run "quartus_map --parallel=1 $PROJ && \
                quartus_fit --parallel=1 $PROJ && \
                quartus_sta $PROJ" ;;
    *)     echo "usage: $0 [map|all|clean]" >&2; exit 1 ;;
esac

echo
grep -hE "was successful|was unsuccessful" \
     "$ROOT/$QDIR/output_files/$PROJ".{map,fit,sta}.rpt 2>/dev/null || true
