# Production native memory diagnostic

`tools/run_native_cedar_memory.py` freezes a qualified native stage and runs
its actual `dorado_native_machine` with the original BaseBoard EPROM bootstrap.
It selects source HDL from the completed Quartus **Used in Netlist** table,
hash-checks the complete staged source inventory, and preserves the exact QSF
defines. It does not plant IM, CPU, cache, map or guest IOCB state.

External devices supplied by the harness are:

- The independent XSDS two-chip SDRAM model, with the production controller,
  zero-fill, arbiter and eight storage boards left intact.
- Normal PS/2 C make/break events through the production key queue and the
  production boot-button window. Break follows an observed Cedar BFN113 request.
- HPS sector transfers against a read-only host seed. Guest writes update an
  in-memory copy only; this is not a persistence test. Each transfer has a
  150,000-host-tick service delay before buffer transfer and acknowledgment.
- Raw packets through the production Ethernet wire adapter. `host.cpp` uses
  the isolated `ethernet.c` packet service, without the C CPU/machine/memory
  emulator. Packet timers advance with free-running host time, independently
  of the enabled machine clock. This adapter currently lacks the daemon's
  periodic idle STP probe; it does not qualify complete network recovery.

Prepare once with `--prepare OUT --stage QUALIFIED_STAGE --mapping MAP_REPORT
--medium SEED`, then run with `--run OUT`. Paths resolve to the frozen inputs;
the runner refuses to overwrite an existing run. Observe its existing PID/log
instead of restarting on an observation timeout. `host-cycle-limit.txt` can be
extended during a run; it is sampled every ten million host ticks.

The qualified MC1668 control is also supported with its `mc1668-candidate.json`
and `--qualification COMPLETED_REVIEW` when that review lives outside the stage.
The complete source inventory must match the review exactly. This control lacks
the newer operand observer; its operand-ring fields are explicitly zero and
must not be interpreted as processor values. Its PC and device observations
remain available. The candidate's frozen running harness is left untouched.

The qualified5c65 old-word/early-response recipe is supported through
`timing-paths-native.json` and its completed preassembly qualification. Optional
`--storage-traffic` adds only a passive observer to the harness: actual gate
categories,128-group pending fanout and admission waits, raw request/first-reply/
consumption latency, block reasons and a bounded8,192-request prefix. It changes
no staged machine HDL. The trace cap can leave the last request without its
reply; cumulative counters continue after the cap. It is a diagnostic, not an
assurance that Cedar boots or that a new controller improves native speed.

`tools/prepare_storage_queue_diagnostic.py` creates the paired transport
candidate from that frozen control and the archived component-tested native
preparation. Only the wrapper, SDRAM backend and added queue differ. Its source
list comes from the qualified control's Used in Netlist table plus the queue;
it has not been fitted. Full native128/16-bit-local/23-bit-raw simulation must
still pass startup and firmware before any hardware conclusion.

The `guest` progress counter counts enabled **fabric edges**, not Dorado
microinstructions and not MIPS. The diagnostic retains a 4,096-sample physical
operand/PC ring, dumps the first observed task-0 trap at CIA221 after113, and
stops after a persistent RequeueSearchSQ sequence or the configured host limit.
Neither stop reason establishes firmware failure or desktop success by itself. The progress `bb` field's low16 bits
are the BaseBoard MCA address bus, not the6502 instruction PC: operand and I/O
cycles can report RAM/port addresses. Use actual instruction-fetch evidence to
attribute a firmware routine. The staged BaseBd source constructs this field
from MCA.15..00.
This diagnostic excludes MiSTer sys_top, Main, Linux, direct-DDR transport and
HDMI capture. Complete FPGA snapshots remain deferred.

`test_host.cpp` sends a captured physical Cedar113 request and independently
compares the returned payload against the original EB after its 512-byte header.
Its recorded result is packet-adapter evidence, not a native firmware boot.

The SystemVerilog C-release/trap comparisons now use `32'o113` explicitly.
Leading-zero `0113` is decimal in SV and octal in C/C++; the former harness
would have waited for the wrong boot-file number. The actual extracted release
slice is regression-tested against both decimal113 (keep held) and the captured
packet value75 (release), in both simulators, including a rejected old comparison.

For future memory comparisons use `--storage-windows`. It adds passive
`storage_windows.sv.inc` and `window-control.txt` (initially `1 2`): epoch1,
trigger2 = actual Cedar113. Trigger0 is immediate,1 after an issue. Raising the
epoch opens a later window; no stale request repeats. The observer records all
8,192 admitted requests AND their last replies, coherent clock categories,
first/consumed latency, fanout, per-group waits, read classes and row availability.
It reports reset aborts/terminal partial captures explicitly. Changing this file
selects observation only. No new guest/device-state import or stimulus bridge.

`tools/test_native_storage_windows.py` validates the exact observer and actual
C-release slice in both simulators. `tools/report_native_storage_windows.py`
checks the raw trace/ledger and retains incomplete windows. These tests qualify
observation, not original firmware, fitted timing or a hardware speedup.
