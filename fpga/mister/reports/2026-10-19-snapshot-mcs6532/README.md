# Actual MCS6532 wrapper: complete phase and RIOT state

**Simulation, fresh classification and corrected matched50MHz routed fits
PASS.** The actual wrapper now exports all1098modeled bits: the separately
qualified1097bit MOS RIOT plus its original initialized phase_d history.
Words0..132 address the RIOT;133 addresses phase_d;134..255 error without
writing. Original rising-phase CE and bus output-enable masking are retained.
Both wrapper and child freeze before access, and reply routing handles
back-to-back child/phase/invalid requests without a pipeline warm-up edge.

Accepted v2:91,501host/4,469held/4,135requests/2,600writes,17,772real phase
rises and4,083late-pin changes. All1098bits and256indices, dense saved RAM,
phase HIGH at save, arbitrary inputs/advance during held access, child/phase
reply switching, candidate-only destructive continuation, full restore,
first HIGH resume with RESET INACTIVE and no synthetic RIOT tick, subsequent
fall/rise and20,000more steps pass. The unchanged original wrapper/RIOT and
an independent RAM/port/timer/interrupt/phase model check all physical state
and25public outputs plus internal OE observation. Eight broken RTL variants
and two changed-source controls reject. V1 passed, but v2 strengthens first
resume by preventing reset from masking an extra tick and compiles immutable
captured inputs rather than live repository test/reference paths.

Fresh wrapper XML saves phase_d and all1097RIOT bits, checks default
init_7800=0, classifies the source-pinned unused-power reduction as constant0,
and rejects12damaged schemas. The vendor classifier now keys PA7 history to
its actual source declaration/procedure rather than incidental Verilator
file IDs;12vendor-schema/source-location controls also reject. Earlier
standalone/wrapper/board file-ID forms remain in preserved reports.

The first routed fixture is REJECTED: Quartus10207 cannot synthesize its
private cell_inst.oe observation. The corrected monitor uses the identical
CS1 && !CS2_n && RW_n equation from launch pins. No wrapper/RIOT RTL, clock,
SDC or timing budget changed. Both attempts are retained.

Corrected matched fits and independent audit are terminal PASS, all four
conditions/minimum **+0.162ns**, no illegal/unconstrained clocks/I/O or extra
exceptions;10damaged-fit/schema/binding controls reject. Actual child
parameter is0. Snapshot synthesis total1121 =1098state+23interface bits:
child1116(1097state+18response+shared frozen) plus wrapper own5(phase_d, done,
error, one phase-result bit and selector). Identical frozen registers merge;
unused child done/error prune. Baseline total1091/own1 plus child1090.
All state remains in logic registers. Fitter shell totals672ALM/1208regs ->
2064ALM/1351regs; zero block RAM. These are isolated shell/optimization counts,
not an enabled whole-core snapshot budget. All25compiled input hashes and
accepted immutable test-input hashes were verified after archival extraction.
Raw sources/log/XML/report text and JSON above100KiB are gzip stored unchanged.

No production BaseBoard or running MiSTer RTL is changed by this experiment.
Next: remaining BaseBoard SRAM/EPROM registered reads/clock-reset families,
complete board classification/restore/timing, then global backends/input/ARM/
server/matching-disk barriers and physical FPGA machine save/restore.

The repository oracle header has only its extra EOF blank line removed after
testing. Exact compiled header bytes are preserved gzip-compressed in both
executed-inputs records and tools; C++ tokens and RTL are unchanged.
