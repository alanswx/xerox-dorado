# Actual native BaseBoard inventory — review only

The fresh Verilator elaboration uses frozen Native58's actual
`BaseBd_nominal.v`, explicit SYSPER4, all compiled cells, the netlist-derived
6502 and the MOS RIOT implementation. It identifies **40,366 sequential
bits before classification**. This is an inventory, not a complete snapshot
implementation or a state-completeness proof.

The large groups are 32 i2125 RAM slices including registered reads (32,800
bits), the actual 6502 node vector (1,725 bits), and five RIOTs (5,485 bits),
plus their five wrapper phase-history bits. RIOT's block-local `old_pa7`
register is included. Ten RIOT field-parent lexical scopes in the initial
report refer to five actual devices, not ten RIOTs; the reproducible report
lists the five device paths explicitly.

Eight EPROM sockets retain 72 registered read/output-enable bits in total.
Only four sockets have explicit INIT_FILE assets in this compiled board.
The remaining socket memory semantics need explicit review; this report
does not assume their uninitialized contents are zeros. CLOCKGEN binds N4,
HALF2, ECP_LO0, SCP_LO1. Other clock, reset, edge and precapture histories
remain in the review list. Diagnostic/initial-only/derived candidates still
need classification.

`initial/` preserves the first exploratory driver and outputs.
`reproducible/` preserves the rerun from `inventory_baseboard.py`, including
commands, fresh XML, full candidate list and source provenance. All 140
archived source/tool inputs were read back and hash verified. Raw sources,
logs, XML and JSON larger than 100 KiB are losslessly gzip compressed.

Reproduce with:

```
python3 fpga/mister/snapshot/inventory_baseboard.py \
  --frozen-root build/rtl-executive/native58-dsketh-baseline-src \
  --output NEW_DIRECTORY
```

The archived board/cells/vendor/EPROM inputs identify the actual source
scope. External wrapper/backend/input/ARM/server/disk-generation state,
coherent machine freeze and atomic FPGA restore are outside this inventory.
