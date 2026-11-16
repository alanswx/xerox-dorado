#!/bin/sh
# The cold-start files of the one core (DoradoOne.sh WORLD cold):
#   make_cold.sh OUTDIR
# For each world, the machine as the C emulator has it just after its
# microcode is loaded (Initial's netboot of the world's .eb; for Cedar, after
# the germ is planted too), from there the core boots the world itself:
#   OUTDIR/<world>-cold.snap        the ARM host's snapshot (the wasm32 bake:
#                                   the 32-bit ABI; native and wasm snapshots
#                                   are not portable)
#   OUTDIR/<world>-cold.snap.native the same instant, this machine's ABI (the
#                                   simulation's, and what the records are from)
#   OUTDIR/<world>-cold.snap.regs   the scan chain (gen_scan.py --regs --cold)
#   OUTDIR/<world>-cold.snap.rams   the load records
#   OUTDIR/<world>-cold.snap.pack   a fresh Trident pack (lisp, smalltalk)
#   OUTDIR/<world>-cold.snap.pdi    the Pilot work volume (cedar)
# Copy all but the .native files to /media/fat/dorado/. Run from rtl_core/;
# needs ../dorado/build/dorado, ../dorado/build/dorado-node.js and a
# sim_cedar binary (make cedar; CEDAR_BIN, default obj_cedar/Vdorado_rtl_top).
# The same DORADO_FAKE_TIME in both bakes: the two must be the same machine.
set -e
OUT=$(cd "$1" && pwd)
RTL=$(pwd)
BIN=${CEDAR_BIN:-$RTL/obj_cedar/Vdorado_rtl_top}
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
bake .native ./build/dorado
cd "$RTL"
for w in lisp smalltalk cedar; do
    D="$OUT/$w-cold.snap.pack"; [ $w = cedar ] && D="$OUT/cedar-cold.snap.pdi"
    I="$OUT/init_$w"; mkdir -p "$I"
    SIM_WORLD=$w CEDAR_SPLIT=1 CEDAR_DISPLAY=1 DORADO_DISPM_PRESENT=1 DORADO_PDI_IGNORE_LABEL_FLAGS=1 \
        CEDAR_DUMP_INIT="$I" "$BIN" 0 "$OUT/$w-cold.snap.native" "$D" > /dev/null 2>&1
    python3 tools/gen_scan.py --regs "$I" "$OUT/$w-cold.snap.regs" --cold
    python3 tools/gen_scan.py --rams "$I" "$OUT/$w-cold.snap.rams"
done
ls -l "$OUT"
