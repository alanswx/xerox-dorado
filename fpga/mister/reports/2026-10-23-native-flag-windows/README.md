# Native timing and capture-order qualification

This receipt preserves the complete v5 native failure and the corrected
component implementation of the original l06/l07 flag latches. No image was
assembled or loaded; the qualified hardware core and measured speed are unchanged.

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

`manifest.json.gz` maps 16,018 members to 2,634 deduplicated compressed blobs;
lossless readback verifies 849,017,908 raw bytes, 30,978,436 compressed blob bytes.
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
