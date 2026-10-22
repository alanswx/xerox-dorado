# BaseBoard crystal ratio: opt-in component correction

The actual RevAm Basebd01.sil drawing (PDF page3) labels a10MHz crystal and1MHz MCPreClk. e07 reloads1011 and divides by FIVE; g08 divides by two. The old MC12061 fixed fabric/8 therefore gives fabric/80. At boardSYSPER4 it runs5/3 too fast relative to the CPU; it is not a divide16 counter or32MHz crystal.

MC12061's new optional SYSPER accepts samples per60ns instruction, so a future board supplies2*boardSYSPER. Default0 retains the old divider waveform exactly. Package-pin chain tests at board ratios2/4/8/16 produce6000 crystal/600 MCPreClk cycles per10000 ordinary instructions in BOTH Icarus and Verilator. Wrong fixed divider, missing factor2, wrong crystal and wrong preload reject.

Original full BaseBoard/6502/EPROM tests pass the exact F3A7 reset vector/eight startup bytes with both legacy and explicitly scaled clock. The failed nested-defparam compile is preserved; the corrected staged generated board changes only d08's parameter. This is not full Bootstrap/Initial/113/Cedar/device-timing qualification. No generated production board or FPGA was changed.

Reproduce the isolated gate by compiling `verilog/verilator/tb_baseboard_crystal.sv` with the four frozen cell files, top tb_baseboard_crystal, using Icarus `-g2012`/vvp or Verilator `--binary --timing`. Modes1..4 select the rejecting controls (`-Ptb_baseboard_crystal.MODE=N` in Icarus). The existing `make -C verilog baseboard-test` tests the legacy default; the frozen staged board and compile log record scaled ROM integration.

Verify lossless evidence: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-baseboard-crystal`. No instruction-speed or60ns/full-firmware/timing claim.
