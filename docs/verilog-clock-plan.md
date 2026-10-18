# Dorado RTL clock implementation plan

Started 2026-10-18 UTC. Native26's successful Cedar desktop is the hardware
reference. The user has released MiSTer14 for reloads/faster builds; keeping
that session alive is no longer a constraint. See the [clock audit](verilog-from-sil.md#clock-architecture-audit-2026-10-18)
for measurements and the [handoff](verilog-handoff.md) for deployed artifacts.

## Objective and constraints

Restore the original throughput and clock relationships: HM section 2.3
defines a 30 ns clock, a new ordinary instruction every two clocks (60 ns),
and three overlapping execution cycles. IM/TPC instructions extend the
control sequence; those extra clocks are architectural behavior.

The current ceiling is 4.167 M instructions/s at 33.333 MHz/SYSPER4,
before backend pauses. Simply reducing SYSPER is invalid: the existing
generator distorts the original phases at 4 and collapses them at 2; the
paired original-ROM startup passes at 4 and fails at 2. Simply increasing
the PLL is also unqualified: deployed Native26's Fmax is 41.08 MHz, limited
by storage-ready logic into the falling-edge network_step register.

Two changes are needed: remove avoidable discrete delays in the sampled
chip/clock translation, and reduce storage stalls/control-path delay.
Keep these experiments separate so each result has an attributable cause.
Preserve ECC, read-before-write behavior, real I/O completion and hardware
error reporting. No blanket timing cuts or guest-state repairs.

## Work sequence and acceptance gates

| Stage | Work | Acceptance before proceeding |
| --- | --- | --- |
| 0. Baseline | Preserve Native26, original-ROM4/2 comparison, clock-phase audit, and deployed-fit critical paths. | Saved recipes/results identify the working control and failing faster setting. Complete. |
| 1. Phase contract | Derive clock families from original schematic connections. Build an independent divider reference and a direct-enable prototype. | SCP'/ECP levels, edge order, 1/8-period lead, 3/8-period j02 low window, pre-edge pipeline capture, and scheduler freeze/resume pass. First prototype complete; board qualifications remain. |
| 2. First board cone | Extract ProcL's SHCP'C/T-enable cone and its ProcH counterpart. Translate captures into explicit enables, including their setup data; keep the old implementation selectable in an isolated harness. | Both halves of T agree for LC/Md/hold/task transitions; original SendMIR/Boot0/compute and original-ROM startup pass. Do not copy PRECAP/clock-lag exceptions without re-deriving their ordering. |
| 3. Remaining families | Translate task/Link/IM/TPC control, then Map/cache/IFU and latch/write windows. Retain original extended instruction and RunRefresh behavior. | Architectural-boundary state comparisons, six hardware error checks, storage equivalence, cold Initial/Cedar and disk read/write gates pass. |
| 4. Backing storage | Count pause causes, retain useful lanes from each64-bit DDR return, and shorten the address/readiness-to-pause path with a proved handshake. | Read-before-write/ECC and real disk traffic stay correct; measured stalls decrease; STA checks the actual pause/release relationships. |
| 5. Timers and implementation | Reconcile MC12061, BaseBoard, display, Ethernet and disk rates with the new timebase. Choose a realizable PLL/enable architecture and fit it. | All setup/hold/recovery/removal and clock checks pass; original firmware boots; measured retired instructions and paused time support the speed claim. |
| 6. Hardware | Load a separately named qualified core on MiSTer14 after preserving the current session/artifacts. | Cold Cedar desktop, exclusive keyboard/mouse test, sustained network/disk traffic, stable video and measured throughput. |

Stage2's ProcL/ProcH T controls are now translated and tested in isolation;
the capture-enable sources and full T data/latch path remain to be derived.
Their clock gates and data setup must be derived together. In parallel,
Native29 tests a conservative40MHz PLL with the existing SYSPER4 transform:
a20% instruction-ceiling increase, pending a fresh fit/timing/hardware gate.
This is an incremental speed step, not the full-speed clock redesign. The
existing Executive executable/checkpoint are protected; use separate object
directories and never `+frame` against the golden artifact.

## T-control extraction: implemented, isolated

The original sheets have different capture nets: ProcH h22 uses Clock0'Da;
ProcL d17 uses SHCP'C. Their RbSelMd/RbSelMd' output pins are also swapped.
`tools/check_t_control_wiring.py` checks24 pin bindings per board, package
types, the tied LC/FF075 drivers and write-enable connections directly from
the original Xerox wire lists.

`verilog/rtl/dorado_t_controls.sv` accepts a separate explicit capture enable
for each half and samples its LC/FF/task-comparison setup inputs directly.
`make -C verilog t-controls-test` compares it with an independently clocked
pin-level original gate cone over all8 LC values,256 FF values and4 capture
masks:8,192 comparisons. It separately checks8,164 legal combinations
against HM3.8/Table10, preserves both banks through2,048 no-capture cases,
and rejects late input pass-through. A universal-enable mutation disagrees
in4,096 cases. No reset has been added to the original resetless controls.
Strict module lint passes.

This proves the decode/capture interface **given correct enables**. The test
does not derive those enables from BaseBoard/processor clock gates, model
all T data latches/task files, or qualify firmware with this replacement.
The module remains outside the machine and FPGA source list. Stage2 is
therefore incomplete, and its SendMIR/Boot0/compute/ROM gates are pending.

## Incremental hardware candidate: Native29

Frozen build: `misterubuntu:/home/alans/mister/Dorado_Native29_20261018`.
It clones Native28 and changes only the PLL output from33.333333 to40MHz;
CPU/board/DRAM logic and SYSPER4 are retained. Native28's host key queue
and passive fault diagnostics are included. Ceiling5.0M instructions/s,
before storage pauses, versus4.167M on Native26.

The existing Fmax measurement motivates this candidate; it does not qualify
the new placement. Review all actual clocks/setup/hold/recovery/removal and
unconstrained/illegal clocks, and re-derive any exact own-RAM-address hold
exceptions from the new endpoints before programming. Then cold boot from
the corrected immutable disk seed, exercise keyboard/mouse, and measure
actual progress/pauses separately from the nominal instruction ceiling.

`DORADO_SYS_MHZ=40 sh fpga/mister/build.sh ...` now selects that PLL
frequency; the default remains33.333333. The assembler records the chosen
value in `dorado_clock_config.json`. Shell syntax and a temporary template
fixture pass for both frequencies; invalid/nonfinite/zero frequencies are
rejected before modifying output. Fresh timing and hardware checks remain
mandatory for any selected frequency.

## First prototype: implemented, isolated

[`dorado_clock_phases.sv`](../verilog/rtl/dorado_clock_phases.sv) describes
eight positions of the original BaseBd11 divider. It generates SCP', ECP
and their BaseBd12 j02 OR, plus combinational enables announcing the
transition at the upcoming master edge. A consuming register samples its
pre-edge D directly, instead of detecting a clock transition one master
tick later and compensating with d_pre. Reset selects an arbitrary rotation
of the steady-state divider; it does not emulate analog power-up.

The source is not included in the native core, generated boards or Quartus
project. Its level outputs are diagnostic logic, not routed FPGA clocks.
`advance` freezes the experimental scheduler; it is not a replacement for
the original processor Hold or the independently running memory clocks.
Debug-clock pulses and board-specific gates are not implemented here.

The regression uses an independent event-driven model of the four original
flip-flops: g04/h04 on one VCO edge, g03/h03 on the opposite edge. It checks
every level/edge at one, two and three master ticks per eighth-period slot,
the pulse widths and edge counts, freezing at every phase, and a two-stage
pipeline's simultaneous capture of old upstream data. An actual MC10176
from the current cell library is the failure control: it captures a tick
late relative to the reference. This demonstrates the mechanism; it does
not establish that changing one cell will fix the whole machine.

Reproduce from the repository root:

```
make -C verilog clock-phases-test
make -C verilog clock-phase-audit
verilator --lint-only -Wall --top-module dorado_clock_phases verilog/rtl/dorado_clock_phases.sv
```

**This eight-slot prototype is a timing oracle, not the final speedup.**
Literal implementation at original speed would require a 266.667 MHz
master clock, unsupported by the current fit. The subsequent cone work
must establish which transitions can share a fabric edge using explicit
pre-edge state and which require ordered or related PLL phases. Neither
66.67 MHz/SYSPER2 nor a fixed number of final phases is promised yet.

## Clock-family contract register

This table distinguishes established relationships from integration work
that still needs to be proved. Generated identifiers are navigation aids;
original sheets/manual remain the references.

| Family | Established source/relationship | Remaining translation work |
| --- | --- | --- |
| BaseBoard phases | BaseBd11 g04/h04/g03/h03; SCP'=g04.Q', ECP=h03.Q. ECP leads SCP' by 1/8 clock. BaseBd12 j02 OR, with debug pulse inactive, is low for 3/8. | Add debug pulses and fanout qualification; no analog-delay claims. |
| Run and memory enable | ContA l02–l06 generate processor/memory enables and RunRefresh. HM5.12 requires memory clocks during processor halt when RunRefresh is set. | Preserve asynchronous set/reset, step and stop behavior. Do not use global scheduler advance as processor Hold. |
| Instruction phases | ContA l07: Phase0/Phase1 depend on StartCycle/sPhase0 and clk2'Bf; HM2.3 defines ordinary/extended timing. | Preserve feedback and IM/TPC extension; do not toggle t0/t1 unconditionally. |
| T and task setup | ProcL20 d17 MC10176 clocks on SHCP'C; ProcH20 h22 on Clock0'Da. Both produce TbSelMd/write enables from LC/FF and task comparison. Processor Curr/Next stages also use SHCP'C/Clock1'. | Control decode/capture interface passes8192 cases. Enable-source derivation and complete T setup/data/task ordering remain. |
| Map latch/MCR | MemX f01/g01 MC10175 follow Mapbuf while MapbufLd' is low; f04 F10016 consumes Mapbuf at Clk1'Ab. | Preserve transparency before close and the consuming edge. Current clock lag/PRECAP values are compensation, not the physical specification. |
| RAM writes | F10145A captures address/data in the active write window; existing native fit has narrowly reviewed own-address hold exceptions. | Keep old-data reads, write commitment and setup checks; re-review every new placement. |
| Peripheral clocks | K1115A rates depend on SYSPER; MC12061's fabric/8 currently mis-scales the BaseBoard clock. | Qualify prepared timer correction against original firmware and measure terminal/Ethernet/disk/Junk rates together. |

Evidence for this first implementation lives in
`fpga/mister/reports/2026-10-18-clock-phases/`. Board/firmware equivalence,
FPGA timing closure and hardware speedup are still pending.
