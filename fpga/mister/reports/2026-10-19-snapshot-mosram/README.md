# Independent MosRam socket state qualification

Complete original14/16-bit socket model access:16,384/65,536RAM bits plus
21/23history bits each. Row/column, RAS/CAS/WE history, pending write/data,
raw read pipeline and held output remain independent per socket. No address/
history merging or C-state import. Other densities explicitly reject.

Accepted original/oracle/physical-probe simulation v3 passes5,440,612host
edges/4,997,354administrative requests/all81,964modeled bits, both complete
invalid-index ranges and transport padding. The console's held=9901 counts
ordinary backend-hold calls, not all frozen scan edges.69,239writes/socket,
138,478old-data collisions and136,145held-output reads/socket execute through
normal pins. Destructive whole RAM/history writes, complete restore, quiet
release/recapture, the first pending-write commit/use of saved read pipeline
and20,000subsequent cycles pass. Eleven broken RTLs plus unsupported density
reject. Earlier negative tests expected a RAM diagnostic where the independent
history oracle failed sooner; only expected diagnostics were corrected.

Fresh original XML classifies every mutable field for both densities;
12damaged schemas/inventories reject per density. Derived address/output/
unused-pin constants are recomputed only under the exact original source hash.
The older classification log printed history-density order backwards; the
schemas were correct, final output explicitly says21/23for14/16.

Matched50MHz/20ns embedded paths/staged-input/positive-edge fits pass all four
operating conditions,min+0.166ns,no illegal/unconstrained clocks/I/O.81,920RAM
bits/ten M10Ks retained in both variants;59->137ALM/57->256registers include
shell/admin/pruning, not enabled full-core costs. Source macros and table Type
remain AUTO, as in the unchanged original. Exact2/8M10K counts, zero MLABs,
implementation dimensions and actual M10K_X/Y/N locations prove physical
placement. Both banks' actual mixed-port modes are OLD_DATA. Eight corrupted
source/SDC/macro/memory/hold/warning controls reject. Initial auditors expected
an explicit M10K style label copied from the cache helper; corrected to the
source's AUTO labels plus stronger physical placement checks. No RTL/SDC refit
or fitted-report rewrite was used; all earlier drivers/logs are preserved.

Raw text is gzip-compressed unchanged. This is socket-model qualification;
complete MemX board classification/restore/timing/integration, all remaining
boards/device/ARM state/global barrier and actual FPGA restore remain required.
Production RTL/clocks unchanged; no full snapshot or speed gain claimed.
