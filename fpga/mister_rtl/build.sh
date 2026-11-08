#!/bin/sh
# Assemble the rtl_core MiSTer project from the framework template.
#
#   [CLK_MHZ=50] [CORE=cedar] [PIPELINE=1] fpga/mister_rtl/build.sh [TEMPLATE_DIR] [OUT_DIR]
#
# Needs rtl_core/init/ (make -C rtl_core init) and rtl_core/roms/ (built by
# the rtl_core Makefile). Then: cd OUT_DIR && quartus_sh --flow compile Dorado
#
# CORE=cedar builds the Cedar checkpoint core instead: rtl_core/init_cedar/
# (sim_cedar.cpp CEDAR_DUMP_INIT) becomes init/, and the core waits for
# dorado-hps (rtl_core/hps) on the ARM. CORE=smalltalk is the Smalltalk-76
# checkpoint core (rtl_core/init_smalltalk; dorado-hps -w smalltalk).
set -e
TEMPLATE=${1:-$HOME/mister/Template_MiSTer}
OUT=${2:-$HOME/mister/DoradoRtlCore}
HERE=$(cd "$(dirname "$0")" && pwd)
CORE_KIND=${CORE:-alto}
CORE=$(cd "$HERE/../../rtl_core" && pwd)
[ -d "$TEMPLATE/sys" ] || { echo "no framework at $TEMPLATE/sys"; exit 1; }
INIT=init
[ "$CORE_KIND" = cedar ] && INIT=init_cedar
[ "$CORE_KIND" = smalltalk ] && INIT=init_smalltalk
[ "$CORE_KIND" = lisp ] && INIT=init_lisp
[ -f "$CORE/$INIT/start.txt" ] || { echo "no $CORE/$INIT -- see the comment at the top"; exit 1; }
[ -f "$CORE/roms/bootfile.len" ] || { echo "no $CORE/roms -- run: make -C rtl_core roms/bootfile.hex"; exit 1; }

RESET_PC=$(sed -n 's/^real_PC=0\([0-7]*\).*/\1/p' "$CORE/$INIT/start.txt")
BOOT_WORDS=$(cat "$CORE/roms/bootfile.len")

mkdir -p "$OUT/rtl"
cp -r "$TEMPLATE/sys" "$OUT/"
cp -r "$TEMPLATE/rtl/pll" "$TEMPLATE/rtl/pll.v" "$TEMPLATE/rtl/pll.qip" "$OUT/rtl/"
# System clock (the CPU, I/O and video all run on it)
CLK_MHZ=${CLK_MHZ:-50}
sed -i "s/output_clock_frequency0(\"[^\"]*\")/output_clock_frequency0(\"$CLK_MHZ.000000 MHz\")/" "$OUT/rtl/pll/pll_0002.v"
# PLL_FAST_OUT=<MHz>: a second, unused PLL output, to steer the IP's choice
# of VCO (48 MHz alone gives M=48, N=5, a 480 MHz VCO; a 320 MHz second
# output needs 960). An experiment on the fabric clock the board measures.
if [ -n "$PLL_FAST_OUT" ]; then
  sed -i 's/\.number_of_clocks(1)/.number_of_clocks(2)/; s/output_clock_frequency1("0 MHz")/output_clock_frequency1("'"$PLL_FAST_OUT"'.000000 MHz")/; s/output wire outclk_0,/output wire outclk_0,\n\toutput wire outclk_1,/; s/\.outclk\t({outclk_0})/.outclk\t({outclk_1, outclk_0})/' "$OUT/rtl/pll/pll_0002.v"
  grep -q "outclk_1, outclk_0" "$OUT/rtl/pll/pll_0002.v" || { echo "PLL second output not applied"; exit 1; }
fi
cp "$TEMPLATE/mycore.sdc" "$OUT/Dorado.sdc"
cp "$HERE/Dorado.sv" "$OUT/"
for f in dorado_rtl_pkg.sv dorado_rtl_alu_shifter.sv dorado_rtl_storage.sv dorado_rtl_storage_ddr.sv dorado_rtl_mem.sv \
         dorado_rtl_cpu.sv dorado_rtl_io.sv \
         dorado_rtl_bootsrv.sv dorado_rtl_assist.sv dorado_rtl_mailbox.sv dorado_rtl_top.sv; do
    cp "$CORE/$f" "$OUT/rtl/"
done
cp "$CORE/../verilog/cells/dorado_keyboard.v" "$OUT/rtl/"
rm -rf "$OUT/init" "$OUT/roms"
cp -r "$CORE/$INIT" "$OUT/init"
rm -f "$OUT/init/storage.bin"   # dorado-hps loads it into DDR; the bitstream does not need it
cp -r "$CORE/roms" "$OUT/"

cat > "$OUT/files.qip" <<QIP
set_global_assignment -name QIP_FILE rtl/pll.qip
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_pkg.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_alu_shifter.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_storage.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_storage_ddr.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_mem.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_cpu.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_io.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_bootsrv.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_assist.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_mailbox.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_top.sv
set_global_assignment -name VERILOG_FILE rtl/dorado_keyboard.v
set_global_assignment -name SDC_FILE Dorado.sdc
set_global_assignment -name SYSTEMVERILOG_FILE Dorado.sv
set_global_assignment -name VERILOG_MACRO "DORADO_INIT=1"
set_global_assignment -name VERILOG_MACRO "DORADO_RESET_PC=16'o$RESET_PC"
set_global_assignment -name VERILOG_MACRO "DORADO_BOOT_WORDS=$BOOT_WORDS"
QIP
# The Trident controller is in only where a world uses it (Smalltalk-76's
# pack, through its sector engine on the DDR aux port).
if [ "$CORE_KIND" != smalltalk ] && [ "$CORE_KIND" != lisp ]; then
    echo 'set_global_assignment -name VERILOG_MACRO "DORADO_NO_DISK=1"' >> "$OUT/files.qip"
fi
if [ "$CORE_KIND" = cedar ]; then
    cat >> "$OUT/files.qip" <<QIP
set_global_assignment -name VERILOG_MACRO "DORADO_CEDAR=1"
set_global_assignment -name VERILOG_MACRO "DORADO_CHECKPOINT=1"
set_global_assignment -name SEARCH_PATH init
QIP
fi
# CORE=smalltalk: the Smalltalk-76 checkpoint core (rtl_core/init_smalltalk,
# from sim_cedar.cpp SIM_WORLD=smalltalk CEDAR_SPLIT=1 CEDAR_DUMP_INIT); run
# dorado-hps -w smalltalk on the ARM, which loads storage and the pack.
if [ "$CORE_KIND" = smalltalk ]; then
    cat >> "$OUT/files.qip" <<QIP
set_global_assignment -name VERILOG_MACRO "DORADO_SMALLTALK=1"
set_global_assignment -name VERILOG_MACRO "DORADO_CHECKPOINT=1"
set_global_assignment -name SEARCH_PATH init
QIP
fi
# CORE=lisp: Interlisp-D Lyric, full library (rtl_core/init_lisp, from
# sim_cedar.cpp SIM_WORLD=lisp CEDAR_SPLIT=1 CEDAR_DISPLAY=1 CEDAR_DUMP_INIT,
# DORADO_DISPM_PRESENT=1 DORADO_FAKE_TIME); dorado-hps -w lisp
if [ "$CORE_KIND" = lisp ]; then
    cat >> "$OUT/files.qip" <<QIP
set_global_assignment -name VERILOG_MACRO "DORADO_LISP=1"
set_global_assignment -name VERILOG_MACRO "DORADO_CHECKPOINT=1"
set_global_assignment -name SEARCH_PATH init
QIP
fi
# PIPELINE=1: the CPU with C_PIPE (rtl_core/dorado_rtl_cpu.sv PIPELINE)
if [ "${PIPELINE:-0}" = 1 ]; then
    echo 'set_global_assignment -name VERILOG_MACRO "DORADO_PIPELINE=1"' >> "$OUT/files.qip"
fi
sed -e 's/mycore/Dorado/g' "$TEMPLATE/mycore.qpf" > "$OUT/Dorado.qpf"
sed -e 's/mycore/Dorado/g' "$TEMPLATE/mycore.qsf" > "$OUT/Dorado.qsf"
# Multicorner timing. The MiSTer template leaves TIMEQUEST_MULTICORNER_ANALYSIS
# off, so the fitter analyses and optimizes only one corner and the worst
# process/voltage/temperature corner can miss setup -- which showed up here as
# an HDMI path at -0.050 ns and barely-positive core slack. Analyse and
# optimize every corner instead (no speed cost; measured worst-corner setup
# +0.227 -> +1.588 ns on the core and the HDMI path -0.050 -> +0.517 ns on the
# Cedar pipelined build). Appended with a leading newline so it overrides the
# template's default regardless of how that line is spelled or terminated.
printf '\nset_global_assignment -name TIMEQUEST_MULTICORNER_ANALYSIS ON\nset_global_assignment -name OPTIMIZE_MULTI_CORNER_TIMING ON\n' >> "$OUT/Dorado.qsf"
# The Lisp build's HDMI scan-out path (the framework's, placement-sensitive)
# missed by 0.278 ns at the worst corner with the default seed; seed 2 closes
# every clock (core +1.189 ns, HDMI +0.223 ns, 2026-11-08).
if [ "$CORE_KIND" = lisp ]; then
    printf 'set_global_assignment -name SEED 2\n' >> "$OUT/Dorado.qsf"
fi
echo "assembled $OUT (RESET_PC=0$RESET_PC, boot file $BOOT_WORDS words)"
