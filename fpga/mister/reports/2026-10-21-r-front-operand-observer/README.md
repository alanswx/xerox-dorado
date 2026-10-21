# Original Cedar operand observer: component evidence and rejected V5

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

All6,000 phase rows match prior qualified V26. Transient T/ALU differences from
the older native control predate this observer; no consumed-operand equivalence
is claimed. The original MB and served EB match3,600 shared IM/3,857 shared items.
All sampled loop PCs resolve to RequeueSearchSQ after undoing MicroD placement.

Verify each manifest using `tools/archive_r_front_ready_followup.py --verify DIR`.
Child manifests share immutable parent blobs using relative paths. The original
root manifest is unchanged. No FPGA executable, disk pack or Quartus database
is archived. Main/daemon/protocol/guest storage/clocks are unchanged. Desktop,
speed, authentic FPGA rollback and full goal qualification remain open.
