# Actual MOS RIOT complete modeled state

**Simulation, fresh XML classification and matched50MHz routed fits PASS.**
The actual BaseBoard uses the vendor M6532 with init_7800=0. This adapter
retains128x8RAM and73control/history bits: both ports/directions, registered
read output, interrupt enables/flags, timer, prescaler, increment mode,
rollover, edge polarity and the static block-local PA7 history. All1097bits
are exported in133nonaliased words. Indices133..255 error without writing.
The enclosing cell_MCS6532 phase_d is separate, required wrapper state.
No full BaseBoard or global FPGA snapshot is claimed.

The original ce-gated read, reset priority (including ce=0), RAM, timer and
interrupt ordering are retained. Only old_pa7's static declaration is hoisted
for host access; no initialization/reset is added. Tests compare the untouched
original's complete RAM/control/history vector and outputs, an independent
C++ RAM/port/timer/interrupt model and an independent bit/word access oracle.

Accepted v2:54,701host/4,448held/4,109accepted/2,593writes, all1097bits as
isolated set bits, all256indices, all128RAMwords, allfour prescalers,27modeled
underflows/2,113PA7events, ce/reset/read/write/select transitions, late changing
pins with advance asserted during freeze, dense saved RAM, candidate-only
destructive continuation, complete restore, first-edge equivalence and
20,000additional steps. Nine broken RTL variants fail, plus two changed-source
guards. Fresh actual-binding XML classifies all state and rejects ten damaged
inventories/schemas, including missing RAM/read-output/PA7 history.

The first fixture is REJECTED: it left advance=0 through frozen scans, so
ignore_freeze survived. The corrected fixture exercises held advance and late
inputs, randomized entry/hold/read requests and dense saved state, and counts
actual modeled timer/PA7 decisions rather than flag rises caused by scan writes.
The adapter itself did not change. Initial partial test outputs and generated
RTL/build logs are retained; the failed run did not emit a final source-hash
manifest, so only the accepted v2 has complete executed test-source provenance.
The first classifier also rejected the standalone XML's @e procedural scope
label; the final classifier recognizes the two observed @e/@fg forms while
requiring the exact pinned source, field name, width, category and unique path.

Matched50MHz fits are terminal. All four conditions pass with minimum
**+0.176ns**, no illegal/unconstrained clocks/I/O or extra timing exceptions.
Eight damaged-fit/schema controls reject. Enabled synthesis retains
**1118registers=1097state+18response+3handshake**; baseline normal-state pruning
leaves1090. Fitter shell totals660ALM/1206registers ->1645ALM/1350registers;
no block RAM. These include launch/capture/admin/physical duplication and are
not an enabled whole-core budget. All compiled source hashes (apart from the
Quartus-managed QSF revision line) were checked after archive extraction;
complete normalized QSF assignments are independently checked by the auditor.
Raw source/log/XML/report text and JSON above100KiB are losslessly gzip stored.

Next: enclosing phase-history/state routing and full BaseBoard integration,
remaining BaseBoard RAM/EPROM read pipelines/clock-reset state, then global
wrapper/backend/input/ARM/server/disk-generation barriers and actual FPGA save/
restore. Original clocks/timers and speed are unchanged by this experiment.
