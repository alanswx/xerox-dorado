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
# dorado-hps (rtl_core/hps) on the ARM.
set -e
TEMPLATE=${1:-$HOME/mister/Template_MiSTer}
OUT=${2:-$HOME/mister/DoradoRtlCore}
HERE=$(cd "$(dirname "$0")" && pwd)
CORE_KIND=${CORE:-alto}
CORE=$(cd "$HERE/../../rtl_core" && pwd)
[ -d "$TEMPLATE/sys" ] || { echo "no framework at $TEMPLATE/sys"; exit 1; }
INIT=init
[ "$CORE_KIND" = cedar ] && INIT=init_cedar
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
set_global_assignment -name VERILOG_MACRO "DORADO_NO_DISK=1"
set_global_assignment -name VERILOG_MACRO "DORADO_RESET_PC=16'o$RESET_PC"
set_global_assignment -name VERILOG_MACRO "DORADO_BOOT_WORDS=$BOOT_WORDS"
QIP
if [ "$CORE_KIND" = cedar ]; then
    cat >> "$OUT/files.qip" <<QIP
set_global_assignment -name VERILOG_MACRO "DORADO_CEDAR=1"
set_global_assignment -name VERILOG_MACRO "DORADO_CHECKPOINT=1"
set_global_assignment -name SEARCH_PATH init
QIP
fi
# PIPELINE=1: the CPU with C_PIPE (rtl_core/dorado_rtl_cpu.sv PIPELINE)
if [ "${PIPELINE:-0}" = 1 ]; then
    echo 'set_global_assignment -name VERILOG_MACRO "DORADO_PIPELINE=1"' >> "$OUT/files.qip"
fi
sed -e 's/mycore/Dorado/g' "$TEMPLATE/mycore.qsf" > "$OUT/Dorado.qsf"
sed -e 's/mycore/Dorado/g' "$TEMPLATE/mycore.qpf" > "$OUT/Dorado.qpf"
echo "assembled $OUT (RESET_PC=0$RESET_PC, boot file $BOOT_WORDS words)"
