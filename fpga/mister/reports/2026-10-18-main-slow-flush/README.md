# Native41c: actual Main delayed-flush qualification

MiSTer14, 2026-10-18 about21:29:42UTC. Native41 RBF a97af936,
disk-cache Main40e14b6b PID16875, server e237dffc PID16872. The original
firmware boot reached Viewers; this test changes no FPGA/guest state.

`main/tests/slow_sync_probe.cpp` is a qualification-only DSO built with the
pinned GCC10.2.1 ARM compiler/libstdc++ ABI. It queues one bounded callback
through the actual Main `offload_try_add_work` entry. The job owns a duplicate
of the mounted image descriptor, waits for an explicit arm file, sleeps4200ms,
then calls real `fsync`. It never writes image bytes or accesses SPI. Keep the
DSO resident until Main exits; callbacks and their destruction use its code.
This is deliberately delayed flush **work**, not a measured4.2s kernel syscall
or an injected delay inside the disk cache's `pwrite` implementation.

Actual Main result: **PASS for continued instruction and disk service**.
The worker window is107483064252983..107487264388008 monotonic ns:

- Three actual read requests/ACKs and21 write requests/ACKs complete while
  the worker is occupied. No physical writes run during the window; queued
  writes/coalescing and successful fsyncs follow its end.
- No overlapping `poll_gap` event (recorded above20ms), trace drop or error.
  Four in-window Main samples show instruction issues advancing with no
  pack/CRC/RX/abort fault. See `qualification.json` and the full trace prefix.
- Real keyboard submits Return plus `echo 42`; mouse moves40,20. The captured
  image precedes worker completion, independently checked on ARM. It shows
  the redirected-file command still running. **Do not claim Echo completed
  during the window.** The later screenshot shows exact42 and a new prompt.
- A subsequent real Main durability barrier succeeds, then Cedar
  `type ///rtlflush42.txt` reads exact `worker42` and returns to its prompt.
  This is coherent/durable-host readback, **not cold-remount persistence**.
- No new guest network packet/server log bytes occur during the window.
  **Packet progress under worker delay is not qualified by this test.**

The setup and post-window durability barrier use matching Main ELF/GDB at
`input_poll`; their known foreground pauses are outside the measured window.
The worker test does not add SPI owners. Unit bridge tests first pass on
Linux and actual MiSTer ARM, covering descriptor ownership, unchanged content,
queue rejection, duplicate refusal, arm timeout and drain-before-DSO-unload.
Build/reproduce with `main/tests/test_slow_sync_probe.py PINNED_MAIN_SOURCE`;
`--emit DIR` writes the cross-compilation harness. `run-live.py` documents
this one deployment, not a reusable launcher: its PID/hash/path and initially
absent arm/event files are asserted and it must not be rerun blindly.

After readback, normal guest `boot` was typed. The actual screen changed
from Viewers to the germ's812 display. Cold image remount/readback is still
pending; retain the mutated Native41c pack and both marker files.
