# Terminal15ns MIR-mask implementation budget: REJECTED

The source-bound generated MIR-mask candidate passes the
[bounded original-graph ordering gate](../ordering-budget/README.md), but
the combined implementation still fails15ns. All functional parity/fault
results remain registered. No timing waiver, assembly or FPGA load occurred.

| Condition | Setup / evaluator setup | Hold | Minimum pulse width |
| --- | ---: | ---: | ---: |
| Slow1100mV100C | -1.996ns | +0.386ns | +6.467ns |
| Slow1100mV-40C | -1.758ns | +0.303ns | +6.471ns |
| Fast1100mV100C | +4.615ns / +5.609ns | +0.166ns | +6.180ns |
| Fast1100mV-40C | +4.925ns / +7.064ns | +0.137ns | +6.116ns |

3391 ALMs,3001 registers and exactly four256x9 MLAB banks/9216 bits. RAM
initialization remains None, asynchronous reads remain unregistered, and
the qualified nonoverlap/capture-epoch contract remains in place. There are
no M10K blocks, DSPs or PLLs in this isolated virtual-I/O fixture.

The worst path has19 levels and16.683ns data delay:

```
launch[1390] = globalQ753 = ContA j02.Q1 / StopAtT1
 -> d_stop / control normal0 resolution
 -> actual MIR FF/JCN mux
 -> stack pointer/address carry and mux
 -> functional stk_l asynchronous MLAB read
 -> R parity -> faults[3] = result[242]
```

The launch maps to the actual original StopAtT1 maintenance register, not a
BaseBoard address interpreted as a PC. The previous786-Q fit's worst path
started at BNT and took17.633ns across22 levels. The current circuit includes
additional producers, so this is not an isolated optimization A/B, a claimed
MHz increase or a Cedar throughput result. Both rejected fits are retained.

The build completed in isolated
`/home/alans/mister/Dorado_RFrontMask_1_20261023`. Builder839339 and tool
PIDs839351/841068/848403/849369 are all absent. Every stage returned0; this
does not imply passing timing. Terminal source readback accepts only the
known Quartus17.0.2 version annotation. Candidate SHA256 remains
`e695318064d80a9e308ff5e76e3fd57d86bb55412a448b163da135856883dd0e`;
top SHA256 remains
`70dfecbde9d5cd3cad02b9d7670bebd5305c645600f66a06aa470bbdcf4b774b`.

The reader checks all four conditions, targeted launch/result paths, virtual
I/O constraints, exact project source list, unchanged qualified dependencies,
RAM inference and dimensions. Twenty damaged/rehashed-evidence controls
reject structurally. Negative timing controls are omitted because the baseline
already fails timing; malformed evidence must raise rather than merely return
REJECTED. The older Ready PASS/20 controls and786-Q REJECTED/19 controls still
reproduce unchanged after the reader extension.

Verify this lossless receipt:

```
python3 tools/archive_r_front_ready_followup.py --verify \
  fpga/mister/reports/2026-10-23-r-front-mir-mask/timing-rejected
```

Next derive the original control/MIR and data-phase capture boundaries to
shorten this remaining functional path. Preserve late NoDispatch captures,
old/new data epochs and all parity/fault endpoints. Actual native producers,
original60ns/timers, physical memory IO, boot and final stress remain open.
Hardware5c65 speed and volumes are unchanged.
