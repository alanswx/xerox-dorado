# Native51: full-core ContB clock-enable integration, BUILD IN PROGRESS

Not timing-qualified or deployed. Live MiSTer14 remains the selected Native49
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

At05:03:55UTC19Oct, handle62531 and remote Quartus fitter748087 are LIVE.
Map746420/helpers747609/747610 are COMPLETED: synthesis0errors/471warnings;
map summary preserved. Warnings/routed timing are not yet reviewed. Local log `build/rtl-executive/native51-contb-build-v3.log`.
Revalidate/poll that same handle or inspect its project process/log; never
restart from an observation timeout. Flow is map, fit, asm, STA, clock audit
and four-condition device reports. Integration source hashes/assets are
archived. No RBF, routed timing pass or hardware qualification is claimed.

Commands: on Mac run `build_native_contb.py --prepare-assets NEW_DIRECTORY
--comment-reference FROZEN49_MC10170_SOURCE`; copy assets and matching builder
to misterubuntu, then run `build_native_contb.py --build --assets DIRECTORY
--output NEW_PROJECT`. The default baseline is the frozen Native49 project.

Next review actual complete-core setup/hold/clock/SDRAM timing and source
provenance before any board load; preserve/reload via existing Main/daemon
barriers. Qualify original-ROM startup, Cedar desktop/input and measured
progress. Then connect bounded state access only alongside the coherent
machine/SDRAM/disk/server pause and remaining state inventory. Whole-machine
snapshots, original60ns cadence and final repeated/stability gates remain open.
