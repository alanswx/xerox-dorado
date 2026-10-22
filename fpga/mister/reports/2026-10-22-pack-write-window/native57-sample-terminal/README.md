# Address-sampled write-window fit: positive timing, preassembly review pending

The same V4 builder2338117 completes all nine original stages; reporter2404367 completes its three read-only stages. Collector2414040 confirms all recorded build/report PIDs absent and all503 compiled source hashes unchanged. The974-member downloaded collection passes exact SHA/readback; this981-member receipt also includes the initial review and frozen report/collector tools. No fitter restart, source/clock/SDC relaxation, assembly or hardware load occurred.

All31 device summary checks are positive (+0.269ns setup/+0.242ns hold). All316 original detailed timing categories plus16 new physical half-cycle sample-input/output categories pass across all four operating conditions; detailed minima are+0.210ns setup/+0.115ns hold. Both sides of the new sample have over4.15ns setup and8.34ns hold margin in these reports. Unconstrained-path summaries match the qualified parent.

The strict parent RAM comparison initially rejects a real physical change: cache banks3/5/8/10 list2 M10Ks each instead of their shared pair's3. This is classified explicitly with the existing independent40 reader, which proves all14 geometries, OLD_DATA modes, disjoint sites/capacities and294,912 logical bits. Only those four physical counts are normalized in the in-memory comparison; original reports remain byte-exact. All other RAM behavior,170 logical rows/165 parameter sets and shared seven-block BaseBoard ROM pool pass strict comparison. Total M10Ks rise330 to332; DSPs remain35. No memory-mode waiver is introduced.

This initial review is not assembly authorization: exact source-recipe regeneration, detailed sample/clock/enable inventory, warning differences and the guarded assembly gate remain pending. Original firmware, physical memory, Cedar/rollback, closed-volume behavior and pinned speed tests must still qualify the resulting candidate separately. The live MiSTer remains on the earlier reviewed core.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-pack-write-window/native57-sample-terminal`
