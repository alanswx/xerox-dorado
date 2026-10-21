# Native register capture and point diagnostics

This immutable running-prefix receipt preserves699 members /597 blobs,525
shared from the preceding control prefix. Both exact native simulator PIDs
were checked live at collection. Neither had reached the Cedar request/trap.
Verify using `python3 tools/archive_r_front_ready_followup.py --verify DIR`.

The candidate's read-only LLDB logger now includes actual RM/STK arrays,
saved/live event inputs, evaluator Q/next values and the common root clock.
The baseline logger reads its original physical SRAM packages and T/R latch
Qs instead. Both derive logical RM words using the physical bit/address
mapping already checked in the original-image fixtures. Layouts come from
the exact generated headers; no inferior function calls or guest writes are
used. Optimized module-port placeholders remain excluded from interpretation.

Both loggers have a callback at the existing first MTrap display call, plus
their periodic progress callback and the existing finish breakpoint. The
macOS libc++ format-string layout was independently calibrated for short/long
strings and recognized the real progress format. No first-trap callback had
fired at collection. A control callback initially selected the wrong positional
ABI and stopped on a Python exception; its rejected source, correction and
same-process recovery are retained. `reason` is now keyword-only. This was a
host logger error, not a simulated device/CPU failure or a simulator restart.

`check_native_front_observation.py` independently evaluates captured inputs in
Icarus with the exact frozen component and physical RAM inputs. The440M and450M
observations reproduce all752 calculated next-state bits. Events are0/33807,
repeated events0/0 and unsettled false/false. A deliberate expected-state bit
flip in an offline copy is rejected with exit1. The checks expose diagnostics
otherwise unused in the native netlist; they are **not an independent oracle
for the processor's behavior** and do not cover Cedar or consumed memory data.
The first tool version and its executed inputs are also retained.

No FPGA, Main, daemon, original firmware, clock or CPU RTL changed. Desktop,
speedup, persistence and authentic FPGA Checkpoint/Rollback remain unqualified.
