# Explicit RBase closing capture

Status: **PASS_BOUNDED_RBASE_CLOSING_CAPTURE**. The component and converted
RBase/StkP directed cases pass. Native phase generation/integration, full
processor arrival coverage and FPGA timing remain open.

`verilog/rtl/dorado_rbase_window_step.sv` retains the original64 ProcL g15
F10145A bits externally. While the write window is open it follows the
arrived address/data. When the window closes it captures the pre-transition
address and g14 data, before a19 exposes the next task. Closed windows retain
the file. There is no reset, extra file, pipeline state or architectural clock.
This is the synchronous capture rule needed for native implementation;
native scheduling and timing still have to supply the correct epochs.

`rewrite_rbase_window.py` inserts that helper into the existing local R-front
adapter. The original cone supplies before/after write gates, physical address
and latched data. Every other source change reverses byte-for-byte to its
parent. The physical state map and9216 RM/STK bits are unchanged.

## Evidence

The original RAM oracle is unchanged. Its standalone waveform keeps address
stable before/during the write and15ns after close; write pulses exceed30ns.
An independent monitor checks the F10145A's specified address setup/hold,
minimum pulse width and data setup. All16 addresses and all16 data nibbles
are written. Final data also settles between sampling events, so omitting
the final closing capture cannot pass by relying on an earlier open write.

Both engines pass768 comparisons,256 closes with a different next address,
256 open-data changes and256 independently checked legal write pulses.
Writing the next address at close and omitting the final capture fail at
comparison2 in BOTH engines (four negative cases).

The original directed RBase/StkP stimulus now drives actual ContB dASEL,
dBSEL and dLC inputs rather than raw processor control pins. Real MIR clocks,
shared CP transactions and actual task-selection logic determine their arrival.
The cases fill every RBase slot with both nibble extremes (32 fills), then
load/check all256 StkP values. TaskingOff/On is stored by real instructions.

| Profile | Icarus | Verilator |
|---|---|---|
| balanced | pass | pass |
| fast_gates | pass | pass |
| later_control | pass | pass |
| parc_parity_min | pass | pass |
| parc_parity_max | pass | pass |
| later_r_memory | pass | pass |

Each run compares844 physical Qs,9216 RM/STK bits,100 clock channels,94 control
flags and existing functional outputs over10121 events:242 writes,4166 reads,
391 old/new port epochs. Clock counts agree between engines within profiles.
Five channels43/91/92/93/95 remain unexercised. The other original tasking/
LOW-NoDispatch directed cases still need conversion to actual producers.

## Retained limits

The [earlier stress discrepancy](../../2026-10-23-r-front-control/experiment/README.md)
still reproduces at1456 on Q644..647 in BOTH engines with the helper enabled.
The zero-delay oracle writes the next task during a0.5ns address/write overlap;
the explicit native capture rule retains the old transaction. Physical RAM
behavior during that overlap is unqualified. Do not relabel it as a passing
full arrival gate or change/filter the oracle to hide it.

Next: wire this pre-transition capture into the native schedule within the
original60ns cadence, and meet the full15ns FPGA budget. Retain the native
visible-R latch ownership established by the earlier full-image integration:
bits572..587 belong to the original g06/g07 latches, not a second predicted
copy. A844-Q native slice would therefore have828 evaluator-owned bits plus
those16 original physical bits. Preserve current Q maps and original startup.
Close missing clock/tasking coverage and IM/parity/MCR/hold-data/DMD/B-bus
sources, then run native ROM/timing/timer/SDRAM and full A-F acceptance gates.
No Main, daemon, volume, hardware clock, loaded core or speed result changes.

## Reproduction

```
python3 tools/test_rbase_window.py --output /tmp/dorado-rbase-window --engine verilator
python3 tools/test_r_front_control.py \
  --base build/rtl-executive/r-front-ready-qualified-v1-20261020 \
  --reference build/rtl-executive/r-front-control-reference-v2-20261023 \
  --output build/rtl-executive/dorado-rbase-directed-fresh \
  --engine verilator --rbase-close --actual-directed
```

Use `--wrong-close`/`--omit-close` for the primitive negatives; use
`--profile early_refresh --rbase-close` for the retained full-graph failure.
The review command is `tools/review_rbase_window.py --output /tmp/dorado-rbase-review`.
The lossless receipt has4775 members/475 blobs/1975730 compressed blob bytes.
It retains the weaker initial tests, both full pilot passes, twelve directed
passes, required failures and exact compiled sources/adapters/cones/logs.
The first archive aggregation selected its review directory as a simulation;
that selector was corrected without rerunning tests, and its completed review
is preserved under `prior-review/`.
