# Passive firmware windows and corrected113B simulation handshake

The native harness used `0113` in SystemVerilog, which is decimal113;
the original packet's113B is decimal75. C/C++ correctly interprets `0113` as
octal, so the interface crossed two different literal conventions. The harness
would not release C or arm its post113 trap observation on the real request.
Both comparisons now use `32'o113`. This is a simulation-stimulus/observation
fix; original firmware, FPGA RTL/clocks and physical MiSTer behavior are unchanged.
The hardware keyboard helper already matches the actual octal packet text.

The exact C-make/wait/break slice extracted from the ACTUAL corrected harness
passes Icarus and Verilator: decimal113 must leave C held, decimal75 must release
it within the existing1024-clock polling cadence. The actual old comparison
compiled into that slice fails in BOTH engines for releasing on decimal113.
This is not merely two tests sharing the same mistaken constant.

The new optional passive observer waits for a requested epoch/trigger rather
than filling its trace cap before the CPU runs. Each window records matching
raw A/R pairs through the FINAL capped ACK, first-valid and consumed latency,
coherent host-gate/raw-block/fanout partitions, group admission waits, completed
line/in-flight past/current/future/cold read classes, writes and row availability
at command-requiring admission. Row availability is not a fitted IO guarantee.
It accepts only one raw owner at a time, handles same-edge consumption/issue,
and reports reset-aborted and terminal-partial windows explicitly. No assignment
targets the DUT or stimulus. A no-argument DPI function is polled procedurally;
a continuous wire cannot rely on a host-only state change generating an HDL event.

Exact observer fixturev4 gives identical results in both simulators: three
complete3-request windows and one reset-aborted1-request window, held first
response, wrong-BFN rejection, first-issue/manual triggers, all six read/write
classes and row hit/closed/conflict. Independent parser checks the raw ownership,
timeline and complete clock/request partitions. These are observer/component
tests, not firmware qualification. Earlier compile/task-final and trigger
development failures are retained, including their executed sources and logs.

Runtime control is a small text file containing `epoch trigger`:
trigger0 immediate,1 after an actual issue,2 after actual Cedar113. Default
`window-control.txt` is `1 2`. Raise the epoch to request another window; a stale
epoch cannot re-open it. Files select recording only; they do not affect guest
or external-device state. `--storage-windows` implies `--storage-traffic` when
preparing a future native diagnostic. `report_native_storage_windows.py` retains
partial/aborted windows instead of labeling them complete.

The old v1 pair was deliberately bounded at400M host ticks after finding the
wrong BFN wait, before CPU/113. Both actual simulations exit0 at HOST_LIMIT with
all recorded compilation/test stages0 and no model/device fatal. Their raw
driver states say TERMINAL_FAILED despite saved HOST_LIMIT fields: the runner
placed final progress printing inside its failure handler. The precise old
exception was not retained; closed tool output is the likely explanation, not
proven exception attribution. Keep the contradictory raw state. New runner
handling preserves saved results on BrokenPipeError and records real exception
details. A mocked-command lifecycle test proves closed-output success and actual
stage failure remain distinct; it is not an RTL or firmware test.

At400M, v1 control/candidate instruction issues and BFNs areZERO. Full physical
23-bit selftest passes; the earlier matching power-up prefix remains valid.
Do not reinterpret their read-only background traffic as Cedar or claim the
fixed handshake has already booted firmware. Both old processes are verified
absent. Their source hashes, original bad comparison, full terminal logs and
deliberate-bound record are preserved.

Corrected paired diagnostics are compiled and verified LIVE:
`native-cedar-5c65-windows-v2-20261022` driver1628/test2284 (160 HDL sources),
`native-cedar-queue-windows-v2-20261022` driver1652/test2331 (161 HDL sources).
Sources/devices/seed/clocks/stimulus match between arms, with only the three
qualified transport sources differing. Both are initializing RAM; no firmware
window or speed conclusion yet. Observe these jobs, never restart on a polling
timeout. Their frozen drivers predate the final BrokenPipe guard, so inspect
actual test return codes/logs if final reporting fails again.

The field appendix contains the clear post-OSD native cold readback screenshot
that arrived after its helper's8s bound. It was retrieved as the same request,
without a repeated action. Cold native files/input pass separately; HDMI remains
black and unresolved. Original60ns/all-device/native-fit/final stress gates stay
open; goal active. No new core assembly/programming or Main change.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-23-native-firmware-windows/qualification`.
