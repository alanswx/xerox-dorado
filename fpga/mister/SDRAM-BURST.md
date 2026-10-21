# Experimental raw SDRAM burst buffer

`dorado_storage_sdram_burst.sv` is a separate backend behind the existing raw
16-bit port. Four fully tagged lines hold eight halfwords each (64 data bytes
total). BL8 reads prefetch adjacent raw cells; every write reaches SDRAM and
invalidates its matching line. The guest's CPU,4K-word cache,map,ECC and DRAM
front ends retain their original behavior. Main and the ARM daemon are unchanged.

The backend retains request ownership, response backpressure and accepted
operation drainage across reset. An aborted refill cannot revalidate a line
or leak a completion. It reserves refresh headroom with a6us threshold, enough
to drain a complete burst and refresh both ranks at the supported20..50MHz.
Mode0x223 selects sequential BL8/CAS2 reads with single-location writes.

The isolated pin suite passes seven positive cases (BL1 reference;1/2/4 lines;
20/33.333/50MHz;50MHz with6ns clock delay) and six runtime rejection controls.
Those catch stale write data, orphan refill, early sampling, wrong burst mode,
short tRCD and the original chip-model hold glitch. The existing BL1 suite also
passes all frequency/delay cases and three timing-error controls after the
model correction. The fix's primary source is in the model README.

The complete64K-chip MSA fixture compares every serial/ECC output over12,012
logical board cycles and all9,437,184 physical storage bits. Baseline and burst
produce the exact same12,380-record raw request/final-memory sequence. Physical
read commands fall3008 ->750 and host cycles122779 ->113841 (7.28% fewer).
The raw sequential pin workload takes361 ->240 host ticks; the alternating
two-line workload takes176 ->122. These are component measurements, not native
instruction throughput or Cedar qualification. Unrelated cold reads take a
longer physical burst; workload locality must justify selecting this backend.

Run `tools/test_storage_sdram_burst.py --output FRESH_DIR --controls` and
`tools/test_storage_sdram_burst_board.py --native FROZEN_NATIVE_RUN --output
FRESH_DIR`. Every run freezes its inputs and records processes/results. Earlier
V1/V2 failed attempts are retained: V1 exposed the model's natural-burst OE
glitch; V2 passed50MHz but exceeded the model's refresh interval at20MHz.
V3 corrects the refresh headroom and passes. Do not restart those attempts.

A native timing candidate is staged from the qualified MC1668 parent. Its only
production changes are the wrapper's raw-backend type and QSF source entry,
plus this new module. It retains50MHz/SYSPER4,SDRAM,disk buffering,direct DDR
networking and inactive administrative state access. Full-device timing and
memory review, original-firmware Viewers/input and pinned speed remain pending.
The component results alone do not qualify assembly or deployment.

Frozen component results, rejected attempts, raw traces and native source/live
prefix are in [the evidence receipt](reports/2026-10-21-sdram-burst/README.md).
