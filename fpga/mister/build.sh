#!/bin/sh
# Assemble a MiSTer core from the framework template plus this repo's RTL.
#
# The core is ASSEMBLED rather than checked in whole, because sys/ belongs to
# the MiSTer framework and the pin assignments belong to the DE10-Nano -- both
# come from the template, and duplicating them here would mean maintaining a
# stale copy. What lives in this repo is only what is ours: Dorado.sv and this
# script.
#
#   ./build.sh [TEMPLATE_DIR] [OUT_DIR]
# defaults suit the build machine:
#   ./build.sh ~/mister/Template_MiSTer ~/mister/Dorado_MiSTer
set -e
TEMPLATE=${1:-$HOME/mister/Template_MiSTer}
OUT=${2:-$HOME/mister/Dorado_MiSTer}
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../.." && pwd)

[ -d "$TEMPLATE/sys" ] || { echo "no framework at $TEMPLATE/sys"; exit 1; }
[ -f "$REPO/verilog/generated/dorado_backplane.v" ] || {
    echo "no generated RTL -- run 'make -C verilog backplane' first"; exit 1; }

mkdir -p "$OUT"
cp -r "$TEMPLATE/sys" "$OUT/"
cp "$TEMPLATE/files.qip" "$OUT/" 2>/dev/null || true
cp "$TEMPLATE/Template.qsf" "$OUT/Dorado.qsf"
cp "$TEMPLATE/Template.sdc" "$OUT/Dorado.sdc"
cp -r "$TEMPLATE/rtl" "$OUT/" 2>/dev/null || true
sed 's/Template/Dorado/g' "$TEMPLATE/Template.qpf" > "$OUT/Dorado.qpf"
cp "$HERE/Dorado.sv" "$OUT/"
rm -f "$OUT/Template.sv" "$OUT/rtl/mycore.v"

# Point the project at our sources. THE TEMPLATE'S FILE LIST IS files.qip, NOT
# the qsf -- the qsf only does `source files.qip` -- so renaming Template.sv in
# the qsf alone leaves the project with no `emu` at all, which is exactly how
# this failed the first time. Fix both.
sed -i.bak -e 's|Template\.sv|Dorado.sv|g' -e 's|Template\.sdc|Dorado.sdc|g' \
    -e '/mycore\.v/d' "$OUT/files.qip"
rm -f "$OUT/files.qip.bak"

sed -i.bak \
    -e 's|Template\.sv|Dorado.sv|g' \
    -e 's|Template|Dorado|g' \
    "$OUT/Dorado.qsf"
rm -f "$OUT/Dorado.qsf.bak"

{
  echo ""
  echo "# ---- the Dorado, generated from PARC's Sil wire lists ----"
  # SEARCH_PATH so the PROM cells' $readmemh (paths relative to the repo root)
  # resolve from the project directory. Without it every PROM package fails to
  # elaborate; this is the same fix fpga/quartus needed.
  echo "set_global_assignment -name SEARCH_PATH \"$REPO\""
  for f in "$REPO"/verilog/generated/dorado_backplane.v \
           "$REPO"/verilog/generated/*-Rev-*.v \
           "$REPO"/verilog/generated/msa.v \
           "$REPO"/verilog/cells/cell_*.v \
           "$REPO"/verilog/vendor/6502/*.v \
           "$REPO"/verilog/vendor/riot/*.sv; do
    [ -f "$f" ] && echo "set_global_assignment -name SYSTEMVERILOG_FILE $f"
  done
} >> "$OUT/Dorado.qsf"

echo "assembled $OUT"
echo "  framework: $TEMPLATE"
echo "  RTL:       $REPO/verilog"
grep -c SYSTEMVERILOG_FILE "$OUT/Dorado.qsf" | sed 's/^/  sources:   /'
