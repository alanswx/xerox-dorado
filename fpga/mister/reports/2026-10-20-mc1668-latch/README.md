# MC1668 is a high-transparent latch

The old cell incorrectly treated the MC1668 as an edge-triggered flip-flop
with set/reset dominating its clock. Motorola's 1978 data book, PDF page 235 /
printed 4-30, specifies a **dual clocked latch**: while C is high, Q follows D
regardless of S/R. While C is low, S/R act or Q holds. Closed S=R=1 is undefined.
PARC's timing dictionary independently marks both data arcs as high-transparent.

The corrected `cell_MC1668.v` follows that truth table at each fabric sample.
It retains deterministic set priority for the undefined closed S=R=1 case;
that case is excluded from qualification. No reset or pipeline stage is added.
The single-page original sheet is `DoradoDocs/datasheets/MC1668.pdf`.

Both Icarus and Verilator pass 1,425 observations of the manufacturer table:
1,312 D changes while C stays high, 176 closed-hold checks, 1,056 observations
with clock priority over S/R and 768 with independent half clocks. Four compiled
bad variants reject: edge-only updates, wrong transparent level, set priority
while open and shared half clocks.

The original wire-list inventory locates all 12 positions: ProcH/ProcL l06+l07
(operand sign/odd and delayed bypass) and eight MemC address latches a14/a15/
b14/b15/c14/c15/d14/d15. Every original half leaves at least one of S/R
unwired, so the excluded closed S=R=1 case cannot arise from those connections.
These are real production-cell semantics, but the
surrounding board timing and original-firmware startup remain unqualified for
this correction. Native59's preserved source and deployed image are unchanged.

Run from a checkout with a fresh output directory:

```
python3 tools/test_mc1668.py --output build/mc1668-new
```

The qualified run uses frozen inputs, including the manufacturer sheet. Raw
source, logs and the usage inventory are losslessly archived with byte counts
and SHA256 in the compressed manifest. Binaries and objects are omitted.
After extracting and checking the manifest, the frozen script can reproduce
the suite from `qualified/executed-inputs/tools/test_mc1668.py`.

This proves the sampled cell truth table. It does not prove analog timing,
the native sampling cadence, complete cache/processor behavior or a speedup.
