# Complete MSA chip-width cache: qualified component saving

Matched complete-MSA fit: **1,721 compact -> 1,600 chip-width ALMs**, saving 121.
Same 50 MHz registered shell, 991 virtual pins, 2 ns max/0ns min IO, device and area
policy. All four conditions pass setup/hold/pulse width with minimum +0.119 ns,
zero unconstrained clocks/IO; recovery/removal have no paths in this synchronous
fixture. Five damaged fit/source/constraint cases reject. Actual builder 2425360
is absent and terminal. No assembly/deployment or speed gain is claimed.

Only unused cache_data padding is trimmed, from 16 bits to WIDTH 8/12. Raw 16-bit
requests/responses, all original guest pipeline registers, 866 FF/history bits,
helper/pin/parameter equations and clock/hold behavior remain. Each cache holds
the actual original chip vector, including all data/ECC bits in WIDTH. Actual
MSA state 1538 bits /378 fields =866 FF/history +672 bridge; 86 words, last 8 bits.
All remaining actual physical fields are exported. Older snapshots require
rejection by a new schema; no old-representation compatibility claim is made.

Both original RTL oracles deliberately receive NONZERO raw high padding bits.
Frontend: 59,341 host /1,768,540 checks /5,010 guest /10,930 requests /906 writes /
12,032 scan /9 snapshots; full 1,310,720 original chip-array bits compare, with
pending reads/writes/write-done/reset and eight bad actual RTL cases rejected.
MSA: 879,344 host /13,288,504 checks /40,002 guest /55,568 requests /8,568 writes /
524 state operations /793,615 scan edges. All remaining physical fields, original
normal outputs and raw traffic agree through full inversion/restore, first
resume, 20,000 further guest advances, invalid bounds, abort and quarantine.
Seven actual bad RTL variants reject. These are valid-state comparisons, not
four-state equivalence of arbitrary older implementation representations.

Exact source inverse returns qualified 1650-bit compact board/helper bytes after
only cache type/assignment/scan-width edits. Ten rehashed damaged source/schema
cases reject. Fresh XML accounts for all 1538 candidate bits, all 2690 original
reference bits and 81 private bits; no sequential exclusions. Fourteen geometry/
reference-presence/alias controls reject. Native/raw-memory/ARM state is separate.

All 500 raw members pass readback. Extracted board/helper/fixture generation,
source/XML/fit controls and BOTH original-RTL/DRAM test rebuilds pass against
committed immutable original and compact receipts through the recorded reports
symlink. Tools closure and original cell bytes are included. Objects/databases/
executables are excluded; sources/reports are lossless gzip, large JSON gzip.
This component saving does not prove whole-core capacity or timing.
