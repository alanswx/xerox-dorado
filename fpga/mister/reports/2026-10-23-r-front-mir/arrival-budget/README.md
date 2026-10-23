# Extended arrival checks and rejected combined timing fit

The combined786-Q/9216-RAM-bit candidate is unchanged from the prior ordering
experiment (`eb5a25a8b188c90963098d4b2372cad18aeb553a2f90b50483f81e815a06ba83`).
Six additional runs pass: Icarus and Verilator, each with the inherited PARC
minimum/maximum parity-arc controls and a9.9ns R-memory read profile. These are
correlated controls on selected arcs, not independent full-board skew bounds.
The PARC profiles enable the inherited g16 setup/hold assertion monitors.

Each run performs11507 events with all786 Qs,9216 memory bits,67 clock channels
and functional ports compared after every event. It combines actual MIR RBase/
StkP commands, randomized external producers, HIGH-root CP and LOW-root CP
transactions. Both engines and all three profiles agree:182 write-active
samples,4406 read-active samples,584 parity-port epoch changes; every clock
channel is exercised, all Ready bits change. This remains a bounded component
contract. Actual IM/IFU/Hold and other native arrivals are still required.

The first combined15ns Quartus fit is **REJECTED**:

| Check | Result |
| --- | --- |
| Worst setup | **-2.961ns** (slow100C); slow-40C is-2.601ns |
| Worst hold | +0.157ns |
| Worst minimum pulse width | +6.117ns |
| Resources |3330ALMs,2980registers, four256x9 MLAB banks |
| Memory |9216bits, asynchronous read addresses/outputs, no initialization |
| Fixture |1409launch bits,1170functional result/invariant bits,15ns clock |

Only source_pins/fa_n remain simulation observations outside the registered
budget, following the parent fixture. All architectural Qs, clock events,
functional memory ports, parity and unsettled/shared-state checks are retained.
No clock exception, period relaxation, assembly or load was introduced.

The original full path report identifies22 logic levels/17.633ns of data delay:
old ContA h09.Q2 (`BNT.2`, fixture q70/launch693), task-priority/clock resolution,
MIR FF mux, stack address, the real stk_h MLAB read, visible-R/parity, then
`parity_ok[1]` (result238). This is a functional endpoint. Moving parity to
unregistered observations would conceal the failed budget.

The source inspection also corrects the next integration target. StopTasks and
RWTPCorRWIM are internal Control-A producers, although still external pins65/1
of this selected component. StopTasks is a13.9, with its original tasking-state/
FF-decode fan-in; RWTPCorRWIM is d21.2, from `JCN.2'b` and e21's wired-OR
`Return'a`. Preserve the separate physical MIR JCN copies. Close those paths
and actual MIR/IM/Hold arrivals, derive the original capture-stage boundary,
then shorten the measured functional path before another fit. This receipt
does not establish that any one proposed edit will meet timing.

The job at `/home/alans/mister/Dorado_RFrontMIR_1_20261023` completed map, fit,
STA and all four conditions. All five job PIDs are absent; exact input readback
allows only Quartus's LAST_QUARTUS_VERSION annotation. No build database or
FPGA image is in this receipt. Top SHA256:
`0ee3ad83d661d041c387324bd13c33b01fbf62f7db8cdbcd198f64c4f27799b6`.

Nineteen malformed/rehashed evidence controls reject structurally, including
changed fixture/evaluator/Ready/front/Control-A/cone bytes and wrong RAM/clock/
report coverage. Negative-slack controls are omitted for an already rejected
baseline; accepting another negative number would prove nothing. The existing
Ready/R-front timing review reproduces +0.622ns setup/+0.171ns hold,2944ALMs/
2809registers/9216MLAB bits, and all20 existing reader controls still reject.

Reproduce the functional extension using `tools/test_r_front_mir.py` with
`--iterations 1024 --producer-random --front-directed --cp --low-cp` and
`--profile parc_parity_min`, `parc_parity_max` or `later_r_memory`; select either
engine. `fit_r_front_mir.py` validates the matrix and prepares the frozen budget
fixture. Quartus runs only on misterubuntu. `audit_control_clock_fit.py` reviews
the result; `inspect_r_front_mir_timing.py` binds the critical path to fields and
original board pins. `archive_r_front_mir_budget.py` preserves this evidence.
Verify the receipt with:

```
python3 tools/archive_r_front_ready_followup.py --verify \
  fpga/mister/reports/2026-10-23-r-front-mir/arrival-budget/receipt
```

The native paired firmware runs and their observation controls remain separate
and unchanged. The archived watcher is a running prefix, not a completed boot
or speed result. Qualified5c65/Native59 and disk volumes are preserved. The full
60ns/66.7MHz/native producer/firmware/persistence/fresh-boot/stress goal remains
open; this work has made no FPGA speed claim.
