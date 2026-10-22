# Host disk-cache changed-sector comparison: component PASS

The FPGA currently returns a whole track for a guest sector write. Main now captures the same bounded request into64KiB owned temporary RAM, compares overlapping portions of512-byte host sectors with its shadow and marks only changed sectors. Existing dirty bits survive identical updates and return-to-original bytes while an older worker snapshot is in flight. No API/object layout changes. Whole-track SPI transfers remain unchanged; no native speed gain or hardware boot/deployment is claimed.

Full Linux cache and actual adapter/worker/SPI/lifecycle/error tests pass on misterubuntu. Includes4.2s worker stalls, one-job bounds, all64MiB physical oracle, partial IO/EINTR, ENOSPC/fsync EIO, ownership and process/core-load close barriers, two existing mutation controls. New compact tests verify zero physical IO for repeated identical31-block unaligned track, four changed sectors including partial ends, exact whole-file close/remount and old-snapshot/update/reversion races.

Both compact tests also pass as ARM binaries on MiSTer14, using only256KiB/64KiB temporary files. The full64MiB suite is NOT run alongside live Main. Original-code control fails with expected exit23, unchanged track dirtied sectors. Stdout/stderr buffering places this expected diagnostic last in the combined ARM log. Binary hashes, frozen sources and exact test scripts are retained; binaries and FPGA images are excluded.

The first adapter setup failed because the isolated Main copy is not a Git repository; copying pinned original input.cpp then running the exact patched handler suite succeeded. Live Main21e6/server08452 remain unchanged during native Rollback testing. Later deployment must use the normal guest/loader close boundary and a pinned performance comparison. Host-write reduction does not remove full-track SPI pauses or reach the original60ns cadence.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-disk-cache-changed-sectors`
