# K1115A complete modeled state qualification

The source-pinned adapter preserves the original signed 32-bit accumulator,
oscillator output, initialization and parameter arithmetic verbatim. It exports
all 33 bits as 18 + 15 bits, with zero read padding and ignored write padding.
It is a snapshot primitive; it does not correct oscillator rates or timers.

Original-model and independent signed-arithmetic oracle tests pass across
SYSPER 2/4/16 and 10/20/50 MHz frequency parameters (nine independent cells):
152,259 host edges, 115,050 advances, 2,212 administrative requests, 2,220 held
edges, and 12 observed signed wraps. All 33 bits per cell are written/read,
including negative accumulators and exact comparison boundaries. Invalid
indices, freeze entry, running requests, destructive state changes, restore,
quiet release/recapture, the first resumed edge and 20,000 later cycles pass.
Eight broken RTL variants and two changed-original-source controls reject.

Fresh original XML classification covers the accumulator and output; eight
damaged schemas reject. The unused constant wire is optimized into an
initial-only candidate by Verilator, and is reconstructible under the full
original-source hash. The classification's access map splits the accumulator
into 18 + 14 bits and places the output in bit 14 of the second word.

Matched isolated 50 MHz routed fits pass all four conditions, minimum snapshot
slack +0.145 ns, no illegal/unconstrained clocks or I/O, no cuts, no RAM.
Shell/admin/pruning counts are 514 -> 735 ALMs and 333 -> 762 registers.
Five damaged-source/constraint/macro/hold/warning controls reject. These are
not full-board or enabled whole-machine costs.

Earlier harness attempts are retained: a Verilator C++ reserved-name rewrite
was avoided by renaming a fixture port, and C++ test input writes were masked
to their declared 18-bit width. Neither required an RTL change. Earlier
classification attempts corrected the unused constant's category and made
the field-to-word access description explicit. Raw text is gzip-compressed
unchanged; obj/db/executable products are excluded.

Next is complete DispY state access using its actual compiled field-board
source and exact current parameter binding. Full machine state/barriers,
physical FPGA restore and the original 60 ns/timer migration remain open.
