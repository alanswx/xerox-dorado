# Same-edge board readiness and faster native timing

This receipt preserves5815 members/1283 blobs, including terminal component
results, the rejected66.7MHz full-native build and frozen57.1MHz source stage.
Earlier immutable receipts supply identical source blobs by relative reference.
No new FPGA image was assembled or loaded. MiSTer14's unchanged50MHz core still
runs Cedar; the included03:00 UTC internal screenshot shows two CommandTools,
exact keyboard echo and the previously written/read-back file marker.

## Qualified components

The common step selection is distributed over the two post-edge readiness
reductions and registered once per MSA on the SAME host edge. No extra host
cycle or stale-ready sampling is introduced. The original144-chip oracle
passes12012 cycles/all9437184 stored bits; the full positive raw trace including
host timestamps remains byte-identical. Three compiled bad reductions fail at
runtime. Width8/12 groups pass40000 checks each in Icarus and Verilator; stale
readiness controls fail. The combined board-ready/fixed-capture pair passes the
same original-board trace and three runtime controls.

The one-MSA15ns fit passes all four operating conditions: minimum setup+0.123ns,
hold+0.072ns, ready-to-step+6.113ns and late-step-to-prediction+2.827ns. One board
ready register replaces the sixteen child registers after synthesis. Warning
and source review passes; eight damaged receipts reject. A failed first launcher
import occurred before Quartus and is retained. Quartus's exact version-only
QSF append is bound separately; no timing constraint was weakened.

SDRAM capture uses fixed lane slices with control enables, preserving the actual
capture/response edges. The three-bit command register retains its RAS/CAS/WE
values instead of adding a one-hot decoder after the output register. The final
backend passes the26-action canonical pin matrix,66.7MHz nominal/delayed-pin
profiles, reversed-cache control and the16-action native trace/fault matrix.
Full23-bit sweeps at57.142857 and66.666667MHz each complete50331648 requests:
both address patterns and verified zero-fill across all8388608 raw cells.

Both faster full backplanes pass8M startup comparisons over175 outputs/all128
groups; the compiled disconnected-step control fails at810ns. These are array
backend startup PREFIX checks, with zero CPU issues/writes. They do not establish
original firmware, physical SDRAM or Cedar correctness on a faster FPGA image.

## Full native66: timing failure, rejected

All nine build stages and the three repaired read-only report jobs are terminal.
The953-member downloaded archive verifies outer SHA
`f61b17cce720a36a2162dfcdd4bc618585a775fd06562fb37e1827342afd3096` and every
decompressed member's size/hash. All503 source hashes verify. Quartus rc0 means
the reports completed: the core setup summary is-1.877ns and the detailed worst
path is-1.943ns. The path runs from MemC/MemD data through BMux/MC10181 ALU,
ProcH data/diagnostic selection and DMuxData to ProcL k22's input snapshot.
Other negative CPU paths are retained; shortening one path does not guarantee
closure. No setup exception or guessed diagnostic multicycle waiver was added.

Memory-ready-to-network_step now has+1.093ns minimum at the7.5ns half-cycle;
prediction setup+0.833ns. Physical SDRAM output setup is+3.369ns. Thus these
memory cones improve, but the whole-device candidate FAILS timing and cannot be
assembled/deployed. Even its SYSPER4 ordinary ceiling would be8.333M/s before
waits, below the original60ns/16.67M target.

The first read-only reporter/collectors encountered state-file startup/rewrite
races; their failed scripts/states/logs remain in the terminal receipt. Only
reporting was repaired; the original Quartus fit was never restarted. The
[helper-recovery supplement](helper-recovery/README.md) preserves the original
collector's nested failure inputs omitted by the terminal file selection. The57MHz
reporter had a similar pre-state startup failure; its waiting collector536783
was explicitly stopped after verifying the producer failure. Repaired reporter
591029/collector591030 follow the unchanged original builder536779. The frozen
57MHz stage here is source evidence. The [separate terminal receipt](native57-terminal/README.md)
now verifies951 members: all31 device summaries and observed detailed slacks
are positive, but strict final qualification/assembly guards remain pending.

## ALU follow-up

Boolean carry lookahead preserves all16384 binary input combinations and all
seven MC10181 output pins in both simulators, with independent HM3.7 arithmetic
checks and compiled wrong-GG/lost-Cn controls. This changes no state/capture
epoch and no canonical cell. It is a functional component proof, not a speed
claim. The [separate503-source stage and live build prefix](alu-native66-source/README.md)
preserve the fresh full-native timing experiment.

Superseded pin/trace/full-memory runs remain identified by actual inputs. The
first pins attempt hit the original50MHz simulation guard, not a data mismatch.
The trace-v1 Python syntax failure occurred before simulation and is retained
as a pre-run failure. Final v3 pin/trace and full66-v2/full57-v1 runs include
the command-encoding attribute. Full-memory simulation is distinct from the
still-required physical-memory/original-ROM/Cedar/input gates.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-board-ready`
