# Native SDRAM buffer capacity: component comparison

The exact reviewed 57.142857 MHz native9e controller supports four or sixteen fully tagged BL8 lines. Its128-way interleaved group mapping makes sixteen lines sufficient to hold one word across all groups. This is a transparent physical backing buffer, separate from the original Dorado4K-word architectural cache.

Identical512-request synthetic sweeps require64 physical reads/3888 host ticks at4 lines versus16 reads/3331 ticks at16 lines:75% fewer physical reads and14.326% fewer component ticks. Independent pin-level chips also pass existing rank/bank, held-response, write invalidation, reset/orphan-fill and refresh checks.

An unmutated focused harness passes. Disabling invalidation, allowing aborted fills, dropping a group-tag bit and capturing one clock early each compile successfully and fail the intended data/orphan oracle. The first control attempt stopped earlier than its expected diagnostic at a sequential read-count assertion; its terminal failure is preserved. V2 places the explicit stale-data oracle first and validates that harness against unchanged RTL.

The deployed hit counter is unwired; zero does not measure zero hits. No production RTL, FPGA, Main or daemon changed. This is not an actual native instruction-speed result. Original144-chip board/full native firmware plus independent all-device fit/timing and pinned hardware comparison remain prerequisites for activating16 lines.

Run tools/probe_native_storage_cache.py with the qualified frozen source stage, preassembly qualification and a new output directory, then tools/test_native_storage_cache_controls.py with that positive directory and a separate new output. All original commands and frozen inputs are retained. Verify this receipt with tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-cache-capacity.
