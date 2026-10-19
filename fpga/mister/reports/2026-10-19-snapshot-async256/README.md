# Async256 RAM state access: original/oracle/restore and MLAB timing PASS

Isolated MB7071H256x4 and F10414256x1 adapters preserve all1280 modeled RAM
bits under the actual guest advance. Production cells/core are unchanged.
These primitives are used by processor register/stack/parity files and other
boards; this report does not classify or restore an entire board/machine.

The generator pins the entire original primitive hashes. Both were independently
verified byte-identical to the actual frozen Native49 source tree. It keeps the exact
pin order, active-low selects, synchronous level-write and asynchronous read.
One shared address/data write port retains MLAB inference. During a held
freeze, state scans borrow the existing asynchronous read address: guest
outputs CAN change. Every consumer/frontend must be held by a future global
barrier. Release restores the combinational guest read before a guest edge;
no guest warm-up edge is inserted. Running/entry/release requests are ignored.
Indices0..255 export/import raw RAM,256..511 error without mutation. Reads
respond after one host edge; writes return zero, avoiding a collision-data
promise. No reset or initial RAM contents are invented.

Final simulation:34864host edges/7019ordinary held edges/3584requests,
all1280bits and256invalid addresses, independent select/write enables, late
actual enables, destructive candidate-only continuation against stopped
originals, complete restore/immediate outputs/first guest edge and20000more
random cycles. Both untouched originals and separate arrays check memory,
outputs and ordinary writes (6583MB7071H/6269F10414). Fourteen altered RTL
variants fail for enable/freeze/access/address/write faults; four altered
original sources reject. Fresh original XML classifies all candidates as
saved RAM or reviewed combinational/constant wires. Twelve damaged coverage/
geometry/duplicate/layout cases reject. No fitted FPGA state-offset ABI used.

First fit is REJECTED and preserved: separate conditional normal/restore
write addresses defeated MLAB inference (warning10999 for both families),
expanding212ALMs/47registers into1598/1371. This was caught despite passing
original/oracle simulation. The revised fit shares write address/data and
reruns simulation and timing; no rejected RTL is promoted.

Qualified project `/home/alans/mister/SnapshotAsync256_v2_20261019`;
inputs `SnapshotAsync256_inputs_v2_20261019`. Fit62969 TERMINAL, all map/fit/
STA/corner jobs sequential. Matched50MHz/opposite-edge launch/capture shell,
10ns embedded path budget, virtual I/O2ns max/0ns min. All12 applicable
setup/hold/pulse-width checks per variant pass over four conditions; minimum
snapshot slack+0.354ns. Recovery/removal have no paths. No exceptions or
unconstrained clocks/I/O. Actual synthesis AND fitter tables retain exactly
two MLAB memories256x4/256x1,1280bits/40MLAB cells, zeroM10Ks. Resources
212->227ALMs and47->64registers include shell/admin pruning; not a whole-core
snapshot budget. Synthesis warnings reduce; fitter warning counts unchanged.
Six corrupted source/constraint/QSF/memory-inference/physical-placement/hold
controls reject. Exact source hashes tie final simulation to fitted inputs.

Raw sources/reports/logs are gzip-compressed without altering their bytes.
The initial audit import failure and successful self-contained audit are
preserved; no fitted RTL/constraints changed during audit. Earlier simulation
logs preserve the old two-write design and fault-diagnostic refinements.
No FPGA programming, full board/global freeze, complete Cedar snapshot,
restore-time benefit, original60ns migration or speed improvement is claimed.

Reproduce: `test_async256.py --output NEW --mutations` and
`classify_async256.py --output NEW --negative-controls` on the Mac;
`fit_async256.py --prepare NEW` then `--run NEW` on misterubuntu;
`audit_async256.py NEW --output qualification.json --negative-controls`.
Next integrate these families with the remaining processor board state,
classify that complete scope and qualify real board continuation/timing.
