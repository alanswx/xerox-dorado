# First direct phase/enable prototype — 2026-10-18 UTC

Plan: `docs/verilog-clock-plan.md`.
Implementation: `verilog/rtl/dorado_clock_phases.sv`.
Regression: `verilog/verilator/tb_clock_phases.sv`.

Original references: HM2.3, BaseBd11 g04/h04/g03/h03 and BaseBd12 j02.
The independent reference is event-driven original flip-flop wiring, not
the current oversampled cell chain or the DUT's phase table.

Commands from the repository root:

```
make -C verilog clock-phases-test
make -C verilog clock-phase-audit
verilator --lint-only -Wall --top-module dorado_clock_phases verilog/rtl/dorado_clock_phases.sv
```

Results: all three commands exit0. Strict prototype lint has no warnings.
The historical audit still emits its existing unsigned-comparison warnings
for N=1 and confirms the quantized small-N phases. See its earlier detailed
report at `../2026-10-18-clock-audit/`.

| Ticks per eighth-period slot | Complete periods | Captures | Legacy MC10176 observed late |
| --- | --- | --- | --- |
|1|72|72|36|
|2|36|36|17|
|3|24|24|10|

Every case matches all SCP'/ECP/j02 levels and edge enables, the three-slot
low window, one rise/fall per period and pre-edge two-register pipeline
capture. Every case pauses at all eight phases without duplicate captures
or lost phase. Legacy late counts are input-dependent instances of the
known extra-tick delay, not whole-machine error rates.

No machine uses this prototype. No generated board, production clock
logic, RBF, PLL, live Cedar memory or input state changed. The old CLOCKGEN
received comment/unit corrections only. Native28's independently running
keyboard-buffer build finished fit/STA/audit; its timing review is pending
and it has not been deployed.

Limits: ideal nominal divider, arbitrary steady-state rotation, no analog
delays or power-up model. Board gates/debug pulses, latch windows, extended
IM/TPC sequences and original RunRefresh/stop semantics are pending. A
literal eight-slot full-speed scheduler needs266.667MHz; the final FPGA
architecture must be derived and timed separately. This is not a claimed
hardware speedup or a replacement for boot/regression gates.
