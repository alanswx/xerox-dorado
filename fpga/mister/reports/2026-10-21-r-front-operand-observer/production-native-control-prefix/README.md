# Paired production memory diagnostic: running prefixes

This immutable receipt adds a full-memory control from the qualified MC1668
stage. All498 reviewed stage files match exactly, with158 actually-used source
HDL files selected from its completed Quartus map report. The control compiles
and its packet-only link check passes. Driver83197/test83509 were confirmed live
at collection; no original-firmware outcome is claimed. Its operand observer
is absent, so harness operand-ring fields are zero placeholders.

Candidate V38 driver79190/test79583 remains the same existing cold run. It is
not restarted. Both use the same seed, external packet service and host-based
timer argument, production device timing and original EPROM boot. The control
adds no CPU/memory emulator or guest state/IOCB completion. The comparison lists
all nine changed common files and fifteen candidate-only files; it is a source
inventory, not a claim that their behavior is equivalent.

Read-only LLDB observations use layouts compiled from the exact generated
headers, which are included. Some generated module-port fields are optimized
placeholders, initialized but never assigned during evaluation. The early
logger printed those fields; they are **not live pins**, and no cause or speed
claim relies on them. Its corrected summaries use assigned internal cache hit,
wait and pending-write state. Sparse observations may land within evaluation;
they do not establish consumed-data equivalence or instruction throughput.
No inferior function call, guest write or debugger core is included.

572 members /528 unique blobs,479 shared from the preceding production prefix.
Verify with `python3 tools/archive_r_front_ready_followup.py --verify DIR`.
No binaries, packs, FPGA images or mutable live logs are included. Later results
belong in a new extension. Native firmware, desktop/input, speed, persistence,
authentic FPGA rollback and all final acceptance gates remain open.
