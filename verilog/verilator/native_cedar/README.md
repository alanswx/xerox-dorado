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

The `guest` progress counter counts enabled **fabric edges**, not Dorado
microinstructions and not MIPS. The diagnostic retains a 4,096-sample physical
operand/PC ring, dumps the first observed task-0 trap at CIA221 after113, and
stops after a persistent RequeueSearchSQ sequence or the configured host limit.
Neither stop reason establishes firmware failure or desktop success by itself.
This diagnostic excludes MiSTer sys_top, Main, Linux, direct-DDR transport and
HDMI capture. Complete FPGA snapshots remain deferred.

`test_host.cpp` sends a captured physical Cedar113 request and independently
compares the returned payload against the original EB after its 512-byte header.
Its recorded result is packet-adapter evidence, not a native firmware boot.
