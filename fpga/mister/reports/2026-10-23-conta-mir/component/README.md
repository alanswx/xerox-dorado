# Coupled Control-A clock, task and MIR component

`dorado_conta_mir_step.sv` couples 98 original physical Qs: 32 control/phase,
16 task selection, 16 Ready, 12 CP and 22 MIR. Actual CP NoDispatch/rStop/
StopAtT1/rCT/rMIR/ClrReady drive the selected clock, task and MIR consumers.
The actual clock/task feedback, six MIR fanout nets and Ready wakeup/masking
are internal. Both controller transfer functions are byte-identical to the
prior qualified task evaluator; their source and hash comparison are retained.
No reset, initial value, copied memory or architectural pipeline stage is added.

The reference is one original Control-A wire-list graph with independent
event-driven storage and the existing cell pin functions. Every one of the98
physical Qs, 22 clock channels, 39 flags and four Next bits is compared after
every boundary event. The selected graph is not the entire Control-A board.
StopTasks, RWTPC/IFU, memory Hold and other unselected producers remain explicit.

Primary v2 passes both simulators across three illustrative delay profiles:
15,729 events/profile, 518 CP captures, 1,050 MIR captures, 19 task changes and
all16 current tasks seen. Real MIR Notify reaches each task with no external
Ready/Notify stimulus. A lower current task yields to task15 during memory
Hold. Independent BNT/BNT' startup pairs and NoDispatch maintenance reset pass.
Seven actual bad RTL variants reject in BOTH engines: external NoDispatch
clock control, old CP MIR/clear epochs, merged BNT, missing Notify/StopMIR,
and missing NoDispatch task reset.

LOW-root CP extension v3 passes both engines/all profiles: 17,010 events,
774 CP captures, 1,050 MIR captures and20 task changes. The additional bus
transactions settle separately from root transitions. V4 adds stopped arbitrary
physical storage, including the otherwise unobserved MIR half, WriteIM clocks
and manual cadence: 19,044 events, 1,370 MIR captures,199 ordinary two-clock and
65 extended six-clock instruction intervals, plus26 WriteIM-phase observations.
Its six positives reuse the unchanged DUT bytes; already qualified damaged
variants are not rerun in that additive extension. These are clock counts,
not a measured60ns FPGA result or independent analog skew characterization.

Initial v1's smaller workload passes; it did not yet have the directed yield/
maintenance/independent-BNT cases or rejection controls. The standalone first
reference inventory prototype used a MemSH observation where the clock channel
requires l06.ca. That tap was corrected BEFORE any component test; both the
unqualified prototype and every actual executed reference are preserved.

Fifteen extra fixture inputs touch only unconsumed halves of selected packages.
Exact pin-function dependency checks pass; two cell changes that feed live
preemption/Notify reject. The shared helper still reproduces the previous
eight-boundary audit and controls exactly. These producers remain required in
the original full machine; nothing was tied off or removed from native RTL.

One isolated Quartus17.0.2 fit on misterubuntu passes all four operating
conditions at15.000ns: minimum setup+4.649ns, hold+0.178ns, pulse width+6.550ns;
launch-to-result setup at least+4.759ns. Resources including the registered
measurement shell:458ALMs/561registers, no RAM/DSP/PLL. The shell wires356
nominal launch and186 nominal result bits, including all event/repeat/unsettled
checks. Synthesis can merge/prune duplicates. Input staging is fixture-only.
I/O retains2ns max/0ns min; no timing exceptions or clock relaxation. All five
build PIDs are absent; only Quartus's version annotation changed. Eight damaged/
rehashed reader cases reject, including a zeroed state result. The previous
MIR/CP/Ready timing review remains unchanged. No assembly or native load.

Next: compose this producer/control slice with the processor/RBase/stack/RM/STK
path, close the remaining real producer arrivals and qualify original-ROM startup
before a native replacement or frequency change. Complete native66.7MHz/CDC/
physical IO, original60ns cadence/timers and final fresh-boot/stress gates remain.
This receipt does not establish a hardware or Cedar speed improvement.

The native watcher appendix records the SAME tests2284/2331 and watcher7144.
Both are live and initialized but have no CPU/113 window yet. Original
DoradoContinuous.masm lines190-216 waits15 nominal100ms ticks after the
last release before dispatching a multi-button boot. This is a source-derived
minimum quiet interval, not proof that a sampled live PC is in that routine or
an attribution of the entire elapsed boot time. Continue the existing runs;
never restart them on an observation timeout. Hardware stays5c65/Main13244/
daemon13243; cold native files/input PASS, HDMI OPEN, speed unchanged. Goal active.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-23-conta-mir/component`.
