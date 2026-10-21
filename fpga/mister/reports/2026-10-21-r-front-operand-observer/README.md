# Original Cedar operand observer: component evidence and rejected V5

Current result: corrected V6 passes full source/memory/timing review,19 damaged
controls and guarded assembly, and is normally loaded on MiSTer14. Its live
marker passes. Original Cedar113 reaches native812/RequeueSearchSQ;80 physical
operand/PC observations are retained. See [qualified-v6](qualified-v6/README.md).
No desktop, speedup, rollback or complete-goal qualification is claimed.

The [production native memory prefix](production-native-memory-prefix/README.md)
adds the full original-EPROM/eight-board SDRAM diagnostic, explicitly rejected
earlier harness attempts, packet-adapter checks and a restored live video test.
V38 was still running at collection; no terminal firmware result is claimed.
The [paired control prefix](production-native-control-prefix/README.md) adds
the qualified MC1668 source control, successful compile/link checks and actual
live-process observations for both runs. Both firmware outcomes remain pending.
The [native register/point-observation prefix](native-front-observation-prefix/README.md)
preserves read-only first-trap capture setup, logger recovery and isolated
capture-consistency checks. The [default resolver refresh](default-resolver-refresh/README.md)
corrects a stale local executable to match the committed inverse-placement tool.

The root manifest freezes V5 staged inputs and terminal V29/V31/V30 component
tests:1,871 members/539 unique blobs. **V5 is rejected as an operand diagnostic**:
it changed an unused top copy. Its CPU ports pass the component gates, but its
FPGA top never selected those ports. No assembler, RBF or hardware load for V5.

`rejected-v5/` preserves all963 collected source/report members and the original
passing timing-reader result. That result did not cover diagnostic connectivity.
The explicit rejection is in `corrected-v6-source-binding/` under
`rejected-v5/observer-rejection.json`; it takes precedence over that result.

`corrected-v6-source-binding/` preserves542 members: corrected frozen V6 sources,
source reconstruction and component reuse proof, synthesis source inventory,
inverse-placement source analysis, failed V28 preparation, and current tools.
Five changed HDL files, including actual `core/Dorado.sv`, are used in the
synthesized netlist. No fit/STA qualification or hardware-marker success is
claimed by this extension. V6 full fit/report jobs were still live at collection.

V29 passes55M original-image samples (3,600 Cedar IM words,256 IFUM entries,
entry238 executed, native-control final RM/map) and6,875,000 output pin checks.
V31 passes14M samples/all16,384 map entries/256 RM words and1,750,000 pin checks.
V30 intentionally swaps output-only T/Md bytes and fails its pin check at3835.
V28 failed preparation only; no simulation started. All four are terminal.
V6 board/evaluator inputs match tested V5 exactly; only the two emu copies differ.
The corrected top mux is outside those component tests. Its live invalid-page
marker `d0d0bad0` must be verified before interpreting hardware operand pages.

`readout-integration/` preserves the actual-top/real-UIO command70 test and
17 frozen text inputs/results with SHA256 readback. Twenty transactions pass,
including pages, marker, fault priority and atomic data/status/PC with every
input changing during the reply. Removing the observer define fails. The first
test attempt had a declaration syntax error; its source/compiler log is kept.
This test does not execute the CPU or qualify hardware timing.

`retained-conditions/` preserves terminal V32/V33 original-Initial/map runs:
892 members/439 blobs, plus hashed review/preparer/analyzer files. Their6,000
original phase rows and final RM/map dumps match their respective parents
byte-for-byte. Retained zero/sign/overflow/carry flags match atall6,000 samples,
including4,500 closed-clock samples. T/A/ALU differ only at60/30/26 isolated host
samples and match again on the next sample. This does not cover Cedar's later
queue loop. V32's first preparer inherited the parent's unused private frame
output; its bytes match and the inspector classifies that single extra file.
The corrected preparer copies declared inputs only; both versions are kept.
The actual rejected V5 is also rejected by both new source-binding checks.

`bounded-host-pauses/` preserves882 terminal members/428 blobs (407 shared from
the root). V34/V35 each run14M guest samples/15,738,741 host ticks, including
1,734,978 paused ticks. Eight native-board host/advance connections replace
continuous fixture clocks; all CPU/helper/ROM bytes remain unchanged. All16,384
map entries/256 RM words and6,000 phase rows match the respective unpaused
parents. Complete source diffs and the executed reviewer are retained. This
does not exercise actual SDRAM responses arriving during a pause or Cedar's
later queue fault, and does not qualify a firmware boot or speed gain.

All6,000 phase rows match prior qualified V26. Transient T/ALU differences from
the older native control predate this observer; no consumed-operand equivalence
is claimed. The original MB and served EB match3,600 shared IM/3,857 shared items.
All sampled loop PCs resolve to RequeueSearchSQ after undoing MicroD placement.

Verify each manifest using `tools/archive_r_front_ready_followup.py --verify DIR`.
Child manifests share immutable parent blobs using relative paths. The original
root manifest is unchanged. No FPGA executable, disk pack or Quartus database
is archived. Main/daemon/protocol/guest storage/clocks are unchanged. Desktop,
speed, authentic FPGA rollback and full goal qualification remain open.
