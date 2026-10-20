# Serial69: source-qualified native memory pause, physical build pending

Start from immutable Serial65, seed1, original periods/IO/constraints. Real
arbiter/SDRAM idle and initialization qualify the late memory freeze. All eight
MSAs use separate board/frontend holds; all nineteen boards retain guest hold
through thaw. The owner and UIO share the derived service vector. Other service
ACKs remain explicit virtual inputs; assembly is disabled. Snapshot control is
cleared by FPGA configuration only, never PLL/menu/framework/parser reset.

Independent inverse review checks659 parents/660 actual inputs and2,761 original
pins/parameters; ten rehashed wiring/status/reset/constraint controls reject.
Actual-QSF native XML uses320 HDL files/15 macros, preserving every original
board/transport parameter, with no native UNOPTFLAT loop.

Correct the earlier partial generic-sequential inventory: SDRAM rows104 bits
and dq_out16 bits are persistent global task destinations reached by clocked
processes. Both are required. With these existing120 bits and new2-bit pause
phase, account2,094,301 native bits: 1,957,048 physical +4,578 private board +69
private transport +132,606 WRAPPER ACCESS PENDING. No sequential/task exclusion.
Eleven function-local candidates/215 bits are automatic scratch or static
returns assigned on every path before use, with no global writes/NBAs. Fifteen
geometry/task-presence/clock/alias controls reject. Native XML scope excludes
top emu/UIO, external RAM bytes and ARM services; no complete snapshot claim.

All1,149 raw members read back losslessly. Extracted source/pin/ten-control
review, fresh XML/task review/fifteen controls and exact660-input/three-record
restaging replay with immutable receipt dependencies and no working HDL.
The Quartus builder2566540 is at /home/alans/mister/Dorado_Serial69MemoryPause_20261020;
observe that SAME process/children through terminal, never restart on timeout.
Capacity, all-condition timing and304 adapter crossings still require review.
No hardware, complete snapshot, faster clock or measured speed is claimed.
