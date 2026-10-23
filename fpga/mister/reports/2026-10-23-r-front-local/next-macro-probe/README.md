# Actual MIR-driven NextMacro producer probe

The original ContA e22 package drives IFUNext'a from two wired-OR outputs.
One combines JCN.6'/7'/5'; the other JCN.1/2'a/0. Existing d21 inverts their
result into NextMacro. All these inputs are existing physical MIR Qs, so this
closes a producer without adding state, a cycle or a RAM copy.

`dorado_r_front_next_macro_step.sv` differs from the qualified792-Q wrapper
only by module name and that producer expression. The generator inverse-checks
the delta against parent SHA256
`0337c3990e0ccd051f14108d1af22d1ea7a5d66cc7337b6c50ba8ef8a1066982`.
New candidate SHA256:
`cac2c60601194c10f24ba785d620c84ce2d961b32c9682af4d2f6a9670f6a733`.
JCN.2'a comes from d24.B's complemented output, distinct from d23.B's copy
used by RWTPCorRWIM. The candidate keeps the closing HIGH, transparent LOW
and settled source epochs inherited from its parent.

The independent reference adds actual e22 package wiring to the original
ContA graph. It removes IFUNext'a as an external boundary, preserving all
other pin indices. Obsolete raw slot313 still varies independently in the
test. No candidate equation supplies the reference producer.

Both Icarus and Verilator pass the full balanced-profile sequence:11854
events,318 write-active events,5012 read-active events and801 old-parity
capture epochs. All792 Qs,9216 RAM bits,68 clocks and functional outputs
compare; all100 front Qs and16 Ready Qs change. This includes real MIR/CP
RBase/StkP commands, HIGH-/LOW-root maintenance, late tasking/branch capture,
old RAM parity, write/read nonoverlap and write-window address/select checks.

Using the obsolete external IFUNext'a instead fails resolved-source comparison
at check6221 in BOTH engines, after successful compilation. All four runs/
eight actions are terminal. The different807->801 parity-epoch count relative
to the parent comes from the actual producer changing the exercised address
sequence; both engines compare the entire sequence to the new original graph.
This is not a speed measurement.

Reproduce with an unused output directory:

```
python3 tools/test_r_front_next_macro.py \
  --base build/rtl-executive/r-front-ready-qualified-v1-20261020 \
  --output NEW-DIRECTORY --engine verilator
```

Repeat with `--engine icarus`; `--mutation` deliberately substitutes the
obsolete source and must fail. The source generator can emit a new candidate
with `--candidate NEW-FILE` and/or original graph with `--reference NEW-DIR`.
Verify the lossless receipt with:

```
python3 tools/archive_r_front_ready_followup.py --verify \
  fpga/mister/reports/2026-10-23-r-front-local/next-macro-probe
```

This probe covers one illustrative delay profile. Extended arrival/skew,
other branch/d17 producers, actual ContB IM/control and MemC Hold/PRhold,
physical timing and original-ROM native integration remain open. The previous
combined15ns fit remains REJECTED. No new fit, timing exception, assembly,
FPGA load or speed gain is claimed.
