# Actual BaseBoard 2716 EPROM read-pipeline state

Four-state simulation, fresh actual-board classification and corrected
matched50MHz routed fits PASS. All eight sockets'72read-pipeline bits are
preserved. No production BaseBoard or live MiSTer RTL changed.

Each socket exports d[7:0] and en_q in local word0; indices1..3 reject without
writing. Read padding is zero. Original address/data-pin equations and ROM
initialization remain unchanged. Four assets are immutable; four empty
INIT_FILE sockets retain unknown/uninitialized memory. They are never
silently replaced by zero-filled images. A source-hash guard pins this rule.

Accepted simulation-v3 uses four-state Icarus, eight untouched original cells
and an independent immutable-ROM/read-pipeline oracle.34735host/331held/
328accepted/184writes/328late changes pass: all72bits and32combined indices,
every original ROM address before and after restores, empty-socket X values,
X/Z pipeline states, CS/PD combinations, input changes during backend holds,
freeze/held separately, padding/invalid/running/entry/thaw requests, dense
save, candidate-only destructive continuation/restore/first real-edge resume
and20000subsequent steps. Eleven broken RTL variants and two changed sources
reject. v1 passed simulation but its hardware mapping is rejected below;
v2 is retained REJECTED for Icarus declaration-after-use under default_nettype
none. v3 moves that declaration before use and adds immutable captured helper
source and restore-data/early-resume controls.

Fresh actual nominal BaseBoard SYSPER4/CLOCKGEN N4 classification accounts
for every EPROM field: d/en_q SAVE, image arrays IMMUTABLE_IMAGE, unused-power
constant0 and derived address/outputs. Empty arrays have no assignments in
the original source and are absent from the writer-based candidate inventory;
this does not imply zero contents. All other board state remains present and
unclassified.13bad schemas/source/image/socket controls reject. Original
board/cell/four firmware assets match Native59 compiled-input hashes.

The first fit is REJECTED: directly restoring the ROM read register d turns
65536ROMbits into logic (1529ALMs/563regs/zeroRAM). The corrected adapter keeps
the original read register/ROM inference and a separate restored-data value
selected until the first real guest advance. No warm-up edge is inserted;
the first ordinary edge captures the current address and enables normally.
Four-state restore equivalence covers this added private implementation state.

Corrected matched opposite-edge launch/capture fits share50MHz/10ns embedded
paths/2nsmax0nsmin virtual-I/O budgets, no cuts. Both variants pass all four
conditions, minimum candidate slack+0.165ns, no illegal/unconstrained clocks
or I/O. Each retains four2048x8 ROM macros/65536bits/eightM10Ks, two distinct
sites per independently addressed image. All8192firmware bytes per variant
are decoded from the actual synthesized MIFs and compared with the exact
source assets. Empty sockets infer no ROM. Shell totals271ALM/89regs ->
347ALM/531regs include launch/capture/admin/pruning, not full-core overhead.
The isolated independent-address packing is not Native59's shared-address
packing. Twelve damaged-fit controls reject, including an altered synthesized
firmware byte. All28compiled input hashes and an independent audit pass again
after archive extraction. Raw sources/reports/logs/XML/MIF/largeJSON are gzip
stored unchanged. Only the actual HDL MIFs are retained from Quartus db;
other build databases and Icarus executable objects are excluded.

No complete BaseBoard or machine save/load is implemented. Next: remaining
279clock/reset/FFbits, complete board qualification, then enabled whole-core
budget and coherent backend/input/ARM/server/matching-disk save/restore.
