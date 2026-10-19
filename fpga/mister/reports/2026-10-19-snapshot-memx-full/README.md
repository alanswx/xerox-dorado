# Complete modeled MemX state qualification

The frozen native 64K-map MemX board now has independent export/import for
all 1,378,312 modeled mutable bits: 74 FF packages (819 bits), ten small
files (640 RAM + 100 write-window bits), 21 independent 64K map planes
(1,376,256 RAM + 483 socket-history bits), and 14 board phase bits.
Eight immutable PROMs (2,048 bits) are hash-pinned. No socket histories or
addresses are merged. The 24-bit state index selects 106 packages with
17-bit per-package rows; each large RAM has separate history words.

The accepted v2 simulation passes 26,630,466 host edges, 26,429,183 held
edges, and 26,419,185 administrative requests. It covers all modeled bits,
all 16,777,216 indices, padding, an independent per-socket RAM/history oracle,
destructive whole-state writes and restore, recapture, the first resumed
pending write/read pipeline, and 20,000 further cycles. All 21 map writers
and all ten small-file writers execute through original board pins. Nine
broken RTL variants reject. The earlier attempt passed the candidate but
expected the response-truncation fault at RAM export; the independent
auxiliary-state check rejected it sooner. Only that expected diagnostic
changed; both executed drivers are preserved.

Fresh original-board XML classifies every modeled bit and the eight PROMs;
18 damaged schemas/inventories reject. The 1,060 excluded diagnostic bits
are simulator-only, with their original source guards checked.

Matched isolated 50 MHz fits pass all four operating conditions, minimum
snapshot slack +0.131 ns, with no illegal/unconstrained clocks or I/O.
Both variants retain all 1,376,256 map RAM bits in 168 M10Ks. All 21 actual
physical rows retain 65,536x1 geometry, eight M10Ks each, unique M10K
locations, zero MLABs, and OLD_DATA mixed-port behavior. AUTO source/style
labels remain unchanged. Shell/admin/pruning counts are 793 -> 2,920 ALMs
and 1,540 -> 3,321 registers; these are not enabled whole-core costs.
Ten corrupted source, PROM, constraint, memory, placement, warning and timing
controls reject.

Raw text is gzip-compressed unchanged. Executed sources, commands, initial
attempt, fit provenance, reports and auditor are included. Native57 is
building this adapter into Native56b with administrative inputs inactive.
Complete machine barriers, remaining devices/ARM state, actual FPGA save/
restore, original 60 ns clocks and final release qualification remain open.
No speed gain or complete-machine snapshot is claimed.
