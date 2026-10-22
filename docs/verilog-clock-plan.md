# Dorado RTL clock implementation plan

Started 2026-10-18 UTC. Native59 is preserved as a qualified 50 MHz SDRAM/direct-DDR
Cedar desktop/input baseline; StorageReadCapture1 now qualifies the corrected BL8 path. Native49/fresh49b and Native34 preserve earlier
50 MHz references; Native26 preserves the earlier 33.333 MHz reference.
The user has released MiSTer14 for reloads/faster builds; keeping
that session alive is no longer a constraint. See the [clock audit](verilog-from-sil.md#clock-architecture-audit-2026-10-18)
for measurements and the [handoff](verilog-handoff.md) for deployed artifacts.

## Active delivery plan and goal (revised 2026-10-20)

**Latest status (2026-10-22 03:14 UTC): working Cedar stays on MiSTer14;
memory timing improves, CPU cone limits66.7MHz.** Main14025/server14024 remain
alive, actual03:00 internal screenshot shows the desktop, CommandTools, keyboard
echo and warm file marker. CRC/RX/abort0, TX dropped30. Actual hardware remains
50MHz/SYSPER4; the last measured idle-login rate remains2.592922M issues/s
(15.56% of16.67M/s), not a new desktop speed measurement.

The same-epoch per-MSA ready reduction and fixed-lane SDRAM/trace capture pass
original144-chip/all-bit/exact-host-trace, dual-simulator temporal groups, pin/
fault suites and full8M-cell simulation at57.1/66.7MHz. One-MSA15ns timing passes
all corners (setup+0.123ns,hold+0.072ns), with one ready register and no extra
host cycle. Full128-group startup prefixes pass at both clocks but have zero
CPU issues/writes and an array backend: no original-firmware qualification.

Full native66.7MHz is terminal and rejected. Memory-ready-to-step+1.093ns and
SDRAM output setup+3.369ns pass, but CPU setup is-1.877ns in the summary/-1.943ns
in the detailed MemC/MemD->BMux/ALU->ProcH diagnostic->DMuxData->ProcL k22 path.
No false/multicycle waiver was added. The953-member terminal receipt preserves
all503 sources and failed/repaired read-only report helpers; original fit was
not restarted. [Component/rejected-native evidence](../fpga/mister/reports/2026-10-22-storage-board-ready/README.md)
verifies5815 members/1283 blobs losslessly. No image assembled or loaded.

Intermediate57.142857MHz builder536779 and repaired reporter591029/collector591030
are now terminal, all stages rc0 and every recorded PID absent. All503 source
hashes remain unchanged; the [951-member terminal receipt](../fpga/mister/reports/2026-10-22-storage-board-ready/native57-terminal/README.md)
verifies. CPU setup+0.721ns/hold+0.093ns and all31 summary/observed detailed
slacks are positive; strict recipe/RAM/warning/category/guard review remains
pending. The original fit was never restarted. Its
SYSPER4 ordinary ceiling is7.143M/s before waits;66.7MHz would be8.333M/s, still
below the original60ns/16.67M target. Neither is measured FPGA speed yet.

An MC10181 Boolean carry rewrite passes all16,384 binary input combinations and
seven outputs in both simulators, independent HM3.7 arithmetic and real bad
controls. A fresh full-native66.7MHz fit with only this combinational cell changed
is running: builder607315/reporter607316/collector607317 at
`Dorado_StorageBoardALUNative66_1_20261022`. Registers/capture epochs and all
cache/Map/ECC/peripheral logic/constraints are unchanged; manifest explicitly
records changed CPU combinational logic. The [source/live-prefix receipt](../fpga/mister/reports/2026-10-22-storage-board-ready/alu-native66-source/README.md)
does not grant assembly permission. Finish the57MHz terminal review and the
existing ALU job's full terminal/all-corner/source/RAM/guard review, then actual physical-memory/
original-firmware Cedar/input qualification. Authentic rollback and final cold
persistence/repeated boots/stress remain open; whole FPGA snapshots deferred.
Earlier progress below remains historical context.

**Earlier hardware qualification (2026-10-22 00:12 UTC): corrected SDRAM core boots interactive Cedar.**
MiSTer14 runs StorageReadCapture1 RBF0c578915, Main14025/server14024. All8,388,608
raw12-bit cells pass both address patterns and zero verification; first burst
is exactlyA59/A58/A5B/A5A/A5D/A5C/A5F/A5E. The old local chip model launched
one clock late, masking the corresponding late controller capture. Independent
CAS reference, corrected pin/full-memory/matrix/board checks, native timing
(minimum+0.118ns), RAM/source review and18 rejection controls pass.

Original firmware has delivered Cedar BFN113 and reached its login prompt
(actual23:34 screenshot); normal C is released. The FTP_UNSERVED line is
misleading because the microcode handler runs later in the same function.
Three pre-Guest idle-login windows now average **2,592,921.60 issues/s**,
15.56% of the16.67M/s target (60ns),33.1% above the previous1,947,686.01/s.
Clock cadence is unchanged. This is one login workload, not full desktop speed.
The CPU/cache/map/ECC are unchanged MC1668-parent bytes. The separate
experimental Ready/front replacement is not integrated here; its native812
regression remains an independent gate and does not inherit this Cedar pass.
All34 base transfers and final guest ACK pass (305.897..306.043s,37replays);
normal profile-No was accepted at the actual prompt. The loader is slower than the
earlier286s observation, so do not claim an overall boot speedup from idle
issues/s. The SAME original boot now passes Viewers, mouse/Cmd click and
focus, exact short/alphabet/digit echoes, and normal warm file create/type
readback (`///rtlreadcap1persist.txt` returns `rtlreadcap1persist`). Latest
CRC/RX/abort0, TX dropped27. Clear settled internal raster; HDMI capture-card
verification remains separate. The114-member/111-blob desktop/pause receipt
passes lossless verification;88,659 disk-prefix records show no errors/gaps.
Canonical controller/model defaults and exact historical replay now pass (details below).
The memory-ready prediction now passes functional checks and one-board15ns
timing below. All128 native groups pass startup-prefix coupling checks; the
full native same-clock fit is terminal and316 categories/source/memory inspection
passes. Damaged-evidence/assembly guards and physical-memory/original-firmware
qualification remain pending. Shorten the measured remaining half-cycle paths
before claiming a faster clock.
Qualify the experimental CPU front separately before merging the faster phase path. Correction
and native/memory/login receipts committed/pushed as7d5b78b7. The full corrected native/memory/login archive is
[here](../fpga/mister/reports/2026-10-21-sdram-read-capture/native-fit/README.md). Clock acceleration, authentic FPGA
rollback, cold persistence, three fresh boots and two-hour stress remain open.
Whole FPGA snapshots remain deferred. Earlier hardware/build statuses below
are historical; see the latest top-of-handoff status for current identities.

The completed installer cause sample selects counters through Main, then detaches
before each timed interval and restores all selectors. Separate five-second
windows show storage-only pause58.42%, disk-only0%, simultaneous0%, and held
instruction cycles3.92%. Enabled edges were36.04% in their separate interval;
do not sum these into an atomic partition. This implicates the SDRAM path for
this workload, not ARM-disk blocking across every workload. Current fit's worst
original setup path is MSA3 row[5] to the falling-edge storage observer (+0.785ns)
and functional network_step gate (+0.790ns), both a10ns half-cycle relationship.
The next speed component must shorten that actual address/ready/gate cone while
preserving original read-before-write and pause/response epochs. Simply
registering a stale ready-high value can advance the machine incorrectly.
Retain this measured50MHz/SYSPER4 control; validate phase, peripheral rates and
physical SDRAM I/O for a faster candidate. Isolated15ns CPU slices do not prove
a faster whole-device clock. Local field: `storage-readcapture1-native-field-20261021/installer/`.

Canonical sources now use the physically qualified wait2 by default. The broader
BL1 suite also exposed a local DQM launch-stage error at20MHz/high addresses;
independent dynamic-mask CAS2/3 comparison passes16 checks in both vendor and
corrected local models and rejects the previous local model at runtime. Default
BL1 full geometry/four positive profiles/three negatives, BL8 matrix26 actions,
selftest/trace12, original144-chip MSA4/every9,437,184 bits, fresh CAS6/DQM6 and18
reader rejections pass. The prior native review is byte-identical; current
assembly binds the source-helper dependency. No hardware, clocks or measured
speed changed. See the [348-member receipt](../fpga/mister/reports/2026-10-22-canonical-sdram/README.md).

Implemented component: expose a preview of each original row/column latch's
post-edge address without changing stored state. Predict the existing host
cache-valid/waiting/write-completion updates, comparing hold and advance paths
BEFORE the late machine-step selection. This preserves request/response epochs,
including simultaneous response and logical write events. Registered readiness
matches the original combinational equation after each tested host/machine edge.

The original144-chip MSA oracle passes12,012 cycles and all9,437,184 stored bits;
its complete positive raw trace, including host timestamps, matches canonical
BL8 byte-for-byte (112,849 host cycles). Three compiled runtime controls reject
stale ready, old address and omitted write readiness. Width8/12 groups each pass
40,000 checks in BOTH Icarus and Verilator, with reset, arbitrary stalls and1,948
responses while paused; both engines reject stale-ready runtime controls.

The full ONE-board component with all16 groups and native altclkctrl gate fits
at15ns across all four operating conditions: setup minimum+0.121ns,
hold+0.067ns, pulse+6.547ns. All setup/hold unconstrained categories zero; no new
timing exceptions. All16 logical ready registers plus group14's physical copy
and prediction/ready-to-gate/late-step paths are reviewed. Resources1520ALMs,
2635registers, noRAM/DSP. All four stages are terminal/rc0. Exact source review
accounts for Quartus's version-only QSF append and all warnings; eight damaged
receipts reject, including rehashed clock/gate/prediction changes.
[429-member receipt](../fpga/mister/reports/2026-10-22-storage-ready-prediction/README.md)
preserves the executed sources and results. Services here are registered virtual
ports: this does not qualify full128-group fanout, CPU, physical SDRAM I/O,
original-firmware operation or a hardware speedup. No new RBF/load occurred.

All128 native groups now use existing contb_advance=network_step in a503-file
stage from the corrected BL8/MC1668 parent, at unchanged50MHz/SYSPER4/admin inactive.
CPU/cache/map/ECC, top/machine wrappers, physical SDRAM backend and timing constraints
are unchanged. Two full backplanes compare all175 outputs and each group's
address/cache/write/read state after every host edge. V2 passes8M checks and
1,562,032 delayed raw reads/paused responses, with native BaseBoard clock feedback,
a normal20ms boot pulse and forced pauses. A compiled eight-board disconnected-step
control fails at1,080ns with the explicit readiness assertion. These are startup
PREFIX checks: zero CPU issues/writes and an independent raw array backend. They
do not qualify original-ROM completion, Cedar or physical SDRAM. Separate original
MSA/all-bit/write tests remain in the component receipt.

Full native build at `/home/alans/mister/Dorado_StorageReadyNative1_20261022` is
TERMINAL: all nine stages and all three read-only follow-ups return0; builder120341,
fit133372, reporter153502 and collector185040 are absent. Both ready filters work
and find the same128 registers. All503 source hashes are unchanged and the
939-member terminal archive passes size/hash readback. No reporting failure,
constraint/fitted-source repair, refit, assembly or load occurred.
[1776-member integration/prefix receipt](../fpga/mister/reports/2026-10-22-storage-ready-native/integration/README.md)
passes lossless verification. The new
[963-member terminal-fit inspection receipt](../fpga/mister/reports/2026-10-22-storage-ready-native/native-fit/README.md)
also verifies losslessly:170 memory entries/165 parameter tables unchanged,
40 independent cache M10Ks/seven-block ROM pool preserved,128 ready registers
without copies,31 nonnegative summaries and316 required nonempty timing categories
with identical unconstrained coverage. Parent map/fit/STA warnings are unchanged
except two added unused assertion-reference warnings. Known trace alias warnings
are recorded with all97 actual trace bits present. No critical warnings/errors.

Minimum hold+0.006ns is preview row[6] to its own saved_addr[14], fully timed
because the old typed address exclusion does not match the renamed cell. No new
exception was added. Worst setup+1.060ns is dq_capture[11] to the passive first-burst
capture on a10ns half-cycle. The128-ready reduction to network_step has minimum
setup+1.820ns; late step to prediction+2.567ns. None proves a66.7MHz full core.
Next speed component: compute the same two post-edge readiness cases/reduction
once per MSA and select/register at the SAME host epoch, reducing the late
128-ready fan-in. Recheck original144-chip/all-bit/host-trace and full native
coupling; shorten SDRAM capture/diagnostic paths and validate actual faster I/O
timing separately. Formal damaged-evidence/assembly guards and physical8M-cell/
original-ROM/Cedar/input qualification remain required. Preserve the separate experimental CPU-front
integration gate. Hardware remains50MHz/SYSPER4 and2.59M idle-login issues/s;
original60ns operation is still the target. Authentic rollback and final cold
persistence/repeated-boot/stress tests remain open.

Historical StorageSelftest1 hardware: **pre-guest raw SDRAM test fails at address0**, expected0xA59,
actual0xA58. MarkerB157 and two metadata samples confirm phase1/failed1/ready0;
the guest stays held. Address1's pattern is0xA58, suggesting a shifted burst,
but read alignment, write corruption and a data-bit fault remain distinguishable
possibilities. Do not call this a proved capture fix or a guest boot failure:
no firmware boot was attempted with this diagnostic image.

StorageSelftest1 RBFc63d0a6a is loaded normally on MiSTer14, Main11889/server11888,
unchanged binaries and50MHz/SYSPER4. All eight build stages, fitted inventory,
31 device summaries/196 original/56 backend/8 observer/24 self-test categories
pass (minimum+0.116ns). All170 RAM entries/165 parameter tables preserve their
contracts;332M10Ks/35DSPs. Thirteen damaged-evidence controls reject; assembly
passes. The full pin-model test previously passed50,331,648 transactions across
8,388,608 raw cells, so the physical/model discrepancy remains unresolved.

The passive first-burst trace is now implemented/tested:97 retained bits, same
cache-fill data/event, no controller timing/state/data feedback; markerB178 at
pages24..27. The bounded test rejects swapped trace lanes; the full23-bit run
passes50,331,648 transactions and retains the original burst. Native501-source
fit is LIVE on misterubuntu (builder3616462,fit3620332; placement passed/routing),
with waiter3623270 queued for the SAME fit's reports/inventory. Source-cone review
confirms the actual synthesized top/wrapper/backend. Component receipt:
[trace evidence](../fpga/mister/reports/2026-10-21-storage-burst-observer/component/README.md).

Next: complete that build's original/new timing and memory/source review plus
rejection controls, assemble/load normally, then compare its first actual burst
with the written pattern and physical clock constraints. Distinguish ordering
from write corruption/data-bit failure; compare a BL1 control if needed. Repair
the proved cause, repeat full raw-memory testing on14, then resume original-
firmware Cedar/input/pinned issue+hold qualification. Preserve the failing image
and original firmware. No wait-count/model changes just to pass tests.
Last working Cedar login remains1,947,686.01 instruction issues/s (~11.7% of
original16.67M/s); clock acceleration, rollback, persistence, repeated boots and
sustained operation remain pending. Complete FPGA snapshots stay deferred.

Normal Main barriers preserved BootFault2's closed seed and complete trace with
two barriers/no gaps. Full952-member native/field/input-prefix archive:
[terminal evidence](../fpga/mister/reports/2026-10-21-storage-selftest/native-fit/README.md).
Control simulation83509 stopped naturally on its old logger bound at1510M;
the prepared read-only correction was loaded at that stop and the same process
resumed without interruption or guest-state writes. Control1470M already shows
CPUc0fc7000; the earlier statement that both jobs still showd0fff000 was wrong.
Current identities/results and next TODOs are in the
[handoff](verilog-handoff.md#current-hardware-raw-sdram-mismatch-before-guest-release-2026-10-21-utc).
The following paragraphs retain earlier live statuses for history.

V38/V39 completed three normal boot-button pulses via only their bench input
counter; final input observations1170M/1040M host ticks still show startup.
No CPU/IM/RAM/clock/cycle/IOCB writes. Existing processes/models and read-only
capture callbacks are preserved; diagnostic host limits extend to3B. Input
completion is not firmware qualification. Continue the original runs to a
meaningful boot result; do not restart on observation timeout.

The paragraphs below retain earlier progress; hardware identities and diagnosis
above supersede their live-status statements.

Production-memory continuation: V38 is running on the Mac (driver79190,
simulator79583), using exact V6 native CPU/boards/helpers/original EPROMs,
full eight-board SDRAM and the independent physical chip model. SDRAM zero-fill
and original BaseBoard execution are observed; Cedar boot is not yet observed.
No CPU/IM/IOCB plant or C guest execution. Packet timers now use free-running
host time; V37's guest-coupled timer attempt was explicitly rejected/terminated.
V36 was a compile-only syntax failure. Captured113 packet-adapter check passes
63 frames/31,376 exact payload bytes; this is not firmware qualification.
Current TODO is to follow the existing job to the first Cedar trap/queue loop
and inspect its retained pins/state, then fix the demonstrated data/hold fault.
Progress `guest` counts enabled fabric edges, not instructions. The runner,
scope limitations, debugger11479 and immutable prefix are in the handoff.
No hardware load or measured speed gain. The live progressive1080p60 capture
experiment remained black and the previous1080p30 mode was restored normally.
Matched full-memory MC1668 control V39 now also runs (driver83197/test83509,
LLDB66643), with all498 reviewed stage files exact and158 used HDL inputs.
Compile/link checks pass; seed/service/host timing match V38. Compare both
original-firmware outcomes before changing RTL. The control has no operand
observer; its zero operand-ring fields are placeholders. Both jobs remain live,
not terminal firmware qualifications; V38 was not restarted or modified.
Both existing runs now have read-only first-MTrap register/event capture via
LLDB and periodic physical RM observations. Control's logger callback ABI error
was corrected and the same process continues. Isolated Icarus point evaluations
reproduce candidate next-Q at440M/450M (no repeated/unsettled events); an offline
corruption rejects. These are capture-consistency checks, not independent
processor correctness or a Cedar gate. The stale default local mbdis executable
is rebuilt to match the committed inverse-placement source. No guest RTL,
clock, firmware or hardware binary changed; continue the same cold runs to113.

The raw BL8 native candidate is now full-device timing/memory reviewed and
loaded on MiSTer14:31 summaries,196 original paths and56 new buffer categories
pass (minimum+0.107ns). All170 RAM rows and165 parameter tables retain their
contracts;332M10Ks total. Seven damaged-evidence cases reject. RBFf46eb4f5,
Main9242/server9241, unchanged50MHz/SYSPER4. Original Initial requests113;
the short native raster is not a Cedar desktop. Hardware throughput remains
unmeasured. Continue this same original boot and diagnose receive/memory
progress before choosing the backend or resuming clock acceleration.
Terminal receipt `fpga/mister/reports/2026-10-21-sdram-burst/native-fit/`.

Earlier component/live-fit observations, retained for history:
an isolated memory-speed component now passes while those runs continue:
the raw BL8 backend buffers four eight-halfword lines and preserves the guest
CPU/cache/map/ECC. Complete native MSA/original-chip comparison matches12,012
logical cycles,9,437,184 bits and the exact12,380-record request/final-memory
sequence, with122779->113841 host ticks. Seven pin cases/six rejection controls
and the existing BL1 regression pass. V1/V2 failures are retained; the exposed
natural-read model OE glitch is corrected against the primary hold-time spec,
and V3 reserves earlier refresh for20MHz. None changes the frozen boot models.
A499-file same-clock candidate from qualified MC1668 has completed fitting on
misterubuntu (`Dorado_SDRAMBurst1_20261021`, builder3523028). No assembly/load
or hardware throughput claim. All31 ordinary timing summaries are positive;
detailed condition/crossing reports remain in progress. The component and
live-prefix receipt is `fpga/mister/reports/2026-10-21-sdram-burst/`.
Finish its actual timing/memory/source review
and original-firmware/input/pinned issue+hold gates before choosing it. This
does not replace the remaining 60ns clock work or the V38/V39 fault diagnosis.

2026-10-21 continuation: the visible-R v4 FPGA boot still displays Pilot812.
Inverse MicroD placement now resolves all sampled CIA239/23a/6c1/6c2 to
`RequeueSearchSQ`; physical proximity to BootOrStart/MiniMixer was misleading.
Output-only operand ports pass original-image/map and deliberately swapped-byte
tests. Observer V5 was rejected before assembly because it patched an unused
top copy. Corrected V6's actual `core/Dorado.sv` and all four board/wrapper files
are confirmed used by synthesis. V6 now passes full timing/memory/source review
(minimum+0.029ns, actual100ps margin/four corners),19 damaged-evidence rejections
and guarded assembly. It is normally loaded on14 with a fresh seed and unchanged
Main3401/server3400. The live `d0d0bad0` marker and selector restoration pass;
original Cedar113/C release now passes after three verified OSD BaseBoard boot
pulses (the initial USB-chord attempt selected default Alto110). Native capture
shows812;80 live operand/PC pairs confirm RequeueSearchSQ but do not establish
its earlier cause. HDMI capture is black while the native framebuffer is white.
V4's closed pack remains f3bce2a8 and its
trace's983-event transition gap is retained explicitly. Additional bounded
Initial/map runs match all6,000 retained flag samples and final RM/map; transient
T/ALU differences there do not establish a Cedar-loop cause. Main/daemon and
the50MHz/SYSPER4 cadence are unchanged. Last measured qualified login remains
1,947,686.01 issues/s; no speed gain claimed. See the current
[handoff](verilog-handoff.md#operand-diagnosis-corrected-pc-mapping-and-compiled-observer-2026-10-21-utc)
and [component/source receipt](../fpga/mister/reports/2026-10-21-r-front-operand-observer/README.md).

V34/V35 now also pass the bounded native host-clock/advance comparison:14M
guest samples each,1,734,978 paused host ticks, all16,384 map rows/256 RM words,
and unchanged6,000 parent phase rows/final dumps. Only fixture clock wiring and
bench change; CPU/helper/ROM bytes remain unchanged. This covers synthetic
pauses, not actual SDRAM response arrival during a pause or the later Cedar
fault. Exercise that memory boundary next; no speedup or firmware repair yet.

Deliver a faster, correct Dorado on MiSTer that cold-boots the original
firmware to Cedar's Viewers desktop and an interactive CommandTool. Use the
confirmed 128 MB SDRAM module for bulk main memory, keep hot architectural
state on chip, and use reserved DDR queues for direct ARM-daemon packet
transport. Remove blocking disk work from Main's polling loop. Finish the
clock/enable and timer migration to target the original 60 ns ordinary
microinstruction cadence. Report measured instruction progress and waits;
a higher PLL frequency or an enabled-edge ceiling is not proof of speed.

This is the replacement implementation goal authorized on 2026-10-20. The
user cleared the previous paused goal and the new tracked goal is ACTIVE.
**Speed and authentic Cedar Checkpoint/Rollback come first; complete FPGA
snapshots are deferred follow-up.** Snapshot-specific Main coordination,
whole-state access and daemon serialization are not prerequisites. Preserve
those prototypes and evidence without integrating them into the speed core.
Minimize and isolate necessary Main changes; Main remains the sole SPI owner.

The numbered milestones below are the delivery order and take precedence
over historical candidate diaries. Restart speed integration from qualified
Native59, whose state-administration inputs are inactive, rather than treating
the unqualified Serial74 snapshot-enabled source as the next deployable core.
Retain useful qualified clock components only after checking their contracts
against this normal-operation baseline.
Native34 is the earlier desktop/input reference; Native35 provides
the earlier diagnostic baseline. SDRAM41c now passes original-firmware
Viewers/input and41d cold marker readback. Direct-DDR network47 passes real
startup/login, loader traffic and packet progress during delayed Main worker
work; full47/48 Viewers/input and active STOP/reload now pass. External IFS/ContrAlto tunnelling is
optional follow-up,
not a prerequisite for the performance goal.

Task/address epoch v2 now passes the coupled15ns budget: setup+0.474ns,
hold+0.099ns, mpw+6.112ns, four conditions;2,845ALMs/2,784regs/9,216MLABbits.
It uses pre-event task/address data under the existing producer setup contract,
retaining all16 waves, checks, interfaces and15ns constraint. The88-Q gate passes
73,761 events/profile in both simulators/three profiles and15 bad variants;
reset/maintenance/Hold checks pass1,617 in both/three. The736-Q/9,216-RAM gate
passes six profiles in both simulators. Only task RTL/qualification JSON differ
from the failed baseline's prepared files. The worst path falls from18 to13
levels;17 damaged reader controls reject. Trial v1's-3.645ns fit is preserved as
rejected. All jobs are terminal. Subsequent Ready/arbiter proof below closes
that boundary and classifies the unused package-half constants. Live producer
arrivals and native integration remain open; no hardware speed gain yet. Goal ACTIVE.

The [task/address epoch receipt](../fpga/mister/reports/2026-10-20-task-capture-epoch/README.md)
preserves3,001 raw members with lossless readback/extraction and byte-identical
replay of rejected v1 and passing v2 timing reviews. All45/47/87 accepted
task/control/front frozen inputs were rechecked.

Actual CP/Ready producer follow-up now also passes:12-Q CP,16-Q Ready and
28-Q coupled original circuits, both simulators/three profiles at4113/98516/
99470 events respectively,7/8/3 rejected bad variants. The coupled component
passes four-condition15ns timing (+5.640ns setup/+0.155ns hold),126ALMs/205regs,
no RAM/DSP/PLL. Actual ClrReady is internal; all three SetReady drivers remain.
[Portable raw evidence](../fpga/mister/reports/2026-10-20-task-producer-logic/README.md)
includes extracted replay. Next couple actual arbiter/control/MIR/phase/Hold/IFU
sources before native integration. All jobs terminal; live speed unchanged.

Ready now connects to the task arbiter/processor captures in a104-Q component.
Both simulators/three profiles pass77923 events, including15 Ready-only task
wakeups and256 independent initial BNT/BNT' pairs;5 bad variants reject.
The original BNT/BNT' packages are independent storage. Four-condition15ns
fit passes (+4.942ns setup/+0.157ns hold),390ALMs/509regs. Default Ready and
CP/Ready regressions pass. The pin-level dependency proof also establishes
that all12 earlier Control-A constant stubs affect only unused gate halves
in the selected components; keep their surrounding original full-machine
logic intact. Next couple actual Ready with the full RBase/stack/RM/STK
component and close live Notify/preemption/Hold/IFU/MIR arrivals before native
integration. [Raw proof and timing](../fpga/mister/reports/2026-10-20-ready-arbiter-capture/README.md).
All jobs terminal; no hardware speed change yet.

The full Ready/RBase/stack/RM/STK combination now passes:752 processor Qs plus
9216 RAM bits,59 clocks,13173 events/profile in both simulators/six profiles,
all15 Notify-only task wakeups,9 rejected bad RTL variants and stable original
write windows. Four-condition15ns timing passes (+0.622ns setup/+0.171ns hold),
2944ALMs/2809regs/9216MLABbits.20 strict damaged reader controls reject.
[Portable raw proof](../fpga/mister/reports/2026-10-20-r-front-ready-path/README.md).
The exact native inventory verifies498 source identities and maps91 Q-storage
packages plus12 RM/STK packages. Next stage the actual native replacement at
unchanged50MHz/SYSPER4, close live producer wiring/arrivals, run all-device
timing and ROM checks on the modified native wrappers, then original-firmware
Cedar/input. The older generic-board ROM fixture is insufficient for changes
confined to native wrappers. Clock acceleration follows that integration.
The component jobs are terminal. Actual native replacement source is now
staged at unchanged50MHz/SYSPER4. Its full-backplane startup diagnostic passes
2M host cycles/499,911 root edges/359 paused samples/1,998,976 alias checks,
zero unsettled states;9 CP strobes but no instruction issues in that short
power-up interval, so it does not qualify Initial/Cedar. The Initial-ROM gate
now passes using the EXACT changed native wrappers:960 words read back,15
task initializations and all six error sources enabled for2,611,683 samples.
A second run with live alias/unsettled/repeated-event assertions also passes;
434 frozen inputs/run verify. Full native timing/memory review now passes:
31 summaries/196 legacy plus136 central-Q targeted reports/four conditions,
minimum+0.118ns; summary setup+1.333ns/hold+0.249ns. All512 inputs unchanged;
30,933ALMs/52,481regs/331M10Ks/35DSPs. Four256x9 banks and every other memory
contract pass; cache3/5 alone shares a previously reviewed pair. All retained
disk/Ethernet state matches, with only one zero-state combinational gate gone.
Eight PHYSICAL instruction banks also enforce OLD_DATA after a negative test
exposed a missing reader check. Eleven final damaged-evidence controls reject.
Builder138712, fitter146109, serial reviewer165785/STA182575 and all stages are
absent. Assembly201198/201199 succeeds; RBF57a552af is staged/hash-verified and
now tested on14. The first original-firmware attempt requests BFN113 twice,
receives microcode and enters Cedar/Mesa trap/transfer routines, but never
reaches login. Three ARM track reads are acknowledged in~3ms; no pack/network
errors or disk writes. Preserve this as an unqualified hardware regression,
not a service timeout or speed success. The first CLOSED pack remains exactly
the original seed;3862 complete trace records/no gaps/two loader barrier_ends.
A repeat from another original seed on Main16808/server16807 reproduces the
same Mesa trap loop and three completed reads, with unchanged binaries/RBF. An overlong launcher initially caused a stock-Main
re-exec; exact stopped-boundary resume fixes its name without another FPGA
load. Keep that setup interruption in the receipt. Both corrected matched Mac
fixtures fail before the Cedar handoff. Follow-up CONTROL traces show packet0
accepted and copied, then EOF followed by a checksum return/retry. Wire readback
matches all15,688 original EB words with valid CRCs and a zero image checksum.
The late SeqNo0 dump follows a restart, not initial packet rejection. These
generic-memory/direct-ROM-entry fixtures cannot establish the cause of the
FPGA's later Mesa trap loop. PCT operands at a CIA change are unsettled; use
the new passive phase trace to inspect consumed data. It confirms the first240
checksum words and16 mismatching MD words in a later isolated range. Native RAM
package shadows pass both16M Initial and50M through receive/checksum, with a
rejected shadow-only data fault. The control binding the exact deployed memory
controllers now reaches Cedar BootOrStart, then waits in MesaIFUNotReady through
55M. Original Initial passes; full loaded IM/IFUM readback remains unverified.
Its raw MSA/ROM-entry fixture is still not a hardware boot. See terminal results
in the handoff. Establish
a passing control, then debug the first storage/producer divergence and qualify
same-clock Cedar/input before accelerating.
No physical speed gain claimed; last QUALIFIED login figure remains1.947686M
issues/s. Current50MHz/SYSPER4 ceiling6.25M/s; target16.667M/s before waits.
Authentic rollback, persistence, three fresh boots and2h stress remain open.

Native V15 now isolates a CPU-integration regression: preserved MC1668 native
CPU wrappers pass3,600 loaded IM words/256 IFUM entries and Cedar entry238;
new Ready/front CPU wrappers fail3,856 items without entering Cedar. I/O, memory,
IFU helpers and the original stream/pack are identical. Both pass Initial.
RAM shadows with actual memory controllers pass50M stored-word/closed-write
checks and settled asynchronous read-pin comparisons. A three-sample Q filter
missed20 packages and is explicitly rejected as incomplete; a one-sample20M
capture probe observes all91 packages but ends before the download. Full50M
capture and paired cache/storage line traces are now terminal. Q3 passes all91
packages through50M (minimum2,107,861 checks/package); it remains an open-loop
settled-input check, not a closed-loop board-timing qualification. V17 retains
the native baseline's full-image PASS and the candidate's3,856 mismatches.
The first line observer matched an idle I/O address as well; its two V16 jobs
were deliberately stopped and their full logs preserved with verified gzip
readback. Corrected V17 narrows the predicate to the four boot-image lines and
bounds emitted rows. Only observer bench/helper inputs change; the other419/
434 inputs remain identical. V18's matched native inspection at13.9M now finds
all256 RM words identical but8,191 map entries different: the candidate's
real-page fields are zero. In4,000 storage samples, VA/Hold/RAS/CAS/write strobes
match while every sampled DRAM latched-address vector differs. These are raw
row/column indices, not virtual page numbers.
V19 catches a physical capture failure: at sample9,393,607 the candidate's B
bus drops from page1 to0 for one host sample. MapBuf closes at9,393,608 and
retains0, even though B has returned to1. The baseline remains stable and writes
page1. The candidate's later pages in the same window also store0. This is
component evidence of the failed map-data handoff, not yet the FPGA's complete
failure diagnosis. Both V19 runs finish14M with original Initial checks and
6,000 phase rows. All map control/clock/address records in the window match;
22 of23 sampled map writes differ. V20's added operand observer fails compile
because overlapping vector-name substitutions generate an invalid net. V21
fixes only that observer and adds passive R/dR/dRm, bypass and R/B-mux
clock/select visibility to identify the producer epoch before changing RTL.
The [map capture receipt](../fpga/mister/reports/2026-10-21-r-front-map-capture-diagnostic/README.md)
preserves terminal observations, the failed observer and verified live V21
prefixes, with lossless readback and receipt-only analysis replay.
V21 is now terminal and confirms the candidate exposes R=0 one host sample
before the baseline while the identical retained B selector still selects R.
V22 restores only the four original visible-R latch packages (16 bits) to test
this boundary; source/helper/observer identities are audited. It now finishes
14M with original Initial checks: all16,384 map entries/all256 RM words match
the native baseline, and all6,000 sampled R/B/BMux/MapBuf/write-data records
match. All23 sampled writes are correct. V23 runs the
same diagnostic through the full original-image gate. Predicted central R is
still an internal duplicate, so neither is a reviewed production candidate.
Their current handles and the archived producer extension are in the handoff.
The correction now has a single-owner representation:736 evaluator Qs plus16
original visible-R latch Qs, exported through explicit board ports. V26 passes
original Initial and exact16K map/256 RM comparisons; V27 passes the full
original3,600-IM/256-IFUM image handoff,640 matched CPU receive records and the
native oracle's final RM/map dump. Actual native startup/pause and synthesis
memory reviews pass. The direct-ROM fixture still ends at the oracle's task12
wait, so it is not a Cedar desktop gate. A v2 synthesis attempt rejected
hierarchical R references; its inputs/logs are preserved. V3 finishes all builder stages but is rejected for a real -0.031ns storage
hold path at slow1100mV/100C (`u_storage_5|old_data[5]` to `read_pipe[5]`,
ungated host to gated machine clock). It must not be assembled or deployed.
The supplemental R reporter initially expected16 physical registers; corrected
classification covers16 originals plus11 named fitter copies. All160 additional
central/R reports pass (+0.163ns minimum), without overriding negative global hold.
The rejected fit and separate corrected reports are closed and preserved.

V4 now PASSES full timing/memory/source review:31 summaries/196 legacy/
160 central-R paths/four conditions, minimum+0.062ns, global core setup+1.418ns/
hold+0.317ns. All513 compiled sources verify;17 damaged controls reject. Its
unchanged RTL/ROM/original SDCs retain50MHz/SYSPER4, with100ps additive hold
uncertainty explicitly present in20 sampled central paths/corner. One synthesis
warning records skipped timing-driven synthesis; full fitted STA runs/applies
the margin. Five unchanged host-video shift pipelines remain in registers:
all four sources match baseline/272 named bits retained/153 remaining RAM rows
and all153 native memory parameter tables match. All native memory families
pass; the seven freed M10Ks explain324 total.16 primary R bits+13 fitter copies
are all accounted;705 central physical registers remain. No Dorado RAM or timing
check is waived. All build/report jobs are terminal; guarded assembly succeeds.

Reviewed RBF7a90adb9 is now normally loaded on14, unchanged Main32012/server32011,
fresh original seed in `CedarRVisible4_20261021`, Main sole SPI owner. Original
BFN113/C release and actual Pilot812 pass, but login/Viewers/input do not yet.
Five real disk requests have matching ACKs in3.157..3.213ms, no writes/zero ring
drops at the passive observation.16 PC snapshots find task0 mostly239/23a and
6c1/6c2. Continue this original boot and diagnose the next native handoff before
acceleration; do not replace evidence with an assumed slow boot or service timeout.
The on-disk trace has preload only; the requests are in Main's live RAM ring.
No debugger-induced gap is a speed/cache measurement. Boot2 pack/logs are
closed/preserved including its one explicit transition trace gap.
[Qualified fit and first firmware-boot evidence](../fpga/mister/reports/2026-10-21-r-visible-hold-closure/qualified-v4/README.md).
Last qualified login remains1.947686Missues/s; no new speed gain/rollback claim.
Baseline's later09c
wait is task12 starvation after2,153 emulator samples, not established IFU
failure or a desktop boot. Current process handles/receipt paths are in the
handoff. No hardware speed gain, clock/Main/daemon change or rollback claim.

### Milestones and acceptance

| Milestone | Change | Evidence required to finish |
| --- | --- | --- |
| 0. Preserve the working speed baseline | Use qualified Native59 with state administration inactive. Freeze its original firmware, Main/server binaries, benchmark inputs and original-control-strobe issue counter; reconstruct from its archived compiled inputs. | Original Native59 timing/hardware receipts remain authoritative. Restaging checks all413 original compiled inputs and498 archived baseline files. Keep issue counts distinct from retirement and edge ceilings. Recheck deployment identity and use pinned workloads for subsequent comparisons; cold persistence and repeated boots remain final gates. |
| 1. Move bulk RAM to SDRAM | Add a selectable SDRAM raw-storage backend, initially retaining the same 16 MiB padded data/ECC layout for 8 MiB guest RAM. Derive controller geometry, initialization, refresh and IO timing from an appropriate MiSTer 128 MB controller/module reference. Keep the DDR backend selectable. | Independent storage/ECC/read-before-write/reset/stall fixtures agree. Constrain the new SDRAM clock and IO, pass all-device timing, then cold-boot Cedar and verify input and persisted disk writes. Compare with the same-frequency DDR control. |
| 2. Reduce memory request overhead | Use measured traffic to improve grouping, locality and the single-outstanding frontend. Investigate unnecessary raw fetches on address changes; change them only after proving the original DRAM output pipeline remains correct. | Fewer raw operations and/or shorter waits on pinned workloads, with storage equivalence and original firmware preserved. Demonstrate a repeatable end-to-end improvement; increasing buffer capacity alone is not an acceptance criterion. |
| 3. Keep disk I/O responsive | Adapt the Mac write-coalescing approach and move blocking file reads/writes/flushes to a bounded ARM worker. Main remains the sole SPI owner and handles explicit completions. Separately reduce whole-track write amplification where guest sector/CRC semantics permit. | Read-after-write, neighboring bytes, eviction, remount/core-load draining and write errors pass. An injected multi-second file operation does not block Main's poll/input service; target under 100 ms poll gaps in that controlled test. Real Cedar disk writes persist after close/remount. |
| 4. Connect the daemon through DDR | Adapt the Mac/Sun frame-ring pattern for direct `dorado-server` access. Reserve a non-overlapping region, retain the full 2048-byte payload capacity, define ownership/order/generation, and add an acknowledged stop/resume handshake around core loads. Keep SPI/socket transport selectable during qualification. | Random DDR stalls, full/wrap, malformed lengths, reset/reload and stale-generation tests pass. Original DskEth/CRC/wire timing stay in RTL. Boot-file and STP payloads match, Cedar cold-boots, and packet delivery continues while Main has delayed disk work. |
| 5. Finish accurate clocks | Integrate the independently checked phase contract and T-control cone, then remaining processor/control/IFU/map/cache latch families. Calibrate BaseBoard, Junk/RCLK, display, Ethernet and disk timers together. | Board/microengine and extended-instruction checks, original-ROM startup, all-device timing and cold Cedar/input pass at each step. Demonstrate the original 60 ns nominal ordinary-instruction cadence and report actual instruction progress, holds and external waits separately. |
| 5a. Qualify authentic Cedar Checkpoint/Rollback | Use Cedar's own Checkpoint command, boot/germ outload/inload and disk checkpoint file. Reuse the software-emulator test procedure without importing emulator state. | A quiescent desktop checkpoints and repeatedly restarts on FPGA with working applications, exact keyboard echo and mouse/click. Files created or modified after a checkpoint survive rollback, as Xerox specifies. Measure startup time and verify close/remount and a fresh power/core-load startup with the saved volume. |
| 6. Qualify and release the combined result | Combine individually qualified speed changes, repeat pinned benchmarks, run sustained disk/network/input activity, and publish reproducible artifacts. Full hardware snapshots are deferred. | Three consecutive fresh-seed original-firmware Cedar desktop boots; repeated authentic Cedar rollback; exact short and alphabet/digit echo; mouse/click; verified disk persistence; at least two hours sustained operation without pack/CRC/RX faults. Archive timing, traces, captures, measured speed and remaining limits; commit and push. |

### Immediate work after resumption

Live update: the interrupted MC1668 load was continued from its verified stopped
boundary, preserving the old volume and its explicitly incomplete historical
trace. Main3486/server3485 now run the corrected same-clock candidate. Original
firmware/Cedar login/all34 base transfers/final guest ACK and Viewers/mouse/click/
exact keyboard/warm file readback pass; idle issues average1,947,686.01/s,
effectively unchanged. No speed gain, cold persistence or rollback claim yet.
The complete596-bit R sign/odd component now couples functional RM/STK ports
(9,216 bits in four256×9 MLAB banks). Both simulators and isolated15ns timing
pass (setup+4.190ns/hold+0.105ns). The final R address/select stages now pass
separately at636 bits/45 clocks in both simulators and isolated15ns timing
(setup+3.982ns/hold+0.155ns). Their coupled functional RAM handoff now passes all
six primary profiles and15ns timing (setup+3.241ns/hold+0.136ns). Extra directed
startup/control checks also pass six profiles in both simulators. The old g16
sample and original write-window stability are explicit gates. Remaining
front/bypass/IFU/Ready/MIR/Hold producers
and native clock integration remain open. Exact evidence
and process state are at the top of the handoff.

The front RBase/stack/bypass extension passes six profiles in both simulators
at736 processor bits/57 clocks, with11,236 events/profile. All16 RBase slots,
all256 expected pointer loads, every added bit/clock, resolved sources and stable
write windows pass. Raw decoder/adapters pass4,352 checks in both engines; seven
bad variants reject. Its ordinary-port synthesis representation passes another
16,384 arbitrary-state/input comparison in both engines. Both fits are terminal:
observation-inclusive v5 misses15ns setup by3.041ns; full functional-interface v6
still misses by2.617ns (hold+0.156ns). V6 keeps every state/event/port/invariant bit,
leaving only210 post-state observation bits unregistered. Its actual18-level
critical path is task priority/current-task feedback into `unsettled`. The task
epoch v2 correction described above now closes this isolated budget while
retaining the check and original15ns constraint. Next qualify the actual control,
MIR, Ready/IsNext, Hold and IFU producers before native integration.
See the handoff and [component contract](verilog-r-front-contract.md). All jobs
are terminal; hardware remains unchanged. Speed,60ns whole-board timing and
rollback remain open.

The source/report receipt is [archived here](../fpga/mister/reports/2026-10-20-r-front-path/README.md):
4,821 raw members, lossless readback/extraction and identical replay of both
rejected timing reviews. The next timing target is the task core's priority/
Switch/CurrentTask feedback; make its original capture epochs explicit and
requalify all callers after changing it. Goal ACTIVE, all jobs terminal.

1. Preserve the unintegrated host-service/Main-client prototype as deferred
   work. Its v6 component evidence passes, but native wiring and the actual
   Main coordinator do not exist. Do not expand that work for this goal.
2. **Done:** reconstruct and hash-check the archived Native59 normal-operation inputs.
   Retain SDRAM, disk-worker/cache and direct-DDR network behavior and the
   original pin, memory and timing contracts. No snapshot-only virtual controls.
3. Continue the original clock-family migration: the coupled32-Q phase/run/
   refresh component and its isolated15ns timing now pass (receipt below).
   **Done for an isolated slice:** qualify Hold/repeat and processor T/task
   captures with this controller (64 Qs), then actual task-selection feedback
   and task-file addresses (88 Qs; conditional arrival contract below). Next
   **Done:** T data/write/read windows with distinct per-task values (404 bits;
   conditional arrival contract below). **Done for an isolated slice:** T parity
   and R parity checking at its data/RAM-parity boundary (540 bits; below). Next
   **Done for an isolated slice:** R operand latches/local bypass/sign Qs (592
   bits; below). **Done:** actual native MC1668 copy and Original Initial
   component regression. Full same-clock candidate passes all-device build/memory/timing review
   (`Dorado_Mc1668_20261020`, RBF15730f44; job terminal, do not restart).
   **Done:** normal load continuation, actual original firmware/Cedar login and
   base34/finalACK and the SAME boot's Viewers/mouse/click/exact keyboard/warm
   marker for this correction. Cold persistence remains a final gate.
   **Done for an isolated slice:** complete596-bit sign/odd nets, both simulators
   and15ns timing (setup+4.242ns/hold+0.135ns). **Done for an isolated slice:**
   functional RM/STK data/parity ports coupled to this component; real MLAB
   implementation passes15ns (setup+4.190ns/hold+0.105ns). **Done separately:**
   final R address/select register/mux graph,636 bits/193 packages; all45 clocks
   and256 addresses per port, both simulators/five profiles/seven bad variants;
   isolated15ns setup+3.982ns/hold+0.155ns. Include all four mixed low-address
   drivers and preserve DblClock'B's different StartA qualification. **Primary
   simulation/timing PASS:** these actual address/select stages now drive functional
   RAM, preserving old g16 data and321 stable original write windows per half;
   every636 processor/9,216 RAM bit is compared after every event in six profiles.
   First fit failed-26.346ns from an unrolled arbiter preview; the separated
   pre-event clock/address cone passes15ns at+3.241ns setup/+0.136ns hold. Extra
   directed startup/control qualification also passes1,680 events/profile in both
   simulators across six profiles. Sixteen damaged reader cases reject;2,315
   raw receipt members are losslessly archived/read back/extracted, with positive
   and rejected reviews reproduced byte-identically. Next front/bypass/IFU and
   Ready/MIR/hold producers, then remaining
   board/latch/RAM families against original references. The existing
   eight-slot phase oracle is not a deployable full-speed clock solution.
4. Fit only a justified native candidate, pass all operating conditions and
   SDRAM I/O timing, then perform original-firmware and pinned speed tests.
5. Qualify Cedar's original checkpoint/rollback and final combined gates above.

Deferred FPGA snapshots remain valuable for arbitrary software and debugging,
but require a separate future milestone and explicit acceptance scope. Cedar
rollback restores the software world, not disk contents or a cycle-exact set
of FPGA/ARM internals. See [the original guide](../DoradoDocs/manuals/Introduction_to_Cedar_7.0.md)
and [our existing C-emulator checkpoint gate](cedar-checkpoint.md).

### MC1668 native full-build review PASS (2026-10-20)

RBF 15730f44 passes exact-source, complete memory/OLD_DATA and four-condition
timing review: 31 summaries / 196 detailed categories, minimum +0.115ns, no new
constraints or clock/I/O coverage change. Six damaged evidence controls reject.
The job and all ten recorded processes are terminal. Resources 30,468 ALMs /
52,881 registers / 332 M10Ks / 35 DSPs. Only two MC1668 sources changed; 50MHz/
SYSPER4, Main/server and bulk-memory/network/disk policies remain unchanged.

Next: normal Main loader/old-volume preservation and fresh original-firmware
Cedar boot/input/persistence on 14. No new image has been deployed yet. Follow
current source/image paths and steps at the top of the handoff. Receipt:
`fpga/mister/reports/2026-10-20-mc1668-native-build/`. Clock integration toward
60ns and the rollback/final gates remain open; full snapshots stay deferred.

### Actual native MC1668 copy and ROM regression (2026-10-20)

The twelve FPGA positions use a generated cell copy. The new stager changes
that copy and the generic cell, preserving all other Native59 inputs, inactive
administration, 50MHz/SYSPER4, services, storage and constraints. Both simulators
pass 1,425 manufacturer-table observations plus 4,272 paused advances; three
broken copies reject. Initial's F43 component fixture passes 15 task initializations
and 2,611,683 samples with all errors enabled. This is not a native BaseBoard boot.

The same-clock full candidate is live at
`/home/alans/mister/Dorado_Mc1668_20261020`; builder 3979425/fitter 3983817 were
confirmed alive after routing. Observe the existing job. Full fit/memory/four-
corner/crossing review and physical Cedar boot/input/persistence remain pending.
No new image has been loaded; last measured hardware rate remains 1,947,654.12
issues/s. Details and continuation commands are at the top of the handoff;
completed component evidence is in `reports/2026-10-20-mc1668-native-regression/`
under `fpga/mister/`. Full snapshots remain deferred.

### R operand staging and a real latch-model correction (2026-10-20)

**MC1668 is HIGH-transparent, not edge-triggered.** The original Motorola table
also gives its open clock priority over S/R. The production cell is corrected;
both simulators pass 1,425 truth-table observations and reject four bad variants.
The part appears at ProcH/ProcL l06+l07 and eight MemC address latches. Original-
firmware/cache/processor regression is required before deploying this correction.

The new coupled R/T/task/parity slice covers 592 original bits, including R's
Pd/Md data, file/IFU read and visible operand latches plus l07's local state.
Both simulators pass 23,672 events per profile across five profiles; eleven bad
variants reject and one scoped old-bypass-data equivalence passes. Isolated 15ns
FPGA timing passes all four conditions (setup +4.450ns, hold +0.159ns). This does
not qualify native 60ns operation. RM/STK functional read data/parity and bypass/
IFU producers remain boundaries; storage/address/write and complete sign/odd
flags (l06's other contributions) remain open.

Detailed scope, measurements and source anchors are at the top of
[the handoff](verilog-handoff.md). Receipts: `reports/2026-10-20-r-operand-path/`
and `reports/2026-10-20-mc1668-latch/` under `fpga/mister/`. Native59 remains at
1,947,654.12 instruction issues/s; no new core/Main/daemon was deployed.

Next: original-ROM/native regression for MC1668, then functional RM/STK ports,
l06's flag contributions and remaining Ready/MIR/Hold/board timing, followed by
native integration and the Cedar boot/rollback/final gates. Full snapshots stay
deferred. Large RM/STK arrays stay behind RAM ports.

### T parity and R parity checking qualified (2026-10-20)

`dorado_t_parity_step.sv` extends the coupled T path to 540 original bits:
T-parity SRAMs, both complete g16 packages, T/R checkers and the actual low-half
fault capture into g18. Its independent wire-list graph covers 150 packages /
1,506 connected pins. Clock1 samples the OLD parity address before the task
address changes. R data/RAM-parity outputs and R bypass/IFU masks remain boundary
inputs; R storage and the inherited Ready/MIR/Hold producers are not qualified.

Both simulators agree on 23,400 checks per profile across five arrival profiles.
All 540 bits, 25 clocks and parity/fault/window outputs are compared. Directed
injections cover 16 T-data faults, two stored T-parity faults and 16 R-data faults,
with 33 mask checks. Twelve compiled bad variants reject. Both halves retain 583
write windows with no address movement inside a write window.

Three profiles retain the prior illustrative delays. Two targeted correlated
controls additionally use PARC's original `ecldict.analyze` annotations for
F10000 clock-to-Q, MC10170 A and SRAM read delay. They enforce the g16 data
setup/hold limits 1.7/1.1ns; minimum measured setup is 44ns and hold 3/ 10.9ns.
Both simulators record 16,400 setup and 10,700 hold samples. The inherited clock
network remains illustrative: this is not full-board min/max/skew, analog or
60ns qualification. An initial monitor counted constant initialized pins
differently; explicit observation epochs correct that without changing RTL.

One terminal isolated 15ns Quartus fit passes all four conditions: setup +4.470ns,
hold +0.139ns, pulse width +6.548ns; evaluator setup at least +4.470ns. The virtual-
I/O shell uses 1,232 ALMs / 2,001 registers, no RAM/DSP/PLL; targeted collections
cover 725 launch / 583 result registers. No exceptions, assembly or deployment.
All five recorded processes were absent at collection. The independent reader
matches all three RTL dependencies to executed inputs; ten damaged/rehashed
controls reject and all four prior reader profiles still pass.

Receipt: `fpga/mister/reports/2026-10-20-t-parity-path/`. Frozen executed sources
and reports are losslessly archived and read back. Extracted timing review and
reader controls reproduce; a second full archive simulation run is not claimed.
Native59 remains at 1,947,654.12 instruction issues/s; no new core/Main/daemon
was deployed. Complete FPGA snapshots remain deferred.

Next: qualify the R read/IFU/bypass latches with functional RM/STK ports; then
remaining producer/board timing and native integration from Native59. Keep large
RM/STK arrays behind RAM ports rather than whole-state transport. Original-ROM
boot, speed, Cedar rollback, repeated fresh boots, cold persistence and two-hour
stress remain open.

### T-data, task-file writes and visible T reads qualified (2026-10-20)

The new `dorado_t_file_step.sv` couples the unchanged 88-Q task/capture kernel
to both g18 HoldDly packages, k01/k02 transparent T-data latches, four 16x4
T-file SRAMs, j03/j04 file/IFU read latches and i03/i04 visible T latches.
The independent wire-list extraction covers 404 original bits in 139 packages /
1,403 connected pins. Pdata/dMD, IFU data/sign/control, unused g18 inputs and
the inherited Ready/MIR/memory-Hold producers remain explicit boundaries.
No native core, Main or daemon is changed.

Both Icarus and Verilator pass 21,962 events in EACH of the three illustrative
arrival profiles from the prior task slice. All 404 bits and 25 clock channels,
write/latch/bypass gates and task/hold flags are compared. Directed tests write
and rewrite distinct values in all 16 tasks through Pd, Md and FF075 TgetsMd;
perform 16 held-write checks; revisit every task through the real read latches,
including a switch down to task 0; and check signed IFU T input without changing
SRAM. Random ordinary/extended instructions cover all LC values. Each run sees
1,698 transparent data updates (462 Md / 1,236 Pd), 113 changed-memory
observations, 593 write windows per half, 3,612 closed-latch data probes and
558 delayed-Hold blocks. Neither half's SRAM address moves inside a write window.
Seventeen compiled defective variants reject; two scoped equivalence controls
pass. Archive-extracted replay reproduces the full suite.

The new data/write non-overlap assertion explains the equivalence controls:
under the tested running contract, using the old or settled T-data image for
a write gives the same value, and the low write gate can use the masked high
control without changing these observations. This does not authorize merging
the original high/low capture FFs or removing their startup/producer contracts.
The original latches and both control paths are retained.

The expanded graph also checks every consumed net for omitted wired-OR
drivers. ProcH g18 D5 needs BOTH f20's FF076 decode and h17's Freeze contribution;
previously that output was unused by the slice. The reference now includes both.
The first asynchronous-SRAM reference used nonblocking assignments in a
combinational process; Verilator warned about converting their semantics and
exited without a passing receipt. The accepted reference uses explicit physical
pin sensitivity and blocking level-sensitive SRAM updates; the mux latch keeps
its explicit arrival delay. Both simulators agree. Failed coverage/diagnostic
runs and their frozen source inputs are retained as history, not qualification.

One isolated 15.000ns Quartus17.0.2 fit on misterubuntu passes all four operating
conditions: minimum setup +4.378ns, hold +0.179ns, pulse width +6.548ns; internal
launch-to-result setup at least +5.336ns. Resources including the staged virtual-
I/O shell are 941 ALMs / 1,526 registers (562 ALMs contain virtual pins), no
RAM/DSP/PLL. Targeted collections cover 553 launch / 439 result registers. The
shell preserves 2ns max / 0ns min I/O and adds no native pipeline stage. There
are no timing exceptions, assembly or deployment. The job is terminal and all
five recorded processes were absent at collection. The independent reader checks
BOTH RTL dependencies against executed simulation inputs; ten damaged/rehashed
receipt cases reject, including a changed child kernel. Prior 32/64/88-bit
reader profiles still pass. Archived timing review and controls reproduce.

Receipt: `fpga/mister/reports/2026-10-20-t-data-path/`. This remains a conditional
digital arrival contract with 20ns boundary settling and NoDispatch low, not
vendor delay characterization, analog setup/hold, complete processor/boot
qualification or a 60ns operating result. T/R parity, the ALU and register-file
producer paths, Ready/MIR/memory Hold, startup/freeze coupling and remaining
board/peripheral timing are still open. Native59 remains the hardware reference
at 1,947,654.12 instruction issues/s. Complete FPGA snapshots stay deferred.

```
make -C verilog t-file-test T_FILE_OUT=build/t-file-new
```

The subsequent parity qualification above supersedes this section's next step.
The original T-parity investigation started at k04, ProcH i11 / ProcL i13 SRAM, g16 capture and d06 check; c06 checks
R parity. Preserve g16's setup data across its Clock1/address handoff. Keep
large RM/STK arrays behind functional RAM ports rather than expanding a whole-
state transport; full snapshots are deferred. Preserve
ordinary/extended instruction semantics and verify original-ROM diagnostics,
firmware boot and actual instruction progress. Cedar rollback, repeated fresh
boots, disk close/remount persistence and two-hour stress remain acceptance gates.

### Task-selection feedback and task-file addressing (2026-10-20)

The clock migration now has an 88-Q coupled task/address component,
`dorado_task_capture_step.sv`: the prior 64 Qs, complete h08/h09 BNT registers,
f14/f15 Switch/SwitchUp/TPCBypass packages, j08/j09 CTask packages, and the
ProcH i12 / ProcL i14 CurrLast address registers. Its independent original
wire-list graph contains 115 packages / 1,102 connected pins. Actual Next,
SwitchUp and CTask feedback is internal. The four prior clock/transfer functions
are reused exactly except the clock-function name. Ready/IsNext storage, MIR
block/tasking decode, maintenance and memory-Hold producers remain boundaries.

**A new capture-order bug was caught before native integration.** Updating
Switch/Next and then using that live value for processor captures takes the
incoming task early. Processor capture must preserve the task setup value
from before the root event through the phase-Q/SHCP cascade. The new evaluator
does so without adding a pipeline register. CurrLast uses each half's actual
DblClock (buffered pp clock), and LastNext uses the negative phase-Q select;
Clock1 is not the CurrLast capture clock. A probe from frozen inputs shows why
a zero-delay graph was insufficient: Icarus rejects the live-source defect
at task 1, while Verilator accepts it for the full run. The explicit-delay
original graph rejects it in both. The probe and its source/logs are archived.

Both simulators now agree on 73,761 ordered events in each of three arrival
profiles: gate/ordinary-FF/SE-phase-FF delays of 1/1/1ns, 0.5/1/0.5ns, and
1/2/1ns. Each run compares all 88 Qs, 25 clock channels and task/hold flags;
all 16 hardware wakeups, 706 held-task checks, 304 priority-yield checks,
61 split T-capture events, 675 held fetches, 289 resumed fetches, 1,043 events
with Next changing around processor capture, and 5,931 address changes pass.
Fifteen compiled bad variants reject, including the live-source defect,
priority/Ready-mask errors, wrong CT clocks, blocked held-task priority,
merged Hold nets, and wrong CurrLast clock or phase select.

This is a **conditional digital arrival contract**, not characterized vendor
or board timing. The delay profiles are illustrative, explicitly recorded,
and do not establish analog setup/hold or a 60ns operating rate. Tests allow
20ns between boundary updates and settling observations, exercise normal and
extended running with NoDispatch held low, update Hold/StopTasks in the high
clock window, and exclude simultaneous CTask asynchronous set/reset. The
separate controller checks remain authoritative for that component; coupled
startup/freeze/producer timing is still open. Provisional arbitrary low-window
control tests and overlapping set/reset cases are retained as unqualified history.

One isolated 15.000ns Quartus17.0.2 fit on misterubuntu passes all four operating
conditions: setup +5.058ns, hold +0.151ns, pulse width +6.555ns, internal evaluator
setup at least +8.057ns. It uses 309 ALMs / 470 registers including the staged
measurement shell, no RAM/DSP/PLL. Targeted timing collections contain 187
launch / 115 result registers. I/O retains 2ns maximum / 0ns minimum delays;
there are no timing exceptions, assembly or deployment. This measures the
FPGA combinational budget, not the original arrival profile. All five recorded
job processes were absent at collection. The timing reader and nine damaged
receipt controls pass; the old 32-Q and 64-Q reader profiles still pass.

Receipt: `fpga/mister/reports/2026-10-20-task-capture/`, with frozen inputs,
raw losslessly compressed logs/reports, a zero-delay diagnostic, and archive
replay of all three profiles / both simulators / fifteen bad variants. Native59
remains unchanged at 1,947,654.12 instruction issues/s; no new hardware speed,
full processor or original-firmware boot qualification is claimed.

```
make -C verilog task-capture-test TASK_CAPTURE_OUT=build/task-capture-new
```

Next: connect the real T-data transparent latches, delayed Hold and the T-file
write window to these task addresses; check per-task data across switches and
holds. Complete the Ready/MIR/hold producer and remaining board/memory timing
boundaries, then integrate with Native59 and qualify original-ROM diagnostics,
firmware boot and measured speed. Authentic Cedar rollback, repeated boots,
cold disk persistence and two-hour stress remain open. FPGA snapshots stay
deferred; necessary Main changes remain isolated.

### Coupled processor capture slice qualified (2026-10-20)

`dorado_processor_capture_step.sv` extends the qualified controller to all
64 Qs in complete original packages: ContA's 32 plus each processor half's
phase pair, six T-control Qs and Curr/Last task registers. The independent
wire-list extraction has 70 packages / 642 connected pins. It audits the
BaseBoard common-clock fanout and the named ContA-to-processor backplane
connections; the four StartCycle/clock-enable seams have different source/sink
connector positions, so a straight-through pin assumption would be wrong.
The original 32-Q controller functions are reused byte-for-byte except names.

Both Icarus and Verilator pass 74,872 ordered binary events, comparing all
64 Qs, 22 clock channels and nine hold/repeat/task flags. Coverage includes
all 16 task pin patterns, 218 held-task checks, 195 priority-yield checks,
279 differing high/low T-capture events, 211 suppressed fetch events and
197 resumed fetch events. All twelve compiled bad variants reject, including
shared T clocks, incorrect Clock1 polarity, merged Hold pins, blocked priority
yield, wrong Curr/Last capture and missing TgetsMd. Archived-source replay
reproduces the full suite in both simulators.

Original wiring findings: memory Hold is backplane E26; processor PRhold is
E42 and locally buffered. A higher-priority task may run during PRhold.
ProcH captures T controls on Clock0, ProcL on SHCP; Clock1 uses the **positive**
phase-Q through SE10212, not an inverted MC10195 assumption. The BaseBoard's
CLK.ca/ph/pl are parallel copies of one root. Hold/SwitchUp are clocked
producer outputs, exercised in the high clock window (HM section 4.9 and
ContA f15). Provisional tests using opposite-direction independent roots or
arbitrary low-window Hold/SwitchUp swaps exposed gate hazards outside that
contract; those logs are retained as history, not successful qualification.
Fixture seeding temporarily suppresses phase initialization so asynchronous
sPhase0 does not defeat the test's load of original Qs.

One isolated Quartus 17.0.2 build on misterubuntu passes all four operating
conditions at 15.000ns: minimum setup +5.671ns, hold +0.151ns, pulse width
+6.552ns; launch-to-result setup margin is at least +10.514ns. It uses
174 ALMs / 290 registers including the registered measurement shell, no
RAM/DSP/PLL. The shell stages virtual input before launch; it adds no native
pipeline stage. Delays remain 2ns maximum / 0ns minimum, with no exceptions.
The 23 constant result bits are the repeated-event and unsettled flags;
107 launch and 86 result registers remain after synthesis. The independent
reader verifies frozen sources against simulation, all four corners and
unconstrained paths; nine damaged/rehashed evidence cases reject. Its
previous 32-Q profile still reproduces the earlier passing timing result.
The job is terminal, all five recorded processes absent, with no assembly.

Receipt: `fpga/mister/reports/2026-10-20-processor-capture/`. Raw sources,
reports and logs are losslessly compressed; simulator objects/binaries and
Quartus databases are omitted. This is component qualification only. Actual
Next/SwitchUp/priority and Hold producers, T data/ALU/writeback, MIR timing,
analog skew and the remaining boards are outside this slice. No native core,
Main or daemon changed; Native59 remains the hardware reference at
1,947,654.12 instruction issues/s. Complete FPGA snapshots remain deferred.

```
make -C verilog processor-capture-test PROCESSOR_CAPTURE_OUT=build/processor-capture-new
```

Next: close the actual task-arbiter/hold-producer and T data/writeback ordering
boundary before native clock integration; retain the ordinary/extended cadence
checks. Then finish board/memory/peripheral timing and original-firmware boot.
Cedar rollback, three fresh boots, cold disk persistence and two-hour stress
remain acceptance gates, as listed in the active delivery plan above.

### Coupled control clocks and FPGA budget qualified (2026-10-20)

The run/refresh and instruction-phase feedback now share one tested component,
`verilog/rtl/dorado_control_clock_step.sv`. Its32 existing Qs cover all halves
of fourteen original sequential packages, including MemSH, SC&NoFreeze and
CRamClock. The independently extracted reference contains28 packages /235 pins.
Both Icarus and Verilator pass69,864 ordered binary events: all32 Qs, nine clock
channels, enable/decoded outputs,196 ordinary two-clock intervals,64 extended
six-clock intervals, single-step stop/restart and2,568 MemSH transitions while
halted with refresh enabled. Twelve broken variants reject. External controls
meet setup; internal phase/run feedback is live, not supplied by the fixture.

An initial early-exit evaluator failed isolated15ns timing (743ALMs,
all-condition setup -19.867ns). Removing intermediate stability gates preserves
the circuit functions and collapses the implementation to82ALMs. The arbitrary-
function Z3 fixed-point theorem is UNSAT for a counterexample; two invalid
stability criteria admit counterexamples. The RTL rewrite checker allows only
removal of that guard. Both simulators, all12 bad RTL cases, the theorem and
its controls reproduce from archive-extracted inputs.

Final isolated Quartus17.0.2 fit on5CSEBA6U23I7 passes all four conditions:
minimum setup +5.678ns, hold +0.190ns, pulse width +6.551ns. The evaluator's
launch-to-result setup margin is at least +11.237ns at a15.000ns clock. It uses
82ALMs /137 registers including the measurement shell, no RAM/DSP/PLL. Virtual
I/O keeps2ns maximum /0ns minimum delays; the shell stages inputs on the
opposite edge before the launch register. Its staging path has a7.5ns budget;
the evaluator retains15ns. This fixture adds no native CPU pipeline stage.

Earlier fixed-wave fits without this input staging passed internal setup but
failed a virtual-input cold hold path (-0.329ns); enabling multicorner analysis
alone did not change that fit. All rejected experiments are retained. The
original targeted-report filter also missed output-register aliases; later
reporting checks nonempty launch/result collections. The independent reader
checks source hashes, constraints, all four corners and unconstrained paths;
seven damaged/rehashed evidence cases reject. Only Quartus's version annotation
is allowed as a prepared-QSF delta. No timing exceptions or period relaxation.

Receipt: `fpga/mister/reports/2026-10-20-coupled-control-clock/`. Four isolated
builds are terminal, recorded processes absent at collection, no assembly or
programming. Functional replay and read-only timing review reproduce from the
archive. No production core/Main/daemon changed and the last qualified hardware
rate remains1,947,654.12 instruction issues/s. This is component timing, not
full-core speed or board-I/O qualification.

```
make -C verilog control-clock-test CONTROL_CLOCK_OUT=build/control-clock-new
```

Next: Hold/repeat and processor T/task capture ordering with this coupled
controller, then complete native integration/timing and original-firmware gates.
The original BaseBoard divider phase relationships and peripheral timebases
still need integration. Cedar rollback, three fresh boots, cold disk persistence
and two-hour stress remain open; complete FPGA snapshots stay deferred.

### Run/refresh control contract qualified (2026-10-20)

`dorado_run_control_step.sv` translates22 existing Qs in ContA's run/refresh
controller, including the six-bit i03 single-step/run pipelines and j04/k01/k02/
l02/l03/l04/l05/l06. It propagates derived clock events and asynchronous
set/reset to a stable boundary with a bounded combinational evaluator. It
adds no state or reset and is not instantiated in a native core.

The independent reference is extracted directly from the original wire list:
22 packages /181 pins, real combinational pin functions and independent
MC10176/MC10231/SE10231 event-driven FFs. Tied ECL outputs are ORed. All22 Qs,
processor/memory enable outputs, wired decodes and seven clock-event channels
match across66,632 binary ordered events in EACH of Icarus and Verilator.
Manual cases cover run startup, processor halt with memory still enabled by
RunRefresh, the single-step shift/stop latch, stop retention and rStop recovery.
128 seeded states drive32,768 mixed pin transitions, each separated into data
setup and clock events. Ten broken refresh/stop/pipeline/clock variants reject.

The setup contract is material: an initial fixture changed data and root clock
simultaneously, exposing the original D/clock path boundary. The corrected
fixture settles data first. This is not proof for arbitrary simultaneous data/
clock changes, analog delays or X/Z clock states. A coupled phase scheduler must
preserve pre-edge data and actual fanout ordering. Hold/Freeze, the combined
instruction-phase feedback, processor T/task capture, downstream memory refresh
activity and native timing/firmware qualification remain pending.

Receipt: `fpga/mister/reports/2026-10-20-run-refresh-contract/`. All executed
source/dependency bytes match the working files. Archive extraction and fresh
rebuild reproduce the graph, both simulator results and all ten failures.
Earlier raw setup-failure and provisional logs are retained as history.
No core, Main, daemon, PLL or measured hardware speed changed.

```
make -C verilog run-control-test RUN_CONTROL_OUT=build/run-control-new
```

Next: couple this controller and the seven instruction-phase Qs using the
original graph, then qualify Hold/Freeze and processor T/task capture ordering
before native integration. Full FPGA snapshots remain deferred.

### Speed work resumed: baseline and clock checks

`tools/stage_native_speed.py` reconstructs the Native59 normal-operation project
from frozen receipts, verifies all413 original compiled inputs against the
separate original qualification report and all498 archived baseline files, and
allows only project-root relocation in configuration. Fresh staging and a
separate verify pass succeed at
`build/rtl-executive/native-speed-baseline-20261020`. No working generated HDL
or Serial74 source is used. No new fit/deployment/speed claim.

The existing original-wiring/T-control, independent phase and divider audit
checks pass (`make -C verilog t-controls-test clock-phases-test clock-phase-audit`;
log `build/rtl-executive/speed-resume-clock-contract-20261020.log`). T controls:
8,192 comparisons,8,164 manual checks,2,048 holds; the universal-enable bad
variant disagrees4,096 times. At N4 the divider's ECP lead is1/4 period rather
than1/8 and its low window1/4 rather than3/8; at N2 the ECP/SCP edges coincide.
This reproduces the known phase-quantization defect, not a newly fixed clock.
Next: complete the original T/StartCycle/Run/extension event-order reference
before translating capture enables or reducing the divider.

### Original instruction-phase contract qualified (2026-10-20)

`verilog/rtl/dorado_control_phase_next.sv` now expresses the seven original
ContA phase-register next values as explicit capture enables: k03.B pre-start,
k04.B StartCycle, l07.B/A Phase0/1, k05.B/A Phase2/3 and j01.B Phase4. This is
a combinational next-state component, not seven additional pipeline stages;
it is not instantiated in the native core. Original sPhase0 set/reset priority
is evaluated at the supplied event boundary. No new power-up/reset state,
PLL change or native timing exception is introduced.

The original-wiring audit checks13 packages,74 pins,30 absent/pulldown inputs
and unique drivers. The pre/main clock pins have a common Boolean source
`CLK.ca' | Stop | Run'` through different original buffer chains; Boolean equality
is not proof of coincident arrival or permission to remove their ordering.
The candidate keeps separate pre/main capture inputs. Run/Hold/Freeze state,
physical fanout delay, SC&NoFreeze, CRamClock and scheduling of asynchronous
control changes remain outside this component.

Against actual combinational pin functions and independently event-driven
MC10231 flip-flops:546,739 Icarus checks (including524,288 exhaustive four-state
transitions),22,451 Verilator checks,2,048 known-state cases,20,000 mixed
closed-loop events, and exact manual ordinary2-clock /IM-TPC6-clock cadence.
All eight broken extension/cascade/clock-merge/initialization/four-state/phase
variants reject. A first fixture's cadence query read D before combinational
settling and failed its coverage count; the corrected query waits for D and
all cases pass. Earlier raw logs are preserved as history, not qualification.

Receipt: `fpga/mister/reports/2026-10-20-control-phase-contract/`.
All nine executed input files match the working sources. Archive extraction
and a fresh rebuild/replay reproduce wiring, both simulators and all eight
negative results without working RTL inputs. See receipt proof for terminal
status. No FPGA clock, Main, daemon or measured speed changed.

Reproduce with a fresh output directory:

```
make -C verilog control-phases-test CONTROL_PHASE_OUT=build/control-phase-new
```

Next: translate and test upstream Run/Stop/Hold/Freeze/RunRefresh controls and
the actual processor T-clock capture ordering before enabling this component
in a native candidate. Full FPGA snapshots remain deferred.

### Benchmark contract

The Serial74/73 entries below are preserved historical snapshot experiments.
Their pending state-access/Main-coordinator work is **not** the active next step.
Native59 remains the last qualified hardware baseline at 1,947,654.12 instruction
issues/s; no faster core or FPGA rollback has been qualified yet.


**Serial74 native phase source integrated (2026-10-20):** local stage
`build/rtl-executive/serial74-native-phase-distribution-20261020`, intended remote
path `/home/alans/mister/Dorado_Serial74PhaseDistribution_20261020` (provenance
only; no upload/job). All662 frozen Serial73 inputs are verified;663 actual
inputs include the exact qualified nineteen-register falling-edge distributor.
All479 qualified MC10176/MC10231 helpers are enabled with original PRECAP values.
The original global gate remains; no extra pipeline stage, clock or top-level
port is added. Independent inversion restores every original source, all2,761
pins, raw drain, live guards, hold/quarantine logic, periods and placement policy;
13 rehashed broken source variants reject.39 parent files change plus one added
source. No Quartus, assembly or deployment mode exists in the staging driver.

Fresh actual-QSF XML accounts for2,094,333 REQUIRED bits: physical1,957,048,
private board4,578, transport69 and wrapper ACCESS PENDING132,638. All19 new
phase bits are REQUIRED/ACCESS PENDING; all old geometry is preserved. All479
helper phase/PRECAP bindings match;34 damaged state/binding/alias controls reject.
The120 task-written SDRAM bits remain required and1,239 function scratch bits
retain the previous lifetime review. Core/UIO state, raw SDRAM bytes and ARM
remain outside this native-machine XML. Schema remains board-only af0b647cb43a3a62.
Receipt: `fpga/mister/reports/2026-10-20-snapshot-native-phase-distribution-source/`.
Archive-extracted frozen tools reproduce all663 staged bytes, fresh complete XML,
classification and all47 negative controls. Only the two XML path hashes differ;
every other classification/verification value agrees. The initial extracted
attempt lacked cell-model audit dependencies; the corrected closure is archived,
with no HDL changes. All raw bytes read back and every replayed input is retained.
Next: native state access, actual Main/service controls and remaining RAM/DMadr
timing paths before a combined fit. Native59's measured speed is unchanged.

**Local advance / payload qualification (2026-10-20):** all eleven complete
boards pass8,399,378 checks over2,104,093 host edges against their frozen
originals: original pins, classified small physical fields, sampled large RAM,
scan/read/destructive-import/restore,2,000 resumed steps per board, forced hold
and partial-import quarantine.383 MC10176/MC10231 instances opt in; other
payload/RAM families retain their existing hold checks. Default-off helpers
invert byte-exactly, including the BaseBoard serial-port variant.

The actual frozen Serial73 owner/router/memory/network/disk control cone passes
27,501 checks in two fresh simulations, including all19 scan targets, four-state
controls and parser-reset quarantine; three broken edge/permission/late-hold
variants reject. All662 parent inputs and all19 native board hold/phase hookups
are checked. The control experiment uses nineteen38-bit leaf models; it is
separate from complete physical-board qualification and not a full-native run.

All eight complete MSAs /128 raw groups and real SDRAM pin models pass
52,931,566 checks with the same4,405 guest instructions /41,760 reads /7,472
writes as the qualified reference. All12,304 original state bits, pins and raw
controls agree; four pending read/write pauses, member3 partial-import quarantine
and live refresh pass.96 MC10176 instances opt in. Wrong-edge and missing-payload-
enable variants reject. Admission requests are stable before capture, and every
request must produce exactly one guest edge; the earlier7169-edge fixture race
is retained as superseded evidence. No clock period or SDRAM timing was changed.

Receipt: `fpga/mister/reports/2026-10-20-snapshot-phase-distribution/`.
The independent verifier matches every executed board/control/MSA asset to the
current generator. An archive-extracted rebuild reproduces all eleven board
results, five control scenarios and three eight-MSA cases using only archived
HDL/headers/memory files and fresh compiler objects. All raw bytes read back;
the final receipt retains every originally replayed member.
**Follow-up: Serial74 integrates these assets and accounts the nineteen added
bits; state access, actual clocked Main controls and remaining RAM/DMadr timing
paths remain open.** No full-core fit, hardware reload or speed gain is claimed.

**Local-enable component prototype (2026-10-20):** nineteen independent
falling-edge advance registers match the existing global register without an
extra cycle. Two optional FF families (MC10176/MC10231, both PRECAP modes)
match frozen Serial73 originals across all38 physical bits:86,145 checks,
21,212 edges,2,926 phase transitions,30 scan requests,16,384 four-state
default-off cycles, destructive import/restore and partial-import quarantine.
Six broken phase/permission/payload/pre-capture/hold/write variants reject.
Earlier fixture compile/coverage failures are retained alongside the passing
v4 run in `fpga/mister/reports/2026-10-20-snapshot-phase-payload-component/`.
This proves only a component with a synthetic synchronous hold producer.
The optional payload simplification requires advance to imply no hold, and
scan-write to imply hold, at every capturing edge. **Native producer proof,
complete-board/RAM/SDRAM tests, state access and routed timing remain TODO.**
No native integration, Quartus build, deployment or speed change is claimed.

**Serial73 is terminal:41,642/41,910 ALMs; timing REJECTED at setup -1.095 ns /hold -1.326 ns. Registered readiness-to-advance now passes at +1.940 ns, but outgoing advance/predictor, snapshot-hold, virtual controls and additional board-data paths still fail. Main coordination, full snapshots, faster clocks and final FPGA qualification remain open.**

The last qualified hardware is Native59: Cedar Viewers/mouse/exact keyboard and
warm disk persistence pass. Its last qualified rate remains **1,947,654.12
instruction issues/s**. This work has not deployed a new core/Main/daemon or
produced a new speed measurement.

**TERMINAL Serial73ReadyPrediction (2026-10-20):** remote
`/home/alans/mister/Dorado_Serial73ReadyPrediction_20261020`. Builder2721605,
fitter2723784 and all project processes are ABSENT. Map, fit, ordinary STA,
clock/IO audit, four device conditions and304 board-crossing groups completed
sequentially. Only after all of those processes ended, reporting-only
STA2745148 measured32 readiness path groups; it completed successfully and
confirmed all eight actual aggregate registers in every condition. All662
prepared inputs remain exact. No assembly, RBF/SOF or hardware reload.

Capacity passes:41,642/41,910 ALMs (268 free),52,561 fitted registers,
329 RAM blocks /2,498,471 bits,35 DSPs and3 PLLs. This is178 ALMs above
Serial71. **TIMING REJECTED:** setup -1.095 ns at slow100C /-0.651 ns at
slow-40C; hold -1.326 ns at slow-40C /-0.963 ns at slow100C, also negative
in both fast conditions (-0.426/-0.189 ns). SDRAM IO passes (minimum+2.020 ns).
Actual clocks/phase match Native59; zero illegal/unconstrained clocks and
exactly the inherited3 input /50 output framework IO exceptions. This is not
complete physical framework IO coverage.

The registered predictor achieved its intended cut: cache-to-`network_step`
setup is positive in all conditions (minimum+1.940 ns). Its hold paths also
pass. The remaining half-cycle outgoing `network_step` path is now critical:
-1.095 ns to MemX MosRam write-enable, with additional BaseBoard/MemC enables;
`network_step` back to the predictor register is -0.965 ns (slow100C) and
-0.651 ns (slow-40C). The worst hold path is virtual
`CAPACITY_SERVICES_HELD[0]` to UIO `services_snapshot[0]`; virtual transport
reset to owner fault is also negative. Do not hide these with timing exceptions.

All304 board-crossing groups were read:40 setup-failing groups, no crossing
hold failures. Triage of EACH failing group's worst endpoint finds27 outgoing
advance groups,11 snapshot scan/admission/hold groups, and2 original board
control/data groups: ContA `u_e24.qb` (JCN) to MemX `u_l23.d_pre[5]` (-0.519 ns)
and MemD `u_l21.d_pre[5]` (-0.363 ns), through the DMadr path. This is a worst-
endpoint triage, not an exhaustive classification of every reported path.
Local advance copies alone are not proven sufficient to close the design.

Independent source/native-state review still passes:2,761 original board pins,
unchanged periods/phase/placement and raw drain equations; exact component
predictor assets and all662 staged files. Fresh XML accounts for2,094,314
required bits (physical1,957,048 /private board4,578 /transport69 /wrapper
ACCESS PENDING132,619). The8 aggregate bits require access; top network/UIO11
bits remain outside this count. Task-written SDRAM120bits stay required;
1,239 function scratch bits are reviewed without retained history. Schema
`af0b647cb43a3a62` remains board-only. Source/state/capacity/timing/crossing/
readiness readers reject13/24/17/6/3/6 damaged controls.

Terminal packet `/private/tmp/dorado-serial73-terminal-20261020.tar.gz`:
SHA256 `71d0f167279cb9062028c59d2a53fa9916235c9d10388093bd7dfd17302dbbb8`,
6,809,574 bytes /1,076 raw members. The lossless receipt is
`fpga/mister/reports/2026-10-20-snapshot-native-ready-prediction-rejected/`:
1,976 raw members,325,313,132 raw /21,218,555 stored bytes.
All raw members read back. Extracted frozen source independently reproduces
all662 staged file bytes, fresh XML/complete classification, every capacity/
timing/crossing/readiness result and corruption control. The final receipt
retains every originally replayed raw byte and adds complete extracted replay
evidence plus the additional-path triage. Local review:
`build/rtl-executive/serial73-terminal-qualification-20261020`; extracted replay:
`build/rtl-executive/serial73-terminal-extracted-replay-v1-20261020`.
The earlier source-only receipt remains immutable historical provenance.

Next retain the exact NEXT-ready predictor, then qualify explicit same-falling-
edge local advance AND snapshot-hold distribution using original complete-board
state/pin/traffic and partial-import quarantine oracles. Inspect the two ContA
JCN-to-DMadr cones separately. Connect actual clocked Main/service controls
before expecting the virtual-control hold failures to disappear. Preserve the
global clock-gate phase, live guards and real DDR visibility; account all new
state. No blind seed/fanout/global changes or period relaxation. Full machine
snapshots, original60ns clocks and final FPGA gates remain open; no speed gain.

**Previous native source: Serial72 (source only)** at local
`build/rtl-executive/serial72-native-disk-pause-20261020`. No upload, Quartus,
assembly, RBF or hardware change. The stager cannot run Quartus; qualify a
structural advance change first. All661 Serial71 parent sources are reviewed
and662 actual inputs retain2,761 original pins, normal readiness, clocks,
periods and placement policy. Fourteen damaged source/control/status variants
reject. A real three-bit disk coordinator holds all consumers through HPS-latch
release and retains fault quarantine; actual disk ACK AND external ARM permit
tightens service bit1 alongside memory0/network2. No permission is tied high.

New optional UIO flag11/status words18..20 report coherent local ACKs, mask7
and version ac01 separately from host permission. Memory/disk/network adapter
ACKs are available; input/video are unavailable/zero. Main can now observe
FPGA disk drain BEFORE leasing ARM cache/image state. Raw ACKs do not grant
state access; physical DDR visibility and actual Main coordination remain open.
Default-off UIO agrees on all348 original bits; enabled status adds5 REQUIRED
top bits. The reader retains board-only schema/ABI2 checks, rejecting nine bad
statuses. Enabled old clients require this extension reader.

Real Pilot/HPS +disk/memory-coordinator tests preserve131,823 observed bits
through nine pauses; payload/CRC/neighboring bytes agree. Four broken variants
reject. Three coordinators pass104 controlled-ACK chain checks; four-state
disk control passes65,826 checks over64 drain /16,384 ready combinations,
rejecting six broken variants. Real UIO/owner tests reject five compiled
variants; default-off medium and archive-extracted rebuilds pass.

Actual-QSF native inventory requires2,094,306 bits, including132,611 wrapper
ACCESS PENDING. Medium2 +disk controller3 add5 required wrapper bits. Eighteen
inventory/alias controls reject; task-written120bits and function scratch215
remain reviewed. The5 new UIO top bits and prior6 network top bits are outside
this count and still need access. No full snapshot claim. Reports:
`2026-10-20-snapshot-disk-services-barrier/` (833 raw members) and
`2026-10-20-snapshot-native-disk-pause-source/` (1,179 raw members); all raw bytes,
extracted source/fresh XML and exact662-input restaging verify.

**Registered NEXT-ready component qualification (2026-10-20):** predict the
original frontend's exact post-rising-edge readiness flags, AND sixteen per
MSA and capture one external bit per MSA on that same rising edge. This moves
the long frontend-to-global-enable cone toward a full host-clock period while
retaining the original guest advance phase. Actual routed timing is unproven.
All original sequential writes and raw drain equations remain byte-inverse to
frozen Serial72; reset-free cached flags retain live reset/initialization,
disk-ready and global-hold guards. Hold/scan priority and accepted-request,
response-kind and reset transitions are included. There are no new frontend
or physical-board bits; all eight external aggregate registers are REQUIRED
implementation state accounted in Serial73; native state access remains pending.

Four-state WIDTH8/12 tests pass5,916,160 checks:262,144 binary cases,
263,680 unknown-valued cases and65,536 between-edge live-guard cases; seven
compiled damaged predictors/registers reject. Only this exhaustive arbitrary-
state fixture removes the original simulation-only legality diagnostic from
BOTH reference and candidate; complete-board/SDRAM tests retain assertions.
The complete original-oracle MSA passes22,081,944 checks,7,034,752 readiness
checks,40,002 guest edges and all1,538 original physical bits through full
read/invert/write/restore,20,000 post-restore steps and quarantine/recovery;
two damaged group/hold variants reject. The real eight-MSA/128-client arbiter/
50MHz SDRAM test passes43,560,320 checks,9,385,584 readiness-edge comparisons,
4,405 guest edges,41,760 raw reads,7,472 writes and four pending-I/O pauses.
It covers8,465,170 scan edge observations and a partial member3 import followed
by parser reset: all eight consumers stay quarantined while physical refresh
continues. Both stale-current and wrong-clock-edge variants reject. Original
state/pins/raw traffic remain independent frozen-RTL oracles.

Serial73 now integrates these exact qualified sources and eight registers,
with independent inverse review and fresh native state accounting (above).
The terminal timing review above rejects outgoing advance, snapshot hold,
virtual controls and additional board-data paths. No RBF, hardware speed or complete snapshot
is claimed. Main coordination/full state access and the original
60ns clock/timer migration still follow.

Evidence: `fpga/mister/reports/2026-10-20-snapshot-ready-prediction/` contains
967 lossless raw members (13,728,150 raw /3,147,938 stored bytes). All members
read back. `verify_ready_prediction.py` extracts frozen sources and rebuilds
all three suites and their failure controls; exact inputs/results agree
(simulator log paths are retained but excluded from log-hash equality).
The first rebuild exposed a missing `tools/sil_netlist.py` import dependency;
its raw failure is retained, the frozen tools closure was completed, and all
three unchanged HDL/test suites then rebuilt. Final archive source/result
bytes match that successful rebuild. The local replay is
`build/rtl-executive/ready-prediction-extracted-rebuild-v2-20261020`.

**Qualified disk boundary (not yet in native RTL):** an opt-in/default-off pause
drains already-issued HPS requests after the guest clock stops, then holds the
host FSM and track-buffer port. Partial serial blocks, pending RAM stores and
dirty generations remain saved machine state; no guest write is completed or
partial block flushed. Two required wrapper bits hold the pause latch and media
identity. Known inactive media can pause; attachment changes/reset/unexpected
host activity revoke readiness. Global fault quarantine remains the caller's
job. Main must acquire its ARM disk lease only AFTER this FPGA acknowledgment.
Tests observe131,818 bits including all131,072 buffer bits, with nine pauses
across real Pilot serial/HPS paths and an independent whole-image/Fire-code
oracle. Default-off gates pass, three serial and six four-state broken variants
reject, and560 four-state checks cover256 admission combinations. Frozen and
archive-extracted rebuilds repeat exact inputs/results; all913 raw receipt
members read back and the extracted verifier passes. Evidence:
`2026-10-20-snapshot-disk-rtl-pause/`. No native integration, timing, state access
or full restore is claimed. These two new disk bits are NOT in Serial71's
2,094,301-bit native inventory.

**TERMINAL Serial71NetworkPause** at
`/home/alans/mister/Dorado_Serial71NetworkPause_20261020`. Builder2664220,
fitter2666775 and all project processes are absent; all661 prepared inputs
remain unchanged. No assembly/RBF/SOF/reload. Terminal packet SHA256
`df16a82ded4be9f2f0f472ff36c0259f54106d14c4818acb5a0b4f2ede6f1e3c`.
The fit uses41,464 /41,910 ALMs (446 free),52,155 registers,329 RAM blocks /
2,498,471 bits,35 DSPs/3 PLLs. Actual memory/network coordinators and production
engine/parser/wire are retained. Timing REJECTS it: setup-1.668 ns slow100C /
-1.326 ns slow-40C; hold-0.292 ns slow-40C (other hold corners pass). Seventeen
of304 crossing groups fail setup. SDRAM IO minimum+1.662 ns passes, actual
clocks match Native59, zero illegal/unconstrained clocks; exactly inherited3
input /50 output framework IO exceptions remain, not full framework IO coverage.
The worst path remains MSA3Odd waiting -> falling network_step; init-ready ->
network_step and network_step -> MSA pre-capture also fail. Hold fails on virtual
service inputs into UIO status. Continue real service integration and structural
advance/phase work without seed/fanout/global trials or period relaxation.
It derives from Serial69 with identical ordinary clocks, enables, board pins,
seed and timing policy. The final global hold freezes the DDR adapter; a
separate parser request permits framed release before any guest clock resumes.
Two settling clocks qualify ready. Retained service hold also keeps all memory
consumers/frontends held during parser release; fault/reset quarantines until
complete FPGA reconfiguration. Service bit2 ANDs the actual network boundary
with the external permission. Physical DDR visibility and Main/disk/input/video
barriers remain required, not implied by adapter idle.

Real engine/parser/wire/C-peer tests observe all34,344 network bits, including
32,768 packet-RAM bits, with seven compiled broken variants rejected. Four-state
checks cover256 drain /65,536 ready combinations (394,272 checks); the two real
coordinators pass79 controlled-ACK chain checks and reject early memory release.
Five legacy parser-pause cases pass. Frozen and archive-extracted builds repeat
these results. Independent native inverse review validates660 parent /661
actual inputs and2,761 original pins, rejecting twelve rehashed wiring/clock
controls. Native XML still classifies2,094,301 required bits, with132,606 wrapper
ACCESS PENDING; fifteen controls reject. The six added top-level parser/control
bits are outside that native count and need access. No full snapshot claim.
Reports: `2026-10-20-snapshot-network-barrier/` (328 raw members) and
`2026-10-20-snapshot-native-network-pause-source/` (1,277 raw members); all raw
bytes read back and extracted proofs pass. Terminal rejection report:
`2026-10-20-snapshot-native-network-pause-rejected/` (2,244 raw members).
Source/state/capacity/timing/crossing controls reject12/15/15/6/3 damaged cases;
old69/70 capacity gates repeat8/11 controls. All raw bytes read back, extracted
rejection/source/fresh XML checks repeat and restaging reproduces all661 actual
inputs. No hardware adoption or speed improvement. The new disk hooks are NOT
in this terminal core. See the handoff for the full receipt.

The eight-board circuit uses one pause coordinator and the real member router,
128-client interleaved/prefetch arbiter, 50MHz SDRAM backend and independent
XSDS pin models. Original frozen MSAs are bit/state/traffic oracles. It passes
15,403,567 checks /4,692,793 host edges /4,405 guest advances, 41,760 reads /
7,472 writes, four pauses starting with two accepted reads/two writes, every
688 state words across all eight boards, and 25,854 physical refresh commands.
A parser/router reset during a partial import on member3 keeps ALL eight
consumers frozen and refresh running after the caller drops hold. Three compiled
early-freeze/member-routing/nonsticky-hold controls reject. All 125 archive
members read back and a fresh extracted circuit rebuild/run repeats the exact
counts without working HDL. Evidence:
`fpga/mister/reports/2026-10-20-snapshot-eight-msa-sdram-pause/`.

**TERMINAL Serial70MemoryFanout:**
`/home/alans/mister/Dorado_Serial70MemoryFanout_20261020`. Builder2602043 and
all children are absent; no build from this sequence remains live. All 660
prepared inputs remain unchanged. Terminal packet SHA256:
`9dffd8993b2f049cdb3bf21112585bd2e60c86f08b35197892ded6fcecd1f56e`.
Actual mapper limit256 inserts four replication cells; routed paths use
network_step~SynDup_3 with256 loads. Capacity is 41,449 /41,910 ALMs,
52,826 registers, 329 RAM blocks, 2,498,471 bits,35 DSPs/3 PLLs. Setup fails
-0.561 ns slow100C /-0.328 ns slow-40C. Hold passes every condition, minimum
+0.111 ns. SDRAM IO/clocks/inherited framework exceptions still pass; sixteen
of304 crossing groups fail setup. Source/state/capacity/timing/crossing controls
reject6/15/11/6/3 damaged cases, including wrong applied settings and absent
replication. All2,207 raw archive members and extracted source/fresh XML/
capacity/timing/crossing controls pass; restaging repeats all660 exact inputs.
Reports: `2026-10-20-snapshot-native-memory-fanout-source/` (precursor) and
`2026-10-20-snapshot-native-memory-fanout-rejected/` (terminal). No RBF/SOF,
assembly, deployment or speed change. The new network/parser pause is NOT in
this frozen run. Return to Serial69 for real service integration; no more
undirected fanout limits, global-signal assignments or placement-seed trials.

**TERMINAL Serial69MemoryPause:** `/home/alans/mister/Dorado_Serial69MemoryPause_20261020`.
Builder2566540 and all project children are absent. All 660 actual prepared
inputs remain unchanged. Terminal packet SHA256:
`cd009807214dea2ab6a35515a2e35a4a99fc6f044d0cd52cbe4770ba5235a271`.
No Quartus job remains live; no assembly, RBF/SOF or reload occurred.

Serial69 starts from the better Serial65 RTL and original placement seed1.
It connects the qualified late freeze to all eight MSAs and retained guest hold
to all nineteen boards. Actual arbiter/SDRAM idle and initialization gate pause
admission; normal CPU frontend readiness remains separate and byte-equivalent.
The owner and UIO expose the same derived service vector. Its memory bit is
ANDed with the external permit; the other service ACKs are still virtual.
Snapshot control now initializes only on a COMPLETE FPGA configuration; PLL,
menu/framework and parser resets cannot clear a partial-import quarantine.
Normal architectural reset/firmware/clock behavior is preserved. No assembly.

Independent inverse review checks all 659 parent inputs, 660 actual inputs,
2,761 original pins/parameters, unchanged periods/IO/constraints and the board-only
schema af0b647cb43a3a62. Ten rehashed damaged wiring/status/reset/clock controls
reject, including fake idle and a ready/abort feedback loop. Actual-QSF XML
uses 320 HDL sources/15 macros, no native UNOPTFLAT loop. Extracted source,
fresh-XML classification/control and exact660-input restaging replays pass.
Evidence: `fpga/mister/reports/2026-10-20-snapshot-native-memory-pause/`
(1,149 members, source/classification precursor). Terminal evidence is in
`2026-10-20-snapshot-native-memory-pause-rejected/`: 2,207 raw members, all
lossless readback and extracted source/fresh XML/capacity/timing/crossing reviews
pass, and restaging reproduces all 660 exact input hashes without working HDL.

**Inventory correction:** the previous 2,094,179-bit count included only generic
`sequential` candidates. Two real SDRAM fields were separately marked
`subroutine_review` and omitted from that total: rows[0..7] (104 bits) and dq_out
(16 bits). Both are global nonblocking task destinations reached from clocked
processes; both are REQUIRED. With those existing120 bits and the new2-bit pause
phase, the accounted native state is **2,094,301 bits**: 1,957,048 physical board,
4,578 private board, 69 private owner/router and **132,606 WRAPPER ACCESS PENDING**.
No sequential/task state is excluded. The eleven function-local candidates /
215 bits are reviewed as automatic scratch or static returns definitely
assigned before use on every path, with no global writes/NBAs; they are not
persistent machine state. Fifteen geometry/clock/task-presence/alias controls
reject. This is not top emu/UIO, external RAM bytes, ARM or a complete snapshot.
Older immutable reports retain their original partial count; do not quote it
as complete native mutable-state coverage.

Previous nineteen-board physical experiments (all TERMINAL, no deployment):

| Candidate | ALMs | Worst setup | Minimum hold | Result |
| --- | ---: | ---: | ---: | --- |
| Serial65 readiness | 41,414 | -0.038 ns | +0.106 ns | Best baseline, still fails setup |
| Serial66 fanout64 | 41,360 | -1.067 ns | +0.117 ns | Rejected |
| Serial67 global signal | 41,280 | -6.078 ns | +0.118 ns | Rejected |
| Serial68 seed2 | 41,395 | -0.287 ns | -1.758 ns | Rejected |
| Serial69 real memory pause | 41,468 | -0.435 ns | +0.076 ns | Rejected |
| Serial70 fanout256 | 41,449 | -0.561 ns | +0.111 ns | Rejected |
| Serial71 network/memory pause | 41,464 | -1.668 ns | -0.292 ns | Rejected |
| Serial73 registered NEXT-ready +disk | 41,642 | -1.095 ns | -1.326 ns | Rejected; ready-to-advance +1.940 ns |

Their all-condition/304-crossing evidence and negative reader controls remain
in snapshot-serial-ready /serial-fanout-rejected /serial-global-rejected /
serial-seed2-rejected reports. Actual clocks/SDRAM IO and inherited framework IO
exceptions are checked: zero illegal/unconstrained clocks, SAME three input/
fifty output exceptions, explicitly not complete physical framework IO coverage.
Serial69's independent terminal review checks all four conditions and 304
crossing reports (fourteen setup-failing groups). Slow-40C setup is -0.204 ns;
hold and SDRAM IO pass every condition. Capacity/source/state/timing/crossing
readers reject 8/10/15/6/3 damaged controls. The worst path remains MSA2Odd
storage15 write_pending -> readiness -> falling-edge network_step (10 ns).
Other failures include network_step -> MemX RAM write enable and img_size/pack
readiness -> network_step. These are real setup paths; no cut or relaxed period
is permitted.

Daemon PAUSE4/CONTINUE5 already passes isolated Mac/Linux tests with identical
86 inputs, exact EFTP/BSP continuation, protected paused MMIO and a pause-aware
retry clock. STOP/EOF remain lease revocation, never snapshot continuation.
That code is host-qualified, not deployed ARM32/whole-machine pause. Evidence:
`2026-10-19-snapshot-server-pause/`. The earlier one-MSA real-memory pause and
four-state/idle gates remain preserved. Disk phase staging separately qualifies
at +4 ALMs and 18 new REQUIRED wrapper bits; it is NOT in Serial69.


The actual Main disk cache/adapter now has a nonblocking snapshot lease. It
holds the mounted generation, defers normal SPI capture/ACK, drains accepted
work on the existing worker and exports bounded clean chunks only after fsync.
Final Linux candidate/4 compiled mutation/legacy cache+adapter gates pass.
The exact static ARM test also passes on MiSTer14 with temporary images and SPI
spies: three normal/write-error/sync-error cases, max poll gap 3.533 ms during a
4.2-second write, full 256 KiB file and close/remount oracle. Failed writes
retain 31 dirty sectors; failed sync retains needs_sync. All 319 raw archive members
and extracted input/log/provenance checks pass. Evidence:
`fpga/mister/reports/2026-10-20-snapshot-disk-pause/`.
This is the ARM FILE-WORK boundary only: stop guest disk activity and finish
already-issued FPGA disk requests BEFORE requesting this lease. Whole-machine
ordering, actual FPGA disk/network/input/video ACKs and complete restore remain
open. No production Main/core/daemon was replaced.

Network/parser pause now passes actual RTL/C-peer continuation tests. The
DDR engine finishes accepted frame/DDR work, preserves epoch/indices/timer phase,
and waits for ACK/commit tails. The network UIO latches hold only between
transactions, ignores shared EXT_BUS snapshot traffic while held, and releases
only with the bus deselected. This adds one REQUIRED parser state bit. The
production hooks remain disabled/tied low until real service coordination.
The enabled production mux/UIO/engine/wire fixture holds all 34,339 observed
sequential/task bits, including both packet RAMs (32,768 bits total), and resumes
partial serial RX and guest TX capture with exact payload/CRC. Nonzero RAM seeds
exercise unused words. Four early-hold/release/missing-hold/ungated-guest controls
reject. Engine tests separately retain all 677 candidate bits, full 8TX/16RX
queues plus pending TX, accepted reads and stalled frame tails, with four bad
controls. Default-off production ring/wire/CRC regressions pass. All 277+125 raw
members and extracted source rebuilds pass. Evidence:
`fpga/mister/reports/2026-10-20-snapshot-network-pause/` and
`2026-10-20-snapshot-network-uio-pause/`. This is NOT in Serial69/70. Main/daemon
coordination, two wire-settling clocks, physical DDR write visibility, complete
native state access/import and Cedar restore remain open. No hardware changed.

Main now has a nonblocking same-epoch daemon snapshot lease. Its real network
poll bypasses packet/capability SPI while a lease is pending, drains prior
control records before PAUSE/CONTINUE, and validates paused health replies over
the socket. CONTINUE releases ownership only after its exact ACK. Four actual
pinned Main lifecycle prologues refuse held/failed leases before hardware
mutation. Failure retains the lease; no silent STOP/reconnect/reset/fallback.
Seven normal/fault cases pass on Mac, Linux and MiSTer14 ARM; six compiled bad
variants reject. Feature inputs are identical across all three builds (21),
and both hosts pass the existing seven regression families with identical 94
inputs. ARM maximum normal poll call is 0.030 ms. All 1,104 raw archive members
and extracted receipt/source checks plus a fresh 13-case rebuild pass. Evidence:
`fpga/mister/reports/2026-10-20-snapshot-main-lease/`.
This is the Main/daemon lease only, NOT whole-machine admission. The coordinator
must still validate compatible Core/daemon builds, finish FPGA disk requests
before taking the ARM image lease, wire actual FPGA/network/input/video ACKs,
establish physical DDR visibility and implement genuine fault recovery/reseed.
No production Main/daemon/core changed; Native59's measured rate remains.

Historical snapshot delivery sequence (superseded 2026-10-20): state-access,
Main/daemon admission, raw-memory export and complete bundle restore are deferred.
Follow the immediate work and milestones at the top of this document. Preserve
Serial73's timing rejection and Serial74's component/source evidence for later
reuse; neither is a qualified speed build.

Goal ACTIVE; the complete requested outcome remains unfinished.

Previous terminal nineteen-board result:

**The complete nineteen-board core now fits: 41,381 / 41,910 ALMs. Timing rejects it: setup -0.239 ns, hold -0.155 ns.**

Serial64Chipwidth is the first legal routed capacity result for all eleven main
boards and eight snapshot-enabled MSAs. It saves 1,068 ALMs from Serial63's
42,449 and 5,321 from Serial62Area's 46,702, leaving 529 ALMs. Actual fit reports
52,561 registers, 329/553 RAM blocks, 2,498,471 block-memory bits, 35 DSPs and 3 PLLs.
The cache holds all original chip WIDTH 8/12 data/ECC bits; raw 16-bit transfers,
8 MiB guest/16 MiB padded backing, all original guest registers and clocks stay.
The exact qualified board-only schema is `af0b647cb43a3a62`.

All 659 parent inputs and 413 Native59 receipts pass. Independent source review
checks 2,761 original pin/parameter bindings, qualified assets, unchanged clock/
IO/area policy and all constraints, with 23 damaged source/policy/alias cases
rejected. Actual Quartus leaves all 659 prepared inputs byte-identical. Fresh
actual-QSF XML uses 319 HDL /15 macros /3,945 instances, accounting for 1,957,048
physical board +4,578 private board +69 private owner/router +132,484 wrapper
ACCESS PENDING =2,094,179 sequential bits, none excluded; 14 damaged cases reject.
Top emu/UIO, external raw RAM and ARM services still require full snapshot work.
Four damaged capacity/process cases reject. Capacity PASS is NOT timing PASS.

All four device conditions and nineteen adapter crossing scopes are retained.
The independent timing reader verifies setup/hold/recovery/removal/pulse width,
SDRAM input/output setup/hold, actual clock definitions and BOTH setup/hold IO
exception tables. Clocks match Native59; zero illegal/unconstrained clocks.
Exactly the inherited three input/fifty output framework exceptions remain;
this is explicitly not a claim that every physical framework IO is timed.
SDRAM IO passes. Slow 100C setup is -0.239 ns, slow -40C setup -0.150 ns; hold is
-0.088 ns/+100C and -0.155 ns/-40C. Fast conditions pass. Other adapter crossing
violations are retained, not hidden by the worst-three summary. Six damaged
reader cases reject against the KNOWN PASSING Native59 baseline; the failing
actual design is reviewed as REJECTED_TIMING, not used as a false control gate.

Setup endpoints reach the falling-edge `network_step` register from MSA
write_pending/readiness and snapshot scan fault/busy flags (ten-nanosecond path).
Real slow 100C hold is u_media.cyl[6] -> cached_cyl[6]. The worst slow -40C hold
paths are virtual CAPACITY_SERVICES_HELD / CAPACITY_TRANSPORT_RESET inputs,
which still need real service-barrier integration. Do not waive these ports or
tie acknowledgments high. Routing warnings anticipated the observed failures.

The `/home/alans/mister/Dorado_Serial64Chipwidth_20261019` run is TERMINAL;
builder 2433951 and fitter 2436350 are absent. No job remains live. Assembly was
disabled: no RBF/SOF/reload. Evidence:
`fpga/mister/reports/2026-10-19-snapshot-serial-chipwidth/`.
All 2,761 raw members pass lossless readback. Extracted tools reproduce all 659
staged bytes after rebase; source/pin/alias, full native XML controls, capacity
review and timing-rejection review plus passing-baseline reader controls replay.
Native59 timing baseline bytes are included. No working HDL is consumed.

Next concrete clock-path experiment: the core currently ANDs storage_ready
(already gated by per-board snapshot hold) with !board_state_hold. Investigate
using the qualified NORMAL frontend-ready/drain aggregate instead, still ANDed
with storage_initialized and !board_state_hold. Prove equality on actual MSA
normal traffic and the eight-board owner/quarantine/unknown-input cases BEFORE
adoption. This removes redundant hold propagation rather than adding latency,
new state or relaxing periods. The aggregate covers frontend readiness only;
it is NOT proof that the arbiter/SDRAM controller has drained every transaction.
Keep the existing full raw-service barrier requirement.

Also prove the disk-cache same-edge hold condition from the actual two-clock
gating: cached_cyl loads only when need_track && head<5, which deasserts pack
ready before a guest edge can change cyl/head. Verify reset/remount, tags,
arbitrary extra stalls and the real falling-edge gate; do not cut a real path.
No such new exception is applied yet. Then qualify the structural change, real
service input synchronization/barriers and another unchanged-period full fit.

Complete wrapper/raw-memory/input/video/Main/daemon/server snapshots, genuine
reseed, connection-preserving pause and matching disk generations remain open.
The live server's lifecycle resume reconfigures Pup connections, so it cannot
be used unchanged for snapshot resume. Use deployed ARM source/binary receipts,
not the older C sources incidentally carried in the frozen FPGA source tree.
Original 60 ns clocks/timers, pinned speed comparisons, three fresh Cedar boots/
cold persistence and two-hour stress remain required. Native59 and the last
qualified 1,947,654.12 instruction issues/s are unchanged by this work. Goal ACTIVE.

Previous component qualification and build launch:

**Chip-width MSA caches qualify at 1,600 ALMs versus 1,721; the complete Serial64Chipwidth fit is running.**

The bridge cache now holds exactly each original chip group's WIDTH (8 or 12)
low data bits, preserving the raw 16-bit port, original guest pipeline and all
866 original FF/history bits. Only unused implementation padding is removed.
One complete MSA has 1,538 physical bits /378 fields (672 bridge bits), 86 words,
last 8 bits. Nonzero raw padding is deliberately supplied in the original RTL
oracles. Full-MSA testing passes 13,288,504 checks /40,002 guest advances, all
remaining physical state/normal pins/raw traffic, full inversion/restore,
first resume and 20,000 further advances; seven bad RTL variants reject.
The frontend gate passes 1,768,540 checks, all original chip-array bits, nine
pending/reset snapshots and eight bad variants. Source inverse review rejects
ten rehashed damaged cases; fresh XML accounts for all 1,538 candidate +2,690
unchanged reference +81 private bits with no exclusions, fourteen controls.

Matched complete-MSA Quartus uses the same 50 MHz shell, 991 virtual pins, 2 ns/0 ns
IO and area policy: 1,721 baseline /1,600 chip-width ALMs. All four device conditions
pass with minimum +0.119 ns and zero unconstrained clocks/IO; five damaged fit
cases reject. Builder 2425360 is absent; the component job is TERMINAL.
Evidence: `fpga/mister/reports/2026-10-19-snapshot-msa-chipwidth/`.
All 500 raw members pass readback; extracted source/fixture generation, both
original-RTL/DRAM rebuilds, source/XML/fit controls pass. No native speed claim.

All eight real adapters also qualify over SPI under new board-only schema
`af0b647cb43a3a62`, 1,957,048 physical board bits. The first eleven maps stay;
slot11 has 86 words/member, stride 256, dense eight-member payload 1,538 bytes.
Both owner/router reject words 86..255 before dispatch. Tests pass 75,935 ticks /
326 SPI packets /129 checks, including 34 Python-built packets; old main, original
MSA and prior compact schema tags reject before changes. Ten actual bad RTL
variants, ten rehashed catalog mutations, six inverse source controls and twelve
fresh XML/schema/deadline/alias controls reject. Actual state totals are 12,304
physical MSA +648 private MSA +417 private transport, none excluded. Guest advances
are stopped and other endpoints/services remain explicit stubs here.
Evidence: `fpga/mister/reports/2026-10-19-snapshot-msa-chipwidth-transport/`.
All 380 raw members and extracted catalog/fixture/RTL, dense-format/source/XML
controls and rebuilt eight-board SPI replay pass. No native pause claim.

Serial64Chipwidth integrates these exact qualified bytes in frozen Serial63.
All 659 parent inputs /413 Native59 receipts and 2,761 original pin/parameter
bindings pass; 23 damaged source/policy/alias cases reject. All real timing
constraints remain byte-identical after path rebase; no new cuts. Fresh actual
QSF inventory uses 319 HDL /15 macros /3,945 instances, with 1,957,048 physical board
+4,578 private board +69 private owner/router +132,484 wrapper ACCESS PENDING:
2,094,179 sequential bits, none excluded; fourteen damaged cases reject.
Top emu/UIO, raw SDRAM and ARM state still require complete snapshot work.

The remote job `/home/alans/mister/Dorado_Serial64Chipwidth_20261019` is ACTIVE,
builder 2433951. Observe that same PID/child and driver.log; do NOT restart from
a timeout or a state file. Staged packet SHA256:
`a1e8e12bc900ebc46381331e13d148ec416a7c8b9353fab5aa36f392b675d8cc`.
Local source/remote/input XML/audits are under
`build/rtl-executive/serial64-chipwidth-{source,remote,inventory}-v1-20261019`.
Assembly is disabled; no RBF/reload. Last terminal whole-core fit remains
42,449 /41,910 ALMs, 539 over. Do not infer a combined fit from per-MSA savings.

Next: observe Serial64 through terminal fit and (only if legal) all-device timing,
verify all actual inputs/clock and memory policies, archive and review the result.
Then finish real wrapper/raw-memory/input/video/Main/daemon/server capture,
connection-preserving pause, true reseed and matching-disk generation barriers.
Original 60 ns clocks/timers, pinned performance comparisons, three fresh boots/
cold persistence and two-hour stress remain open. Native59/last qualified
1,947,654.12 instruction issues/s is unchanged. Goal ACTIVE.

Previous complete compact fit:

**The complete compact nineteen-board fit saves 4,253 ALMs, but still fails capacity: 42,449 / 41,910 ALMs, 539 over.**

Serial63Compact integrates the exact qualified compact MSA/transport receipts
and schema `56e997e5c1fb54d0` in the reviewed Serial62Area project. All 659 parent
inputs and 413 Native59 receipts are checked. Only the qualified replacement
assets, router name, schema and metadata change; obsolete MSA hold exceptions to
the removed saved_addr/saved_data registers are deleted. Original clocks, real
IO constraints, area policy, memory layout and five virtual service contracts
remain. The independent review directly checks all 2,761 original pins and
parameters against Native59 and rejects 23 damaged source/policy/alias cases.
A complete equivalent Quartus alias set is accepted; partial/duplicate/foreign
sets reject. Actual Quartus leaves all 659 prepared inputs byte-identical here.

Fresh Mac XML uses the actual QSF's 319 unique HDL files / 15 macros / 3,945
instances. It accounts for 1,957,944 physical board bits, 4,578 private board
bits, 69 private owner/router bits and all 132,484 unchanged wrapper bits with
ACCESS PENDING: 2,095,075 sequential bits, no exclusions. Fourteen missing-state,
geometry, parameter and exact-alias cases reject. Top emu/UIO, external raw RAM
and ARM services are still outside this inventory and remain required work.

Synthesis passes with 49,242 registers (previous 58,736). The fitter then exits 3:
4,265 LABs / 4,191 available, 42,449 ALMs / 41,910 (previous 46,702). Its summary
reports 48,574 registers, 2,498,471 block-memory bits, 35 DSPs and seven virtual
pins. All nineteen boards and three transport entities remain in the hierarchy.
These are failed-fit diagnostics, not a legal placement or physical RAM result;
zero RAM blocks in that failed summary is not memory qualification. No STA,
assembly, RBF or reload follows. Four damaged capacity/process cases reject.

The remote `/home/alans/mister/Dorado_Serial63Compact_20261019` run is TERMINAL;
builder 2412173 and fitter 2414597 are absent. Do not restart it. No job remains
running. Evidence: `fpga/mister/reports/2026-10-19-snapshot-serial-compact/`.
All 2,254 raw members pass lossless readback. Extracted tools reproduce all 659
staged inputs after path rebasing; extracted source/pin/alias controls, complete
native inventory controls and terminal capacity review/controls pass. Source
review initially needed a namespaced view for prefixed native helper filenames;
that adapter was fixed without an RTL change. Startup preflight had 20 source
controls; the final independent reviewer adds three meaningful alias controls.

Next: qualify a cache that stores exactly each chip group's WIDTH bits rather
than unused high padding from its 16-bit raw word. The component-only prototype
has 38/54 actual bits versus 46/58, with every remaining bit exported. It passes
1,768,540 checks against untouched original RTL/DRAM, with NONZERO raw padding
supplied deliberately, all original chip bits compared, nine snapshots across
pending read/write/write-done/reset and eight bad RTL cases rejected. The exact
raw 16-bit request/response interface and guest pipeline are retained; no original
CPU/cache/map/ECC state is removed. This prototype is archived separately within
this report and was NOT used in Serial63Compact. Complete-MSA integration,
independent geometry/source review, a new schema, matched area/timing and another
full-core fit remain pending. Do not infer capacity from its smaller bit count.

Complete wrapper/input/video/raw-memory/Main/daemon/server snapshots, genuine
reseed ownership, connection-preserving pause and matching-disk generations
remain required. Original 60 ns clocks/timers, pinned speed comparisons, three
fresh Cedar boots/cold persistence and two-hour combined stress remain open.
MiSTer14's last qualified Native59 configuration and measurement of
1,947,654.12 instruction issues/s are unchanged by these experiments. Goal ACTIVE.

Previous compact transport qualification:

**All eight compact MSA adapters now qualify through SPI under a new schema. Next: the complete nineteen-board fit.**

`serial_state_format_msa_compact.py` binds the qualified component receipts to
board-only schema `56e997e5c1fb54d0`: 1,957,944 actual physical board bits.
The first eleven board maps are unchanged; slot11 indexes the eight members
with unit bits 10..8 and word bits 7..0. Each member has 1,650 bits / 92 words,
last word 12 bits. Unit stride remains 256 addresses, with no per-member padding
in the dense 1,650-byte eight-member payload. Both the outer owner and member
router reject words 92..255 before dispatch. The 4,096-cycle budget, twelve
frozen bits, five explicit service contracts and reset/quarantine ownership stay.
This schema is incompatible with old 2,690-bit member snapshots by design.

The four-state SPI test passes 78,859 host ticks / 293 packets / 113 result
checks, including 34 actual Python-formatted packets through all eight real
compact adapters. It checks unique member values, first/final words, new holes
and high bits, old main-only AND old MSA schema rejection, held competing
requests through completion, missing/unknown boundaries, timeout, active parser
reset, sticky quarantine and unchanged Ethernet. Ten actual damaged RTL cases
reject; ten rehashed catalog mutations and status/padding checks reject. This
transport fixture stops guest advances and retains eleven endpoint/five service
stubs. The original guest behavior and restore/resume qualification are the
separate complete-MSA receipt; no native pause or FPGA snapshot is claimed.

Fresh XML accounts for all 13,200 physical MSA  + 648 private MSA  + 417 private
transport bits, with no sequential exclusions. Twelve inventory/schema/deadline/
exact-alias controls reject. Exact inverse source review checks unchanged owner,
UIO and router logic apart from the explicit bound/module edits, plus the complete
compact-board source proof; six damaged source cases reject. The first transport
run exposed an outer-owner bound still set to 150: it returned child error 3
rather than rejecting hole 92 as error 2. Updating that generated owner bound to 92
fixes it; the old-bound control now reproduces the failure. The first XML runner
omitted an imported test utility; its corrected v2 inventory is authoritative.

Evidence: `fpga/mister/reports/2026-10-19-snapshot-msa-compact-transport/`.
All 414 raw members pass lossless readback. Extracted catalog/fixture/vector/RTL
regeneration is byte-identical; dense-format/source/XML controls and a rebuilt
actual eight-board SPI test pass against committed immutable receipts.

Next: replace the exact qualified compact MSA/transport assets and schema in the
nineteen-board Native59 capacity experiment. Retain all original pins, main-board
adapters, memory policy, clocks and real IO constraints. Remove only obsolete
MSA hold exceptions whose saved_addr/saved_data destinations no longer exist;
add no setup cuts. Verify every compiled source and regenerate a complete native
state inventory before another full fit. The old whole-core result remains
46,702 / 41,910 ALMs; the matched component saving of 492 ALMs does not prove that
the combined design fits. The compiled-native schema, whole-core fit/timing and
real external service barriers are still pending. No Quartus job remains live.

Complete wrapper/raw-memory/input/video/ARM/server state, true reseed ownership,
connection-preserving pause and atomic matching-disk generation remain required
for full snapshots. Original 60 ns clocks/timers, pinned performance comparisons,
three fresh Cedar boots/cold persistence and two-hour stress remain open.
MiSTer14/Native59 is unchanged at the last qualified 1,947,654.12 issues/s. Goal ACTIVE.

Previous compact component qualification:

**Compact raw-memory bridge qualifies on one complete MSA: 2,213 -> 1,721 ALMs, saving 492. Native integration is next.**

The candidate removes five duplicate implementation fields per group (65 bits):
transaction address, tagged cache address, saved write data, old data and saved
operation kind. Original 866 FF/history bits and the byte-identical guest DRAM
pipeline remain. Ready gating makes address/write data stable during accepted
raw operations. A pure combinational output on the original address latch
predicts actual row/column changes; the new untagged cache invalidates only on
that guest edge. Write acknowledgment retains the old word until the guest
samples it, then commits the new cached value. No original CPU/cache/map/ECC
state is dropped. All actual remaining bridge registers are snapshot-accessible.

One MSA now has 1,650 physical bits / 378 fields (866 original FF/history + 784
bridge bits), versus 2,690 / 458. Scan is 92 words, last 12 bits. Component tests
against untouched original RTL and DRAM arrays pass 1,802,415 checks, including
pending read/write/write-done snapshots, reset, all-new-field scan/restore and
full raw-array comparison; eight actual bad RTL cases reject. Complete-MSA
comparison adds 14,097,552 checks / 40,002 guest advances, all normal outputs,
raw traffic, complete inversion/restore/abort/first-resume and 20,000 resumed
advances; seven bad RTL variants reject. A cross-group wiring mutant survived
the first random workload; history is retained and the independent exact-wiring
source audit rejects that same alias after rehashing its metadata. Dedicated
address tests vary 12,000 independent physical states at ROW_BITS7/8, including
equal-address captures, and reject four damaged predictor equations.

Fresh XML accounts for all 1,650 candidate + all 2,690 original reference +81
private bits; no sequential exclusions. Fourteen inventory/alias controls and
nine independent source controls reject. Original FF/helper bodies, normal board
pins/parameters/equations and guest DRAM process are checked against committed
qualified bytes. This is valid reachable-state behavioral testing, not arbitrary
old-schema compatibility or four-state equivalence of different representations.

Matched full-MSA Quartus fits use the same 50 MHz shell, 991 virtual pins, 2 ns/0 ns
IO budgets, device and area policy, without a new gated clock or timing cuts.
Actual ALMs are 2,213 baseline / 1,721 compact; fitted registers 4,684/3,403. All four
device conditions pass, minimum +0.119 ns, with zero unconstrained clocks/IO;
five damaged fit/source/constraint cases reject. Builder 2399564 and shell 2399560
are absent; the remote `/home/alans/mister/Dorado_MsaCompact_v2_20261019` run is
TERMINAL. No assembly/RBF/reload or speed change. Do not infer combined capacity
from isolated savings.

Evidence: `fpga/mister/reports/2026-10-19-snapshot-msa-compact/`.
All 666 raw members pass lossless readback. Extracted source/fixture generation,
source/XML/fit controls, both original-RTL/DRAM rebuilds and address-prediction
units pass. The first prepared fit packet was not run; the corrected v2 driver
keeps generator imports out of its remote execution path. Early local shell
port-shape preparation assertions left no staged build and changed no RTL.

Next: qualify the new compact board-only catalog and all eight real adapters
through SPI, with 92 word bounds and a new incompatible schema tag. Preserve the
other eleven maps, all service ownership, quarantine and true-reseed requirements.
Then integrate the exact qualified candidate in the nineteen-board Native59
capacity experiment with all original clock/IO constraints. Serial62Area's
46,702/41,910ALMs is still the last full-core result; a legal fit and all-device
timing remain unproven. Complete wrapper/ARM/disk-paired snapshots, original 60 ns
clocks, pinned comparisons, three fresh boots/cold persistence and two-hour stress
remain open. Native59 remains deployed at the last qualified 1,947,654.12 issues/s.
Goal ACTIVE.

Previous shared-clock experiment:

**Shared physical-FF clock control is rejected for area: 2,222 versus 2,213 ALMs.**

The complete-MSA component retains all 458 fields / 2,690 physical bits. Its
78 FF helpers use a dedicated ALTCLKCTRL global clock (866 fanout); host memory
frontends and scan control keep running. The gate samples raw guest intent at
the falling edge, before the latched actual-advance flag becomes available.
All four device conditions pass, minimum slack +0.157 ns, with zero
unconstrained clock/IO paths. Five damaged fit/phase/constraint cases reject.
This is a matched component result; no native gate/barrier or speed claim.
Do not integrate this candidate: it costs nine additional ALMs.

The authoritative simulation-v3 supplies raw intent before that falling edge,
then checks the latched guest flag. It passes 32,840,341 checks / 40,002 guest
advances, original board outputs/raw traffic, all physical fields, complete
save/invert/restore and 20,000 resumed advances; four actual bad RTL variants
reject. Earlier low-latch and v2 contracts are retained as exploratory history,
not vendor-netlist equivalence. Fresh XML accounts for every board bit plus the
required clock phase and fixture enable/monitor; seventeen damaged cases reject.
Six source controls and both 5,000-cycle clock-model units also pass. The first
Quartus fixture omitted five combinational sources and failed; the corrected
isolated run completed. Both remote builders are terminal; no RBF or reload.

Evidence: `fpga/mister/reports/2026-10-19-snapshot-msa-clocked-rejected/`.
All 692 raw archive members pass lossless readback. Extracted generation,
source controls, original-DRAM test rebuild, XML/alias controls, clock-model
units and matched-fit review pass against the committed qualified receipts.

Next: compact the native nondeferred raw-memory frontend by eliminating duplicate
address/write-data transaction buffers. The first component prototype has
46/58 actual physical bits versus 111/123, with all its remaining registers
exported under a future new schema. The original guest DRAM pipeline is retained;
a completed write commits its cached new value only after the guest samples the
old word. Guest advances require ready, so address/data stay stable while a raw
transaction is pending. An exact address-change predictor must invalidate the
untagged cache at the real address-register edge. Component original RTL/DRAM,
all-new-state scan/restore and eight damaged RTL cases pass; complete MSA
prediction/integration, independent inventory, area/timing and full-core capacity
remain unqualified. No reduction is adopted into the core yet. No area saving
is inferred from a register count.

Serial62Area still needs 46,702 / 41,910 ALMs. Complete wrapper/ARM snapshots,
real pause/disk-generation barriers, original 60 ns clocks/timers, three fresh
boots, cold persistence and two-hour stress remain open. Native59 is unchanged;
last qualified measurement is 1,947,654.12 instruction issues/s. Goal ACTIVE.

Previous same-coverage area experiment:

**Same-coverage area optimization saves 2,548 ALMs, but Serial62Area still fails capacity: 46,702 / 41,910 ALMs, 4,792 over. Both experiments are terminal.**

Serial62Area changes four supported Quartus17 options: Aggressive Area mode,
Minimize Area with Chains packing, Area synthesis, and physical combinational
area optimization ON. All 659 staged files otherwise match Serial61 after path
rebasing. All nineteen qualified adapters / 1,966,264 physical board bits,
macros, original clock/IO constraints, memory policy, transport and virtual service
contracts are retained. No fresh XML is claimed; HDL/macros match the earlier
logical inventory. Synthesis passes, but fitting needs 4,701 LABs / 4,191 and
exits3, versus Serial61's 4,954 LABs and 49,250 ALMs. Neither fit is legal.
No STA, assembly, RBF or hardware reload follows. The failed summary's zero RAM
blocks is not a physical memory result; area-mode inference remains unqualified.
Original clock periods are not relaxed.

The independent source/policy audit rejects 27 damaged cases, including all 23
inherited source cases through the area reviewer. Its optional reviewer test API
retains the old default behavior, separately rechecked with 23 controls. Terminal
Quartus aliases pass. All 413 Native59 receipts and 659 remote/local input matches
pass; 658 non-QSF inputs remain byte-identical after the build. The capacity
comparison explicitly adapts only the source-review envelope for the shared
resource reader. Actual reports retain all nineteen board / three transport
scopes; four damaged capacity cases reject. Evidence:
`fpga/mister/reports/2026-10-19-snapshot-serial-area/`. All 1,553 raw members pass
readback; extracted source/policy/alias controls, byte-identical 659-file
restaging and terminal equal-coverage capacity review pass.

Remote project `/home/alans/mister/Dorado_Serial62Area_20261019` is TERMINAL;
builder2346741 and fitter2349070 are absent. Inputs remain isolated at
`/home/alans/mister/Dorado_Serial62Area_inputs_20261019`. Do not restart it.
No job from these experiments remains running.

The preceding explicit frontend D/enable factoring is REJECTED: matched enabled
native8/12 fits use 278 ALMs for qualified serial / 282 factored, despite passing
2,237,064 untouched original RTL/DRAM-oracle checks and eleven RTL failure
controls. Four-state comparison adds 12,143 ticks and three RTL failure controls.
Both fits pass four conditions, minimum +0.160 ns, with no unconstrained paths;
four damaged fit cases reject. All 650 raw members and extracted DRAM/four-state/
fit-review replays pass. This candidate is absent from Serial62Area and the live
core. Evidence: `fpga/mister/reports/2026-10-19-snapshot-storage-factored-rejected/`.

Next prototype shared, glitch-free hardware clock enabling for ordinary physical
FF helpers on actual guest advances and scans, first on one complete MSA. Keep
host memory work/controllers running and retain all 458 fields / 2,690 bits and
their schema. Test raw stalls, clock-high request changes, first/final scan edges,
abort/sticky quarantine and first resumed operation against untouched original
state/pins. Account for gate phase state and related-clock timing; then measure
matched area before native integration. No such clock change is implemented here.
Do not drop state or infer a combined fit from isolated savings. A legal enabled
complete-core fit and all-device timing remain required before deployment.

Complete wrapper/input/video/network/raw-memory/Main/daemon/server state and real
pause/disk-generation barriers remain required, including true reseed ownership
(the current PLL-unlock reset proxy cannot be a production quarantine clear).
Then qualify atomic disk-paired FPGA Cedar save/continue/restore. Original 60 ns
clocks/timers, pinned comparisons, three fresh boots, cold persistence and two-hour
combined stress remain open. MiSTer14/Native59 and the last qualified rate of
1,947,654.12 issues/s are unchanged. Goal ACTIVE.

Previous nineteen-board integration:

**Nineteen-board serial integration passes source/XML audits, but the combined fit is rejected: 49,250 / 41,910 ALMs, 7,340 over capacity.**

`build_serial_core.py` stages the exact qualified eleven main-board adapters,
eight MSA adapters and twelve-slot owner/router in frozen Native59. All 2,761
original board pins/parameter bindings and original ordinary core equations
are preserved. The independent audit rejects twenty-three damaged source/QSF/
schema/constraint cases. Real clock periods/IO stay unchanged; the renamed MSA
own-source hold cuts retain exact endpoints with an explicit scan-mode mux
exclusion proof. Added virtual-contract IO is constrained; no setup cuts added.

Fresh Mac XML uses the actual QSF-selected 319 unique HDL files, fifteen macros
and 3,945 native-machine instances. It accounts for 1,966,264 physical board bits,
4,594 private board bits, 69 private owner/router bits and 132,484 unchanged
wrapper bits with ACCESS PENDING. Fourteen missing-state/geometry/parameter/
exact-alias cases reject. No sequential exclusions; other categories remain
review items. emu/UIO, top-level MiSTer, raw SDRAM and ARM state are outside XML.
All 413 frozen Native59 input receipts verified before remote staging; all 659
staged files match the local project after path rebasing.

Quartus synthesis passes. The fitter then exits3: 4,954 LABs required / 4,191
available, 49,250 ALMs / 41,910. All nineteen board scopes and three transport
entities remain in the failed-fit attribution. The eight MSA adapters account
for 13,804.5 estimated ALMs. These are failed-fit diagnostics, not legal placement
or a prediction from summed savings. The earlier 45,228-ALM experiment did not
include eight enabled MSA adapters, so coverage differs. No STA/assembly/RBF or
hardware reload followed. The failed summary's zero RAM blocks is no RAM result.

Run `/home/alans/mister/Dorado_Serial61Capacity_20261019` is TERMINAL; builder
2322395 and fitter2325036 are absent. Inputs remain isolated at
`/home/alans/mister/Dorado_Serial61_inputs_20261019`. Do not restart it.
Evidence: `fpga/mister/reports/2026-10-19-snapshot-serial-core/`. All 1,546 raw
archive members pass readback; extracted source/XML/terminal-capacity controls,
actual Quartus aliases and byte-identical 659-file restaging pass.

Next: investigate shared enable/scan-mux factoring in the actual MSA frontends
and FF adapters, preserving every physical field. Qualify any reduction against
the frozen original, full save/restore/abort/first-resume tests and matched
component timing before another combined fit; do not assume an isolated saving
solves the whole-core budget. A legal enabled fit/all-condition timing remains
required. Then finish wrapper/input/video/network/raw-memory/Main/daemon/server
state and real pause/disk-generation barriers. PLL-unlock is only a reset proxy
in this capacity experiment: production must prevent quarantine clearing without
genuine machine reseed. Drain accepted raw transactions before frontend freeze;
keep physical SDRAM refresh running. Daemon pause/resume must preserve connections
and rebase timers rather than using reconfiguring lifecycle RESUME. Qualify
atomic disk-paired FPGA Cedar save/continue/restore.

MiSTer14/Native59 is unchanged; last qualified rate remains 1,947,654.12 issues/s.
Original 60 ns clocks/timers, pinned comparisons, three fresh boots, cold disk
persistence and two-hour combined stress remain required. Goal ACTIVE.

Previous component qualification:

**The shared transport now qualifies all eight actual MSA adapters through SPI. Native builder wiring, real service barriers and a new combined fit are next.**

The new `board_state_fabric_msa.sv` / `board_state_uio_msa.sv` retain slots0..10
and add slot11 through `msa_state_group.sv`. Index bits10..8 select the eight
MSAs in the explicit native member order; bits7..0 select words0..149. Holes
and high bits reject. The group latches its member through completion, requires
every member frozen and aggregates hold/fault ownership. Unknown child ownership
fails closed. The shared budget is 4,096 cycles, covering actual 2,690-edge scans.
Status reports twelve slots/frozen bits; old transport sources remain unchanged.

`serial_state_format_msa.py` binds the qualified receipts into board-only schema
`b80a00977d7e7992`: 1,966,264 physical board bits / 401 runs, including all 21,520
MSA bits. Each member has a 256-word address stride but dense payloads have NO
per-member padding. An independent integer oracle verifies all 1,200 MSA words /
2,690 bytes; invalid holes/padding/status and nine rehashed damaged catalogs reject.
Cached packet lookup runs remain immutable after source-JSON changes.

The four-state test passes 115,191 host ticks / 260 SPI packets / 97 result checks,
including 34 actual Python-formatted packets through all eight real MSA adapters,
the inherited legacy transport contract and unchanged Ethernet. It covers unique
member readback after all writes, first/final words, coherent twelve-bit status,
competing indices held THROUGH completion, busy release, invalid indices/schema,
missing/unknown member/service acknowledgments, timeout, parser-vs-SYSTEM reset,
partial-scan quarantine and stable physical state. All ten damaged RTL variants
reject, including coherent member aliasing, live completion selection, shortened
timeout and a truncated frozen bitmap. Guest advances are stopped here; the other
eleven endpoints and foreign services remain explicit test stubs, not native pause.

Fresh XML retains all 21,520 physical MSA bits + 664 private MSA bits + 417 private
transport bits, with no sequential exclusions; twelve geometry/binding/exact-alias
controls reject. Inverting only the approved owner/UIO edits returns the committed
original serial transport bytes; seven source controls reject. Icarus required
declarations before use, so the qualified MSA's administrative fabric is relocated
with exact byte identity after inverse; original pins/logic/state maps are unchanged.
The surviving live-owner negative control exposed an insufficient fixture interval,
now extended through completion. No production RTL correction was needed for it.

All 641 raw archive members pass hash readback. Consumed original receipt bytes,
including both processor variants, regenerate the exact catalog from extraction.
Extracted MSA/source/fixture regeneration, format and source/XML controls pass.
Evidence: `fpga/mister/reports/2026-10-19-snapshot-msa-transport/`. All jobs here
are terminal. No new Quartus area/timing, RBF, hardware reload or speed gain.

Next: integrate the twelve-slot transport, eleven main boards and eight indexed
MSAs in the native builder, retaining the qualified declaration normalization.
Finish wrapper/input/video/network/raw-memory/Main/daemon/server state and real
pause/disk-generation barriers, then qualify enabled combined capacity/timing and
atomic FPGA Cedar save/continue/restore. Last combined capacity remains 45,228 /
41,910 ALMs (3,318 over); last qualified Native59 remains 1,947,654.12 issues/s.
Original 60 ns clocks/timers, pinned benchmarks, final three fresh boots, cold
persistence and two-hour combined stress remain required. Goal ACTIVE.

Earlier complete-board qualification:

**Complete MSA board serial access is qualified as a component. Shared transport/native wiring and real service barriers remain next; hardware speed is unchanged.**

`generate_msa_board.py` binds the frozen native board source, 78 original
sequential packages and 16 unchanged storage groups. All 866 FF/history + 1,824
frontend bits form one 2,690-bit physical chain, 150 words with eight valid bits
in the final word. Global hold and scan abort are separate; a lost boundary
keeps every helper held. Local reset re-entry is tested under an independent
global hold. Native SYSTEM reset still must mean genuine machine reseed.

The actual frozen original board and generated adapter agree over 2,507,118 host
edges / 22,720,360 checks: 40,002 guest edges, 55,552 raw requests and 8,672
writes with variable backend stalls. All 458 fields and normal board/raw-port
outputs are compared. 904 completed API calls cover every valid word, full
physical inversion, destructive reads, complete restore, final-word padding,
invalid 24-bit indices, competing valid requests and 20,000 resumed guest cycles.
Partial-scan abort, lost external hold, raw handshake suppression and held reset
re-entry pass; all seven actual RTL mutations reject. The corrected pre-edge
counter records 2,421,007 physical scan edges, including the partial abort.
This does not save external raw RAM or qualify the physical SDRAM controller.

Fresh XML accounts for all 2,690 physical bits per side plus 83 private bits,
without sequential exclusions; twelve geometry/schema/exact-module-alias
controls reject. Inverting only the reviewed administrative edits recovers the
original board bytes, including package pins/parameters and memory grouping.
All eight helper bodies match their committed qualifications after namespace
inversion; seven damaged-source controls reject. The source audit's own added-
logic control exposed an overbroad fabric erasure, now restricted to an exact
allowlist. Candidate/helper RTL is byte-identical across all four executed
simulation versions; their corrections affect the C++ fixture or diagnostics.

All 344 raw archive members pass hash readback. Extracted original board/cell/
frontend inputs regenerate identical sources; extracted source/XML controls
pass. Evidence: `fpga/mister/reports/2026-10-19-snapshot-msa-board/`.
All experiments here are terminal. No combined fit, RBF or FPGA reload occurred.

Next: qualify a larger shared response budget and indexed mapping for all eight
MSAs (the 2,690-bit chain exceeds the existing 2,048-cycle limit), then wire them
into the native machine and finish wrapper/raw-memory/input/ARM/server state and
actual pause/disk-generation barriers. Qualify combined capacity/timing and
atomic FPGA Cedar save/continue/restore. The last combined result remains
45,228 / 41,910 ALMs, 3,318 over; isolated component qualification is not a new
core fit. Last qualified Native59 stays 1,947,654.12 issues/s. Original 60 ns
clocks/timers, pinned benchmarks, final three boots/cold persistence/two-hour
stress remain required. Goal ACTIVE.

Earlier frontend qualification:

**The native storage frontend now has qualified physical serial access. MSA board integration and the coordinated raw-memory boundary remain pending.**

`generate_storage_group.py` binds the exact archived native nondeferred source
(SHA256 `542b7f2b27a27fca6d7263e8d2a9631ce24de13cab952c791687aef4ee30fcfa`).
Both normal process bodies and original pin logic are retained. The actual
pre-edge guest enable advances the guest-clocked fields; host completion keeps
running through guest stalls. Final hold freezes both domains, suppresses raw
request/response handshakes and permits rotation of the existing physical FFs.
The drain-ready output is a pre-hold condition, not a persistent acknowledgment
while physical state rotates. Every accepted raw transaction must be drained by
the eventual external coordinator; physical SDRAM refresh must keep running.

Both native widths pass against the original frontend AND original DRAM bit
chips: 110,808 host edges / 2,237,064 checks / 5,010 guest edges / 10,948 raw
requests / 924 writes / 63,324 scan edges. Nine destructive save/restore exercises
include pending reads, pending writes and physical write-done before guest
commit; first legal guest edges, changing held pins/reset/handshakes, unheld
scan attempts, full walking-bit loads, eight reset episodes and all 1,310,720
oracle RAM bits pass. Ten actual RTL mutations and two changed-source controls
reject, including a falsely asserted drain-ready boundary.

Fresh frontend-only XML retains 234 physical bits per side, two private hold
acknowledgments and 64 fixture address bits, with no sequential exclusions;
nine damaged inventory controls reject. The functional fixture retains the DRAM
oracle. Its hierarchical RAM initialization is deliberately outside this XML,
whose generic reader correctly rejects such cross-hierarchy assignments.
Initial working-tree deferred-capable-source trials were rejected by the native
source identity check; the final generator and both references use the frozen
native bytes, and the complete suite reproduces there. All 263 raw archive
members pass hash readback. Extracted native-source regeneration, XML/state
controls and alternate-source rejection pass. Evidence:
`fpga/mister/reports/2026-10-19-snapshot-storage-group/`.

Next: combine the 866 original FF/history and 1,824 frontend bits per MSA board,
qualify complete board access/hold/abort, then finish remaining wrapper state and
actual service barriers. A single 2,690-bit MSA chain exceeds the existing
2,048-cycle transport budget; its mapping/deadline must be explicitly qualified.
No new combined area/timing, RBF, hardware restore or speed result is claimed.
Live Native59's last qualified rate remains 1,947,654.12 issues/s. Complete
snapshots, original 60 ns clocks/timers, pinned comparisons and final boot/
persistence/two-hour stress gates remain open. Goal ACTIVE. All jobs here are
terminal.

Earlier board and primitive qualification:

**All eleven main-board serial adapters now pass namespace and global hold/abort qualification. Native MSA/wrapper state and service barriers are next; no new full-core fit or deployment is claimed.**

`generate_serial_boards.py` derives the adapters from the exact qualified fit
sources and receipts. All 148 module definitions have board-specific names,
including the differing BaseBoard helpers. ContA's counter cast is normalized
with its valid-index equivalence checked. Inverting the namespace and exactly
four administrative hook edits recovers the original source bytes; 66 damaged
source controls reject. Original clocks, normal processes, RAM grouping,
parameters, pins and state addresses are preserved.

Every board passes 20,180 host edges / 81,029 checks against its original
qualified adapter: 20,000 ordinary cycles, 32 forced-hold probe rows, partial
scan abort and 32 further held edges. Physical fields and direct large-RAM
samples are compared; every helper's hold flag is checked. These tests supplement
the earlier exhaustive board memory gates rather than repeating full memory
traversal. Nine actual RTL mutations on ContA, MemX and BaseBoard independently
reject missing forced hold, scan abort and auxiliary-access abort guards.

Fresh reference/DUT XML retains all 1,944,744 physical and 3,930 private bits,
with no sequential exclusions; 77 damaged inventory/alias controls reject.
A narrow XML adapter resolves Verilator's encoded cell module references only
to an exact existing specialization. Raw XML remains unchanged; normalized XML
and every alias rewrite are retained. All 2,983 raw archive members pass lossless
hash readback. Regeneration from extracted original receipts is byte-identical;
extracted source and classification audits pass. Evidence:
`fpga/mister/reports/2026-10-19-snapshot-serial-board-global/`.
The board-only catalog identity remains `c876f8b45b768814`.

**MSA primitive access is now qualified; native MSA board/frontend access is pending.**
`generate_msa_ff.py` adds pinned-source adapters for SN74166, SN74S174 and the
shared MK4096P address latch (both 7- and 8-bit row geometry). Original pin
logic and sequential process bodies are preserved under host advance; held
loads include all sampled clock/strobe history. Independent state/pin oracles
and the actual original RTL agree over 106,111 host edges / 2,118,261 checks,
including exhaustive TTL pins, walking state bits, held/unheld/invalid writes,
destructive restore, the first resumed edge and 20,000 further cycles. Seven
actual RTL mutations reject; three changed-source controls reject. Fresh XML
accounts for all 50 representative physical bits per side and zero private
bits, with no sequential exclusions; seven damaged inventory controls reject.
V2 makes the same truncated PRNG seed explicit as a 32-bit literal, removing a
compiler warning without changing any result. Native classification V2 corrects
subset summary metadata without changing decisions.

Replaying the exact native XML and revalidating all 216 source receipts now
classifies all eight MSA instances: 21,520 required bits / 3,664 fields, all SAVE,
no sequential exclusions. Each board has 866 original FF/history bits and 1,824
frontend bits. Nine damaged classifications reject. The three new primitive
families cover 636 bits per board (5,088 total); 230 original bits per board
reuse existing families. MSA board access, all frontend access and their actual
hold boundary remain pending. All 364 raw archive members pass hash readback;
extracted original sources regenerate identical helpers and extracted XML/source
classification controls pass. Evidence:
`fpga/mister/reports/2026-10-19-snapshot-msa-ff/`.

The other native-wrapper candidate state remains review-only. Expose the MSA
board/frontend state, then complete the other wrapper/backend, input, video,
network and external 16 MiB raw-memory state. Implement actual backend/Main/
daemon/disk-generation barriers and preserving server pause/resume with timer
rebasing. SDRAM refresh must continue while guest interfaces are held. SYSTEM
reset must mean a genuine machine reseed; parser reset or a soft reset leaving
shifted physical state cannot release quarantine.

Qualify combined serial capacity/timing and atomic disk-paired FPGA Cedar
save/continue/restore before claiming full snapshots. The last combined result
remains 45,228 / 41,910 ALMs (3,318 over). Live Native59 and its last qualified
1,947,654.12 issues/s are unchanged. Original 60 ns clocks/timers, pinned
comparisons, three final fresh boots, cold persistence and two-hour stress
remain required. Goal ACTIVE; all experiments in this entry are terminal.

Earlier transport qualification (superseded next-action list):

**Serial transport ownership and the compact eleven-board format are qualified as isolated components. The new transport costs 39 additional ALMs and passes four-condition timing. Native machine/service barriers and the combined serial core are not yet integrated.**

`board_state_fabric_serial.sv` replaces the old 16-edge deadline with a bounded
2,048-cycle budget for the longest 2,004-cycle physical scan. Hold ownership
is independent of the SPI parser: a transport reset while held/active,
timeout, lost local/service boundary or child fault preserves quarantine until
SYSTEM reset. Explicit clean release remains possible after a normal session.
Busy release and competing requests are rejected. Child hold/fault ownership
is aggregated across all eleven boards. Admission requires the local domain,
all eleven board acknowledgments and all five foreign-service acknowledgments.
Global hold and scan abort are separate outputs: integration must keep EVERY
RAM/FF/foreign consumer held while abort stops the scan controller. These are
contracts, not native backend or ARM pause implementations; no acknowledgment
may be tied high to bypass the missing implementation.

`board_state_uio_serial.sv` uses ABI2 with key, version and four16-bit schema
words on every control/request. Exact deselected 8-word controls and 13-word
requests commit; malformed/short/long/old-ABI/wrong-schema packets are inert.
Status is coherent, reports all ownership/acknowledgment/error fields, and
permanently identifies board-only scope. Commands76..79 coexist with the
unchanged Native59 Ethernet70..75 interface. The old ABI1 sources are retained
unchanged as the earlier experiment.

Four-state V6 passes 12,238 host ticks / 141 packets / 44 checks using the REAL
2,004-bit scan controller and explicit stub boards/foreign domains. Full scan
rotation, last-word padding, valid competing requests, busy release, partial
packets, counter rollover, coherent status, missing/unknown/lost acknowledgments,
active/paused/idle parser resets, timeout and sticky quarantine pass; all
nineteen damaged RTL controls reject. V1 fixes declaration order. V2 corrects
an earlier/later valid diagnostic. V3's competing request was out of range and
independently blocked by address validation; V4 uses a valid address. V4/V5
then correct observation timing for malformed controls and one mutation's token
separator. Candidate transport RTL is unchanged across these fixture
corrections. Histories are archived. Fresh XML accounts for all389private
transport +728staged-shell bits, no sequential exclusions; thirteen damaged
inventories/bindings reject.

Matched old/new enabled transport fits preserve the same staged-positive-edge
50 MHz/20 ns shell and SDC: 577 ->616ALMs; 1,089 ->1,165registers; no RAM.
All four device conditions pass, minimum+0.109ns. Exact source/clock/constraint,
owner/parser retention, constant-field pruning, warnings and unconstrained-path
checks pass; nine damaged fit controls reject. The fit uses an explicit prototype
schema constant; the combined core must instantiate its actual catalog identity
and repeat complete-core timing. This is not a measured whole-core area delta.

`serial_state_format.py` binds all eleven qualified final simulation receipts
and complete address maps into `dorado-serial-boards-v1` / ABI2, with dense LSB0
payloads and385compact runs covering all1,944,744modeled board bits. IFUM/cache/
map RAMs and their pipelines were tested through separate oracle/probe buses,
so those full maps are explicitly included alongside the flat FF/device maps.
The catalog fingerprint is SHA256 and its diagnostic schema ID is
`c876f8b45b768814`. A matching whole-build/disk/service bundle is still required
for machine restore; this catalog is board-only. The formatter validates once
and copies immutable lookup runs, avoiding per-word full-catalog hashing.
Every board payload round-trips; an independent cross-byte integer oracle,
invalid maps/padding/frames/status and mutation of the original JSON are tested.
Twenty-four actual Python-formatted packets drive the real UIO/owner/longest-scan
RTL at the actual catalog identity, alongside the existing contract tests.

All891raw files pass lossless hash readback; extracted fit/XML/negative controls
and catalog regeneration from archived original receipt bytes pass. Evidence:
`fpga/mister/reports/2026-10-19-snapshot-serial-transport/`.
Quartus session17418, transport67925 and format41200 are terminal.

All eleven individual serial boards remain qualified (latest BaseBoard saves
839ALMs, ContB58, in isolated fits). The last combined result remains
45,228 /41,910ALMs, 3,318over capacity. No production builder, RBF, live FPGA,
clock, timer or speed change is claimed. Next: namespace the three differing
BaseBoard FF helpers and normalize ContA's equivalent counter cast; connect
forced global hold and separate abort to every board. Classify/expose the
154,037native-wrapper candidate bits (review-only), top-level input/network/
video and16MiBexternal raw RAM, then implement actual backend/input/Main/daemon/
disk-generation barriers and acknowledgments, including a preserving server
pause/resume path and timer rebasing. Qualify actual combined capacity/timing
and coherent, atomic disk-paired FPGA Cedar save/continue/restore.

Original60ns clocks/timers, pinned comparisons, three fresh boots/cold persistence
and two-hour stress remain open. Last qualified Native59 idle stays
1,947,654.12issues/s. Goal ACTIVE. All experiments in this entry are terminal;
older running-job entries are historical.

**Enabled60 exceeds FPGA capacity; the first state-preserving area fix is qualified and a new full-core build is running.**

The original all-enabled experiment is TERMINAL: synthesis passed, fitter
failed (exit 3). It needs 5,505 LABs / 4,191 available; the failed summary
reports 54,403 ALMs / 41,910, an excess of 12,493. No STA, assembly or reload
followed. The eleven-board source audit remains valid. Failed-fit attribution
puts five BaseBoard RIOTs at 7,341.5 ALMs; these are diagnostic estimates,
not a legal placement or a prediction for the next fit. Evidence:
`fpga/mister/reports/2026-10-19-snapshot-enabled-capacity/`.

Normal and administrative RIOT RAM ports now share their mutually exclusive
address/data/write-enable path. There is no added state, reset change, latency
change, valid mask or snapshot ABI change. The original/independent oracle
passes all 1,097 physical bits and addresses; six broken RTL controls reject.
Fresh XML and synthesis retain the same 1,097 physical + 21 transport bits,
with no sequential exclusions. Matched old/new enabled fixtures pass all four
conditions (minimum +0.167ns), 1,645 -> 782 ALMs, zero block RAM, and seven bad
fit/retention controls reject. These are isolated counts, not a whole-core
saving. Full BaseBoard positive restore gate passes again: 526,012 host edges,
all 40,366 bits and 131,072 indices, 402,862 requests and 20,000 post-restore
cycles. Ordinary extra-SRAM writer counts remain zero; their unchanged earlier
primitive/bank gates still provide that coverage. Evidence:
`fpga/mister/reports/2026-10-19-snapshot-riot-shared-port/`.

The next all-enabled build is LIVE at
`/home/alans/mister/Dorado_Enabled60SharedRIOT_20261019`, builder PID 1875288,
local session 53356. Synthesis passed; fitter PID 1878054 is running.
Only M6532_snapshot.sv differs among the
22 prepared HDL assets; all eleven boards and 1,944,744 modeled bits remain.
Both local and exact remote source audits pass, including fifteen damaged
controls. Observe this same job; capacity/timing remain unqualified. The
builder still omits assembly/deployment. All other new qualification jobs are
terminal. Live Native59 is unchanged; last qualified idle remains 1,947,654.12
issues/s, with no clock, timer or speed improvement claimed.

Fresh native-wrapper support XML identifies another 154,037 sequential
candidate bits outside the eleven boards, mainly the disk buffer and raw
memory frontends. This is REVIEW ONLY; all candidates still need classification
and access. Top-level input/network/video, external raw RAM and ARM state are
outside that XML. Existing daemon lifecycle RESUME calls configure(), losing
old protocol connections; snapshot pause/resume needs a distinct preserving
path, including timer rebasing, rather than reusing core-load STOP/RESUME.

Next: read the new full fit and use its measured attribution for further
RAM-port/state-read/decode area reductions without dropping state coverage.
Then complete support-state/global barriers, the atomic disk-paired bundle
and actual FPGA Cedar restore. Original 60ns clocks/timers and final three
fresh boots, cold persistence, pinned comparisons and two-hour combined stress
remain open. Goal ACTIVE.

**All eleven board state interfaces are connected; transport and source integration pass. Full-core fitting is running.**

The new bounded fabric and diagnostic EXT_BUS commands 76–79 enable all eleven
qualified adapters, including the complete BaseBoard: 1,944,744 modeled board
state bits. Exact packet validation, full-width index checks, one outstanding
owner, all-board frozen acknowledgments and timeout/lost-boundary quarantine
are implemented. Status is coherent and explicitly identifies board-only scope.
This does not yet capture raw memory, wrapper/input state, ARM work/server
connections or the matching disk generation.

Four-state simulation v6 passes 9,974 host ticks, 433 packets and 201 checks,
including actual unchanged Native59 Ethernet UIO coexistence. All 13 broken
RTL controls reject. An initial index-zero decode unknown was fixed with
always_comb; earlier diagnostic attempts are retained. The independent source
audit checks all 413 frozen receipt files, 2,137 original board pins, complete
ordinary backplane equations, 85 other core files, all state routes, original
clock/memory policies and unchanged constraints. Fifteen damaged integrations
reject. Both local staging and the exact remote build inputs pass. Quartus's
complete set of relative source aliases resolves to the same eighteen files.
All 398 archived raw files pass lossless readback. Evidence:
`fpga/mister/reports/2026-10-19-snapshot-board-state/`.

The enabled experiment is `/home/alans/mister/Dorado_Enabled60_20261019` on
misterubuntu, builder PID 1827796 / fitter PID 1835666. Synthesis passed; fitter
is confirmed actively using CPU. Observe this same job rather than restarting.
No assembler/deployment runs. Capacity and routed timing remain unqualified.
Live Native59 is unchanged; last qualified idle rate is 1,947,654.12 issues/s.
No clock, timer or speed improvement is claimed.

Next: finish this fit and independently audit all-device/eleven-board timing,
state retention and memory placement. Resolve capacity/timing costs while
preserving complete state coverage. Then implement coherent backend/input/
ARM/server/disk barriers, the atomic bundle and actual FPGA Cedar restore.
Original 60 ns clocks/timers and final three fresh boots, cold persistence,
pinned performance and two-hour combined stress remain open. Goal ACTIVE.

**Complete BaseBoard save/restore qualification passes; it is not yet installed in the live core.**

The actual nominal board now has a single bounded interface for all 40,366
modeled state bits: the original 1,725-node 6502, five complete RIOTs, four
SRAM byte banks, eight EPROM pipelines and all 279 clock/reset bits. The
48 selectors expose 4,905 valid words in a 17-bit address space. All shared
helpers remain byte-identical to their qualified versions; the new CPU
wrapper only forwards state access to the original vendor node adapter.

Accepted simulation v8 compares unchanged original RTL, every physical state
bit and all 100 public output bits, plus independent SRAM and RIOT models.
It passes 526,012 host edges, 402,862 requests, every state bit, all 131,072
indices, dense capture, candidate-only destructive continuation, restore,
first-edge resume and 20,000 more cycles. All 11 broken variants reject.
The standalone firmware stimulus writes none of the extra SRAM banks; normal
writes remain covered by the earlier 32-cell/byte-bank oracle qualification
and original wire-list grouping, not by a claimed full-board writer gate.
Fresh complete-board XML maps all 40,366 mutable and 65,536 immutable image
bits with no excluded sequential fields; 17 damaged schemas reject. Empty
EPROM sockets retain the original uninitialized semantics.

Matched 50MHz fits and independent audit pass all four conditions, minimum
+0.159ns, no illegal/unconstrained clocks or I/O and no added cuts. Both retain
98,304 RAM/ROM bits in 11 M10Ks with old-data SRAM and exact synthesized
firmware bytes. All 279 FF bits remain in their cell hierarchies; the CPU
has 1,744 synthesis registers and each RIOT 1,115, including private response
state. Shell totals 3,754 ALMs/6,657 regs -> 10,702 ALMs/8,356 regs; these are
isolated board counts, not whole-core enabled capacity. Eight bad-fit controls
reject. All 155 compiled inputs, accepted simulator inputs and helper bytes
pass archived extraction/hash/independent-audit readback. Evidence:
`fpga/mister/reports/2026-10-19-snapshot-baseboard-full/`.

The full simulation needs a 32MiB compiler stack; the test now configures its
own child process. Earlier compile/fixture failures and the terminal compiler
diagnostic are retained. Only that diagnostic's 20.4GB of derived AST objects
were removed; exact inputs, logs and an object-size manifest remain.

No live RTL, speed or timer rate changed. Next: integrate this board into the
complete-core experiment and check capacity/timing with every administrative
path enabled. Then implement coherent backend/input/ARM/server/matching-disk
barriers, the atomic snapshot bundle and actual FPGA Cedar restore. Original
60ns clocks/timers and final three fresh boots, cold persistence, pinned speed
comparisons and two-hour combined stress remain open. Goal ACTIVE.

**All remaining BaseBoard clock/reset primitive families are qualified; complete-board integration is next.**

Nine new local adapters cover70representative state bits and87actual board
bits: SN259/SN163/SN175/SN74, MC1690, MC12061, MC14521B, MPQ3303 and actual
CLOCKGEN N4. Accepted simulation v4 checks unchanged original cells and an
independent state/output oracle across170,049host edges/477held/184writes,
all70bits/all local indices/all65,536pin patterns, high-clock histories,
partial25bit counter words, overflow/POR saturation, candidate-only destructive
continuation/restore/first edge and20,000more. Eleven broken RTL variants
and nine changed sources reject. Earlier fixtures are retained rejected:
v1/v2 drove out-of-width C++ port values;v3missed an invalid-write mutation
because the write matched current state. V4 masks the18bit port and writes
complementary data at each invalid row; no candidate RTL changed for these fixes.

Fresh primitive classification maps every field and rejects78bad schemas.
Fresh actual BaseBoard classification accounts for all279clock/reset bits
across30packages and preserves all other inventory candidates;13damaged
source/schema/PRECAP/CLOCKGEN/SYSPER controls reject. The other192bits reuse
MC10176/MC10136/SN174 adapters byte-identically to Native59's compiled inputs.
The actual clock-generator file has older comments than the workspace copy;
its Verilog tokens match, and tests/fits use the actual Native59 bytes.

Matched50MHz fits and independent audit pass all four operating conditions,
minimum+0.145ns, no illegal/unconstrained clocks/I/O or extra cuts. Synthesis
retains all70cell state bits exactly. Shell totals277ALM/150regs ->314ALM/
302regs, noRAM; these are not whole-core enabled costs. Eight damaged-fit
controls reject. All61compiled inputs, accepted simulator inputs and raw
archive bytes pass extraction/hash/independent-audit readback. Evidence:
`fpga/mister/reports/2026-10-19-snapshot-baseboard-ff/`.

No production BaseBoard/live core/timer rate changed. MiSTer14 Main29511 and
server29510 remain present; latest observed CRC/RX/abort0,dropped23. Last
qualified idle rate remains1,947,654.12issues/s. All our qualification jobs
are terminal. Next: integrate every qualified family into a complete BaseBoard
adapter and prove full classification/save/restore/timing. Then enabled
whole-core capacity/timing and coherent backend/input/ARM/server/matching-disk
barriers/atomic bundle/physical Cedar restore. Original60ns clocks/timers and
final three fresh boots/cold persistence/pinned performance/two-hour combined
stress remain required. Goal ACTIVE.

**BaseBoard EPROM snapshot qualification passes, including empty-socket unknowns.**

All eight actual 2716 sockets now have qualified nine-bit read-pipeline state
(72 bits total). Four original images remain immutable and four empty
INIT_FILE sockets retain their uninitialized semantics. Four-state Icarus
simulation checks the untouched originals and an independent ROM/pipeline
oracle: 34,735 host edges, every pipeline bit/all32indices/everyROMaddress,
X/Z state, input/enable holds, destructive continuation/restore and first
real-edge resume. Eleven broken RTL variants and two source guards reject.
Fresh actual-board classification accounts for all eight pipelines and four
immutable images;13bad schema/source/image controls reject.

The first fit is REJECTED: directly restoring d prevents block-ROM inference
and costs1,529ALMs/563regs/zeroRAM. The corrected adapter leaves the ROM read
pipeline intact and holds restored data separately until the first real
guest edge; it adds no guest warm-up edge. Accepted simulation is v3; v2's
Icarus declaration-order failure is retained. Corrected matched50MHz fits
and independent audit pass all four conditions/min+0.165ns, no illegal or
unconstrained clocks/I/O. Both variants retain65,536firmware bits/eightM10Ks;
all synthesized firmware bytes match all four assets in both variants.
Shell totals271ALM/89regs ->347ALM/531regs include admin/pruning and are not
whole-core overhead. Twelve damaged-fit controls reject. All28compiled
inputs and raw archived bytes pass extraction/hash/independent-audit readback.
Evidence: `fpga/mister/reports/2026-10-19-snapshot-i2716/`.

No production BaseBoard/live FPGA change or speed gain is claimed. Native59
remains the interactive baseline; its last qualified idle issue rate is
1,947,654.12/s. All new simulation/Quartus/audit jobs are terminal.
Next: remaining279BaseBoard clock/reset/FFbits (reuse qualified families),
then complete BaseBoard classification/restore/timing. Enabled whole-core
capacity/timing, coherent backend/input/ARM/server/matching-disk barriers,
atomic bundle and physical Cedar restore remain open. Original60ns clocks/
timer calibration and final three fresh boots/cold persistence/pinned speed/
two-hour combined stress remain required. Goal ACTIVE.

**BaseBoard SRAM snapshot qualification passes. Native59 remains the interactive hardware baseline.**

The actual 32 Intel 2125 slices are grounded in the original wire list as
four independently selected 1024x8 banks. The adapter reuses the qualified
RAM transport unchanged and preserves all 32,768 RAM bits plus 32 registered
read bits, including the output-disable behavior during writes. Original
MCD wire names define word-bit order; no 6502 byte-order assumption is made.

Simulation passes 164,821 host edges, all 32,800 state bits, all 8,192 combined
indices, CE/WE combinations, backend holds, destructive continuation/restore
and first-edge resume. Nine broken RTL variants reject. Fresh actual-board
XML classification accounts for every SRAM field and retains other board
state outside this scope; 12 bad schemas and 11 bad grouping controls reject.
Matched 50MHz fits and an independent read-only audit pass all four operating
conditions, minimum slack +0.211ns. Both variants retain 32,768 RAM bits in
four physical M10Ks with old-data collision behavior. Isolated shell totals
are 171 ALMs/112 registers -> 290 ALMs/330 registers; these are not whole-core
snapshot overhead. Ten damaged-fit controls reject. All 24 compiled inputs
and archived raw bytes pass extraction/hash/audit readback. Evidence:
`fpga/mister/reports/2026-10-19-snapshot-i2125/`.

No production BaseBoard or live core was changed. Native59 Main29511 and
server29510 are confirmed present, with RBF db6883e4. Latest observed counters
are CRC/RX/abort0 and dropped23; do not claim zero drops or sustained final
qualification. The last qualified idle rate remains 1,947,654.12 issues/s.

Next: qualify the eight EPROM read registers and remaining 279 clock/reset/
FF bits, then complete BaseBoard classification/restore/timing. Check enabled
whole-core snapshot capacity and timing, coordinate backend/input/ARM/server/
matching-disk state and barriers, and demonstrate physical Cedar save/restore.
Original60ns clock/timer migration and final repeated boots/cold persistence/
pinned performance/two-hour combined stress remain open. Goal ACTIVE.

**Native59 full Viewers/input/warm file PASS; complete MCS6532 wrapper PASS.**

Live MiSTer14 Main 29511 / server 29510 / RBF db6883e4 are unchanged.
Ready, CommandTool, Clock, WatchTool and EditTool are visible. Real mouse
movement to native912,8 Cmd and click opens a second initialized CommandTool;
separate focus737,608 and .2s/event typing produce exact short and alphabet/
digit echoes. Normal `echo rtl59persist > ///rtl59persist.txt` then `type`
returns rtl59persist. The first capture was mid-render; the settled capture
completes without retyping. Background startup remains active in the first
pane. No normal59 guest Boot, cold marker, clean shutdown or global snapshot
is claimed. Idle measurement remains 1,947,654.12 issues/s; no speed gain.

Bounded desktop collector 21786 is terminal: 90,163 complete disk records,
no partial final record, recorded disk error or trace loss. Raw archived log
bytes were rehashed against its manifest. Evidence native59-field/desktop
includes captures, exact input tools/logs and warm-marker proof. Closed58
/tmp trace duplicates alone were removed after exact retained SD hashes and
no-open-FD checks, freeing 11,766,051 bytes. Old58 pack and SD traces remain;
active59 files were untouched. Current tmpfs has about31MiB free.

Complete actual MCS6532 wrapper now passes all 1,098 bits (1,097 RIOT +
phase_d),134 valid words/all256indices, independent full-state/output/timer/
phase oracle and untouched original wrapper. Accepted v2:91,501 host /
4,469 held / 4,135 requests / 2,600 writes /17,772 phase rises /4,083 late
changes; high-phase save, RESET INACTIVE at first high resume, no synthetic
RIOT tick, subsequent fall/rise and20,000more. Eight broken RTL/two changed
sources reject. V1 passed; v2 strengthens the first edge against reset
masking and compiles captured immutable test/reference inputs.

Fresh wrapper classification saves phase and all child state, rejects12
bad schemas. Vendor classifier now pins the PA7 declaration/procedure/source
instead of incidental XML file IDs;12 source/schema controls reject. Corrected
matched wrapper fit99666 and independent audit are terminal PASS across four
conditions/min+0.162ns/noillegal/unconstrainedIO/noextraexceptions;10 damaged
fit/schema/binding controls reject. Actual child init_7800=0 checked. Snapshot
synthesis1121=1098state+23interface bits (child1116/own5); shared frozen merges,
unused child done/error prune. Baseline1091/own1, child1090. Fitter shell
672ALM/1208regs ->2064ALM/1351regs, no block RAM. Not whole-core enabled cost.
First fit20549 is retained REJECTED: Quartus10207 on a private oe monitor.
V2 fixture uses the same public-pin OE equation, with unchanged RTL/SDC/clock.
All25 compiled inputs and accepted test sources were archived/read-back
verified. Evidence snapshot-mcs6532. RIOT milestone PUSHED e7a8fc7a. No live
production BaseBoard change was made; the new helpers remain isolated.

All our Quartus, simulations, audits, observers, collectors and input jobs
are terminal. Only normal Main/server/guest startup are running. Keep59 until
the next qualified load; its bottom CommandTool remains focused737,608.

Next: finish the actual BaseBoard's32 i2125 RAM slices/registered reads,
eight EPROM read registers and remaining279 clock/reset/FF bits, reusing
qualified families. Qualify grouping, complete board classification/restore
and matched timing on actual nominal SYSPER4/CLOCKGEN N4 sources. Then check
resource/timing with all administrative paths live before treating isolated
or admin-off counts as a global fit budget; implement coherent backends/
wrapper/input/ARM/server/matching-disk-generation bundle and actual FPGA
restore. Original60ns clocks/timer calibration, final3freshboots/coldpersist/
pinnedperf/two-hour combined stress still required. Goal ACTIVE.

**Native59 base loader qualified; complete RIOT primitive qualified.**

Native59 remains on MiSTer14 (Main 29511, server 29510, RBF db6883e4,
DDR epoch aa846a314ec45d23). The uninterrupted original-firmware boot has
completed all 34 transfers / 33 BCDs / 1,345,843 bytes and the exact End.bcd
peer/probe guest ACK. Visibility interval: 334.464367174–334.714855993 s.
Three damaged ACK/data/peer controls reject. Observer local 68059 / remote
29777 exited normally after the ACK; PID absence was independently checked.
Do not restart it. Bounded SD-spooled collector 77615 is terminal and verifies
all 34 served-source hashes after timing, plus retired58 pack 8b5777e6 and
107,856 closed disk records / barrier_end. No recorded disk errors or trace
loss; this is not physical received-payload hashing or a machine snapshot.

Whole-loader issue bounds: 553,796,638–557,144,415. Interior rate:
1,662,555.10–1,662,761.55 issues/s. There were 112 protocol replay records:
56 in AMTypes, 56 in Tioga (58 had 46 in EditTool). CRC/RX/abort remain zero;
drops are nonzero. This single manually gated first boot does not establish
a speed gain or a causal replay regression. Idle remains 1,947,654.12 issues/s.

The actual profile prompt was captured; lowercase n + Return selected No.
BootEssentials finished, and TiogaFonts is installing normally. Do not send
more login/profile input or reset. Full59 Viewers/input/warm marker remains
pending. Evidence: native59-field/post-loader; all captures retained.

The actual MOS RIOT init_7800=0 now passes complete 1,097-bit state access:
128x8 RAM + 73 control/read/timer/PA7-history bits in 133 words. Accepted v2:
54,701 host / 4,448 held / 4,109 requests / 2,593 writes, every bit/index,
all RAM words/four prescalers, 27 underflows / 2,113 PA7 events, late input
changes with advance asserted during hold, dense saved state, destructive
continuation/restore/first edge/20,000 more. Nine broken RTL variants and
two changed sources reject. Fresh actual XML classification rejects ten
bad schemas. First fixture is retained as rejected (advance stayed low in
freeze, so ignore_freeze survived); corrected tests, not RTL, close that gap.

Matched RIOT fits (19536 / builder 1685860) and independent audit are terminal
PASS across four conditions, minimum +0.176 ns, no illegal/unconstrained I/O
or extra exceptions; eight damaged-fit controls reject. Enabled synthesis
retains 1,118 = 1,097 state + 18 response + 3 handshake registers. Baseline
normal pruning leaves 1,090. Fitter shell 660 ALM / 1,206 regs -> 1,645 ALM /
1,350 regs, zero block RAM. These are isolated counts, not global snapshot
cost. All 19 compiled fit inputs were archived/read-back verified. Evidence:
snapshot-riot. Wrapper phase_d is separate required state, not yet covered
by this vendor primitive. No production BaseBoard/clock change was made.

Next: finish this same59 Viewers/mouse/exact keyboard/warm marker test;
qualify each MCS6532 wrapper's phase history and state routing; finish
remaining BaseBoard RAM/EPROM read pipelines/clock-reset families and full
board state qualification. Then wrapper/backends/input/ARM/server/matching
disk generation/global atomic bundle and actual FPGA restore. Original
60 ns phases/timers, final three fresh boots/cold persistence/pinned repeats/
two-hour combined stress remain required. Goal ACTIVE.

**Native59 fresh original-firmware Cedar login PASS; Guest/base loader running.**
Live Main29511/server29510/RBFdb6883e4/newDDR aa846a314ec45d23 unchanged.
Actual login screenshot and real Guest/Grapevine-down/Basic.Loadees entry
captured. Preliminary issue1/state43 and845 cleared withoutretry. C watcher
49389 is TERMINAL after actual original Initial BFN113; neverrestart.
Observer local68059/remote29777 VERIFIED LIVE,20minute bound; output
/tmp/dorado59-loader-observer.jsonl, stopfile /tmp/dorado59-loader-observer.stop.
Do not stop/restart until actual34-file sequence/End.bcd finalguestACK.

Fixed observerREADY ->33sample idle ->immediate Guest wrapper19177 TERMINAL.
Rates1,947,836.01/1,947,570.25/1,947,556.10, mean1,947,654.1206588426issues/s,
unchanged58(1,947,767.345); no speed gain. Qualified StartCycleRun/Jam issues,
excludingRepeatCur, not drainedretirements;50MHz host edges checked. Initial
status measurement is preserved separately in preview-idle and is not the
fixed cadence run. Prior manual login gate stillvaries, so this is not the
final pinned threebootbenchmark. Guest input completed; currentbase packages
are transferring, CRC/RX/abort0. No GDB/hashwork during timed loader.
Evidence native59-field/login and actual captures. Source/binary/timing/
deployment/complete6502state qualifiers PUSHED b9d7b45a/d73377ae. AllQuartus/
6502audit/deploy/boot/input/measurement collectors terminal; only loader
observer remains active in addition to normal Main/server.

Next: observe SAMEboot through actualbase34/finalACK, stopobserver normally,
confirmPIDabsent, run tools/analyze_fpga_loader.py and server/analyse_loader_progress.py
against same source hashes/reference; boundedSDspooled postloader collection.
Only answer actualvisible profile prompt, then BootEssentials/fonts/STARTs/
Viewers/mouse/exactkeyboard/warmmarker. Snapshot next actualRIOT/remaining
BaseBoard, then wrappers/backends/input/ARMserver/diskgeneration/globalbundle/
physicalrestore. Original60ns/timers,3freshboots/coldpersist/pinnedperf/2hstress
remain required. GoalACTIVE; no global snapshot or full59desktop claim.

**Native59 is now loaded; original firmware boot running. Netlist6502 routed qualification PASS.**
Native59 build/source/memory/timing qualification was committed and PUSHED
b9d7b45a before normal deployment. Live Main29511/server29510, RBFdb6883e4,
new actual SPI74epoch aa846a314ec45d23/flags0x16/fault0/errors0. Normal59
loader job56592 is TERMINAL/cache-ready/DDR active. Original f3bce2a8 seed
was checked untouched before final cache activation. Normal58guestBoot
captured actual812; realMainnetwork/diskbarriers and temporary29487exit
preserve58pack63,578,624B/SHA8b5777e6d31bf82057e4f039c593b2fc88be87bdec16890b20d9206455946ab4.
Closed58trace107,856records endsbarrier_end/no recordeddiskerror/loss;
old27199/27195/29487 independently absent. Warmmarker only, no coldmarker/
cleanOSshutdown/fullsnapshotclaim. Evidence native59-field/startup and captures.

Core view/input/counter selected through Main only at input_poll. New C
watcher local49389 is TERMINAL PASS:NEW original Initial TX13/uCodeBoot264/
BFN113 captured and C automatically released; boot pulse70832 TERMINAL.
Preliminary issue1/state43 ended withoutretry; actual issued count now grows,
CRC/RX/abort0. Actual post-request screen845 captured. Do not restart the
watcher. Original59Initial request is qualified; Cedar login/desktop/input/speed remain unqualified. Last
qualified58idle1.947767Missues/s; no new speed claim. Collector22324 TERMINAL,
streamed fixed prefixes/SDspool/one parsed record at a time; no activepackcopy.

Actual netlist6502 corrected matched fit v2 builder27664 and independent
audit are TERMINAL PASS:fourconditions/min+0.151ns/noillegal/unconstrainedIO/
noextraexceptions/eightdamaged controls reject. All1725node registers retained,
plus18response/3handshake=1746synthesisentityregs; original767normal regs after
alias/constantpruning. Fitter shell totals443ALM/912regs ->1649ALM/1990regs,
zeroRAM. Not fullcoreenabled snapshot cost. Initial169085virtualpinfixture
failure retained; v2 changes scalarvirtualpindeclarations only, identical
CPUadapter/SDC/budget. Evidence snapshot-netlist6502 plus companion simulation.

Next: observe SAME59firmware after newBFN113 to login, measure idle then
Guest/loader/Viewers/input. Snapshot implementation next actualRIOT+remaining
BaseBoard primitives/RAM/EPROM readhistories/clock-reset state, completeboard
classification/restore/timing; then wrapper/backends/input/ARMserver/matching
diskgeneration/globalatomicbundle/physicalFPGArestore. Original60ns/timers,
threefreshboots/coldpersist/pinnedperf/twohourstress stillrequired. GoalACTIVE.

**Native59 full-core qualification PASS; hardware test next.**
The original Native59 builder (1544016/local87022) is terminal. Independent
read-only audit99108 is terminal PASS:31 summaries/196 detailed timing
categories/four conditions/40 board-corner crossing sets, minimum+0.114ns.
All413 compiled input hashes were verified after extraction.31 damaged
controls reject. Resources with all ten adapters administratively inactive:
30,431ALMs/52,887regs/332M10Ks/35DSP. Native59 separates Native58's paired
cache groups:40 versus38 distinct blocks, same14 logical banks/294,912bits/
OLD_DATA. DskEth133 surviving mutable helpers match normal-state retention;
same eight disconnected helpers pruned. One zero-state MC10174 hierarchy
label differs. Nine immutable ROMs retain nine separate blocks;24PROMassets
across DskEth/MemX/DispY match. No clock/policy/speed or global snapshot claim.
RBF db6883e495bacd51c188f0b22c459937071d4860516ae102bb84c4c7bf441cad,
local build/rtl-executive/native59-candidate; evidence native59-dsketh.
Not deployed yet. Next checkin/push, normal58guestBoot/captureactualMP,
realMainbarriers/preserve58pack, then fresh59originalfirmware/Cedar/input.
Native58 remains interactive Main27199/server27195/RBFeebc0e1f, lastidle
1.947767Missues/s; no new speed measurement.

Actual BaseBoard inventory is reproducible/archived:40,366sequential bits
BEFORE classification,32i2125RAMs/registered reads, actual1725node6502,
fiveRIOTs including block-local old_pa7, clock/reset/edge histories. Eight
EPROM sockets/fourexplicit images; empty-socket semantics still need review.
Evidence snapshot-baseboard-inventory; no board completeness claim.

Actual netlist6502 primitive SIMULATION+CLASSIFICATION PASS:50,972host/
7,832held/6,747requests/4,218writes/all1725bits/all128indices/all16384pin
vectors/destructive restore/firstedge/20Kmore. Eight brokenRTL/two source
changes/eight damaged schemas reject. Vendor CRLF bytes and combinational
netlist/relaxation preserved; no behavioral CPU substitute. Evidence
snapshot-netlist6502-simulation. First isolated matched fit REJECTED by
critical169085:three scalar outputs were not matched by vector virtual-pin
patterns. Fixed fixture declarations only; fresh v2 build local27664,
/home/alans/mister/SnapshotNetlist6502_v2_20261019. Observe same job;
no standalone timing qualification yet. Then RIOT/remaining BaseBoard,
wrapper/backends/input/ARMserver/diskgeneration/globalatomicbundle/actual
FPGArestore. Original60ns/timers/final3freshboots/coldpersist/pinnedperf/
2hstress remain required. Goal ACTIVE.

**Native59 full-core build LIVE; Native58 remains qualified and interactive on MiSTer14.**
Full DskEth+58desktop milestone PUSHED80247606. Native59 job87022/local, remote
builder1544016/Quartusmap1544037 confirmed live; /home/alans/mister/Dorado_Native59_20261019,
parentDorado_Native58_20261019; inputsDorado_Native59_inputs_20261019. Exactexecuted
builder localnative59-build_native_dsketh-executed.py and remote build_native_dsketh_20261019.py.
All11sharedhelpers byte-identical,8newfiles, samewrapper/clocks/SDCs/policies/
nativeMemDdelay2. All10adapters admininactive; no full snapshot or speed claim.
Observe samejob/PID; never restart from timeout. Need terminalmap/fit/asm/STA/
audit/devicecorners/adaptercrossings, then independent31summary/196detailed/
fourcondition/40board-corner checks and allsource/RAM/PROM retention beforeload.

Current14 Main27199/server27195/RBFeebc0e1f passed originalfirmware/fullViewers/
mouse/exactshort+alphabetdigits/warmrtl58persist create/type. No normal58guestBoot,
coldmarker/cleanOSshutdown or fullsnapshot yet. Lastidle1.947767Missues/s/no gain.
When59qualified, issue normalguestBoot/captureactualcode, preserve58viarealMain
barriers, then fresh59originalseed/firmware/Cedar/input. Allourisolatedfits/tests/
collectors/inputjobs TERMINAL; no staleCwatcher/loaderobserver running. Remaining
snapshot work afterDskEth integration: actual BaseBoard/vendor6502, wrapper and
backends/input/ARMserver/diskgeneration/globalatomicbundle/actualFPGArestore.
Then original60ns/timers/final3freshboots/coldpersist/pinnedperf/2hstress. GoalACTIVE.

**Native58 full Viewers/mouse/exact keyboard/warm file PASS; complete DskEth modeled state qualification PASS.**
Same Main27199/server27195/RBFeebc0e1f/50MHz-SYSPER4/adminOFF. Ready/Viewers/
Clock/Watch/Edit visible; mouse toCmd912,8/click/initialized secondCommandTool/
separatefocus737,608; exactshort and alphabet/digits, normal ///rtl58persist.txt
create/type return rtl58persist. Collector50038 TERMINAL/SDspooled97,870complete
diskrecords/no recordederror/loss. Warmreadback only; no normal58Boot/coldmarker/
cleanOSshutdown/fullsnapshot. Freed23,532,399tmpfsbytes onlyfromclosed56b/57trace
duplicates after exactSDhash/no-open-descriptor checks; current58logs untouched.
Evidence native58-field/desktop; localcedar58-launch. Lastidle1.947767Missues/s,
no gain. Leave this core running until next qualified normal load/preservation.

FullactualDskEth2,656mutable/12,032immutablePROMbits PASS:117FFpackages/18small
files+writewindows/twoSNtimers/twoCRCs/oscillator/34monitor/ninephasebits.
256,389hostedges/50,293held/25,232requests/all2,656bits/all8,192indices/destructive
per-wordshadowrestore/quiet/firstedge/20Kmore; all18normalRAMwriters exercised;
9brokenRTL reject. Firstfixture randompinsmissedTxFIFO, v2C++forwarddeclerror,
v3missingMemSHphase; acceptedv4 adds actual4-sample clocks+memoryhalfcycle and
stableI/Osetup. NoRTLfix to passcoverage. FreshactualXML16badschemasreject;
tied outputwiresliteral0/1 classifiedreconstructible, otherunknownstate rejects.
Compile141statehelperregistercounts/mutablefileslogicregs verified. Fullfit55672
TERMINAL/read-onlyaudit26852PASS, all4conditions/min+0.147ns;11badfitcontrols
reject. Baseline1054ALM/2213regs/5M10Ks(immutableROMs) vs enabledisolatedcandidate
3480ALM/3699regs/0M10Ks(PROMsLUTs), same14assethashes. Not wholecore cost.
Evidence snapshot-dsketh-full preserves exact sources/rawreports/earlierattempts.
Timerqualification PUSHED2f33f30c. No native59 hardware or globalsnapshotclaim.

Native59 builder/assets PREPARED, not running yet: actual frozen58sources in
native58-dsketh-baseline-src; native59-dsketh-inputs-v2-20261019.11existinghelpers
byte-identical/8newfiles, exactqualifiedschema guard, DskEthSYSPERbinding retained,
all10adapters adminOFF, no clocks/wrapper/SDC/policy/nativeMemDdelay2changes.
Next checkin/push qualification, build59 onmisterubuntu, audit31summary/196detailed/
fourconditions/40board-corner crossings/allRAM/PROM/sourcebeforeload. Then normal
58guestBoot/Mainbarriers/preservepack and fresh59firmware/Cedar/input. Complete
BaseBoard/backends/wrapper/input/ARMserver/diskgeneration/globalatomicFPGArestore,
60ns/timers and final3boots/coldpersist/pinnedperf/2hstress remain required.

**SN74123 complete timer primitive qualification PASS; full DskEth adapter assembly in progress.**
Original/frozen58source match; all70bits (two signed32bit counters/six histories)
in four18bit words. Six parameter modes incl actualSYSPER4/TW_NS3000/TW400:
110,122hostedges/2,880held/2,872requests/4,800cellwrites, expiry/retrigger/negative
boundaries/destructive restore/quiet/firstedge/20Kmore PASS.9brokenRTL/2changed
source guards reject. FreshXMLactualbindings/signedtypes/allfields;17bad schemas
permode reject. Matched50MHz fits13703 TERMINAL/read-onlyauditPASS/fourconditions/
min+0.140ns;522->602ALM/520->904regs/noRAM/noillegal/unconstrainedIO;5badfit
controls reject. Exact rawreport/source evidence snapshot-sn74123. No production
cell/rate/clock or global snapshot change.

Live Native58 Main27199/server27195/RBFeebc0e1f unchanged; actualNo accepted and
BootEssentials/TiogaFonts/PressFonts/Fontmetrics progressing in captures. No
Viewers/input qualification yet; observer27455 remains terminal/absent. Last
idle1.947767Missues/s, no gain. Keepbootrunning/no extra login input. Allour
Quartus jobs TERMINAL. Next full DskEth classification/restore/isolated timing
including34bit passive receive monitor, all18small files/write windows,117FF
packages, twoCRCs/twoSNtimers/oscillator/ninephasebits. Draftgenerators currently
unqualified. Initial source guard rejected MC10170's older explanatory comment;
review confirms exact Verilog token stream unchanged. Actual compiled file is
retained and both hashes recorded, no cell logic changed.14immutable PROMs.
Then finish58desktop/input and full DskEth/native integration, remaining global/
ARM snapshots, original60ns/timers and final gates. Goal remains ACTIVE.

**Latest Native58 base34/final guestACK PASS; profileNo accepted, BootEssentials advancing.**
Same Main27199/server27195/RBFeebc0e1f. Idle mean1,947,767.345issues/s unchanged57.
Actual34files/33BCD/1,345,843bytes/finalACK322.790994587..322.958138693s, whole
issues532,207,991..535,653,632/interior1.652349..1.652559M/s.46replays, ALLduring
EditTool, versus57's12acrossDFPackage/AMModel; nonzeroqueue/drop10, CRC/RX/abort0.
This single first-boot/manual-login-gate comparison is not a pinnedrepeat,
speedgain or causalreplay/RTLregression diagnosis. Observer11019/27455 TERMINAL
explicitstopAFTERACK/independentlyabsent; do not restart. Actualn/Return accepted
No at visible profileprompt; BootEssentials importingnormally. Collector42659
TERMINAL boundedSDspooling/source34hashesPASS AFTERtiming (not received-payload
hashes). Retired57trace108,830/endbarrier/noerrors/loss and pack7fb124ae reverified.
Exact finalobserver/counterbounds/analysernegativecontrols/sources/captures in
native58-field/post-loader; localcedar58-launch. No new desktop/input claim yet.

DskEth smallFF/actualoscillator coverage PUSHED f2a5b02c; allourQuartus/testjobs
TERMINAL. Next finish58BootEssentials/fonts/STARTs/Viewers/mouse/exactkeyboard/
warmmarker; implement/qualify SN74123 full70bit timers+histories, thencomplete
DskEthboard/monitor/phase restore/timing/integration and remaining BaseBoard/
backends/wrapper/input/ARMserver/diskgeneration/globalatomicbundle/actualFPGA
snapshotrestore. Original60ns/timers, pinnedrepeats,3freshboots/coldpersistence/
2hstress stillrequired. Goal ACTIVE; no completion/speedpromotion.

**Latest Native58 login/Guest and idle measurement PASS; new DskEth primitives qualified.**
Same live Main27199/server27195/RBFeebc0e1f. Actuallogin captured; observer11019/
remote27455 LIVE, bounded20minutes, started before fixed33sample measurement.
Measured59371 TERMINAL, three10s1,947,630.71/1,947,825.94/1,947,845.38issues/s,
mean1,947,767.345, unchanged57; no gain. ActualGuest completed; base loader still
active at26/34transfers,257safterBasic; CRC/RX/abort0, nonzero drop counter8.
Do not restart observer or send more login input. Need final actualACK, stop same
observer then profileNo/BootEssentials/Viewers/mouse/exactinput/warm marker.

Four new source-pinned local DskEth state-word families now PASS:MC1650(2bits),
MC10136(12),SN169(8),SN174(13,PRECAP0/1). Original/independentstate+outputoracles,
111,271hostedges/2,157held/5,230cellwrites/all4096pins/allbits/padding/destructive
individualrestore/quiet/firstedge/20Kmore;8brokenRTL/4changedsource guards reject.
FreshXML5modes/allsequential/constant/derivedcandidates;54bad schemas reject.
Matched50MHz isolatedfits35256 TERMINAL/read-onlyauditPASS/fourconditions/
min+0.164ns;93->113ALM/76->168regs/noRAM/noillegal/unconstrainedI/O;5badfit
controls reject. No production/shared-helper changes. Evidence snapshot-dsketh-ff.
Oscillator test extends9->10modes including actualDskEth23530/SYSPER8, all33bits/
signedboundaries/restoration/20Kmore/8brokenRTL pass; original/generatorunchanged.
Evidence snapshot-oscillator-dsketh is SIMULATION ONLY for addedpair; fullboard
fit/timercalibration remain pending. NeedSN74123 full70bit signed timers+history,
fullDskEth receive-monitor/phase classification/restore/timing and nativeintegration.
ThenBaseBoard/backends/wrapper/input/ARMserver/diskgeneration/globalbundle/FPGA
restore, original60ns/timers/finalgates. Last pushed c7ab6d41.

**LIVE Native58 normal load and original firmware progress PASS; CRC9401 snapshot primitive qualified.**
Normal load77345 TERMINAL2026-10-19 15:14:18UTC, Main27199/server27195,
temporary27171 normally retired. RBFeebc0e1f, fresh untouchedf3bce2a8seed before
finalcache; all9adapters adminOFF, unchanged50MHz/SYSPER4/SDRAM/directDDR.
ActualMainSPI74 epoch146c287efcde2428/flags0x16/fault/errors0, daemonalone64KiB
1FF00000ring; Main no ring/probeDSO. Setup11659/triple96792/collector2217
TERMINAL. C watcher26133/remote27261 TERMINAL/independently absent after NEW
originalInitial BFN113/Crelease. Earlyissue1/state43pause ended withoutretry.
CapturedafterBFN113 actual812; Cedarlogin/loader/desktop/input/new speed pending.
Last measured57idle1.947741Missues/s; no58speed claim. Localcedar58-launch.
Normal57Boot actual812 and realMainbarriers preserve63,578,624byte volume
SHA7fb124ae9211fa2c04532a48d788a5f81f1e89921811feab7c88b3fd06729701;
old25287/25286/temp27171absent;108,830retireddiskrecords/endbarrier/noerrors/loss.
No coldmarker/cleanOSshutdown/snapshotclaim. Evidence native58-field; collector
bounded SDspooling. Removedonly4duplicate/tmpRBFs afterexactpersistentSDhash
verification;~15.8MBfreed. Build qualification+57desktop PUSHED eab5c0ca.

Complete CRC9401 primitive now PASS:19bits per PRECAP0/1,108,190hostedges/
1,123held/1,120requests/576restoredcellwords, all8polynomialselects/all19bits,
destructive restore/firstedge/20Kmore;8brokenRTL/2changedsource guards reject.
FreshXML actualPRECAP bindings and10damagedschemas per mode reject. Matched
isolated50MHz fits68940 TERMINAL/read-onlyauditPASS/fourconditions/min+0.170ns,
65->97ALM/65->201regs/noRAM/noillegal/unconstrainedIO;5badfitcontrols reject.
CRCsource byte-identical frozen57. NoCRCbehavior/timer/productionchange.
Evidence snapshot-crc9401. ActualDskEthinventory correctedforreceive_monitor,
4,546seqbits, tentative2,656mutable including34passivedebugbits; reviewonly.
NeedMC1650/MC10136/SN169/SN174/SN74123 state primitives, exact23530/SYSPER8
oscillator coverage, fullDskEthclassification/restore/timing, BaseBoard and all
remaining device/ARM/global snapshot boundaries. Then60ns/timers/finalgates.

**Latest: Native57 full desktop/input/warm marker PASS; Native58 full-core audit PASS, not yet deployed.**
The captured desktop shows Ready/Viewers, mouse Cmd click, initialized second
CommandTool, exact short and alphabet/digit echoes, and normal create/type of
///rtl57persist.txt. Main25287/server25286/RBF064e3784 unchanged. No cold marker
or complete snapshot claim. Desktop collection now spools bounded prefixes on
SD: /tmp was almost full and the first disk-spooled attempt ran out of tmpfs;
that failure is preserved, no guest pack touched. Accepted SD-spooled collector
11288 TERMINAL; desktop/ retains exact fixed prefixes and reviewed captures.

Native58 build60721 and read-only audit v3 TERMINAL PASS:31summary/180detailed/
fourconditions/36crossing checks, min+0.116ns;30,688ALM/52,887regs/330M10Ks/35DSP.
All9adapters adminOFF, original clocks/wrapper/SDCs/policies/nativeMemDdelay2
unchanged. TenPROMs match. Native-only cache audit verifies all14banks and
294,912bits/OLD_DATA: banks3/5 and8/10 share3M10Ks perpair,38unique versus40
baseline (44per-row allocations double-count sharing). First strict isolated
packing audit rejected; v2fault injector matched a non-table line; v3restricts
to physical table and27corrupt controls reject. No RTL/SDC/refit to fix audit.
Exact source/reports/earlier attempts in2026-10-19-native58-dispy. CandidateRBF
 eebc0e1fa669ba48ae35ebfe7ba93161afa48eca40247ff79cee9e9853293ae2
at build/rtl-executive/native58-candidate. Not deployed. Next archive/commit/push,
normal57guestBoot/capture actual code, Mainbarriers/preservepack, normal58load
and fresh originalfirmware/Cedar/input/speed qualification. No speed gain claimed.

Remaining snapshot state includes DskEth/BaseBoard/backends/wrapper/input/ARM
server/diskgeneration/global atomic bundle/actualFPGArestore. Fresh actual DskEth
inventory v2 includes receive_monitor omitted in first failed elaboration;
CRC9401 two-PRECAP19bit primitive simulation/mutation controls pass, classification
and isolated fit pending. These new primitive files are not yet qualified/committed.
Then original60ns/timers, pinned performance,3freshboots/coldpersistence/2hstress.

**Latest57 login/Guest/base34/finalACK and new measurement PASS; Display state qualification PASS; Native58 building.**
Main25287/server25286/RBF064e3784 unchanged. Idle three10s mean1,947,741.199
issues/s matches56b; no gain. Base34/33BCD/1,345,843bytes/finalACK314.016264..
314.224719s, issuebounds514,614,872..518,073,064, interior1.643766..1.643981M/s.
12replays(DFPackage1,AMModel11), nonzeroqueue/drop; CRC/RX/abort0. Observer56194/
25586 TERMINAL stopafterfinalACK/absent; neverrestart. All34servedsourcehashes
matchaftertiming. Same33sample recipe butmanualgatevaried; no causalreplay/
pinnedrepeat/speedpromotion. lowercase n acceptedactualNo; BootEssentials/fonts
advance; full57desktop/mouse/keyboard/warmmarker pending. post-loader evidence.
FirstcollectorOOM-killed26048whilematerializingretiredJSONrecords/hashpack;
Main/serverlive. Streamingvalidation+1MiBhashing retry32590TERMINAL validates
109,152retiredrecords/noerrors/loss and oldpack928d73a9. Correcteddriver+dmesg
retained; aftertimedloader/no guestpatch. Native57 LinuxOOM not a corefault claim.

ActualDispY complete13,993mutable/2,048PROMbits original/oracle/physical probes/
all524,288indices/destructive restore/recapture/firstedge/20Kmore PASS;8brokenRTL
reject. Freshnative-macro/default XML14badcontrols reject. Actualbackplane omits
boardSYSPER too: boardANDoscillator default16, fabric4; boardparameterunused under
pinnedsource. Earlierfixture4 andmacro-freeXML retained as earlier attempts;
acceptedfixturesuseactualdefaults/nativeF10415M10K. Readonlyaudit passes all4,
min+0.121ns;8corruptcontrolsreject. Enabledisolatedsnapshot nine256x4MLABs/
three1024x1M10Ks. Baselinef04prunes3unusedbits andpacks3wordplanesin1M10K;
snapshotadminretains4bits andrequires3M10Ks. Originalno_rw_check baseline
DONT_CARE becomes tested modeledNBA OLD_DATA.1159->2966ALM/1358->2434regs,
not enabledwholecorecost. Evidence snapshot-dispy-full. Earlier displayreview
README corrected to distinguish fixture4 from actualdefault16. NoRTLtimerfix.

Native58 build60721 LIVE /home/alans/mister/Dorado_Native58_20261019;
localnative58-dispy-inputs, frozen actual57source native57-dispy-baseline-src.
NewDispYboard/K1115Ahelper only;10sharedhelpers byte-identical. Same clocks/
SDCs/wrapper/policies/nativeMemDdelay2, all9adapters adminOFF. Need full180paths/
31summary/fourconditions/36board-corner audit and RAM/source proof before load,
plus finish57desktop/input/preservation. NoNative58audit yet; do not deploy or
restartbuild on timeout. Next remaining DskEth/BaseBoard/wrapper/backends/input/
ARMserver/diskgeneration/globalbundle/FPGArestore, then original60ns/timers/
final3freshboots/coldpersistence/pinnedperformance/2hstress. Goal remains active.

Latest57firmware progress: original newBFN113 observed, C released normally;
watcher13824 TERMINAL, remote25347 expected absent. Earlyissue1pause ended
normally withoutreset/retry. Actual845 nowvisible in capture. OriginalCedar
login/base34/finalACK/fullViewers/input/newmeasurement still pending. Measurement/
loaderobserver scripts prepared undercedar57-launch, notrun; observerPIDguard
now25286 anddoc reflectsactual observer-before33samples recipe. Neverrestart
oldwatcher/triplepushes. Evidence native57-field/firmware-progress preserves
terminalCwatchlog/capture/scripts separately from historicalstartup collection.

**LIVE Native57 normally deployed; original firmware startup active, Cedar pending.**
Load91010 TERMINAL14:07:38UTC, Main25287/server25286/temp25263normallyretired.
RBF064e37848a123cbc0affeeb07ff309d242aff8b8deca9c0992c78e8ee56bc268.
Same50MHz/SYSPER4/SDRAM/no rawcache/directDDR/policies/nativeMemDdelay2,
all8adapters/adminOFF. Untouched f3bce2a8seed verified beforefinalcache.
ActualMainSPI74[53460,1,0,8176,0,1,22,0,0,48683,5467,62856,55886,2048,8,16,0,0],
epochda4ef588155bbe2b/flags0x16/fault/errors0. Daemonalone64KiB1FF00000mapping,
Main no ring/probeDSO. Setup96152/triplepush37029/collector29366 TERMINAL.
C watcher13824/local,remote25347 LIVE awaitingnewBFN113; it holds C and must
release automatically on request. Do not restart watcher or triplepushes.
Initial issue1/state43pause remains at collectedstartup time; prior56b/55also
paused early. No reset/retry. OriginalBFN113/germ/login/loader/desktop/input/
newmeasurement NOT YET QUALIFIED. Last verifiedidle56b1.947742Missues/s,
no speedgain. Localcedar57-launch, evidence native57-field. AllourQuartusjobs
TERMINAL; Native57 full31/164/fourcondition/32crossings timing PASS above.

Retired56bactualMainbarriers+normaltemporaryexit preserve63,578,624bytes,
SHA928d73a9bbe0c9928cc9307af7a2b40f3eab14483b7ad40311a4460323cde50a.
Old23403/23400/temp25263absent; traceendsbarrier_end/no recordederror/loss.
NormalguestBoot812after Viewers/input/rtl56bpersistclose/readback proved;
no cleanOSshutdown/coldmarker/machine snapshot claim. Native57 freshseed is
separate. Next inspect same watcher/firmware progress, neverrestarton timeout;
qualify originalCedar/input/speed, then complete DispY (inventory/oscillator
primitive qualified), DskEth/BaseBoard/wrapper/backends/input/ARM/diskgeneration/
global atomic snapshot+actualrestore. Original60ns/timers/final3freshboots/
coldpersistence/pinnedperformance/2hstress remain open.

**Native57 full-core audit PASS; ready for normal load, not deployed yet.**
Build40422/audit50821 TERMINAL.31summary/164detailed/fourconditions/32board-corner
coverage,min+0.116ns;30,734ALM/52,945regs/332M10Ks/35DSP. All8adapters adminOFF.
All21MemXplanes retain1,376,256bits/168uniqueM10Ks/OLD_DATA in baseline+candidate,
all8actualPROMbytes match; inheritedMemD/IFUM/IM/MLAB geometry retained.
18corruptcontrols reject; source/clock/SDC/policy/nativeMemDdelay2 unchanged.
RBF064e37848a123cbc0affeeb07ff309d242aff8b8deca9c0992c78e8ee56bc268,
localnative57-candidate; exact evidence native57-memx. No speed/snapshotclaim.
Normal56bguestBoot command after warm marker now PASS: actual812 captured,
proof uploaded, Main23403/server23400 stillown oldcore. Hostbarrier/pack
preservation happens only during next normal57loader. No cleanOSshutdown/
coldmarker claim. Next commit/push qualification, normal57load/preserve56b,
actualMaincapability/newDDRgrant, Cwatcher before originalCoolBoot and fresh
Cedar/input/speed. Localcedar57-launch prepared; do not rerun deploy on timeout.

Latest follow-up: K1115A oscillator primitive complete-state simulation/freshXML/
matched isolated50MHz fits now PASS. Nine parameter modes,33bits each, signed
wrap/comparison boundaries, destructive restore/quietrecapture/firstedge/20Kmore;
8brokenRTL+2changedsource reject,8damagedschemas reject,5damagedfits reject.
All4conditions min+0.145ns,no unconstrained clocks/I/O or RAM; shell/admin counts
514->735ALM/333->762regs, not wholecore. Earlier fixture reserved-name/input-width
and classification constant/access-description corrections preserved; noRTLfix.
Evidence snapshot-oscillator. Actual frozen DispY_field review is archived in
snapshot-dispy-inventory:548sequentialentries/14,308bits beforediagnostic exclusion,
140FFpackages/nine256x4RAMs/three16x4files/three1024x1RAMs+privatepipelines/
33oscillatorbits/eightphasebits. Actual oscillator FREQ50000/SYSPER16default;
parentboardSYSPER4does notpropagate. Preservebinding during snapshot work; timing
calibration belongs in clockmilestone. FullDispY implementation is next.
Native57 fit/asm/STA/audit terminal stages; device-corners/crossings still active,
read-only audit50821 waits for builder1210961 to end. Do not start anotherSTA.
Normal56bBoot is being issued after warm marker readback; actual code/proof
and host-barrier preservation still pending. Live Main23403/server23400.

**Latest: Native56b full desktop/input/warm file PASS; complete modeled MemX qualification PASS; Native57 fitting.**
Same live Main23403/server23400/RBF0bbe092f,50MHz/SYSPER4/SDRAM/directDDR,
all snapshot admin inactive. Viewers/Ready/Clock/Watch/Edit, actual Cmd mouse
click, initialized second CommandTool/separate focus737,608, exact short and
alphabet/digit echoes and normal ///rtl56bpersist.txt create/close/type pass.
Read-only collection:95,817complete disk records/no recorded errors/loss.
No normal56bBoot/cold marker yet. Idle1.947742Missues/s; no speed gain.
Evidence native56b-field/desktop; localcedar56b-launch.

Complete modeled MemX now passes all1,378,312mutable bits/2,048immutablePROM
bits, independent21socket histories, destructive whole-state restore/recapture/
firstpendingwrite/20,000more.26,630,466host edges/26,419,185requests/all24-bit
indices; nine brokenRTL reject. FreshXML18damagedschemas reject. Matched50MHz
isolated fit42440 TERMINAL, all4conditions min+0.131ns;1,376,256RAMbits/
168M10Ks retained, all21OLD_DATA geometries/uniquephysical locations proven.
Ten corrupt-fit controls reject. Evidence snapshot-memx-full; raw text unchanged.

Native57 build40422 remains live: /home/alans/mister/Dorado_Native57_20261019,
currently quartus_fit. Frozen56b plus MemXboard/newMosRamhelper only;10shared
helpers reused byte-identical, clocks/wrapper/SDCs/policies/nativeMemDdelay2
unchanged, ALL admin inactive. Exact executing builder v1 preserved locally.
Its corner-script substitution kept Native56 report labels (crossingsNative57);
audit_native_memx.py requires exact unchanged scripts and all164categories/
31summary/32board-corner crossings. Future builder now fixes labels and adds
PROMs to the prebuild source checks; final audit explicitly checks all8actual
baseline/candidate PROM bytes regardless. No fit restart or timing mutation.
Read-only audit sources staged Native57_Audit_20261019; do not audit until all
builder stages terminal. Current fullcore timing/deployment NOT qualified.

Next TODOs: finish57 all-device audit/archive/check-in before normal reload;
normal56bguestBoot/captureactualcode/Main barriers/preservepack; fresh57original
firmware/Cedar/input measurement. Then DispY (actual DispY_field.v, not generic
board), DskEth, BaseBoard, wrapper/backends/input/ARMserver/diskgeneration and
atomic global bundle/actualFPGArestore. Original60ns/timermigration and final
3freshboots/coldpersistence/pinnedperformance/2hstress remain required.

Live56b visible login/Guest/base34/33BCD/1,345,843bytes/final guest ACK now PASS,
321.952726..322.161282s; issue bounds530,173,424..533,584,518/interior1.651306..
1.651517M/s,46Tioga replays. Idle1,947,741.690issues/s matches55, no gain.
Same33-sample recipe, earlier manual gate varied. Observer57449/23698 terminal/
absent after finalACK; all34source hashes verified after timing, not payload
hashes. Actual lowercase n accepted; BootEssentials active/full desktop pending.
Independent14/16MosRam complete-state restore/XML/matched50MHz fits now pass,
all81,964bits/firstpendingwrite/20,000cycles,min+0.166ns/tenM10Ks retained,
actual placement/OLD_DATA proved. Full MemX/global snapshots/60ns still open.


Live56b normal load12:59:00UTC, Main23403/server23400, actual fresh epoch
f0eca3ad955c5ee2/flags0x16/fault/errors0. Same50MHz/SYSPER4/policies/native2delay,
all seven adapters/admininactive. Original firmware/new BFN113/C release and
actual845 pass; no retry/reset after normal early pause. Login/new measurement/
base34finalACK/full desktop/input pending. Last qualified idle is55's1.947777M
issues/s, no new speed claim. Retired55full desktop/input/warm marker/normalBoot812
pass; actual Main barriers preserve55volume7dc98469. No cold55marker/fullsnapshot/
60ns/finalrelease claim. Localcedar56b-launch, evidence native56b-field.


Native56b full-core native2 MemD integration is now qualified:31summary/
148detailed/four-condition timing,min+0.116ns;all14cache physical RAMs OLD_DATA,
inherited IFUM/IM/MLAB geometry retained. Eight shared adapters byte-identical;
exact same clocks/SDCs/wrapper/storage/network policies, all admin inactive.
30,303ALM/52,996regs/332M10Ks/35DSP, not enabled snapshot cost. Build34403/
audit29951 terminal. Audit dependency/scoped-negative-control corrections did
not change RTL/constraints or rerun fitting. RBF0bbe092f downloadednative56b-
candidate, NOT DEPLOYED; no speed claim. Live55full desktop/input/warm marker
passes. Next normal55 Boot/preservation, normal56bload and original firmware/
Cedar/speed qualification. Original60ns/global snapshots/final gates open.


Live55 full Viewers/Ready/Clock/WatchTool/EditTool/mouse/second initialized
CommandTool/separate focus737,608/exact short/alphabet/digit echoes and normal
warm ///rtl55persist.txt create/close/type readback PASS.86,220complete live
disk records show no recorded error/trace loss. No normalBoot55/cold marker,
physical snapshot or new speed claim. Native56b remains building34403; all
source/clock/policy/admin settings unchanged except qualified native2 MemD.


Native2 MemD correction is now independently qualified:296,591mutable bits,
generated/nested-scope XML regression, original/oracle/physical-probe restore,
14,265actual input captures and firstedge/20,000cycles pass. Matched isolated
four-condition50MHz fits pass min+0.107ns, all14cache banks OLD_DATA,
294,912bits/38->40M10Ks. Fresh full-core56b build34403 is live; same actual
storage delay2, clocks/SDCs/policies, all admin inactive. Failed first56zero-delay
integration is preserved, not resumed. Live55fonts completed/STARTs advancing;
full desktop/input and full56b timing remain pending. No speed gain claim.


Correction after initial Native56 integration: actual Native55 QSF uses
DORADO_STORAGE_INPUT_DELAY=2. The initial zero-delay MemD adapter correctly
refused it, so no RBF/hardware change resulted. Earlier zero-delay isolated
results are valid only for zero delay; their "actual native" label was wrong.
Qualify all37input-pipeline bits plus generated-scope XML coverage at delay2
before a fresh full-core integration. Preserve the real delay; do not tie it
away to pass the build. The target is296,591 modeled MemD state bits.


19Oct continuation: live55 base34/33BCD/1,345,843bytes/final guest ACK pass,
339.016350..339.224798s. Whole-loader issues563,541,985..566,984,017,
interior1.666950..1.667154M/s,57protocol replays(Tioga22/Watch35); drop7,
CRC/RX/abort0. Observer21775/58164 terminal/absent after finalACK, no restart.
All34source hashes verified after timing, not payload hashes. No speed gain.
Profile timeout accepted Yes before initial No arrived; actual visible default
No accepted after normal filename/Return loop. BootEssentials advancing,
full55desktop/input pending. Current Main21577/server21576/RBF3eee3109 unchanged.
Complete native MemD modeled state296,554bits,14cache/eight small-file writers,
allstate/all20bitindices/destructive restore/firstedge/20,000cycles and freshXML
coverage now pass. Matched isolated four-condition50MHz timing min+0.164ns,
all14cache macros/physical RAMs OLD_DATA,294,912bits/38->40M10Ks. Only native
storage input delay0 supported; nonzero explicitly rejects. Full-core MemD
integration/global snapshots/original60ns remain open. Evidence snapshot-memd-full.
The following entries preserve earlier qualification stages.

Live55 normally loaded11:51:35UTC, Main21577/server21576, actual fresh DDR
epochd642d051fd61576c/flags0x16/fault/errors0. Same50MHz/SYSPER4/policies and
all six board adapters/admin inactive. Original firmware/BFN113/released C/
visible Cedar login pass. Three10s idle windows mean1,947,777.041issues/s,
matching54; no gain. Measurement/real Guest76634 TERMINAL; observer21775/58164
LIVE for loader. Same33-sample cadence, earlier manual gate varies.55final
ACK/desktop/input pending. Retired54 full desktop/mouse/exact keyboard/warm
marker and guestBoot845 pass; actual Main barriers preserve54pack(ed9d5241).
No cold marker/full snapshot/original60ns/final release gate claim.
Native55 full-core MemC timing/source audit PASS31summary/132detailed/four
conditions,min+0.040ns. Same50MHz/SYSPER4/SDCs/policies, all admin inactive;
candidate not deployed, no speed claim. Native54 actual Viewers/Ready/Watch/
Edit/Clock is visible; mouse/initialized separate CommandTool/exact keyboard/
warm marker qualification is in progress. Optional first profile tool installs
continue. MemD cache grouping and4096-word RAM primitive now pass review/
simulation/classification/matched timing(+0.168ns); full MemD/global snapshots
and original60ns clocks remain open. Evidence native55-memc/snapshot-memd-grouping/
snapshot-f10470-word. Current authoritative details are in the handoff.
Native54 now passes original firmware/login/base34/exact final guestACK:
332.810528..333.019019s,551.667..555.196M whole-loader issues/interior1.661338..
1.661545M/s,102protocol replays(Tioga46,CommandTool56). Idle1,947,774.216issues/s
matches53, no gain. More issued work/replay variation remains open; unchanged49
also retries. Same33-sample measurement->Guest cadence, earlier manual gate
varies. Observer20367/54671 TERMINAL/absent after actual finalACK; all34source
hashes verified after timing, not physical received hashes. No accepted;
BootEssentials advancing, full54desktop/input pending. Complete MemC isolated
state/restore/classification/matched timing now pass,min+0.152ns,21,504MLABbits/
672cells retained; rejected original-only input-hold shell preserved. Native55
full-core integration building99325, same clocks/policies/admininactive. No
snapshot4a/original60ns or final repeat/stress completion claim.
Native54 full-core IFU timing now PASS31summary/116detailed/four conditions,
min+0.114ns. Normal fresh-seed load11:00:58UTC, Main20155/server20154, fresh
actual DDR epochb95517266503bd85/flags0x16/fault/errors0. All state admin inactive;
unchanged50MHz/SYSPER4/wrapper/SDCs/policies. Original firmware qualification
in progress; no new instruction rate, speed gain or60ns claim. Retired53
normal guest Boot845 and actual Main barriers preserve its volume, SHA52bd3634;
no cold marker readback. Complete MemC model23,718bits/44writers/restore and
classification now pass; isolated fits51478 running, not FPGA qualification.

19Oct10:39UTC: Native53 full Viewers/mouse/Cmd/separate focus/exact short and
alphabet/digit echo and warm rtl53persist file create/close/readback PASS.
No new speed measurement or cold53 marker readback. Full IFU isolated simulation/
freshXML classification/restore/first resume now pass28,653modeled bits with
11independent RAM groups and original omitted CE; matched50MHz positive-edge
fits pass four conditions,min+0.157ns,27,648bits/four->elevenM10Ks. All11actual
snapshot RAM macros/physical rows OLD_DATA; nine bad fits reject. These shell/
admin resources are not enabled full-core costs. Native54 full-core integration
is building84906 on misterubuntu, same50MHz/SYSPER4/SDCs/all admin inactive.
Global snapshots, original60ns migration and final repeated/stress gates remain
required. Evidence native53-field/snapshot-f10415-word/snapshot-ifu-full.

19Oct09:54UTC: Native53 original firmware/Pilot/login and34base transfers/final
guestACK PASS311.447031..311.698212s. Idle1,947,681.730issues/s matches52 within
0.0055%; no speed gain. Whole-loader issues509.221..514.480M/interior1.642263..
1.642483M/s, zero protocol replays.52 had56/more issued work; unchanged49b
also had0/similar time. Any adapter effect/replay cause remains unproven. Same
33-sample measurement->Guest cadence; earlier manual gate varies, not final
pinned repeat benchmark. Observer18045/22501 TERMINAL/absent after actual final
ACK. All34 source hashes match after timing, not physical-payload hashes.
No accepted; actual BootEssentials advancing, full53Viewers/input pending.
New counter analyser reproduces49b/52 whole-loader results and rejects six
damaged evidence controls. IFUM review preserves11separate address groups
and omitted CE ports; not snapshot4a completion or original60ns migration.

19Oct09:38UTC: qualified53 normally loaded09:34:06UTC, Main17783/server17782,
all state admin inactive. Actual fresh DDR epoch9501221728fd84ec, flags0x16/
fault/errors0, originalf3seed untouched before final caching. Original firmware
new BFN113/release and visible Pilot germ812 pass; Cedar login/loader/input and
speed measurements pending. Retired52 preserved after real Main barriers,
63578624-byte pack SHAd12f56883a0cfe3e7c5e1f2c08207c812f79aa9a05b481dc70817240da6eed48.
No reset/retry of original boot; no whole snapshot or60ns claim.

19Oct09:30UTC: Native53 processor-adapter full-core timing PASS31summary/
100detailed checks/four conditions,min+0.115ns. All state admin inactive,
exact frozen52 clocks/wrapper/SDCs, no new cuts/unconstrained clocks; framework
I/O unchanged.12processor MLABs/eight OLD_DATA IM banks retained. Original
hardware boot pending; no speed or full snapshot claim. Live52 now passes
full desktop/mouse/Cmd/focus/exact keyboard/warm rtl52persist marker; normal
guest Boot captured812. MC10135 next-IFU
primitive simulation/XML coverage now passes all7bits/four PRECAP modes and
restore, with7 damaged RTL/3 source/8 schema controls. Matched negative-edge
launch/positive-edge capture50MHz fit passes min+0.164ns/all conditions/no cuts;
five bad-fit controls reject. Earlier positive-edge virtual-input hold failure
and unrepaired multi-corner repeat remain rejected/preserved. IFU integration
pending. These are milestone4a steps, not completion of4a or60ns migration.

Current hardware update19Oct:47/48/49 original Viewers/input and
active-DDR STOP/unmap/normal reload/fresh epochs pass. Clean47b idle1,595,628.966;
48 arbitration1,694,447.318 (+6.1931%);49 interleaving1,947,687.412 (+14.9453%
vs48,+22.0639% vs47b,11.6861% nominal). Each has three adjacent windows in one
boot. Fresh49b repeats exact49 RBF with new Main8321/server8318/original seed:
1,947,691.366 (+0.000203%). This is two49 boots, not three final release boots.
48 base34-transfer loader373.0718..373.3220s,49 first312.55875..312.87163s,
49b311.33993..311.59041s; exact final guest ACK and34 source hashes qualify.
Manual pre-Guest idle delays varied; not a final automated repeated benchmark.

50 deferred reads pass original boot/login/base transfers but demonstrate no
useful benefit: idle+0.01861%, loader311.63439..311.86355s, inside observed49
variation, for3323 additionalALMs/2832regs. Reject50 as baseline; retain optional
RTL/tests/evidence. Actual Main barriers retire50 normally and preserve its
physical disk. Fresh49b full Viewers/mouse/Cmd/focus, exact short/alphabet/digit
echo and warm marker pass; normal guest Boot reached germ855 and normal Main
barriers preserve its pack. Live51 ContB exact-advance adapters pass all-device
timing, original firmware/login/base transfers; snapshot admin inactive.
Idle1,947,710.746 issues/s matches49b (+0.000995%). Loader333.686..333.915s is
longer than49b311.340..311.570s under the same new analyser. Counters bracket
7.32..8.69% more issued work, slightly higher interior throughput and50MHz
host progress.128 replay events occur in Tioga/CommandTool/Watch versus49b0.
Cause remains OPEN; do not promote51 based on idle equality. Fixed-cadence
fresh repeats and packet-delivery investigation are next.51 missing-profile
default/No retry accepted; BootEssentials/fonts/STARTs and full Viewers Ready,
mouse/Cmd/focus/exact short/alphabet/digit echo and guest marker create/close/
readback pass. No cold51 marker readback yet; first profile tool continues
optional installs. This is one fresh51 desktop, not the final repeated gate.
Native51 is now normally retired/preserved after germ812. Retired51b exact-RBF
fresh repeat gives idle1,947,720.553 and loader333.175..333.426s, with matching
extra issued work and93 replays now in Imager/CommandTool. Its33-sample
measurement->Guest cadence is fixed; preceding manual login gate still varies.
Fresh49c same-recipe control below confirms retries also occur in selected49.
51b all34/final ACK, full Viewers/input and warm marker now pass; initial
combined open/focus input did not echo, explicit refocus/retry passed. Normal
guest Boot captured germ812. No cold51b marker readback.
Fresh selected49c same33-sample measurement->Guest recipe now passes
34transfers/final guest ACK323.149..323.399s,56 EditTool replays. Idle
1,947,761.574issues/s matches;532.378..535.804M whole-loader issues, interior
1.65299..1.65321M/s. Protocol replays are therefore not unique to51. Earlier
manual login gate still varies, and any51 effect remains unproven. All34
source hashes match after timing; observer14915 terminal/absent. Profile
No retry accepted; full49c desktop/input/warm marker now pass. Normal guest
Boot captured845 (MPCodes mesaRuntimeInitialized); retired/preserved for52.
Evidence native49c-control; no fully pinned repeated or physical-payload proof.
Same50MHz/SYSPER4 clocks, SDRAM/no raw
cache/direct DDR, arbitration/interleaving ON, deferred reads OFF. Physical
packet-byte proof, fixed-cadence repeated comparisons and sustained final gates
remain open. Snapshot4a and60ns clock migration remain required. Evidence:
native49b-repeat/native50-field/native51-contb/native51-field/native51b-repeat; older diaries
are historical. No speed gain or complete physical snapshot is claimed for51.

Live52 loaded08:34:08UTC, exactbe80e03f/full-core timing31/68/four conditions;
all state admin inactive. Main16101/server16100, fresh actual DDR epoch
70afd9602c702cec, originalf3seed untouched before caching. Original firmware/
BFN113/Pilot/Cedar login pass; fixed33-sample measurement->Guest mean
1,947,787.783issues/s matches49c. Timed loader/final ACK now PASS321.875..322.042s,
56AMTypes replays/530.135..533.505M issues; observer16342 terminal/absent. All34
source hashes match after timing. Full52Viewers/input/warm marker now pass; normal guest Boot captured812. No speed gain/full snapshot/60ns claim. Evidence
native52-field; retired49c preserved63578624B SHA7edf6ada62cb7f1c2658b0008773c6b561c0d54de96b7dce8c20ba7139a91e87.

Use three explicitly recorded workloads: idle Cedar login, a fixed cold
loader sequence through all 34 base packages, and a desktop script with
known CommandTool output, mouse actions and file transfer/write activity.
Pin the volume seed, served files, Main/server binaries, clock settings and
input sequence. Record wall time, instruction progress once validated,
pause categories, raw reads/writes, buffer hits, memory service latency,
disk service/flush duration and Main polling gaps. Repeat performance
comparisons three times and retain each result; do not compare unrelated
boot phases or independently changed implementations as one speed claim.

Expand coherent diagnostic snapshots when necessary: Native35's selectable
pairs are coherent individually, but its different views are different
windows. CPU retirement, memory-service and pause correlations need either
one common snapshot or a captured transaction trace. Attribute changes
separately: M10K placement, SDRAM, request sequencing, ARM disk, DDR packet
transport, then clock migration. A failed test or negative result is recorded
and addressed before that change is promoted.

### Completion and scope

The goal is complete only when the combined core passes milestone 6, the
SDRAM/disk/DDR transport and complete snapshots are operational, original phase/timer
relationships and nominal cadence are verified, and reproducible measurements
show the improvement over the frozen baseline. An unresolved clock or
correctness blocker remains open; it is not replaced by a PLL-only speed
claim. Original guest-visible CPU/cache/map/ECC behavior is preserved, with
no imported C machine state or host completion of guest instructions/IOCBs.

47 normal guest Boot/active-DDR STOP/unmap/full fresh reload and different
epoch grant pass.48 full Viewers/input and actual delayed-worker completed
input pass; its idle and loader measurements are preliminary as above.
Continue matched49 then50 memory comparisons and remaining DDR transport
qualification. Complete snapshot4a is required; isolated RAM access passes,
actual board access/classification is next. The clock migration remains
unfinished; no higher-PLL or full-speed claim.
Native41b's failed pack remains preserved;41d closes the separate clean
cold-remount persistence case. Only
MiSTer14 is allocated. Use misterubuntu for Quartus and the Mac for Verilator.
Do not overwrite the protected Executive simulation/checkpoint artifacts.
Keep commits and the handoff current at each milestone. No new permission
is needed for the already authorized builds, reloads and reversible tests.

**Update,2026-10-18 18:25UTC:** Native41 loaded all34 Basic loadees, answered
the profile question No and entered BootEssentials, then stalled at PrintTV
because the old ARM server mixed producer/replay cursors. Wire-only regression
reproduces delayed forward ACK after a rewind and the resulting byte-window
underflow; the bounded-history fix passes native and MiSTer ARM loss/sequence
wrap/ring wrap/context-reuse tests. Fixed daemon e237dffc is staged for the next
fresh boot. At18:22:13UTC, exact assertions allowed an ARM-only repair of the
old producer to its already ACKed cursor; PrintTV completed and later packages
continued. This boot is now explicitly assisted, not a whole-boot gate. No
guest CPU/memory/IOCB changes. Full evidence: reports/2026-10-18-bsp-history.

Native43 (commitc69d93f9) optionally selects a pending raw request at response
completion. Independent round-robin/payload/reset/stall tests and full MSA
state/output equivalence pass; the matched SDRAM board fixture needs6.406%
fewer host edges. All31 summary/36 all-device timing categories pass,
min+0.117ns, no new cuts. It is not deployed or FPGA-speed-qualified.
Retain41 component qualification and42 matched device control, then qualify43
with the fixed server and repeat pinned performance comparisons. Details and
hashes are in the handoff and native43/storage-prefetch reports.

At18:28UTC a fresh41b run replaced the old41 session after the old daemon hit
the same ACK race again. Same RBF a97af936/Main1fb2/seed f3bce2a8, fixed
ARM-tested daemon e237dffc, no guest-state import or cursor assistance.
Main13741/server13734; original firmware CoolBoot and C watcher active.
At18:33UTC firmware advances into SetRun/CPUfc1; BFN113/login still pending.
Preserve the original boot sequence and poll the existing watcher, rather
than restarting on an observation timeout. Exact current state is in the
handoff and reports/2026-10-18-native41b.
Update18:39UTC: actual BFN113 served18:34, C released; HDMI login18:35;
three idle windows average1595705.59 issues/s (9.574% nominal), reproducing41.
Real Guest accepted and Grapevine-down fallback completed; Basic.Loadees is
being served. All keys up, no input/GDB helper. Full desktop/input/persistence
and matched42/43 hardware comparisons remain pending.
Update18:49UTC: all34 loadees loaded, profile No accepted; fresh41b passes
PrintTV.df/PrintTVImpl without repair and loads later AMTypes. The nonblocking
worker-enqueue prerequisite also passes actual Linux/ARM queue/heartbeat/
drain tests (reports/2026-10-18-offload), with blocking/empty-wake controls.
It is not integrated disk I/O and does not finish milestone3; the live Main
is unchanged. Drain hooks must precede FPGA programming, remount, fork and
reboot, covering both coalescer buffers and queued file work.
Update19:01UTC: BootEssentials and TiogaFonts completed;41b is attaching
PressFonts/FontMetrics with no cursor repair or hardware faults. DDR42 and
optimized43 candidates are verified/staged in MiSTer RAM but not loaded.
Offline full-MSA raw trace with independent final-memory oracle suggests
group/word interleaving improves locality: modeled128-line read fetches
2992->1500, uncached row hits0->5447 of10880 operations. This is synthetic
locality evidence, not real Cedar traffic, a latency model or FPGA speedup;
no mapping/cache/burst RTL change is implemented. Preserve42/43 comparison
order, then test a separate optional mapping experiment if real traffic
supports it. Source/trace/negative controls: reports/2026-10-18-storage-locality.
Update19:32UTC:41b has passed full fresh Viewers, real mouse/Cmd/focus, exact
short/alphabet/digit echo, and guest local marker create/close/readback.
Cold remount is pending; optional startup commands are still finishing.
Native44's default-OFF backing group/word permutation passes independent
packing/order/reset/array/full-MSA/physical-state checks; matched SDRAM fixture
144461->122779 host cycles with identical logical behavior. All31 summary/36
all-device checks pass min+0.099ns, no new cuts or clock-coverage gaps. It is
not deployed or speed-qualified.42/43 comparisons and real Cedar measurements
remain required before promotion. Current artifacts/state are in the handoff.

Update19:48UTC: **Native41b post-desktop stress FAILED.** Latched fault
802a020a/47fc/000018ff is a4s host watchdog awaiting WRITE ACK assertion at
LBA6399. Main/server remain live; CRC/RX/abort zero. Exact blocking syscall
was not observed. Prior long O_SYNC writes are a suspect, not a proven cause
for this event. The physical pack is preserved unchanged with matching SHA256
0046452c45d333b30020a47546e5fdfa65b4ecb9edd46d14cbb9ba048bf02b7b;
the copy excludes dirty FPGA track RAM. No reset, fault clear or host
completion. Earlier desktop/input/marker close/read passed, but the typed
boot was not visibly accepted and cold remount never completed. Prioritize
milestone3 actual file-worker/coherence/lifecycle integration with monotonic
request/ACK/file/poll tracing and injected multi-second storage stalls;
then repeat fresh persistence/stability gates before promoting candidates.

Update20:12UTC: milestone3's cache/adapter is implemented and passes actual
worker/file tests on Linux and MiSTer14 ARM. One <=64MiB image preloads into
ARM DDR before mount notification; Main alone ACKs into reserved coherent
RAM, and one worker snapshot coalesces dirty sectors into <=64KiB file writes.
Error data is retained and normal remount/program/reexec/reboot/termination
requires successful drain+fsync. Tests inject4.2s writes and verify concurrent
SPI-spy read/write/poll progress, complete physical close/remount oracles,
queue saturation/capacity/error/generation/partial IO and exact patched Main
SIGINT/SIGTERM lifecycle behavior. ARM regular test-loop max gaps2.782ms
cache and4.977ms adapter; explicit lifecycle drain gaps are separate.
This is not whole-running-Main or FPGA qualification. Full pinned GCC10.2.1
Main build40e14b6b passes, not deployed. Next fresh same-RBF41c must use the
new monotonic request/ACK/file/poll observer, then real input/network,
cold-remount marker persistence and sustained Cedar stress. Reports disk-cache;
Native41b remains halted and preserved. Complete snapshots remain milestone4a.

Update20:17UTC: disk-cache Main40e1 is deployed on fresh same-RBF Native41c;
Main16875/server16872. Actual image cache ready; C watcher70748 armed after
final Main initialization, three real CoolBoot pulses completed. Original
firmware spin-up, BFN113/login and all subsequent gates pending. Match new
ELF symbols for Main controls. Current config/recipe/captures are native41c
reports; failed41b physical image remains unchanged and preserved.

Update20:25UTC: Native41c original firmware/BFN113 and actual HDMI Cedar
login PASS. C watcher70748 completed/released, boot modifiers UP. Idle
three10s windows1595653.86/1595625.89/1595901.82 issues/s, mean1595727.19
(9.574% original nominal), essentially identical to41b. This is one boot,
issues not drained retirements. Real Guest typing requested. Full Viewers,
input, durable persistence and stress pending. No guest-state repair.

Update20:53UTC:41c passes all34 loadees/profile No and BootEssentials,
now attaching FontMetrics, with no pack/CRC/RX/abort fault. Actual worker
trace has thousands of writes, no file errors or trace drops; maximum30.516ms
in the20:30 prefix, not a controlled real-Main multi-second stall test.
Three~377ms poll gaps occurred during known GDB video/counter controls;
other observed gaps in that prefix are below100ms. Keep controlled worker
unit/SPI-spy results separate from entire-Main qualification.

Optional deferred internal DRAM reads (Native45) pass original-cell nonzero
pipeline/old-before-write/reset tests at14/16-bit addresses, complete arrays,
cached DDR/MSA and physical XSDS equivalence. All7872 per-owner write payloads
and final1499 nonzero words match baseline. SDRAM synthetic host cycles
122779->115685, not a hardware speed claim. Default generated12 files remain
identical, opted wrappers lint. Native45 build completes at79%ALMs/330M10Ks;
All31 summary/36 path categories across four conditions PASS min+0.017ns,
including SDRAM IO,0 illegal/unconstrained internal clocks. No new timing
cuts/clocks/placement policies, not deployed;14 remains41c.

Update21:13UTC:41c reached full fresh Viewers/Clock/Watch/EditTool at21:03.
Real mouse/Cmd/focus and exact short/alphabet/digit echo pass. Local marker
///rtl41cpersist.txt created/closed; type reads rtl41cpersist20261018 exactly
and returns to prompt at21:08. Cold remount is pending; preserve this volume.
Optional first-tool imports continue. Actual disk trace and current input/
screenshots are archived with native41c; no current pack/CRC/RX/abort fault.
At21:16 Main stdout/stderr moved to /tmp/dorado41c-main.log to keep
diagnostic logging off FAT. Old log prefix retained; guest unchanged, fourth
known GDB control pause. Future samplers use the new path.
Complete snapshots, controlled whole-Main multi-second file stall, sustained
and three-fresh-boot qualification remain open. Main40e1/source and Native45
optional memory/timing evidence are checked in and pushed.

Update21:35UTC: actual Main4200ms delayed flush work passes continued disk
service/instruction progress:3 read/21 write ACKs during the window, no poll
gap above20ms, errors or trace loss. A bounded qualification DSO queues work
on Main's actual worker, delays then fsyncs an owned mounted-image descriptor;
this is not observed kernel latency. No FPGA/guest state or SPI ownership is
changed. Echo42 completes afterward; no guest packet traffic occurs inside
the window. Packet/input completion under delay remains open. Actual Main
durability barrier and marker `worker42` readback pass; cold remount does not
yet. Normal guest Boot has entered the germ812 screen. Evidence lives in
reports/2026-10-18-main-slow-flush. Keep this gate's limits in qualification.

**Implementation update,2026-10-18 17:33UTC:** Native41 SDRAM has cold-booted
original firmware to the actual Cedar login on MiSTer14. Real Guest input
is accepted; Basic boot is running. Main11453 is the
unchanged1fb2 binary; daemon11445 is89007902 (quiet-after-Cedar plus BSP
probes). Fresh seed f3bce2a8, pinned Pup time1792324800,50MHz/SYSPER4,
no raw line cache. All31 summary checks/36 all-device path categories,
including SDRAM I/O, pass min+0.114ns. Full Viewers/input/persistence remain
unqualified. Three10s idle-login windows measure1.595677M issues/s,9.574%
of nominal original throughput and10.20% above39b. Separate windows show
25.5765% enabled/74.4241% storage-only waits,0 pack/overlap,~3.036M physical
reads/s,0 backing hits, held0.1825%, clocks/issue2.00364. READ->return interval
is~4 fabric clocks versus DDR~9.94 (a separate-window estimate). This is one
boot and includes the cache/placement differences below; use42 to isolate
the device and repeat complete pinned comparisons before a final speed claim.
HDMI required Main's known framebuffer enable/disable workaround; the recurring
reinitialization defect remains open. Full captures/measurements are native41.

Native39b's original-firmware Cedar login provides the actual DDR baseline:
three10s windows give1,447,940/1,448,181/1,447,903 issues/s, mean~1.448M/s
=8.69% of nominal16.667M/s. Held starts are~0.177%, clocks/issue~2.00367.
Separate windows show23.2067% enabled and76.8169% storage-only pauses;
pack-only/overlap0,~2.754M physical reads/s, backing read hits~0.039%.
These views are not one atomic all-counter observation. Issue is not drained
retirement. This is three windows in one boot, not three fresh-boot runs.
Evidence: reports/2026-10-18-native39/idle-login* and per-view records.

The new d4 daemon had omitted the installed1d5 variant's BFN113 broadcast
suppression. Repeated gatewayInfo kept Cedar's documented drain-until-quiet
routing loop open at fileInitialized860. Restoring quiet mode on the ARM
service alone produced Time206/address223 requests and the login screen in
~30s, without FPGA/Main reset or guest memory/IOCB writes. The service repair
restored its already-observed BFN113 metadata; that boot is not a whole-boot
time benchmark. The corrected89007902 daemon passes native/ARM quiet3.2s,
quiet-reset, boot/routing/STP and BSP dropped-tail recovery after an actual
Cedar113 request. The old flood binary fails the new regression. See
reports/2026-10-18-quiet-bsp. Native41 must now pass a fresh unassisted boot.

Native42 is a matched cache-off DDR control: identical41 clocks, M10K/logic
placement and generated CPU/memory boards, only raw backend wiring and its
constraints differ. All29 summary checks/20 all-device categories pass,
min+0.115ns; no added exceptions or internal-clock coverage gaps. Not loaded.
RBF45af2e30...,29788 ALMs/330 M10Ks. Use42 before attributing an isolated
SDRAM device speedup:39 still has128 read-cache lines and different tiny-file
placement. Selectable SDRAM passes independent worst-case pins/frequencies,
refresh/reset and full MSA equivalence.37/38's failed timing and40/41's fixes
remain recorded; no failed build was deployed.

Native36 remains the assisted Viewers/mouse/exact short/alphabet/digit Echo
reference. A local marker file was created/closed/read back; cold remount is
pending. Its lost20-byte Helvetica7 completion tail motivated the automatic
BSP probe fix. Native34 remains the last unassisted desktop/input reference.
The independently checked issue monitor passes15 real-board microprogram
cases; hardware measurements now agree with the clock/cycle partition.
Remaining tasks include maintenance/reset/task-switch qualification,
Native41 cold Cedar/input/persistence, paired measurements, request overhead,
ARM disk work, DDR transport, complete snapshots and original clock phases.

Snapshot scope includes IM/IFUM, register/task files, processor/control
pipeline and clock phase, main memory, cache/map/ECC and their registers,
BaseBoard CPU/RAM, serial disk position/controller state, display state,
Ethernet buffers/wire state, input queues and any outstanding host work.
Generate a versioned state schema from the same cells/wire lists rather
than relying on FPGA register readback or dumping only main RAM. First
support matching builds; cross-build restore needs an explicit, tested
compatibility rule or migration. Record ROM/microcode/configuration hashes.
At capture, establish a barrier across guest execution and host services:
drain or serialize outstanding transactions, flush/capture dirty disk data,
and snapshot local Pup/BSP/STP transfer state and timers. SDRAM refresh must
continue during pause/save/load. A live external peer cannot be rewound;
define tunnel disconnection/recovery explicitly instead of claiming it was
captured. Keep cold-boot qualification alongside fast-restore benchmarks.

Before expanding state access across the machine, inventory every mutable
cell/board register and memory, including generated delay/pipeline registers.
Synthesize one representative board with the proposed access path and check
area and timing: Native41 already uses71% of ALMs, so the state-access fabric
must have a measured resource budget. Use hierarchical, bounded state access
rather than exposing a giant parallel state mux. A manifest must account for
every state element as saved, drained, or provably reconstructible. Restore
must validate the entire bundle and disk generation before changing live
state, then load with guest clocks frozen and resume all modeled devices
from one captured phase. The user-facing operation is Save/Load machine state;
first qualify identical-build Cedar desktop restores and measure the result
before publishing a restore-time promise.

The inventory must include raw-storage frontend state. A pending raw write
may already have changed external RAM before its logical chip commit, with
old_data preserving the prewrite pipeline value. A snapshot must serialize
that state or explicitly normalize its exported RAM/pending write, rather
than silently invalidating caches. Deferred pipeline addresses/known-value
flags matter too. First qualify identical-build restore. Any later canonical
chip/word schema or raw-layout migration needs its own equivalence tests. Existing tools/sil_verilator_state.py
and sil_fpga_checkpoint.py provide exact-build RTL checkpoint/instance/XML
mapping; reuse that inventory rather than its host ABI offsets. Baking a
simulation checkpoint into power-up initializers is not live FPGA save/load.
The representative prototype must retain synchronous RAM inference and its
read pipeline, including old-before-write values. A restore port that simply
conditions the mandatory BRAM read can turn those arrays into logic. Check
state loading/read-clock control and phase/history restoration against the
original cell, then measure area and routed timing. Drain/close physical SDRAM
transactions while refresh continues; classify backend open-row/refresh
bookkeeping explicitly as reconstructible instead of replaying stale physical
RAM-command state. These are design requirements, not implemented features.

Native45's candidate raises ALM use to79%; evaluate state-access area/timing
on a representative board before committing to machine-wide instrumentation.

Milestone4a has begun with `tools/sil_state_inventory.py`: unflattened XML
per-instance destinations, bit/array geometry and source provenance, without
host ABI offsets. A freshly elaborated Native45-style SDRAM fixture covers
4158 native instances; all candidates remain UNCLASSIFIED. It includes RAM
read pipelines, deferred old_data/known/address state, helper-task writes and
readmem ROM loads. Real-elaboration/original-F10415 coverage and failure-control
tests pass. External raw SDRAM, Dorado.sv queues/clock/video state, Main and
server are outside this first inventory. This is not complete coverage or
online save/load. Evidence: reports/2026-10-18-snapshot-inventory/. Next extend
scope/classification and qualify bounded access on one representative board
before machine-wide hardware instrumentation.

Snapshot RAM prototype update00:36UTC19Oct: isolated1024x34 F10415 bank
export/import passes against34 unchanged original cells and an independent
array oracle. Every RAM/read-pipeline bit, all1023 invalid indices,
destructive continuation, first-edge resume and30000 random running steps
pass; four broken RTL variants fail. Matched50MHz virtual-I/O Quartus fits
retain34816 bits/fourM10Ks:101->163 ALMs (+62),0->43 registers. All12
setup/hold/pulse checks per variant pass across four conditions, snapshot
minimum+0.096ns, no unconstrained paths. All34 inferred mixed-port collision
modes are OLD_DATA. The five damaged-report/source controls are rejected.
Separate-address writes lost RAM inference; per-bit restore enables cost34
M10Ks, so the qualified interface transfers complete34-bit words through a
single write port. These results apply only to the isolated bank. Actual
board grouping/strobes/history, machine-wide freeze, external RAM/device/ARM
state, atomic bundles and physical Cedar restore remain unimplemented.
Source: fpga/mister/snapshot; evidence: reports/2026-10-19-snapshot-ram.

Actual ContB grouping now follows the original wire list and generated
combinational fanout:144 F10415A slices,34 data+2 stored parity bits per
physical word,147456 RAM bits and144 read-pipeline bits. Literal pin names
produce124 tiny groups; proven expressions reduce these to eight18-bit
groups, two independently written halves per bank. Independent Boolean truth
checks and six damaged-board controls pass. Common inversion of all address
copies is outside this grouping proof. No board ports/RAM/clock change or
new fit is claimed; next qualify actual18-bit strobes/access and preserve
read outputs during ordinary backend holds. Evidence: snapshot-contb reports.

The18-bit enabled snapshot bank now passes original-cell/array tests including
20225 ordinary backend-held edges and recapture before the first resumed guest
edge. A single host clock with the actual guest advance retains read/write
cadence and OLD_DATA; five bad variants fail. Registered launch/capture-shell
fits retain twoM10Ks/18432bits, all12 checks per variant/four conditions pass,
min+0.255ns; six damaged-report controls reject. Counts53->102ALMs/48->138regs
include fixture/admin registers and cannot be projected to the whole core.
Two-clock RAM templates failed inference; direct-virtual-input baseline hold
failures are preserved. No cuts; final fixture keeps2ns/0ns I/O and10ns internal
paths. Actual ContB/global state/barriers/physical snapshots remain unimplemented.
Evidence: reports/2026-10-19-snapshot-enable.

Actual ContB IM access now passes in a separate generated board, retaining
all other original logic.97,418 host cycles/57,392 state operations compare
all71 board outputs and144 raw pipeline bits with the original board plus
an independent array oracle. Actual half-bank writes, parity/address/CE,
falling-edge-latched clock enables, backend holds, invalid/cross-bank access
and IM export/invert/restore/resume pass; four damaged RTL controls fail.
Matched clock-gated board fits retain147456bits/16M10Ks and pass all12 checks
per variant/four conditions, min+0.168ns. No cuts or unconstrained paths;
287->600ALMs/325->584regs include the registered shell/admin pruning delta.
Eight damaged-evidence controls reject. Evidence: snapshot-contb-board reports.
This is not full-board restore:99 other sequential candidates/209bits in22
MC10176/MC10231 packages still need state ports, with remaining inventory
categories to classify. Next qualify actual full-board continue/restore,
then whole-core barriers/state/timing. No production RTL/FPGA snapshot added.

The complete modeled ContB extension now exports all209 remaining q/setup/
clock-history bits.132764 host cycles/82735 state accesses pass original-board
comparison, all209 FF bit walks and all invalid15-bit addresses. Candidate-only
continuation under real board logic, full147809-bit state restore and20000
subsequent execution cycles match the stopped/untouched original from the
first actual guest edge. Six damaged RTL variants fail. Fresh XML classifies
all original board candidates as saved or continuous-wire recomputation;
seven missing/width/parity/duplicate/new/overlap/alias controls reject. External
inputs and clock phase remain outside this board boundary.

Matched full-state board fits retain147456 RAM bits/16M10Ks and eight18-bit
OLD_DATA macros. All12 checks per variant/four conditions pass min+0.165ns;
no timing cuts or unconstrained paths. Shell counts288->774ALMs/320->693regs
include admin/pruning overhead and cannot project whole-core timing/resources.
Nine damaged-evidence controls reject. Reports: snapshot-contb-full. This
qualifies the representative modeled board, not machine restore or a global
pause. Next integrate the access fabric into a complete-machine experiment,
classify other boards/wrapper/devices/input and external SDRAM/Main/server
state, implement coherent barriers/bundles and qualify whole-core timing.
Complete physical Cedar save/load,60ns clocks and final release gates remain
required. Production RTL/QSF and the live49b FPGA are unchanged.

Native51 complete-core ContB host-clock/exact-advance integration now passes
31 summary/52 detailed timing reports/four conditions, minimum+0.115ns.
Includes16 additional bidirectional ContB/other-board setup/hold reports from
sequential audit1060 after build62531 completed. Original36 device reports,
unchanged constraints, no new cuts/illegal/unconstrained clocks; existing
framework I/O coverage counts unchanged, not zero. All8 instruction RAMs keep
1024x18 OLD_DATA.30640ALMs/52921regs/330M10Ks/35DSP. All state admin inactive:
not physical save/load or its future enabled-access resource budget. Original
50MHz/SYSPER4; hardware/speed gates pending. RBF3d253187, native51-candidate,
remote Native51_v3_20261019. Next normal guest Boot/preserve49b and normal
barrier/fresh-seed original-firmware51 software gate. Live49b unchanged so far.

Exact frozen49 native hierarchy inventory now replaces the earlier45-style
illustration:4158 instances/8644 sequential candidates/2107636bits. Qualified
ContB maps all147809 bits within the actual hierarchy; other scopes remain
UNCLASSIFIED. Non-deferred frontend removes pipe_addr/pipe_known only;
old_data/pending/cache/read/write state remain. Outer wrapper/devices/input/
clock/transport, external16MiB raw SDRAM and ARM Main/server still need scope/
classification/access/barriers. Evidence snapshot-native49-inventory and
native51-contb. Full snapshots,60ns migration and final gates remain required.

Native52 extends51 with the qualified ContA exact-advance adapters. Full-core
build19107 and collector21557 are terminal;31 summary/68 detailed/four-condition
checks pass, minimum+0.112ns, unchanged cuts/clock/I/O coverage, eight actual
IM macros1024x18 OLD_DATA.30553ALMs/52682regs/330M10Ks/35DSP. Exact new10230
warning reviewed with pinned0..8 range proof and failure controls; no compiled
source/constraints changed. All snapshot admin inactive; no physical save/load
or enabled-access budget. RBFbe80e03f in native52-candidate; report native52-controls.
Live52 now passes original firmware/Pilot/Cedar login after49c same-cadence
control; timed loader/desktop/input and speed qualification remain open.49c
also replays, so51 effects remain unproven. No speed promotion yet.

The next snapshot primitive families MB7071H256x4/F10414256x1 now pass all1280
modeled state bits, original-cell/array continue/restore/first-edge tests,
14 faulty RTL variants and fresh XML classification/12 damaged schemas.
Freeze borrows the async read address; all consumers need an external hold.
Single shared write port retains1280physicalMLABbits/40cells/zeroM10Ks.
Matched isolated pair212->227ALMs/47->64regs, four-condition min+0.354ns,
no cuts/unconstrained paths; six damaged-fit controls reject. First two-write
version expanded to1598ALMs/1371regs and is rejected/preserved. Report
snapshot-async256; no board/global/physical save/load or speed gain claimed.
Next full processor board state qualification and remaining machine scope.

### DDR control integration and matched idle evidence (2026-10-18)

Native42's matched raw-DDR idle-login windows average1,447,771 StartCycle
issues/s (8.6866% of nominal original cadence). SDRAM41c averages1,595,727:
10.2196% faster in ONE matched boot comparison. Adjacent windows are not
independent boots. Separate cause windows show76.7900% storage pause,
23.2095% enabled and zero disk pause; do not treat them as one atomic sample.
All34 loadees/fonts and original-firmware Viewers desktop complete on42;
mouse/Cmd/focus and exact short/alphabet/digit echo with new prompts pass.
Repeated paired workloads and final cold persistence/stress remain open. Exact inputs/limits: native42-field reports.

The optional DDR wrapper/ownership mux, SPI74/75 capability/atomic grants,
daemon control+reservation-checked mapper and Main nonblocking manager are
implemented. Actual-service/guarded-memory and actual patched Main lifecycle
prologues pass on Linux/production-runtime ARM. Late ACKs and physical drain
refuse programming/reexec/reboot/init; EOF reconnect ordering was caught and
fixed. Default and ungranted-DDR real-server regressions pass. No physical
DDR mapped/deployed yet, and no full wire/Cedar/delayed-Main claim.
Native46 (frozen41 plus only network integration) passes31 summary checks,
36 detailed categories/four conditions, min+0.112ns,73% ALMs/330M10Ks,
zero illegal/unconstrained clocks and no added cuts. Pinned Main21e67a3f
links; live42 still uses40e1/e237. Static ARM test-thread cleanup failed;
dynamic production-runtime tests pass. See DDR control/native46 reports.
Snapshot4a remains required and incomplete; these transport STOP boundaries
alone do not capture complete guest, wire, disk or server session state.

### Native46 startup refusal and Native47 correction (2026-10-18)

Combined production wrapper/UIO/DDR/wire+C tests now pass with independent
serial payload/CRC, captured real Dorado packet, full2048-byte traffic,
pauses/mid-frame phase, ownership handoff, STOP and restart. Broken mux and
ungated-wire controls fail. No full CPU/DskEth board in this fixture.

On actual46, Main21e6 observed disabled/epoch0/fault0 but not-stopped/owned
(flags0x10), and correctly refused exit. A real daemon STOP ACK still left
that indication false. No RESUME or daemon /dev/mem mapping occurred.
42 retired after normal guest Boot/loader barriers, physical pack0c5ab6f5.
46 seed remainedf3bce2a8. Both daemons exited normally, staging Main's disk
barrier returned true, and whole-board reboot returned14 to stock MENU.
This recovery does not qualify the hot-reload handshake.

Native47 decodes stopped from the existing drained STOPPED FSM state rather
than a redundant initialized-high flag. Simulation traffic/stop traces stay
identical; early-stop controls fail. Actual Main regression also refuses the
exact initial disabled/not-stopped observation. Isolated47 fitter/timing and
hardware startup/grant PASS (Main1509/server1506, epochfdfe0a77fc6c87b5).
All31/36/four-condition timing checks pass, min+0.111ns; actual daemon maps
exactly1FF00000/64KiB while Main does not. Original firmware boot underway;
Cedar login and real BFN113 pass. Idle login averages1,595,641 qualified
issues/s (9.57385% of nominal original cadence), one boot/three adjacent
windows, not a matched network/backend or repeated performance gate. Guest
requested after measurement; loader/full Cedar/active STOP-reload/stress
still pending. No
CPU/storage/clock/constraint changes. Do not claim a
proved synthesis cause or physical DDR success before hardware evidence.

### Native47 actual delayed worker plus physical DDR (2026-10-19 UTC)

During4200ms of actual Main worker flush work,10 read/100 write requests
are acknowledged, with no overlapping poll gap>20ms or trace errors/drops.
A real Watch.bcd STP metadata response168B plus EOC is acknowledged at its
exact endpoint by the guest inside the same interval; six complete guest
packets are conservatively bounded there. Inner Main snapshots advance
3,904,318 issues over150,001,118 host edges, with no CRC/RX/abort/pack fault
and stable TX drop count15. The probe delays then real-fsyncs the mounted
image FD; this is not a4.2s kernel-call claim. No command/mouse was injected
in this loader window. Keep DSO resident to process exit; fresh Main required
for later clean performance comparisons. Missing metadata/ACK failure
controls do not qualify. Evidence: native47-worker-ddr reports.
All34 loadees/profile No pass; BootEssentials/font installation and full
Viewers/input/active lease reload/stress remain pending. Snapshots and full
clock/timer migration are still required, not supplied by this test.

## Objective and constraints

Restore the original throughput and clock relationships: HM section 2.3
defines a 30 ns clock, a new ordinary instruction every two clocks (60 ns),
and three overlapping execution cycles. IM/TPC instructions extend the
control sequence; those extra clocks are architectural behavior.

The original FPGA baseline ceiling is 4.167 M instructions/s at 33.333 MHz/SYSPER4,
before backend pauses. Simply reducing SYSPER is invalid: the existing
generator distorts the original phases at 4 and collapses them at 2; the
paired original-ROM startup passes at 4 and fails at 2. Simply increasing
the PLL is also unqualified: deployed Native26's Fmax is 41.08 MHz, limited
by storage-ready logic into the falling-edge network_step register.

Two changes are needed: remove avoidable discrete delays in the sampled
chip/clock translation, and reduce storage stalls/control-path delay.
Keep these experiments separate so each result has an attributable cause.
Preserve ECC, read-before-write behavior, real I/O completion and hardware
error reporting. No blanket timing cuts or guest-state repairs.

## Work sequence and acceptance gates

| Stage | Work | Acceptance before proceeding |
| --- | --- | --- |
| 0. Baseline | Preserve Native26, original-ROM4/2 comparison, clock-phase audit, and deployed-fit critical paths. | Saved recipes/results identify the working control and failing faster setting. Complete. |
| 1. Phase contract | Derive clock families from original schematic connections. Build an independent divider reference and a direct-enable prototype. | SCP'/ECP levels, edge order, 1/8-period lead, 3/8-period j02 low window, pre-edge pipeline capture, and scheduler freeze/resume pass. First prototype complete; board qualifications remain. |
| 2. First board cone | Extract ProcL's SHCP'C/T-enable cone and its ProcH counterpart. Translate captures into explicit enables, including their setup data; keep the old implementation selectable in an isolated harness. | Both halves of T agree for LC/Md/hold/task transitions; original SendMIR/Boot0/compute and original-ROM startup pass. Do not copy PRECAP/clock-lag exceptions without re-deriving their ordering. |
| 3. Remaining families | Translate task/Link/IM/TPC control, then Map/cache/IFU and latch/write windows. Retain original extended instruction and RunRefresh behavior. | Architectural-boundary state comparisons, six hardware error checks, storage equivalence, cold Initial/Cedar and disk read/write gates pass. |
| 4. Backing storage | Count pause causes, retain useful lanes from each64-bit DDR return, and shorten the address/readiness-to-pause path with a proved handshake. | Read-before-write/ECC and real disk traffic stay correct; measured stalls decrease; STA checks the actual pause/release relationships. |
| 5. Timers and implementation | Reconcile MC12061, BaseBoard, display, Ethernet and disk rates with the new timebase. Choose a realizable PLL/enable architecture and fit it. | All setup/hold/recovery/removal and clock checks pass; original firmware boots; measured retired instructions and paused time support the speed claim. |
| 6. Hardware | Load a separately named qualified core on MiSTer14 after preserving the current session/artifacts. | Cold Cedar desktop, exclusive keyboard/mouse test, sustained network/disk traffic, stable video and measured throughput. |

Stage2's ProcL/ProcH T controls pass in isolation and source wiring is
checked. Explicit edge-enable integration and the full T data/latch path
remain. The separate incremental storage work now supports a50MHz fit:
Native34 passes original firmware, all34 Cedar base packages, BootEssentials/
fonts, Viewers, real mouse/click/focus and exact short/alphabet/digits Echo.
All four device timing conditions pass; no pack/CRC/RX fault through nearly
one hour of host observation. This is an incremental speed step, not the
full-speed clock redesign. The original timeout's specific cause remains
unproved, although its clock-budget defect is fixed and it did not recur.

Global pauses still dominate: interactive desktop with startup Bringovers
sampled19.8755% enabled at50.0134MHz, implying a ceiling1.2422M ordinary
microinstructions/s(~7.45% original). This is not retired throughput, an
idle desktop benchmark or a controlled A/B speedup. Next measure exact
DDR/HPS wait durations and reduce the dominant storage traffic. General
builder defaults remain33.333MHz/cacheOFF pending a controlled comparison.
The existing Executive executable/checkpoint are protected; use separate
object directories and never `+frame` against the golden artifact.

## T-control extraction: implemented, isolated

The original sheets have different capture nets: ProcH h22 uses Clock0'Da;
ProcL d17 uses SHCP'C. Their RbSelMd/RbSelMd' output pins are also swapped.
`tools/check_t_control_wiring.py` checks24 pin bindings per board, package
types, the tied LC/FF075 drivers and write-enable connections directly from
the original Xerox wire lists.

`verilog/rtl/dorado_t_controls.sv` accepts a separate explicit capture enable
for each half and samples its LC/FF/task-comparison setup inputs directly.
`make -C verilog t-controls-test` compares it with an independently clocked
pin-level original gate cone over all8 LC values,256 FF values and4 capture
masks:8,192 comparisons. It separately checks8,164 legal combinations
against HM3.8/Table10, preserves both banks through2,048 no-capture cases,
and rejects late input pass-through. A universal-enable mutation disagrees
in4,096 cases. No reset has been added to the original resetless controls.
Strict module lint passes.

This proves the decode/capture interface **given correct enables**. The test
does not derive those enables from BaseBoard/processor clock gates, model
all T data latches/task files, or qualify firmware with this replacement.
The module remains outside the machine and FPGA source list. Stage2 is
therefore incomplete, and its SendMIR/Boot0/compute/ROM gates are pending.

## Incremental hardware candidate: Native29

Frozen build: `misterubuntu:/home/alans/mister/Dorado_Native29_20261018`.
It clones Native28 and changes only the PLL output from33.333333 to40MHz;
CPU/board/DRAM logic and SYSPER4 are retained. Native28's host key queue
and passive fault diagnostics are included. Ceiling5.0M instructions/s,
before storage pauses, versus4.167M on Native26.

The new placement passes29 checks, minimum+0.019ns, core setup+1.499ns,
zero illegal/unconstrained clocks. Exact MemD h15 and MemC j07 own F10145A
write-address bit1 holds were reviewed (each1source/4sinks), preserving
setup/data/enables/reads. Fmax42.55MHz. Original firmware/BFN113 and Cedar
login work on MiSTer14; fast Guest input is accepted, real Basic.Loadees
retrieval underway. Viewers and full interaction remain pending. The fitted
RBF is `a963f81c353b240a5528bdffb1b1d6c43df3dd0e8aa37afed38e142292337acf`.

`DORADO_SYS_MHZ=40 sh fpga/mister/build.sh ...` now selects that PLL
frequency; the default remains33.333333. The assembler records the chosen
value in `dorado_clock_config.json`. Shell syntax and a temporary template
fixture pass for both frequencies; invalid/nonfinite/zero frequencies are
rejected before modifying output. Fresh timing and hardware checks remain
mandatory for any selected frequency.

## Passive throughput measurements: Native30 candidate

Native30 retains Native29's40MHz/SYSPER4 and adds passive enabled/host fabric
edge counters plus coherent command70 debug snapshots. It is a separate
frozen build at `misterubuntu:/home/alans/mister/Dorado_Native30_20261018`;
Native29's build inputs remain unchanged.

`make -C verilog speed-monitor-test` compares enabled counts with an actual
gated clock through pauses/reset/wrap. UIO tests at512 and1024 words verify
that changing both debug values/status during the reply cannot tear the
observation; the pre-change implementation fails this regression.

These counters will separate global backend pauses from CPU clock division.
They count fabric edges, not instructions. See the
[protocol/measurement notes](../fpga/mister/main/README.md) for units and
wrap handling. A read-only Native26 desktop scaler-header baseline changes
1.70 times/s over10s in one fixed scaler-buffer header. Buffer reuse may
skip complete fields: this is only that slot's header-change cadence, not
a source-field, CPU or HDMI frequency. Do not infer retired MIPS from it.

## Measured arbitration bottleneck: Native31 at50MHz

Native29's new limiting path takes22.666ns from an MSA address, through
readiness and the128-way arbitration/payload selection, into address_latched
(25ns relationship,1.499ns slack). The old scan rotates a variable-index
request lookup, selects an owner, then performs a second large payload mux.

The replacement in `dorado_storage_arb.sv` uses a balanced tree carrying
the selected owner/address/data/write together. At each node, requests at
or above next_group take precedence; within each class the lower index
wins. This preserves the exact cyclic order and the original outstanding-
request FSM, including reset, read-before-write and response backpressure.

Independent cyclic-scan scoreboard: all masks/pointers at1/3/7 groups, plus
all128 pointers with random masks (8,371 transactions). Payload stability
through request/response stalls passes. Three-group array/reset and complete
MSA14/16-bit geometry comparisons pass, including all2,359,296/9,437,184
stored bits and outputs. Quartus17 required explicit generate/genvar syntax;
the corrected source synthesizes. No speed gain is inferred from RTL alone.

Frozen Native31 at `misterubuntu:/home/alans/mister/Dorado_Native31_20261018`
combines this tree with Native30 diagnostics at50MHz. Fit succeeds in15m05s,
84% ALMs,305 RAM blocks. All29 STA checks pass, min+0.080ns, core setup
+1.210ns,0 illegal/unconstrained clocks. Both fast/slow hold reports pass;
no new post-fit exceptions. Worst path is now MSA readiness -> network_step,
7.673ns data against10ns; Fmax56.88MHz is not an operating qualification.
The original EccComputeErr inferred-latch warning matches Native29.

Loaded on MiSTer14 around03:48UTC; original firmware/BFN113, Cedar login
and fast Guest input pass. Actual HDMI was restored via Main host-FB toggle;
fresh counters confirm1080p30/74.25003MHz. Viewers/input qualification pending. A20s firmware count sample gives
26.4617% enabled edges and50.01MHz host clock. This halted-CPU workload is
not Cedar performance. Ceiling6.25M/s before pauses,37.5% of original Dorado;
50% above33.333MHz is a ceiling comparison, not measured guest acceleration.
Evidence: `fpga/mister/reports/2026-10-18-native31/`.
The original clock-family migration and unused DDR return lanes remain.

### Original-board physical budget

A separate slow-corner destination audit now measures processor, memory and
I/O cones independently of backend arbitration. Worst processor path is
ContB b24.qb (ASEL0) -> ProcL k22.d_pre[5] (DMuxData),17.182ns over21 logic
levels, with+2.527ns margin against20ns. Memory/I/O copies of that DMuxData
capture take17.172/16.772ns. These continuously sampled setup copies are
part of the existing chip translation; they are not new original-hardware
pipeline stages. Merely moving the readiness gate would not qualify a
66.667MHz fit. The enable/latch migration must cover diagnostic/data-setup
sampling as well as the main T/task path. Do not cut these paths without a
proved replacement contract. Reports: native31/board-paths.tcl and reports.

## Backing storage: optional line buffer started

Native31's real Cedar loader sample gives16.9255% enabled fabric edges;
83.0745% are global pauses (50.0034MHz host). These are not instructions.
This justifies using all four16-bit lanes from a raw64-bit DDR read before
attempting another PLL increase.

`dorado_storage_ddram.sv` now has an optional128-line buffer (default OFF).
Full tags prevent false hits; writes invalidate and still wait for DDR;
reset invalidates and drains outstanding responses. All original DRAM/
ECC/cache behavior stays upstream. Independent scoreboard and full board
comparisons pass, including all9,437,184 bits. Three-group DDR reads drop
4449 ->509 with identical1505 chip cycles/463writes. Cache-disabled and
missing-invalidation failure controls prove the traffic/coherence tests.

A separately frozen Native32 candidate enables the buffer at50MHz/SYSPER4.
Fit14m23 and29 summary checks plus full global fast/slow timing pass,
overall min+0.084ns; no new exceptions/unconstrained clocks. Loaded on14
around04:19UTC. Original firmware is loading; first enabled-edge sample
25.0156% versus26.4617% uncached shows no firmware gain. Cedar loader/desktop
benefit and full input qualification remain pending. Default cache stays OFF.
Counters currently combine pause causes; separate DDR/HPS duration is not
measured. Native31 control completed the loader and advanced BootEssentials. Evidence/reproduction:
`fpga/mister/reports/2026-10-18-ddr-linebuffer/`. No hardware speedup claim yet.

### Remove cold-miss overhead: Native33 experiment

Native32's buffer adds a lookup cycle on a miss, a possible contributor to
its firmware regression. The independent Native33 candidate issues misses
during that lookup cycle after synchronous tags settle; hits remain wholly
off the DDR bus. Busy and zero-latency returns, writes, reset/orphan drain,
and every original-board bit pass. A new latency contract fails on the old
Native32 adapter. The full16-bit fixture's simulated host time drops235us
->213us, same12012 chip cycles/2992reads/7872writes. FPGA benefit unmeasured.
Native33 is building at50MHz with only this adapter delta; timing/hardware
qualification pending. Native32 continues its original firmware/Cedar run.

## Host wall-time watchdog: Native34 correction

Native32 stopped at27th retrieve with reason8, cylinder124/head0/LBA18893.
It had passed firmware/login/Guest and timing; it is not hardware-qualified.
The host watchdog remained133333332cycles while PLL increased33.333 ->50MHz:
its4s budget became2.6667s. Host service is wall time; SYSPER/guest time does
not apply. The actual failing Main latency/ACK phase was not captured, so
the scaling defect is proved but its role in this particular fault is not.

pack_hps now uses4*HOST_CLOCK_HZ; assembly derives that macro from the same
validated PLL setting and records matching Hz/4s metadata. Passive host
fault details retain the ACK phase/SD flags. Scaled-frequency regressions
accept3s service and fault missing/stuck ACK at4s with sticky phase/LBA;
the fixed-count failure control faults prematurely at500Hz. Existing CRC/
persistence/neighbor protection and assembly fixtures pass. No completion
or ACK is fabricated. Native34 fit13m52 passes29 summary checks and20 timing categories across
ALL four available device conditions (fast/slow,-40/100C), min+0.101ns,
zero missing clocks and no new cuts. Actual elaboration50000000Hz/200000000
cycles matches4s. Loaded on14 at05:09UTC; original firmware/BFN113/login
and fast Guest input pass. All34 packages, BootEssentials/fonts, Viewers
and mouse/exact Echo subsequently passed. Saved3435 heartbeats through
06:07UTC show maxgap1.1344s, no late probes/pack/CRC/RX fault. The historical
Native32 host latency/ACK cause remains unproved; no general defaults promoted.

Read-only tools/mister_watch_main.py timestamps Main heartbeats and samples
late-I/O /proc state into RAM, without SPI/ptrace/guest mutation. It was
validated on the preserved live fault and remained active through Native34
loading and interactive tests. Preserve first-fault data if a later timeout
occurs. Native34 now supplies the interactive baseline for further work.

## Next measurement from the qualified Native34 baseline

### Memory placement and host stalls (2026-10-18)

The user confirms **128 MB external SDRAM on every target MiSTer**; this
can be a required configuration. Currently its pins are undriven in
`fpga/mister/Dorado.sv`. The native configuration provides **8 MiB of
Dorado main RAM (4M 16-bit words)**, four storage modules/eight MSA boards
using 64Kbit chips. Its raw backing occupies16MiB in DDR at0x30000000:
8MiB data,1MiB ECC and padding between16-bit chip groups. ARM Linux does
not serve these RAM accesses. The original memory controllers, map, cache,
ECC and serial memory bus are upstream RTL.

Native34's actual fitter RAM Summary gives307/553 physical M10Ks and
34840/41910 ALMs. Logical RAM rows sometimes share M10K locations: summing
their block counts gives387 and is WRONG for physical usage. Evidence:
`fpga/mister/reports/2026-10-18-memory-placement/`.

| Storage | Present placement | Recommendation |
| --- | --- | --- |
| 8MiB main RAM /16MiB raw backing | Shared DDR | Dedicated128MB SDRAM backend candidate |
| Map (168KiB logical bits) |168 M10Ks | Keep on chip; preserve ports/timing |
| Cache data/parity (36KiB allocated) |38 physical M10Ks | Keep on chip; allocation alone does not establish active guest cache capacity |
| IM including parity (18KiB) |4608 MLAB cells | Try M10K placement; reads are already registered |
| IFU decoding (3.375KiB) |864 MLAB cells | Same placement experiment |
| Small register/task/flag arrays | MLAB | Keep near logic; respect asynchronous ports |
| Raw DDR read buffer (1KiB payload plus tags) |2 physical M10Ks | Size/layout from real traffic measurements |
| Disk track buffer (16KiB) |16 M10Ks | Retain; examine dirty-sector write amplification |
| Disk image and file-server tree | ARM filesystem | Add coherent write coalescing on ARM |

Native36 is a separate **placement-only experiment**, enabling
`DORADO_F10415_M10K`; Native35 retains historical MLAB placement. The
F10415A read/write always blocks are identical, with no added RTL read
latency. Besides IM and IFU, this moves three1Kbit DispY arrays. The fitter
packs the migrated arrays into16 ContB,8 IFU and1 DispY physical M10Ks.
Both pass29 summary timing checks and20 categories across all four
device conditions, with no illegal/unconstrained internal clocks or new
exceptions (35 minimum+0.088ns;36 minimum+0.068ns). Native35 uses35736 ALMs/
307M10Ks;36 uses27644 ALMs/332M10Ks. Native35 was loaded on MiSTer14 at
~14:27UTC for original firmware/pause measurements;36 is not hardware-tested.
Neither is yet a fully qualified successor. The historical default remains
unchanged.

The completed Native34 passive observer now establishes actual long ARM
disk-write stalls. Its largest Main heartbeat gap is4.730966s. Samples
from06:22:35.873 through06:22:38.673 show ARM write syscall4 to fd25,
the active pack, with the same7680-byte buffer/length/file position.
That is at least2.80s of sampled pending write, not proof that one syscall
lasted the whole heartbeat gap. The descriptor is O_SYNC. There were no
pack faults. This supersedes conclusions drawn only from the06:08 prefix;
the particular Native32 failure remains untraced. Evidence is
`late-arm-writes.json` and `native34-full-observer.jsonl.gz`.

The deployed Main (SHA1fb2e098...) uses blocking generic O_SYNC disk writes;
its pinned build predates the newer Mac write coalescer. The local Mac
reference (`Main_MiSTer/support/mac/mac_disk.cpp`) holds up to eight64KiB
runs per disk, coalesces adjacent/contained writes, flushes overlapping reads
and drains at mount/idle/age boundaries. It is restricted to Mac SCSI cores,
so Dorado does not benefit. Its flush still blocks Main. A Dorado adaptation
must preserve read coherence, neighboring bytes, remount/core-load draining
and write-error handling; batching alone cannot guarantee responsive Main
while flushing. The core's existing track buffer may flush about16KiB for
one changed guest sector. Measure and reduce that amplification separately.

Main also transports network packets. The ARM-local `dorado-server` serves
Pup boot, Mayday/EFTP, BSP/STP, directory and time from staged files, through
`/tmp/dorado-net.sock`. Main alone owns SPI; RTL packet buffers/wire adapter
connect to the original DskEth controller. The server runs no guest CPU or
guest RAM. Main's blocking disk write therefore delays packet delivery too.
The custom Main is selected by Dorado's per-core INI; stock Main is untouched.

The user requested comparison with other cores' DDR Ethernet transports.
Local source review confirms MacPPC7300 (`MacPPC7300_enet.sv` plus Main's
`support/mac/mac_eth.cpp`): eight TX/eight RX slots, PPCE magic/epoch,
producer/consumer counts and a64-bit DDR master at0x30400000. Sun's
`eth_hps.sv`/`support/sun/sun_enet.cpp` uses0x1FF00000, SSETH002 magic and
generation, eight TX/sixteen RX slots, explicit publication barriers and a
DDR arbiter; NeXT uses the same window with four slots each way. Main's
`shmem_map` maps `/dev/mem` with O_SYNC/MAP_SHARED. These handlers are still
polled from Main; moving bytes through DDR alone does not avoid Main's
blocking disk writes. Sources/hashes/findings are recorded in
`reports/2026-10-18-memory-placement/ethernet-ddr-references.json`.

Use that frame-ring pattern for a future **direct daemon-to-DDR** transport,
with Main retaining lifecycle/OSD/input, capability/generation checking,
single-owner32-bit indices, payload-before-publish barriers, explicit
endianness, bounded/full/wrap/reset rules, and the DDR engine running even
while guest clocks pause. Keep DskEth, real CRC and wire pacing in RTL and
preserve the existing server's raw-frame boundary. Do not copy modern NIC
register emulation, FCS injection, filters or10Mb/s pacing into Dorado.
Main must quiesce the daemon and receive acknowledgement before changing
cores, then resume it only after the new core's capability/generation is
validated. DDR magic can survive FPGA reload, so a stale magic word alone
cannot authorize continued writes into another core's memory.
Keep the current SPI/socket path selectable until equivalence/cold-boot
qualification. No DDR network implementation is deployed yet.

Milestone4 implementation has begun: `dorado_ethernet_ddr.sv` and the C
`server/dorado_ddr_ring` endpoint pass cross-language tests against independent
DDR/registered-wire-buffer oracles. Full2048-byte frames, both full rings,
zero/count-wrap, random stalls/read latency, mid-frame and pending-read/
stalled-write stops, stale epochs and malformed indices pass; byte-order/
epoch/premature-stop failure controls are detected. C also passes on actual
ARM with full-system ordering/stop barriers. Proposed64KiB window0x1FF00000
is outside MiSTer14's live System RAM and reviewed framework/raw-storage
regions. No /dev/mem access, integration, fit or hardware speed claim yet.
Acknowledged Main/daemon lifecycle and fresh SPI capability/grant checks are
the next required implementation step before activation. Protocol/evidence:
fpga/mister/dorado-ddr-protocol.md and reports/2026-10-18-ddr-network-endpoints/.

Two reuse details need explicit handling: Mac's0x30400000 overlaps Dorado's
current0x30000000..0x30ffffff raw backing; and a2048-byte slot with an inline
header cannot hold our full2048-byte payload. Reserve a non-overlapping
window and use separate descriptors or larger slots. Sun's0x1FF00000 is a
precedent, not yet an allocated Dorado window.

External connectivity is a separate layer. The original hardware manual's
Ethernet chapter specifies a3Mb/s link with8-bit stations and16-bit packet
checksum. [IFS](https://github.com/livingcomputermuseum/IFS#readme) documents
UDP/raw-modern-Ethernet encapsulation for experimental Ethernet and gateway
connections to other Xerox networks. An optional compatible UDP tunnel in
the daemon can connect vintage peers; Cedar STP interoperability still
needs testing. Current Pup requests are not directly understood by ordinary
Internet TCP/IP services; that requires suitable guest software or an
explicit service gateway. Keep local boot/file services as the default.

SDRAM implementation sequence:

1. Use Native35's passive counters to separate global DDR/pack pauses,
   raw request counts, cache hits, DDR issue-busy and read-return time.
   Compare within one selected view and a pinned workload; each pair is
   coherent, different views are different observation windows.
2. Implement a selectable raw-storage SDRAM backend with the existing
   request/response contract, retaining the known DDR backend for comparison.
   Reserve16MiB initially, preserve group/ECC layout, initialization,
   read-before-write and reset/drain rules. Drive the physical module with
   correctly constrained clock/IO and controller initialization/refresh.
3. Run independent storage/ECC/reset/stall equivalence, all-device STA and
   original-ROM/Cedar/input checks. Compare fresh seed, PLL, server, workload
   and input. Dedicated SDRAM removes shared-DDR contention; it does not by
   itself remove the current one-operation/128-requester serialization.
4. From measured traffic, consider grouped transfers, pipelining or a larger
   backing buffer. Keep original guest-visible cache/map/ECC behavior. Avoid
   allocating the spare SDRAM to an RTL disk cache before fixing the ARM
   write path and measuring misses; there is already a core track cache.

Native35 diagnostics retain command70 and the existing Main binary. Its
Debug monitor views show (`machine=`, `cpu=`): enabled/host; DDR-only/host;
pack-only/host; overlap/host; DDR reads/writes; cache hits/raw reads;
DDR busy/return-pending cycles; ARM read assert/release; ARM write
assert/release; maximum DDR pause/host; maximum ARM read/write transfer;
raw reads/writes; raw completions/host; init/reset pause; pack read/write
completions. Faults retain priority. Counters are modulo2^32, use intervals
under60s at50MHz. Categories enabled/reset/pack-only/DDR-only/overlap
partition host edges, with gate inputs captured on the actual falling edge.
Independent wrap/overlap/edge tests pass; a wrong-edge mutation fails.

Native35 original firmware/BFN113/Cedar login pass. At idle login, separate
approximately10s windows show23.2104% enabled,76.7903% DDR-only pauses, zero
pack-only/overlap pauses, and0.03810% backing read-cache hits. Commands run
at~2.75M reads/s (96 writes in that window). The busy/return view records
zero issue-busy cycles and273759018 read-return-pending cycles (~54.74% of
50MHz host time). Comparing the separate steady windows estimates~10 cycles
per read return; this is not a single coherent combined measurement. The
remaining pause share includes frontend/handshake sequencing. The enabled
instruction ceiling is1.45065M/s, not measured retirement or an A/B speedup.
Evidence/scripts are in `reports/2026-10-18-native35/`. This prioritizes
read-return latency, request grouping/sequencing and the SDRAM experiment;
it does not show command-issue contention at the exposed DDR port during
that idle interval. Internal memory scheduling can still affect returns.

The Native34 loader sample (`reports/2026-10-18-native34/loader-counts.json`)
observed49.9948MHz host edges and17.4388% enabled edges over20.31s while
loading packages. This is a different interval from earlier samples, so it
is not an A/B speedup or a retired-instruction rate. The large pause share
makes cause attribution the next useful measurement before another PLL change.
A later20s font-attachment sample gives50.0029MHz and19.8033% enabled,
with an instruction ceiling1.2377M/s (~7.43% of original16.667M/s).
This remains an edge-derived upper bound, not measured retirement. The
old quarter-speed description counted only nominal clock division; global
backing-store pauses make actual elapsed speed substantially lower.
Coarse1Hz status observations since05:29UTC (through05:54:35) found DDR not
ready in1165/1532 samples, pack not ready in42, enabled in325, no overlap.
These are instantaneous observations subject to sampling/aliasing, not
cycle-duration measurements; they point toward the DDR adapter first.
Do not benchmark from the herald's guest clock: MC12061 calibration remains
open, independently of the correct physical fabric-edge measurement.

Add passive host-domain counters for the actual falling-edge gate decision:
reset/operator pause, pack-not-ready, DDR-not-ready and enabled. Define a
priority for simultaneous causes and retain overlap separately; category
sums must equal host edges over one coherent snapshot. Latch gate inputs
with network_step so the accounting reflects actual machine edges rather
than a later combinational readiness value. Separate pack request count/
service duration and raw DDR read/write/hit/miss counts explain the pauses.
Do not change the gate, ACK timing, original memory/ECC or clock families.

Keep the existing Main command70 layout compatible. Additional diagnostics
need an explicit selectable view and coherent snapshot, with fault priority
preserved. Test category accounting through overlap, stopped machine clocks,
reset and counter wrap; prove diagnostics leave functional outputs unchanged.
Then fit/review all four device conditions before deployment. Compare a
pinned workload against Native34 at the same frequency, volume seed, server
and input sequence. Select the next storage change from these measurements.
The original phase/enable migration remains required for full-speed timing.

A passive trace of the synthetic64K MSA board fixture reproduces2992 actual
DDR read fills from3008 raw reads and7872 writes (12012 chip cycles, all
9,437,184 stored bits/output behavior identical). Offline cache replay
matches that exact fill count. Hypothetically packing four neighboring
storage groups into a64-bit word reduces read fills to1500 for that trace;
a row/column transpose gives3008, and retaining current write-hit cache
lines gives2992. Writes remain7872 in all cases, so this does not establish
a whole-system benefit. These are analysis-only layout alternatives, with
no changed RTL, volume mapping or deployed build. Replay and captured requests:
`fpga/mister/reports/2026-10-18-ddr-linebuffer/locality/`. Measure the real workload
and write traffic before choosing a layout/coalescing implementation.

## First prototype: implemented, isolated

[`dorado_clock_phases.sv`](../verilog/rtl/dorado_clock_phases.sv) describes
eight positions of the original BaseBd11 divider. It generates SCP', ECP
and their BaseBd12 j02 OR, plus combinational enables announcing the
transition at the upcoming master edge. A consuming register samples its
pre-edge D directly, instead of detecting a clock transition one master
tick later and compensating with d_pre. Reset selects an arbitrary rotation
of the steady-state divider; it does not emulate analog power-up.

The source is not included in the native core, generated boards or Quartus
project. Its level outputs are diagnostic logic, not routed FPGA clocks.
`advance` freezes the experimental scheduler; it is not a replacement for
the original processor Hold or the independently running memory clocks.
Debug-clock pulses and board-specific gates are not implemented here.

The regression uses an independent event-driven model of the four original
flip-flops: g04/h04 on one VCO edge, g03/h03 on the opposite edge. It checks
every level/edge at one, two and three master ticks per eighth-period slot,
the pulse widths and edge counts, freezing at every phase, and a two-stage
pipeline's simultaneous capture of old upstream data. An actual MC10176
from the current cell library is the failure control: it captures a tick
late relative to the reference. This demonstrates the mechanism; it does
not establish that changing one cell will fix the whole machine.

Reproduce from the repository root:

```
make -C verilog clock-phases-test
make -C verilog clock-phase-audit
verilator --lint-only -Wall --top-module dorado_clock_phases verilog/rtl/dorado_clock_phases.sv
```

**This eight-slot prototype is a timing oracle, not the final speedup.**
Literal implementation at original speed would require a 266.667 MHz
master clock, unsupported by the current fit. The subsequent cone work
must establish which transitions can share a fabric edge using explicit
pre-edge state and which require ordered or related PLL phases. Neither
66.67 MHz/SYSPER2 nor a fixed number of final phases is promised yet.

### Processor clock-source wiring checked

`tools/check_t_control_wiring.py` now follows the source paths as well as
T's capture pins in the ORIGINAL wire lists. It checks package types,
33 connected pins and15 unwired gate/reset inputs across the two boards.
With the documented pulldown of unwired MECL inputs:

- ProcH l01 -> g13 -> i18 gives
  `Clock0'Da = CLK.ph' | CLKEnable'c | StartCycle'a`.
- ProcL l02 samples `StartCycle'a` on the positive edge of
  `CLK.pl' | CLKEnable'b`; g12 NOR and c18 OR give `SHCP'C = ~l02.Q`.
  The original l02 capture has no reset on this half.

This makes the remaining enable work concrete: the high capture follows
an OR-gated clock edge, whereas the low capture follows the enable
flip-flop's falling Q. Changes in StartCycle or clock-enable can themselves
create edges; deriving both from one nominal phase would miss this. The
full ContA StartCycle/Run/extension ordering and same-edge setup still need
an event-driven reference before integration. No guessed shared enable,
new reset or production-clock change is introduced by this audit.

`make -C verilog t-controls-test` passes the expanded original-wiring check
and existing8192 comparisons/8164 legal manual checks/2048 holds; its
universal-enable failure control still detects4096 disagreements.

## Clock-family contract register

This table distinguishes established relationships from integration work
that still needs to be proved. Generated identifiers are navigation aids;
original sheets/manual remain the references.

| Family | Established source/relationship | Remaining translation work |
| --- | --- | --- |
| BaseBoard phases | BaseBd11 g04/h04/g03/h03; SCP'=g04.Q', ECP=h03.Q. ECP leads SCP' by 1/8 clock. BaseBd12 j02 OR, with debug pulse inactive, is low for 3/8. | Add debug pulses and fanout qualification; no analog-delay claims. |
| Run and memory enable | ContA l02–l06 generate processor/memory enables and RunRefresh. HM5.12 requires memory clocks during processor halt when RunRefresh is set. | Preserve asynchronous set/reset, step and stop behavior. Do not use global scheduler advance as processor Hold. |
| Instruction phases | ContA l07: Phase0/Phase1 depend on StartCycle/sPhase0 and clk2'Bf; HM2.3 defines ordinary/extended timing. | Preserve feedback and IM/TPC extension; do not toggle t0/t1 unconditionally. |
| T and task setup | ProcL20 d17 MC10176 clocks on SHCP'C; ProcH20 h22 on Clock0'Da. Both produce TbSelMd/write enables from LC/FF and task comparison. Processor Curr/Next stages also use SHCP'C/Clock1'. | Control decode/capture interface passes8192 cases. Clock-source paths now checked against original wire lists; edge-enable integration and complete T setup/data/task ordering remain. |
| Map latch/MCR | MemX f01/g01 MC10175 follow Mapbuf while MapbufLd' is low; f04 F10016 consumes Mapbuf at Clk1'Ab. | Preserve transparency before close and the consuming edge. Current clock lag/PRECAP values are compensation, not the physical specification. |
| RAM writes | F10145A captures address/data in the active write window; existing native fit has narrowly reviewed own-address hold exceptions. | Keep old-data reads, write commitment and setup checks; re-review every new placement. |
| Peripheral clocks | K1115A rates depend on SYSPER; MC12061's fabric/8 currently mis-scales the BaseBoard clock. | Qualify prepared timer correction against original firmware and measure terminal/Ethernet/disk/Junk rates together. |

Evidence for this first implementation lives in
`fpga/mister/reports/2026-10-18-clock-phases/`. Board/firmware equivalence,
FPGA timing closure and hardware speedup are still pending.
