# Native23: physical signed-overflow detector

Only functional RTL change from Native21: ProcH d13 MC10170 selects
LEGACY_B_INVERT=0, the documented XOR. The generator emits the override.
The old extra inversion reverses ContA's Overflow-prime condition used by
Mesa JLB. CompactVM consequently takes the less-than path on equality,
vacates its own VP3e02..3e05, and the next local-frame store faults.

`make -C verilog/verilator signed-compare-test SYSPER=4` passes 256 arithmetic
and branch vectors; a frozen pre-fix ProcH fails 0-0-1 (the JLB equal case).
The test drives the physical detector's five inputs, with independently
computed signed-wide arithmetic, then uses ContA c21's gate function.
Processor compute, module-switch and map-buffer-hold regressions also pass.

Diagnostic167M->170M replay preserves all four germ map entries and avoids
the old fault/queue loop. At169M FaultInfo remains0003 (old4080), with the
original germ entries intact, and CPU executes page flush/copy work.
The replay changes only the combinational detector function. State migration
explicitly verified all POD field names/types and offsets/sizes (only generated
internal temporary hashes differ), preserving every payload byte. See the
audit. It is a diagnostic, NOT a cold or FPGA boot qualification. No C state.

Fresh cold DDR220M component run is in progress, with the actual adapters,
clock pauses, original Initial/Cedar images and disk controller. An initial
scratch invocation used default SYSPER16, was stopped and preserved as
cedar-native23-sysper16-rejected.log; qualification restarted atSYSPER4.
Quartus Native23 completed on misterubuntu, original Native21 clock ratio4.
The old placement-specific e04 exception was removed before fitting. Actual
reported failures are again exactly ContA e04 a_lat[1] to its own three inferred
write-address bit1 registers. The cell captures a_lat only !p13 and commits
mem[a_lat] only p13 && !we_d && ce_lat. The exact1-source/3-sink hold-only
exception is reapplied with cardinality assertions; setup/read/data/enables
stay checked. All29 timing checks then pass, minimum +0.097ns. Clock audit:
zero illegal/unconstrained clocks. External I/O coverage is reported separately.

Native23 image SHA89726440d5d599a5b01428e4f29b54d3969064508191016e100375746ff765e2
was programmed on MiSTer14 with a verified fresh pack. Original firmware boot
is in progress. Focused regression and diagnostic replay pass; full cold220M
qualification remains active. No FPGA Cedar OS claim yet.
