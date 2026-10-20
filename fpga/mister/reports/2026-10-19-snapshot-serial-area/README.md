# Same-coverage area optimization: 2,548 ALMs smaller, still rejected

Serial62Area requires **46,702 / 41,910 ALMs**, versus Serial61's **49,250**:
2,548 fewer, still 4,792 over capacity. Fitter requires 4,701 LABs versus 4,191
available and exits3. No STA, assembler, RBF or hardware reload followed.
The failed summary's zero RAM-block count is not a physical RAM result.

This is an equal-coverage comparison: all 659 staged input files match after
path rebasing except four explicitly reviewed optimization assignments in QSF:
Aggressive Area mode, Minimize Area with Chains packing, Area synthesis and
physical combinational area optimization ON. All nineteen qualified adapters,
1,966,264 physical board bits, macros, original clock/IO constraints, memory
policy, twelve-slot transport and virtual service contracts are unchanged.
The rejected D/enable-factored frontend is not used. No fresh RTL XML is claimed;
the identical HDL/macros retain the earlier complete logical inventory.

The installed Quartus17 metadata documents these legal modes and their possible
timing tradeoffs. The actual map report selects Area synthesis; the fitter
reports aggressive area and Minimize Area with Chains. Register retiming and
duplication options remain unchanged. Physical memory inference may differ under
area optimization; it has not been qualified as a legal placed design.

The independent source reviewer rolls back only the four exact approved options
before running the original full-equation/pin/constraint audit. All 27 damaged
source/policy controls reject. Its 23 inherited cases run through the area
reviewer after the undamaged area project passes. The shared test helper adds
an optional reviewer callback; the unchanged default path passes all 23 original
controls. Terminal Quartus source aliases are independently accepted only as a
complete exact set. All 413 Native59 receipts and 659 remote/local input matches
pass; all 658 non-QSF inputs remain byte-identical after the build.

The terminal capacity reviewer reads the actual reports and all nineteen board /
three transport scopes. It explicitly adapts only the source-audit report
envelope for the shared resource reader, retaining the 23 inherited controls and
four separate area-policy controls. Four damaged capacity reports reject.
Hierarchy attribution is diagnostic and cannot predict another fit by summing
isolated savings.

All 1,553 raw archive members pass lossless readback. Extracted source/policy
controls, actual Quartus alias audit, byte-identical 659-file restaging and the
terminal equal-coverage resource review/controls pass. Sources/reports retain
original bytes, compressed without whitespace edits; databases/objects excluded.

Both remote handles are absent: builder2346741, fitter2349070. The immutable
project is `/home/alans/mister/Dorado_Serial62Area_20261019`; inputs are isolated
at `/home/alans/mister/Dorado_Serial62Area_inputs_20261019`. Do not restart it.

Next prototype a shared, glitch-free hardware clock enable for ordinary physical
FF helpers during real guest advances and physical scans. Keep controllers and
host memory work running, retain every captured field and guest cadence, and
verify clock-high request changes, scan endpoints, abort/quarantine and resumed
operation against the untouched actual MSA before measuring area/timing. A new
gate's phase state and real timing coverage must be accounted for. No such clock
change is implemented or qualified here.

Native barriers/wrapper/ARM/server state, true reseed ownership and atomic
disk-paired FPGA save/continue/restore remain required. Original 60 ns clock/timer
work and final boot/input/persistence/performance/stress gates remain open.
MiSTer14/Native59 and its last qualified 1,947,654.12 issues/s are unchanged.
