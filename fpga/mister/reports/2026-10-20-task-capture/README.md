# Task selection, processor capture and task-file address ordering

This is a conditional digital arrival contract and an isolated FPGA budget
measurement. No native integration, board-skew/setup qualification, faster
hardware boot or 60ns operating result is claimed. Native59 remains the last
hardware reference at 1,947,654.12 instruction issues/s. FPGA snapshots stay
deferred; speed and authentic Cedar rollback remain the active goal.

The 88 original Qs include the prior 64-Q controller/processor slice plus:
ContA h08/h09 BNT, f14/f15 Switch/SwitchUp/TPCBypass, j08/j09 CTask and ProcH
i12 / ProcL i14 CurrLast address registers. The independent graph extracts
115 packages / 1,102 connected pins. Next/SwitchUp/CTask feedback is live,
including the priority encoder and Ready/IsNext gating. Ready/IsNext storage,
MIR tasking/block decode, maintenance and memory-Hold producers stay external.
The prior four clock/transfer functions are checked unchanged except names.

A live task input during the phase-Q/SHCP cascade captured the incoming task
early. The candidate now preserves the task setup value from before the root
event, without adding a pipeline register. CurrLast uses buffered pp/DblClock;
LastNext selects on negative phase-Q. The original zero-delay graph could not
qualify this ordering: from frozen inputs, Icarus rejects the live-source
variant on task 1 while Verilator passes its complete 73,761-event run. The
explicit-delay graph rejects the defect in both simulators. A reproducible
zero-delay diagnostic and its raw logs/source/provenance are included.

Both simulators agree on all 88 Qs, 25 clock channels and task/hold flags for
73,761 events in EACH of three delay profiles. Gate/normal FF/SE-phase FF:
1/1/1ns, 0.5/1/0.5ns, 1/2/1ns. These delays are illustrative arrival experiments,
not characterized vendor delays. They deliberately remove dependence on delta
scheduling and do not prove physical setup/hold or permissible board skew.
Each run covers all 16 hardware wakeups, 706 held-task checks, 304 priority
yields, 61 split T captures, 675 held fetches, 289 resumed fetches, 1,043 Next
changes around capture and 5,931 task-file address changes. Fifteen compiled
bad variants reject. Full archive replay repeats all profiles, both simulators
and all bad variants.

Boundary observations allow 20ns settling. Tests cover normal/extended running
with NoDispatch low, Hold/StopTasks updates in the root's high window, and
non-overlapping CTask asynchronous set/reset. No behavior is established for
overlapping set/reset or arbitrary asynchronous producer timing. The separate
32/64-Q qualifications retain their stated scope. Complete startup/freeze/
producer timing, T-data latches, the actual T-file write window and remaining
board/memory paths are open. Historical diagnostic logs lack frozen executed
inputs; they are not accepted qualification receipts.

One terminal Quartus17.0.2 fit on misterubuntu targets 5CSEBA6U23I7, one15.000ns
clock, 2ns max / 0ns min virtual I/O, opposite-edge input staging, no timing
exceptions or register retiming, no assembly/deployment. Four operating
conditions pass: minimum setup +5.058ns, hold +0.151ns, pulse width +6.555ns;
internal evaluator setup at least +8.057ns. Resources: 309 ALMs / 470 registers
including the shell, no RAM/DSP/PLL. After constant propagation the targeted
collections cover 187 launch and 115 result registers. This measures only the
FPGA combinational budget; it does not characterize the original-circuit
arrival profiles. All five recorded processes were absent at collection.
Only Quartus's version annotation changed the prepared QSF. The independent
reader and nine damaged/rehashed evidence cases pass; its previous 32-Q and
64-Q profiles also pass.

Raw members are losslessly gzip-compressed and indexed by raw name, byte count
and SHA256 in archive-manifest.json.gz. Decompress to a fresh directory and
verify each member before replay. Objects, simulator binaries and Quartus
working databases are omitted.

From a checkout (fresh output):

```
make -C verilog task-capture-test TASK_CAPTURE_OUT=build/task-capture-new
```

After archive extraction, entry points are:

```
python3 qualified/executed-inputs/tools/test_task_capture.py --output /absolute/fresh/simulation --mutations
python3 review-tools/tools/audit_control_clock_fit.py timing --qualified qualified --output /absolute/review.json
python3 review-tools/tools/test_processor_capture_fit.py timing --qualified qualified --component task_capture --output /absolute/fresh/reader-controls
python3 entrypoints/probe_task_capture_zero_delay.py --qualified qualified --output /absolute/fresh/zero-delay-diagnostic
```

Next: T-data transparency, delayed Hold and write-window/per-task data checks;
then Ready/MIR/Hold producers, native integration and original-firmware gates.
Cedar rollback, repeated boots, cold persistence and two-hour stress remain.
