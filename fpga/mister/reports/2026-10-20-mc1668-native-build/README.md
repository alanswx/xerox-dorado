# Corrected MC1668 native build

**Full build/source/memory/timing PASS. Firmware/Cedar hardware regression is
pending; this receipt does not claim a hardware speedup or completed clock migration.**

RBF SHA256: `15730f449bae786a3cee9ffa93d4bbc7de2e90083483f1a59cfd607c02af49a8`.
It is 3,978,736 bytes; local image:
`build/rtl-executive/mc1668-native-terminal-20261020/Dorado.rbf`.
Remote project: `/home/alans/mister/Dorado_Mc1668_20261020` on misterubuntu.

The exact Native59 baseline is reconstructed from 413 compiled / 498 archived
files. Only the generic MC1668 and its actually instantiated native copy change.
Both normal processes are qualified against the manufacturer latch truth table.
The existing state-admin interface remains inactive. Main/server, clocks, pins,
memory layout, original board wiring, source assets and constraints are unchanged.
The fixed cadence remains 50MHz/SYSPER4. No R/T clock kernel is integrated here.

All stages are terminal: map, fit, STA, clock coverage, four corners, control/
ContB/all-ten-board crossing reports and assembler. All ten recorded process
IDs were absent at collection. Exact source/configuration readback passes.
All 31 timing summaries / 196 detailed categories / four conditions pass; minimum
detailed slack +0.115ns. Forty board/corner crossing collections have nonempty
endpoints. Illegal/unconstrained clocks remain zero; I/O coverage matches Native59.
There are no new constraints or timing exceptions.

Resources: 30,468 ALMs / 52,881 registers / 332 M10Ks / 35 DSPs. MC1668 synthesis
rows retain 22 normal latch registers versus 38 before, removing 16 obsolete
edge-history registers; duplicate inputs share storage. Global physical register
counts also reflect ordinary fitter optimization and are reported separately.

Strict existing readers verify instruction RAM, all twelve processor memories,
MemC MLABs, IFU, all fourteen cache banks in the original 40-block layout, map,
display and disk/Ethernet memories. Geometry and OLD_DATA requirements remain;
all 210 DskEth child state counts match Native59. Six damaged/rehashed review
controls reject: relaxed constraints, changed qualified cell, missing detailed
path, negative slack, wrong memory read-during-write mode and incomplete source
coverage. The separate native-cell and Initial component receipt supplies the
truth-table/pause proof and original ROM diagnostic gate.

The first pending reviewer draft mis-added the register totals (36/20); the
accepted reader checks 38/22 and the exact per-position rows. It also decodes
Quartus17's Latin-1 fitter report. Raw bytes remain losslessly preserved.
These corrections change no HDL, completed simulation or RBF.

Raw sources/reports/logs and the executed review tools are gzip-compressed;
the manifest records every raw byte count and SHA256 and passes readback.
Quartus databases and RBF bytes are omitted. `qualification.json` is compressed
when larger than 100KiB. To review from the matching checkout after extraction:

```
python3 tools/audit_mc1668_native.py /absolute/extracted-build --qualified /absolute/native-cell --initial /absolute/initial-component --output /absolute/qualification.json
python3 tools/test_audit_mc1668_native.py /absolute/extracted-build --qualified /absolute/native-cell --initial /absolute/initial-component --output /absolute/fresh/reader-controls
```

The image must be placed at `core/output_files/Dorado.rbf` for its hash check.
The matching original Native59 receipts and exact review-tool versions are
required; archived tool source permits reconstruction. No second full simulator
replay is claimed here. The next step is the normal Main loader, old-volume
preservation and a fresh original-firmware Cedar boot/input/persistence test.
Complete FPGA snapshots remain deferred.
