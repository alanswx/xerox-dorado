# Isolated T-control translation

Original references: HM3.8/Table10, Table11a TgetsMd, ProcH-Rev-Ce and
ProcL-Rev-Ci drawing PDF page22 / printed20.

`make -C verilog t-controls-test` exits0:

- Original wire-list checks:24 pins each, package types, tied LC/FF drivers,
  write-enable connectivity and different capture clocks.
- 8,192 direct-enable comparisons against independently clocked original
  combinational gates:8 LC values x256 FF values x4 capture masks.
- 8,164 legal-input checks against the manual's LC table,2,048 no-capture
  holds, and post-edge data changes without pass-through.
- A mutation giving both control banks one common enable disagrees in4,096
  cases, demonstrating that the test detects the proposed bad shortcut.
- Strict Verilator `--lint-only -Wall --top-module dorado_t_controls` exits0.

The original boards have different RbSelMd/RbSelMd' pin assignments and
different clock nets (ProcH h22 Clock0'Da, ProcL d17 SHCP'C). The translated
interface normalizes signals by name and takes independent capture enables.
FF075/LC combinations other thanLC5 are tested electrically but excluded
from manual semantics because HM3.8 declares those combinations illegal.

The regression supplies the enables. Their derivation from original clock
gates, T data latches, task files and complete processor/firmware equivalence
are NOT qualified. No generated board or FPGA uses this new module yet.
The separate Native29 candidate changes the existing core PLL only; it does
not use either experimental clock/control module.
