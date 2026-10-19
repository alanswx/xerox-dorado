# Complete modeled MemC state qualification

The frozen board accounts for 23,718 mutable bits: 507 FF bits, five phase
bits, 21,504 bits in 21 asynchronous 256x4 RAMs, and 1,472 RAM/230 write-window
bits in 23 small register files. Simulation-only original register-file
diagnostics are excluded explicitly. There is no immutable PROM on this board.
The four-bit F100181 output-latch primitive is qualified separately.

Final original/restore simulation passes 682,448 host edges, 187,527 held
edges, 13,334 late-enable changes, 177,507 state requests, all modeled bits,
all valid/invalid addresses, independent package/row writes, transport padding,
destructive continuation/restore, immediate equality, first resume, recapture
and 20,000 subsequent cycles. All 44 original RAM writers fire through the
normal board inputs. Directed clock/decoder/MemBase bank sequences supplement
random inputs; earlier attempts correctly reject incomplete writer coverage.
Ten faulty RTL variants fail. Fresh XML classification and sixteen damaged
schema/source controls pass. Encoded Verilator double-underscore package
names are mapped explicitly for the source-pinned inventory.

Matched 50 MHz fits pass all four operating conditions, minimum +0.152 ns,
with no cuts or unconstrained paths. Both retain all 21,504 MLAB bits/672
cells and zero M10Ks. Resources are 2,294 -> 5,390 ALMs and 2,340 -> 3,357
registers, including shell, administration and otherwise-pruned state.
Eight damaged-fit controls reject. Both variants use negative-edge external
input staging followed by positive-edge launch/capture: the embedded MemC
paths retain 20 ns budgets with unchanged 2 ns maximum/0 ns minimum virtual
I/O and identical cell RTL/SDC. This is an isolated test shell.

The first positive-edge external input shell fails an original-only virtual
input hold path at slow/-40 C (-1.960 ns), and is rejected. All raw fit,
auditor and executed-source evidence is retained. No production constraint
was relaxed. The accepted shell adds the same input staging to both variants.

Full-core integration is separate (Native55). Global device/ARM barriers,
coherent disk/server bundles, complete FPGA snapshots and original 60 ns
clocks remain required. Async RAM scans can change outputs, so every consumer
must be held by the eventual global barrier. This report does not qualify
an enabled full-core resource budget or claim a speed gain. Text is compressed
with gzip without content changes; build databases are excluded.
