# Original count-branch and IM/TPC command producers

`dorado_r_front_commands_step.sv` retains792 physical Qs,9216 functional RAM
bits and68 clock channels. It closes the actual ContA count-branch and
IM/TPC command logic in the independently extracted processor/R-front graph.
SHA256: `3110f49e6dc9e3eef6b3dcdd341d1d1ee05b3f5d14b1a3613663dfd419eee350`.
This is bounded digital ordering; no native or physical timing qualification.

The reference adds original a14/a21/b15/d19/f22/f23/d20/k11 packages, keeping
all wired-OR drivers and distinct MIR JCN copies. f24/a21 and a19/b15 decode
the FF count branch; f22/f23/d19 decode the JCN count branch; a14 and d21
produce jcnt. d20 decodes Return-class IM/TPC commands. k11's NAND of RIM'
and RTPC' supplies d17's real input. WIM' is now the control qualifier that
sets/captures the CRamClock state, rather than a fixture input.

The source generator inverse-checks all edits against the qualified792-Q
NextMacro wrapper and104-Q Control-A source. Only the new decode functions,
their connections, the explicit317-pin fixture and d17 data source differ.
The actual WIM qualifier uses its resolved view; d17 samples OLD MIR data.
JCN.2'a in d20 is d24.B's complement; RWTPCorRWIM retains d23.B's distinct
physical copy. No register, pipeline cycle, reset, initialization or RAM copy
is added. Original parent modules remain unchanged.

Final `r-front-commands-full-v4-20261023` passes all six delay profiles in BOTH
Icarus and Verilator:13449 events,318 write-active events,5604 read-active
events and801 old-parity capture epochs per run. Every event compares all792
Qs,9216 RAM bits,68 clock channels and functional outputs. All clocks fire;
all100 front Qs and16 Ready Qs change. The inherited host-advance hold,
read/write nonoverlap, old-parity, post-commit source and write-window address/
select checks remain active. PARC parity-min/max and9.9ns RAM reads pass.

Directed commands use real MIR/CP transactions. All eight d20 selections
assert their expected WIM/WTPC/read results and d17's captured value. Each
selection has the original two- or six-clock instruction interval:120 total
intervals, plus16 observed WriteIM phase samples. Twelve FF cases exercise
ASEL/BSEL enable and selector neighbors; four JCN cases distinguish count
branch from its neighboring encodings. All16 RBase entries at both extremes,
256 StkP values, HIGH-/LOW-root CP and late task/branch epochs remain tested.

Four damaged sources reject in BOTH engines after successful compilation:

* External RIM/RTPC: physical Q786 mismatch at check234.
* New-MIR d17 data: physical Q786 mismatch at check222.
* External WIM: shared control-state disagreement at check7146.
* External count branch: board-crossing disagreement at check7375.

All20 cases/40 compile-test actions are terminal. jcnt's processor DecCnt
result is unconsumed in this selected graph, so its board wire is explicitly
compared. No state-only passing mutation is treated as source qualification.

Twelve remaining package-half inputs are proven unused in this selected graph
by tracing exact pin-function dependencies to terminal outputs. The newly
included k11 has multi-gate Call paths; stopping at the first combinational
consumer would not establish their status. Decoder vector declarations are
expanded and agree with the old scalar reader where it supports the cell.
Five live/exported dependency controls reject, including a newly live Call
output after the multi-stage path. Full-machine Call inputs remain required.
Slots315/316 keep Call12/15 distinct; obsolete8/309/310/311/313 no longer drive
these selected producers. l17's input to d17 remains an explicit boundary.

Earlier attempts are preserved. The first Icarus probe ran256 random iterations
and is not a same-workload engine comparison. Both v2 directed fixtures stopped
with matching original/candidate state when their cadence check found Run had
been omitted; v3 restores actual Run/refresh inputs, with unchanged RTL. The
first audit reader could not expand a vector decoder in a negative control;
no audit pass was recorded until the complete reader rejected all controls.

Reproduce with unused output directories:

```
python3 tools/qualify_r_front_commands.py \
  --base build/rtl-executive/r-front-ready-qualified-v1-20261020 --output NEW-DIR
python3 tools/audit_r_front_command_boundaries.py \
  --reference NEW-DIR/balanced-verilator-original/reference --output ANOTHER-DIR
python3 tools/archive_r_front_ready_followup.py --verify \
  fpga/mister/reports/2026-10-23-r-front-commands/ordering
```

Next test a separate MIR-clock masking simplification to shorten the rejected
task-priority -> MIR mux -> RAM/parity path. This receipt does not qualify that
experiment. Remaining producers include l17/clock1/RBMux.04, actual ContB IM/
ASEL/BSEL/StopMIRClk and MemC Hold/PRhold. The previous combined15ns fit remains
REJECTED; no new fit, exception, assembly, FPGA load or speed gain is claimed.

The original native simulations remain2284/2331, with unchanged5B limits,
inputs and post113 window controls. Watcher1 reached its own observation bound;
watcher2 is a read-only successor, not a restarted simulation. Both watcher
records here are observation evidence, not firmware/Cedar or speed proof.
