# Complete actual BaseBoard state: isolated save/restore qualification

Simulation v8, fresh complete original-board XML classification and matched
50MHz routed fits PASS. The production/live core is unchanged. This is a
complete board adapter, not a whole-machine pause or physical snapshot.

All 40,366 modeled state bits are included: original 1,725-node netlist 6502,
five 1,098-bit RIOT wrappers, four 1,024x8 SRAM banks plus 32 raw read bits,
eight nine-bit EPROM pipelines and 279 clock/reset/FF bits. The board retains
its actual SYSPER4/CLOCKGEN N4 binding and original pin/clock/ROM equations.
48 selectors expose 4,905 valid words; the remaining 17-bit indices error
without writing. Parent decoding checks complete row widths before truncating
indices for children. Child replies are routed on the same accepted edge;
FF reads are captured before writes, and write/invalid replies return zero.
The caller must hold every external consumer before confirming the boundary.

Every shared helper is byte-identical to its prior qualification, including
the CRLF vendor CPU clone. The new MCS6502 wrapper is combinational and only
forwards access to the qualified node adapter. Original board/cell/vendor
sources are compiled byte-identically. The existing named riot_stuff block
allows read-only PA7 observation without changing original code. Four exact
EPROM images are immutable; four empty sockets retain uninitialized semantics.

Accepted v8: 526,012 host / 427,972 held / 402,862 requests / 33,334 late enable
changes. Every state bit, all 131,072 indices, padding, entry/running/thaw
requests, dense capture and complement writes, a quiet recapture, destructive
candidate-only continuation, full restore, first real-edge resume and 20,000
more cycles pass. All 100 public output bits and full physical state compare
with the unchanged original; independent SRAM/read-pipeline and RIOT/phase
models also pass. There are 5,940 normal RIOT rises. The deterministic initial
SRAM pattern is fixture-only; no CPU, RIOT or clock state is planted. Normal
extra-SRAM writer counts are all zero in this standalone stimulus. Their
ordinary write semantics remain covered by the earlier 32-cell/byte-bank
oracle tests and exact wire-list grouping; this is not an all-writer full
board qualification or a standalone firmware-boot completion claim.

Eleven broken variants reject: ungated FF advance, running request acceptance,
invalid FF rows, wrong reply selector, wrong SRAM selector, lost raw SRAM read,
CPU word alias, missing RIOT phase row, lost EPROM enable, counter word alias
and ignored clock freeze. Fresh complete XML classifies all 1,739 candidates:
40,366 SAVE bits and 65,536 immutable image bits, with no sequential state
excluded. It checks every physical field slice against nonaliased transport
bits. Seventeen bad schemas reject, including lost procedural PA7 history.
Other candidates are source-pinned continuous combinational/constant expressions.

Both matched staged-input/positive-edge launch/capture fits use 50MHz, 20ns
embedded paths and 2ns/0ns virtual I/O budgets, no cuts. All four conditions
pass, minimum candidate slack +0.159ns, no illegal/unconstrained clocks/I/O.
Both retain 98,304 SRAM/ROM bits in 11 unique M10Ks: four SRAM sites and seven
shared-address EPROM sites. Logical memory-row block counts are not physical
site counts. SRAM mixed-port collisions are Old data. Every synthesized
firmware byte in both variants matches its asset. Synthesis retains all 279
FF bits exactly; CPU has 1,744 registers and each RIOT 1,115 including private
response state. Shared freeze/interface registers can merge or prune. Fitter
shell totals 3,754ALM/6,657regs -> 10,702ALM/8,356regs. This delta is not a
whole-core enabled budget. Eight damaged-fit controls reject. All155compiled
inputs, accepted simulator inputs, helper hashes and independent audit pass
again after archive extraction. Raw sources/logs/XML/reports/MIF/large JSON
are gzip stored unchanged; only actual HDL MIFs are retained from Quartus db.

Earlier attempts are retained. V1 is rejected for a decimal-literal/ternary whitespace
syntax error, corrected thereafter. V2 and its terminal diagnostic exhausted Verilator's 8MiB stack
in scheduling; a 32MiB child stack resolves it with unchanged RTL. V3's Python
limit-setting attempt failed before compilation; the CLI now re-execs through
a process-local shell limit. V4 exposed fixture C++ naming/scalar-port errors.
V5's original/candidate comparison passed to the independent RIOT observer
failure; the observer had reversed CS/address pin order. V6 fixes that fixture
and passes the complete run. V7 rejects a mutation-target parser before running
cases; v8 selects exact parsed instances and passes all eleven bad controls.
Only the terminal diagnostic's 20,444,141,410 bytes of derived AST objects
were deleted; the exact executed inputs, log and object manifest are retained.

Next: integrate the board into an experiment with every board's state paths
enabled and check whole-core resource/timing limits. Then coordinate raw RAM,
wrapper/device phases, input, ARM disk cache/server connections and matching
disk generation in an atomic bundle, and prove actual FPGA Cedar restore.
