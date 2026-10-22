# Native write-window fit rejected: real address-capture hold path

All9 builder stages terminate normally, but the actual timing summary fails: setup minimum+0.686ns, hold-0.044ns at slow100C and-0.008ns at slow-40C. No assembly or FPGA load. All299 collected report/log/manifest hashes verify; the complete terminal reports are retained here.

Worst path is msaOdd/u_dram_addr_14/col[3] -> u_storage_14/saved_addr[3], same host clock,0.634ns clock skew versus0.590ns minimum data delay. It is a real SDRAM address-preview/request capture, not a new disk-window RAM path and not a waived or halted-only path. The change in placement exposed the failing hold. Keep all clock and data timing constraints.

A separate stage at /home/alans/mister/Dorado_PackWindow57Hold_20261022 asks Quartus to physically repair hold on ALL_PATHS, already supported by existing component recipes. It preserves the exact503-source recipe and tested adapter, adding only the fitter assignment in QSF. No SDC relaxation, clock change, seed search, functional retiming or firmware shortcut. Builder2166275 is RUNNING; this receipt contains its staging only, not timing completion or permission to assemble. Follow its recorded run-state/PIDs. Current hardware remains RBF7c4d39e4/Main21e6/daemon08452.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-pack-write-window/native57-rejected-hold`
