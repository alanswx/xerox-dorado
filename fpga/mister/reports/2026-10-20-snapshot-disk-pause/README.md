# Nonblocking ARM disk snapshot lease

The actual Main cache/adapter can hold the mounted image generation, defer
normal SPI reads/writes before consuming or acknowledging data, drain accepted
file work through the existing bounded worker, and export clean RAM chunks
only after the write/fsync boundary. Premature continue, lifecycle replacement,
stale generations and IO errors retain the lease. Main remains the SPI owner.

The caller must first stop guest disk activity and finish already-issued FPGA
requests, then hold image selection. This cache lease alone cannot drain an
FPGA request whose response it has just deferred. No whole-machine coordinator
or UI integration is included.

Final Linux qualification passes three snapshot cases, four compiled broken
variants and the existing cache/adapter gates (including delayed work, queue
pressure, short IO, error recovery, close/remount, signal drain and SPI ownership).
The static ARM binary runs all three cases on MiSTer14 using temporary images
and SPI spies. With an injected 4.2-second write, its maximum observed poll gap
is 3.533 ms. The normal case compares all 262,144 bytes with an independent
oracle and the physical file, then closes/remounts and checks every sector.
ENOSPC retains 31 dirty sectors; fsync EIO retains the unsynced state.

Linux-v1 is an earlier candidate pass. Linux-v2 preserves the complete pass
and the exact ARM-build provenance. Linux-v3 corrects a test-driver metadata
variable: v2 stored mutation text in original_main_inputs instead of the four
original hashes. Actual C++/ARM executed inputs are byte-identical in v2/v3;
the final driver, all gates and ARM build pass again. The ARM-runtime receipt
is tied to v2's exact binary SHA256 and both sets of executed input hashes.

All 319 raw members (2,296,373 bytes; 663,751 stored) read back losslessly.
Extracted evidence/proof/verify.py rechecks terminal processes, input/log hashes,
candidate/mutation outcomes, ARM identity and original Main hashes without
working source. The six final worktree files match the executed v3 source.
Executable/object files are excluded; raw commands, source and logs are retained.

No production Main/daemon/core deployment, actual FPGA service barrier, disk
snapshot file/COW bundle, complete state import or Cedar restore is claimed.
