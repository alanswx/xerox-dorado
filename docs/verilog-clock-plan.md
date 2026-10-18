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

The original FPGA baseline ceiling is 4.167 M instructions/s at 33.333 MHz/SYSPER4,
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
a20% instruction-ceiling increase. Its fit/timing and actual firmware/login
now pass; Viewers/input qualification continues.
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

The new placement passes29 checks, minimum+0.019ns, core setup+1.499ns,
zero illegal/unconstrained clocks. Exact MemD h15 and MemC j07 own F10145A
write-address bit1 holds were reviewed (each1source/4sinks), preserving
setup/data/enables/reads. Fmax42.55MHz. Original firmware/BFN113 and Cedar
login work on MiSTer14; fast Guest input is accepted, real Basic.Loadees
retrieval underway. Viewers and full interaction remain pending. The fitted
RBF is `a963f81c353b240a5528bdffb1b1d6c43df3dd0e8aa37afed38e142292337acf`.

`DORADO_SYS_MHZ=40 sh fpga/mister/build.sh ...` now selects that PLL
frequency; the default remains33.333333. The assembler records the chosen
value in `dorado_clock_config.json`. Shell syntax and a temporary template
fixture pass for both frequencies; invalid/nonfinite/zero frequencies are
rejected before modifying output. Fresh timing and hardware checks remain
mandatory for any selected frequency.

## Passive throughput measurements: Native30 candidate

Native30 retains Native29's40MHz/SYSPER4 and adds passive enabled/host fabric
edge counters plus coherent command70 debug snapshots. It is a separate
frozen build at `misterubuntu:/home/alans/mister/Dorado_Native30_20261018`;
Native29's build inputs remain unchanged.

`make -C verilog speed-monitor-test` compares enabled counts with an actual
gated clock through pauses/reset/wrap. UIO tests at512 and1024 words verify
that changing both debug values/status during the reply cannot tear the
observation; the pre-change implementation fails this regression.

These counters will separate global backend pauses from CPU clock division.
They count fabric edges, not instructions. See the
[protocol/measurement notes](../fpga/mister/main/README.md) for units and
wrap handling. A read-only Native26 desktop scaler-header baseline changes
1.70 times/s over10s in one fixed scaler-buffer header. Buffer reuse may
skip complete fields: this is only that slot's header-change cadence, not
a source-field, CPU or HDMI frequency. Do not infer retired MIPS from it.

## Measured arbitration bottleneck: Native31 at50MHz

Native29's new limiting path takes22.666ns from an MSA address, through
readiness and the128-way arbitration/payload selection, into address_latched
(25ns relationship,1.499ns slack). The old scan rotates a variable-index
request lookup, selects an owner, then performs a second large payload mux.

The replacement in `dorado_storage_arb.sv` uses a balanced tree carrying
the selected owner/address/data/write together. At each node, requests at
or above next_group take precedence; within each class the lower index
wins. This preserves the exact cyclic order and the original outstanding-
request FSM, including reset, read-before-write and response backpressure.

Independent cyclic-scan scoreboard: all masks/pointers at1/3/7 groups, plus
all128 pointers with random masks (8,371 transactions). Payload stability
through request/response stalls passes. Three-group array/reset and complete
MSA14/16-bit geometry comparisons pass, including all2,359,296/9,437,184
stored bits and outputs. Quartus17 required explicit generate/genvar syntax;
the corrected source synthesizes. No speed gain is inferred from RTL alone.

Frozen Native31 at `misterubuntu:/home/alans/mister/Dorado_Native31_20261018`
combines this tree with Native30 diagnostics at50MHz. Fit succeeds in15m05s,
84% ALMs,305 RAM blocks. All29 STA checks pass, min+0.080ns, core setup
+1.210ns,0 illegal/unconstrained clocks. Both fast/slow hold reports pass;
no new post-fit exceptions. Worst path is now MSA readiness -> network_step,
7.673ns data against10ns; Fmax56.88MHz is not an operating qualification.
The original EccComputeErr inferred-latch warning matches Native29.

Loaded on MiSTer14 around03:48UTC; original firmware/BFN113, Cedar login
and fast Guest input pass. Actual HDMI was restored via Main host-FB toggle;
fresh counters confirm1080p30/74.25003MHz. Viewers/input qualification pending. A20s firmware count sample gives
26.4617% enabled edges and50.01MHz host clock. This halted-CPU workload is
not Cedar performance. Ceiling6.25M/s before pauses,37.5% of original Dorado;
50% above33.333MHz is a ceiling comparison, not measured guest acceleration.
Evidence: `fpga/mister/reports/2026-10-18-native31/`.
The original clock-family migration and unused DDR return lanes remain.

### Original-board physical budget

A separate slow-corner destination audit now measures processor, memory and
I/O cones independently of backend arbitration. Worst processor path is
ContB b24.qb (ASEL0) -> ProcL k22.d_pre[5] (DMuxData),17.182ns over21 logic
levels, with+2.527ns margin against20ns. Memory/I/O copies of that DMuxData
capture take17.172/16.772ns. These continuously sampled setup copies are
part of the existing chip translation; they are not new original-hardware
pipeline stages. Merely moving the readiness gate would not qualify a
66.667MHz fit. The enable/latch migration must cover diagnostic/data-setup
sampling as well as the main T/task path. Do not cut these paths without a
proved replacement contract. Reports: native31/board-paths.tcl and reports.

## Backing storage: optional line buffer started

Native31's real Cedar loader sample gives16.9255% enabled fabric edges;
83.0745% are global pauses (50.0034MHz host). These are not instructions.
This justifies using all four16-bit lanes from a raw64-bit DDR read before
attempting another PLL increase.

`dorado_storage_ddram.sv` now has an optional128-line buffer (default OFF).
Full tags prevent false hits; writes invalidate and still wait for DDR;
reset invalidates and drains outstanding responses. All original DRAM/
ECC/cache behavior stays upstream. Independent scoreboard and full board
comparisons pass, including all9,437,184 bits. Three-group DDR reads drop
4449 ->509 with identical1505 chip cycles/463writes. Cache-disabled and
missing-invalidation failure controls prove the traffic/coherence tests.

A separately frozen Native32 candidate enables the buffer at50MHz/SYSPER4.
Quartus synthesis passes and retains both RAMs; fit/timing/hardware pending.
Native31 remains the live control. Evidence/reproduction:
`fpga/mister/reports/2026-10-18-ddr-linebuffer/`. No hardware speedup claim yet.

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
