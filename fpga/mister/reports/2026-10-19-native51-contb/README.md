# Native51: full-core ContB clock-enable integration, TIMING PASS

Timing-qualified, NOT DEPLOYED. Live MiSTer14 remains the selected Native49
RBF in its fresh49b Cedar session, Main8321/daemon8318. Native51 is a separate
full-core build to verify the qualified control-board adapters against the
other original boards and the real shared clock distribution.

The complete modeled ContB state-access experiment is qualified separately
in snapshot-contb-full. This integration deliberately holds ALL snapshot admin
inputs inactive: there is no global barrier or state transport yet. It cannot
save/load a physical machine. Quartus can prune inactive admin paths, so its
resource counts will not budget an enabled full-core snapshot implementation.
Original clocks remain50MHz/SYSPER4, with no measured speed claim.

Starting from frozen working Native49, only these existing files change:
generated backplane (select full ContB and supply host clock/advance), generated
native wrapper (pass advance), and core wrapper (use existing network_step,
the same falling-edge-latched machine_enable as the global clock gate).
Qualified board/RAM/FF adapters are added; core QSF adds those files. SDRAM,
raw layout/arbiter/pipeline, cache/map/ECC, original other boards, device clocks,
Main/server and all timing constraints remain frozen. No exceptions are added.

Builder checks every qualified original ContB source against frozen49 before
building and asserts the exact changed/added source set. One MC10170 source
differs only in full-line documentation comments. Both raw versions and a
narrow code-line equivalence check are archived: complete code lines and
inline comments must match byte-for-byte; block comments/continued lines are
not accepted. Assignment, inline-comment and block-comment controls reject.
The frozen49 source is retained unchanged. No parity-logic difference is waived.

Preparation attempt1 stopped before project creation on that raw-source hash
guard. Attempt2 stopped before Quartus because the generated file contains
an unused test wrapper with the same storage port line. Its terminal log and
partial project `/home/alans/mister/Dorado_Native51_20261019` are preserved;
do not restart it. The builder now targets the active backplane's unique
header. Actual current project is
`/home/alans/mister/Dorado_Native51_v3_20261019`, assets/builder in
`/home/alans/mister/Dorado_Native51_inputs_v3_20261019`.

Build62531/map/fitter COMPLETED.31 summary checks and36 original all-device
reports pass. Added bidirectional ContB-to-other-board setup/hold checks add
16 reports across four conditions:52 total, minimum+0.115ns. No added cuts or
illegal/unconstrained clocks. Existing framework I/O counts remain exactly
unchanged (3 input ports/13paths,50 output ports/154paths); SDRAM I/O and
ContB crossings are constrained. This is not a zero-unconstrained-I/O claim.

All eight instruction-memory macros retain1024x18 simple-dual-port OLD_DATA.
Only26 unused-object synthesis warnings are added; other synthesis/fitter
warning categories match the working49 baseline, no Critical Warnings.
Full-core counts30640ALMs/52921registers/330M10Ks/35DSP, versus49's30473/52929/
330/35. Admin paths are inactive/prunable, so this is not the future enabled
snapshot-access resource budget. RBF3d253187b42615e53fde1dc1bed688eb0328265d007bffdc3c161196294ccac3.

The first crossing audit79065 overlapped the still-running device audit.
Its18 files are preserved in crossings-first-overlap.tar.gz and superseded.
Definitive sequential crossing audit1060 ran AFTER62531 completed; all four
conditions/pass reports and exact script are archived. Core/source/QSF/SDC
and instruction-memory inference are checked by collect.py. Quartus appended
relative aliases of four already-approved absolute source paths; only those
exact physical aliases are deduplicated. Extra source/SDC/macro controls reject.
Parser corrections and provisional collector logs are preserved; qualification
uses the final actual-table parser, not report contents-list titles.

No hardware boot, speed improvement or physical state save/load is claimed.
Local candidate build/rtl-executive/native51-candidate contains exact RBF and
qualification. Complete timing evidence and fitted source hashes are archived.

Commands: on Mac run `build_native_contb.py --prepare-assets NEW_DIRECTORY
--comment-reference FROZEN49_MC10170_SOURCE`; copy assets and matching builder
to misterubuntu, then run `build_native_contb.py --build --assets DIRECTORY
--output NEW_PROJECT`. The default baseline is the frozen Native49 project.

Next preserve/reload via existing Main/daemon barriers after normal guest Boot. Qualify original-ROM startup, Cedar desktop/input and measured
progress. Then connect bounded state access only alongside the coherent
machine/SDRAM/disk/server pause and remaining state inventory. Whole-machine
snapshots, original60ns cadence and final repeated/stability gates remain open.
