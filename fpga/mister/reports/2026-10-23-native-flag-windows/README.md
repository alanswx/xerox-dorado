# Native timing rejection and physical R flag windows

This receipt preserves the complete v5 native failure and the corrected
component implementation of the original l06/l07 flag latches. No image was
assembled or loaded; the qualified hardware core and measured speed are unchanged.

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

`manifest.json.gz` maps 6,689 members to 1,627 deduplicated compressed blobs;
lossless readback verifies 422,292,860 raw bytes, 15,860,049 compressed blob bytes.
`summary.json` records the flag gates; `capture-followup-summary.json` records
the terminal v6 and bounded pointer checks. `collection.json.gz` retains the
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
source comparison. The broader pointer matrix is recorded as a verified live
job, not a pass, in the follow-up receipt. The separately staged v7 remains an
experimental fit with assembly disabled. Native source/phase and firmware correctness, 60 ns cadence/timers,
faster SDRAM and the complete Cedar/Rollback/persistence/boot/stress goal remain.
