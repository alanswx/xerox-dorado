# Dorado RTL speed handoff — 2026-10-26

The active goal is still the original 60 ns machine, interactive original-firmware
Cedar, authentic Checkpoint/Rollback and persistent files, followed by three fresh
boots and two hours of disk/network/input testing. Complete FPGA snapshots remain
deferred. MiSTer14 is available under the user's latest allocation. Build Quartus
on `misterubuntu`; run simulation on the Mac.

## Current speed and timing

No new FPGA image has been loaded during this work. The last retained hardware
measurement is **4,025,473 StartCycle issues/s**, not retired instructions. The
current diagnostic schedule is **120 ns per ordinary microinstruction before
stalls**, versus the original **60 ns** target. These ceilings are 8.33 and
16.67 million ordinary microinstructions/s respectively, not measured throughput.

ReadyBinary job **1930755** is terminal, with all nine build/report stages exiting
zero, but CPU timing fails: **−8.909 ns** setup at slow/100°C and **−8.635 ns** at
slow/−40°C. Its worst data path is 22.862 ns against a 15 ns relationship. The fit
uses 36,478 ALMs (87%). Reported SDRAM timing checks pass; they do not qualify the
whole core. Job **1865964** is also terminal and rejected. Neither image was
assembled, selected or loaded. Do not restart either completed job.

The measured path is:

```
ContB ASEL.0a (held bit 799)
 -> ContA FA/branch decode -> ContB dBSEL.2
 -> sparse-fall eligibility
 -> shared control capture frame 670
 -> processor T capture events
 -> whole-machine enable / G3 (8,464 loads)
 -> MemC cache-tag RAM enable
```

About 75% of that data delay is routing. A faster SDRAM clock does not repair this
dependency. The separate direct-inner Ready experiment remains unselected because
Ready comparison has ceased to be the worst path.

Source bindings for further phase work: LC0/LC1/LC2 are held bits **806/809/808**,
from ContB **h24.A/i24.B/i24.A**. Their data are dLC.0/1/2; their clocks retain
the separate clk0'D copies and RepeatCurDc, with sLC sets and wired-OR rMIRa
reset. Shared frame 670 is old controller Q10 at the third root settling wave,
bound to **ContA k02.A / CLKEnable'b**. Settling waves are combinational
evaluation steps, not the original t0/t1/t3 instruction phases.

## New evidence: remove an unnecessary falling-edge data dependency

The original settled-HIGH identity certificate allows **36 IM data inputs** to
change across the falling root without changing the control result or its shared
capture epochs. This is an edge-specific result: it retains the actual rising
captures, CP maintenance inputs, Hold/Ready, resets and storage barriers.

All paths below are relative to `build/rtl-executive/`:

| Evidence | Result and scope |
| --- | --- |
| `native-control-phase-cut-v1-20261025` | All **795** control/frame bits match for arbitrary binary input changes under a certificate computed by the original RTL. Includes **432 frame bits**. Omitting a CP selector or using an uncertified entry produces counterexamples. A cover reaches changed dBSEL.2 with the original CRam event. |
| `native-control-phase-closure-v1-20261025` | The actual CP-prefix/root producer establishes the next identity certificate. All **156 owners**, event/repeated channels and unsettled status checked. A stale published owner fails. |
| `native-control-phase-cache-v3-20261025` | **Unbounded induction passes** for the actual specialized packet cache, original producer and Hold-independent LC projection. Owner/history power-up is unconstrained; only real cache initializers apply. Input histories are arbitrary, and other barriers may withhold publication. A stale published ASEL fails. Ordinary native path only; this is not paired-mode qualification. |

Cache proof v1 had redundant identical CP function definitions. V2 uses unique
definitions; v3 additionally includes the actual LC selector projection. Keep all
three histories; use **v3** as the current certificate proof.

The candidate changes only the existing sparse-fall masks in the control transfer,
capture transfer and Ready comparison. It adds no state or host/guest cycles.
The qualified capture-path stability comparison shrinks from 85 to 49 inputs.
It must not be used merely because an arbitrary caller asserts `entry_fast`: the
actual identity certificate is essential.

Complete native qualification **passes** in
`native-control-phase-native-v3-20261026`: the actual caller owner projection,
all **2,970 outputs / 1,982 arbitrary binary inputs** under the computed entry
contract, and a real missing-certificate counterexample. There are no SAT input
assumptions; the entry contract is explicit and justified by the separate native
cache induction. This is not unrestricted equivalence for callers claiming a
false certificate. V1 exposed an obsolete proof-harness declaration:
`captured_r_flags_next` and `captured_read_next` were missing their actual widths
in the older projection builder. V2 supplies those widths as arbitrary input
fields. The corrected **actual caller owner projection passes**. V2 then hit a
WASM compiler exception while elaborating duplicate complete processor graphs;
it produced no equivalence verdict. V3 shares every unchanged module and copies
only the changed modules and their ancestors, retaining all checked outputs.
Preserve both earlier failures; neither establishes a physical RTL defect.

The combined, hash-verified evidence is retained in
[`native-control-phase-summary.json`](../fpga/mister/reports/2026-10-23-native-flag-windows/native-control-phase-summary.json).

## Next actions

1. Review the retained component, closure, native-cache induction and complete
   native comparison together. Qualification covers the actual native default
   wave parameters. Preserve the original unsupported-parameter mask fallbacks
   before generalizing this private candidate or creating a new stage.
2. Replay the changed candidate through both simulators, including X/Z fallback
   and the complete original caller checks. Bind it to the actual native source
   bank, retaining every owner, RAM, clock and readiness assertion.
3. Create an isolated stage, run optimized native lint and a structural/source
   audit, then fit the **full core** on `misterubuntu`. Collect all supported
   device conditions, CPU/SDRAM timing and crossings. Measure the actual new
   limiting path; do not infer timing closure from logical equivalence.
4. Continue the original t0/t1/t3 phase integration and timer calibration. This
   falling-edge change does not by itself convert 120 ns into 60 ns. Overlap
   preparation in existing phases; preserve older-instruction completion during
   Hold, special IM/TPC cycles and write forwarding.
5. Only load a reviewed, timing-qualified image. Measure pinned CPU progress and
   each wait cause on MiSTer14, then complete all Cedar/input/Checkpoint/Rollback/
   persistence/fresh-boot/endurance gates in the full goal.

The reproduction command is described by
`tools/qualify_native_control_phase_cut.py --help`. Its inputs are the ReadyBinary
stage, phase-cut v1, closure v1 and cache v3 above. The terminal v3 proof should be
reviewed and reused. Use a fresh output directory for a genuinely changed
experiment; never restart a live or completed run because observation timed out.

## Preserve ongoing runs and artifacts

The following original-ROM processes were confirmed live by `ps` during this
session. Revalidate their handles before calling them live again. Their sources,
limits, controls and media were not changed:

| PID | Run directory | Frozen host limit |
| --- | --- | --- |
| 65832 | `native-prepared-hold-startup-v1-20261025` | 1.5 billion |
| 55173 | `native-riot-blockram-startup-v1-20261025` | 1.5 billion |
| 38212 | `native-common-clock-startup-v2-20261025` | 1.5 billion |
| 62048 | `native-cp-read-j19-boot-v1-20261025` | 5 billion |

Latest inspected logs: prepared-Hold reached 410M host ticks with zero StartCycle
issues and its preparation-wait count unchanged at 190,628,639; do not charge
that cumulative initialization count as runtime CPU overhead. RIOT reached 590M
and common-clock 920M, also before instruction issues. J19 reached 2.53B host
ticks / 105,603,022 issues, with one TX and zero reported pack/CRC/RX faults;
the last direct inspection still had the first boot reply unaccepted. Recheck
the actual receive state before treating that observation as current. Do not accelerate its
timer, inject state, change its medium or restart it to force a retry.

Preserve qualified **5c65**, **Native59**, original firmware, all disk volumes and
protected Executive artifacts. Protected medium:

```
build/rtl-executive/cedar-work-native-hints.pack
63,578,624 bytes
SHA256 f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51
```

User-owned worktree changes must remain outside this milestone:
`CLAUDE.md`, `HISTORY-CLEANUP-HANDOFF.md`, `HISTORY-CLEANUP-commit-map.txt`.

The ongoing checklist is [verilog-clock-plan.md](verilog-clock-plan.md). Historical
receipts and full timing details remain in [verilog-handoff.md](verilog-handoff.md).
