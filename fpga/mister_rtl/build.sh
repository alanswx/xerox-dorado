#!/bin/sh
# Assemble the rtl_core MiSTer project from the framework template.
#
#   fpga/mister_rtl/build.sh [TEMPLATE_DIR] [OUT_DIR]
#
# Needs rtl_core/init/ (make -C rtl_core init) and rtl_core/roms/ (built by
# the rtl_core Makefile). Then: cd OUT_DIR && quartus_sh --flow compile Dorado
set -e
TEMPLATE=${1:-$HOME/mister/Template_MiSTer}
OUT=${2:-$HOME/mister/DoradoRtlCore}
HERE=$(cd "$(dirname "$0")" && pwd)
CORE=$(cd "$HERE/../../rtl_core" && pwd)
[ -d "$TEMPLATE/sys" ] || { echo "no framework at $TEMPLATE/sys"; exit 1; }
[ -f "$CORE/init/start.txt" ] || { echo "no $CORE/init -- run: make -C rtl_core init"; exit 1; }
[ -f "$CORE/roms/bootfile.len" ] || { echo "no $CORE/roms -- run: make -C rtl_core roms/bootfile.hex"; exit 1; }

RESET_PC=$(sed -n 's/^real_PC=0\([0-7]*\).*/\1/p' "$CORE/init/start.txt")
BOOT_WORDS=$(cat "$CORE/roms/bootfile.len")

mkdir -p "$OUT/rtl"
cp -r "$TEMPLATE/sys" "$OUT/"
cp -r "$TEMPLATE/rtl/pll" "$TEMPLATE/rtl/pll.v" "$TEMPLATE/rtl/pll.qip" "$OUT/rtl/"
# 40 MHz system clock
sed -i 's/output_clock_frequency0("[^"]*")/output_clock_frequency0("40.000000 MHz")/' "$OUT/rtl/pll/pll_0002.v"
cp "$TEMPLATE/mycore.sdc" "$OUT/Dorado.sdc"
cp "$HERE/Dorado.sv" "$OUT/"
for f in dorado_rtl_pkg.sv dorado_rtl_alu_shifter.sv dorado_rtl_cpu.sv dorado_rtl_io.sv \
         dorado_rtl_bootsrv.sv dorado_rtl_assist.sv dorado_rtl_top.sv; do
    cp "$CORE/$f" "$OUT/rtl/"
done
rm -rf "$OUT/init" "$OUT/roms"
cp -r "$CORE/init" "$CORE/roms" "$OUT/"

cat > "$OUT/files.qip" <<QIP
set_global_assignment -name QIP_FILE rtl/pll.qip
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_pkg.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_alu_shifter.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_cpu.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_io.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_bootsrv.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_assist.sv
set_global_assignment -name SYSTEMVERILOG_FILE rtl/dorado_rtl_top.sv
set_global_assignment -name SDC_FILE Dorado.sdc
set_global_assignment -name SYSTEMVERILOG_FILE Dorado.sv
set_global_assignment -name VERILOG_MACRO "DORADO_INIT=1"
set_global_assignment -name VERILOG_MACRO "DORADO_NO_DISK=1"
set_global_assignment -name VERILOG_MACRO "DORADO_RESET_PC=16'o$RESET_PC"
set_global_assignment -name VERILOG_MACRO "DORADO_BOOT_WORDS=$BOOT_WORDS"
QIP
sed -e 's/mycore/Dorado/g' "$TEMPLATE/mycore.qsf" > "$OUT/Dorado.qsf"
sed -e 's/mycore/Dorado/g' "$TEMPLATE/mycore.qpf" > "$OUT/Dorado.qpf"
echo "assembled $OUT (RESET_PC=0$RESET_PC, boot file $BOOT_WORDS words)"
