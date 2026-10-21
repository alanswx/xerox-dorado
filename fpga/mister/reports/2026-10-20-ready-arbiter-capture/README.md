# Ready storage connected to task arbitration and processor captures

The 104-Q component connects actual Ready/IsNext to the original task pipeline
and processor captures. One extracted 130-package / 1,274-pin circuit supplies
the independent delayed reference. No fixture supplies Ready or IsNext to
arbitration. All three original SetReady drivers remain present.

## A physical distinction exposed by coupling

BNT and BNT' occupy separate h09/h08 MC10141 packages. Their normal loaded
contents are complementary, but that relation does not hold for arbitrary
power-up contents. Initial v1/v2 comparisons matched stored state but rejected
the IsNext outputs when those packages disagreed. The Ready module now accepts
independent negative BNT data when `INDEPENDENT_BNT=1`; its default interface
retains the earlier complementary-boundary contract.

The coupled evaluator uses the actual separately stored packages. Its Ready
capture consumes pre-event BNT, CTask and control data at the same logical
event as the original gated task clock. Both actual Ready clock fanouts and
all four F10000 packages are retained, including the unused physical bit.

## Functional qualification

Final v4 passes **77,923 events per profile**, both Icarus and Verilator,
across balanced, fast-gates and later-control delay profiles. Every 104 state
bit, 27 clock/event channels, 30 Ready/IsNext outputs, task Next and existing
processor flags are compared at each event. Every clock has directed coverage.

All 16 tasks run. Each of tasks 1..15 also runs through Notify -> Ready ->
priority with every direct wake wire zero. All 256 independent initial BNT /
BNT' pairs are checked before a normal load overwrites them. Both simulators'
coverage lines verify 15 Ready-only wakeups and 256 pairs in every profile.
The final summary includes 699 held / 311 yielding captures, 672 held / 292
resumed fetches, 1,179 coincident Next changes and 6,516 address changes.

Five damaged wiring variants reject: ignoring Ready, ignoring IsNext masking,
wrong capture clock, using newly captured task data and restoring the old
external Ready stub. The earlier v3 completed all profiles at 74,787 events;
v4 adds directed coverage with byte-identical RTL. The default Ready contract
still passes 98,516 events/profile in both simulators/three profiles and eight
bad variants. The CP/Ready coupled regression still passes 99,470 events with
three rejected bad variants.

These are conditional digital arrival checks using illustrative delays. They
do not establish vendor skew tolerance or whole-board 60 ns operation. Notify,
preemption, memory Hold and other live board-data producers remain boundaries.

## Timing and source identity

The isolated registered Cyclone V shell passes all four conditions at the
original 15 ns constraint: **setup +4.942 ns, hold +0.157 ns, minimum pulse
width +6.555 ns**. It uses 390 ALMs / 509 registers, no RAM/DSP/PLL. All 278
launch and 189 result interface bits remain in the fully constrained fixture;
there are no timing exceptions, dropped checks or added pipeline cycles.

The fit was prepared from completed v3. Every fitted RTL byte also matches
the stronger completed v4; independent timing review against either gate
reproduces the same result byte-for-byte. All prepared inputs read back
unchanged except the permitted Quartus version annotation. Nine damaged /
rehashed evidence controls reject, including both child RTL dependencies.
The earlier rejected and accepted task/address timing reviews remain
byte-identical with the extended reader.

All stages exited zero. Builder85780/map85783/fit86047/sta86750/corners86823
were absent at collection. Every local simulation/debug job is terminal.
An extra diagnostic Icarus run confirmed advancing simulation time; the longer
event runs were not restarted because of an observation timeout.

## Correcting the twelve-constant boundary inventory

The earlier inventory listed every non-rail Control-A package input. Those
twelve constants enter unrelated gate halves whose affected outputs are
unwired or have no selected consumer or observation. A pin-function dependency
proof checks all twelve in the old task, new Ready/task and full R-front
selections. It expands the actual Boolean expressions to physical input pins;
it does not infer dependencies from package names or simulation samples.

Four controls reject: routing CP into live memory-run logic, GetTLink into
stop control, Midas data into task switching, and exporting a formerly dead
output as an observation. The final proof freezes 26 input files and replays
from them. Initial proof v1 lacked two other boards' extraction dependencies;
v2 includes them and replays independently.

This proves the constants do not affect these selected components. It does
**not** prove the signals inactive in the complete machine. Original CP/MIR
logic outside the replacement slice must remain connected and functional.

## Raw receipt and next action

The manifest preserves 1,523 raw members totaling 25,832,209 bytes, stored
losslessly as 5,331,718 bytes: sources, generated references, logs, failed trials,
terminal reports and portable tools. Non-JSON text and JSON over 100 KiB use
lossless gzip; compiler databases, objects and binaries are excluded. Readback
and extraction verify every selected byte. Accepted frozen input counts are
54 (coupled v4), 15 (default Ready), 24 (CP/Ready) and 26 (dependency proof).
Extracted tools reproduce both timing reviews and the dependency proof. This
is not a full simulation rerun after extraction.

```
python3 fpga/mister/snapshot/archive_receipts.py --readback --output fpga/mister/reports/2026-10-20-ready-arbiter-capture
```

Extract manifest members to `raw_name`, gunzipping when `stored` differs, and
verify length/SHA-256. Given extracted directory `$R`:

```
python3 "$R/review-tools/audit_control_clock_fit.py" "$R/task-ready-capture-fit-v1-terminal-20261020" --qualified "$R/task-ready-capture-v4-20261020" --output timing.json
python3 "$R/task-control-stubs-v2-20261020/executed-inputs/tools/audit_task_control_stubs.py" --replay "$R/task-control-stubs-v2-20261020/executed-inputs" --output stubs.json
```

Next connect this Ready state to the full RBase/stack/RM/STK component while
preserving its original memory/source epochs, then qualify live Notify /
preemption / Hold / IFU and MIR arrivals against native wiring. The dead
package halves are not a prerequisite to that integration; their surrounding
original logic remains required. Native integration, hardware speedup, cold
persistence, Cedar rollback and final hardware gates remain open. No FPGA,
Main, daemon or volume changed. Last measured login rate remains
1,947,686.01 instruction issues/s; the unstalled target is 60 ns / 16.67 million/s.
