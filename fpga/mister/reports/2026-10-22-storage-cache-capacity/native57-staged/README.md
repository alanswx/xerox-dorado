# Native backing-buffer candidate: staged, fit pending

The exact reviewed9e503-source parent is preserved in the pack-write-window native57 sample/assembly receipts. This stage changes precisely one native wrapper: permitted CACHE_LINES4 to16, plus the previously unwired storage_events[8] to initialized&&backing_cache_hit. All project-root relocations are separate from HDL changes; clocks, constraints, CPU, original architectural cache/Map/ECC, physical SDRAM controller, disk adapter, Main and network remain unchanged. The larger physical buffer is256 bytes.

Existing independent physical-chip and complete original144-chip MSA gates pass4/16 lines. Synthetic128-group sweeps improve, original one-board workload does not; actual hardware speed remains unmeasured. Four real bad staging inputs (changed native HDL, wrong clock, a nonrejecting mutation gate, and changed original-board trace) each reject before writing a candidate, with original parent inputs unchanged.

One fit-only job launched as builder3388976 at /home/alans/mister/Dorado_StorageCacheNative57_1_20261022. Inspect that job; do not restart after an observation timeout. No assembly, RBF load or qualified timing result is claimed. All-device timing, coverage, actual RAM/register mapping, warnings, source/recipe review and assembly guards are required before hardware firmware/performance tests. The original60ns CPU phase goal is separate and remains open.

Exact stager, rejection driver, sequential builder,503-source manifests and changed wrapper are retained. Reconstruct against the prior source-bound9e parent; do not substitute the older canonical BL8 controller. Verify: tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-cache-capacity/native57-staged.
