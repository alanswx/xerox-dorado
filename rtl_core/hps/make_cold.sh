#!/bin/sh
# The cold-start files of the one core (DoradoOne.sh WORLD cold):
#   make_cold.sh OUTDIR
# For each world, the machine as the C emulator has it just after its
# microcode is loaded (Initial's netboot of the world's .eb; for Cedar, after
# the germ is planted too), from there the core boots the world itself:
#   OUTDIR/<world>-cold.snap        the ARM host's snapshot (the wasm32 bake:
#                                   the 32-bit ABI; native and wasm snapshots
#                                   are not portable)
#   OUTDIR/<world>-cold.snap.pack   a fresh Trident pack (lisp, smalltalk)
#   OUTDIR/<world>-cold.snap.pdi    the Pilot work volume (cedar)
# Copy them to /media/fat/dorado/. The scan chain and the load records are
# made by the host from the snapshot (dorado-hps-one -C; hps_host.cpp
# derive_regs), so there are no .regs/.rams files and no simulation run here.
# (NATIVE=1 also writes <world>-cold.snap.native, this machine's ABI, for the
# simulation: HPS_SNAPSHOT with HPS_DERIVE=1 HPS_COLD=1.)
# Run from rtl_core/; needs ../dorado/build/dorado-node.js.
# The same DORADO_FAKE_TIME in both bakes: the two must be the same machine.
set -e
OUT=$(cd "$1" && pwd)
cd ../dorado
cp -f build/good-packs/lisp-lyric-full-19000.pack "$OUT/lisp-cold.snap.pack"
cp -f build/good-packs/smalltalk76.pack           "$OUT/smalltalk-cold.snap.pack"
cp -f ../CedarDisk/CedarDorado-work.pdi           "$OUT/cedar-cold.snap.pdi"
export DORADO_FAKE_TIME=1783285880 DORADO_SNAPSHOT_NO_PACK_FLUSH=1
bake() {    # bake SUFFIX COMMAND...
    X=$1; shift
    DORADO_DISPM_PRESENT=1 "$@" --eb worlds/aemu.eb --disk 0="$OUT/lisp-cold.snap.pack" \
        --boot-reason disk --no-alto-boot --cycles 33000000 \
        --snapshot-out "$OUT/lisp-cold.snap$X" > /dev/null
    "$@" --eb '../chm/microcode/SmalltalkDorado.eb!1' --disk 0="$OUT/smalltalk-cold.snap.pack" \
        --boot-reason disk --no-alto-boot --cycles 33000000 \
        --snapshot-out "$OUT/smalltalk-cold.snap$X" > /dev/null
    # (the germ is planted at about 67.3 M BaseBoard cycles)
    DORADO_PDI_IGNORE_LABEL_FLAGS=1 "$@" --boot-reason disk --no-alto-boot \
        --eb '../chm/dorado/CedarDorado.eb!6' --germ ../chm/cedar/germ-alt/Dorado.germ-6.1.6 \
        --pilot-disk "$OUT/cedar-cold.snap.pdi" --ftp-root ../chm/cedar/stp-root \
        --cycles 70000000 --snapshot-out "$OUT/cedar-cold.snap$X" > /dev/null
}
bake ""      node build/dorado-node.js
[ -z "$NATIVE" ] || bake .native ./build/dorado
ls -l "$OUT"
