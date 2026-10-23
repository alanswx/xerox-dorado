# Compiled full native comparison: running prefix only

This receipt freezes both diagnostics' complete inputs, provenance, successful
compile logs and a running log prefix. Both actual simulation PIDs were verified
live with `ps` at archive time. This is not a terminal startup, firmware or speed
result. Observe the existing jobs; do not restart after a polling timeout.

Control: `build/rtl-executive/native-cedar-5c65-traffic-v1-20261022`, PID85205,
session10485,160 compiled-used HDL sources from the exact qualified5c65 recipe.
Candidate: `build/rtl-executive/native-cedar-queue-traffic-v1-20261022`, PID85896,
session46590,161 HDL sources: the control's source list plus the queue. Actual
native geometry is128 groups/16-bit local/23-bit raw addresses in both.

The candidate differs only in the wrapper, backend and added queue, bound to
the preceding component receipt. Clocks/CPU/cache/Map/ECC, original EPROMs,
read-onlyf3bc seed, normal three-button boot/PS2-C and external devices match.
Guest disk writes update a volatile copy only. No CPU/memory/IOCB state import.

Both are still performing actual physical-memory initialization. Counters and
bounded raw traffic capture start after storage initialization. No instruction
rate or native firmware hold distribution is established by this prefix.
Historical full diagnostic runs took about3.3hours/1.5B host ticks.

Next observe startup, original firmware progress and actual128-group wait
attribution; compare matching traffic and instruction progress. Quartus fitting,
all-corner timing, FPGA speed, Cedar persistence and final stress gates remain
open. Hardware5c65/Main/daemon and existing volumes are unchanged.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-row-writes/native-diagnostic-prefix`.
