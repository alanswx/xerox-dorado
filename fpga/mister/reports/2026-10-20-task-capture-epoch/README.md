# Task/address capture epoch: isolated 15 ns timing passes

The task and task-address data now use the pre-event setup epoch already
required by the coupled processor component. Live Qs still resolve derived
clocks and retain state. This removes serialized same-event task feedback
without adding an architectural register or pipeline cycle. The 16-wave solver,
repeat/unsettled checks, all state and functional ports remain intact.

This is component qualification. No FPGA, Main, daemon or guest volume was
changed; native integration, whole-board 60 ns timing and faster Cedar remain
unqualified. Complete FPGA snapshots remain deferred.

## Functional evidence

The `task-epoch-qualified-v2-20261020/` gate compares all 88 task/processor Qs
and 25 clock channels after 73,761 events per profile, in both Icarus and
Verilator across three arrival profiles. All 16 tasks, Hold/repeat, fetch
held/resumed and coincident Next/address captures pass. All 15 deliberately
damaged variants reject in balanced Icarus; two mutation anchors follow the
changed address expressions without reducing the cases.

The additional `task-epoch-control-v2-20261020/` gate compares the same state
and clock events through eight rounds of reset/Freeze, CurrentTask maintenance,
StopTasks and separate processor/memory Hold transitions: 1,617 events/profile,
both simulators across three profiles. Producer changes settle with root HIGH;
simultaneous asynchronous CurrentTask set/reset is excluded.

The full `r-front-task-epoch-v2-20261020/` gate compares every 736 processor
and 9,216 RAM bit after 11,236 events/profile in both simulators across six
profiles. All 57 clocks and 100 added front bits are exercised, all 16 RBase
slots and 256 expected StkP loads checked. Original write windows have zero
address/select changes: 266 RBase windows and 155 RM/STK windows per half.
There are 620 writes, 3,362 reads and 810 old/new parity-source handoffs.
All behavior, clock, arrival, RAM and window summaries match the baseline.
Seven damaged front variants reject; both simulators pass 4,352 raw decoder/
adapter cases. Shared combinational logic is not independently validated here;
the delayed storage/capture oracle is independent.

## Identical fixture, corrected data epoch

| Fit | Setup | Hold | Pulse width | ALMs / registers |
| --- | --- | --- | --- | --- |
| Rejected v1 first-task-clock snapshot | -3.645 ns | +0.105 ns | +6.112 ns | 3,039 / 2,772 |
| Accepted v2 pre-event task/address | +0.474 ns | +0.099 ns | +6.112 ns | 2,845 / 2,784 |

Both fits complete four operating conditions at the original 15 ns constraint,
with fully constrained I/O and no timing exceptions. Four 256 by 9 MLAB banks
retain unregistered read addresses/outputs, no initialization and the qualified
DONT_CARE collision/nonoverlap contract. No M10K, DSP or PLL is used.

The input comparison proves only task RTL and functional qualification JSON
changed from the baseline functional fit: top, constraints, other RTL,
raw/normalized cone, helpers and 1,211 launch / 971 registered result widths
are identical. All state, events, RAM ports and unsettled checks remain
registered; only the same 210 post-state test observation bits are unregistered.
The prior cone-lowering miter remains applicable by exact byte identity.

The representative slow -40C worst path falls from the baseline 18 levels /
17.003 ns task feedback to 13 levels / 13.828 ns. The rejected v1 selection mux
had retained a 22-level feedback path. Its passing functional simulations and
failed implementation are preserved separately; do not deploy it.

All 17 damaged/rehashed timing-evidence controls reject, including generated
cone changes and wrong RAM parameters. Prepared/collected input readback
matches except the permitted Quartus version annotation.

## Raw archive and replay

3,001 raw members totaling 115,131,905 bytes are stored losslessly as
11,077,499 bytes. Non-JSON text and JSON over 100 KiB use gzip. Compiler
databases, objects and executables are excluded. The manifest records every
raw name, length and SHA-256. Readback and extraction verified all members;
the 45 / 47 / 87 accepted task/control/front frozen inputs were rechecked.
Both the rejected v1 and accepted v2 timing reviews reproduce byte-identically
using extracted tools/data. This is not a full simulation rerun after extraction.
Supplemental replay/readback JSON and this README are outside the raw manifest.

```
python3 fpga/mister/snapshot/archive_receipts.py --readback --output fpga/mister/reports/2026-10-20-task-capture-epoch
```

Extract each manifest member to `raw_name`, gunzipping when `stored` differs,
and verify its recorded length/hash. Given extracted directory `$R`:

```
python3 "$R/review-tools/audit_control_clock_fit.py" "$R/task-epoch-fit-v1-terminal-20261020" --qualified "$R/r-front-task-epoch-v1-20261020" --output rejected.json
python3 "$R/review-tools/audit_control_clock_fit.py" "$R/task-epoch-fit-v2-terminal-20261020" --qualified "$R/r-front-task-epoch-v2-20261020" --output accepted.json
```

## Process state and remaining work

All jobs are terminal. Rejected v1 PIDs 40338/40342/40956/43502/43779 and
accepted v2 PIDs 56691/56698/57170/59590/59879 are absent in the final job
state proof. No fit needs restarting.

MiSTer14 remains Main3486/server3485/RBF15730f44. The 00:13:18 UTC October 21
read-only sample records good3346/dropped33/CRC0/RX0/abort0. Last measured login
issue rate remains 1,947,686.01/s. This sample is not the two-hour combined
stress gate and no speed gain is claimed.

The boundary inventory identifies 12 retained Control-A CP/MIR/control
constants; processor stubs from the task-only fixture are already explicit
inputs or connected logic in the full front. Close actual CP/MIR/TaskingOff/
GetTLink/WTPC and Ready/IsNext/Hold/IFU producers before native integration.
An inventory is not arrival qualification. Cold persistence, FPGA Cedar
Checkpoint/Rollback, three fresh boots and final combined stress remain open.
