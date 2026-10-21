# Corrected SDRAM capture: native build and physical memory pass

The one-clock read-capture correction is grounded in the independent CAS and full-memory/port/board tests in the sibling component receipt. All 501 compiled sources reconstruct the preserved parent plus passive diagnostics and the explicit wait3-to2 correction. Root legacy controller bytes remain unchanged; use tools/stage_storage_read_capture.py for this variant.

All eight build stages, supplementary fitted inventory, 31 device timing summaries, 196 original, 56 backend, 8 first-error, 24 memory-test and 8 burst-trace timing categories pass. Minimum +0.118 ns. All 170 RAM entries and 165 parameter tables retain contracts; guest cache uses 40 M10Ks and BaseBoard ROM shares its seven-block pool. Fit: 31,033 ALMs, 53,383 registers, 332 M10Ks, 35 DSPs. Eighteen damaged-evidence controls reject. Exact executed reviewer, assembler and loader are preserved. No timing path or constraint is waived.

MiSTer14 RBF0c578915 passes both address patterns and verified zero-fill across all 8,388,608 raw 12-bit cells before storage release. Both metadata reads are phase6/ready1/failed0; the first burst is exactly A59 A58 A5B A5A A5D A5C A5F A5E. Selectors restore and Main detaches. Main14025/server14024 retain qualified binary hashes; Main remains sole SPI owner. Normal loader barriers preserved the previous closed seed and its 4,522-record trace with two barriers and no gaps.

Original firmware requests Cedar BFN113 and reaches the Cedar6.1 login (included actual internal screenshot). The FTP_UNSERVED diagnostic is not a missing microcode response: its handler occurs later in the same function. Three adjacent idle-login windows measure 2,592,835.27 / 2,593,033.74 / 2,592,895.80 instruction issues/s, mean 2,592,921.60 (15.56% of 16.67M/s; about 33.1% above the prior 1,947,686.01 login result). These count StartCycle issues excluding RepeatCur, not drained retirements. Clocks remain 50MHz/SYSPER4. This is one boot and login workload, not full Cedar desktop/input, clock acceleration, rollback, cold persistence, repeated-boot or sustained-operation qualification.

The Guest login/loader observation is a live prefix. Existing V38/V39 simulations retain their frozen legacy model and controller and must not be treated as physical timing validation. Bitstreams, active packs, compiled objects and the external vendor model body are excluded.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-sdram-read-capture/native-fit`
