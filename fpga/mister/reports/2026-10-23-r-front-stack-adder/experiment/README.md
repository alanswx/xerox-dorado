# Stack-adder equivalence passes; combined 15 ns timing is rejected

ProcL j17/k17/l17 are three MC10182 slices wired for six-bit addition with
LOW mode/select/initial-carry inputs. The generated candidate replaces only
these combinational instances with native arithmetic, retaining all eight
connected output/carry nets. Original reference cells, architectural state,
RAM ports and capture epochs are unchanged. The candidate was not promoted.

The evidence classes are separate:

- All 4096 generic operand pairs and all 512 combinations of the actual nine
  independent physical inputs compare in Icarus and Verilator. Four broken
  carry-boundary cases fail after successful compilation.
- Two new complete processor comparisons pass 13515 events each, with 328
  write-active events, 5608 read-active events and 797 old-parity epochs.
  All 792 state bits, 9216 RAM bits, 68 clocks and functional ports compare,
  including the real LOW-root NoDispatch MIR capture.
- The existing twelve profile/engine runs and two frozen-clock failures are
  inherited through the exhaustive binary combinational equivalence bridge.
  They are not twelve new delay-profile runs or analog/glitch equivalence.

The source comparison verifies identical top, constraints, project source
ordering and all 24 RTL dependencies. Only the compiled combinational cone
changes. This isolated arithmetic-only comparison produced worse timing:

| Condition | Setup / evaluator setup | Hold | Minimum pulse width |
| --- | ---: | ---: | ---: |
| Slow 1100mV 100C | -2.476 ns | +0.353 ns | +6.468 ns |
| Slow 1100mV -40C | -2.783 ns | +0.271 ns | +6.467 ns |
| Fast 1100mV 100C | +4.625 / +5.696 ns | +0.196 ns | +6.182 ns |
| Fast 1100mV -40C | +4.915 / +7.074 ns | +0.138 ns | +6.117 ns |

3432 ALMs, 3000 registers, exactly four 256x9 MLAB banks. RAM initialization
is None; reads remain asynchronous/unregistered. All functional parity/fault
endpoints, the exact six-line SDC and all four conditions remain checked.
The previous mask fit missed setup by 1.996 ns; this fit misses by 2.783 ns.
Mapping/routing also change, so neither a better adder delay nor a hardware
speedup is inferred from a lower logic-level count.

The worst path has 18 levels and 17.466 ns data delay:

```
launch[636] = before_clock (event evaluator's prior root level)
 -> capture detection -> Control-A WIM -> MIR/low-front selection
 -> functional rm_l MLAB read -> parity_ok[3] = result[246]
```

At 100C, after_controls[290] (CP bus bit 8) reaches faults[3] through the
control/capture and rm_l path in 17.147 ns. These paths are different from
the preceding StopAtT1 -> stack-read path. The next structural work must
establish the original MIR/register-read/capture boundaries, preserving
late CP changes and old/new epochs. HM section 2.3 places MIR at t0 and the
selected RM/STK read capture at t1, one original 30 ns clock later; that
architectural interval alone does not establish a safe FPGA partition.

All four Quartus stages returned zero in
`/home/alans/mister/Dorado_StackAdder_1_20261023`; builder 1011033 and tool
PIDs 1011045/1012870/1019949/1021081 are absent. Exact terminal source readback
allows only Quartus's version annotation. No assembly or FPGA load occurred.

The reader rejects 20 damaged/rehashed timing artifacts and six damaged
arithmetic-proof records. Older mask rejection/20 controls and Ready pass/20
controls reproduce. Timing-negative controls are omitted for the already
rejected baseline; malformed evidence must raise structurally.

Both preparation versions are preserved: v1 had the correct RTL but omitted
the new driver snapshots from its source ledger and was never fitted. v2
binds the executed driver/rewrite sources and retains inherited sources
separately. The earlier generic-only unit run is also preserved.

This receipt contains 6032 members in 507 deduplicated blobs (2,985,244
compressed bytes). Verify lossless readback with:

```
python3 tools/archive_r_front_ready_followup.py --verify \
  fpga/mister/reports/2026-10-23-r-front-stack-adder/experiment
```

Hardware 5c65, Main, the daemon and volumes are unchanged. Original 60 ns
native timing, remaining real producers, physical SDRAM clock crossings and
IO, Cedar boot comparisons and the full acceptance/stress requirements remain
open. No firmware or hardware speed qualification is claimed here.
