# RBase, stack pointer and bypass component

`verilog/rtl/dorado_r_front_step.sv` extends the coupled R-memory component
from 636 to 736 processor bits and from 45 to 57 observed clock channels.
The four functional RM/STK ports remain inside `dorado_r_memory_step`; every
one of their 9,216 data/parity bits is compared with the original circuit.
This is an isolated migration component. It has not been integrated into the
native core or qualified for a complete 60 ns board cycle.

The reference generator extracts 259 packages from the original wiring lists.
Consumed wired-OR nets retain all their drivers, including all nine outputs
driving `RmBypass'` and all eight driving `StkBypass'`. It also includes the
complete physical FA/FB/FC decoder required by these gates. The reference's
delayed storage models determine capture ordering independently of the candidate.
The candidate's generated combinational cone deliberately shares the reference's
gate wiring and canonical cell functions; this is not an independent validation
of those cell functions.

## Additional physical storage

| State bits | ProcL package | Function |
| --- | --- | --- |
| 639:636 | g14 | LOW-transparent RBase write-data latch |
| 703:640 | g15 | Original 16 by 4 per-task RBase file |
| 709:704 | h14 | RBase read latch, write qualifier and NextMacro delay |
| 715:710 | l14 | StkP.0 through StkP.5 |
| 717:716 | l15 | StkP.6 and StkP.7, in package order |
| 721:718 / 725:722 | i16 / i17 | LOW-transparent stack write-address latches |
| 729:726 / 733:730 | l16 / l18 | Saved stack pointer |
| 735:734 | j19 | Stack over/underflow capture gates |

The 64-bit RBase file uses explicit component state. The large RM/STK banks
retain their functional memory ports, without an extra memory copy or state
scan interface. Test fixtures initialize storage only in the comparison bench.
Native RBase implementation and full-core resource inference remain unqualified.

## Decode and capture contract

The new boundary receives raw FF and negative ASEL/BSEL signals, rather than
assuming raw FF's upper two bits always equal decoded FA. For ASEL 0 through 3,
the physical circuit decodes FA as zero regardless of those raw bits. When
FFok is inactive all FA outputs remain inactive. The child evaluator only
consumes FA0-specific functions, so its effective FF input preserves the raw
FB/FC fields and enables FA0 only when the real decoder does.

The two FFok decoder inputs share an audited boundary: ContA f24 is an MC10211,
and each wired-OR FFok net combines one replica from each of its two NOR gates.
The generator checks the actual package type and every driver pin. Likewise,
the positive and negative delayed-MD boundary signals represent complementary
outputs of one original pipeline flip-flop, rather than independent inputs.

RBase/stack capture data comes from the pre-event Q epoch. During LOW transparent windows,
g14 and the stack write-address latches resolve before the level-sensitive
RBase write. The saved-pointer F10000s use their actual ORed clock inputs:
Clock1'Db and StkPSaveEn'. The separate original storage model retains that
gating; it does not treat the save gate as a data enable on an unrelated edge.
The existing old-address/parity capture contract for g16 is retained.

Remaining producer boundaries include MIR, IFU, ALU writeback, counters, I/O,
Ready and Hold. Generous settled intervals and correlated timing profiles do
not establish independent skew tolerance or a 60 ns cycle.

## Task/address capture epoch and timing

Trial v1 retained task data at its first task-clock capture. All functional
gates pass, but it worsens the implementation budget: setup -3.645 ns, hold
+0.105 ns, pulse width +6.112 ns, 3,039 ALMs/2,772 registers/9,216 MLAB bits.
The selection mux retains priority feedback into task-address/T-parity logic.
Its terminal source/report receipt is kept as a rejected trial.

The accepted v2 uses the pre-event task and task-address data, matching the existing
processor-capture setup contract. The repeat flag, phase selection and Last/
Next address sources are sampled from that epoch. Live Qs still resolve clocks
and retain actual state. No extra architectural register, memory copy or pipeline
cycle is added. The 16-wave solver, repeat/unsettled checks, full interface and
15 ns clock constraint are unchanged.

The v2 88-Q/25-clock gate passes 73,761 events/profile in both simulators across
three arrival profiles, all 16 tasks and 15 rejected bad variants. Additional
reset/Freeze, maintenance, StopTasks and separate Hold checks pass 1,617 events/
profile in both simulators/three profiles. The v2 full R-front gate passes all six profiles in both simulators, with
identical behavior/clock/arrival/RAM/write-window summaries. The passing fit has
only task RTL and its qualification record changed from baseline;
all other prepared sources, constraints, helpers and widths are identical.
Its four-condition15ns fit now passes: setup +0.474 ns, hold +0.099 ns, minimum
pulse width +6.112 ns. Resources: 2,845 ALMs, 2,784 registers, four256x9 MLAB
banks (9,216 bits), no M10K/DSP/PLL. All 17 damaged reader controls reject.
Native integration/speed remain open.

## Verification and next action

The qualification command is:

```
python3 tools/test_r_front.py --mutations --output build/rtl-executive/r-front-task-epoch-v2-20261020
```

The current v2 run is TERMINAL and passes all six profiles in both Icarus and Verilator:
11,236 resolved events per profile, comparing all 736 processor and 9,216
RM/STK bits after every event. All 57 clock channels are exercised; all 100
added storage bits change. Directed tests fill every RBase slot with both
nibble extremes and check all 256 expected StkP loads. Balanced observes 266
RBase write windows across all 16 addresses with zero address changes, plus
155 RM/STK windows per half with zero address or select changes. There are
620 write observations, 3,362 reads and 810 old/new parity-address handoffs.

The raw decoder/adapters pass 4,352 checks in both simulators, including all
4,096 ASEL/FF/disable cases. Seven damaged variants reject. All 87 frozen
executed primary inputs match current v2 source. PARC correlated min/max profiles enforce
g16 setup/hold limits of 1.7/1.1 ns and observe 702/2 ns, respectively. These
generous fixture intervals do not prove the complete 60 ns timing contract.
The other illustrative profiles can observe 1 ns hold and are not vendor
qualification. Shared combinational logic and remaining producer boundaries
still limit the claim.

`tools/fit_r_front.py` prepares the unchanged isolated 15 ns fixture only from
a completed, source-matching qualification. It cannot assemble or deploy an
image. Its synthesis representation uses explicit generate syntax and ordinary
ports in place of reference-style hierarchical package observations. The reader
reconstructs and checks that exact representation from the frozen qualified cone;
the original cone is preserved beside it. Both simulators pass the 4,352 original
checks on the lowered form and a separate 16,384-sample, 1,085-output comparison
against the qualified cone, including arbitrary Qs, all pins and both roots.

The first three attempts are terminal map failures (implicit generate, inline
genvar, then hierarchical port observations). Their sources, errors and absent
process checks are retained. The observation-inclusive v5 fit completes all four
conditions but is REJECTED: setup -3.041 ns, hold +0.164 ns, pulse width +6.115 ns;
3,117 ALMs, 3,060 registers and exactly 9,216 MLAB bits. Its 19-level critical
path computes the next task and then re-reads the RBase file for the exported
`source_pins[156]` observation, captured at `result[164]` in that fixture.

`source_pins` and `fa_n` are test observations of the already computed next Qs;
neither feeds state, a RAM port or an invariant. The functional fixture retains
every state bit, event/repeat bit, write/read/control/address/data/parity port and
unsettled check. It uses the same 15 ns constraints and all 1,211 launch bits,
with 971 registered result bits. It leaves only those 210 observation bits as
unregistered wires. They remain checked by full-state simulation and the
representation comparison. `--capture-observers` reproduces the older 1,181-bit
experiment. The reader keeps the two fixture profiles distinct and reproduces
the rejected v5 review byte-identically.

The baseline functional-interface v6 job is TERMINAL and REJECTED for 15 ns: setup
-2.617 ns, hold +0.156 ns, minimum pulse width +6.117 ns. Four operating conditions,
source/constraint coverage and all four uninitialized, unregistered-read 256 by 9
MLAB banks pass review. Resources are 2,979 ALMs, 2,785 registers, 9,216 MLAB bits
and zero M10K/DSP/PLL. Its real critical path has 18 logic levels from task
request/Ready through the priority/current-task feedback to `unsettled`
(result120), with 17.003 ns data delay at the slow -40C condition.

The first-epoch v1 implementation remains TERMINAL/REJECTED as described above.
V2 is TERMINAL/PASS at `misterubuntu:/home/alans/mister/task-epoch-fit-v2-20261020`.
The worst slow-40 path now has 13 logic levels/13.828 ns data delay, replacing
the baseline 18-level task-feedback path. Its five processes
(56691,56698,57170,59590,59879) are absent. All simulation, fit and reader work is
terminal. The original 15 ns constraint, complete state/functional ports and
convergence/repeat checks are retained. Remaining CP/MIR/Ready/IsNext/Hold/IFU
producer closure, native integration, Cedar speed, original rollback and final
hardware gates remain open.

The accepted task/address data epoch and rejected first-capture trial are
preserved in the [portable raw receipt](../fpga/mister/reports/2026-10-20-task-capture-epoch/README.md).
Both timing reviews replay byte-identically from extracted sources/reports.

Actual CP/Ready producer components now pass separately and coupled, including
all three SetReady drivers and i02.Q3 asynchronous clear. Their28-Q component
passes four-condition15ns timing, setup+5.640ns/hold+0.155ns. See the
[producer receipt](../fpga/mister/reports/2026-10-20-task-producer-logic/README.md).
They are not yet coupled to this736-Q front's arbiter/control graph. Its actual
CP/MIR/phase/Hold/IFU producer timing remains unqualified; do not treat these
separate fits as a complete native timing proof.

Ready/IsNext now couples to the task/processor capture graph at104 Qs, passing
both simulators/three profiles and isolated15ns timing (+4.942ns/+0.157ns).
The original BNT and BNT' packages retain independent initial state. The next
step is coupling this Ready storage to the full R-front/RAM component.
The pin-function dependency proof corrects the earlier12-constant Control-A
inventory: those inputs reach only unused/unobserved gate halves of the selected
component. They do not block this integration; original surrounding CP/MIR
logic remains required. Live Notify/preemption/Hold/IFU/MIR arrivals remain open.
[Raw proof, cases and replay](../fpga/mister/reports/2026-10-20-ready-arbiter-capture/README.md).

The full Ready/R-front/RM/STK combination now passes at752 processor Qs and
9216 RAM bits in both simulators/six profiles (13173 events), including all59
clocks,15 Notify-only wakeups and original write-window/source-epoch checks.
Its15ns four-condition fit passes (+0.622ns/+0.171ns), retaining four256x9 MLAB
banks. Native integration is next at the unchanged baseline clock first; live
producer arrivals, all-device timing and original firmware remain unqualified.
[Full raw receipt and native inventory](../fpga/mister/reports/2026-10-20-r-front-ready-path/README.md).
