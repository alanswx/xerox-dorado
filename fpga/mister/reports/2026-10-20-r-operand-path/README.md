# R operand staging and MC1668 latch correction

A real cell-model bug was found: **MC1668 is a HIGH-transparent latch**, not an
edge-triggered FF. Motorola's original 1978 data book, PDF 235 / printed 4-30,
gives C=1: follow D regardless of S/R; C=0: S/R or hold. PARC's dictionary
independently marks high-transparent data arcs. `cell_MC1668.v` now follows this
truth table at each fabric sample. Closed S=R=1 is undefined and excluded;
its deterministic set priority is retained. No new reset/pipeline is added.
The original single-page sheet is now in `DoradoDocs/datasheets/MC1668.pdf`.
Both simulators pass 1,425 table observations; four bad variants reject.

Original usage is twelve packages: ProcH/ProcL l06+l07 and eight MemC address
latches a14/a15/b14/b15/c14/c15/d14/d15. This is a production source correction,
not a new deployed core. **Original-firmware/cache/processor regression with
this corrected cell is required before native deployment.** Native59's archived
inputs and hardware reference remain unchanged.

`dorado_r_operand_step.sv` adds j01/j02 bypass-data latches, g05/h05 file/IFU
read latches, g06/g07 visible-R latches and l07 local sign/odd/bypass state to
the unchanged T/task/parity slice. Its independent original graph covers
592 bits / 168 packages / 1,732 connected pins. Delayed bypass follows its
original open latch window and is held when closed. R data and delayed masks
now feed the parity/fault path internally. All original latch stages remain.
RM/STK read data and stored parity, RbBypass/RisIFdata and inherited producers
remain functional boundaries; RAM storage/address/write timing is unqualified.
The l07 local Qs are not the complete architectural sign/odd flags: l06 adds
two more wired-OR contributions per half and remains outside this slice.

Both simulators pass 23,672 events per profile across five profiles, comparing
all 592 bits, 29 clocks, read/write/visible gates and parity/fault outputs.
Each run observes 1,444 R-read updates, 510 visible-R changes, 8,068 Md / 1,308 Pd
open-latch observations, 8,248 file / 1,128 IFU observations, 37,968 closed checks
and six directed source/mask scenarios. Eleven bad variants reject, including
edge-only l07 sign/bypass and use of the current bypass instead of its latched
mask. One scoped old-bypass-data equivalence passes under the asserted
non-overlapping read/visible windows; it does not authorize removing stages.

The inherited three illustrative profiles and two targeted PARC parity-arc
controls pass. New R latches use illustrative delays; full-board analog/skew
and native 60ns operation remain unqualified. One isolated 15ns Quartus fit passes
four conditions: setup +4.450ns, hold +0.159ns, pulse width +6.548ns; evaluator
setup at least +4.500ns. The virtual-I/O shell uses 1,308 ALMs / 2,178 registers,
no RAM/DSP/PLL; targeted collections cover 784 launch / 647 result registers.
No timing exceptions, assembly or deployment. All five recorded processes
were absent at collection. Reader checks match all four RTL dependencies to
executed inputs; ten damaged evidence controls reject and five prior profiles
still pass. Archived readback and extracted reader checks reproduce.

Receipts: `fpga/mister/reports/2026-10-20-r-operand-path/` and
`fpga/mister/reports/2026-10-20-mc1668-latch/`. Frozen tests execute from their
archived inputs; a second full simulation replay after extraction is not claimed.
No Main/daemon change or hardware speedup: last measured rate remains
1,947,654.12 instruction issues/s. Complete FPGA snapshots remain deferred.

Next: first run original-ROM/native regressions with the corrected MC1668
semantics; then close functional RM/STK address/read/write/parity ports, l06's
remaining sign/odd contributions, Ready/MIR/Hold and other board timing. Keep
large arrays behind RAM ports. Native clock integration, faster original-
firmware Cedar/input, rollback, repeated boots, cold persistence and two-hour
stress remain required.


From a checkout with fresh output:

```
make -C verilog r-operand-test R_OPERAND_OUT=build/r-operand-new
make -C verilog mc1668-latch-test MC1668_OUT=build/mc1668-new
```

The qualified run executes from frozen inputs. Raw source/reports/logs are
losslessly compressed; the manifest records raw sizes and SHA256. Objects,
executables and Quartus working databases are omitted. Verify and extract
manifest members before running these replay commands:

```
python3 review-tools/tools/audit_control_clock_fit.py timing --qualified qualified --output /absolute/review.json
python3 review-tools/tools/test_processor_capture_fit.py timing --qualified qualified --component r_operand --output /absolute/fresh/reader-controls
python3 qualified/executed-inputs/tools/test_r_operand.py --output /absolute/fresh/simulation --mutations
```

Extracted timing review and reader controls are rerun. No second full simulator
replay after extraction is claimed. The separate MC1668 receipt covers the
production cell truth table; this evaluator's fit does not fit a complete core.
