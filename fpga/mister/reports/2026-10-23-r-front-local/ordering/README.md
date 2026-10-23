# Original Control-A producers coupled to the processor/R front

`dorado_r_front_local_step.sv` composes the qualified104-Q Control-A producer
with the processor/RBase/stack/RM/STK graph. It retains the786-Q parent map,
appends original d17 at791:786, and keeps one9216-bit functional RAM and68
clock channels. Candidate SHA256:
`0337c3990e0ccd051f14108d1af22d1ea7a5d66cc7337b6c50ba8ef8a1066982`.
This is bounded digital ordering, not native integration or physical timing.

The independent reference adds the actual original a13/d17 tasking and
e21/d21 branch circuitry. Actual package outputs drive processor StopTasks,
RWTPCorRWIM, NextMacro and jcnt arrivals. Both distinct physical JCN copies
and wired-OR contributions remain. The local decoder copies the104-Q decoder
byte-for-byte. Original processor package/storage models remain unchanged.

Ten generated adapters thread a separate StopTasks capture-data input through
the existing hierarchy. Only task-transfer data and SwitchUp preview data use
that view. Clock gates retain the resolved producer. Inverse transformations
must reproduce each qualified parent source byte-for-byte. No added pipeline
cycle, architectural state, RAM copy, reset or initialization is introduced.

Final `r-front-local-full-v8-20261023` passes all six profiles in BOTH Icarus
and Verilator: balanced, fast gates, later control, PARC parity-min/max and
9.9ns R-memory read. Each run checks11854 events,318 write-active events,
5012 read-active events and807 old-parity capture epochs. Every event compares
all792 Qs,9216 functional RAM bits,68 clock channels and functional outputs.
All68 clocks fire; all100 front Qs and16 Ready Qs change. The inherited gate
retains host-advance hold, read/write nonoverlap, old RAM parity, post-commit
source epochs and write-window address/select stability.

Real MIR/CP transactions select all16 tasks, write all32 RBase extreme cases
and load all256 StkP values. TaskingOff is stored through original microcode;
the obsolete raw StopTasks pin is not forced to select tasks. CP banks are
swept through128 values each with the root HIGH and LOW. The setup/cleanup
uses the actual distinct ASEL.0'a decode.

Five damaged variants reject in BOTH engines after successful compilation:

* New StopTasks capture data: shared-state disagreement at check201.
* Old StopTasks clock view: shared-state disagreement at check207.
* New branch capture data: shared-state disagreement at check262.
* Missing NextMacro crossing: resolved-source mismatch at check65.
* Missing jcnt crossing: explicit board-crossing mismatch at check65.

The two targeted LOW-root tests capture old task data when clearing MIR creates
a new StopTasks clock, and old branch stretch when one CP transaction changes
rMir and NoDispatch. Uniform new/old source views are not interchangeable.
jcnt reaches the processor DecCnt gate, whose output is unconsumed in this
selected slice. The earlier state-only mutation therefore passed; it is
preserved as incomplete coverage. The final gate explicitly compares actual
board-crossing wires in addition to state and functional outputs.

The updated dependency audit proves eleven other package-half inputs unused
in the combined selected graph. It rejects five bad classifications/live-cell
connections. The smaller104-Q component's thirteen-unused-boundary conclusion
does not carry over to the two newly observed d21 crossings.

All22 cases/44 compile-test actions are terminal. Earlier fixture attempts
and audit receipt-copy failures remain in the lossless archive, with explicit
notes. The directed v5 test completed the RBase/StkP commands but failed its
cleanup because ASEL decode was disabled; the corrected fixture changes no
candidate RTL. Compiler databases/executables/disk/snapshot bytes are excluded.

Reproduce with unused output directories:

```
python3 tools/qualify_r_front_local.py \
  --base build/rtl-executive/r-front-ready-qualified-v1-20261020 \
  --output NEW-DIRECTORY
python3 tools/audit_r_front_local_boundaries.py \
  --reference NEW-DIRECTORY/balanced-verilator-original/reference \
  --output ANOTHER-DIRECTORY
python3 tools/archive_r_front_ready_followup.py --verify \
  fpga/mister/reports/2026-10-23-r-front-local/ordering
```

Next close ContA e22's IFUNext'a/NextMacro decode from existing distinct MIR
bits, then branch-count/other d17 sources, actual ContB IM/ASEL/BSEL/StopMIRClk
and MemC Hold/PRhold. The source-only frontier inventory is not arrival proof.
Then shorten/refit the real RAM/parity path. The previous combined15ns fit
remains REJECTED (-2.961ns); no timing waiver, new fit, assembly, FPGA load or
speed gain follows from these tests. Full60ns/timers/66.7MHz physical IO/native
ROM/Cedar/persistence/fresh boots/stress gates remain open.
