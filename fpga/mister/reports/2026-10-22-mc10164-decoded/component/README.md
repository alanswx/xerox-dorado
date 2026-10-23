# Decoded MC10164/MU10164 data path: exhaustive component proof

The actual rejected native path traverses several processor/result/diagnostic muxes after the ALU before BaseBoard f62 RIOT read data. This experiment predecodes unchanged select/active-low-enable controls before reducing the late data, using explicit retained decode wires. Registers, package pins and board connections are unchanged. It is an experiment on exact qualified690f cells, not a canonical-cell or FPGA deployment change.

Both aliases match all4096 binary input combinations in both simulators. Icarus additionally exhausts all16777216 four-state combinations, including disabled output, X/Z data and unknown select indexes. A self-equality/conditional guard preserves the original array-index X behavior for unknown selectors; it is constant true for binary hardware controls. Five actual wrong-select/lost-enable/removed-unknown-guard variants reject. Fatal UNOPTFLAT is enabled and actual compile logs contain no circular warning. Frozen inputs remain unchanged. These are functional component proofs, not native timing, physical hazards, firmware or a speed gain.

All executed inputs, tools, terminal action/results and rejected variants are preserved without simulator binaries. Verify with `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-mc10164-decoded/component`.
