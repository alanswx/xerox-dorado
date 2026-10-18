#!/bin/sh
# Assemble a MiSTer core from the framework template plus this repo's RTL.
#
# The core is ASSEMBLED rather than checked in whole, because sys/ belongs to
# the MiSTer framework and the pin assignments belong to the DE10-Nano -- both
# come from the template, and duplicating them here would mean maintaining a
# stale copy. What lives in this repo is only what is ours: Dorado.sv and this
# script.
#
#   ./build.sh [TEMPLATE_DIR] [OUT_DIR] [GENERATED_CHECKPOINT_DIR]
# defaults suit the build machine:
#   ./build.sh ~/mister/Template_MiSTer ~/mister/Dorado_MiSTer
# DORADO_SYS_MHZ selects the core PLL frequency (default33.333333).
# A different frequency needs fresh fit/STA/clock and hardware qualification.
set -e
TEMPLATE=${1:-$HOME/mister/Template_MiSTer}
OUT=${2:-$HOME/mister/Dorado_MiSTer}
CHECKPOINT=${3:-}
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../.." && pwd)
DORADO_SYS_MHZ=$(python3 - "${DORADO_SYS_MHZ:-33.333333}" <<'PY'
from decimal import Decimal, InvalidOperation
import sys
try:
    value = Decimal(sys.argv[1])
    if not value.is_finite() or value <= 0:
        raise ValueError()
    result = format(value, '.6f')
    if Decimal(result) <= 0:
        raise ValueError()
except (InvalidOperation, ValueError):
    raise SystemExit('DORADO_SYS_MHZ must be a finite positive frequency in MHz')
print(result)
PY
)
DORADO_SYS_HZ=$(python3 - "$DORADO_SYS_MHZ" <<'PY'
from decimal import Decimal
import sys
value = int(Decimal(sys.argv[1]) * 1000000)
if not 0 < value <= 500000000:
    raise SystemExit('DORADO_SYS_MHZ must produce 1..500000000 Hz for the host watchdog')
print(value)
PY
)

# Fail before staging an unsupported SDRAM clock. Its controller/pin phase
# contract is intentionally narrower than the generic PLL selector.
if [ -n "$CHECKPOINT" ]; then
  python3 - "$CHECKPOINT/manifest.json" "$DORADO_SYS_HZ" <<'PY'
import json, sys
config = json.load(open(sys.argv[1]))
if config.get('storage_backend', 'ddr') == 'sdram':
    if not config.get('native') or not 20000000 <= int(sys.argv[2]) <= 50000000:
        raise SystemExit('Native SDRAM backend requires a20..50MHz host clock')
PY
fi

# Direct frames use the host DDR port freed by the native SDRAM backend.
# Keep the DDR raw-memory control selectable with the existing SPI transport.
if [ "${DORADO_DDR_NETWORK:-0}" = 1 ]; then
  [ -n "$CHECKPOINT" ] || { echo "DDR network requires a generated native machine"; exit 1; }
  python3 - "$CHECKPOINT/manifest.json" <<'DDR_NETWORK_JSON'
import json, sys
c = json.load(open(sys.argv[1]))
if not (c.get('native') and c.get('storage_backend') == 'sdram' and c.get('ethernet_wire')):
    raise SystemExit('DDR network requires native SDRAM storage and the original Ethernet wire adapter')
DDR_NETWORK_JSON
fi

[ -d "$TEMPLATE/sys" ] || { echo "no framework at $TEMPLATE/sys"; exit 1; }
BASE=Template
[ -f "$TEMPLATE/$BASE.qsf" ] || BASE=mycore
[ -f "$TEMPLATE/$BASE.qsf" ] || { echo "no Template.qsf or mycore.qsf in $TEMPLATE"; exit 1; }
[ -f "$REPO/verilog/generated/dorado_backplane.v" ] || {
    echo "no generated RTL -- run 'make -C verilog backplane' first"; exit 1; }

mkdir -p "$OUT"
cp -r "$TEMPLATE/sys" "$OUT/"
cp "$TEMPLATE/files.qip" "$OUT/" 2>/dev/null || true
cp "$TEMPLATE/$BASE.qsf" "$OUT/Dorado.qsf"
cp "$TEMPLATE/$BASE.sdc" "$OUT/Dorado.sdc"
cp -r "$TEMPLATE/rtl" "$OUT/" 2>/dev/null || true
sed "s/$BASE/Dorado/g" "$TEMPLATE/$BASE.qpf" > "$OUT/Dorado.qpf"
cp "$HERE/Dorado.sv" "$OUT/"
rm -f "$OUT/Template.sv" "$OUT/rtl/mycore.v"
# Some framework releases keep the emu port declaration in the example core.
if [ ! -f "$OUT/sys/emu_ports.vh" ]; then
  python3 - "$TEMPLATE/$BASE.sv" "$OUT/sys/emu_ports.vh" <<'PY'
import pathlib, re, sys
source = pathlib.Path(sys.argv[1]).read_text()
match = re.search(r'\bmodule\s+emu\s*\((.*?)^\s*\);', source, re.M | re.S)
if not match:
    raise SystemExit('cannot locate framework emu ports')
pathlib.Path(sys.argv[2]).write_text(match[1] + '\n')
PY
fi

# The PLL defaults to 33.333 MHz. SYSPER counts samples per 30 ns clock, and
# each instruction takes two clocks: SYSPER=4 therefore gives 4.167 M
# microinstructions/s (0.25x hardware speed), the validated simulation ratio.
#
# Patched here rather than in the template so the retune belongs to this repo
# and survives a template refresh.
python3 - "$OUT/rtl/pll/pll_0002.v" "$DORADO_SYS_MHZ" "$DORADO_SYS_HZ" <<'PY'
from pathlib import Path
import json, sys
path = Path(sys.argv[1])
source = path.read_text()
old = 'output_clock_frequency0("20.000000 MHz")'
if source.count(old) != 1:
    raise SystemExit('PLL retune did not apply -- template PLL frequency changed?')
path.write_text(source.replace(old, f'output_clock_frequency0("{sys.argv[2]} MHz")'))
(path.parents[2] / 'dorado_clock_config.json').write_text(
    json.dumps({'sys_mhz': sys.argv[2], 'sys_hz': int(sys.argv[3]),
                'hps_timeout_seconds': 4}, indent=2) + '\n')
PY

# Point the project at our sources. THE TEMPLATE'S FILE LIST IS files.qip, NOT
# the qsf -- the qsf only does `source files.qip` -- so renaming Template.sv in
# the qsf alone leaves the project with no `emu` at all, which is exactly how
# this failed the first time. Fix both.
sed -i.bak -e "s|$BASE\\.sv|Dorado.sv|g" -e "s|$BASE\\.sdc|Dorado.sdc|g" \
    -e '/mycore\.v/d' "$OUT/files.qip"
rm -f "$OUT/files.qip.bak"

sed -i.bak \
    -e "s|$BASE|Dorado|g" \
    "$OUT/Dorado.qsf"
rm -f "$OUT/Dorado.qsf.bak"

{
  echo ""
  echo "# ---- the Dorado, generated from PARC's Sil wire lists ----"
  echo "set_global_assignment -name NUM_PARALLEL_PROCESSORS ${DORADO_QUARTUS_JOBS:-1}"
  # Physical host watchdog, not a guest clock or a SYSPER-derived timer.
  echo "set_global_assignment -name VERILOG_MACRO \"DORADO_HOST_CLOCK_HZ=$DORADO_SYS_HZ\""
  # Optional for bring-up iterations: retiming took over twenty minutes on
  # the diagnostic native core. This changes placement optimization only;
  # setup/hold analysis is still required before deploying the result.
  if [ "${DORADO_QUARTUS_RETIME:-1}" = 0 ]; then
    echo 'set_global_assignment -name PHYSICAL_SYNTHESIS_REGISTER_RETIMING OFF'
  fi
  if [ "${DORADO_QUARTUS_PHYSICAL:-1}" = 0 ]; then
    for DORADO_OPT in COMBO_LOGIC REGISTER_DUPLICATION REGISTER_RETIMING \
                      COMBO_LOGIC_FOR_AREA ASYNCHRONOUS_SIGNAL_PIPELINING; do
      echo "set_global_assignment -name PHYSICAL_SYNTHESIS_${DORADO_OPT} OFF"
    done
  fi
  echo 'set_global_assignment -name VERILOG_MACRO "DORADO_STORAGE_INPUT_DELAY=2"'
  # SEARCH_PATH so the PROM cells' $readmemh (paths relative to the repo root)
  # resolve from the project directory. Without it every PROM package fails to
  # elaborate; this is the same fix fpga/quartus needed.
  echo "set_global_assignment -name SEARCH_PATH \"$REPO\""
  echo "set_global_assignment -name SYSTEMVERILOG_FILE $REPO/fpga/mister/dorado_mouse.sv"
  echo "set_global_assignment -name SYSTEMVERILOG_FILE $REPO/fpga/mister/dorado_key_queue.sv"
  echo "set_global_assignment -name SYSTEMVERILOG_FILE $REPO/fpga/mister/dorado_speed_monitor.sv"
  if [ "${DORADO_DDR_NETWORK:-0}" = 1 ]; then
    echo 'set_global_assignment -name VERILOG_MACRO DORADO_ETHERNET_DDR'
    echo "set_global_assignment -name SYSTEMVERILOG_FILE $HERE/dorado_ethernet_ddr.sv"
  fi
  if [ -n "$CHECKPOINT" ]; then
    [ -f "$CHECKPOINT/sources.f" ] || { echo "missing checkpoint sources: $CHECKPOINT" >&2; exit 1; }
    if python3 - "$CHECKPOINT/manifest.json" <<'NATIVE_JSON'
import json, sys
sys.exit(0 if json.load(open(sys.argv[1])).get('native') else 1)
NATIVE_JSON
    then
      echo 'set_global_assignment -name VERILOG_MACRO DORADO_NATIVE'
      cp "$HERE/native_timing.sdc" "$OUT/native_timing.sdc"
      echo 'set_global_assignment -name SDC_FILE native_timing.sdc'
      if python3 - "$CHECKPOINT/manifest.json" <<'INSTRUCTION_JSON'
import json, sys
sys.exit(0 if json.load(open(sys.argv[1])).get('instruction_monitor') else 1)
INSTRUCTION_JSON
      then
        echo 'set_global_assignment -name VERILOG_MACRO DORADO_INSTRUCTION_MONITOR'
        echo "set_global_assignment -name SYSTEMVERILOG_FILE $HERE/dorado_instruction_monitor.sv"
      fi
      if python3 - "$CHECKPOINT/manifest.json" <<'SDRAM_JSON'
import json, sys
sys.exit(0 if json.load(open(sys.argv[1])).get('storage_backend') == 'sdram' else 1)
SDRAM_JSON
      then
        echo 'set_global_assignment -name VERILOG_MACRO DORADO_STORAGE_SDRAM'
        cp "$HERE/sdram_timing.sdc" "$OUT/sdram_timing.sdc"
        echo 'set_global_assignment -name SDC_FILE sdram_timing.sdc'
        for DORADO_SDRAM_PORT in 'SDRAM_DQ[*]' 'SDRAM_A[*]' 'SDRAM_BA[*]' \
          SDRAM_nCS SDRAM_nRAS SDRAM_nCAS SDRAM_nWE SDRAM_DQML SDRAM_DQMH; do
          echo "set_instance_assignment -name FAST_OUTPUT_REGISTER ON -to $DORADO_SDRAM_PORT"
        done
        echo 'set_instance_assignment -name FAST_INPUT_REGISTER ON -to SDRAM_DQ[*]'
        echo 'set_instance_assignment -name FAST_OUTPUT_ENABLE_REGISTER ON -to SDRAM_DQ[*]'
      fi
      if python3 - "$CHECKPOINT/manifest.json" <<'CONTROL_MONITOR_JSON'
import json, sys
sys.exit(0 if json.load(open(sys.argv[1])).get('control_error_monitor') else 1)
CONTROL_MONITOR_JSON
      then
        echo 'set_global_assignment -name VERILOG_MACRO DORADO_CONTROL_ERROR_MONITOR'
      fi
    else
      echo 'set_global_assignment -name VERILOG_MACRO DORADO_CHECKPOINT'
    fi
    if python3 - "$CHECKPOINT/manifest.json" <<'DISPLAY_FIELD_JSON'
import json, sys
sys.exit(0 if json.load(open(sys.argv[1])).get('display_field') else 1)
DISPLAY_FIELD_JSON
    then
      echo 'set_global_assignment -name VERILOG_MACRO DORADO_DISPLAY_FIELD'
    fi
    if python3 - "$CHECKPOINT/manifest.json" <<'RECEIVE_MONITOR_JSON'
import json,sys
sys.exit(0 if json.load(open(sys.argv[1])).get('receive_monitor') else 1)
RECEIVE_MONITOR_JSON
    then
      echo 'set_global_assignment -name VERILOG_MACRO DORADO_RX_MONITOR'
    fi
    if python3 - "$CHECKPOINT/manifest.json" <<'PYMON'
import json,sys
sys.exit(0 if json.load(open(sys.argv[1])).get('pack_fault_monitor') else 1)
PYMON
    then
      echo 'set_global_assignment -name VERILOG_MACRO DORADO_PACK_FAULT_MONITOR'
    fi
    echo 'set_global_assignment -name ALLOW_POWER_UP_DONT_CARE OFF'
    cp "$HERE/checkpoint_timing.sdc" "$OUT/checkpoint_timing.sdc"
    echo 'set_global_assignment -name SDC_FILE checkpoint_timing.sdc'
    if python3 - "$CHECKPOINT/manifest.json" <<'PY'
import json, sys
sys.exit(0 if json.load(open(sys.argv[1])).get('hps_pack') else 1)
PY
    then
      echo 'set_global_assignment -name VERILOG_MACRO DORADO_HPS_PACK'
      echo "set_global_assignment -name SYSTEMVERILOG_FILE $REPO/fpga/mister/dorado_pack_mount.sv"
    fi
    if python3 - "$CHECKPOINT/manifest.json" <<'PY'
import json, sys
sys.exit(0 if json.load(open(sys.argv[1])).get('ethernet_host_port') else 1)
PY
    then
      echo 'set_global_assignment -name VERILOG_MACRO DORADO_ETHERNET_HOST_PORT'
    fi
    if python3 - "$CHECKPOINT/manifest.json" <<'PY'
import json, sys
sys.exit(0 if json.load(open(sys.argv[1])).get('ethernet_wire') else 1)
PY
    then
      echo 'set_global_assignment -name VERILOG_MACRO DORADO_ETHERNET'
      echo "set_global_assignment -name SYSTEMVERILOG_FILE $REPO/fpga/mister/dorado_ethernet_wire.sv"
      echo "set_global_assignment -name SYSTEMVERILOG_FILE $REPO/fpga/mister/dorado_ethernet_uio.sv"
    fi
    while IFS= read -r f; do
      case "$f" in /*) ;; *) f="$REPO/$f" ;; esac
      echo "set_global_assignment -name SYSTEMVERILOG_FILE $f"
    done < "$CHECKPOINT/sources.f"
  else
  for f in "$REPO"/verilog/generated/dorado_backplane.v \
           "$REPO"/verilog/generated/*-Rev-*.v \
           "$REPO"/verilog/generated/msa.v \
           "$REPO"/verilog/cells/cell_*.v \
           "$REPO"/verilog/cells/dorado_terminal.v \
           "$REPO"/verilog/cells/dorado_keyboard.v \
           "$REPO"/verilog/cells/dorado_trident.v \
           "$REPO"/verilog/vendor/6502/*.v \
           "$REPO"/verilog/vendor/riot/*.sv; do
    [ -f "$f" ] && echo "set_global_assignment -name SYSTEMVERILOG_FILE $f"
  done
  fi
} >> "$OUT/Dorado.qsf"

# Calling quartus_map directly does not execute the framework's pre-flow hook.
# Supply its required include for both staged and --flow builds.
printf '`define BUILD_DATE "%s"\n' "$(date +%Y%m%d)" > "$OUT/build_id.v"

echo "assembled $OUT"
echo "  framework: $TEMPLATE"
echo "  RTL:       $REPO/verilog"
echo "  core PLL:  $DORADO_SYS_MHZ MHz (requires timing qualification)"
grep -c SYSTEMVERILOG_FILE "$OUT/Dorado.qsf" | sed 's/^/  sources:   /'
