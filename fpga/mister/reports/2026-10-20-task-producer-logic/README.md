# Original CP and Ready/IsNext producers

The actual Control-A CP maintenance registers and Ready/IsNext circuits now
pass independent circuit comparison, including their coupled asynchronous
clear path. The coupled implementation also passes the isolated 15 ns budget.
These components are not instantiated in the native core. No FPGA, Main,
daemon, disk image or speed change is claimed.

## Circuit and coverage

`dorado_cp_control_step.sv` implements a01's active-low CP decoder and both
six-bit j02/i02 MC10176 packages. Capture occurs on the selected strobe's
positive-going release. Every physical Q is retained, including stages whose
outputs are unwired. Both address polarity and the package pin data permutation
are checked against the extracted wiring. Eleven CP-bus nets are audited to
the actual BaseBoard MC10124 output pins and both backplane endpoints.

`dorado_task_ready_step.sv` retains all four F10000 packages, the one unused
physical Q, IsNext's two decoders, both notify/preemption decoder pairs and
every Ready-feedback AND. Each SetReady wired-OR net has all three original
drivers. Pending readiness survives until the selected task consumes it;
notify or preemption on that same capture can keep it set. ClrReady is the
original asynchronous master reset. Freeze and Repeat retain their original
clock gating. Unrelated inputs on the selected d08 package remain explicit.

`dorado_task_producers_step.sv` connects i02.Q3 directly to Ready's clear.
The reference is one extracted 18-package / 201-pin original circuit with that
net internal. It is not two references connected by candidate-produced data.
The other task/phase/MIR inputs remain boundaries; the task arbiter is not yet
coupled to this component.

| Gate | Physical Qs | Events per profile | Rejected bad RTL variants |
| --- | --- | --- | --- |
| CP v2 | 12 | 4,113 | 7 |
| Ready v3 | 16 | 98,516 | 8 |
| Coupled producers v1 | 28 | 99,470 | 3 |

Every gate passes both Icarus and Verilator across three illustrative decode/
clock-to-Q delay profiles. Every physical bit changes. CP checks all eight
addresses and 128 data patterns, inactive-strobe data changes, and address
transitions with data already settled. Ready covers all 32,768 combinations
of BNT/current-task/notify operand and three decoder enables, all 15 Ready
and IsNext outputs, reset and independent Freeze/Repeat controls.

Coupled coverage: 32,798 Ready captures, 128 CP0 captures, 190 CP1 captures,
314 clear observations and 300 held observations. It checks all 28 state bits,
four clock/event channels and 30 Ready/IsNext outputs after every event.
The bad coupled variants reject a constant-clear stub, old clear epoch and
wrong CP bit. Data is settled before the exercised clock changes. These are
digital ordering profiles, not vendor delay/skew or whole-board 60 ns proof.

## Implementation budget

The registered Cyclone V fixture retains all state, functional outputs and
clock events: 98 launch bits / 66 registered result bits, original 15 ns
constraint, fully constrained virtual I/O and no timing exceptions. All four
operating conditions pass: setup **+5.640 ns**, hold **+0.155 ns**, minimum
pulse width **+6.555 ns**. Resources: 126 ALMs / 205 registers, no RAM/DSP/PLL.
No inferred latch warnings. The source review verifies all three RTL files
against the frozen qualification; prepared input readback allows only the
Quartus version annotation.

Nine damaged/rehashed evidence controls reject, including changes to each
child RTL dependency. The previous rejected and accepted task/address timing
reviews remain byte-identical, and its 17 damaged controls still reject.
All Quartus stages exited zero. Builder73298/map73299/fit73466/sta74104/
corners74168 were absent at collection. The local simulation handles also
completed; no job requires a restart.

## Raw receipts and replay

The root archive preserves 308 raw members: 2,168,175 bytes stored losslessly
as 756,688 bytes. It includes accepted and development attempts. CP v1 was
superseded by v2's settled-data address fixture. Ready v1 stopped during graph
generation because two unrelated d08 inputs needed explicit bindings. Ready
v2's balanced comparisons passed, but an ineffective mutation wrote a bit
subsequently overwritten by the candidate loop; v3 corrects it to unused
physical d09 bit12 and all eight controls reject. Neither failed development
gate is promoted to final qualification.

The separate `implementation/` archive preserves 1,147 raw members:
25,343,881 bytes stored as 2,577,891 bytes. It contains prepared/terminal fit
inputs, full four-condition reports, reader controls and portable review tools.
Databases, objects and executables are excluded from both archives. Manifest
readback and extraction verify every raw member; 13 / 15 / 24 accepted
CP/Ready/coupled frozen inputs were rechecked. The timing review reproduces
byte-identically using extracted tools and evidence. This does not claim a
full simulation rerun after extraction.

```
python3 fpga/mister/snapshot/archive_receipts.py --readback --output fpga/mister/reports/2026-10-20-task-producer-logic
python3 fpga/mister/snapshot/archive_receipts.py --readback --output fpga/mister/reports/2026-10-20-task-producer-logic/implementation
```

Extract each manifest member to `raw_name`, gunzipping when `stored` differs,
and verify length/SHA-256. Given the extracted implementation directory `$R`:

```
python3 "$R/review-tools/audit_control_clock_fit.py" "$R/task-producers-fit-v1-terminal-20261020" --qualified "$R/task-producers-v1-20261020" --output review.json
```

## Next integration boundary

Connect Ready/IsNext to the actual arbiter and close CP effects in the
controller, rather than using the earlier component's inactive CP constants.
MIR tasking/WTPC/GetTLink control, memory Hold and IFU arrivals remain open.
Preserve the pre-event source epochs and original three-driver SetReady nets.
Repeat full coupled state/arrival and timing checks before native integration,
then original-firmware Cedar/input/rollback and final hardware qualification.
The last hardware login measurement remains 1,947,686.01 instruction issues/s;
the original unstalled cadence target is 60 ns, or 16.67 million/s.
