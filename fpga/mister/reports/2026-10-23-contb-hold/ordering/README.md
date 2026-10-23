# Memory-produced Hold and ContB capture ordering

The maintained experimental component is
`verilog/rtl/dorado_contb_hold_step.sv`, SHA
`bd23230a24789c1544e119aaa828bf8c39d12113c8517c2faeede686fb418c90`.
Its ContB helper is `dorado_contb_mir_step.sv`, SHA
`a793d9bb15bb12bdab9b0048b94ef69462ac668498b020b890a952ebed102fea`.
Both files are byte-identical to the compiled passing inputs. Neither is
instantiated in the native FPGA machine. The maintained 798-Q processor graph,
hardware 5c65, Main, daemon and disk volumes remain unchanged.

## Original producer closure

The combined circuit preserves 46 original Qs: the previous 40 ContB bits,
MemC l02 at41:40, e22 at43:42 and e23 at45:44. It retains31 separate clock
channels and34 functional flags. These six additional Qs belong to the original
packages; they are not a new pipeline stage or a synthesized power-on reset.

Original MemC l01/l06/h12/i18/j18 produce the phase-1 clocks. Hold is the wired
OR of e23.Q0, inverted e22.Q0 and inverted e22.Q1. Distinct d23 outputs fan out
to PRhold and CBHold. The old raw CBHold input at3 no longer supplies the ContB
clock. The reference uses the actual BaseBd j02/k01/l01 parallel distribution
to the two boards, including both wired CLK.mc' outputs. Source pulse generation
and analog board skew remain outside this comparison.

The unused l02 B half is retained. Its Q and complement both drive Hia, which
also drives its own asynchronous reset. Binary Hia=1/Q1=0 comes from that
physical wiring; it is not invented initialization. Both e23 halves are also
retained: A uses PrClk1'Da and B uses clk1'Dc, so IgnoreProc must not gate B.

## Results

Icarus and Verilator pass all three illustrative delay profiles: balanced,
fast gates and later control. Each run compares7,260 events, all46 original
Qs,31 clock channels and34 flags. All clock counts agree across engines.
All physically reachable bits are seen both ways, except the original
Hia-reset l02 B half, which remains reset as its circuit specifies.

Each run exercises96 held-data release sequences using all three actual Hold
sources. New IM data settles while the memory-produced Hold remains asserted.
The phase-1 capture releases Hold without capturing that waiting instruction;
the following eligible edge captures it. All20 connected MIR clock channels
are explicitly checked for no early edge. The test also exercises phase and
enable changes, isolated LOW-root IgnoreProc capture, asynchronous DisHold,
maintenance manifold releases, shared reset and CP transactions.

Seven deliberately wrong variants reject after successful compilation in
both engines:

| Wrong behavior | First failing event |
| --- | ---: |
| Restore external CBHold input | 4 |
| Use uncaptured raw hold-request data | 3 |
| Retain old Hold after a real capture | 4 |
| Drop the reference-hold wired-OR driver | 4 |
| Reverse the memory phase polarity | 4 |
| Ignore IgnoreProc on the gated hold clock | 949 |
| Gate BLretry's separate clock with IgnoreProc | 949 |

The new memory boundary audit checks42 physical pins and eight unwired inputs,
traces eight unused selected gate-half inputs, and rejects five changes that
make those inputs live. Nine BaseBd fanout pins are checked at generation.

## Limits and next work

The raw-input handoff failures preserved in commit65affeb0a remain valid. This
experiment uses the original memory register/clock paths to produce CBHold;
those paths pass the tested held-data releases. This is not a claim that the
standalone settled-level helper supports arbitrary simultaneous raw changes.

The tested external contract changes CA enable/MemSH boundaries at a HIGH root;
it also exercises isolated LOW-root IgnoreProc and DisHold changes. Actual
Control-A enable/MemSH/BNT/UseDMD/reset producers, the stored MCR DisHold control,
actual reference/Md/miscellaneous hold-data and IM/parity/DMD remain open.
Analog minimum pulse widths, skew, native boot and FPGA timing are unqualified.

Next couple the actual Control-A stages and field/reset crossings to this
memory-produced Hold path, preserving old-data and resolved-clock epochs.
Then integrate with the complete processor graph and close the remaining
producers before native original-ROM/60ns validation. Full physical66.7MHz
SDRAM, timers, Cedar/rollback/persistence/input/fresh-boot/stress acceptance
remain part of the active goal. No hardware speed gain is claimed here.

## Receipt

`qualification.json` records the bounded result and remaining boundaries.
`manifest.json.gz` maps1,389 exact source/log members to176 gzip blobs,
543,856 compressed bytes. Lossless readback verifies every blob. It includes
the initial generated-test syntax failure, both pilots, the complete matrix,
wrong variants, original sources, maintained RTL and current generators.

Two terminal pilots were reused after exact testbench/candidate/source checks;
the other18 cases were new comparisons. All40 compile/test actions are terminal.
Reproduce with a fresh output directory:

```sh
python3 tools/qualify_contb_hold.py --output build/rtl-executive/contb-hold-new-review
```
