# Coupled original control clocks: circuit and isolated timing qualification

**Component qualification only. No native core, faster FPGA boot or hardware
speed gain is claimed.** Full FPGA snapshots remain deferred.

The current fixed-wave component includes every Q in fourteen original ContA
sequential packages: the 22 run/refresh Qs plus complete k03/k04/k05/l07/j01
packages. All 32 bits, including MemSH', SC&NoFreeze and the microcode-RAM clock
state, are compared with an original-wire-list extraction of 28 packages and
235 pin connections. Original combinational cells and independent event-driven
flip-flops supply the reference; phase/run feedback is internal to the graph.

Both Icarus and Verilator pass 69,864 ordered binary events, all nine clock
channels and the enable/decoded outputs. Directed checks measure 196 ordinary
two-clock intervals and 64 IM/TPC six-clock intervals, single-step stop/restart,
and 2,568 MemSH transitions during halt with refresh enabled. Twelve bad RTL
variants fail after compiling. External controls settle before a root-clock
transition. This does not establish analog setup, X/Z clock behavior, the
surrounding Hold/repeat circuit or processor T/task captures.

The first fixture could not seed arbitrary phase Qs while the prior Run/Stop
state still asserted sPhase0. The corrected fixture lets that asynchronous
control settle before loading phase Qs. Both provisional logs remain history.

The initial guarded evaluator passed the circuit checks but failed timing.
Removing intermediate wide stability gates preserves the clock/transfer
functions and leaves the final check. An SMT proof using arbitrary deterministic
functions establishes that a fixed point is absorbing, including edge flags;
two weakened criteria admit counterexamples. The rewrite checker verifies that
only the guard changes. This is a binary rewrite proof, not a circuit or timing
proof. Fresh archived-source simulation and SMT runs reproduce the results.

| Registered fixture | ALMs | Registers including shell | Worst setup | Worst hold | Result |
| --- | ---: | ---: | ---: | ---: | --- |
| Guarded evaluator, original shell | 743 | 104 | -19.867 ns | +0.154 ns | Rejected |
| Fixed waves, original shell | 82 | 91 | +7.068 ns | -0.329 ns | Rejected |
| Fixed waves, multicorner analysis | 82 | 91 | +7.068 ns | -0.329 ns | Rejected |
| Fixed waves, registered input staging | 82 | 137 | +5.678 ns | +0.190 ns | Pass, isolated component |

All fits target the same Cyclone V 5CSEBA6U23I7 and 15.000 ns clock. The final
fixture stages virtual inputs on the opposite edge before the launch register;
that staging path has a 7.5 ns budget and the evaluator retains its full 15 ns
launch-to-result budget. Virtual I/O remains 2 ns max/0 ns min. No timing
exceptions or clock-period relaxation are added. This shell adds no native
pipeline stage. The evaluator's minimum internal setup margin is +11.237 ns;
overall minimum pulse-width margin is +6.551 ns. No RAM, DSP or PLL is used.

The earlier targeted register filter missed output aliases. The corrected audit
requires nonempty launch/result collections. The independent reader checks all
four conditions, clock period, constraints, unconstrained paths, source identity
against simulation, and prepared inputs. Only Quartus's LAST_QUARTUS_VERSION
annotation may change the QSF. Seven damaged/rehashed evidence cases reject.
The four isolated projects are terminal; their recorded processes were absent
at collection. No assembler, RBF or programming step was run.

Current evidence is under `fixed-qualified/`, `fixed-replay/`, `fixed-point-proof/`,
`fixed-point-replay/`, `timing-staged/` and `current-review-tools/`. Earlier
qualified/replay and rejected timing receipts are retained in full. All raw
members are indexed in `archive-manifest.json.gz` and stored losslessly gzip
compressed; compiler objects, binaries and Quartus databases are omitted.

From a checkout, choose a fresh output directory:

```
make -C verilog control-clock-test CONTROL_CLOCK_OUT=build/control-clock-new
```

For archive replay, run Python on
`fixed-qualified/executed-inputs/tools/test_control_clock.py --output /absolute/fresh/path --mutations`.
Use the extracted `current-review-tools/tools/audit_control_clock_fit.py` on
`timing-staged` with `--qualified fixed-qualified` for timing review. The reader
and its seven evidence controls reproduce exactly from the extracted receipt.

Next: Hold/repeat and processor T/task captures with this coupled controller,
then native integration, all-device timing and original-firmware qualification.
BaseBoard phases and peripheral timebases, Cedar rollback, repeated boots and
disk/stress gates remain open. Native59 is the last hardware-qualified core.
