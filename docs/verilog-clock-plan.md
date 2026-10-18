# Dorado RTL clock implementation plan

Started 2026-10-18 UTC. Native34 is now the50MHz verified Cedar desktop/
input baseline; Native26 preserves the earlier33.333MHz reference. The user has released MiSTer14 for reloads/faster builds; keeping
that session alive is no longer a constraint. See the [clock audit](verilog-from-sil.md#clock-architecture-audit-2026-10-18)
for measurements and the [handoff](verilog-handoff.md) for deployed artifacts.

## Active delivery plan and goal (2026-10-18)

Deliver a faster, correct Dorado on MiSTer that cold-boots the original
firmware to Cedar's Viewers desktop and an interactive CommandTool. Use the
confirmed 128 MB SDRAM module for bulk main memory, keep hot architectural
state on chip, and use reserved DDR queues for direct ARM-daemon packet
transport. Remove blocking disk work from Main's polling loop. Finish the
clock/enable and timer migration to target the original 60 ns ordinary
microinstruction cadence. Report measured instruction progress and waits;
a higher PLL frequency or an enabled-edge ceiling is not proof of speed.

This is an active implementation goal. The numbered milestones below are
the delivery order and take precedence over historical candidate diaries.
Native34 is the fully qualified desktop/input reference; Native35 provides
the earlier diagnostic baseline. Selectable SDRAM is implemented and
passes simulation and all-device timing; its hardware qualification and DDR
network transport remain pending. External IFS/ContrAlto tunnelling is
optional follow-up,
not a prerequisite for the performance goal.

### Milestones and acceptance

| Milestone | Change | Evidence required to finish |
| --- | --- | --- |
| 0. Establish the smaller working baseline | Qualify Native36's opt-in M10K placement on MiSTer14. Freeze recipes, original seed, Main/server binaries and benchmark inputs. Define an instruction-progress counter from the original control-board strobes. | Original firmware, Cedar desktop, exact keyboard echo, mouse/click and disk persistence pass; all four timing conditions pass. Independent known microprograms check the counter, including extended instructions and holds. Until then retain the explicit edge-ceiling label. |
| 1. Move bulk RAM to SDRAM | Add a selectable SDRAM raw-storage backend, initially retaining the same 16 MiB padded data/ECC layout for 8 MiB guest RAM. Derive controller geometry, initialization, refresh and IO timing from an appropriate MiSTer 128 MB controller/module reference. Keep the DDR backend selectable. | Independent storage/ECC/read-before-write/reset/stall fixtures agree. Constrain the new SDRAM clock and IO, pass all-device timing, then cold-boot Cedar and verify input and persisted disk writes. Compare with the same-frequency DDR control. |
| 2. Reduce memory request overhead | Use measured traffic to improve grouping, locality and the single-outstanding frontend. Investigate unnecessary raw fetches on address changes; change them only after proving the original DRAM output pipeline remains correct. | Fewer raw operations and/or shorter waits on pinned workloads, with storage equivalence and original firmware preserved. Demonstrate a repeatable end-to-end improvement; increasing buffer capacity alone is not an acceptance criterion. |
| 3. Keep disk I/O responsive | Adapt the Mac write-coalescing approach and move blocking file reads/writes/flushes to a bounded ARM worker. Main remains the sole SPI owner and handles explicit completions. Separately reduce whole-track write amplification where guest sector/CRC semantics permit. | Read-after-write, neighboring bytes, eviction, remount/core-load draining and write errors pass. An injected multi-second file operation does not block Main's poll/input service; target under 100 ms poll gaps in that controlled test. Real Cedar disk writes persist after close/remount. |
| 4. Connect the daemon through DDR | Adapt the Mac/Sun frame-ring pattern for direct `dorado-server` access. Reserve a non-overlapping region, retain the full 2048-byte payload capacity, define ownership/order/generation, and add an acknowledged stop/resume handshake around core loads. Keep SPI/socket transport selectable during qualification. | Random DDR stalls, full/wrap, malformed lengths, reset/reload and stale-generation tests pass. Original DskEth/CRC/wire timing stay in RTL. Boot-file and STP payloads match, Cedar cold-boots, and packet delivery continues while Main has delayed disk work. |
| 4a. Complete machine snapshots (user added) | After SDRAM/ARM I/O interfaces settle, add generated state export/import for every sequential chip/board and mutable RAM, plus device/clock phase, input queues, disk state and local server connection state. Use the ARM daemon and DDR staging to save/load an atomic, checksummed snapshot bundle with its matching disk image or COW generation. | Restore the full machine at a defined safe boundary. Save/continue/restore tests reproduce subsequent state, output, input, transfers and disk writes. Test pending operations and reset/error cases. Qualify a saved Cedar desktop on FPGA with a measured restore-time benefit. Reject incompatible schemas/builds before modifying the machine. |
| 5. Finish accurate clocks | Integrate the independently checked phase contract and T-control cone, then remaining processor/control/IFU/map/cache latch families. Calibrate BaseBoard, Junk/RCLK, display, Ethernet and disk timers together. | Board/microengine and extended-instruction checks, original-ROM startup, all-device timing and cold Cedar/input pass at each step. Demonstrate the original 60 ns nominal ordinary-instruction cadence and report actual instruction progress, holds and external waits separately. |
| 6. Qualify and release the combined result | Combine individually qualified changes including complete snapshots, repeat the benchmark set, run sustained disk/network/input activity, and publish reproducible artifacts with the handoff updated. | Three consecutive fresh-seed original-firmware Cedar desktop boots; verified complete desktop snapshot restore; exact short and alphabet/digit echo; mouse/click; verified disk persistence; at least two hours of sustained operation without pack/CRC/RX faults. Archive timing, traces, captures, measured speed and remaining limits; commit and push. |

### Benchmark contract

Current hardware update22:16UTC:41d passes real cold-remount readback of
both41c marker files after new Main/cache and full FPGA original boot. Its
volume is preserved after normal guest Boot and actual loader durability
barriers.42 matched raw-DDR/no-cache control is now loaded with Main40e1,
e237dffc server and fresh f3bce2a8 seed. Idle measurements and software boot
qualification are pending; no new speed claim. Keep later candidate diary
"not deployed" labels historical. Evidence: native41d and native42-field.

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

The immediate next action is instrumented, bounded coherent ARM disk service:
Native41b post-desktop stress failed on a host-write watchdog. Preserve its
halted state and physical pack; cold-remount persistence and matched42
comparison remain pending. Only
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
physical payloads/full Cedar/active STOP-reload/stress still pending. No
CPU/storage/clock/constraint changes. Do not claim a
proved synthesis cause or physical DDR success before hardware evidence.

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
