# Native timing and capture-order qualification

This receipt preserves the complete v5 native failure and the corrected
component implementation of the original l06/l07 flag latches. No image was
assembled or loaded; the qualified hardware core and measured speed are unchanged.

## Guarded CP-prefix cache

`cp-prefix-cache-summary.json` retains the implemented canonical/last-settled
guard, first-step fallback and tagged269-bit prefix pipeline. The567-bit key
contains every prefix input; the root's11CP pins are also checked. Only exact
canonical unchanged prefixes bypass. Reset invalidates eligibility and cache.
The original four SRAMs remain the sole storage owners and receive commit
permission only when `advance && cp_ready`.

Both final v2 engines pass14,884checks/238writes/6,928reads/392epochs, retaining
all844physical Qs/9216RAM bits, every original fatal and all100clock counts.
Fixture coverage13,347direct/1,537staged,1,553wait edges includes7changed-input
retries and1reset invalidation. This is NOT native CP frequency or speed.
Both engines reject stale-tag, corrupted-prefix and unguarded-write controls
at check0; corrupted prefix fails an original source assertion, and unguarded
write fails observation of the actual SRAM owner's enable. Earlier passing v1
fixtures lacked that explicit owner-enable check and remain as history.

Direct Verilog-derived four-step proof compares475control/event/flag outputs
plus write permission for958arbitrary inputs per step, first-step reset only.
Each of the three broken variants has an independent SAT counterexample.
The stronger proof adds pure ghost functions observing saved payload/key and
canonical identity. The actual scheduler is inverse-byte-identical after
removing those observations. Base and induction length1 prove the combined
invariant and ready-result/write-exclusion property, with arbitrary input/reset
histories and exact RTL auxiliary power-up values. This is unbounded digital
component safety, not original analog timing or native all-board integration.
The initial frontend failure on ANSI output initializers remains: the next
proof lowers exactly three constant-zero initializers to identical initial
statements, with an inverse byte check. All sources/scripts/logs are retained.

The full15ns component fit finishes map/fit/STA/four-corner reports but is
**REJECTED**. All original1650launch/1400result indices and the SDC remain;
reset launch1650/readiness result1400 are added and timed, without exceptions.

| Condition | Global setup | Owner setup | Late Hold -> owners | Hold |
|---|---:|---:|---:|---:|
| Slow100C | -5.381ns | -4.875ns | +0.848ns | +0.378ns |
| Slow-40C | -5.619ns | -5.036ns | +1.022ns | +0.338ns |
| Fast100C | +3.774ns | +4.252ns | +7.170ns | +0.179ns |
| Fast-40C | +4.353ns | +5.978ns | +8.461ns | +0.157ns |

All five UCP classes are zero.5442ALMs include1527virtual-pin ALMs;
6068registers, no M10Ks. The global worst sink is canonical_valid feedback;
the owner path goes from launch608=before-pin205 through the wide fast_path
comparison, root CP selection/tasking/ContB and R flags to result1148=physical
Q592. The guard replaced the old prefix path but now adds latency ahead of
the root. Next compute direct/staged control results in parallel and select
afterward, and derive eligibility from control-settled state while retaining
the full diagnostic. Re-prove and time the actual result before native use.
The fitted DB remains on misterubuntu at
`/home/alans/mister/cp-prefix-cache-fit-v1-20261023`.

All four native tests were verified live at unchanged5B bounds; source/log
observations are retained, not a completed boot claim. Main/daemon/hardware
and last4.025473M StartCycle issues/s are unchanged. Global board/backend
pauses, actual CP frequency/arrival/pulse contracts, original60ns phases/all
timers, physical66.7MHz SDRAM and the full Cedar/Rollback/persistence/three-
boot/input/two-hour stress acceptance remain. Full FPGA snapshots are deferred.

## Phase-preserving Hold and direct RTL proof

`phase-hold-formal-summary.json` records the new alternative to duplication:
LOW control/memory clocks cannot rise to re-capture Hold, so MIR and task-clock
evaluation can retain initial Hold, with DisHold clearing it AFTER the first
transfer. HIGH masks the substituted operand. Other clocks/data, observable
Hold, original before-clock evaluation, all owners and the sole four SRAM
banks remain unchanged. There is no added register or guest cycle.

Four source-bound recurrence/mask properties prove with Z3; four wrong masks
have counterexamples. The first malformed SMT parenthesis and two source-
generation/compile failures are retained. The stronger proof compiles the
complete old/new Verilog atomic functions with project-local YoWASP Yosys:
**955 arbitrary binary state/input bits, all363 output bits**, including156
Qs,56events,56repeated events,94flags and unsettled. No input/state assumptions,
unknown-cell skipping, X matching or timing exceptions are used. Tool/package
versions, exact HDL, proof scripts and complete logs are retained. This proves
a digital rewrite relative to the existing evaluator; it does not resolve
the rejected original CP-arrival profile or qualify native timing.

Eight settling passes prove equivalent to24. Four are REJECTED: a real
counterexample differs in unsettled even though Q/events/flags agree. Both
simulators replay the exact363-bit result. The wrong-Hold formal control
differs in six event bits, also replayed by both simulators. This illustrates
why state equality alone is insufficient. Independently, with the NEW
diagnostic assertions removed and the original bench BYTE-IDENTICAL, both
simulators reject inverted Hold at check16: physical settled Q changes at
commit. Neither the reference nor any original fatal check was altered.

Both final MIR+task/eight-pass original-graph comparisons pass
**14,884checks/238writes/6,928reads/392epochs**, all844 physical Qs/9216RAM bits
and all100 clock counts.476,288 per-wave clock comparisons pass per engine;
63 HIGH epochs exercise different substitute/actual Hold values. LOW
DisHold-clear fixture coverage remains ZERO; the direct RTL proof covers
arbitrary binary DisHold. Earlier MIR-only24/eight-pass results, failures and
all original timed inputs are retained.

The full1400-result15ns component fit is TERMINAL and **REJECTED**:

| Condition | Global setup | Owner setup | Late Hold -> owners | Hold |
|---|---:|---:|---:|---:|
| Slow100C | -7.495ns | -7.325ns | +2.154ns | +0.377ns |
| Slow-40C | -7.699ns | -7.567ns | +2.128ns | +0.325ns |
| Fast100C | +3.724ns | +3.778ns | +8.257ns | +0.193ns |
| Fast-40C | +4.244ns | +5.271ns | +9.203ns | +0.158ns |

All map/fit/STA/four-condition queries finish with no build/report repair.
All five reported UCP classes are zero.4169ALMs include1526virtual-pin ALMs;
3511registers, no M10Ks. The fitted DB is retained on misterubuntu; source/
reports are archived here without build databases. No result or constraint
was removed. Global worst launch293 maps to after-pin291 (CP bank select),
through CP-prefix/root/tasking/ContB; worst owner result1146 is physical Q590.
Late Hold now meets timing, but the remaining CP composition does not.
This improves the rejected duplicated implementation, not hardware speed.

The next CP-prefix bypass condition is also formally checked. A settled
transaction's exact saved after-pins/root form a canonical unchanged next
transaction: same state/flags and zero events/repeated/unsettled. Perturbing
the saved CP-strobe before-pin yields a counterexample. The source-bound
`cp-identity-native-seam-summary.json` confirms staged unfitted v9 saves
`r_front_before<=r_front_live` and the prior root with the same owner commit
enable. Canonical/last-settled guards, first-step fallback, a conditional CP
pipeline and all-board/backend pause contracts are NOT implemented/qualified.
Measure actual CP events before assuming a rare path. The next design must
bypass only under the proved contract and stage required CP events with one
SRAM commit; it must retain FOUR15ns ticks/60ns for ordinary instructions.
The earlier unconditional120ns pipeline remains rejected.

All four original/corrected-input native processes were verified live again
at the same5B bounds. Corrected runs have passed memory initialization and
entered original BaseBoard code, with no Cedar113 release/window yet. Hardware,
last measured4.025473M StartCycle issues/s and Main/daemon are unchanged. The
complete native timing/clock/timer/Cedar/Rollback/persistence/input/stress goal
remains active; complete FPGA snapshots remain deferred.

## Late Hold selection and stopped-clock input reset

`late-hold-selection-summary.json` retains the two stateless alternatives for
the actual MemC15.sil+2 late Hold pin, followed by same-cycle selection. BOTH
full original comparisons pass **14,884 checks /238 writes /6,928 reads /392
epochs** with all844 physical Qs,9216 RAM bits,69 original fatal checks and100
clock counts retained. Forecasts instantiate no RAM or sequential storage;
the original evaluator remains the sole four-bank SRAM owner. Inverting the
actual full-state selection fails the independent original check15/Q840 in
both engines. There are2709 distinct full-state checks, exclusively at Q840;
the eight closed R flags have ZERO distinct alternative coverage. These are
bounded transfer tests, not proof of flag influence, native timing or speed.

The isolated15ns fit is **rejected**. Sources and the entire1400-result shell
and SDC are retained. Initial Quartus17 genvar parsing fails; the next attempt
uses the already-proved separate-genvar spelling and finishes synthesis, fit
and STA. The first corner query misses top-level register names; subsequent
escaped-name and in-netlist escaping-mode queries also fail. All are retained.
The final literal-name query completes on the SAME fitted database, covering
844 owner registers and the real late input at every condition:

| Condition | Global setup | Owner setup | Late Hold -> owners | Hold |
|---|---:|---:|---:|---:|
| Slow100C | -17.177ns | -16.283ns | +7.047ns | +0.420ns |
| Slow-40C | -17.162ns | -15.997ns | +7.345ns | +0.396ns |
| Fast100C | -2.332ns | -1.665ns | +9.920ns | +0.200ns |
| Fast-40C | +0.295ns | +0.891ns | +10.837ns | +0.178ns |

The worst global path ends at result355, an `unsettled` diagnostic. The worst
functional owner path ends at result1150, physical Q594; both start at
launch686, before-controls bit283. Thus removing diagnostics would not qualify
this candidate. The early control CP-prefix/root composition remains too long.
All five reported unconstrained path classes are zero. The fit uses9093 ALMs
(1526 containing virtual pins),3748 registers and no M10Ks; the source has one
physical R-file owner. Synthesis retains1193 result registers after optimizing
constants; no source result field was removed. No exception or refit was used
to obtain the final reports.

`late-hold-timing-input-reset-paths-summary.json` is the authoritative timing/
input receipt. Its metadata-only correction anchors detail rows: the first
receipt's parser matched the summary column labels for source/sink names.
Raw reports, slacks, source hashes and the rejection remain identical; the
original receipt is retained with the explicit correction link/hash.

The actual native boot-input failure is independently observed by read-only
debugger inspection of process29618: raw PS/2 and delivered event are621,
queue count0, but ALL FOUR keyboard words are0000. Active-low0000 means every
key is held. The original gated machine clock has no edges while reset is
asserted. Asynchronous reset alone also fails the real native cold test,
because reset begins high at time zero without a rising edge in Verilator.
Both failed asynchronous-only native tests stop immediately at their new
matrix assertion, preserving their exact sources/logs.

The keyboard and terminal utility adapters now declare power-up state matching
reset and accept asynchronous reset with a stopped source clock. Terminal
reset/configuration sends81ffff80 directly, avoiding a same-event keyboard
nonblocking-assignment dependency. Pure pending-word selection replaces an
automatic block temporary rejected by Icarus, retaining the same priority.
The original boards/ROM, source clocks, memory, key queue and boot stimulus
remain unchanged. Final tests in BOTH simulators cover cold reset low/high,
no machine edges, queued C, exact83fbff80 serial delivery and stopped warm
reset. Eight functional cases pass and eight controls reject: missing
power-up values and each/both asynchronous resets removed. Existing queue and
keyboard/terminal tests pass in both. The earlier unsupported-Icarus syntax
failure and less-complete passing tests remain archived.

The final source is recorded in `input-reset-synthesis-summary.json` and
`keyboard-stopped-reset-v5-20261023`. Actual Quartus synthesis rejected the
intermediate reset implementation's unconditional mouse_taken assignment.
The final implementation keeps strobe history clocked separately and assigns
mouse_taken's constant in both reset/normal branches. It passes the same16
dual-engine cases and isolated Cyclone V synthesis with0 errors/9 visible
warnings. This does not qualify placement/routing, reset recovery/removal,
full-core timing or a native boot. The failed synthesis source and report
remain archived. The matching input-reset-v2 simulations were deliberately
stopped with SIGTERM for this known source rejection, with their bounds,
prefixes and termination reasons preserved. No observation timeout caused a
restart; the new v3 tests bind both final simulator and synthesis receipts.

Original native processes29201/29618 are verified live at their unchanged5B
bounds; both have requested Alto110. Fresh source-changed control/queue
processes58600/58753 pass the real cold assertion at host6/guest0 and remain
live, with original5B bounds, EPROMs, clocks, memory model, C/button stimulus,
SkipDiskWait jumper and read-only host pack. Their frozen inputs and process
observations are retained as observations, not a Cedar boot pass. No guest
state was planted and no old process restarted. Actual113/C release and Cedar
storage attribution are the next gates. Hardware/speed remain unchanged;
complete60ns/physical-SDRAM/Rollback/persistence/input/stress acceptance remains.

## Registered control cut and cadence budget

Both original-reference comparisons pass the experimental 475-bit registered
control/operand boundary: **14,884 checks / 238 writes / 6,928 reads / 392
epochs** per engine. All 844 physical Qs and 9,216 RAM bits are compared. All
69 original fatal checks remain verbatim; all 100 clock counts match the
parent runs. Candidate bytes match between engines, and only the candidate
factorization and bench stage exercise change. Original timed cells and RAM
references are byte-identical to their parents.

The first host edge records the control result while SRAM advance is disabled.
The second commits the same four SRAM banks once, retaining old owner state
and inputs across both stages. Physical views after commit are recomputed
from the new owners; a transaction's saved event descriptor is not reused.
This proves a bounded component transaction, not native consumer scheduling.
The original fast-gates CP miter remains rejected and unchanged.

**The nonoverlapped native schedule is rejected.** An ordinary original
instruction spans two 30 ns source-clock periods and four distributed-root
transitions. At 66.7 MHz, 60 ns allows four host ticks. Two ticks for each
root transition would need eight ticks, or **120 ns**. Passing an isolated
15 ns fit would not repair that cadence, so no Quartus job was started and
the unrun fit scaffold was removed. This rejects that specific schedule,
not a phase-specific split or an overlapped pipeline with proved forwarding.

The audit uses the frozen native `cell_CLOCKGEN` source and reproduces its
N=16 digital phase table. Its inactive-debug root table does not qualify the
N=2 phase quantization, real pulse widths, Run/Step or extended instructions.
The next implementation must fit existing capture edges, or prove overlap and
all board consumers while retaining the original instruction/phase count.
Precomputing late Hold/MIR alternatives with a final selection is only a
possible combinational experiment; it must share the original SRAM banks
and preserve every event/capture invariant before any native fit.

`control-cut-budget-summary.json` retains both terminal comparisons, their
exact transforms, the cadence rejection and immutable observations of the
same live native jobs. No native run was restarted or extended, no state was
planted and no core was loaded. Full A-F goal remains active; hardware and
measured speed are unchanged.

## Exact-state CP timing replay

The saved pre-event snapshots from Icarus and Verilator are byte-identical:
844 physical state bits,9,216 functional RAM bits and before/after input pins.
The short replay uses the unchanged full timed original graph and its original
functional RAM. Before **each** of11 trials per engine, every state and RAM bit
must equal that snapshot. This makes the capture reproducible without rerunning
the14,884-event history. It seeds a component fixture, not a native machine.

Both engines agree at all **eight** offsets meeting the MC10176's documented
setup/hold requirements. CP offsets-16/-8/-4/-2.5ns give g18 Q0;
+1.5/+4/+8/+16ns give Q1. Offsets-1/0/+1ns violate the2.5ns setup/1.5ns hold
aperture. Only zero skew disagrees: IcarusQ1/VerilatorQ0, reproducing the
original profile failure. The invalid-margin trials remain explicit diagnostics;
they do not qualify a candidate or erase the original failed full comparison.
`cp-flag-replay-summary.json` retains all results and initialization attempts.

The initial time-zero load failed exact restoration in both engines. A real
load edge restored Icarus, while Verilator still showed bits23/25/29 under the
old phase-init reset during CP loading. The final procedure loads controls,
waits for their asynchronous resets to settle, then loads all Qs again. The
original reset priority and every original cell stay unchanged; exact full-state
and RAM comparisons remain mandatory before every trial. This is no FPGA
snapshot implementation or native CPU-state plant.

The added clock7 observer leaves all g18 outcomes unchanged. Both engines
measure LOW widths16.5/8.5/4.5/3.0/1.5/0.5ns at offsets
-16/-8/-4/-2.5/-1/0ns; positive offsets give no clock7 edge. Consequently the
ideal clock7 edge at zero skew follows only a **0.5ns LOW pulse**. Physical
capture of that pulse and the relevant minimum-width contract remain open;
toggle-frequency ratings alone do not establish a minimum pulse specification.
`cp-clock7-width-summary.json` preserves both complete waveform replays.

This establishes original-g18 timing behavior, not candidate equivalence,
other sink timing, native source setup, a fitted image or a speed gain. The
original profile rejection is retained. Next audit actual native CP/root phases
and all relevant capture/pulse contracts before native timing/integration.
Both original native runs continue at their original bounds; the full A-F goal
remains active and complete FPGA snapshots remain deferred.

## CP-before-root subevents

**Latest: the profile matrix is terminal and rejected.** Fast-gates Icarus
fails event12839 at physical Q88: original ProcH g18 captures1 for FF076|Freeze
while the candidate predicts0. All clock comparisons passed before that state
rejection. The paired Verilator fast-gates case passes with byte-identical
candidate, bench and timed RTL. Capture setup/arrival is unresolved; this is
not permission to mask Q88, alter the original FF or choose a simulator.
`cp-subevents-matrix-summary.json` retains the complete terminal matrix,
source equality, rejection and recorded process absence. The earlier primary
summary's live7/12 observation remains historical evidence.

Both passive pin-arrival diagnostics are now terminal. At8988202.500ns D falls
and C rises at exactly the same modeled timestamp. Icarus's clock process sees
D1 and retainsQ1; Verilator seesD0 and updatesQ0 at8988203.500ns. Neither
original outcome changed when observed. The original Motorola1978 data book,
PDF page129/printed3-82, specifies **2.5ns minimum setup and1.5ns minimum hold**
for MC10176/MC10576; the extracted page was visually checked. Thus this raw
stimulus has zero margin and cannot establish a uniquely reliable physical
result. `cp-flag-arrival-summary.json` preserves both complete diagnostics,
pre-event state/RAM, the source-page PDF/image/text and their hashes.

The original matrix rejection remains. No bit, reference or fatal is changed.
Next replay the saved state with CP timing offsets, measure source/capture
margins and establish the native producer contract before qualifying the model.

`cp-subevents-summary.json` preserves two complete balanced passes and the
completed Verilator timing profiles, earlier rejected compositions, negative
controls and immutable observations of actual live Icarus/native handles.

The corrected v3 composition passes **14,884 events in both simulators**:
238 writes,6,928 reads,392 address epochs. The extra transactions exercise both
CP pulse/root alignments across the four CP banks in this component. All original
844-Q,9,216-RAM-bit,100-clock,package-pin,physical-output and settled-state
rejection checks remain unchanged. Eight address-package pin audits cover
28,700 captures per engine; the old/new address data views differ at zero tested
captures, so exhaustive address-epoch qualification remains open.

The control evaluator first sees local CP pins at the old root, then the
distributed root and remaining inputs. Each intermediate Q is a wire; no
storage, RAM copy, new clock or guest instruction cycle is added. The actual
task/controller evaluator must do the same: clearing NoDispatch while the root
is LOW creates a LOW interval at clock7 before the root rises. Canonical f14
events carry their prefix/root ownership through a four-bit wire descriptor.
Every atomic transfer function and the physical RAM port implementation stay
unchanged. Native timing of this composition has not been assessed.

The first joint-only version gets beyond11327 but rejects12839 in both engines.
The second also updates a processor helper, but that helper does not supply the
failing clock count; it still rejects12839 in Verilator. Both failed sources and
logs are retained. Starting from v3, disabling joint ordering restores11327;
disabling task ordering restores12839 in BOTH engines. All four negative
controls are now terminal; each Icarus candidate matches its Verilator partner
byte-for-byte. `cp-subevents-negatives-summary.json` adds the terminal Icarus
trees and exact rejection checks, preserving the primary receipt's earlier
live observations.

The profile matrix ends with **7 passes,1 failure and4 unrun Icarus profiles**.
All six Verilator profiles and balanced Icarus pass. Profile cells are copied exactly
from the independently qualified original timing profiles. The expanded bench
and all checks are retained. All recorded matrix processes are absent; do not
restart that terminal matrix. Its probe/driver freeze is released.
Do not assemble, select a native fit or claim a corrected full machine from
these bounded component results. Source arrival contracts, complete profile
qualification, native integration and physical timing remain required.

New passive ProcH g18 C/D/Freeze/Q arrival diagnostics keep the same candidate,
original timed graph and every fatal, printing pre-event state and RAM for
bounded replay. The Icarus v1 and Verilator v2 diagnostics are terminal;
no prior job was restarted. Verilator v1 stopped at compilation because one
9216-bit display exceeded its8192-bit argument limit; v2 splits that diagnostic
output only. Observation itself may perturb same-delta ordering, so these are
diagnostics and cannot qualify or erase the original failing profile.

The SAME queued original-ROM native run29618 now executes FinishCP at
host371308340/guest145150301 and host371316957/guest145155634, PCF8AB/data00,
instruction mask1 on both observed transactions. This independently progresses
past the watchdog interval; it does not qualify all CP transitions, Cedar113 or
representative storage traffic. Both original native jobs retain their exact
sources, normal inputs, original5B bounds and in-memory-only medium writes.
Hardware and measured speed are unchanged; the full A-F goal remains active.

## Original-ROM watchdog startup

`baseboard-watchdog-summary.json` and the lossless archive preserve eight
terminal isolated-board diagnostics, exact native sources/EPROMs and passive
read-only observations of the existing full-machine runs. No RTL, original
timer, native bound, guest state, disk image or hardware was changed.

The 256-byte zero-page clear passes at SYSPER4 and16, with precisely256 stores
and256 DEX instructions. Irregular host-domain clock pauses also pass. The first
JSR/RTS through MidasSetup returns at349348 enabled ticks. Extending beyond the
old eight-byte boot gate exposes a reset at STX$0601 (Watchdog DDR), PCF3EE:
PA6 changes low while WatchdogIn and g22A Q' are high. The actual XOR/NAND and
j08 reset FF therefore restart the CPU. g21's original Q21 divider keeps
counting MCPreClk falling edges through these CPU resets.

Full original-divider startup observes **396 resets**, followed by the first
g22 phase at **139,809,848 enabled ticks**, g21 counter **1,048,576**. The ROM
then reaches its genuine FinishCP INC/DEC sequence at **145,117,901 ticks**,
PC **F8AB**, raw bank_n110/data000001000. Every clear in the observed startup
still has exactly256 stores/DEX operations. No counter seed or shortened timer
is used. This is the deterministic startup of this model; it does not establish
power-up behavior for every original physical board or calibrate all timers.

The shorter startup experiment's second-reset fatal is retained. The first
extended observer also rejected an early endpoint: programming MCP DDR creates
a physical strobe atPCF3E9 before the watchdog boundary, which is not execution
of FinishCP. v2 observes that DDR strobe after the boundary; the final test
additionally requires PCF8A5..F8AB, with unchanged physical models and160M bound.
These are different observation endpoints, not fixes to the firmware or gates.

LLDB read-only inspection uses field offsets compiled from the exact running
model header and stops at an evaluation entry, then detaches/resumes. The
queued native run shows80,112,008 enabled ticks, watchdog counter600,841 and
g22A=B=0, consistent with the initial phase. Early decoded input/IR node aliases
are explicitly unusable: some corresponding netlist outputs are tied off.
Only actual package/owner fields and meaningful PC/X outputs inform this result.
These snapshots are not settled-state comparisons or wall-time benchmarks.

Both full-machine tests29201/29618 were verified live again and their immutable
prefix observations are archived. Keep their original5B limits and exact inputs;
the isolated board's idle other-board inputs do not qualify native CP/root
ordering, Initial, Cedar or representative128-group firmware storage traffic.
The CP timing failure below remains open; v9 remains unfitted. Hardware and
measured4.025473M StartCycle issues/s are unchanged. Full goal ACTIVE.

## Latest: CP timing contract remains open

The complete v7/v8 fits are rejected. v7 setup is
-5.081/-4.818/+3.989/+4.748 ns; v8 is -4.638/-4.142/+4.101/+4.824 ns.
Both have minimum hold +0.116 ns and pulse +1.122 ns. All nine stages and the
central/RAM/visible-R query are terminal; each 953-member collection reads back
exactly with every recorded process exited and no image. The actual worst
moves from MemC c15.qb/Aad.5b to k11 task-bank capture (22 levels, 21.886 ns)
to MemC b15.qb/Aad.7a to closed flag storage global592 (23 levels, 21.525 ns).
Neither is selected. Native host external UCP remains visible.

Both RM and shared-address profile matrices are now terminal **12/12**.
The shared data view covers all 36 edge-triggered address bits, retaining
actual clocks, h15 transparency, physical owners, RAM ports and the complete
original oracle. Independent package D-at-clock checks pass 23,580 captures
in eight groups per simulator. There are zero distinct old/new epoch samples;
inverted bank data rejects at original output check15. v9 source staging and
structural elaboration pass, but no remote project/build was started.

Additional CP/root-coincidence tests reject in both simulators at event11327;
the prior RM-only version rejects at the same event. At the existing fatal,
a diagnostic observes the independent delayed original's shared state as
`000008f0607ce3bf`, equal to the child and different from the joint result
`000008f0617ce3bf` at Q24 (k04.A, SC&NoFreeze). The diagnostic still fails;
it is not a qualifying gate. Physical-state, clock and solver checks remain.

The first clock-only patch incorrectly changes the BEFORE view and rejects6735.
The second limits the change to AFTER and passes that first boundary, then
rejects12839: original clock7 has one edge while the candidate predicts zero.
Both patches are failed experiments. Ordered CP/root subevents and the real
producer setup contract need review before fitting another candidate.

Separate original-ROM native runs use the period SkipDiskWait' jumper:
MCS6532 PB1 is grounded, matching DoradoReset.masm DiskWait523..528. Every
EPROM byte, guest/storage clock, CPU/memory model, medium and three-press input
is retained, along with the original 5-billion-host-tick bound. The 2-second
clock-settle delay remains. A passive observer records real 6502 PC and CP
NoDispatch/input-clock timing. Both compile and were verified live during
SDRAM initialization. They are pending experiments; the original runs remain
terminal and neither original limit was extended. The host opens the pack
read-only and keeps simulated writes in memory.

`clock-order-followup-summary.json` records these terminal proofs, failed
patches and immutable snapshots of the live native observations. Continue
from the actual jobs, not from a RUNNING file alone. No hardware change or
speed qualification follows. The full A-F objective remains active.

## Complete native v5 result

All nine build/report stages and the separate central/RAM/visible-R query
finished successfully. Collection checks every compiled source hash and verifies
that the recorded processes have exited. Setup still fails:

| Condition | Setup slack, ns |
|---|---:|
| Slow, 100 C | -6.235 |
| Slow, -40 C | -5.868 |
| Fast, 100 C | +3.960 |
| Fast, -40 C | +4.663 |

Minimum hold is +0.109 ns; minimum pulse width is +1.122 ns. This is a regression
from v2's -4.630 ns setup, at the same 57.142857 MHz and SYSPER=4. The actual
worst path starts at MemC a15.qa (**Aad.4a**), passes the a12 cache tag RAM and
compare, pin386, control/MIR and pointer next state, then the RbBypass comparison
and ProcL l07A flag. It has 25 logic levels and 23.085 ns data delay.

The central audit covers 84,061 keepers, 775 central registers (688 primary,
87 duplicates), 61 history registers, all 16 original visible-R owners and all
four SRAM groups. Keep the host external unconstrained paths visible; this
native design does not inherit the isolated fixture's zero-UCP claim.

## STK read-address inference

Read-only fitted-register queries resolve the OUTCLOCK inference: the 16 STK
read-address bits capture **in parallel** with eight existing address-owner
FFs, with exactly the same physical data, clock and enable drivers. v2 has 17
physical copies including a duplicate; v5 has 16. No additional pipeline stage
is present in these fits. This does not establish initialization, collision
semantics, physical timing or boot correctness.

The initial v5 reader assumed the FF data pin was always `asdata` and rejected
a valid `d` packing. Its failure and reader are retained. The repaired reader
accepts either sole data input and compares the actual driver, clock and enable.
It reuses the terminal query without refitting or restarting it. The old
RUNNING receipt is explicitly superseded by `review-repair.json`.

## Corrected R flag windows

The eight original MC1668 bits use closed storage with a live HIGH-transparent
Q view, following the original package truth table. Set applies while closed;
HIGH clock follows D. Reset remains unwired LOW. The current implementation
stores a newly asserted closed Set in the same owner commit, preserving the
original fixed-point check as well as every physical-state comparison.

- Both independent unit runs pass 327,680 comparisons across 65,536 transitions
  and all 128 old/new input-state combinations per bit. Four damaged variants
  fail in each simulator, including omission of the newly asserted Set.
- Full integration passes in Icarus and Verilator, including all 844 physical
  state bits, 9,216 RAM bits, 100 clocks and 11,300 events. New assertions check
  the eight actual clock, data and Set pin views.
- Six original timing profiles in both simulators pass: **12/12**. The parent
  six-pin stack-address matrix is also terminal **12/12**.
- Omitting closing flag capture rejects at check15; hiding the live flag output
  rejects at check14, in both simulators.

Initial compile and integration failures are retained. Deferring Set storage
passed the standalone physical-Q comparison but failed the full owner's settled
state check at event14. The corrected implementation adds Set-after handling
and unit idempotence assertions; no original rejection check was removed.

Private native v6 stages this tested implementation with the same state/RAM,
16 visible-R owners, pin history, advance/storage barrier, clock and constraints.
Structural elaboration passes, retaining three UNOPTFLAT warnings and the
inherited media MULTIDRIVEN warning. The full-core fit is now terminal and still rejected: setup
-3.878/-3.817/+3.675/+4.477 ns; minimum hold +0.114 ns, pulse +1.122 ns.
It uses 35,593 ALMs and 54,668 registers. The 950 collected members match
compiled source hashes and all recorded processes exited. The worst path is
now MemC b15.qa/Aad.6a through b12 cache tags/compare, pin386, Hold/MIR,
after_child212, the RBase file and the low RM address capture at ProcL h10B
(global605, upper_q17): 20 levels, 20.717 ns data delay. All 796 central
keepers (688 primary/108 copies), 61 history registers, 16 original R owners
and four SRAM groups are covered. The 3 input/50 output host UCP remains.
This is improved timing, without native selection, firmware or speed qualification.

## Evidence and remaining work

`manifest.json.gz` maps30,443 members to3,394 deduplicated compressed blobs;
lossless readback verifies904,386,697 raw bytes,35,177,730 compressed blob bytes.
`summary.json` records the flag gates; `capture-followup-summary.json` records
the terminal v6 and bounded pointer checks. `address-followup-summary.json`
adds the terminal pointer matrix, bounded RM capture and original native
traffic observations. `collection.json.gz` retains the
remote v5 collection envelope and member hashes. Sources, reports, original
reference models, test benches and failed receipts are included; FPGA images,
simulator executables and Quartus databases are excluded.

Next: review the remaining RBase/address capture crossing and the separate v7
fit. The subsequent
h14/l14 capture-data experiment must keep its coverage limits explicit:
h14 owns RBase/task/NextMacroDly; l14 owns StkP[0:5]. The balanced actual-pin audit
agrees at the package clocks but has zero samples where old-control and resolved
capture data differ. A passing comparison does not prove that unexercised
distinction. A further Verilator run passes 12,840 full comparisons, including
512 LOW-root CP transactions, but adds no package edges or distinct capture
epochs. Four wrong-pointer controls reject at check15 in the original resolved
source comparison. The original capture follow-up records the pointer matrix while live; the
later address follow-up records its terminal **12/12** pass. The separately
staged v7 remains an experimental fit with assembly disabled.

The four-input low RM address experiment keeps the existing before-event RBase
mux data only at the g10/h10 capture. Both full comparisons pass; wrong-data
controls reject at check1434. Actual package D-at-clock checks pass 4,393 edges
per simulator, with zero distinct old/new data samples. v8 source staging and
structural elaboration pass, retaining the clock/storage/barrier and all
warnings. Its full-core fit and broader timing-profile matrix are still live;
neither is qualified here. The first matrix driver stopped on a metadata key
before starting any child; its failed receipt and original driver are retained.

The original 5c65 and queue native runs both ended normally at their unchanged
5-billion-host-tick limits. Neither reached the Cedar113 release; both end
with one counted StartCycle and BFN zero. The first 8,191 matching raw
completions take 109,062 versus 102,908 host ticks (5.64266% fewer), including
unowned time that also contains demand gaps. This is power-up-prefix transport
evidence, not native firmware throughput or Cedar/hardware speed. No original
bound was extended and no run was restarted. Native source/phase and firmware correctness, 60 ns cadence/timers,
faster SDRAM and the complete Cedar/Rollback/persistence/boot/stress goal remain.
