# Coupled control and processor capture qualification

Component only: no native core, hardware speedup or Cedar boot qualification.
The full FPGA snapshot remains deferred; Native59 remains the last hardware
reference at 1,947,654.12 instruction issues/s.

The candidate includes 64 original Qs: the previously qualified 32-Q ContA
controller and each processor half's phase pair, six T controls, Curr and Last.
Reference extraction uses 70 original packages / 642 connected pins, original
combinational cells and independent event-driven flip-flops. The BaseBoard
common-root fanout and four named StartCycle/clock-enable seams are audited.
The controller functions match the previous qualified source except names.

Both Icarus and Verilator pass 74,872 binary events and all twelve compiled bad
variants reject. Checks compare all 64 Qs, 22 clock channels and nine flags:
all 16 task pin patterns, 218 held-task checks, 195 priority-yield checks,
279 split T-capture events, 211 held fetch events and 197 resumed fetch events.
Full archived-source replay produces the same result in both simulators.

Hold(E26) and PRhold(E42) are distinct. Higher priority can yield during PRhold.
ProcH and ProcL use different T-control capture clocks. Clock1 uses positive
phase-Q through SE10212. The BaseBoard supplies parallel copies of one root.
Hold/SwitchUp change in the clocked high window; other boundary data meets
setup. Actual producers/arbitration, T data/ALU/writeback, MIR and analog skew
remain outside this slice. Unused gates in selected physical packages have
explicit bindings recorded in the manifest; this is not a whole-board proof.

Provisional logs record wrong Clock1 polarity, independently opposed clock
roots, low-window Hold/SwitchUp swaps, and fixture phase-initialization races.
They are historical diagnostics, without frozen executed-input provenance.
The qualified seed temporarily inhibits phase initialization while loading Qs.
Directed split-capture tests complete meaningful coverage before freezing inputs.

One isolated Quartus 17.0.2 build on misterubuntu targets 5CSEBA6U23I7 with a
15.000ns clock, 2ns maximum / 0ns minimum virtual-I/O delays, opposite-edge input
staging, no timing exceptions, and no assembly/programming. All four operating
conditions pass: setup +5.671ns, hold +0.151ns, pulse width +6.552ns. Internal
launch-to-result setup is at least +10.514ns. Resources: 174 ALMs / 290 registers
including the shell; no RAM/DSP/PLL. Of 109 result bits, the repeated-event and
unsettled flags (23 bits) are constant zero. Remaining register collections
cover 107 launch and 86 result registers. All five recorded job processes were
absent at collection. Only Quartus's version annotation changed the QSF.

The independent reader checks source identity against simulation, four corners,
clock/constraints, nonempty targeted paths and unconstrained paths. Nine damaged
or rehashed evidence cases reject. The earlier 32-Q timing profile also passes
with the updated reader. Archived timing review and controls reproduce.

Every raw member listed in archive-manifest.json.gz is stored losslessly gzip
compressed with its size and SHA256. Simulator objects/binaries and Quartus
working databases are omitted. Decompress each listed stored member to raw_name
under a fresh directory and verify size/hash before replay.

From a checkout, choose a fresh output:

```
make -C verilog processor-capture-test PROCESSOR_CAPTURE_OUT=build/processor-capture-new
```

Archive replay entry points (after extraction):

```
python3 qualified/executed-inputs/tools/test_processor_capture.py --output /absolute/fresh/simulation --mutations
python3 review-tools/tools/audit_control_clock_fit.py timing --qualified qualified --output /absolute/review.json
python3 review-tools/tools/test_processor_capture_fit.py timing --qualified qualified --output /absolute/fresh/reader-controls
```

Next: include real task-arbiter/Hold producers and T data/writeback, then native
integration, all-device timing and original-firmware boot/input. Cedar rollback,
three fresh boots, cold disk persistence and two-hour stress remain open.
