# Drain accepted raw memory work before freezing frontends

The complete chip-width MSA gains one explicit frontend-hold input. FF/board
state can stop immediately while its sixteen frontends finish accepted reads
and writes. The two-bit memory pause coordinator latches frontend hold only
after guest-held, frontend-drained, arbiter-idle and raw-service-idle. It retains
guest hold through frontend thaw and keeps frontend hold through serial state
rotation. Abort/lost ownership quarantines until a true machine reseed; a parser
reset or PLL loss is not a reseed. Physical SDRAM refresh must continue.

The original-RTL physical/traffic oracle passes 17,123,973 checks /1,941,317 host
edges, eight pauses (75 pending reads/five writes), all 86 held state words,
full ordinary serial write/restore and 20,000 post-restore guest steps. Four
early-freeze/ignored-raw/ignored-arbiter/nonsticky-hold variants reject. Backend
queues are an oracle, not the actual native arbiter/SDRAM.

Inverse source review restores the exact frozen qualified board, every helper
and original schema; five damaged variants reject. Fresh XML accounts all
1,538 physical DUT +2,690 original-reference +81 private transport bits, plus
two REQUIRED WRAPPER ACCESS PENDING coordinator bits, none excluded. Eighteen
inventory/alias controls reject. Four-state control tests pass 17,209 checks,
256 drain combinations and 4,096 readiness combinations; four broken controls
reject, including loss of quarantine and unknown acknowledgment admission.

All 918 manifest members read back losslessly. Extracted sources reproduce the
fresh XML/inverse review and controls, four-state candidate/controls, and a new
build/run of the actual complete-MSA candidate, using prior immutable report
dependencies without working HDL. Raw earlier attempts are retained: first
pause stimulus missed accepted work; second lacked pending-write coverage;
third/fourth exposed missing negative-control sensitivity. Fifth passed before
adding explicit retained guest-hold release coverage; sixth is the qualified
final fixture. Compiled objects/executables are excluded.

This component is not integrated into Serial68 or deployed hardware. Real raw
service/arbiter acknowledgments, all-eight-MSA/native wiring, phase access,
full-core fit/timing, wrapper/ARM/disk-paired state and complete FPGA snapshots
remain open. No speed improvement or full-machine snapshot claim.
