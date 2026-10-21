# Native passive first-burst trace: physical displacement confirmed

All eight build stages, register inventory, full timing/memory/source review, sixteen rejection controls and guarded assembly pass. Minimum detailed slack +0.115 ns; 31 device summaries, 196 original, 56 backend, 8 error-observer, 24 memory-test and 8 burst-capture timing categories. All 170 RAM entries and 165 parameter tables preserve contracts; 332 M10Ks, 35 DSPs, 30,928 ALMs, 53,376 registers. All 97 logical trace bits are present. The unused alternate output-name filter warning is explicitly classified; no timing constraint/path is waived. Exact executed reviewer/assembler/loader are preserved separately from current worktree tools.

MiSTer14 captured expected A59 A58 A5B A5A A5D A5C A5F A5E versus actual A58 A5B A5A A5D A5C A5F A5E A5E. The first seven words match the next addresses and the last word repeats. Markers B157/B178, stable failed phase 1, restored selectors and detached Main-owned readout pass. The guest remains held before firmware execution. Main 12905 and daemon 12904 retain their earlier binary hashes; RBF SHA 36ca8c6bf4dfb95d122a7a284633336a986295e2b41afbbbb2b256919e075574.

Normal Main barriers preserve the closed Selftest1 seed unchanged, 4,229 trace records, two barriers and no historical/transition gaps. This is a diagnostic timing/build pass and a physical memory failure, not Cedar or speed qualification.

Subsequent independent CAS tests identify the old local SDRAM model's extra launch cycle: a Micron-authored reference launches one clock earlier and agrees with hardware. The staged controller correction and model correction are qualified in the separate read-capture receipt; the previous simulation pass is not physical-memory validation. No controller wait/data/state changes were made in this passive trace image.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-storage-burst-observer/native-fit`
