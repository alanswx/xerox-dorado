# Verilog from Sil: handoff

## Current objective (2026-10-18 UTC)

**Active goal: deliver a faster, correctness-qualified Cedar FPGA system.**
The ordered delivery plan and completion gates are in
[verilog-clock-plan.md](verilog-clock-plan.md#active-delivery-plan-and-goal-2026-10-18):
qualify M10K placement and instruction measurement; move bulk RAM to SDRAM;
reduce memory-request overhead; make ARM disk work nonblocking/coherent;
connect the daemon through DDR frame queues; finish accurate clock/timer
migration; then repeat fresh boots and sustained end-to-end qualification.
External Xerox-network tunnelling is optional follow-up. Native41d cold
marker persistence and42/47/48/49 original-firmware Viewers/input pass.
Actual active-DDR STOP/unmap/normal reload/fresh epochs pass. Two memory
policies improve preliminary idle login from1.596M to1.948M issues/s
(+22.0639%);49's full34-transfer base loader takes16.136..16.276% less time
than48, ONE boot each. Native50 deferred reads are rejected for negligible measured benefit and
extra3323ALMs. Live49b repeats the smaller selected49 baseline; idle reproduces
within0.000203%, full base loader passes. Continue
complete snapshot4a, accurate clocks/timers and repeated/stability gates.
Representative ContB now passes complete modeled board state restore and
isolated all-corner timing; whole-machine/physical snapshots remain unfinished.
Actual Main worker-delay disk service, physical packet progress and completed
input pass in separate controlled41c/47/48 tests. Sustained combined stress
remains open. Failed41b and retired working volumes remain preserved.
User added **complete core snapshots**, including matching disk/device/server
state for fast Cedar desktop restore. Milestone4a in the delivery plan tracks
this requirement; it is part of final acceptance, not a main-RAM-only dump.

**Native48 original-firmware Viewers and actual delayed-worker input PASS.**
Normal active-lease load completed01:03:46UTC19Oct. Main3781/21e67a3f,
daemon3780/c5996249, RBF40750c11,50MHz/SYSPER4, SDRAM/no cache/direct DDR.
Original f3bce2a8 seed checked unchanged before caching; fresh actual DDR epoch
ba6bc89a420de42c. Original firmware/BFN113/Pilot/login, complete33BCD+list base
loader, BootEssentials/fonts/STARTs and full Viewers pass. Real mouse/Cmd/focus
and exact short/alphabet/digit echo pass. rtl48persist.txt guest create/close/
readback passes; this marker has no cold-reload readback yet. Normal guest
boot returned to the Basic loader/profile question; germ812 was not captured.
Idle mean1,694,447.318 issues/s (+6.1931% versus clean47b,10.16668% nominal);
ONE fresh boot per configuration/three adjacent windows, not repeated gates
or drained retirement. Loader373.0718..373.3220s, no timed loader control.
Remote source hashes match34 files after timing; not physical payload hashes.
All performance precedes the actual-worker probe; three known Main GDB pauses
now include its load. Probe /tmp/dorado48-input-probe.so must remain resident
until Main exits; use a fresh Main for clean future comparisons.

Actual Main worker delay4200ms then successful fsync: Return completes the
pretyped echo worker48 and mouse40,20 moves during the proven window.
Inside capture1 precedes worker end by at least657ms; capture2 is late/excluded.
8 read/28 write ACKs,4,139,265 inner issues, no overlapping>20ms poll gap or
CRC/RX/abort/pack fault. Five altered controls reject for intended reasons.
At02:11UTC good2854/TX drops18, CRC/RX/abort0. Sustained/final gates remain open.
Evidence: native48-field and native48-worker-input; local cedar48-launch.
Main /tmp/dorado48-main.log, disk /tmp/dorado48-disk.jsonl, Cedar48_20261019.
C watcher30731, CoolBoot23417, idle44017, Guest71541 and loader observer4091/
92849 COMPLETED; do not restart. Native49 normal-loader deployment completed
02:41:13UTC19Oct. Old48 Main/server retired normally;63578624-byte pack SHA
f4548e6c53382aeb37e28e458a8c43dfc9dc77592f2208b63059733f2ffb2855 is preserved
at the original path. Full trace ends barrier_end, no errors/loss.

**Retired Native49 original-firmware interactive Viewers PASS.**
Main6414/21e67a3f, daemon6413/c5996249, temporary6399 retired normally; new
seed untouchedf3bce2a8 before caching. RBF6e7866f3,50MHz/SYSPER4, SDRAM/no cache/
direct DDR; only physical group/word interleaving differs from48. Actual
Main SPI74 flags0x16/fault+errors0, fresh epoch5cea2e754bd0665f. Daemon maps
64KiB1FF00000, Main does not map the ring and has no probe DSO. Two matching
ELF GDB controls precede measurement; core input/video0 and issue view1.
C watcher21086 and three CoolBoot pulses98326 COMPLETED; do not restart.
Fresh original BFN113/Pilot/Cedar login PASS. Three adjacent10s windows mean
1,947,687.412 issues/s,14.9453% above48 and22.0639% above clean47b,11.6861%
nominal. ONE boot each, not repeated or end-to-end qualification. No probeDSO.
Read-only loader observer47764/remote6672 COMPLETED after stop and collection;
process missing independently verified. Real Guest accepted, all33BCD+list
loads and exact final guest ACK pass. Actual timing312.55875..312.87163s vs48
373.07180..373.32199s:16.136..16.276% less time, one fresh boot each, not
repeated/full desktop qualification. All34 remote source hashes match after
timing. Profile No visibly accepted; BootEssentials/installer advancing.
At03:18UTC full Viewers Ready, first profile tool, WatchTool/EditTool/Clock
paint. Real mouse moves to912,8/Cmd click opens independent second tool; focus
737,608 then exact short/alphabet/digit echo and new prompts PASS. Guest
rtl49persist.txt create/close/readback PASS; this marker has no cold readback.
Normal guest Boot visibly reaches germ812. Full evidence: desktop-input.json
and captures in native49-field. Input71255/marker71580/Boot30950 COMPLETED.
Native50 normal loader13728 COMPLETED03:25:31UTC. Old49 Main/server and
temporary7647 exit normally; preserved63578624-byte49 pack SHA
f48ce873af077e0e2bb77bcd60f635441fe67010d0838722157b586f6ffb6375 remains at its
original path. Actual disk trace ends barrier_end, no errors/loss.
Final repeated/stress qualification remains open.
Main /tmp/dorado49-main.log, disk /tmp/dorado49-disk.jsonl;
Cedar49_20261019. Helpers cedar49-launch; evidence native49-field.

**Retired47b clean baseline (original-firmware idle login PASS).**
Same47 RBF3df4a799,50MHz/SYSPER4, SDRAM/no raw cache, direct DDR; no memory
optimizations. Normal loader completed00:48:32UTC19Oct. Final Main3277 is
21e67a3f, daemon3276 isc5996249; temporary3263 exited normally, new seed
checked untouchedf3bce2a8 before final cache. No qualification DSO in Main3277.
Actual Main SPI74 flags0x16/fault+errors0, fresh epoch67595ea973a3f023;
daemon maps64KiB1FF00000, Main sole SPI owner. One setup GDB pause selected
core video/input0 and instruction view1. New C watcher32115 COMPLETED after
fresh BFN113/release; three CoolBoot pushes2198 COMPLETED. Do not restart these
handles. Main /tmp/dorado47b-main.log; disk /tmp/dorado47b-disk.jsonl;
Cedar47b_20261019; local helpers build/rtl-executive/cedar47b-launch.
Original firmware/Pilot/Cedar login PASS. Before Guest, measure5616 COMPLETE:
three10s windows1,595,592/1,595,673/1,595,621 issues/s, mean1,595,628.966
(9.57377% nominal). Same-RBF47 repeat differs-0.000783%; no memory gain claim.
TWO setup GDB controls total, both before samples. No Guest input yet.
Next48 qualified matched load/idle comparison. Fresh47b Viewers/input remains
unqualified; retired47 already passes. Evidence:2026-10-19-native47b-login.

**Actual active-DDR lifecycle PASS.** Retired47 normal guest Boot visibly
reached germ812. Matching Main1509 called real dorado_net_barrier: true,
SPI74 flags0x16->1, old epoch preserved, faults/errors0. Independent maps
check proved daemon1506 unmapped /dev/mem. Main stayed Quiesced. Normal
load_core then used actual network/disk barriers before full47b programming;
old Main/server and temporary Main exited normally, no force kill or OS
reboot. Old volume63578624 bytes SHA41c26b7bbb6ea6b0fd7bfe5ea90f420d05c8e94f2224f45a4caf015fe2f985a5
remains Cedar47_20261018/cedar-work.pack; full old disk trace ends barrier_end,
no file/barrier error or trace loss. Evidence:2026-10-19-native47-active-reload.
This is not complete snapshots or hardware forced-bus-stall qualification.

**Native48 timing PASS/deployed.** Frozen47 plus only frozen43 raw
response-boundary successor selection and its generated PREFETCH_NEXT flag.
Same Main/server/DDR transport for the planned comparison; no interleaving,
deferred reads, clocks/constraints or architectural changes. Remote
/home/alans/mister/Dorado_Native48_20261019; exec76623/fitter498946 COMPLETED.
31 summary/36 detailed checks/four conditions pass, min+0.102ns, no new cuts
or illegal/unconstrained clocks;30,533 ALMs73%,52,795 registers,330M10Ks.
Frozen source/fit/timing evidence: native48-arbitration. Do not restart.

**Native49 timing PASS, deployed; original firmware/login and idle measurement PASS.** Frozen48 plus frozen44 group/word
physical interleaving only; no deferred reads, same DDR network/ARM/clock/
constraints. Handle69340/fitter512897 COMPLETED;31 summary/36 detailed checks/
four conditions pass, min+0.115ns, no new cuts/illegal/unconstrained clocks.
RBF6e7866f3. Source/frozen evidence:2026-10-19-native49-interleave.

**Live Native49b: selected baseline repeat, original firmware/base loader PASS.**
Normal load03:46:47UTC19Oct, Main8321/21e67a3f, daemon8318/c5996249,
temporary8310 retired normally; same49 RBF6e7866f3,50MHz/SYSPER4, SDRAM/no
cache/direct DDR, arbitration/interleaving ON, deferred reads OFF. New original
f3bce2a8 seed checked unchanged before caching. Main-owned SPI74 flags0x16/
fault+errors0, fresh epoche062b80b135256f6. Daemon maps64KiB1FF00000, Main
has no ring mapping or probe DSO. Two setup GDB controls before samples.
C watcher9694/CoolBoot81386 COMPLETED. Original BFN113/Pilot/Cedar login pass.
Idle83013 COMPLETE:1,947,691.366 issues/s versus first49's1,947,687.412,
+0.000203% over an independent fresh boot; not three final release boots.
Guest31922 accepted. All33BCD+list/final guest ACK pass311.33993..311.59041s;
34 remote source hashes match after timing. Observer8733/Mac38162 COMPLETED
and absent independently; No34368 sent. Fresh49b full Viewers Ready/Watch/
Edit/Clock, real mouse/Cmd/focus and exact short/alphabet/digit echoes with
new prompts PASS at04:38UTC. Normal guest rtl49bpersist.txt create/close/
readback PASS at04:41UTC; no cold readback yet. Latest good2144/TX drops11,
CRC/RX/abort0. First profile tool continues optional installs. Main /tmp/dorado49b-main.log, disk /tmp/dorado49b-disk.jsonl;
Cedar49b_20261019, helpers cedar49b-launch, evidence native49b-repeat.
Manual pre-Guest idle delay varied, so loader values are observed repeats,
not the final automated pinned benchmark gate. Do not restart old helpers.

**Retired Native50: deferred reads NOT selected.** RBF70886931 all-device
31/36/four-condition timing passes min+0.035ns;33,796ALMs81%,55,761regs.
Original firmware/login/base loader pass; idle1,948,049.932 (+0.01861% vs49),
loader311.63439..311.86355s. No meaningful gain for3323 extraALMs/2832regs;
50 lies inside observed49 loader variation. Optional RTL/tests/evidence remain;
full50 Viewers/input not performed and not a release requirement for rejected
policy. Old Main7660/daemon7659 and observer7959/96827 retired normally.
Actual normal49b loader barriers pass, old trace ends barrier_end without
errors/loss. Physical50 pack63578624 bytes SHA
0b1572b76a2662f372c5a8b79eebf281f7976fc780369de3514f9c20189080f6 preserved
at original path. Retired at profile prompt with no user files, no clean guest
shutdown or complete snapshot claimed. Evidence: native50-field/native49b-repeat.

**Snapshot4a18-bit enabled-bank prototype PASS, NOT integrated.**
Actual ContB grouping is eight18-bit halves (34data+2parity per word), plus
all144 read-pipeline bits. f10415_enable_bank.sv preserves the original guest
read/write edges using host-clock enables; saved raw pipeline survives scans,
release/backend hold and recapture until first real guest advance.18 unchanged
cells plus independent array:57417 host edges/20225 backend-held edges/5125
state accesses, all RAM/raw pipeline/CE/old-data/invalid-index/first-edge tests
and five broken controls pass. Matched opposite-edge registered-shell fits
retain18432bits/twoM10Ks/OLD_DATA. All12 setup/hold/pulse checks per variant/
four conditions pass min+0.255ns; no illegal/unconstrained clocks/I/O, R/R N/A.
Fixture counts53->102ALMs and48->138regs; delta includes shell/admin registers,
not pure bank overhead or a full-core projection. Six damaged-report controls
reject. Two-clock templates fail Quartus memory inference. Direct virtual-I/O
baseline hold failures remain archived; final fixture models real launch/capture
registers with unchanged2ns/0ns I/O and10ns internal budgets, no timing cuts.
Remote isolated fit SnapshotEnable18_shell_20261019/handle56146 COMPLETED.
Source fpga/mister/snapshot; reports2026-10-19-snapshot-enable. Next integrate
actual ContB halves with the exact guest advance and test complete board
behavior before core integration. Global barriers/full-state bus/SDRAM/Main/
server/bundles/physical Cedar save/restore remain required and unfinished.

**Snapshot4a actual ContB IM access/isolated routed fit PASS, not deployed.**
Separate generate_contb.py experiment preserves non-RAM board logic; eight
18-bit banks replace144 original RAM instances. All71 board outputs plus144
raw read bits match the unchanged original board and an independent array
oracle over97418 host cycles/67357 holds/13334 late enable changes/57392 state
accesses. Real-strobe writes cover all8 halves; every RAM/read bit, all invalid
rows, cross-bank responses, destructive IM writes/restore, recapture and first
actual guest edge pass. Four damaged RTL controls reject. Other FFs stay held
while IM is changed/restored; no full-board continuation/restore claim.
Matched board fits use original falling-edge global gate/actual host enable,
50MHz registered shell,2ns/0ns I/O/10ns shell paths, no cuts. Both retain147456
bits/16M10Ks; candidate eight1024x18 OLD_DATA. All12 checks per variant/four
conditions pass min+0.168ns, R/R N/A, no illegal/unconstrained clocks or I/O.
Counts287->600ALMs/325->584regs (+313/+259) include fixture/admin pruning;
not whole-core overhead/timing. Eight damaged-report/project controls reject.
Remote SnapshotContB_20261019/Mac97952 COMPLETED. Evidence snapshot-contb-board;
source fpga/mister/snapshot. Original board inventory separately elaborated:
99 other sequential candidates/209bits in eleven MC10176 and eleven MC10231
packages (q, pre-capture and clock-history). These need bounded state ports;
remaining inventory categories still need classification. Next full-board
continue/restore, then wider scope/whole-core timing/global services barrier.
No production generated RTL/QSF/FPGA state-access changes. Live49b remains.

**Snapshot4a complete modeled ContB restore/classification/isolated fit PASS.**
Full-state generator preserves the audited IM interface and exports all209
q/pre-capture/clock-history bits from eleven MC10176 and eleven MC10231 cells.
Separate adapters retain original nonblocking logic and S/R/PRECAP semantics
under the exact host-clock advance; original production sources unchanged.
State index is15bits: groups0..7 IM, group8 words0..21 sequential packages;
all other addresses error/no mutation. Padding to18bits reads0/writes ignored.

Mac full-controls73290 COMPLETED:132764 host edges/92737 held/13334 lateenable/
82735 state accesses, all71 outputs and147809 state bits. Independent original
board/array oracle,209 FF bit walks, every invalid address, candidate-only
10000-cycle continuation, RAM/FF destructive writes, complete restore and20000
subsequent execution cycles pass from the first actual guest edge. Reference
never patched; fixture-only RAM seeds and captured external inputs. Six bad
variants fail intended causes, including lost history/setup and RAM aliasing.
Fresh original XML classification v3 accounts for every candidate (saved or
continuous-wire recomputation), seven missing/new/overlap/alias controls reject.
The machine must additionally capture external inputs/clock phase and drain
state-port requests; this does not create a whole-machine snapshot barrier.

Remote SnapshotContBFull_20261019/Mac68165 COMPLETED. Matched50MHz registered-
shell fits retain147456bits/16M10Ks/eight1024x18 OLD_DATA. All12 checks per
variant/four conditions pass min+0.165ns; R/R N/A; no cuts, illegal/unconstrained
clocks or I/O. Counts288->774ALMs/320->693regs (+486/+373) include shell/admin
pruning and are not whole-core timing/resource projections. Nine damaged
adapter/project/source/timing/coverage controls reject. Sources *contb_full*
in fpga/mister/snapshot; evidence reports/2026-10-19-snapshot-contb-full.
Next integrate access with the complete-machine hierarchy and classify/access
other boards, wrapper/devices/input and physical SDRAM/Main/server. Establish
coherent global pause, matching disk generation/server state, atomic bundles,
pending/error/reset tests, full-core STA and actual Cedar restore timing.
Production generated files/cells/QSF/live FPGA unchanged;49b remains running.
60ns clock migration and final repeated/stress qualification remain required.

**Native51 complete-core ContB integration TIMING PASS, NOT DEPLOYED.**
Frozen49 plus qualified host-clock/exact-advance ContB adapters. All snapshot
admin inputs remain inactive until global barrier/transport, so no physical
save/load or enabled-access area budget. Other boards/storage/cache/map/ECC/
Main/server/clocks/constraints unchanged. Build62531/fitter748087 COMPLETED;
31 summary and52 detailed reports/four conditions pass, minimum+0.115ns.
16 added ContB-to-other-board setup/hold reports come from definitive sequential
crossing audit1060 COMPLETED after the main build. Superseded overlap79065 is
archived separately, not used for qualification. No cuts/illegal/unconstrained
clocks; existing framework I/O counts unchanged3/13input,50/154output. Do not
claim zero unconstrained I/O. Eight1024x18 OLD_DATA macros proven in fitted
core. Only26 unused-object map warnings added; no other warning increases or
Critical Warnings.30640ALMs73%,52921regs,330M10Ks,35DSP (+167ALMs/-8regs vs49).

RBF3d253187b42615e53fde1dc1bed688eb0328265d007bffdc3c161196294ccac3;
local native51-candidate. Actual remote Dorado_Native51_v3_20261019, matching
assets/builder Dorado_Native51_inputs_v3_20261019. Strict collector checks
sources/QSF/SDC/crossing/RAM inference. Quartus-added duplicate relative aliases
of only the four approved sources are normalized; extra source/SDC/macro
controls reject. Frozen parity documentation-only difference is archived/code-
line identical. Earlier terminal preflights remain preserved; never restart.
Next normal guest Boot/preserve49b, normal active-DDR/disk barriers and fresh
original seed for51; original firmware/Cedar/input/progress qualification.
Live49b remains Main8321/server8318 for now. No speed gain/physical snapshots
claimed. Evidence native51-contb; source build_native_contb.py.

**Exact frozen49 native-machine snapshot inventory/ContB coverage PASS.**
Frozen source/macros copied from actual working49, no newer generator.4158
instances/8644 sequential candidates/2107636bits; other candidates remain
review-only. All1572 actual nested ContB candidates are classified against
qualified manifest:147809 state bits saved, continuous wires recomputed.
Compared old45-style deferred illustration,128 groups lose pipe_addr16 and
pipe_known1 (256entries/2176bits); old_data and pending/cache/read/write state
STILL exist and must be saved/normalized explicitly. Per-scope counts archived.
Outer emu/frame/input/transport/clock state, physical raw16MiB SDRAM, ARM disk/
worker/server remain outside scope. Next extend/classify those and otherboards;
no whole-machine completeness claim. Source inventory_native.py, evidence
reports/2026-10-19-snapshot-native49-inventory, local handle31688 COMPLETED.

**Retired47: direct-DDR original-firmware interactive Cedar Viewers PASS.**
Loaded23:47:39UTC from verified stock MENU. Final Main1509 is21e67a3f,
server1506 isc5996249; staging Main1504 exited normally. RBF3df4a799,
50MHz/SYSPER4, SDRAM/no raw cache and no43/44/45 memory optimizations.
Original f3bce2a8 seed checked unchanged after the temporary Main exited;
final Main cache-ready63578624 bytes and real DDR RESUME ACK observed.
Daemon maps exactly64KiB at physical1FF00000; Main has no mapping there.
Main-owned command74 reports flags0x16, epochfdfe0a77fc6c87b5, fault/errors0.
Original-firmware Viewers/input and later active-lease reload pass; independent
physical packet bytes remain pending. These are retired process IDs.

Core video/input0 and instruction view1 selected with matching21e6 ELF
`/tmp/dorado-main-21e6.elf`; three known Main GDB pauses (setup/capability, idle selection, worker-probe load). Do not use
old40e1 addresses. Current symbols: input_poll85580, video_fb_enablead7fc,
status_set9e98c, disk_barrier67b50 (Thumb call addresses+1).
C watcher47714 COMPLETED and releasedC after new BFN113; do not restart it.
Three real CoolBoot pulses89381 completed. Original47 boot passes the initial
wait and physical-DDR BFN113;23:54UTC capture shows Pilot850 vmInitialized
(normal startup);23:56UTC Cedar login Name prompt is visible. No
CRC/RX/abort/transport/pack fault observed. Login PASS; idle
measurement31425 COMPLETE before Guest: three adjacent10s windows
1,595,651/1,595,632/1,595,641 issues/s, mean1,595,641 (9.57385% of original
nominal cadence). Same speed as41c within this one-boot comparison; Main/
server/network differ, so not a matched backend/performance promotion. Normal
Guest\n\n input sent after measurement and ACCEPTED:00:00UTC19Oct
Guest.pa/Grapevine-down fallback and Basic boot file locating the remote
Basic.Loadees. All34 loadees complete, profile No entered and visibly accepted;
BootEssentials/fonts/FontMetrics and STARTs pass.00:34UTC19Oct capture shows
Viewers Ready, WatchTool/EditTool/Clock and the profile's first CommandTool.
Real mouse/Cmd opens a ready independent second tool. Both `echo rtl47
keyboard ok` and full alphabet/digit echo print exactly and return to `%`.
Settled capture00:38UTC confirms final input drain. Pointer737,608; core input0.
At desktop good2388/dropped24, CRC/RX/abort0. Later STOP/reload passes;
sustained tests remain open. Evidence: reports/2026-10-19-native47-desktop.
Paths: `Cedar47_20261018`, `_Computer/DoradoRTL_Cedar47_20261018/Cedar47.mgl`;
Main `/tmp/dorado47-main.log`, disk `/tmp/dorado47-disk.jsonl`.
Local helpers/handles: `build/rtl-executive/cedar47-launch/`; deployment
99188 completed. Subsequent normal guest Boot/germ812 and active STOP pass.
Physical packet payload bytes remain independently unqualified.

**Actual Main delayed worker + physical DDR PASS.** Probe55bc9a8c is now
loaded in Main1509 at /tmp/dorado47-loader-probe.so, must remain resident
until Main exits. Actual source FD20 is duplicated; real worker waits4200ms
then fsync succeeds. This is delayed flush work, not a4.2s kernel syscall.
Window1915296794925..1919496928940ns:10 read/100 write ACKs; no poll gap>20ms,
trace error/drop or in-window CRC/RX/abort/pack fault. Inner Main snapshots
advance3,904,318 issues/150,001,118 host edges; TX drops stay15. Six complete
physical guest packets, Watch.bcd metadata response168B and its exact EOC ACK
are conservatively bounded in-window. No typed command/mouse test performed.
Evidence: reports/2026-10-18-native47-worker-ddr. Missing metadata/guest-ACK
controls fail the observer qualifier. A checked active-STOP GDB helper is
COMPLETED in native47-active-reload after Viewers/input and normal guest
Boot/germ812. Main1509 has retired, so its DSO lifetime ended normally.
Fresh47b Main3277 has no probe DSO and will supply clean baseline measurements.

Native47 full timing PASS:31 summary checks/36 categories/four conditions,
min+0.111ns, zero illegal/unconstrained clocks;30,480 ALMs (73%),52,639
registers,330 RAM blocks. No new timing cuts or CPU/storage changes.
Build23028/collector66320 are COMPLETE; parent439743/fitter441257 retired.
Do not restart them. Timing/source/build evidence and checked deployment:
`reports/2026-10-18-native47-drained-state`; actual ARM final-daemon raw-socket
boot/Cedar/STP/retry/wrap/history/quiet/reexec byte tests also PASS there.

**Native46 hardware startup FAILED safely, no DDR RESUME/mapping.** Normal
loader programmed46 and changed to immutable Main21e67a3f, staging PID22443.
Command74 through actual Main returned flags0x10: owned, not stopped, enable
clear, no initialized/fault/epoch. Old server has no control listener, so a
separate real STOP-only service22607 was started on an isolated raw socket.
Its STOP ACK did not change the hardware indication; Main timed out the drain
and refused normal exit. No fake STOP/epoch or guest/IOCB repair was used.
Dorado42's original guest Boot was visibly accepted at germ812 before load;
its actual loader disk trace ends with two successful barriers/no errors.
Retired42 physical pack SHA0c5ab6f58c22bac6dfd3cb807ad9a02f534715ca9d5aa189758fa83afa823619,
63578624 bytes, remains at `Cedar42_20261018/cedar-work.pack`.

Both daemons had no /dev/mem mapping and exited normally. Actual staging
Main disk barrier returned true immediately before whole-board OS reboot;
46's physical seed remained exactf3bce2a8. Reboot recovery is NOT a passed
network hot-reload/STOP test. Preparation/INI/new immutable binaries and
failure/recovery metadata remain under `Cedar46_20261018` and
`System_20261018_DDRNetwork`; local diagnostics: cedar46-launch.

Native47 removes only the redundant stopped flag and decodes the existing
already-drained STOPPED FSM state. All zero/wrap/full/mid-frame/accepted-read/
stalled-write/reset/epoch tests pass with identical deterministic traffic
counts; premature-stop/byte-order/epoch controls still fail. Combined actual
production wrapper/UIO/wire+C endpoint passes10,038,206 clocks with independent
serial payload/CRC oracle,2048-byte frames, pauses, pending SPI ownership
handoff and new-generation restart. Broken mux/ungated wire controls fail.
This fixture contains no CPU/DskEth board; original board gates stay separate.
Main regression now explicitly refuses enable=0/epoch=0/not-stopped startup.
Native47 all-device timing and real startup/grant now pass; full boot,
physical packet bytes, active STOP/reload and stress remain unqualified.
CPU/storage/clocks/constraints are frozen46; no new cuts. No new Main binary.

**Retired Native42 matched raw-DDR control (preserved).** Loaded22:15:51UTC;
Main20496/server20495, Main40e14b6b/servere237dffc unchanged. RBF45af2e30,
50MHz/SYSPER4, same logic/M10K placement as41, no raw line cache, original
f3bce2a8 seed and Pup time1792324800. Timing passes29 checks/20 categories/
four conditions min+0.115ns. Main remains sole SPI owner; core framebuffer0,
instruction view1. C watcher38648 and boot pulses55801 COMPLETED; do not
restart them. Guest\n\n and profile No were entered after idle measurement.
All34 loadees, BootEssentials and font/FontMetrics Bringovers complete;
23:04UTC capture shows package STARTs advancing;23:07UTC shows gray
Viewers startup raster/Idle.23:10UTC captures Ready/Viewers, initial
CommandTool doing startup apps, WatchTool/EditTool/Clock icons.23:12UTC
second CommandTool passes exact `rtl42 keyboard ok` and full alphabet/digit
echo plus returned prompts, real mouse movement/Cmd click/focus. Original
first tool continues optional apps; no guest/daemon cursor repair.
No CRC/RX/abort or pack fault observed; TX dropped counter advanced to9.
Paths: `Cedar42_20261018`, `_Computer/DoradoRTL_Cedar42_20261018/Cedar42.mgl`;
Main `/tmp/dorado42-main.log`, disk `/tmp/dorado42-disk.jsonl`.

**Preliminary matched idle result:** three adjacent10s windows average
1,447,771 issues/s on DDR42 (8.6866% of nominal60ns ordinary cadence), versus
1,595,727 on SDRAM41c: SDRAM is10.2196% faster. ONE boot per configuration;
not repeated independent cold boots or drained retirement. Same board/clock/
placement/Main/cache/server/seed/time, differing raw backend. Separate cause
windows show76.7900% storage pause,23.2095% enabled, zero pack pause, and
0.18365% held starts. Cause windows are not simultaneous attribution. Do not
call the6.25M unpaused ceiling actual throughput.13 documented Main GDB
control pauses precede Guest; exclude them from host-poll latency claims.
Evidence: reports/2026-10-18-native42-field/{idle-*,matched-idle-comparison.json}.

**Native41d cold-remount persistence PASSES, retired/preserved.** Both marker
values exactly read in the idle second CommandTool at22:10UTC after a new
Main/cache and full original-firmware FPGA boot: rtl41cpersist20261018 and
worker42, each returning to a prompt. Source41c81c41955 remains unchanged.
After another normal guest Boot, actual Main loader durability barriers
preceded42 programming/restart; retired41d physical image is c7420ed1...
at its original path. Preservation/captures/deployment/observer evidence:
reports/2026-10-18-native41d. No power-loss/full-snapshot/two-hour claim.
The optional post-readback GDB barrier attempt failed before calling Main
(selected language lacked bool); its log remains, later real loader barriers
are the successful boundary. Fifth known diagnostic GDB pause on41d.

**Previous41d bring-up details (not live):** Main19438/server19352;
same Native41 RBF a97af936, Main40e14b6b and server e237dffc. Normal Cedar Boot
was visibly accepted on41c; Main16875 then exited through its real durability
barrier and server16872 stopped normally. Immutable physical source image
`Cedar41c_20261018/cedar-work-clean-boot-preserved.pack` is63578624 bytes,
SHA25681c4195570989a9d3ebf216f5600348af0c57997c738c4e98bf3ea6dd351ca04.
New41d uses a copied image, a new ARM process/cache and full FPGA reload.
This is NOT an untouched-seed performance boot. Old source and preserved
image remain available. No complete FPGA snapshot exists yet; guest markers
must be read after the cold boot before claiming persistence.

Paths: `Cedar41d_20261018/{cedar-work.pack,server.log,deployment.json,
qualification.json,reboot-resume.json}`, `_Computer/DoradoRTL_Cedar41d_20261018/
Cedar41d.mgl`. Main log **`/tmp/dorado41d-main-final.log`**, disk trace
**`/tmp/dorado41d-disk-final.jsonl`**. A temporary loader-reexec Main could
not reopen the first O_EXCL trace; it was normally stopped and final19438
started with these fresh paths and verified cache-ready. Do not use staged
PID19353/19371 or old41c16875. Main remains the sole SPI owner.
Main-only FB disable/enable workaround and passive instruction view1 are
selected through matching40e1 ELF, three known diagnostic GDB pauses.
Three actual CoolBoot pulses finished; C watcher61319 has observed the new
BFN113 and released C. Do not restart it. No current pack/CRC/RX/abort fault.
Guest.pa default login accepted with two Returns. All34 loadees/profile No
and BootEssentials pass. Existing newer font DFs prompt before overwrites;
accept their visible No defaults with Return. FontMetrics confirmation
answered; Viewers reached gray raster/style loads at22:03UTC. Both cold
marker reads remain pending. Four known Main GDB controls: after initial
FB disable/enable and counter selection, `video_fb_enable(0,0)` restored
core video/input from Linux. **1 selects Linux,0 selects core**; leave it0.
Boot C/modifiers were released again with core focus.
Local helpers: `build/rtl-executive/cedar41d-launch/`.

Snapshot milestone4a: logical-path XML inventory covers4158 native instances;
all candidates still UNCLASSIFIED and wrapper/raw-SDRAM/Main/server scope
needs coverage. New isolated1024x34 IM-like RAM bank save/restore passes
against34 unchanged original cells plus independent array oracle: all RAM/
read-pipeline bits, invalid indices, destructive continuation, first-edge
resume and30000 random steps; four broken RTL controls fail. Matched50MHz
virtual-I/O fits retain fourM10Ks/34816 bits with+62 ALMs/+43 registers;
all four conditions pass, snapshot minimum+0.096ns, no unconstrained paths,
all34 collision modes OLD_DATA. Five damaged report/source controls reject.
Separate-address writes lost RAM inference; per-bit restore enables cost34
M10Ks. Whole34-bit words through one write port preserve packing. This is
NOT board/core area/timing or physical save/load. Actual board access/global
freeze, complete inventory/ARM state/bundle/Cedar restore remain pending.
Sources fpga/mister/snapshot; evidence reports/2026-10-18-snapshot-inventory
and2026-10-19-snapshot-ram. Production cells/core unchanged.

**Direct DDR software candidate integrated;46 startup failed,47 pending.**
Optional wrapper/UIO ownership mux, daemon control/validated mapper and Main
nonblocking fresh-epoch manager are implemented. Actual C service/client/
manager pass guarded-memory/SPI tests on Mac/Linux; production-runtime ARM
also passes actual patched loader/reboot/reexec/init prologues. Delayed STOP,
failed cleanup, physical drain, inherited leases, faults/stale ABI, partial
records, second owners and legacy fallback are covered. A real test caught
and fixed old-owner EOF racing a prompt reconnect. Default and DDR-enabled
but ungranted real-server boot/STP/replay regressions pass. Disk worker and
SIGTERM/SIGINT regression still passes with the network barrier linked.
Static ARM test failed at test-thread joining; dynamic ARM passed. Preserve
both; no static-runtime qualification or diagnosed cause claimed.

Native46 freezes41's architectural/cell/storage sources and adds only the
network wrapper/UIO/engine. Full four-condition timing passes31 summary/
36 detailed categories, minimum+0.112ns; illegal/unconstrained clocks0;
73% ALMs,52727 registers,330M10Ks. RBFc7e6f373. Sources/SDCs/hashes/reports:
reports/2026-10-18-native46-ddr-network. No new timing cuts.
New pinned Main21e67a3f (ELF118d90bd) is built at
`misterubuntu:/home/alans/mister/Dorado_DDRNetwork_20261018/Main/bin/MiSTer`;
running Main40e1 stays unchanged. New daemon build is isolated alongside.
Apply new main-ddr-260912.patch AFTER the existing four patches; copy all
Dorado Main sources plus exact server protocol headers. Candidate/control
results: reports/2026-10-18-ddr-network-control. Next qualify original wire+
engine+UIO mux, physical mapping/coherence, byte-exact boot/STP/full Cedar,
reset/reload and packet progress during delayed Main work before promotion.
The sole board is now on timing-qualified47 with a legitimate physical
DDR grant. Preserve42/46 artifacts; finish the original47 firmware boot.

**Previous Native41c qualification (stopped, not live):**
Main16875, server16872. Same RBF a97af936 and e237dffc fixed server, fresh
**Main live log is now `/tmp/dorado41c-main.log` (both stdout/stderr),**
moved at21:16UTC to keep diagnostic writes off FAT; the original
Cedar41c/main.log is the retained prefix and no longer advances. No guest
state/clock/IO patch; fourth known Main GDB control pause. All future
collectors/launchers must use the new path or inspect `/proc/16875/fd/1`.
original f3bce2a8 seed, pinned Pup time1792324800,50MHz/SYSPER4, SDRAM and
no raw line cache. New Main40e14b6b is immutable at
`System_20261018_DiskCache/MiSTer-40e14b6b97d62435`; matching ELF alongside
and `/tmp/dorado-main-40e1.elf`. `DORADO_DISK_CACHE=1`, disk trace
`/tmp/dorado41c-disk.jsonl`, Main net trace enabled. Actual63578624-byte
image-cache-ready observed; initial CPU is in normal BaseBoard spin-up.
No guest-state import or completion/IOCB patch. The normal core loader
programmed the RBF, its temporary automatic Main reexec was normally stopped,
then final Main started with explicit cache/trace environment and a fresh
server before CoolBoot. Deployment checks current binary/preserved pack,
seed/RBF/timing and candidate source hashes. Old41b pack remains0046452c...
unchanged; its dirty FPGA track could not be captured before reset.

The real C watcher session70748 was armed after final Main initialization
and held C through the new13-word type264/BFN113 request.
It completed at the new BFN113 request~20:20UTC and released C; do not restart. Three real boot-button pulses20201
completed and modifiers are UP. Main-only FB toggle33489/72582 completed,
no capture reset; HDMI shows expected uninitialized firmware raster, not OS
text yet. No active GDB. BFN113 and actual Cedar login have passed. Three10s idle windows give
1595653.86/1595625.89/1595901.82 issues/s, mean1595727.19/s=9.574%
nominal, reproducing41b; issues are not drained retirements, and three
windows are one boot. Guest accepted~20:25; all34 loadees complete and profile No sent at20:35.
Actual HDMI passed BootEssentials/fonts/STARTs and **Viewers desktop at21:03**.
Real mouse move/Cmd click opens second CommandTool and focus works; exact
`rtl41cok` and full alphabet/digits echo with new prompts at21:06. Local
`///rtl41cpersist.txt` created/closed and `type` reads exact
`rtl41cpersist20261018` at21:08, returning to prompt. **Cold remount is pending**;
preserve this mutated pack. Optional first-tool imports still run (BiScrollers
near21:08). All keys/buttons up, no active input/GDB helper. No current fault
(CRC/RX/abort0, good2453/dropped6 at21:08:34). Whole-Main controlled delay,
three fresh boots and two-hour final stress remain pending.
Observer prefix/current input evidence: native41c/input-qualification.json,
actual HDMI captures, dorado41c-disk-prefix-2110.jsonl.gz + statistics.
Next qualify a durable close/cold remount of this marker and a controlled
multi-second actual Main file stall while input/packets progress. Preserve
41c before42/43/44/45 comparisons; do not label RAM-cache readback persistence.
Snapshot milestone4a remains planned, not implemented.

**Update21:35UTC:** the actual Main worker delayed-flush gate passes disk
service/instruction progress:4200ms bounded flush work,3 reads/21 writes
acknowledged during the window, no overlapping poll gap above20ms or current
pack/CRC/RX/abort fault. The injected job delays then calls real fsync; it is
not observed kernel latency. Mouse moves and Echo42 completes afterward;
the in-window capture does not prove command completion. There are no new
network packets in the window, so packet progress remains open. A real Main
durability barrier succeeds and `type ///rtlflush42.txt` reads `worker42`.
Evidence/source/bridge tests: `reports/2026-10-18-main-slow-flush/`.
Qualification DSO55bc9a8c remains resident; do not dlclose it. Its one job has
finished. Main16875 remains the sole SPI owner. Setup and post-test GDB pauses
are outside its measured window. Normal Cedar `boot` is now visibly accepted
(germ812 on screen); preserve the mutated pack. A new Main process/image
remount and marker readback are still required for cold persistence.

Fresh paths: `Cedar41c_20261018/{main.log,server.log,cedar-work.pack}` and
`_Computer/DoradoRTL_Cedar41c_20261018/Cedar41c.mgl`. Local helpers
`build/rtl-executive/cedar41c-launch/`, committed native41c reports. New
Main symbols differ: input_poll=84310, status_set=9d71c (Thumb9d71d),
video_fb_enable=ac61c (Thumbac61d). Helpers assert matching40e1 ELF/hash;
never use old1fb2 numeric call addresses with this process. Measure idle-login
issues before Guest with prepared measure-login.py, then cause views if required for a new question.
Memory available after preload317608KiB; the cache resides in ARM DDR.

**Retired Native41b, fresh same-bitstream SDRAM boot with the fixed
e237dffc daemon.** Main13741/server13734. Deployment18:28UTC, RBF a97af936,
Main1fb2, original fresh f3bce2a8 pack, pinned time1792324800; directories
`Cedar41b_20261018` and `_Computer/DoradoRTL_Cedar41b_20261018/Cedar41b.mgl`.
Original firmware CoolBoot requested by three real boot-button pulses after
Main reinitialized/connected. Real C watcher completed at the actual new
13-word type264/BFN113 request at18:34UTC and released C. Tool session59506
is terminal; do not restart it. All boot/typing keys are UP; no active input
or GDB helper. Actual HDMI Cedar login at18:35; three10s idle windows measure
1595829.38/1595659.65/1595627.74 issues/s, mean1595705.59/s=9.574% original.
This reproduces41's idle rate; three windows in one boot are not three fresh
boots, and issues are not drained retirements. Before the later fault, instruction debug
view1 showed machine=issues/cpu=host edges; current fault overrides both fields.
Real `Guest\n\n` at0.1s down/up was accepted, Grapevine-down fallback
finished, and the fixed server serves Cedar6.1/Top/Basic.Loadees at~18:39UTC.
All34 base loadees loaded; the profile question was answered with one `n`
at~18:46UTC, and HDMI confirms No. BootEssentials passed PrintTV.df and
PrintTVImpl without repair and later AMTypes/Interpreter/Viewers/Watch.
All fonts and STARTs completed; **Viewers/Ready./CommandTool/Clock/Watch/EditTool
are up from this fresh unassisted boot.** Real +100,+80 mouse moves, Cmd click
opens a second CommandTool and body focus works. Actual0.1s-down/up keyboard
commands `echo rtl41bok` and `echo abcdefghijklmnopqrstuvwxyz0123456789` show
both exact replies and return to `%`. Initial immediate screenshots preceded
deferred input dispatch while optional startup work ran; the later screenshot
proves both commands/output without retyping. Do not equate helper exit with
guest completion. Created/closed `///rtl41bpersist.txt` through ordinary
`echo rtl41bpersist20261018 > ...`; `type` reads the exact marker and returns
to prompt. **Cold remount remains pending.** Preserve the guest-mutated pack.
No guest
state import or ARM cursor repair in this run. Main's known FB enable/disable
workaround restored HDMI; the earlier firmware raster was uninitialized noise.
Main remains the sole SPI owner. Preserve this run and its fault evidence before subsequent reloads
and the matched DDR42/optimized43 comparisons.
Exact deployment/assertions/restart config and captures: native41b reports;
local launch helpers in `build/rtl-executive/cedar41b-launch/`.

**Preserved failure,19:48UTC: Native41b POST-DESKTOP STRESS FAILED.** The core is
halted on a latched four-second host-transfer watchdog: machine=802a020a,
state=47fc, cpu=000018ff. These fields now report the fault, overriding the
selected instruction counters. Reason8, cylinder42/head0/block2, serial parser
idle, host_state1 awaiting WRITE ACK assertion, sd_wr=1/sd_rd=0/sd_ack=0,
LBA6399. CRC/RX/abort remain zero. Main13741/server13734 ran until the20:15 Native41c deployment.
The exact current fault first appears at line112652 of the archived accumulated
Main log; earlier prefix faults belong to previous runs. The frozen screen
still shows the desktop and marker read. A normal `boot` was typed after
refocusing, but is not visibly accepted; do not attribute this timeout to
Booting.Boot or claim a clean reboot/remount. Optional startup imports were
also in progress. All keys/buttons are up; no active input or GDB helper.

Main's image fd31 has O_RDWR|O_SYNC, position3292160, exactly the end of the
31-block window beginning at LBA6399. Repeated writes to the same cached track
mean this cannot identify which write generation Main last serviced. No
monotonic/syscall observer covered the timeout itself: earlier Native34 long
O_SYNC writes are a strong suspect, not proof of this fault's blocking call.
Do not clear the fault, fake a completion or reset before preserving evidence.
The physical image was copied without mutation at19:48:14UTC to
`Cedar41b_20261018/cedar-work-fault-preserved.pack`,63578624 bytes, SHA256
`0046452c45d333b30020a47546e5fdfa65b4ecb9edd46d14cbb9ba048bf02b7b`;
source size/mtime stayed unchanged and both hashes matched. This copy does
**not** include dirty FPGA track RAM. Original remains untouched.

Next implement bounded coherent Main disk buffering/file work and timestamped
request/ACK/file-operation/poll observations. Cover injected multi-second IO,
read-after-write, neighboring data, queue saturation, failed writes and drain
before remount/core programming/fork/reboot. Then repeat fresh boot, exact
input, actual cold-remount marker persistence and sustained stress. Do not
promote42/43/44 from the failed stress result. Evidence: native41b first-disk-
fault.json, fault-pack-preservation.json, compressed full logs and screen.
The earlier fresh desktop/input/file-close/read successes remain valid;
cold-remount persistence and two-hour stability remain unqualified.

**ARM disk-service candidate implemented, component-qualified and now on41c.**
`fpga/mister/main/dorado_disk{,_cache,_trace}.{cpp,h}` plus main-disk patch
preload one <=64MiB regular image into ARM DDR, coalesce dirty sectors and
run all file IO on Main's actual existing worker. Whole-image dirty capacity
prevents reads queuing behind long O_SYNC writes; one64KiB immutable write
snapshot/job, bounded metadata/trace. Main alone captures/ACKs SPI under the
short cache lock. Failed data is retained with explicit recovery access;
bounds/generation/readonly/error checks happen before capture. All normal
remount/init/core-program/fork/reboot boundaries drain+fsync. SIGTERM and
pinned input.cpp SIGINT defer termination into Main's loop, and reject it on
failed dirty data. Forced termination/power loss still lose volatile RAM.
A Main drain does not flush a dirty FPGA track or Cedar's filesystem.

Actual cache/worker/file tests and adapter SPI spies pass Linux and ARM,
including4.2s stalls, full8-slot queue, overlap/neighbors, complete physical
close/remount oracle,64MiB all-sector-dirty capacity, EINTR/short IO,
write-during-sync, ENOSPC/fsync EIO, stale generations and normal termination.
Linux shared-buffer/lost-dirty controls fail. ARM test-loop max gap2.782ms
cache /4.977ms adapter, not a whole-running-Main/input/network result.
Monotonic trace independently shows >1900 read+write ACKs during the4.2s IO.
An initial ARM harness thread-identity assertion was corrected to runtime
pthread identity, with production source unchanged; failure retained.
GCC10.2.1 full pinned Main build passes, SHA40e14b6b97d62435c6e1ecd5c78bf9959c74b816a8e9452b1e64c28179ca8188;
`misterubuntu:/home/alans/mister/Main_DoradoDisk_20261018/bin/MiSTer{,.elf}`,
local `build/rtl-executive/arm-disk-adapter/`. Source/compiler identities and
reports are in reports/2026-10-18-disk-cache. Immutable Main is now on fresh same-RBF41c with DORADO_DISK_CACHE=1 and
a new /tmp disk trace. Qualify real polls/input/packet delivery,
close/cold remount and stress after original boot.
Keep the failed41b physical pack untouched; use the original fresh seed.

**Native45 deferred-read experiment: simulations and full timing PASS, not deployed.**
Optional default-OFF DEFER_READS tracks the previous internal RAM pipeline
address/known-value flag. It fetches only an observed unknown stage or a
pending write's old word. Pending writes finish first; old_data bypass preserves
same-address prewrite outputs even after raw RAM was updated. This is not
RAS-only request gating. Original MK4096 cells/pin wiring/output cycles and
architectural cache/map/ECC stay unchanged. Helpers expose --defer-reads /
--storage-defer-reads; default generation is byte-identical (12 files each
DDR/SDRAM). Opt-in changes only16 group parameters and mapping/manifests.

Independent original-bit-cell tests pass14/16-bit addresses,4745 cycles,
382 writes,40 retained-pipeline resets each and262144/1048576 final bits;
nonzero prior-column and same/other-column old-before-write hazards included.
Two wrong-address/old-value controls fail. All eight cache/successor/layout
array combinations and matched cached/uncached defaults pass1506 cycles,
463 writes and final physical bits. The revised reset fixture invalidates
a fully warm cache and holds a proven physical miss through reset: changing
addresses alone had not guaranteed a miss, causing the cached default to
wait forever. Fresh builds reproduced that fixture timeout and pass its fix;
initial incremental segfault retained without claiming a proven cause.
Full MSA cached DDR14/16 and independent physical XSDS16 pass all2,359,296/
9,437,184 bits,7872 writes/1111 output transitions/6986 stored ones.
Every7872 ordered write/payload per owner and1499 final nonzero raw words
match the captured baseline. SDRAM fixture reads3008->2256, host cycles
122779->115685 (5.778% fewer); synthetic only, not a real Cedar speed claim.
Default-OFF full MSA is unchanged. Opted native DDR/SDRAM wrapper lint passes
with actual monitor/placement/50MHz macros; missing RX macro first attempt
was a lint invocation issue, retained in evidence.

Isolated Quartus Native45 at `/home/alans/mister/Dorado_Native45_20261018`
clones44; only group frontend +16 msa group parameters/manifest and generator
helpers changed, no added clock/placement/timing cuts. Build84242 completed.
Fit32973ALMs (79%)/54681 registers/330M10Ks. All31 summary checks and36
path categories across four device conditions, including SDRAM IO, PASS
minimum+0.017ns,0 illegal/unconstrained internal clocks, no new timing cuts.
Audit21038 completed; do not restart it. RBF SHA256
9481aba01b4ae5e0404cb79e537171527b2b41a36e76ac682f784fc5c03477a7;
local build/rtl-executive/native45-candidate/Dorado.rbf. Source comparison
checks all362 frozen files: only group frontend,16 msa group parameters,
manifest and native generator differ; one uncompiled storage-generator helper
added. Clock SDCS byte-identical and QSF identical after path normalization.
Not deployed:14 remains41c. Source/simulation evidence storage-defer; local
`storage-defer/` and `native45-build/`. All42/43/44 comparisons remain untested
on FPGA. Do not promote45 from synthetic results or its timing pass without real
pinned Cedar/performance/input/persistence qualification.

**Native44 group-interleaving candidate: simulation AND all-device timing PASS,
not deployed.** Optional INTERLEAVE_GROUPS=1 remaps only raw backing addresses,
leaving original chip-local ports and pipeline/cache/map/ECC intact. Default
OFF; generator marks `group-interleaved-v1` in opt-in manifests. Native43's
successor selection stays enabled,50MHz/SYSPER4/no raw line cache. 16 arbiter
matrix cases plus native128-group/16-bit-address/23-bit-raw case pass independent
integer packing/order/payload/stall/late-arrival checks and51 reset cases;
owner-bit alias control fails. DDR three-group arrays/reset and full MSA16K/
64K-chip cached cases pass; the small random array gets522 reads versus509
before, so interleaving is not a universal cache improvement. Independent
physical XSDS full MSA: all9437184 bits/12012 logical cycles/3008 reads/7872
writes/1111 outputs match; host cycles144461->122779 (15.009% fewer, synthetic).
All10880 canonical operations/order/replies and1499 final nonzero words are
identical across actual layouts. Default12 generated files remain byte-identical
toc83d2261 for DDR/SDRAM; all four option/backend wrapper lint cases pass.
Fit29706ALMs/330M10Ks. All31 summary checks and36 all-device categories,
including SDRAM input/output, pass min+0.099ns;0 illegal/unconstrained internal
clocks, no added timing cuts. Only expected wrapper/manifest/arbiter/qsf-path
input hashes differ from43; architectural boards/clock constraints identical.
RBF SHA `e433d74655c66555889b1ea2502d8694ff76b2f6549f24d2d6f7302baa0d60db`,
local `build/rtl-executive/native44-candidate/Dorado.rbf`. Quartus/all-corner
jobs95449/5420 are completed, do not restart. Reports native44/storage-interleave.
No FPGA speed claim yet. Preserve DDR42 and successor43 comparisons before
loading44; old group-major raw dumps cannot be interpreted as this new layout.

**Further raw-memory locality evidence (offline only).** Optional
`+raw_trace=PATH` in tb_storage_board records completed raw operations plus
an independent final-state oracle from the physical memory model. Traced
and untraced full MSA/XSDS runs retain all9437184-bit equivalence checks and
identical12012 logical/144461 host cycles. `tools/analyze_storage_locality.py`
replays all10880 operations (3008 reads/7872 writes), checks every captured
read and final1499 nonzero raw words/6986 set bits. All captured reads in
this fixture happen to be zero: the first replay-only draft failed to detect
dropped writes, so final-memory validation was added. Discarded-write and
corrupted-read controls now fail independently.
Current128-line64-bit direct-buffer software replay: group-major16 read hits
versus interleaved1508; line fetches2992->1500. Equal-capacity LRU gives the
same result. Uncached XSDS row locality (no refresh/reset model):0/10880 row
hits/misses versus5447/5433 when group bits occupy the low address bits.
This is a synthetic single-MSA trace, not actual Cedar traffic, an RTL
implementation, a latency model or an FPGA speedup. After42/43 measurements,
evaluate an optional bijective group/word backing-address permutation, with
raw/array/board/read-before-write/ECC/reset tests, real Cedar locality and full
timing before loading. A SDRAM burst/read buffer is a separate change.
Evidence and reproduction commands: reports/2026-10-18-storage-locality.

The matched DDR42/optimized43 RBFs and timing manifests are verified/staged
in `/tmp/dorado42-candidate` and `/tmp/dorado43-candidate` on14, not loaded.
Local42 launch/measurement helpers are prepared in `cedar42-launch`; inspect
before execution, and preserve41b's qualified input/persistence evidence
first. No new disk seed has been copied while the live installer writes.

**Disk-worker prerequisite qualified, not integrated.** New
`fpga/mister/main/main-offload-260912.patch` adds a nonblocking eight-slot
enqueue API to the pinned Main worker and rechecks the predicate after an
empty condition-variable wake. Linux and actual MiSTer ARM tests cover a3s
callback, full/empty/stopped handling, FIFO stop/join drain and poll heartbeat.
ARM max enqueue0.020ms/heartbeat gap2.385ms; blocking enqueue fails100ms and
the old wake predicate fails the Linux empty-wake control. These are generic
worker tests, not file-I/O injection or integrated Main responsiveness.
No new Main is loaded;1fb2 still performs blocking O_SYNC disk work.
Actual source review locates `fpga_load_rbf`, `app_restart`/fork and reboot
in fpga_io.cpp: app_restart already joins the worker, reboot only syncs,
and an init-only drain occurs after FPGA programming. Future disk code must
reserve capacity before consuming/ACKing SPI write data; own buffers and
old-image descriptors; use Main-only read completions and generation checks;
keep read/write/neighbor coherence; retain failed dirty data; drain coalesced
and submitted work before remount/programming/fork/reboot. Details/commands
in the Main README; reports/2026-10-18-offload records exact sources and tests.

**Previous Native41 SDRAM:** all34 Basic loadees loaded; BootEssentials
continued after a documented ARM-only BSP cursor repair. This boot was assisted.
Main11453/server11445. Pack/logs/restart config in `Cedar41_20261018`, launcher
`Cedar41.mgl`. RBF SHA
`a97af93667bf20b2f9bebb0959dc67a4710895f6416e91987242e7486a1f9297`.
50MHz/SYSPER4,8MiB guest/16MiB raw backing on the required128MB SDRAM,
no raw line cache. Main1fb2 unchanged, corrected quiet+BSP daemon89007902,
pinned Pup time1792324800; original f3bce2a8 seed. Real firmware/BFN113
at~17:24UTC, actual Cedar login~17:26; no imported state, host completion of
guest instructions/IOCBs or live server-mode repair during startup. At18:22:13UTC,
the old daemon's producer cursors were repaired as detailed below. C watcher
finished and released C; all boot/typing keys up. Native41 hardware has
passed original boot/login and Guest input, **not yet full Viewers/persistence**.
All31 summary/36 all-device categories including SDRAM I/O pass,min+0.114ns;
zero illegal/unconstrained internal clocks and no new exceptions.
Fit29888 ALMs(71%)/330 M10Ks. Reports/input hashes in native41 evidence;
local bitstream `build/rtl-executive/native41-candidate/Dorado.rbf`.

Three10s idle-login windows measured1,595,808/1,595,612/1,595,611 issues/s,
mean1,595,677/s =9.574% of the original nominal16.667M/s. This configuration
is10.20% above39b's1.448M/s.41 differs in raw cache and F10145 placement,
so this is not an isolated SDRAM-device speedup;42 is the matched control.
Held/start0.1825%, board clocks/issue2.00364. Separate windows:25.5765%
enabled,74.4241% storage-only pauses,0 pack-only/overlap,~3.036M physical
reads/s,0 backing hits. Physical READ->return estimate~4.0003 fabric clocks
from separate stable windows (DDR~9.94), not per-request timestamps.
Three windows in one boot are not three independent cold runs; issues are
not pipeline-drained retirements. Full JSON/capture evidence is native41's
`idle-*`. The measured improvement does not satisfy full-speed acceptance.

HDMI was initially black after load despite a clear native screenshot.
The same Main-only framebuffer enable/disable workaround restored actual
1080p30 capture; no FPGA/capture reset. Logs and before/after captures are
retained. The recurring Main reinitialization defect is still open; do not
claim it fixed by the extended-modeline parser patch. The first restored
capture contains a transient mode overlay; `guest-hdmi.png` is clean.

**Superseded by41b:** the old daemon hit the same race again at
`tx_next=0017b520,last_ack=0017b534`, so further cursor repairs were abandoned.
Old server stopped for41b; old pack preserved, complete server log is
`Cedar41_20261018/server-before-native41b.log`, complete Main prefix is
`Cedar41_20261018/main-before-native41b.log`. The open Main log moved to41b.
No current input/GDB uses the retired41 session. Original next action was
to leave41 running through BootEssentials/fonts and qualify
Viewers/mouse/exact keyboard/disk. At~17:32UTC real `Guest\n\n` at
0.1s down/up produced Guest.pa authentication, Grapevine-down fallback and
Basic boot's expected missing `<Guest>6.1>Basic.Loadees` then default `[Cedar]`
lookup. The profile question was answered with `n` alone at~17:44; screenshot
confirms No. No pending input scripts or GDB. Current debug view is memory12 (raw read/write requests),
so Main's cpu= field is NOT a CPU PC. Restore normal CPU view through Main
before interpreting PC/status samples. Collectors: local `cedar41-launch/`
contains `sample-login-views.py PID LOGFILE` and `select-issued.gdb`;
first two post-detach samples are discarded. Main stays sole SPI owner.
After this boot, qualify cache-off42's matching idle workload and cold-remount
36's marker volume. Preserve36's guest-mutated pack.

**BSP race fixed in source and ARM-tested, not yet used for a fresh FPGA boot.**
Native41 stalled at PrintTV after AMEvents completed. The old server rewound
its producer on a duplicate ACK, then accepted the delayed forward ACK:
`tx_next=0014434b`, `last_ack=00144f94`, unsigned outstanding4294964151.
This blocked PrintTV metadata indefinitely. New bounded stream history keeps
producer progress and retransmission cursors separate, including metadata and
completion marks. Native/ARM boot/quiet/STP, dropped metadata/body/tail,
delayed/stale/future ACKs,32-bit wrap,200960-byte ring wrap and closed-slot
reuse pass; the old native binary fails the exact next-file regression.
C unit/snapshot fidelity and forward determinism pass; snapshot ABI unchanged.
Legacy restore discards later host history; full serialization remains4a.
The tested ARM binary SHA is
`e237dffc8dfcb446b51d417a8cbb63bcf52812a861b75f16c4454bdaffda6f94`,
staged at `System_20261018_BSPHistory/dorado-server-e237dffc8dfcb446`.
Do not restart the live old daemon mid-connection: it would lose state.
At18:22:13UTC assertions verified exact old cursors/socket/file/context, then
changed only ARM `tx_next`/`tx_last_end` (working state and slot4) to the already
ACKed00144f94. PrintTV.df/PrintTVImpl completed and AMTypes followed.
No guest memory/CPU/IOCB was changed. This is component qualification only;
next fresh boot must use the fixed daemon and require no assistance.
Primary trace, GDB reads/assertions/repair, screenshot, commands/hashes and
positive/negative test logs: `reports/2026-10-18-bsp-history/`.

**Native43 is ready for hardware qualification.** Commitc69d93f9 adds optional
response-boundary raw-arbiter successor selection, default OFF. It preserves
one outstanding operation, round-robin order, chip pipeline, data/cache/map/ECC
semantics and clocks. Full MSA/independent SDRAM fixture matches all9437184
stored bits,12012 logical cycles,3008 reads/7872 writes/1111 output changes;
matched host edges154349->144461 (6.406% fewer in this synthetic fixture).
Independent fixed-latency streams save exactly one bubble per successor;
stale-origin and disabled-optimization controls fail. Both default DDR/SDRAM
generations stay byte-identical toffd7e70f. All31 summary/36 all-device timing
categories including SDRAM I/O PASS,min+0.117ns,0 clock-coverage gaps/no new
cuts. RBF SHA
`2f7511b8629f5bef6135be7022bf14e1d8f8638ef8b1ad9d7cd16d2e3d15593f`,
local `build/rtl-executive/native43-candidate/Dorado.rbf`;29758 ALMs/330 M10Ks.
No FPGA speedup or cold-boot result yet. After41 component qualification,
measure42's matched DDR control and43 with the fixed server; repeat pinned
comparisons before claiming an end-to-end improvement. Evidence:
`reports/2026-10-18-storage-prefetch/` and `reports/2026-10-18-native43/`.

**Native39b measured actual issues at the Cedar login.** Same50MHz/SYSPER4,
Main1fb2, corrected89007902 daemon, pinned time, original seed. Three10s
windows:1,447,940/1,448,181/1,447,903 issues/s, mean~1.448M/s =8.69% of the
original16.667M/s nominal rate. Held/start~0.177%, board clocks/issue2.00367.
Separate windows show23.2067% enabled,76.8169% storage-only pauses,0 pack-only/
overlap;~2.754M physical reads/s; backing read-cache hit~0.039%.
Separate views are not one simultaneous snapshot; issue is not pipeline-
drained retirement. Three windows came from one idle login, not three boots.
Full records/capture: `reports/2026-10-18-native39/idle-*`.
The known15 microprogram tests (SYSPER4/8/16, same-PC/hold/extended IM/TPC)
verify the count against100 real T increments. Maintenance/reset/task-switch
qualification remains open.39 uses128 raw read-cache lines and MLAB tiny
files;41/42 have no line cache and F10145 logic placement. Use42 to isolate
the backing device.39b logs/pack preserved in `Cedar39b_20261018`, Main log
copied to `main-before-native41.log` before replacement.

**A server-build regression held39 at fileInitialized860.** The installed
1d5e05e9 binary hashes exactly to Ubuntu `Dorado_ServerRoute_20261017/
dorado-server-quiet-arm`, source `server_quiet.c`. The new d4 BSP-probe build
used repository server.c and lost its BFN113 broadcast guard. Period
`PupRouterImpl.InitializeRoutingInfo` drains gatewayInfo until Get's1000ms
timeout; repeated unsolicited gateway packets kept that drain loop open.
Both the initial warm39 and fresh39b showed one GatewayInfo request and
then BusyWait/no time/name request, with clean pack/CRC/RX counters.

Quiet mode was restored by replacing ONLY the ARM daemon on39b at17:13UTC,
then restoring its already-observed BFN113 metadata before registration,
using `restore-server-mode.gdb`. FPGA/Main stayed running; no guest CPU,
RAM or IOCB writes. Within~30s Time206/address223 requests and real Cedar
Please login/Name appeared. That repaired boot is not an unassisted
whole-boot time benchmark. Native41 now boots the corrected service fresh
and must learn113 from the real request, with no GDB server-mode restoration.

Correct daemon: `/media/fat/games/DoradoRTL/System_20261018_QuietBSP/
dorado-server-89007902b6190496`, SHA
`89007902b619049633233143b133246db9c9ba2a062937bf8ef71eb13dabdacf`.
Native and ARM tests pass boot/EFTP/reexec, BFN113/routing, quiet3.2s,
replacement-Main broadcasts restored,66,296-byte STP and1036-byte dropped
completion-tail recovery after two automatic probes in actual Cedar quiet
mode. Previous flood binary FAILS the new quiet test. Shared C state layout
unchanged; no general metadata retransmit ring. Evidence:
`reports/2026-10-18-quiet-bsp/`. Keep this policy in source; do not rebuild a
service from an untracked quiet variant again. BSP probes remain enabled
in quiet mode; only unsolicited boot/routing broadcasts stop.

**Native42 matched cache-off DDR control is timing-qualified, not deployed.**
Frozen Ubuntu `Dorado_Native42_20261018`, same41 boards, clocks, F10145 logic/
F10415 M10K and instruction views. Only raw-backend wiring/constraints differ;
wrapper-prefix/suffix assertions check upstream identity. All29 summary
checks/20 categories across all four conditions pass, min+0.115ns, zero
illegal/unconstrained internal clocks, no added exceptions. Fit29788 ALMs/
330 M10Ks. RBF SHA
`45af2e303267c4583480b29464720b0892a06cbf82881fa00ba1634eab0b8c55`.
Reports/recipe/provenance: native42. Bitstream:
`build/rtl-executive/native42-candidate/Dorado.rbf`.
The no-cache DDR regression passes byte lanes/stalls/backpressure/reset drain.

**Native36's assisted desktop/input and local file remain preserved.** All34
packages, BootEssentials/fonts, Viewers, mouse/click and exact `rtl36ok` plus
alphabet/digits passed after legitimate BSP probes replayed a lost20-byte
Helvetica7 completion tail. The first diagnostic probe had omitted a socket
high half and correctly got NoSocket; corrected full32-bit probes recovered
it without guest patches. Automatic probes now replace that assistance.
`echo rtl36persist20261018 > ///rtl36persist.txt` created/closed a real local
file; `type ///rtl36persist.txt` read it back. Cold remount is pending.
Keep `Cedar36_20261018/cedar-work.pack`; logs/captures and final qualification
are in native36 reports. At16:29UTC good1979/dropped79/CRC0/RX0/aborted0.
Native34 remains the last unassisted full Viewers/input reference.

Selectable SDRAM simulation qualification remains intact: independent chip
pins/20..50MHz/6ns clock-delay controls, ranks/banks/rows/refresh/backpressure/
reset-drain, and all9,437,184 MSA stored bits/output behavior match. Native37
failed physical input setup;38's falling-edge capture fixed it but tiny
F10145 MLAB holds still failed.40/41's logic placement preserves behavior
and closes all-device timing. No failing build was deployed. Remaining goal:
41/42 hardware comparison, request locality/serialization, responsive ARM disk,
DDR frame rings, complete snapshots (including disk/server state), accurate
phase/timer migration, repeated fresh boots/input/persistence and2h sustained
qualification. Snapshot milestone4a requires a full state inventory and a
representative-board area/timing gate before scaling state access.

Native35's original firmware/BFN113 and Cedar login pass. Separate10s idle
windows measure23.2104% enabled,76.7903% DDR-only pauses, zero pack-only or
overlap pauses,0.03810% raw backing-cache hits, and~2.75M DDR reads/s.
No DDR command-busy cycles were observed; read-return pending occupied~54.74%
of host time in its window. These are different windows, not one simultaneous
snapshot or retired throughput. See reports/2026-10-18-native35/idle-analysis.json.
Remaining pause overhead includes frontend sequencing; do not infer all of
it is DDR hardware latency. Native36 desktop/input passes after the
documented BSP recovery; unassisted cold-boot qualification remains open.

**Native34 at50MHz/SYSPER4 passes one full original-firmware Cedar boot and
interactive input test on MiSTer14.** RBF
`0ebe68f456ce46e09b813dd126b605c083b606176ce0bdc797c9f3194f27bbbd`.
All34 base packages, BootEssentials, Tioga/Press/font metrics and Viewers
completed. Fresh BFN113 at05:16UTC; fast Guest input passed; profile question
answered with only `n`; desktop by06:02. Real mouse motion(+100,+80), Cmd
button opening a second CommandTool and body focus worked. At06:04–06:05,
0.1s down/up keyboard bursts received and echoed EXACTLY `rtl50ok` and
`abcdefghijklmnopqrstuvwxyz0123456789`, then returned to `%`. Original
native and HDMI captures are in reports/2026-10-18-native34/. Verification
is visual; OCR was inaccurate and is not a gate. No C-state import or host
completion of guest I/O. All scripted keys/buttons released; OSD closed.

**Speed is still low despite the higher fabric clock.** Unpaused ceiling
6.25M microinstructions/s(37.5% of original), versus4.167M at33.333MHz.
Font attachment sampled19.8033% enabled edges; interactive desktop with
startup Bringovers still active sampled19.8755% at50.0134MHz. The latter
implies a ceiling1.2422M/s(~7.45% original), NOT retired throughput or a
controlled A/B speedup. The old quarter-speed description omitted global
storage pauses. Coarse1Hz status observations point mainly to DDR waits;
next add exact cause-duration counters, then reduce measured memory traffic.
Keep phase/enable migration and MC12061 calibration as separate required work.
Do not benchmark from the herald's guest clock or merely lower SYSPER.

Fit13m52,83%ALMs/307M10Ks; all29 summary checks plus20 timing categories
across ALL four supported device conditions(fast/slow,-40/100C) pass, min
+0.101ns; zero illegal/unconstrained internal clocks, no new exceptions.
HOST_CLOCK_HZ50000000/HOST_TIMEOUT_CLOCKS200000000 preserve4s. Original
CPU/microcode/serial clocking retained;128-line raw DDR cache/no cold-read
bubble. General builder still defaults33.333MHz/cacheOFF; Native34 is the
qualified explicit configuration, not a proved cache performance win.

Native34 used Main24180/server24176 with immutable1fb2.../1d5... binaries;
sole Main SPI owner. Launcher `_Computer/DoradoRTL_Cedar34_20261018/Cedar34.mgl`;
fresh f3bce2a8... seed; retained pack/logs/reboot config in Cedar34_20261018. The
active pack is now guest-mutated. First CommandTool still runs optional
startup Bringovers; second tool is at `%` with test output. Some welcome
app `.load.bcd` lookups report missing files; application-package cleanup
is separate from the verified base desktop/input and clock work.
User releases sessions for reloads; only14 allocated.

**The completed two-hour passive observer captured long ARM disk writes.**
It stopped at its deadline (~07:10UTC), with no pack fault. The full trace
has a4.730966s maximum Main heartbeat gap. Repeated syscall/fd samples show
the same7680-byte O_SYNC write to the active pack pending for at least2.80s;
this is actual host blocking evidence, not proof of the earlier Native32
fault's cause. The earlier06:08 prefix missed these later stalls. Full log
and extracted samples are in reports/2026-10-18-memory-placement/.
No SPI/ptrace/ACK/reset/guest writes were used for this observation.

**All target MiSTers may require128MB SDRAM (user confirmed).** Current
main RAM is8MiB /4M16-bit words, backed by a16MiB padded data/ECC region in
shared DDR; external SDRAM is unused. Move bulk main RAM to a selectable
dedicated SDRAM backend after measuring controller latency/serialization.
Keep hot map/cache/microcode on chip. Native34 uses307/553 physical M10Ks
but83% ALMs: IM and IFU currently consume large MLAB allocations. Native36
tests their already registered F10415A arrays in M10K; no added RTL latency.
Native35 adds passive DDR/ARM pause and traffic counters, same Main protocol.
Both candidates pass29 timing-summary checks and20 categories across all
four device conditions, zero illegal/unconstrained internal clocks and no
new exceptions. Min slack35:+0.088ns,36:+0.068ns. Native35 uses85%ALMs/
307M10Ks;36 uses66%ALMs/332M10Ks. Native36 is now booting on hardware as above.
Native35 replaced34 at~14:27UTC with a fresh f3bce2a8 seed and unchanged
Main1fb2/server1d5 binaries. Original firmware/BFN113 and Cedar login pass.
Server5780/Main5785 (Main re-executes on load); Cedar35_20261018 logs/reboot
config, launcher Cedar35.mgl.
RBF SHA f223890bde59b518b5d4ffcc268542481973b2424c9f34bef578dd1db61e28c9.
The real-C watcher released at the fresh BFN113; all scripted input is up.
Idle pause/traffic measurements are captured; Viewers/input are not yet
retested on35. Placement/SDRAM sequence, disk write-cache reference and
diagnostic views are in docs/verilog-clock-plan.md.

Counter mode ON: normal Main `machine=` is enabled fabric edges, `cpu=`
host edges. Unsigned32bit deltas<60s(wrap85.9s), not retired instructions.
Fault retains priority. Native35 Debug monitor now cycles16 views rather
than toggling two. Private Main status-setter sampling scripts use the
matching `/tmp/dorado-main-1fb2.elf`, break at input_poll on idle login and
discard two post-detach observations; no second SPI owner. Final view is
Clock counts. No pending input scripts.

HDMI1080p30/74.250007MHz, native1072x862/source guest1024x808. At idle login
normal Main host-framebuffer enable/disable restored initially black HDMI.
Fresh physical query used one normal video_mode_adjust(true) atinput_poll
on immutable1fb2(function0xa8881/global0x9715ec; vtimeh offset24). Do not
reuse binary addresses on another Main or pause Main during disk I/O.
Internal screenshots copied after file completion; partial copies were
re-copied before inspection/archive. Clock count mode and guest retained.

**Native32 hardware qualification FAILED before Viewers.** Preserved fault
`807c0200/47fc/000049cd`, reason8/cylinder124/head0/LBA18893, first observed
04:36UTC at27th retrieve(ReadEvalPrintImpl.bcd). Packet CRC/RXbad stayed0.
The fixed133333332-cycle watchdog shrank4s ->2.6667s at50MHz: a proved
clock-scaling bug. The actual failure's host latency/ACK phase was not traced,
so causal diagnosis remains open. Main returns SPI data ACK before O_SYNC
file write returns; a later request could wait behind that write (hypothesis).
Native32 volume is guest-mutated/incomplete; do not reuse as a good seed.
Main-at-first-fault.log, context, volume and observer validation retained
in Cedar32_20261018 and reports/native32/. Filter inherited Main log after
LAST Cedar32.mgl marker; older faults precede it. All four timing conditions
also passed for32 in the expanded post-fault audit; no static timing failure
found. Native33 fit13m56 completed but retains unscaledwatchdog; superseded.

Native34 keeps4*physicalHOST_CLOCK_HZ from builder's SAME PLL selection,
with matching metadata/QSF macro. Host-fault low6bits now are
{host_state[2:0],sd_rd,sd_wr,sd_ack}; serial faults retain wr_bits. Extra=LBA.
States1/2 write ACKassert/release,3/4 read ACKassert/release. No completion
invented. Scaled333/400/500Hz tests accept3s service and faultmissing/stuck
ACKat4s preserving first phase/LBA; fixed-budget mutation failsat500Hz.
CRC/Pilot persistence/neighbor and build configuration fixtures pass.

Native32 passed firmware/login/fastGuest; fresh actual1080p30/74.25003MHz
HDMI needed no workaround. Firmware enabled25.0156% vs26.4617% uncached;
earlyloader18.5936%, Imager20.0104% vs16.9255%. These are observations,
not controlled whole-system speedups; they combine DDR/HPS/global pauses.
Native31 passed all34loadees/BootEssentials before replacement. Native26 is
the previous Viewers/mouse reference; its Echo tests interleaved user typing.
Exclusive exact CommandTool Echo remains pending. Keep further optimizations
can now proceed from Native34's verified desktop, with fault observation retained.

**Clock implementation has started.** The staged plan is
[verilog-clock-plan.md](verilog-clock-plan.md). An isolated direct phase/enable
prototype matches an independent original four-flop divider, preserves the
1/8 lead and3/8 low window, freezes at every phase, and captures pre-edge
pipeline data without the legacy one-tick delay. It is not wired into any
machine. The eight-slot scheduler is a timing oracle, not a full-speed
architecture (literal full-speed implementation would need266.667MHz).
ProcL/ProcH T-control decode/capture now passes8192 comparisons against an
independent original gate cone,8164 legal HM3.8 checks and2048 hold cases;
a universal-enable mutation fails4096 cases. Original wire-list checks
confirm h22 clock=Clock0'Da versus d17 clock=SHCP'C and swapped R-select pins.
The experimental controls require correct independent enables; their
source wiring is now checked (33 connected/15 unwired pins): high clock
is CLK.ph' OR CLKEnable'c OR StartCycle'a; low clock is inverse l02.Q,
which samples StartCycle'a on rising(CLK.pl' OR CLKEnable'b). Edge-enable
integration and the complete T data/latch/task-file path remain unimplemented.
No board/firmware qualification is claimed for this replacement.

Native30 was the separate40MHz diagnostic control; its prolonged fit was
stopped after Native31 passed50MHz timing/login. Frozen sources/logs retained;
only its exact fitter PID3641523 was terminated. Native32 fit was untouched.
Native31 includes its passive counters/coherent UIO plus the balanced arbiter.
No Main/protocol change. Independent cyclic-scan order/payload/stall and
full storage equivalence pass (all9,437,184 bits). See the clock plan.

Cache/no-bubble independent regressions remain sound:458-transaction
scoreboard, busy/zero-latency/backpressure/reset/orphan controls and complete
MSA14/16-bit outputs/every2,359,296/9,437,184 bit pass. Default cache option
remains OFF pending a controlled performance comparison. Native34's host clock correction is independent of these data-path changes. Native34
now passes desktop/input; next measure exact pause causes before optimizing.

## Boot history (current status above takes precedence)

**Native25h cleared the media blocker; Native26 is now booting on MiSTer14.** The original BaseBoard firmware requested
BFN113, disk-loaded Cedar started, and the actual FPGA raster displays
`Please login` / `Name:`. Keyboard injection of `Guest\n\n` reached Cedar:
the server received the real Guest.pa authentication request and the display
shows Guest with the Grapevine-down fallback. This is real RTL CPU, disk and
serial Ethernet; no C-state import or host IOCB completion. Evidence:
`fpga/mister/reports/2026-10-17-native25/fpga-first-os-check.png`,
`fpga-after-guest.png` and `fpga-post-login.png`.

DskEth k04 B now samples Input setup before clk1 (`bedb99cd`). The old
post-edge behavior misses retiring Input or consumes a newly enabled Input
early. Both mutation-sensitive setup regressions and the full held-word /
CRC / EOP sequence pass. Native25 advances through routing, time and name
service on hardware, where Native24b retried. The exact physical receive
buffer has not been recaptured coherently; do not claim its bytes verified.

Native25 cold DDR120M passes: original Initial/Cedar images, germ entry,
8,931 sampled germ words / zero mismatches, IFU670280/670434 and all six
hardware errors enabled/clean for106611683 samples. Timing passes all29
checks (minimum+0.071ns), no illegal/unconstrained clocks. Exact MemX c21
own write-address hold-only exceptions are documented in the report. Its
geometry is still2MB RAM/16K Map.

**Post-login blocker identified:** LoaderDriver cannot open either
`Basic.Loadees` path: `File.FP from directory/cache doesn't correspond to a
local volume file`, then `Can't find any command file`. This exactly matches
the known synthetic-PDI zero-DA-hint defect documented in docs/handoff.md.
C's `DORADO_PDI_IGNORE_LABEL_FLAGS` bypasses both hint words; real RTL checks
them. New `tools/pdi_stamp_file_hints.py` corrects a separate offline copy:
45 labels for VAM/client files, logical hints1/1115 derived from first headers
and independently checked against logical-root FPs. Every file data byte,
other label field and boot-only file remains unchanged; idempotence passes.
Strict C control also exposes the existing attribute-clear workaround; it
cannot independently qualify hint-only repair without separating the two
compatibility behaviors. Diagnostic C build enforces DA while retaining the
attribute workaround: it retrieves Basic.Loadees plus16 files by2.5B BB
cycles. Strict unmodified C still fails its own attribute-clear comparison.
An identical diagnostic run with the uncorrected medium retrieves zero
files and reproduces the FP failure. Do not confuse these C results with
FPGA proof.
Native25h is now deployed with the identical qualified RBF and corrected pack;
original firmware emitted fresh BFN113 at22:27 UTC and C released.
Actual Cedar login reached22:28; Guest accepted. At22:29 the real OS
reached STP lookup/enumerate for Guest/6.1/Basic.Loadees (expected missing)
then retrieves the release copy from Cedar6.1/Top. At22:31 STP_TRANSFER
confirms Basic.Loadees, RPCRuntime.bcd and UserProfileImpl.bcd. The old FP
error is cleared on real RTL. Subsequent loader/capacity progress remains open.
No live guest-memory repair. Viewers/%/Echo/mouse remain open.

Native26 expands to8MB RAM/64K Map and retains the receive fix. Fit and
reviewed timing pass29 checks (minimum+0.074ns), no illegal/unconstrained
clocks. Exact DskEth d08/e16 and MemC g02 same-cell address holds are
reviewed. It is now deployed as the separate hardware control with a fresh corrected pack. The larger-memory cold prototype is
still below the source-sampling coverage gate; it is not qualified.

**New blocker at22:31–22:33:** Native25h paused with state47fc:
pack_fault=1, pack_ready=0, machine_enable=0; CPU233 frozen. This is a
medium fault, not an observed RAM-capacity limit. Stopped pack has91 changed
sectors on tracks0/3/43..47. Basic.Loadees (2361B) and RPCRuntime (18432B)
are byte-exact against served sources. UserProfile's39 data pages were
allocated, but only its first1024 bytes match; its transfer is incomplete.
Do not call two completed modules. Source/label/header writes have worked;
serial versus host fault is not yet distinguished. Preserved files:
`cedar25h-launch/pack-diff-at-fault.json`, `raw-ddr-at-pack-fault.bin`, remote
`Cedar25h_20261017/{main.log.at-pack-fault,server.at-pack-fault.log}`.

Native26 (same media/controller,8MB/64K geometry) is now a separate hardware
control; original firmware/BFN113 completed at22:46 UTC; C watcher64124 released.
OS routing request/reply and actual Cedar login observed22:47; Guest
was accepted through real keyboard. By22:52, loader requests11 files
through WorldVM, passing UserProfile/Idle/DFPackage/BasicPackages/Installer/
Inscript/VersionMap. By22:58, Rosary is loaded and the guest is requesting
TiogaPackage (287232B);24 transfers including Basic.Loadees have started
by23:00. At23:03 all34 transfers including Basic.Loadees and its33 BCDs
through Viewers/End.bcd have started. Final file completion/installer/desktop
remain to observe; pack_fault remains0. User notes simulation also loads
slowly and snapshots bypass this phase. FPGA ceiling is4.167M microinstr/s
(0.25x Dorado) before DDR/HPS pauses; elapsed load time alone is not a fault. Its cold
copy-sampling gate remains open (8880 words,0 mismatch, below8900 threshold).
Native27 adds only passive first-fault observation to26: reason, cylinder,
head, block/state/bit and CRC residue or LBA. Component bad-CRC and host-timeout
probes pass without inventing completion. Initial fit22467 rejected carried post-fit SDC cardinalities before placement.
Post-fit SDC was removed for fitting. Fit71756 was then stopped before
RBF to add BaseBoard debug when Run' is set. Final map/fit restarted via
`native27-candidate/remap.py`, lognative27-remap.log. Fit/asm/STA now finish;
reviewed29 checks pass,min+.115ns, zero illegal/unconstrained clocks.
Exact ProcH l03 own4 bit1/MemX e20 own2 bit2 hold-only review retained.
Not deployed; keep Native26 installer running.

## Video investigation and installer progress (2026-10-17 23:16 UTC)

Actual Native26 screenshot confirms every33 BCD loading line through End.bcd
reports `ok`. Startup reached InstallerImpl and waited for `Do you wish to
install a personal profile? Yes`. At23:08 `No`+Return through the existing
keyboard advanced to real BootEssentials BringOver (User.Profile, SafeStorage,
File, FS etc.). This is a prompt, not a slow-loader hang. pack_fault remains0.

The original display **is 2:1 interlaced**, HM printedp115 / PDFp122,
`DoradoDocs/dorado-hardware-manual/Dorado_Hardware_Manual.md:5612`:
"Only 2:1 interlaced monitors are supported in this design". DispY's actual
OddField latch drives VGA_F1. HDMI should present a stable progressive image.

The active720p60 configuration vertically downscales and keeps one source
field: scaler header width900,height431. MiSTerKVMS HDMI captures show
alternating incomplete text rows. Read-only scaler captures contain both
field IDs, with clean status; interleaving these fields makes text readable
(analysis composite only). Live1080p60 makes Main's **unmodified** internal
capture1072x862 and all text readable; evidence in Native26 report directory.
But HDMI capture goes black on the live mode change. Returning720p timing
and host HDMI transmitter reinit have not restored external capture as of23:16.
Do not claim video fixed or call the black capture guest failure. The1080p60
clock was a diagnostic outside the fixed74.25MHz qualification, subsequently
restored. The1080p30 diagnostic **requested**74.25MHz; later measured-refresh
evidence does not confirm that clock (see correction below). Main video_init subsequently restored original
720p preset/VIC4 after invalidating only its host HDMI cache. INI unchanged;
no core reload. Source still gets720p field reduction. Explicit core-framebuffer selection
and a labelled Main OSD diagnostic also remain black externally. External HDMI remains
black even after transmitter power cycle, full host video reinit, capture14
reopen and supported capture-format renegotiation. User confirms viewer black.
ADV7513 reads PLL9e=12, detectedVIC3e=10 (720p60), channelsa1=00,
AVmute4b=00, power41=10, VIC3c=04; these show clock/timing lock but
do not prove active pixel/DE correctness. Other two capture streams never
changed/restarted. Original full capture config restored.

**Main live-parser defect:** extended21+-item modelines leave Fpix zero,
then setPLL loops at Fout0. First such command stalled Main23:12–23:14;
queued host display commands were saved/cancelled, and GDB returned only
from host video_mode_cmd at its caller using exact immutable1fb2 symbols.
No FPGA CPU/register/memory writes or reload; normal transport resumed and
pack_fault remained0. Avoid extended live modes; short9/11-item forms have
an explicit nonzero pixel clock. Neither running executable was replaced.
A later GDB call at host input_poll boundary reinitialized only Main's HDMI
transmitter; external stream still black. These host interventions must be
included with boot evidence. Native26 guest continues BootEssentials BringOver; later screenshots show
AMTypes/Viewers/Interpreter/ViewerIO/terminal and debugger closure progress.
Installer warned it could not find PrintTV.df, then proceeded with default
continuation. The file IS present locally and on the ARM (2204B), and current
server log has two successful FileLookup replies for it but no retrieve.
Do not call it absent; the failed guest lookup/open remains unexplained.
No new CPU or disk fault observed.

**Historical23:31 update:** internal captures again request1080p30/74.25MHz through
short11-item mode, retaining both fields; HDMI still black. Explicit Main
OSD text also black externally. Installer now fetching Tioga styles, icons,
TIP, Watch etc. Parser fix is prepared in `fpga/mister/main/main-video-mode-260912.patch`:
raw PLL form returns without recomputation; short forms reject zero/nonfinite
clock. Exact-parser regression has an original-source failure control and
passes raw preservation/zero rejection/9/11 forms. Isolated
`misterubuntu:/home/alans/mister/Main_DoradoVideo_20261017` builds with pinned
GCC10.2.1 (compiler in `/usr/local/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf/bin`).
Main SHA6358f310... candidate is **not deployed**. Running1fb2 unchanged.

MiSTerKVMS CLI is `/Applications/MiSTerKVMS.app/Contents/MacOS/misterkvms-cli`.
14 stream: `mkvm://10.0.2.128/Mister%2014`; always pass `--mister 10.0.2.14`.
Use `screenshot PATH`, `record 8s PATH.mp4`; discovery `streams` lists all
three stations, but only14 is allocated. Do not restart the shared capture
service or touch126/192. Native27 fit/asm/review complete:29 checks,min+.115ns, no illegal or
unconstrained clocks. ProcH l03 own addressbit1/4sinks and MemX e20
ownbit2/2sinks hold-only review, unchanged capture/commit exclusion.
RBF ac94263d... not deployed. Reports in native27 directory.

### HDMI recovery and font installation (23:49 UTC)

Same original Native26 boot remains active, Main PID12701 and server12696.
BootEssentials has completed;188 Tioga font names and the Press fonts have
been attached. FontMetrics.df is now attaching its .tfm names, visibly
advancing through the CM fonts. pack_fault remains0 and CPU task/PC advance.
Flat packet counts during these local name attachments are not a hang.

External HDMI recovered after Main's Linux framebuffer was enabled then
disabled at an input_poll boundary. `fpga-hdmi-recovered.png` is an actual
MiSTerKVMS HDMI capture of Cedar, with complete text rows. User confirms
capture working. No core reload, new guest boot, guest-memory write or C
state import. Do not repeat mode changes while startup remains healthy.

Host interventions preceding recovery, retained for reproducibility:

- Pause used the real Main/core Pause input. At storage_ready/pack_ready,
  CPU debug held3081c81c and all16MB raw guest DDR at0x30000000 were hashed.
  Main framework reset was pulsed; its release additionally needed the
  normal core-ID probe's GPIO00->10 handshake. Same CPU debug and raw DDR
  SHA256 `0f4bd71a03cb3a8c1553085c091796a22c1040a812874131956fa7980201a9a1`
  were verified after actual release, before resuming. Remote evidence:
  `Cedar26_20261017/video-reset-observation/{before,after-release}.json`.
  Framework reset alone did not restore HDMI. Native CPU reset is tied to
  its own PLL, while framework RESET gates machine_enable; this preservation
  is observed for this quiescent test, not a general reset guarantee.
- A host-only framebuffer test backed up8294400 bytes at0x22001000, wrote
  bars, enabled Linux FB, then disabled it and restored every original byte.
  `/dev/mem` flush returned EINVAL after the write; Linux redrew its console,
  so the diagnostic image shows Linux login/OSD, not the bars or Cedar.
  This buffer is separate from guest DDR and the core screenshot buffer.
  Backup is `Cedar26_20261017/host-framebuffer-test/host-buffer-before.bin`.
  Cedar became visible on HDMI after returning to core video.

Likely recovery mechanism: framebuffer change makes normal video polling
refresh scaling bounds, whereas live video_mode_cmd does not directly call
video_scaling_adjust. This is a source-grounded hypothesis, not a proved
root cause. The parser patch fixes the separate zero-clock hang only.

**Clock correction:** previous "1080p30 at qualified74.25MHz" statements
described the requested modeline, not a verified physical clock. Main's OSD
reports1920x1080,74.25MHz,60.0Hz: Fpix is requested metadata, refresh comes
from the100MHz hardware counter. Read-only `/proc/12701/mem` inspection of
current_video_info at23:47 finds vtimeh1666666 (60.000024Hz), source
vtime18189197 (~5.50 fields/s),1072x862,interlaced=true. This was a cached measurement, subsequently resolved by a fresh hardware
read below. Separate post-fit148.536MHz audit completed: slow100C scaler setup fails
-2.382ns (o_h_lum_pix.g[7] -> o_poly_lum[0]); hold/recovery/removal pass
at both examined models, with zero illegal/unconstrained clocks. This mode
is NOT timing-qualified. Reports are hdmi148-* in the Native26 report directory.
The original29 passing checks qualify only74.25MHz. INI remains original
720p and running Main remains immutable1fb2; parser candidate is not deployed.

### Fresh hardware timing resolves stale OSD (23:52 UTC)

Read UIO_GET_VRES(0x23) using the running Main's own normal SPI functions
at input_poll, then DisableIO/detach. No helper competed for SPI, no mode
write, no guest state write. Fresh HDMI period is3333332 ticks at100MHz:
**30.000012Hz**, consistent with2200x1125 totals and **74.25003MHz**.
The OSD/current_video_info60Hz value was stale. Thus current live1080p30
uses the original74.25MHz qualification. The extra148.536MHz scaler failure
still stands: do not use1080p60 on this image. Evidence/scripts:
`native26/hdmi-fresh-timing.{json,log,gdb}`. Core input is still1072x862,
interlaced; fresh source period20496971 ticks (~4.879 fields/s), affected
by DDR/HPS stalls. Same boot continues; font metrics almost finished.

### Viewers begins painting (23:56–23:58 UTC)

Native GUI now replaces SimpleTerminal: stippled background and hourglass,
then top herald with `Using []<>cedar.style!1 ... Using []<>basicprint.style!1`
and `Idle`. Real server retrieves Tioga10.ks,Helvetica8.ks,Helvetica7.ks.
User reports moving the mouse works. Preserve that observation, but capture
our own deliberate motion/click and CommandTool keyboard proof after the
windows appear. CPU PCs still advance and pack_fault0; no C state import.
`fpga-viewers-background.png` and `fpga-viewers-first-herald.png` are direct
Main internal captures. This is GUI startup, not yet the completed desktop.

### Interactive desktop and keyboard loss (00:02–00:12 UTC)

Actual native images: fpga-desktop-before-input.png,fpga-mouse-moved.png,
fpga-commandtool-prompt.png. Mouse moves from roughly(534,312) to(634,392)
after a real +100,+80 report. Cmd was clicked at(934,36); a second viewer
appeared and was focused at(759,636). Its `%` prompt accepts commands and
returns results/new prompts. First test at0.8s down/up and0.1s Shift setup
showed `echo FPA` -> `FPA`. Second at2s down/up,1s Shift
setup showed `Echo GA` -> `GA`. User subsequently confirms concurrent
typing `openr tiogadoc`; field-paced input visibly appended to that command.
These runs are confounded, not clean loss measurements. Do not claim
lossless typing or exact FPGA proof; user independently says typing works. Native screenshots and slow-test HDMI clip
are in the Native26 report directory.

Native keyboard decoder is inside the gated machine clock, whereas MiSTer
ps2_key events arrive on ungated clk_sys. A down/up pair during a disk wait
returns the toggle to its old value; the gated decoder sees neither.
New platform adapter dorado_key_queue captures256 events on host clock,
coalesces identical repeat make events, and exposes each transition for two
actual source fields (terminal serial transport plus retrace sampling).
Saturation is sticky/visible via user LED and debug4b51xxxx, no CPU halt.
No guest microcode/KeyBits bypass. Native26 remains unchanged.

`make -C verilog key-queue-test` passes with the real matrix decoder and a
direct unbuffered failure control, including Shift/G across different matrix
words, two-field state hold, stopped-clock capture, repeat coalescing and
overflow/reset. Existing keyboard-terminal-test and key-matrix-check pass.
Native28 isolated build: misterubuntu:/home/alans/mister/Dorado_Native28_20261018,
local native28-build.log, session20861; map/fit/asm/STA/audit completed. Based on Native27 plus
only top-level host-input buffer. Uses native26-before-review.sdc for fit;
review exact new hold paths after placement. Not timing/hardware qualified.

Speed: current33.333MHz/(2*SYSPER4)=4.167M microinstructions/s ceiling;
DDR/HPS stops make actual throughput lower. Current Native26 core setup
slack4.117ns has a15ns half-cycle relationship; fresh TimeQuest Fmax41.08MHz
does not qualify66.67MHz. Worst paths are storage-ready -> network_step,
not the ALU. Existing CLOCKGEN
N=1 is constant, N=2 collapses ECP/SCP phases; historical SYSPER2 ReadBB
failure needs fresh characterization, not a divider-only promotion. Half
speed at33MHz/SYSPER2, then full at66.67MHz/2 are investigation targets,
requiring phase correctness and critical-path closure. MC12061 timer
calibration and64-bit DDR burst utilization remain separate measured work.

### Holistic clock audit requested by user (00:20–00:30 UTC)

See docs/verilog-from-sil.md section Clock architecture audit and
`fpga/mister/reports/2026-10-18-clock-audit/`. Independent ideal divider
wired as BaseBd11's four1690s confirms ECP lead1/8clock and j02 OR
low3/8clock. Actual CLOCKGEN at4 has lead1/4,low1/4; at2 lead0,low1/2;
at1 no edges. Ratios8/16 match nominal reference. This is a simulation
reference derived from the original schematic, not measured real hardware.
Fresh paired original-ROM fixture:4 passes15 task initializations,
2611683 enabled/clean error samples,325 IM addresses;2 fails with zero
initializations/25 addresses. Saved exact recipes and summaries; full logs
`build/rtl-executive/speed/initial-rom{2,4}.log`. Do not overwrite protected
Executive model/golden; private obj_initial_rom2/obj_initial_rom4speed used.

Fresh deployed-fit STA identifies backend readiness -> falling-edge
network_step as worst setup path (4.117ns slack,15ns relationship,~9.9ns
data delay); Fmax41.08MHz. Earlier full-cycle slack/Fmax guesses are
superseded. Global pause/backend architecture and sampled cell clocking
both need review. Preserve original30ns clocks/60ns pipelined instruction
throughput, extended IM/TPC phases, pre-edge data and latch windows.
Recommended next work is explicit schematic-derived phase enables/related
PLL phases, rather than reducing SYSPER blindly. Do not replace real
clock-domain behavior with one universal enable or relax data timing.
Raw DDR single-word locality/unused lanes and whole-machine pauses are a
separate throughput problem. No clock/core change deployed for this audit.

## Active work

Scratch names below are relative to `build/rtl-executive/`.

| Work | Session | Location / next action |
| --- | --- | --- |
| Native25h hardware | PRESERVED; medium fault | Original firmware/login/Guest/Basic.Loadees and RPCRuntime byte-exact; UserProfile incomplete. state47fc/pack_fault halts CPU233. Logs/rawDDR/disk changes preserved before26 load. |
| Corrected CPU monitor | Fit/review COMPLETE | Native24b passes29 checks,min+0.080ns, no illegal/unconstrained clocks. Exact MemC f06/j07/d06 same-cell hold review in native24b reports. CPU EOP tap corrected to18; clean CPU status observed on directed replies. |
| Server diagnostics | Quiet-after-Cedar RESTORED | server1d5e05e9..., PID12696, logserver-cedar23-route.log. Delay/nil/link-broadcast diagnostics did not clear retries. Restart this server before another core load to restore initial BOL, since quiet sessions can reuse Main's same PID/socket path. |
| Native25 receive-full correction | Cold120M/fit/review COMPLETE | 8931 sampled germ words/no mismatch, original Initial/Cedar gates, IFU670280/670434, all6 errors enabled/clean106611683 samples. Timing29 checks,min+0.071ns; exact MemX c21 own4 hold paths reviewed. Hardware now passes routing/time/name and displays Cedar login; Guest accepted. |
| Native26 capacity build | Fit/review COMPLETE | `misterubuntu:/home/alans/mister/Dorado_Native26_20261017/`, `native26-build.log`; four64K-chip modules/8MB physical RAM,64K map, Native25 k04 correction retained. Experimental; timing29 checks,min+0.074ns, no illegal/unconstrained clocks. Cold final500M reviewed; sample coverage fails8880/8900. Actual firmware/BFN113/OS routing succeeded; login under observation. |
| Four64K-module software control | COMPLETE24B | `cedar-64k-map64-control/`: fresh8MB RAM/64K map reaches Viewers (166685px), type2, VirtualBanks0100; valid geometry reference, software only. |
| Native27 passive fault/BB monitor | Fit/review COMPLETE; not deployed | `native27-remap.log`;29 timing checks,min+.115ns, zero illegal/unconstrained clocks. Exact ProcH l03 bit1 own4/MemX e20 bit2 own2 hold-only review. RBF ac94263d...; keep Native26 installer active. |
| Larger-memory cold RTL | COMPLETE500M | `cedar-density64-ddrcheck.log`,500M limit,SYSPER4, four64K-chip modules/64K map,actualDDR. `storage64-candidate/`; component prototype, not production. Final IFU8540094/8543534, original Initial/Cedar pass, all6 errors clean486611635 samples; source gate fails8880/8900 coverage, not a pass. |

Native24 changes only passive observation. **Its CPU EOP tap is wrong:**
EthData_17 is parity from k11. FIFO bit18 passes through l07 to EthData_18,
which is the correct CPU-side EOP. Ignore machine-debug bytes23:16 and7:0
on the deployed image. Bytes31:24 (FIFO EOP count) and15:8 (FIFO status)
remain usable. Directed routing replies reach FIFO with status00; the initial
BOL while the receiver starts had status20. This does not establish CPU receipt.
The generator was corrected and Native24b deployed after timing review;
Native25 now supersedes both images. CPU task/PC
and machine-status fields remain available. RAM/clocks/functional packages
match Native23. Those historical Native24 screenshots have no Cedar OS text; Native25 does.

Fresh read-only physical DDR capture (`native24/raw-ddr-broadcast-observe.bin`)
shows a candidate receive buffer at physical0x8604 with IOCB pointer0350.
Its first wire word at+14 is duplicated:0001,0001,0200,0020,0087 for the
broadcast time reply; multiple routing buffers similarly have2201,2201,0200.
A working C control buffer has2201,0200 immediately. Candidate IOCB at30e50
points to virtual091712 (matching this buffer's+14 offset). This is a strong
first-word Input/Store lead, **not yet a proved cause**: raw DDR is not coherent
with the cache and the precise microcode phase needs tracing. The initial isolated
sequence was invalid for the CPU handoff: missing MemSH kept the address
latch on EControl while the first packet auto-drained. The corrected bench
drives both clock phases and a warmup packet, then checks each held word,
CRC and EOP through addressed Input. It passes; it still does not execute
Cedar CPU instructions. The tracked receive-sequence test preserves it. Evidence: native24/receive-header-observation.json.

A focused Ether05 k04 B regression confirms a receive-full handshake defect:
Input retiring on clk1 is missed by post-edge JK capture, leaving the word
unread; a newly enabled Input can also consume too early. PRECAP_B=1 fixes
both setup cases, with A/RxBOP unchanged. Exact generated-board diff is one
parameter. Full held-word receive and8 good/damaged-CRC phases pass. This is
**not yet verified by a coherent receive-buffer recapture**. Native25
cold120M/timing pass and hardware advances through services to real Cedar
login, accepting Guest. The earlier service wait is cleared; desktop boot
is still open. Tests: ethernet-input-hold-test and
ethernet-receive-sequence-test. Evidence: native25/component-review.json.

## Reproduce the offline media correction

```sh
python3 tools/pdi_stamp_file_hints.py build/rtl-executive/cedar-work-native-chs.pdi build/rtl-executive/cedar-work-native-hints.pdi
python3 tools/pdi_to_trident.py build/rtl-executive/cedar-work-native-hints.pdi build/rtl-executive/cedar-work-native-hints.pack
```

The unchanged baseline stays separate. The tool rejects conflicting nonzero
label hints or logical-root FPs. The corrected pack baseline is preserved on
MiSTer14 as `System_20261017/cedar-work-native-hints-f3bce2a8.pack`; verify
its full SHA before cloning any new writable test pack.

## Capacity findings — corrected at 20:43 UTC

A fresh **2 MB RAM / advertised 16K map** software run reaches Cedar's login at
3B, accepts Guest, then remains at Loading DFPackage through 36B. Its map audit
has no referenced page above 16K. This supports login capacity only.

The **8 MB / advertised 16K map desktop is NOT a valid map-capacity result**:
C advertises the size but indexes its full 64K array, and the final snapshot has
381 referenced pages above 16K. The four-64K-chip, 8 MB control also has high
references. Do not claim 16K suffices for the desktop. Stock 8 MB / 64K reaches
Viewers. The 2 MB / 64K restored control is blank at 20B; do not describe it as
DFPackage or infer its cause without more evidence. Larger-memory generator flags are now prepared and component-tested, but
full cold boot/timing/FPGA desktop qualification remain required.
Software controls use the C IOCB bridge and are never FPGA evidence.

Experimental geometry is reproducible with:
```
python3 tools/sil_fpga_native.py -o build/rtl-executive/native26-density64 \
  --pilot-disk --receive-monitor --storage-chip-kbits 64 --map-kwords 64
make -C verilog memory-density64-test map-density64-test storage-board64-test
make -C verilog/verilator storage-board-test
```
Default map/MSA generated RTL is byte-identical; default MSA comparison still
checks2,359,296 bits. The64K board comparison checks9,437,184 bits over12,012
complete board cycles. Native wrapper lint passes. Default wrapper differs
from fitted24 only in passive-port connection order/whitespace. Density straps
and DDR widths are explicitly checked. Manifest marks larger geometry experimental.
Cold scratch64 completed500M with germ entry and8,880 sampled source words,
zero mismatches; this is below the existing8,900 gate threshold (old16K run
sampled8,931), so it is **not a pass**. Final IFU8540094/8543534, original Initial/Cedar image and all6 errors
clean486611635 samples pass. Missing source-sample coverage remains open.
Do not weaken the gate to claim success.

Density prototype trace budget: at244,622,225 samples a validated exact-build
LLDB write set only testbench n_tow=-1 to stop a repeatedly rearmed printf.
No CPU/RAM/controller/clock/run-limit state changed. Script/log in
storage64-candidate/stop_repeated_tow_lldb.py and trace-budget-lldb.log.
The500M cold run has completed normally. Do not confuse trace control with guest repair.

## Hardware and reproducibility

Only **MiSTer14** is allocated. Other MiSTers are busy. Current Native26 image (experimental8MB/64K):
`586dc7f898709491813bd13deff59cec160be1ac620998cb757dd15e4f3bf4db`.
Previous Native25h/25 image:
`3a8eee4094b9d652bad276a139cf76f1a0abd627d92a21d8df92354bbf31b3c7`.
Previous Native24b image:2cffe643... (same functional RTL as23, corrected monitor).
Previous Native24 image:1cdc35a2... (CPU monitor tap invalid).
Previous Native23 image:
`89726440d5d599a5b01428e4f29b54d3969064508191016e100375746ff765e2`.
Fresh pack baseline: `c0cd6784ea9df43d669dab2c445b3a1a7241048ac87f10afa747927d931bad5c`.
RBF/MGL: `/media/fat/_Computer/DoradoRTL_Cedar26_20261017/`.
Current corrected pack SHA256: `f3bce2a8566b3bac3dd793b2016910c71b0c8d7b061b9d48c14261665802df51`.
Main log: `/media/fat/games/DoradoRTL/Cedar26_20261017/main.log`.
Prior25 evidence: `Cedar25_20261017/main-before-native25h.log` and
`server-before-native25h.log`.
Previous24b log: `Cedar24b_20261017/main-before-native25.log`.
Previous24 log: `Cedar24_20261017/main-before-native24b.log`.
Previous23 log: `Cedar23_20261017/main-before-native24.log`.

Keep immutable Main `System_20261017/MiSTer-1fb2e098e6f0dba6`; never overwrite a
running executable. Active boot-capable quiet server:
`System_20261017/dorado-server-1d5e05e9d293b095`, PID12696,
log `server-cedar23-route.log`. Link-broadcast diagnostic8ecf6cf9... did not
clear time retries; its separate log `server-cedar24-broadcast.log` is preserved.
Default bdfd09ce..., Delay500 and nil500 binaries/logs are preserved; neither
delay nor nil checksum resolved retries. Restart config points to the quiet
server1d5e05e9... and Native26 core/MGL, cwd `System_20261017/dorado`, socket
`/tmp/dorado-net.sock`. Full STP root/all 34 Basic.Loadees are staged. Restart
configuration: `Cedar26_20261017/reboot-resume.json`.

Helpers: `cedar23-launch/c-until-request.py`, `cedar3-launch/cool-boot.py` (three
boot pushes ONCE), `cedar10-launch/type-keys.py`, `mister_mouse.py`. Loading a
Main process does not program the FPGA: use `load_core` through `/dev/MiSTer_cmd`.
Allow roughly five minutes for original firmware startup. Preserve
`verilog/verilator/obj_executive` and `build/rtl-executive/alto-executive.rtl`.
**Never use `+frame`**, which overwrites the golden artifact.

The cold DDR log is `cedar-native23-ddrcheck.log`. A readonly POD capture at
211,584,441 samples is `native23-candidate/cold-ddr-readonly.rtl`, fingerprinted
for `obj_native_rom_cedar_native23ddr`; debugger log records no process memory
writes. It is diagnostic, not a fresh-run qualification. Earlier 167M/199M replay
states use a different model; never interchange them without layout auditing.

Independent timer debt: MC12061's fabric/8 oscillator is 1.667x fast relative
to guest time at SYSPER4. A scratch 10MHz accumulator passes cadence probes at
4/8/16; it is not in production or Native23/24. Qualify full firmware before
promoting it. Native22's slow-clock experiment failed one of 8,931 source reads
and has unreviewed hold violations; never deploy it.

<a id="current-todos-2026-10-17-native23"></a>

## Current TODOs (2026-10-17, FPGA boot)

- [x] Fix MemX j01 module-switch timing; qualify original source copy and Native21.
- [x] Trace the first germ fault and fix ProcH d13 signed-overflow polarity.
- [x] Pass focused regressions, full cold DDR220M, FPGA timing and clock checks.
- [x] Deploy Native23 and complete original firmware/BFN113 boot.
- [x] Observe real disk-loaded Cedar pages and new OS network traffic on FPGA.
- [x] Fix server routing/file-lookup dispatch; pass native/ARM transport tests.
- [x] Test delayed/nil-checksum replies; neither resolves live service retries.
- [ ] Trace the apparent duplicated first receive word in Input/Store; clean FIFO status alone does not prove correct buffer contents.
- [x] Fit/review/deploy Native24 passive monitor (29 checks,min+0.122ns).
- [x] Complete Native24 original firmware boot and inspect FIFO status.
- [x] Review/deploy Native24b corrected EthData_18 EOP tap (29 checks,min+0.080ns).
- [x] Complete Native24b firmware/BFN113; observe good CPU/FIFO EOP status on replies.
- [x] Qualify Native25 k04 correction in cold120M DDR and routed timing.
- [x] Complete Native25 original firmware/BFN113; pass service replies and display Cedar login.
- [x] Type Guest plus two Returns through the real FPGA keyboard; observe Guest.pa authentication.
- [x] Identify post-login Basic.Loadees failure: synthetic file-label DA hints are zero.
- [x] Validate hint repair with paired DA-enforcing C controls (17 served files versus0).
- [x] Boot Native25h original firmware, accept Guest, reach real STP lookup past old FP failure.
- [x] Observe real FPGA Basic.Loadees transfer, then RPCRuntime/UserProfile downloads.
- [ ] Diagnose Native25h pack_fault after RPCRuntime; UserProfile incomplete.
- [x] Native26 original firmware/BFN113 handoff and real OS routing observed22:46.
- [x] Native26 displays real Cedar login (screenshot22:47).
- [x] Native26 Guest accepted and loader passes UserProfile fault point, through WorldVM22:52 with pack_fault0.
- [x] Complete all33 Native26 BCD downloads; actual screen reports every load ok.
- [x] Answer installer personal-profile prompt through real keyboard; observe BootEssentials BringOver.
- [x] Complete Native26 BootEssentials closure; now attaching font metrics.
- [x] Complete font attachments; actual HDMI shows FontMetrics End BringOver/ok and Inscript/VersionMap START ok.
- [ ] Complete remaining STARTs; monitor pack_fault.
- [x] Restore external HDMI after live mode test; actual Cedar HDMI capture readable.
- [x] Resolve requested74.25MHz versus cached60Hz discrepancy: fresh hardware counter30Hz,74.25MHz; separate148.5MHz timing fails.
- [ ] Use qualified-clock progressive output and verify stable deinterlacing, including video.
- [x] Fix Main extended-modeline zero-clock parser in separate patch; mutation regression and pinned GCC10.2.1 ARM build pass.
- [x] Deploy immutable Main40e1 containing the parser fix as part of fresh41c disk-cache qualification; HDMI reinitialization remains open.
- [x] Qualify Native27 passive fault-monitor fit/timing:29 checks,min+.115ns.
- [ ] Reproduce medium fault with Native27 observation if needed; not deployed.
- [x] Complete the33 listed BCDs (34 files including Basic.Loadees) and reach Viewers.
- [ ] Improve cable TX queue: one pending packet caused1 dropped short packet; BSP recovered, but a burst-capacity regression and deeper queue remain useful.
- [ ] Recapture the receive buffer coherently if byte-level header proof is needed.
- [x] Add experimental64K geometry generator; pass cell, full MemX geometry,
  full MSA/DDR9,437,184-bit comparison and unchanged default-board gates.
- [x] Review density500M final IFU/images/all6 errors: pass.
- [ ] Resolve source sampling8880/8900 coverage and complete high Map protocol.
- [x] Qualify Native26 fit/timing:29 checks,min+0.074ns, zero illegal/unconstrained clocks.
- [ ] Qualify Native26 original firmware/hardware startup after reviewing cold coverage.
- [ ] Resolve physical RAM and map capacity with enforced limits; the 16K
  advertised-only desktop control is invalid.
- [ ] Qualify remaining fixture differences: native RAM/cache/map initialization,
  complete BaseBoard handoff, DDR pauses and HPS track-buffer timing.
- [ ] Calibrate MC12061 and qualify original firmware startup/timers.
- [x] Reach the Viewers desktop and a live CommandTool `%` prompt (00:02–00:03 UTC).
- [x] Verify real mouse motion (+100,+80 in native raster), Cmd-button click and new CommandTool focus.
- [x] Exclusive scripted keyboard qualification on Native34: short marker and full alphabet/digits echoed exactly, fresh prompts; prior Native26 overlap is not used as evidence.
- [x] Add stopped-clock key-event queue regression, direct-old-path loss control, chord/hold/repeat/overflow gates.
- [x] Fit/review buffer successor Native29 at40MHz; hardware firmware/Cedar login and fast buffered Guest input pass.
- [x] Complete successor cold Cedar/Viewers/CommandTool/Echo/mouse qualification at50MHz on Native34; Native31 itself was superseded.
- [x] Characterize clock baseline: original-ROM4 passes,2 fails startup; independent divider audit and Native26 Fmax41.08MHz saved.
- [x] Write staged clock implementation plan and test an isolated direct phase/enable source against the original divider, pre-edge pipeline capture and all-phase freeze/resume.
- [x] Extract/check original T-control wiring and translate/test independent control-bank capture interfaces (8192 cases, strict lint).
- [ ] Derive ProcL/ProcH T capture enables and complete data/latch/task-file cone; gate SendMIR/Boot0/compute/original-ROM startup. See docs/verilog-clock-plan.md.
- [x] Review Native29 40MHz fit/STA/clock/hold endpoints; cold firmware/BFN113/login and fast Guest input pass on MiSTer14.
- [x] Add coherent fabric-edge counters and dynamic UIO snapshot regression (old-code failure control); Native30 map passes, fit pending.
- [x] Replace variable-index arbitration scan/payload mux with a balanced tree; independent order/payload/stall and full MSA equivalence gates pass.
- [x] Review Native31 50MHz STA/clock/fast+slow hold paths; all29 checks pass with no new post-fit exceptions. Load on MiSTer14 and measure firmware enabled fraction.
- [x] Measure Cedar loader enabled fraction16.9255%, verify Guest input and actual1080p30 HDMI at50MHz (host-FB toggle workaround).
- [x] Verify Native34 Viewers/exclusive Echo/mouse; desktop with startup Bringovers sampled19.8755% enabled (not idle or retired throughput).
- [x] Prototype optional128-line raw DDR buffer; independent coherence/reset/stall tests and full storage equivalence pass; disabled/stale-data mutations fail.
- [x] Review Native32 fit/STA/clock/fast+slow all-category timing, no new exceptions; load on14.
- [x] Native32 firmware/login/Guest and real loader; record first host timeout807c0200/LBA18893. Hardware qualification failed before Viewers.
- [x] Fix host watchdog clock scaling and add ACK-phase first-fault observation; independent wall-time/CRC/persistence and build-config gates pass.
- [x] Review Native34 fit/all four device timing conditions/clocks; cold firmware/BFN113/login/fast Guest pass, passive host-latency observer running.
- [x] Complete Native34 loader/Viewers/exclusive Echo/mouse and edge-count performance observation; preserve observer prefix, no repeat timeout.
- [x] Add/test/fit exact DDR/HPS pause attribution; capture Native35 idle-login DDR/ARM/cache/transaction windows. Compare controlled workloads before promoting performance defaults.
- [x] Audit actual physical BRAM/MLAB allocation and deployed ARM disk path; preserve later O_SYNC stalls from full observer trace.
- [x] Confirm128MB SDRAM as a supported-target requirement; document8MiB guest/16MiB raw backing placement and migration gates.
- [x] Qualify Native36 M10K Viewers/mouse/exact keyboard after documented BSP recovery; keep placement/performance claims separate.
- [x] Diagnose the lost font-completion tail and test automatic daemon probes natively/on ARM, with old-loop failure control.
- [ ] Use the new daemon for unassisted cold boots; cold-remount36's closed local marker file.
- [x] Implement/select/simulate SDRAM raw-storage backend, full MSA equivalence, independent timing/refresh/clock-delay controls and build preflight.
- [x] Close SDRAM pin/internal timing on40 after capture/logic-placement fixes; all four device conditions and explicit I/O checks pass.
- [ ] Qualify combined41 on hardware and compare backing traffic/serialization against the instrumented DDR baseline.
- [x] Derive instruction issue/held counters from original control strobes and verify15 real-board microprogram cases plus naive-count failure controls.
- [x] Implement optional generated instruction-signal exports and Main coherent-pair views;15 exported-port cases/native wrapper lint/default-OFF compatibility pass.
- [x] Fit/all-device timing Native39 instruction diagnostics on frozen36 DDR baseline; deploy with precise original-firmware boot selection.
- [x] Combine SDRAM/logic placement and passive counter in41; pass all four conditions including explicit SDRAM I/O timing, min+0.114ns.
- [x] Measure39b idle actual issues/held/clocks and pause/traffic views; archive matching inputs and service-repair caveat.
- [x] Restore installed quiet-after-Cedar daemon policy in source; native/ARM quiet/reset and Cedar-quiet BSP recovery pass, old flood binary fails.
- [x] Fit/all-device timing matched cache-off DDR42, min+0.115ns; preserve same41 board/placement policies.
- [x] Native41 original firmware/Cedar login/Guest input on SDRAM; measure three idle issue windows and matching cause views,10.20% above39 configuration.
- [ ] Complete41 full Viewers/input/persistence;42 matched device control remains untested on hardware.
- [x] Diagnose/reproduce PrintTV ACK race; bounded producer-independent packet replay passes native/ARM loss/wrap/ring/reuse tests; old server fails regression.
- [x] Fresh original-firmware/Viewers boot with e237dffc fixed daemon, without ARM cursor repair (41b); later disk stress failed separately.
- [x] Stage/deploy identical-RBF Native41b from untouched seed with tested e237dffc daemon; preserve old41 pack/logs after repeated old-server ACK race.
- [x] Native41b original firmware/BFN113/login and actual Guest input; C watcher completed/released; three idle issue windows reproduce1.5957M/s.
- [x] Native41b all34 loadees/profile No; pass original PrintTV stall point with fixed daemon and no cursor/guest repair.
- [x] Native41b full unassisted Viewers/mouse/Cmd/focus/exact short+alphabet+digit echo; local marker created/closed/read back.
- [ ] Cold-remount persistence remains unqualified:41b halted on host-write timeout before the normal boot was visibly accepted; preserved pack excludes dirty FPGA track. Use a clean persistence case after disk-service repair.
- [x] Optional response-boundary raw arbitration and generator: independent order/payload/reset/stall fixtures, full MSA equivalence and Native43 all-device timing pass.
- [x] Deploy frozen43 arbitration policy as matched DDR-network48; original firmware/login/full base list pass and actual idle gain+6.1931% versus clean47b measured.
- [ ] Repeat pinned end-to-end comparisons; one idle boot per configuration and absolute48 loader duration are not repeated or full desktop gains.
- [x] Collect passive full-MSA raw trace/final physical-state oracle; replay layout/cache/row locality with discarded-write/corrupt-read failure controls.
- [x] Implement default-OFF raw group interleaving; canonical transaction/physical-state/full-board/reset/packing checks, default generator compatibility and Native44 all-device timing pass.
- [ ] Finish48/49/deferred-policy pinned FPGA issue/latency/traffic, full desktop/input/persistence and repeated comparisons;42 and first48 idle controls are measured.
- [ ] Fix recurring initially black HDMI after core load in Main; framebuffer toggle is a workaround, parser patch is a separate defect.
- [ ] Deploy/qualify instruction diagnostics, including maintenance/reset/task switching, and measure actual issue rates with external waits separately.
- [x] Begin4a with generated per-instance logical-path state inventory; actual-cell/read-pipeline/task-write/ROM-load/array/hierarchy coverage and failure controls pass.
- [x] Qualify isolated34-bit IM bank state access: exact RAM/read pipeline/first-edge resume, independent original cells and broken controls; matched four-condition fit retains fourM10Ks, +62 ALMs/+43 registers.
- [x] Prove actual ContB IM fanout grouping against wire list/cell expressions:144 slices, parity and144 read registers, eight18-bit half-banks; truth oracle and six damaged controls pass.
- [x] Qualify18-bit explicit-enable snapshot RAM against unchanged original cells and independent array; retain twoM10Ks/OLD_DATA, matched registered-shell all-corner timing min+0.255ns and damaged RTL/report controls.
- [x] Qualify bounded actual ContB IM state access: unchanged-board all-output/strobe/clock-hold comparison and matched isolated gated-clock all-corner fit,16M10Ks/min+0.168ns.
- [x] Export/classify all209 remaining ContB q/setup/history bits; full147809-bit modeled board save/continue/restore/first-edge equivalence, coverage controls and isolated all-corner fit pass min+0.165ns.
- [x] Integrate ContB adapters with admin inactive into frozen49 complete core;31/52/four-condition timing and fitted8x18-bit OLD_DATA checks pass min+0.115ns. Hardware normal-operation gate remains pending.
- [x] Refresh exact frozen49 native hierarchy inventory and prove qualified ContB coverage inside real machine.
- [ ] Connect active bounded access/global barriers and extend/classify other boards/wrapper/devices/input/raw memory/Main/server; qualify complete physical restore and combined timing.
- [ ] Implement complete snapshot milestone4a: all RTL/RAM/device phases, matching disk generation and local server state; safe barrier/schema checks and Cedar restore qualification.
- [x] Preserve/decode41b post-desktop host-write timeout and unchanged physical pack; exact blocking syscall remains unobserved.
- [x] Implement/test bounded whole-image ARM cache, dirty-run coalescing, worker file IO, Main-only SPI capture and error recovery; actual-worker Linux/ARM and full pinned Main build pass.
- [x] Add monotonic bounded request/ACK/file/poll tracing and durable remount/program/restart/reboot/SIGTERM/SIGINT boundaries; component tests pass.
- [x] Deploy immutable disk-cache Main on fresh same-RBF41c with observer; cache-ready confirmed and real original CoolBoot requested.
- [x] Finish41c fresh original Cedar/Viewers/mouse/Cmd/focus/exact keyboard; local marker created/closed/read back.
- [x] Actual Main4200ms delayed flush work:3 read/21 write ACKs, continued instruction progress, no poll gap above20ms; subsequent marker readback and host durability barrier pass.
- [x] Qualify physical DDR packet progress during actual Main4200ms worker delay: real STP metadata plus exact guest ACK;10 reads/100 writes, no>20ms poll gap or faults.
- [x] Native48 completed pretyped Echo via in-window Return and mouse motion during actual4200ms worker delay;8 read/28 write ACKs, no>20ms poll gap, inside capture and five failure controls pass.
- [ ] Qualify sustained disk/network/input stress; controlled worker success is not sustained qualification.
- [x] Normal41c guest Boot, Main drain/fsync/exit, preserved physical image81c41955; new41d ARM cache and full FPGA reload, original BFN113 reached/C released.
- [x] Read41c markers after41d cold Main/cache/full FPGA boot: exact rtl41cpersist20261018 and worker42 plus new prompt; normal guest Boot/loader barriers and preserved volumes.
- [ ] Qualify sustained fault-free disk/network/input stress and three final fresh-seed desktop boots.
- [x] Deploy timing-qualified42 matched cache-off DDR control with same Main/server/seed and final fresh observer; original CoolBoot requested.
- [x] Measure42 idle-login issues/causes before Guest; matched one-boot SDRAM gain10.2196%, not a repeated qualification.
- [x] Finish42 original-firmware Viewers desktop, mouse/Cmd/focus and exact short/alphabet/digit echo in independent CommandTool.
- [x] Preserve42 through normal guest Boot/germ812 and actual loader barriers; physical pack0c5ab6f5 retained.
- [ ] Repeat pinned workload comparisons after DDR network bring-up.
- [ ] Separately reduce FPGA whole-track write amplification where original guest/CRC semantics permit.
- [x] Prototype default-OFF deferred internal DRAM reads; independent nonzero pipeline/write/reset/array/full-MSA/physical-state/default-generation gates pass.
- [x] Native45 full all-device timing PASS:31 summary/36 categories/four conditions, minimum+0.017ns, no new cuts or clock coverage gaps.
- [ ] Measure45 actual Cedar after preserving42/43/44 baselines; area79% must be weighed against measured gain and snapshot budget.
- [x] Add/test nonblocking pinned Main worker enqueue and empty-wake predicate on Linux/ARM; blocking and old-wake controls fail. Prerequisite only, not deployed/integrated disk I/O.
- [x] Review MacPPC/Sun/NeXT DDR Ethernet host/RTL rings; record source hashes, address overlap and payload-size constraints.
- [x] Implement bounded C/RTL DDR frame endpoints; real interoperability/oracles, full2048-byte/stall/wrap/malformed/reset/stop and failure controls pass, C also on actual ARM.
- [x] Implement Main/daemon STOP/resume/fresh grants, SPI capability/epoch and ownership mux, validated mapping and legacy fallback; guarded actual-service/lifecycle tests and pinned ARM builds pass.
- [x] Native46 all-device timing31 summary/36 categories/four conditions min+0.112ns; original41 CPU/storage sources and constraints preserved.
- [x] Qualify combined production wrapper/UIO/wire/DDR+C endpoint with independent serial payload/CRC and broken mux/pause controls.
- [x] Capture46 disabled/not-stopped hardware refusal; preserve42/untouched46 seed; real STOP-only handshake and disk drain, then full-board recovery.
- [x] Native47 all-device timing31/36/four conditions min+0.111ns; normal temporary Main exit, real fresh DDR grant, exact daemon mapping/header/epoch and cache-ready pass.
- [x] Actual final ARM daemon boot/Cedar/STP/quiet/reexec/retry/wrap/history bytes pass on isolated Unix sockets with DDR enabled but ungranted.
- [x] Finish47 original-firmware physical-DDR Viewers desktop, real mouse/Cmd/focus and exact short/alphabet/digit echo; settled capture shows returned prompts.
- [x] Actual active-DDR STOP/drain/unmap and normal Main loader/disk barriers, preserved47 volume, full same-RBF fresh47b load and different physical epoch grant.
- [x] Qualify completed input during actual delayed Main worker on Native48.
- [ ] Qualify independent physical packet bytes and sustained hardware operation.
- [x] Native48 all-device timing31/36/four conditions and clean47b/48 DDR-network idle comparison; bounded48 full base-loader trace and visible successful loads.
- [x] Native49 matched DDR-network timing/login/idle(+14.9453%) and full base-loader timing(16.136..16.276% less time versus48), one boot each; full Viewers/input pending.
- [x] Native48 full original Viewers/mouse/exact keyboard and warm marker readback.
- [x] Native50 deferred-read all-device31/36/four-condition timing, minimum+0.035ns; deployed/tested/retired, policy rejected.
- [x] Native49 full original-firmware Viewers, real mouse/Cmd/focus, exact short/alphabet/digit echo, warm marker readback and normal guest Boot/germ812.
- [x] Qualify50 original boot/idle/full base loader; reject as baseline for negligible benefit and3323 extraALMs, preserve disk/evidence and return to49.
- [x] Fresh49b original firmware/login/idle/base-loader repeat; independent idle mean reproduces49 within0.000203%.
- [x] Fresh49b full Viewers/mouse/Cmd/focus and exact short/alphabet/digit echo with new prompts.
- [ ] Automate fixed post-idle Guest/profile input timing before final repeated benchmark gates.
- [ ] Finish independent physical payload/reset-stall/fallback hardware qualification for the implemented direct-DDR transport; original DskEth/CRC/wire and selectable SPI path remain.
- [ ] Optional external Xerox-network UDP tunnel compatible with IFS/ContrAlto; test Cedar STP separately from Alto interoperability.
- [ ] Translate remaining control/memory/latch families, reduce storage pauses and close actual STA before selecting/deploying faster clocks. Eight-slot prototype is not the final clock architecture.
- [x] Commit/push confirmed current clock/storage fixes and Native34 evidence;
  preserve Executive/mouse/disk regressions. Space Invaders remains separate.

## Investigation diary (historical; current state above takes precedence)

**Native23 reaches new network traffic, 20:22 UTC; server dispatch fix.**
Original firmware completed and emitted fresh BFN113 at20:20; C key released.
FPGA leaves the old RequeueSearchSQ loop and emits GatewayInfo requests on
socket2/type200. Its screenshot still shows only the maintenance cursor,
not Cedar text. The raw-Pup server's STP dispatcher wrongly intercepted
all type200 packets as fileLookup, swallowing routing. The captured request
fails against the old server and passes with socket2 excluded from that
file-service branch. Native Ethernet tests and ARM routing/BFN113/66KB STP
tests pass. New immutable server SHA bdfd09cec0851e78de580683c4de936fbed46411ba7ff1b5d2e7fcbda5801d66
is active without touching Main/FPGA; log System_20261017/server-cedar23-route.log.
Fresh reboot-resume configuration is in Cedar23_20261017/reboot-resume.json.
Live retries now receive logged201b replies, but the guest still retries.
Receiving/demultiplexing those replies remains under investigation.
No OS prompt claim. Hardware screenshots are in the Native23 report directory.

Software controls: stock8MB/map64K reaches the Viewers desktop by15B. The
2MB/map16K control remains Loading DFPackage at36B; its final map has4097
valid entries, no referenced VP above16K. Independent RAM/map controls now
run from their paired1B snapshots in cedar-capacity-isolation/ (session90636):
8MB/map16K passes all34 loadees by5B and reaches Viewers, but its final
audit has381 referenced VPs above16K. This is not a valid map-capacity result.
2MB/map64K is blank at20B (earlier DFPackage statement was incorrect).
PhysicalRAM limits the2MB control, but advertised-only map controls cannot
qualify the map. Recheck enforced limits before deciding hardware geometry. These are C controls, not FPGA evidence.


**Native23 build complete, 20:14 UTC.** Full map/fit/assembly/STA/audit complete.
Fit took 51m28s wall, 35,041 ALMs (84%), 178 M10Ks, 35 DSPs. Setup passes
(minimum 2.886 ns); three hold paths are -0.023 ns and are under detailed
review before deployment. No illegal or unconstrained clocks. External I/O
coverage remains the framework's separate limitation. No RBF loaded yet.
Cold DDR run has passed 121M; partial source-copy audit has 8,931 sampled
words and zero unexpected mismatches. Final cold summaries are still pending.
The constrained software control at 30B is still loading DFPackage, so desktop
capacity remains unresolved; its login screen is verified.


**Current FPGA RAM/map geometry reaches Cedar login in software, 19:47 UTC.**
Fresh3B controls complete:2MB/map16K28604 px, stock8MB/map64K28601 px.
Visually verified the constrained screen: Cedar6.1.0 / Please login / Name:.
Report native23/software-control-2mb-login.png and software-capacity.json.
This proves capacity for LOGIN, not RTL/FPGA behavior or desktop capacity.
Do not enlarge RAM/map just to get beyond the diagnosed germ fault.

Constrained control now restores its paired3B snapshot/PDI, types Guest+two
Returns at3.1B and advances toward36B with periodic images and FTP trace.
Scratch cedar-2mb-desktop, session28760. It is already fetching STP data;
no desktop conclusion yet. Main emulator sources/binary unchanged; constructor
size/type override exists only in ignored cedar-2mb-reference scratch.
Native23 FPGA fit remains active (routing,172%CPU at27.5min), not stuck/failed.

**Control updates, 19:41 UTC.** Native22 slow8 control COMPLETED220M and is
REJECTED:8931 sampled germ words, one mismatch at VA11cf expected110a/actual7309
@190282853. IFU271842/271996, original Initial/Cedar image gates pass; this does
not make its source-read gate pass. Report native23/native22-control-rejected.json.
Its fit also has three unreviewed -0.023ns holds. Never deploy22.

Native23 cold DDR launch omitted the passive copy trace switches. Enabled ONLY
bootio=1/from85000000 at live sample61252022 using exact compiled offsets,
validated root/counter/current flags and readback; no CPU/RAM/controller changes.
Evidence native23-candidate/enable-trace.log and trace-layout.json. For reproduction
include +bootio +bootiofrom=85000000. Cold run remains session67681 / PID60378.

C capacity check prepared in cedar-2mb-reference: constructor's storage size/type
only changed to1M words/type1 (four256K-word modules), unmodified C CPU and normal
IOCB bridge, disposable PDI. Not FPGA evidence; never import its state into RTL.
The1B stop was too short: all four RAM/map controls were blank, including stock
8MB/map64K. Do not infer insufficient RAM/map from this. Current Makefile recipe
says2B before typing, uses2.5B; old650M comments are stale. Fresh pinned3B controls
are running at cedar-capacity-3b (ram2-map16 and stock ram8-map64), session41103,
with DORADO_PDI_SAVE and snapshots paired to their disposable PDIs.

**Independent timer candidate prepared, 19:30 UTC.** Scratch timer10mhz-candidate
uses exact3/(5*SYSPER) oscillator edge cadence for the10MHz crystal. Nominal
BaseBoard probe PASS at4,8,16:1MHz MCPreClk and16us Pendulum/Refresh relative
to guest time. Reports/baseboard-cadence/candidate10mhz-sysper*.txt. Downstream
Pendulum is quantized to the board clock phase; average period is checked.
Not in production/Native23; full firmware startup qualification remains open.
When promoting, use bounded accumulator width and generator-provided SYSPER.
The full component boot's Pendulum/Refresh are correctly paced substitutions,
so that run does not qualify the physical BaseBoard crystal/timers.

**Cedar OS-file data now transfers in the diagnostic, 19:27 UTC.** Native23
replay through200M completes;199M snapshot still has FaultInfo0003 and all four
germ entries intact. Cache/map-aware low-VM export matches51 NONZERO pages of
BasicCedarDorado.boot!22 byte-for-byte (zero pages excluded). This is evidence
of real boot-file data arriving through the RTL disk path, not an OS prompt.
Report2026-10-17-native23/diagnostic-os-pages.json. A small parse first treated
Entry.value as upper page bits; corrected against BootFile.mesa Entry: one page
word plus two PageValue words. Full1060-page layout parses exactly.

The clock-reading loops correspond to BootChannelDisk.cyan61 Pause[100] in
transferWait; RTClock advances6448@160M ->6c86@169M ->8802@199M, high000b,
consistent with one pulse/~32us. Further disk input follows. Don't call these
loops stuck without checking elapsed guest time and transfer progress.
MPROWS in watch_map uses the old physical low-memory view; after CompactVM/
boot-file page loads, blank rows are not proof of any OS stage. The Cedar-aware
inspector also reads blank199M; the boot file explicitly carries the I/O page.
Use actual FPGA screenshots for display claims. No Native23 deployment yet.

**Overflow root cause confirmed in captured path, 19:21 UTC.** Canonical generator
and regenerated ProcH now select the physical d13 XOR; signed-compare-test
PASS256, frozen old board fails the equal JLB case0-0-1. Compute, storage-module
switch and map-buffer-hold pass. The167M->170M diagnostic replay preserves all
four germ entries and avoids the old fault/queue loop. At169M FaultInfo0003,
not4080; map1f20/1f21/1f40/1f41 remain01fbde/01fbee/01fb8e/01fbbe.
Replayed state was explicitly layout-audited, with every payload byte retained;
this is diagnostic only, not cold/FPGA qualification. Report2026-10-17-native23.
Extended diagnostic169M->200M session71311, cedar-native23-extended-diagnostic.log.

Corrected cold DDR run session67681, cedar-native23-ddrcheck.log, SYSPER4,
220M limit. Earlier SYSPER16 invocation stopped/rejected and preserved; never
use it as the Native23 gate. Checkpoint23 model also rebuilt at4, object
obj_native_rom_cedar_modulecheckpoint23. Quartus Native23 fitting, session6695.
MiSTer14 still runs21; fresh23 pack/profile prepared (baseline hash checked),
no RBF staged/programmed. New physical fix is local pending full qualification.

**Native23 overflow candidate running, 19:17 UTC.** ProcH d13 still selects
MC10170's legacy B inversion. Isolated candidate selects LEGACY_B_INVERT=0,
the documented physical XOR, only at this package. New signed-compare probe
exercises the generated board's actual d13 plus the ContA c21 gate: candidate
PASS256 arithmetic/branch vectors including equal germ bounds; old board fails
first0+0 no-overflow vector. Test stimuli drive the detector's five operand/
result/carry pins; no guest state is imported. The first probe incorrectly
chained a force RHS to a combinational ALU result; result/carry are now explicitly
settled stimulus registers. No such forcing exists in the cold boot.
Scratch native23-candidate, tb_signed_compare.sv (not committed until production
fix qualifies). Fresh cold DDR220M run session48272, cedar-native23-ddrcheck.log,
obj_native_rom_cedar_native23ddr. Separate Quartus pipeline session6695,
misterubuntu:/home/alans/mister/Dorado_Native23_20261017/core. Only functional
change from Native21 is d13; old placement-specific hold exception removed
before refit. No production change or deployment yet. Native22 remains a
secondary slow-clock control. Next: verify the equality path, cold boot and STA.

**First bad map operations identified, 19:12 UTC.** CompactVM's bounds in
cache-visible germ globals are correct: GF09d0, pageGerm09da=3e02,
pageAfterGerm09d4=3e27. Yet the lower-bound equality path proceeds to vacate
VP3e02 at167079245,3e03 at167085893,3e04 at167092101,3e05 at167099229.
The following SDBM1 store faults at VA3e056e and enters the queue loop.
Evidence reports/2026-10-17-germ-queue/first-germ-map-fault.txt.
DCOMP's equal path produces zero and writes it into stack slot1; LI0 writes
slot2 correctly. The subsequent JLB/Overflow condition path is under active
edge-by-edge investigation; stale-stack suspicion is NOT established.
Scratch cedar-compare-flags.log / module-checkpoint/watch_compare. No production
comparison/flag change yet. Replay167M..167.12M from roll1 takes seconds;
keep the immutable160M snapshot and both rolling checkpoints.

Cold200M DDR-adapter run COMPLETED: original Initial and Cedar images PASS,
8931 sampled source germ words correct, IFU1812418 fetches/1813208 acks,
186611683 all-six-enabled clean samples. Same queue loop after~168M.
This is a full cold component run with actual DDR adapters and storage pauses;
it does not qualify OS boot. cold-ddr-gate.json preserves the checker result.
Native22 Quartus pipeline completed; slower-clock cold simulation still running.
No Native22 deployment; MiSTer14 remains Native21.

**Post-germ fault narrowed,19:02 UTC.** The long original-storage diagnostic
reproduces RequeueSearchSQ around168M. Its cache-aware cursor is812,
`germInLoad`; saved FaultParam0/1 give VA0x3e056e inside the germ, and
FltEmuPC013f is SDBM1. FaultVal=3, FaultMapVal=3, FaultInfo4080;
WDC1, CurrentPSB=Process0400, IndexMask0ffc, RTemp0/1/2=0. Driver saved
module-checkpoint/fixed200m.rtl at199999900, with no disk writes; inspector
output fixed200m-inspect.txt. This is a POD diagnostic, not a cold full gate.
Its reporting coroutine does not complete at the C++ host stop, so the
standard checker has missing IFU/Initial summaries; do not call those zero
actual execution. Keep the cold100M qualification separate.

The actual FPGA screenshot's three downsampled pixel rows match812 among
known MPCodes using Xerox's font. This is an inference from the screen;
raw DDR still reads810 and can be stale because it omits dirty cache.
The earlier assertion that810 bounds the current on-screen boot stage was
too strong. See reports/2026-10-17-germ-queue/screen-match.json and cursor-cache.json.

Active first-queue replay: session67810, cedar-first-queue.log, scratch
module-checkpoint/watch / checkpoint_watch.cpp. Exact same RTL model as
modulecheckpoint, j01 delay1, restores85M, traces from166M, rolls two
checkpoints from160M and stops after10000 task0 samples in queue PCs.
Do not overwrite the rolling checkpoints before inspecting the first fault.
Cold DDR run session43707 was extended at98436143 to200M by changing ONLY
host runcycles, validated exact compiled offsets/live values, then detached.
No hardware state writes. Evidence native21-ddrcheck/extend-limit.log.
It matched all70 progress points through70M against the cold original-storage run.

Independent clock bug measured: BaseBd01.sil says Xtl=10Mhz; MC12061 uses
fixed /8 fabric divider. SYSPER4 nominal probe measures80-sample MCPreClk,
1280-sample Pendulum/Refresh; expected133.333 and2133.333 samples for1MHz
and16us. Native timers1.667x fast relative to guest; this also confounds
Native22's slower-clock control. Do not attribute a result to phase alone.
No production oscillator fix yet. Report2026-10-17-baseboard-cadence.
BasicCedar boot layout was parsed:1047 data pages, max virtual page06b8,
full1060 pages; no evidence this payload needs a bigger map. FaultVA3e056e
fits the current16K-entry map. Preserve this arithmetic against stale older notes.

**Active control runs,18:45 UTC.** Native22 is a separate build copied
from21: only MACHINE_SYSPER changes4->8; Native21's placement-specific e04
hold exception is removed before refitting. Project misterubuntu:/home/alans/
mister/Dorado_Native22_20261017/core, pipeline session56613, local
native22-build.log. Do not deploy without functional/STA/clock gates.
Cold8-sample simulation session99662, cedar-native22-slow8.log,220M limit,
object obj_native_rom_cedar_native22slow8. Reply transition times and delay
are doubled from the passing4-sample stream, preserving all packet data.
Scratch native22-candidate. MiSTer14 remains Native21.

C reference uses a DISPOSABLE copy cedar-reference.pdi, standard C IOCB
bridge, and is only a software-control-flow reference: MP810 at BB68200551,
MP812 at93400790. It is not an RTL or FPGA boot. Full log compressed as
cedar-reference.log.gz; initial missing-disk attempt was rejected/stopped
and preserved separately. Source hash comparison covers171 remote/local
board/cell/wrapper files: only MC10170 comments, simulation-only storage
assertion, and unused generic dorado_backplane differ. Native21 MemX hash
matches exactly. No functional cell drift found.

Diagnostic checkpoint inspector module-checkpoint/inspect reads exact-model
POD snapshots without advancing the machine; validates the fingerprint,
dumps RM and cache-visible physical cursor words. It succeeds on baseline85M.
It inherits the snapshot's non-POD limitations; no FPGA/guest writes.

**Maintenance-panel discovery,18:39 UTC.** The tiny mark in the actual FPGA
screen is not just an empty cursor: the six decoded cursor words at0431B
are318c/4a92/3096/489a/4892/33cc. Xerox's ProcessorHeadDorado digitFont decodes
these exactly as810; MPCodes.mesa calls it germStarting. This proves guest
code reached the germ's cursor output, but not DoInLoad or an OS prompt.
`python3 tools/decode_dorado_mp.py build/rtl-executive/cedar21-physical-memory.bin`
reproduces it; reports/2026-10-17-native21/maintenance-panel.json preserves it.
BootSwapGerm has several810 call sites: do not infer which one from the final
code alone. Its Initialize calls MesaRuntimeInit.Start and ProcessorFace.Go
before walking available VM, allocating buffers and returning into Run.

**Native21 FPGA result,18:33 UTC.** Original Cedar request at18:28; C
released correctly. Hardware remains in RequeueSearchSQ239/23a/6c1/6c2 with
white raster/tiny cursor, no CRC/RX errors. Screenshot cedar21-first-fpga.png
in build/rtl-executive. Read-only DDR mmap (PROT_READ at0x30000000,4MiB)
decodes ALL8960 source germ words correctly, except the two expected PilotBoot
request stores02f0=2/02f1=3. Partial relocated germ visible at physical0xfdc00.
Decoder is partially calibrated and omits dirty cache: target zeros do not
prove missing data. Source transfer is now fixed on hardware; OS not booted.

Active diagnostics: cedar-native21-ddrcheck.log (session43707), cold full
component with the actual msa_ddram/group/arb/ddram adapters and a raw Avalon
memory peer; global machine clock pauses using the same falling-edge enable
as FPGA. Separate host clock continues, no CPU/IOCB bridge. Native21-fixed
MemX. Scratch native21-ddrcheck, object obj_native_rom_cedar_native21ddr.
Also cedar-native21-extended.log (session89127): diagnostic original-storage
model restored from baseline85m.rtl with j01 delay1, advancing to200M and
saving fixed200m.rtl near the end. This POD restore excludes non-POD BaseBoard
internals, so it is a diagnostic only; cold and FPGA checks still required.

**Native21 hardware boot started,18:24 UTC.** Commit ebb027ac pushed.
RBF programmed through Main's load_core/MGL path on MiSTer14. All copies hash
identically; fresh pack hash c0cd6784... matches baseline. Original immutable
Main/server unchanged. Main opened real input devices, mounted pack0, then C
watcher was armed and three real boot pushes sent once. Current log:
/media/fat/games/DoradoRTL/Cedar21_20261017/main.log. Wait through the original
firmware ~4.5min startup. Watcher log cedar21-key-watch.log releases C only on
a new BFN113. No OS boot claimed yet. Previous Native19 log copied to19/
main-before-native21.log; open FD follows renamed21/main.log.

**Native21 qualified, 18:24 UTC.** Canonical generator and MemX now contain
only the one-stage j01 pin13 module-selector clock fix. Full cold100M with
four storage modules and authentic BaseBoard pulses passes:8931 sampled germ
words correct, IFU79377 fetches/79468 acknowledgements,15 task initializations,
86611683 clean all-six-error-enabled samples. The small captured module3-to0
regression passes all16 words; old RTL fails word0 DEED vs06EC. Map-hold passes.
Quartus fitPASS; reviewed STA PASS29 checks, minimum+0.097ns, zero illegal/
unconstrained clocks. Exact ContA e04 RAM address hold proof1 source/3 sinks
is preserved with full before/after reports under reports/2026-10-17-native21.
RBF SHA a11ef8f66ee58b2f56d9a8dca45957f6db88b98e66868903b813b50b8495538c.
Next: load Native21 on allocated MiSTer14 with a fresh pack, immutable Main/
server, original firmware three-push boot and C watcher. Actual Cedar OS
login, typed command and screenshot remain required; hardware still Native19.

**Native21 pre-build, 16:55 UTC.** Isolated project
misterubuntu:/home/alans/mister/Dorado_Native21_20261017/core, session44044,
builds fixed one-stage local j01 clock pin delay on otherwise Native19.
Generator/RTL scratch: native21-candidate. Overlay SHA256
548dceff5288404d494874319ce5ccb41aeedfc100e8fe514f682262ad3372c0,
MemX77635850117d27461cd0d7c33d8c784c6bd750e7e3fa847d7d879e1062782b38.
It has no Ab/f04 Native20 changes. Do not adopt or deploy until full germ
copy/IFU gate, STA and clock audit pass. Actual hardware remains Native19.
The MSA c5 leg experiment is rejected: after58M it is still in Initial's
network loader and the Cedar map-write gate is0/16. Its log is preserved;
there is no production MSA change.

**Reproducible four-slot fixture and ongoing experiments, 16:50 UTC.**
`make -C verilog/verilator initial-rom-fourmodule-test SYSPER=4` passes
16M with15 task initializations and2,611,683 clean samples with all six
errors enabled. It uses separate obj_initial_four, all8 storage clocks,
Mb0..3 and +native-clock. Optional +bootio-germ-copy preserves the source
copy in a bounded trace. `tools/check_cedar_rtl_germ.py LOG` passes the
one-module actual-clock run and rejects the four-module control with45
unexpected words / no IFU fetches. Evidence is committed under
fpga/mister/reports/2026-10-17-fourmodule-reproducer and native20-rejected.

Active cold runs (SYSPER4, four modules, actual BaseBoard clock):
- `cedar-module-handoff.log`, session36180: diagnostic MemX j01 local clock
  delayed1 sample, ordinary Native19 elsewhere.
- `cedar-msa-clock.log`, session70185: diagnostic c5 leg delayed2 samples
  before the SO-qualified MSA Sin capture clocks, Native19 MemX.
Both are isolated under module-handoff-candidate / msa-clock-candidate,
not canonical and not qualified. The first attempts used default SYSPER16;
only our two processes were stopped and those logs retained as
*-sysper16-aborted.log. Current restarted runs explicitly use SYSPER4.
MGATE observer covers95330300–95330600; MPATH covers95315000–95333500.

Combined model/checkpoint driver built under module-checkpoint. Cold baseline
session66916 (`cedar-module-checkpoint.log`) will save at85M before disk
input. It uses the existing POD-state descriptor, refuses disk writes and
checks an exact-model fingerprint. Short15k->20k restore comparison has no
core processor/memory differences, but differs in unused BaseBoard6502
internals and trace-history bytes: the transistor CPU has non-POD state.
Therefore require a baseline replay to reproduce the original copy stream
and handoff before trusting it for diagnosis. Clock knobs change only timing
adapters, not guest words. Every proposed fix needs a fresh full cold run
and actual FPGA qualification; these checkpoints never go to the FPGA.

**Native20 rejected by the full functional gate, 2026-10-17 16:38 UTC.**
The Ab/f04 candidate did not change the boot result: source-copy comparison
still finds the identical47 mismatches (45 unexpected plus2 legitimate
PilotBoot request stores), source0200=387f, IFU reference/ack0, queue loop.
It completed98M with original3600IM/256IFUM intact. Do not promote the
scratch generator/RTL or deploy Native20. Its isolated Quartus fit completed
successfully19m01 and STA ran, but functional qualification fails.

The bounded serial trace narrows the fault beyond the earlier f03 guess.
At95330471 the first word of the module0 source0200 fill comes from
**module3** (s3e=387f);95330475's second word is also from module3
(s3o=ffd1). At95330479, module0 supplies the correct third word0518,
then the rest of the line. ModSinEn changes0111->1110 at95330467.
The original MSA h02 contains two module-enable stages (Q4 pin14, then
Q3 pin13 through h01's OR with SO) before the registered Sin outputs.
MemX j01 is the final Sin selector; it loads through inverted LoadSinO
on Clk0'Bc. The f03 Ab change is upstream and does not repair this boundary.
Use original schematics MemX09 (PDF page11) and msa01 (PDF page3), whose
wiring agrees with the generated netlist. A tiny scratch replay lives in
`build/rtl-executive/module-switch-probe/`; it is diagnostic, not a gate.
Canonical remains Native19; FPGA remains Native19 after clean Linux reboot.

**Four-module reproducer and Native20 candidate, 2026-10-17 16:18 UTC.**
The old RTL fails in the component model as soon as it has four original
storage modules: at100M it retains3600IM/256IFUM and86,611,683 clean
all-error-enabled samples, but IFU reference/ack counts are both0 and task0
repeats239/23a/6c1/6c2, like the FPGA. Germ source0200 reads387f instead of
06ec;0304/0305 remain09d0/09e0. There are45 unexpected differing sampled
words, all at offsets0/1/2 of sixteen-word lines (plus the two intentional
PilotBoot request writes). Several wrong words match other germ lines.
Log: `build/rtl-executive/cedar-fourmodules.log`, MGo95,331,274.

Pulse width alone is ruled out: the one-module run driven by the actual
BaseBoard clock finishes106M with776,496 IFU reference/776,866 ack
transitions, exact original IM/IFUM and92,611,684 clean all-error-enabled
samples. Log: `cedar-nativepulse.log`.

The next shared-clock problem is MemX f03: it selects the storage module
for incoming serial data and shares Clk1'Ab with the f04 MCR flags. The
three-stage MCR setup adapter therefore delayed module selection too.
An isolated candidate restores Ab to the ordinary one-stage phase shared
with Aa, then adds two stages only at f04 pin13. f04 sees the identical
three-stage baseline waveform; f03 and the a03 storage-strobe selector
again share a capture edge. No clock generator, data or write-strobe timing
is changed. Fast map/clock regression passes, while Native19 fails the new
f03/a03 clock comparison. Original-ROM16M with four modules and authentic
BaseBoard pulses passes15 task initializations and2,611,683 clean samples.

Candidate files are under `build/rtl-executive/module-select-candidate/`;
**canonical generator/RTL remain Native19 until the full boot passes.**
Full110M four-module/authentic-clock run: `cedar-module-select.log`
(session1791), bounded source-copy trace starts94M. Old four-module probe:
`cedar-fourprobe.log` (session23384,96M), MPATH window95,315,000–95,333,500
records all eight MSA serial outputs, resolved input, receiver/cache words
and module enables; real RM/map inspection follows that window.

Native20 builds separately at
`misterubuntu:/home/alans/mister/Dorado_Native20_20261017/core`
(session79983; synthesis passes2m17, fitting active). It is frozen Native16
plus the committed memory-DMux fixes and this candidate. Overlay SHA256
`8aa84ae27b783e1256cf15789e05e31b59ff7e774772f79c90192f1394a33aff`,
MemX SHA256 `5920b3f7bfa81ecb05aba2d65abaa3a32b0b404f09fb81720c52c2c1720e4ca4`.
Require complete functional gate, STA and clock audit before loading. Fresh
Cedar20 medium matches c0cd6784... and its MGL/profile are staged; no RBF
is on the SD card yet. Live FPGA remains Native19, no OS text. When loading
Native20, preserve/move the CURRENT open log
`Cedar19_20261017/main-after-linux-reboot.log`, not the older main.log.

**Native19 actual outcome and clean reboot, 2026-10-17 15:50 UTC.** The FPGA
still reaches RequeueSearchSQ239/23a/6c1/6c2 with a white raster/tiny cursor,
not Cedar OS text. Original BFN113 transfer succeeds, C releases. A read-only
DDR snapshot now decodes source0200/0304/0305 as06ec/09d0/09e0, confirming
the A-bank clock fix affects real hardware. The decoder remains partially
calibrated and excludes cache; do not infer other physical contents from it.

At the user's suggestion, Linux on MiSTer14 was rebooted15:38. Exact immutable
Main/server arguments, cwd and Dorado-only environment were saved and
restored; Main log is now Cedar19_20261017/main-after-linux-reboot.log. Main
startup arguments alone do not program the FPGA: load the MGL through Main's
load_core interface, wait for input initialization, then start C selection
and the three-push boot once. That proper clean run started15:42; BFN113
again succeeded (TXgood2/CRCbad0), C released, and the same queue loop remains.

Two isolated component runs are active locally. `cedar-fourmodules.log`
uses four original storage modules, matching the FPGA capacity; scratch
fixture supplies all six extra storage clocks and Mb1..3. Its observer is
limited to germ-copy reads/handoff, then the first million instruction
samples, to avoid losing the copy to a fixed trace limit during longer init.
`cedar-nativepulse.log` keeps one module but drives every board clock from
bb_manifold.CLK_ca_p___drv, the actual BaseBoard clock output. A short board
measurement proves Native SYSPER4 is low one tick/high three, while the old
component fixture's square clock is low two/high two. Captured waveform:
`fpga/mister/reports/2026-10-17-native19/baseboard-clock-shape.txt`. Do not
change the real clock generator merely to match the synthetic bench; first
determine which boundary fails with authentic pulses. Both runs use original
Cedar/germ/media and the committed MemX correction, with no guest writes or
forced execution state beyond the pre-existing direct-entry fixture.

**Native19 qualified and loaded, 2026-10-17 15:29 UTC.** MiSTer14 now runs
RBF SHA256 `57bb42028e3441d0d82c972e7e864e4280cf513e15b44c40928c2abba1185de8`.
All29 timing checks pass, minimum+0.128ns, with zero illegal/unconstrained
internal clocks. The original slow hold failures were two MemX e20 a_lat[1]
to its own F10145A RAM write-address bit1 endpoints, -0.050ns. The exact
one-source/two-sink hold review proves !p13 address capture excludes the
p13&&!we_d&&ce_lat write commit; all setup/read/data/enables remain timed.
The bitstream is unchanged by the review. Reports:
`fpga/mister/reports/2026-10-17-native19/`.

Fresh Cedar19 medium matches baseline c0cd6784..., and immutable Main/server
are unchanged. Native18's log is preserved at main-before-native19.log;
Main's open log moved to Cedar19_20261017/main.log before the MGL load.
Three real boot pushes completed once. The original firmware is running;
the C-key watcher releases at the new BFN113 request. Allow firmware waits
to complete. No Cedar OS screen yet.

**Map and storage clock correction, 2026-10-17 15:12 UTC.** The component
machine now executes Mesa instructions from the original Cedar germ. MemX
c02/e23 capture the same storage write/CAS control for the A/B strobe banks,
but Native18's Aa5 timing separated their clock edges and corrupted germ
data. Aa is restored to the ordinary one-stage clock shared with Cb.
Three stages delay preFH'A before c07 pin5 instead of delaying the gate's
qualified MapbufLd outputs; HoldMapBuf can then close the latch promptly.
Ab alone retains the three-stage MCR setup. The expanded map replay checks
the A/B capture clocks as well as map hold/release: new RTL passes, Aa5
fails. Original-ROM startup passes at SYSPER4/16 with all six errors enabled.
Actual first16 map RAM writes pass. Germ-copy trace comparison improves
from265 differing sampled words to2, with the former broken roots now
06ec/09d0/09e0. First IFU acknowledgement is76,410,144, CIA41e, VA03e06ee;
Mesa execution continues. The two source differences at02f0/02f1 are the
original PilotBoot.mc boot-request writes: pRequest=1360B,
bootPhysicalVolume=2, sa4000=3 (definitions18–34, stores58–63). Thus all
sampled words match the file or those specified writes. Thirty of8960 words
were not sampled, so this is not a full checksum.
The completed100M run retains exact3600IM/256IFUM, records600,500 IFU
reference/600,870 acknowledgement transitions and86,611,683 samples with
all six errors enabled without error. SYSPER16 startup has10,422,953 clean
all-error-enabled samples.
Evidence: `fpga/mister/reports/2026-10-17-map-clock-leg/`.

Native19 is building separately on misterubuntu, cloned from frozen Native16
plus the committed memory-DMux fixes and corrected MemX. Canonical cleanup
emits identical RTL tokens. Full STA and clock coverage are required before
loading. MiSTer14 still runs Native18; its actual14:35 screenshot is white
with a tiny cursor, no OS text. The C-key watcher completed and released C
at the BFN113 request. Fresh Cedar19 medium has the same c0cd6784 baseline
hash. Neither Main nor the server was replaced. Cedar is not yet booted on
the FPGA.

**Latest hardware state, 2026-10-17 14:30 UTC.** MiSTer14 now runs
Native18, SHA256 `25e4f127a7490e7852ea0d7d91928297a7636abfa80de3d1bf349a713764a42f`.
All29 timing checks pass, minimum +0.091ns, with zero illegal/unconstrained
internal clocks. Original slow hold -0.013ns was four MemC d06 a_lat[1]
to its own RAM write-address bit1 paths. The exact one-source/four-sink
HOLD review uses the same !p13 capture versus p13&&!we_d&&ce_lat commit
proof as the earlier F10145A reviews; setup/read/data/enables remain timed.
Before/after reports: `fpga/mister/reports/2026-10-17-native18/`.

Native18 uses a separate fresh Cedar18 pack (baseline SHA c0cd6784...),
fixed720p60 profile and the same immutable Main/server. The Native16 log
was copied to main-before-native18.log, then the open log was moved to
Cedar18_20261017/main.log. Main's executable was not replaced. Three real
boot pushes requested CoolBoot once; the firmware is at CPUfc7 with live
F4E0/F693 BaseBoard PCs. Allow the firmware waits to run. C is held by the
watcher and releases at the next actual BFN113 request. No Cedar OS screen
yet. The component run reaches the germ handoff but still reads corrupted
source pointers before the copy; a read-only physical-storage inspection
is running to separate storage contents from cache/refill corruption.

The first map-write trace proves that the requested page reaches Mapbuf,
then the next instruction's saved Link overwrites it before the RAM write.
An expanded passive trace includes the unregistered hold input and
the MemC pair-load clock: the hold request becomes valid two fabric ticks
after c02 samples it. The following instruction overwrites Mapbuf before
the next c02 clock. The native
read-only DDR dump corroborates the alias: decoded physical pagee54,
offsete0 contains32 original germ bytes from file offset8640 (convert the
decoder's little-endian word bytes to the germ's big-endian order). The
original microcode's MapModuleCont is ate53, so its IWriteMap return Link
is exactlye54. The DDR decoder remains partially calibrated, not a general
memory-test qualification.

A separate memory diagnostic-bus bug is fixed in the canonical generator:
MemC l24, MemD l21, and MemX l23 now retain pre-edge serial data when the
BaseBoard switches the shared muffler bus to readback. Only those three
serial-input packages change. `make -C verilog/verilator memory-dmux-test`
checks every intermediate bit across all 2,048 addresses and 24,576 strobes
on all three original boards, with three pulse lengths; it passes. The
old RTL fails at strobe24 (expected001, all three boards read000).
Original-ROM regression passes at16M: all15 task initializations and
2,611,683 samples with all six error sources enabled, no errors. No new
FPGA RBF was built or deployed at that point; Native18 now includes this fix.
Committed/pushed as `d55d7e4c`. The55M Cedar component run also confirms
VirtualBanks=0040 (16K map), replacing the incorrect0400. Original IM3600
and IFUM256 still match; the map-data overwrite remains a separate bug.

**Map hold fix, 2026-10-17 14:10 UTC.** The generator now changes
only MemX Clk1'Aa from three to five fabric delay stages. Clk1'Ab and the
three Mapbuf latch gates retain three stages. This moves c02's capture past
the measured MemC pair/hold setup boundary; the same net also clocks a03's
module-strobe register, so full memory/boot validation is still required.
The replay test drives the measured MemX boundary waveform: old RTL stores
the following Link, while the candidate retains the requested page. It
also checks that releasing hold permits another page to load. Original-ROM
16M startup passes with all15 tasks initialized and2,611,683 clean samples
with all six errors enabled. The SYSPER16 startup gate also passes at64M with10,422,953 clean
all-error-enabled samples.

The90M Cedar component run checks the first16 actual DRAM
write-data values against the original one-to-one map loop, then continues
through disk/germ boot. Its observation window arms after task0 actually
executes Cedar entry238, rather than assuming a fixed download duration.
The full Cedar run passes the first16 actual RAM writes (pages0..15),
so the clock change is now adopted in the canonical generator. Its output
is byte-identical to the frozen Native18 MemX candidate. Isolated Native18 Quartus
build is running on misterubuntu from frozen Native16 plus the three
committed muffler fixes and this candidate. Source overlay SHA256
`0e577794e71bdb75b2f695ba3860e1f46aebc334b89c01c25761fe39fb5d6b17`.
Full90M readback passes3600IM/256IFUM, all15 task initializations, and
76,611,683 clean samples with all six errors enabled. At55M the raw map
contains13,145 distinct RP values while cache-flush vacating is in progress;
the old map held only the caller's Link. The germ reaches MGo at76,409,426
but still fails before the first IFU fetch: source0304/0305 already read
4900/0900 before the copy, versus original09d0/09e0. Disk FIFO data at those
positions was09d0/09e0. Most other copied words match, with corruption
concentrated in several pages. The remaining/new memory-store/refill problem
is not yet localized. Native18 may help compare its DDR-backed storage with
the component DRAM path, but it is not a demonstrated Cedar boot. Require
full STA and clock-coverage qualification before loading any RBF.
Evidence: `fpga/mister/reports/2026-10-17-map-buffer-hold/`.

**Actual hardware frontier, 2026-10-17 11:46 UTC.** Native15 passes
CD2 with Stop=0 and reaches ROM Initial's Ethernet reply loop C66/C6F/C75.
Main's TX good/bad counters remain zero; server receives no request.
Cause is now proven: with BaseBd present, the generic backplane generator
internalizes XmtData' because BaseBd also consumes it. The native wrapper
then leaves ethernet_tx_n undriven. Its required binding existed in the
Python dictionary but was silently skipped because the port did not exist.

`sil_backplane.py --export-net "XmtData'"` now exposes the resolved cable
while retaining BaseBd's input. Native generation requires all external
network/video/terminal/disk device ports, failing if any are missing.
`make -C verilog/verilator native-cable-test` checks the entire generated
native wrapper with the machine paused: a driven DskEth cable reaches both
the external output and BaseBd observer for sixteen samples. The frozen
Native15 fails drive=1/external=0/BaseBd=1; corrected Native16 passes.
This gate is boundary connectivity, not a CPU or protocol boot test.
Evidence: `reports/2026-10-17-native-cable/`.
`initial-wire-test` additionally replays the actual Initial request waveform
through the native1024-word MiSTer packet bridge with605 clock pauses:
all13 request words and the controller-generated CRC are accepted.
The test excludes only the capture's power-up settling at samples0/14.


**Native16 timing-qualified and loaded, 2026-10-17 12:30 UTC.**
RBF SHA256 `fba5c62e98aad5377ef47be34fd007ff41cf48497f7b01132b01e37d00f3a443`.
All29 timing checks pass, minimum +0.128ns, zero illegal/unconstrained
clocks. Initial -0.055ns slow/-0.001ns fast hold endpoints were MemD h04's
own F10145A address-latch to write-address paths, with the same mutually
exclusive capture/write enables audited for Native15. Only those exact
HOLD transfers were excluded; RBF unchanged. Before/after reports and
reviewed SDC: `fpga/mister/reports/2026-10-17-native16/`.
Frozen overlay SHA256
`a262b057698f63ee5e297faca236b8a11e659338c426c539efca774d88ccb9bf`;
remote project `misterubuntu:/home/alans/mister/Dorado_Native16_20261017/core`.
Actual MiSTer14 log: `Cedar16_20261017/main.log`. Prior C helper13431 was
stopped/released before loading; fresh C helper session62200 and three
physical boot pushes started after Main opened its input devices.
Firmware currently waits at CPUfc7, with BaseBoard F4D5/F692 active.
Native15's final pre-load tail had returned to CD2/Stop1, despite earlier
Stop0/reply-loop samples. Its boot was not stable; preserve that distinction.
Cedar is NOT yet booted on FPGA.

**Real Cedar-network handoff, 2026-10-17 12:34 UTC.** Native16's cable
export works on MiSTer14: TXgood=1, CRCbad=0, and the ARM server records the
original13-word station042/BFN113 request. CPU advances into Cedar microcode
with task2 atd86 and display task3 active. The boot-selection C key was
released after this request. Actual FPGA screenshot
`build/rtl-executive/cedar16-first-fpga.png` is a blank raster, not an OS
screen. Task0 repeatedly executes239/23a with6c1/6c2: these decode to
RequeueSearchSQ, not the BootTransfer wait (that is e0a/e03). No IFU fetch
has begun in the corresponding component run. The newer trace below reaches
the germ handoff but fails before executing its first Mesa instruction.
A read-only mmap of reserved DDR0x30000000..0x303fffff captured the actual
raw storage. Some decoded chunks match the original matched germ byte for
byte. The mapping decoder remains a diagnostic, not a qualified memory test.
No guest state was written. Main/server remain the immutable versions above.

The component run was deliberately ended at320M (2993s) after repeating
this same loop. Loaded IM3600/IFUM256 still match, all parity sources clean,
IFU reference/ack transitions both0. Disk served4177 sectors and12528 blocks;
only3 tracks were loaded, ending cylinder4. Exact newer focused run is
session3177, `cedar113-disk-frontier.log`,265M bound with BOOTIO from249M.
The first55M inspection used defaultSYSPER16 and completed before Cedar;
its loaded-image check correctly failed. The replacement session61952 used
SYSPER4 and the Pilot pack and passed3600IM/256IFUM at55M. Its new dump
exposed an existing diagnostic-reader bug: ProcH h06/j06 use the true RBase
nibble while ProcL uses its complement. The correct physical RM indices are
ProcH `a^0x0f`, ProcL `a^0xff`; both nibble data fields reverse. The helper
is corrected. Baseline raw dump `cedar55-rm-map.txt` predates that correction;
`cedar55-rm-map-corrected.txt` recombines the two raw halves without rerunning
or changing guest state. R400=0100, VirtualBanks=0400, PgsPerMod=0400,
RealPages=0400, ModMask=8000. VirtualBanks indicates a256K-entry map while
MosRam models16K. More seriously, all16384 raw map entries hold RP0e35
while the one-to-one loop's page counter is5e86. The first16 raw planes
are contiguous Mapbuf.00..15 (do not move a11 to bit15; that was a
diagnostic decoding error that reported0e6a instead of0e35). A scratch-only MemX c02
PRECAP0->1 candidate reproduced that exact bad map at55M; it is rejected.
Original Initial parity and loaded IM/IFUM checks passed. Its owned Native17
Quartus pipeline and long simulation were stopped; no candidate was deployed
and production RTL remains unchanged. A new passive `+mapprobe` records
BMux, Mapbuf, the expected page number, latch gate and RAM strobes around
Cedar's first IWriteMap. Initial uses the same IM addresses, so its default
arming time is49,570,000 rather than an address alone.

The265M focused trace reaches `GermRemapDone` (5d5) and `MGo` (7e0)
at251,964,474. BootDataPtr=2500 and copy length2300 are correct: all35
germ pages, starting at0200. The first control link at MDS+02fe reads0101,
matching the original6.1.6 germ. The subsequent MDS+0304/+0305 reads return
zero where the original germ contains09d0/09e0. Fault/interrupt handling
then enters RequeueSearchSQ at251,966,482. Thus the immediate frontier is
corrupted or incorrectly addressed germ data before the first IFU fetch,
not a truncated boot file or a successful Cedar OS start.
`build/rtl-executive/cedar-first-queue-transition.txt` preserves the transition.
The old BOOTIO full RM fields used the incorrect high-half reader described
above; CPU, ALU, MD and VA samples are independent of that helper.

Native16 was retried with automated C release at the first real BFN113.
The first watcher lost partial stderr lines; that run is not an early-release
experiment. The corrected watcher session86209 confirmed the new request
and released C immediately, but the same queue loop remains. This rules out
held C as a sufficient cause. No key helper remains active after86209.


**Actual Native15 loaded, 2026-10-17 11:34 UTC.** MiSTer14 now runs
`DoradoRTL_Cedar15_20261017/Cedar15.mgl`, RBF SHA256
`cb7a541fd4bd1bde02ad958afad37ff99a75d6d1c65d38914e43f16c39596b4b`.
All29 timing checks pass, minimum +0.128ns, with zero illegal/unconstrained
internal clocks. Original -0.106ns hold report is preserved. The only
constraint review is three exact F10145A own a_lat-to-write-address HOLD
pairs (DskEth j11/j09, MemC k06): !p13 capture vs p13&&!we_d&&ce_lat
commit are mutually exclusive, the same audited proof as checkpoint cores.
Setup, read, data and enable paths remain checked. RBF unchanged by review;
fixed720p60 only, framework I/O delay coverage remains separate. Evidence:
`reports/2026-10-17-native15/`. Cedar is NOT yet booted on FPGA.
Main fd/log moved to `Cedar15_20261017/main.log`; fresh C helper and
three-push cold boot are being started after Main opened its input devices.
Fresh writable pack has baseline SHA256 c0cd6784...; immutable Main/server
unchanged. Native14 still routing, superseded if Native15 boot is sound.

The extended component fixture confirms the missing-BaseBoard diagnosis:
real generated serial-manifold gates pulse IOReset at samples49,570,809
and49,571,897; the stale disk task clears and Cedar map/cache initialization
continues. This is still a direct-entry component fixture, not native boot.
Its 150M bound is adjustable through
`build/rtl-executive/cedar113-bbman-limit.txt` (once per million samples).

**Component loader proof, 2026-10-17 11:20 UTC.** The B-only Ethernet
fix downloads and installs the original Cedar image: 3,600 writable IM
items and all 256 IFUM entries match. The 64 protected LoadRam words are
excluded. PCT explicitly captures CIA238 in task0 at sample49,570,331,
so the entry claim is not based only on an IM-address census. The 100M
Pilot-pack run then parks at09c (MesaIFUNotReady) in task12, with no IFU
memory request or byte clock. Trace identifies a component limitation:
this full bench has no BaseBoard and its IOReset input is held low after
startup. Cedar requests an I/O reset through the absent BaseBoard manifold;
stale disk wakeups then enter the replaced task12 code when tasking resumes.
Without a pack the stale display task instead enters Cedar map/cache code.
Do not diagnose either as an established native IFU or memory failure.
A new component run includes the generated BaseBoard manifold circuitry,
with explicit ECL ownership and processor-side setup; firmware/CP bus and
clocks remain outside this fixture. That run is not yet validated. Actual
FPGA still runs Native11 CD2; Native14 and Native15 are routing.

**Current frontier, 2026-10-17 10:56 UTC.** Actual FPGA still runs
Native11 and stops at ROM Initial CD2; Cedar has not booted on FPGA.
Native14 is routing with the Initial/MCR/register-parity and RIOT clock
fixes. Native15 adds the Ethernet transmit setup fix and has entered fit.
Require full STA plus zero illegal/unconstrained clocks before loading.
Native15 frozen overlay SHA256:
`a901ddf48c8043f4870141393797c76a735e8b0471f0fde6ac46c914cbffb4e0`.
Its project is `misterubuntu:/home/alans/mister/Dorado_Native15_20261017/core`.
Both builds have automatic post-STA clock audits.

Original-ROM Initial's request used to start with stale FFFF and lose its
last checksum word. DskEth l06 B (TxBusRegFull') sampled its next inputs,
allowing a FIFO write before the bus register captured the new word.
Per-half MC10135 capture parameters now allow PRECAP_B=1 there while
preserving A's receive-data-late behavior. Applying PRECAP to both halves
fixed transmit but failed damaged-CRC reception; that version was rejected.
Canonical B-only RTL passes FIFO parity and all eight receive phases with
good status and bad CRC rejection. `make -C verilog/verilator
initial-network-test SYSPER=4` passes 31M samples: all six errors enabled
for 17,611,683 samples, zero errors, 15 task initializations, and exactly
13 Cedar request words, station 042B, BFN113B, legal nil Pup checksum and
zero hardware CRC residue. These are component gates, not native boot.

The real ARM server accepts that decoded request and serves 31,376 bytes
in 62 data frames, byte-identical to CedarDorado.eb!6 after its 512-byte
header (SHA256 `83c81946859858da2ebfd8802ea592343bf7622b8822763d73578e0696b65d4e`).
A component receive run replays a real server BOL packet before the replies,
providing the normal receiver packet boundary rather than masking data-late
or patching guest state. It is still running; no world-handoff claim yet.

**Native boot frontier, 2026-10-17 09:45 UTC.** Native10 and Native11
independent placements reproduce CIA `6322B` (`0xcd2`), Stop=1, dStop=0,
with instruction parity good and no network transmission. The ROM's Initial
payload is a different build from the standalone `Initial.mb`: 204 of its
224 hunks differ. Decode BootBlock 1 at ROM `0xc016`, origin `0xc40`;
`0xcd2` is BootEmulator, not the standalone image's keyboard table.

A component simulation using that original ROM payload reproduces the exact
stop when the testbench releases its manifold fixture (`+cpu-manifold`).
Without that release, the fixture suppresses Initial's own manifold writes,
including enabling all six error sources, and masks this failure. Actual
memory-error attribution is STPerr (transport-byte parity), not IM parity,
HitPerr or a storage ECC syndrome. MCR's NoWakeups bit samples stale MapBuf
at `0xc43`; NoWakeups is needed to reset transport parity before boot.

The existing opt-in `DORADO_CLOCK_DEPTH_BOARDS=MemX` restores ordering and
clears this memory-parity stop in a 16M-sample original-ROM component run.
DRAM startup (`memrun`) also passes with it. This is **scratch diagnosis**,
not production RTL or a qualified native boot: the longer run next stops
at `0xce1` after a brief R/T parity mismatch at `0xcdf`. The phase trace
then isolated the cause: ProcL g16 samples the next RAM address's parity
while the operand latch retains the preceding read value. ProcH/L g16 now
use the existing F10000 PRECAP option to capture their setup value. With
that change and scratch MemX depth, the full 16M run passes, all six error
sources enabled, Stop=0 and no post-enable error events. Compute, operand,
datapath and single-step gates pass at SYSPER=4. This is still an Initial
component run, not network boot or a native FPGA qualification.
`build/rtl-executive/native-rom-{memx-depth-ram-phase,register-setup}.log`
preserve the failing phase trace and passing run.
Do not mask the errors, alter the guest, or treat the direct-entry fixture
as a BaseBoard boot qualification. The actual FPGA still runs Native11.
Native12 adds passive control diagnostics only. Its first hold report finds
exactly two more same-group own-address transfers (-0.025/-0.022 ns).
The existing disabled-transfer proof applies identically to every storage
group, so native_timing.sdc now enumerates all 128 exact same-group own-
address and own-data HOLD pairs. Setup, request/enable and cross-group paths
remain checked. Both storage equivalence gates and assertions pass. The
unchanged Native12 RBF now passes all 29 reported checks: minimum +0.246 ns,
core setup +2.431 ns, fixed-720p HDMI +4.862 ns. Evidence is in
`reports/2026-10-17-native12/`. Native12 is loaded for passive diagnostics;
it does not contain the two new functional fixes.

**Native13 builds the functional fixes**, using both c07 clock siblings
plus its three Mapbuf latch-gate siblings, and both g16 pre-edge captures.
`memrun-test`, `bootstrap-parity-test`, `exec-parity`, compute/operand/
datapath/step all pass at SYSPER=4. The reproducible
`make -C verilog/verilator initial-rom-test SYSPER=4` passes 16M samples:
15 task initializations and all six error sources enabled for 2,611,683
samples with zero errors. A longer 40M run passes the terminal wait with Stop=0. No boot key is
supplied in that fixture, and no network transmission occurs during it. This is still a component fixture, not the actual
FPGA boot. Project `misterubuntu:/home/alans/mister/Dorado_Native13_20261017`
uses frozen overlay SHA256
`f0c8e9d921ddad3d050ec5a6b094d4de566cbca0a4d1e5a10778d429f1ace7ad`;
Quartus synthesis is in progress. Actual FPGA Cedar remains unqualified.

**Clock coverage correction, 2026-10-17 10:20 UTC.** Native12's
29 nonnegative slack checks did not cover the BaseBoard RIOT clock.
`report_ucp` identifies g08.qa as an unconstrained internal clock; adding
its actual /80 relationship exposes hold down to -8.116 ns. This is a
real qualification gap, separate from framework I/O delays. Earlier
"all checks pass" statements below refer only to the clocks then analyzed.
The RIOT wrapper now uses sys_clk with a PHI2 rising-edge enable, and its
registered read output obeys that enable along with writes and timers.
`riot-clock-test` compares 27,074 PHI2 cycles with the original clocked
model: all RAM, ports, four prescalers and IRQ behavior agree. BaseBoard
ROM and serial manifold gates pass; a 100M-sample real-ROM five-board probe
reaches nonzero manifold programming (1,148 DMD transitions, 2,298 clocks).
That bound does not qualify native boot. Evidence:
`reports/2026-10-17-baseboard-clock/`.

Native13's owned fitter was stopped rather than finish a build with this
clock gap. **Native14** includes the clock correction and both Initial
fixes, frozen overlay SHA256
`7a71cfbfbda1eda78207d603a0a6ec7c342e5df28910ce1fe0fb5d8e9ff854a5`,
project `misterubuntu:/home/alans/mister/Dorado_Native14_20261017/core`.
Synthesis is running. Require zero illegal/unconstrained clocks using
`audit_native_clocks.tcl` and `check_native_timing.py` before deployment.
The actual FPGA is back on Native11; after the long firmware waits it
reproduces CD2. The intermediate FC7/SetRun=0 state is not a new failure.
The Main log retains its fd at `Cedar12_20261017/main.log`.

The component bench can now supply a physical C key, station 042 and an LF
terminal. It sends BFN113B, but the 80M run does not load Cedar: the request
begins with stale FFFF and the reply stream never completes at the guest
protocol level. This run used default storage timing; a comparison with
the FPGA's STORAGE_INPUT_DELAY=2 is running before attributing the fault.
The replay buffer now holds the complete 444,292-transition original Cedar
reply stream and rejects truncation. No guest code/state or completion
flags are patched. Cedar on FPGA remains the objective, not a milestone
already achieved.

The requested sequence is mouse input, display/software compatibility,
backing-disk persistence plus an ARM-local boot server, then unassisted
BaseBoard boot. Do not assume arbitrary Alto games should work on Dorado:
some install custom Alto RAM microcode that AEmu cannot execute. Use the
Executive cursor and actual guest input cells as the first mouse gate;
Invaders remains a compatibility probe, not a claim of perfect emulation.

Mouse bridge implementation: `fpga/mister/dorado_mouse.sv` consumes MiSTer's
25-bit PS/2 packets on the ungated host clock, accumulates relative movement
through machine pauses, reverses PS/2 Y, and maps left/right/middle to active-
low terminal word-4 bits 2/1/0. Reports drain only once even if the terminal
clock stops with its acknowledgement high. The checkpoint exporter exposes
the terminal's existing motion/button ports; CPU/memory state is untouched.

`mouse-test`, `mouse-terminal-test` and `keyboard-terminal-test` pass. A
14M-sample complete checkpoint/HPS simulation delivers `(40,+24)` to Alto
`0424B/0425B`, then `(-10,-12)` to reach `(30,12)`. Left press and release
reach UTILIN as `FFFB` and `FFFF` through DisplayAux.mc's ReadTerminal.
The inspected state is in `build/rtl-executive/mouse-full*.{bin,pgm}`;
`mouse-last.rtl` retains the exact model and medium state.

**The cursor moves on MiSTer14.** Run `/media/fat/Scripts/DoradoMouse.sh`
and wait for Executive. Main's remote virtual USB mouse delivers `(120,100)`,
then `(-60,40)`, then `(120,100)` through the actual PS/2/terminal path.
The final pair of hardware screenshots differs only by the same 51-pixel
arrow translated exactly 120 pixels right and 100 down. `foo` + Return
still prints its expected error and another prompt. **Calculator works with
the mouse:** launch `calculator`, then click `7`, `+`, `3`, `=`; its display
shows `10.`. This qualifies left-button application input on hardware.
Right/middle mapping is still covered only by the focused terminal tests.
[Cursor and keyboard](images/rtl-mister14-mouse-moved-2026-10-17.png),
[Calculator result](images/rtl-mister14-calculator-mouse-2026-10-17.png).

Isolated Quartus project:
`misterubuntu:/home/alans/mister/Dorado_Mouse_20261017/core`, RBF SHA256
`7d85f5e881f164dd4f180f70853e0f7dcf69998ce4f4c08069cb9fa88ed167b3`.
23,981 ALMs, 22,026 registers, 445 RAM blocks, 35 DSPs; core setup
+8.170 ns, hold +0.064 ns with the previous three scoped hold-only RAM
write-address exceptions. Framework I/O is not fully constrained. Evidence:
`fpga/mister/reports/2026-10-17-mouse/`. This build retains the previous HPS
medium while subsequent disk work proceeds separately. The previous
Executive/Invaders launchers remain intact.

**Disk persistence is qualified on MiSTer14.** A guest `Copy mouse.cm
com.cm` (Return confirms the missing-left-arrow prompt), `WriteDirectory`
and `Type mouse.cm` produce a 24-byte file. Host extraction of the actual
HPS-backed image returns the identical bytes. `Bootfrom sys.boot`, then an
FPGA reload retaining the disk followed immediately by another
`Bootfrom sys.boot`, both reach Executive and read the new file correctly.
The first size-before-mount notification race is fixed by
`dorado_pack_mount.sv`; the interrupted-remount protection remains active.
Project `Dorado_Network2_20261017/core` is installed as the separate Mount
test; setup +7.189 ns, hold +0.103 ns after a scoped ContA c04 F10145A
write-address hold review. See `reports/2026-10-17-mount/validation.json`
and `images/rtl-mister14-file-after-reload-boot-2026-10-17.png`.
This is an actual guest disk boot after checkpoint entry; unassisted
BaseBoard startup and abrupt-power-loss durability are separate.

**The ARM-local server runs on MiSTer14.**
`fpga/mister/server/` builds a standalone process from the existing Pup/
EFTP/STP source without the CPU emulator. Its socket test on both Mac and
actual ARM serves 18,728 AEmu microcode bytes and 7,080 Alto boot-file bytes
exactly, retransmits a deliberately dropped EFTP packet and emits idle
breath-of-life/routing broadcasts. It strips the C controller's dummy
CRC/status trailers; the old microcode-reply queue also needs its missing
nil Pup checksum supplied at this boundary. Main transport is implemented
against installed release 20260912 (`47221c18987e101f50caafeb3b615f53b62722ca`),
with 38 microcode frames tested byte-exact under RX backpressure on both
Mac and MiSTer ARM. Temporary Main is loaded using the existing per-core `main=` setting; the
stock binary is unchanged. Main must use `user_io_get_core_name(1)` because
MGL `setname` overrides its display/config name. Network3 delivers twelve
actual CRC-checked guest packets to the ARM service, with zero CRC errors
or drops. This is not yet a supported Pup receive/network-boot gate.

**Station-address bug fixed; full hardware EFTP transfer qualified.**
Zero straps take SIONop's explicit no-NIC branch. Station 377 also fails:
AltoEtherEmu right-justifies FF and ORs 77400B, producing the Alto absence
sentinel 77777B. Both generators now default to normal station 042B.
The real status mux gate checks 00/FF/042/2A; the selectable-station
Network5 build is deployed as `DoradoRTL_Station_20261017` on MiSTer14.
Executive EtherBoot sends a real Pup time request, Mayday, and ACKs for
all 21,820 words / 43,640 bytes of NETEXEC.BOOT!8, including final dally.
Hardware reports 90 good TX, zero CRC errors, zero malformed RX submits,
and zero aborted transactions. Two TX drops preceded the service connection;
the complete transfer adds none. **NetExec then leaves a blank display with
a tiny cursor; its executable handoff remains unqualified.**

Station RBF SHA256:
`2ec166c3433244f8db2a3eea8aad48a7fb415d96661d6a7270913f8eb949d201`.
24,136 ALMs, 22,482 registers, 447 RAM blocks, 35 DSPs. Setup +7.010 ns,
hold +0.100 ns after reviewing the same mutually exclusive F10145A
address-capture/write-commit hold path in DskEth j13. Only this instance's
own write-address hold checks are excluded; setup/read checks remain.
Reports: `fpga/mister/reports/2026-10-17-station/`.

Network4's fit has 24,151 ALMs, 22,606 registers, 447 RAM blocks and 35 DSPs;
setup +7.707 ns, hold +0.091 ns with the same four scoped RAM write-address
hold exceptions. Reports: `fpga/mister/reports/2026-10-17-host/`.
RBF SHA256 `5a9f6bad21e5f11fe365707670452cd35c926ca49e31699d38b050de95cd85a8`.

**ARM server re-exec registration race fixed.** Main can register its new
PID socket before the idle sender discovers the previous process is gone.
The service now probes the old Unix datagram address without sending data,
retires only a disconnected peer, and accepts the new empty registration.
A live second client cannot steal the session. The regression fails against
the preceding server and passes natively and on MiSTer ARM, including idle
breath-of-life without any guest TX. Current test binary:
`/tmp/dorado-main/dorado-server-reexec`; stock MiSTer Main remains unchanged.


`dorado_ethernet_wire.sv` is the next experimental link adapter. Its focused
test covers fractional 340 ns Manchester timing, CRC rejection, 512-word
packets, host backpressure and machine pauses. The checkpoint exporter can
optionally expose `RcvData` and `XmtData'` using `--ethernet-wire`; a complete
controller test is running before connecting the MiSTer transport. It is
not included in the deployed mouse or first persistence builds. That test
found the Ethernet crystal's fixed sampling ratio: at board SYSPER=4 it ran
half-speed. The generator now passes the crystal cell `2*SYSPER` (the cell
counts per 60 ns instruction, the board per 30 ns clock). The actual-board
gate passes at ratios 4/8/16. A captured real FileSystem lookup also proves
the CRC includes the sync bit: check word `7C50`, zero residue with sync,
`3FFE` without it. The link adapter and `eb_replies.py` now include sync;
the fixture tests that exact transmitted packet independently of the encoder.
The complete 100M-sample run now delivers four checked real transmitter
packets to the host buffer with zero bad CRCs. No server response is
injected in that run; the application is still in its discovery timeout.

The real receive test initially exposed another bug: j05's F9401 sampled
the new D value after the Rx FSM changed it on the falling clock edge, so
it saw zeros throughout the packet and accepted damaged CRCs. Its per-
instance pre-edge capture now follows the chip's setup value. The actual
Rx FIFO contains status `0000` for the correct `7C50` check word and
`0008` for a one-bit-damaged `7D50`, with identical payload and no framing/
overflow error. Eight relative input-clock phases pass. The published
F9401 check-vector gate is unchanged. This fix is in the separate final
`Dorado_Network3_20261017` build, not Network2. Both builds include the
mount-order guard; Network2 can qualify disk persistence while Network3
finishes. Main transport now resets on same-name core reloads, and the
local server accepts a fresh connection after the old socket disappears.

BaseBoard's isolated ROM boot gate passes (reset vector F3A7 and all eight
startup bytes match). Integration with the working full machine, medium,
boot server and boot button remains required. The current hardware still
continues saved RTL state from the assisted Initial/AEmu boot.

## Cedar memory and BaseBoard preparation (2026-10-17 UTC)

`dorado_storage_ddram.sv`, `dorado_storage_group.sv` and
`dorado_storage_arb.sv` move only the MSA's raw storage into MiSTer DDR.
The original board's address selection, serial bus, ECC and chip read/write
cycles remain RTL. A missing word pauses the complete machine while the
host clock services DDR; no ARM process reads guest RAM or completes I/O.
The proposed window starts at byte `0x30000000`, outside MiSTer14 Linux RAM
(`mem=511M`) and the template scaler's 8 MB at `0x20000000`.
`tools/sil_fpga_native.py` now assembles these with the real BaseBoard,
terminal and HPS medium without checkpoint initializers. Four-module
Quartus fit succeeds in
`misterubuntu:/home/alans/mister/Dorado_Native1_20261017`: 33,091 ALMs,
42,587 registers, 1,275,472 RAM bits, 175 RAM blocks, 35 DSPs. All reported
slacks are nonnegative; Dorado setup +1.719 ns, hold +0.235 ns. This native
hierarchy matches no checkpoint-only RAM exceptions. The RBF is loaded on
MiSTer14, where passive diagnostics show initialized DDR and running
BaseBoard firmware, but no network TX or working display yet. RBF SHA256
`c68a253a6c6bc17ebdb4d49a8a09c24b8b84f538ee22de19a8e7113c37ab7350`.
Reports: `fpga/mister/reports/2026-10-17-native1/`. Framework I/O remains
incompletely constrained. This is not yet a hardware boot gate.

`tools/sil_storage_ddram.py` consumes canonical `msa.v`, verifies all 144
chips and their common control signals, emits a separate `msa_ddram`, and
records each raw backing bit's chip mapping. Sixteen raw groups represent
one physical board. The backplane generator optionally assembles four
storage modules (eight even/odd boards), giving 1M logical 16-bit words.
This is a capacity increase from the current 64K-word FPGA continuation;
Cedar's minimum memory requirement still needs qualification.

Run `make -C verilog/verilator storage-ddram-test storage-group-test
storage-array-test storage-board-test`. The engine tests byte-lane
preservation, DDR stalls, immediate/delayed responses, backpressure and
reset with an outstanding read. Group equivalence compares 4,009 chip
cycles and every stored bit. Three-group arbitration compares 1,505 cycles
with 4,449 DDR reads and 463 writes, including reset drain. Complete-board
equivalence compares all serial/ECC outputs for 12,012 logical cycles,
2,976 reads, 7,872 writes and all 2,359,296 stored bits; the backing contains
6,986 set bits, so this is not merely an all-zero comparison.

An 800M-sample five-board probe executes the real BaseBoard ROM with a
physical boot-button pulse and nominal DAC/comparator environment. After
power-up watchdog transients it remains stable, passes its voltage checks,
programs the clock/manifold chain and writes MIR0/1/2 475 times each.
CVCC changes 3,210 times following the firmware's DAC, rather than being
forced to a static value. This probe excludes memory/IFU and **does not
qualify cold boot**. Its opcode histogram can count the following address
while SYNC changes, so exact per-PC call counts in that old run are not
reliable evidence. The probe now samples once at MCClk falling; its 4M-sample
regression correctly counts F3AC/F3AE/F3AF instructions rather than the
following operand F3AD.
An earlier probe with a one-time procedural force captured a static RHS
and is explicitly invalid. The generator's optional nominal BaseBoard
variant uses continuous comparator wiring and an external boot-button
input; default board/backplane regeneration remains byte-identical.

`dorado_storage_init.sv` clears the raw DDR window before CPU clocks start.
A complete one-module cold-start simulation verifies every raw word was
cleared exactly once (524,288 words), loads one HPS track, then executes the
actual BaseBoard power-on code. Its old 1B-sample target run was stopped
past 450M samples after reproducing the serial-data fault below; it had
not transmitted a network packet or written storage after initialization.
The FPGA wrapper holds a physical boot-button input during logical samples
200M..240M, following the five-board probe; it does not jam CP or CPU state.
The native UIO status extension exposes passive BaseBoard address/clock/
reset signals and DDR readiness. Focused UIO and Main transport gates pass;
temporary diagnostic Main SHA256:
`766873a17ff55e51a9b17cc35b16e933b002261a9daa9113b892d0609f777211`.
Stock `/media/fat/MiSTer` remains unchanged.

The Cedar-specific native build is fitting in
`misterubuntu:/home/alans/mister/Dorado_Native2_20261017`. It selects Pilot
10-word labels and includes passive ContA run/hold/task/CIA/TPC observation.
No processor state is written by these diagnostics. The packet bridge now
buffers 1024 words to carry Cedar's advertised 1478-byte Pup data payloads;
UIO flags bit 2 advertises this capability, preserving legacy 512-word core
compatibility. Full-size wire CRC, UIO backpressure/commit and Main process
transport gates pass, including on the actual MiSTer ARM.

Cedar's ARM-local STP tree and immutable versioned Main/server binaries are
staged under `/media/fat/games/DoradoRTL/System_20261017`. Launch the server
with cwd `System_20261017/dorado`, explicit BFN110 aemu/NetExec paths and
STP root `System_20261017/chm/cedar/stp-root`; then its existing relative
BFN113 mapping resolves to the matched CedarDorado.eb!6. The current service
is running there; native firmware has not requested a boot file yet.

**Never replace a running Main executable.** Its core-change restart reads
`/proc/<pid>/exe`; replacing the inode leaves a deleted pathname and the
failed exec falls back to rebooting the board. This caused one station-load
reboot and was corrected. Use immutable content-addressed paths. Current
native Main SHA256 is
`1fb2e098e6f0dba64b2ad8f13964e8a6ecef553bab72039ee24e53cb9619e8d2`;
static ARM server SHA256 is
`215276e0b3d780b4a6f6cbb0a4339672d714a152c0a70858950b1cb05dea25f2`.
Stock `/media/fat/MiSTer` is unchanged.


Cedar-specific server qualification now passes on Mac and MiSTer14 ARM:
BFN113 returns all 31,376 microcode payload bytes (62 frames) byte-exact;
an actual BSP/STP retrieve returns all 66,296 bytes of CedarSource.VersionMap
in 45 data packets, including 1,504-byte frames. The client advertises a
1478-byte Pup allocation and acknowledges each byte ID. This also validates
the staged STP root and versioned-file resolution. Run
`make -C fpga/mister/server cedar-test`; ARM evidence is in
`fpga/mister/reports/2026-10-17-cedar-arm/`.

**Two actual BaseBoard manifold defects found during native bring-up.**
`CPDMuxData'` is driven by BaseBd j17's two SN74LS01 open-collector NAND
outputs, with complementary clock inputs. The generator resolved these as
OR, making the net unconditionally high and serial DMuxData unconditionally
low. Consequently the old five-board probe's 41,856 DMux clock edges shifted
only zeros; its 475 MIR strobes were not proof of a successful bootstrap.
This net now resolves with AND, while the ECL buses retain OR.

After that correction, the BaseBoard's own DMD shift register still read
zero for address A35. j17 selects shift data while TDMuxClk is high and
readback when it falls; l21/l22 clock at that same falling edge. Their
SN74LS174 models sampled the new readback value. Only these two instances
now select pre-edge data capture, preserving the serial bit's setup value.
The cell's default remains unchanged for other instances.

`make -C verilog/verilator baseboard-manifold-test` exercises the actual
board j17/l24 selector's eight input combinations and shifts a byte-asymmetric
12-bit address through l21/l22. It fails on the old OR; with AND alone it
still fails with DMD=000; with both fixes it passes at A35. These tests
replace only the serial selector's inputs, not its outputs or stored state.
`Dorado_Native3_20261017` is compiling with both fixes and the Pilot medium.
A five-board ROM-driven probe completed 1.2B samples with the fixes: at 84.6M
samples the actual firmware shifts nonzero addresses through ContB's DMD
(e.g. 001, 002, 004, 009, 012 ... 910), where the old run remained zero.
It records 20,567 DMD transitions and enters the actual first 20-second
disk spin-up wait at DMD=4C4; the bound ends before Bootstrap loading, so
it is a serial-control qualification, not a cold-boot gate. The old zero
readback bypassed that branch. No simulated CPU state was modified;
one debugger call only flushed the simulator host's buffered stdio log.
The final clock-period print overflows a 32-bit intermediate at this bound;
the intact raw counts imply 80 samples per MCClk cycle. Future prints use
64-bit arithmetic. Raw log and qualification limits are preserved in
`reports/2026-10-17-cedar-arm/firmware-*`.
Native2's superseded fit was stopped after over twenty minutes in optional
physical retiming. Native3 restarts with retiming disabled; reproduce that
build setting with `DORADO_QUARTUS_RETIME=0 fpga/mister/build.sh ...`.
A second fit of the same mapped RTL runs in `Dorado_Native4_20261017`
with every optional physical-synthesis stage disabled.
Reproduce this setting with `DORADO_QUARTUS_PHYSICAL=0`.
Native5 tries the documented `quartus_fit --effort=fast` on that same
mapped netlist. Native4 and Native5 both finish routing (18m34s and
12m18s respectively) and remain in post-fit processing; final timing is
not yet available. The successful
Native1 baseline used **two** fitter processors, while the newer defaults
used one; Native7 copies the mapped field-correct Native6 design and sets
`NUM_PARALLEL_PROCESSORS=2` (reproduce with `DORADO_QUARTUS_JOBS=2`).
The obsolete Native3 physical-optimization run was stopped. Native4/5/6
remain available until a timing-clean candidate is selected.
Setup/hold validation remains mandatory before deployment. No startup
parity-state modification has been made; the serial faults explain why the
firmware could not configure the processor's manifold in the first place.

**Native field identification is now wired for the scaler (hardware pending).**
HM p.115 supports only 2:1 interlaced displays; DispY's VCW `OddField`
latch selects the half-line VSync phase. The native wrapper previously tied
MiSTer VGA_F1 to zero (only the Executive checkpoint supplied a field ID).
The optional generated DispY `_field` variant exports that actual latch,
which the native wrapper forwards to VGA_F1. It adds no state or display
logic. Default DispY regeneration is byte-identical; removing the variant's
module suffix, port and direct alias reproduces the canonical board exactly.
The complete four-module native machine lints cleanly. `display_field` in
the manifest enables `DORADO_DISPLAY_FIELD`, keeping old native exports
buildable. `Dorado_Native6_20261017` is compiling this change separately;
Native3/4/5 remain first cold-start candidates and have the older field tie.
No claim of a Cedar display on hardware is made yet.

**Native4 timing review.** The first corrected RBF fits at 35,234 ALMs,
44,170 registers and 178 RAM blocks. Its initial timing report has core
setup +1.857 ns and three data hold failures (-0.098, -0.094, -0.083 ns)
from `dorado_storage_group.write_data` to that same group's `saved_data`.
These are mutually exclusive transfers: a new DRAM write implies
`write_pending=0`; an advancing machine clock requires `ready`, hence a
cache hit; together those make `req_valid=0`, disabling `saved_data` on
the source-changing edge. While a write is pending, the delayed CAS/WE
controls are both low and cannot recapture `write_data`.
`native_timing.sdc` excludes only those three instances' own data hold
checks. Next-edge setup and request/control timing remain checked.
Simulation assertions guard the exclusion's conditions; group and complete
board equivalence pass with 254 and 7,872 writes respectively. The RBF
itself is unchanged. Initial timing and detailed paths are preserved in
`reports/2026-10-17-native4/`; repeated full timing passes every reported
check, with core setup +1.857 ns and hold +0.110 ns. The unchanged RBF
SHA256 is `ecca2fbcfe514800a71465ffe3276ac5afa214b7a4caac57166aa59f4ec8da94`.
It is now loaded using `DoradoRTL_Cedar3_20261017/Cedar3.mgl`, with the
separate Pilot pack and a real held C key for BFN113 selection. Hardware
has reached the firmware's spin-up delay; no network boot request yet.
Native5's different placement has clean core hold but an HDMI setup failure
of -0.030 ns, and has not been loaded.

**Cold-start frontier: the Bootstrap entry handoff.** Native4 releases
`Run'` after the disk waits, but its passive processor word then remains
`30fe0f81`: Run'=0, Hold=0, SetRun=1, SetSS'=1, task 0, CIA=7740B,
TPC=7601B. `BootDefs.mc` identifies 7740B as Bootstrap's entry. This is
not proof that Bootstrap executed; no packet has reached the ARM service.
The BaseBoard continues its ROM's normal analog/status polling. A real
three-push boot sequence produces intermediate jam states, then returns
to the same entry stall. The period `doradocontinuous.masm CheckBootButton`
distinguishes one push (Dorado handles it), two (InitMap), and three through
six (CoolBoot, reload microcode). MiSTer `reset_combo=0` drives the native
boot input using left Ctrl + left Alt + right Alt; no Shift is used.
Hardware excerpt: `reports/2026-10-17-native4/bootstrap-entry-stall.txt`.

Native6 and Native7 both fail HDMI setup (-0.093 and -0.354 ns), so neither
was loaded. Native8 repeats the field-correct mapped RTL with standard
fitter effort and two processors. Native9 adds passive observations and
uses the same fitting settings. Its BaseBoard debug word's high bits are
31 StopMIRClk, 30 Freeze, 29 CRamClock, 28 CLK.ca', 27 CLK.cb',
26 CLK.OutBase', 25 CLK.InBase, 24 Stop, 23 dStop, 22 RunClk'a,
21 FreezeAC, 20 FreezeBD. The existing low 20 bits and processor word are
unchanged. No processor state or clock is driven by these aliases.
Full native lint passes; default ContA regenerates byte-identically and
default backplane matches after normalizing its output-basename comment.
The firmware-only probe was extended toward 2.1B samples, then stopped
after its 650M progress report once the short parity test isolated the
same startup failure. That partial repeat is not a qualification result.

**The IM parity blocker is reproduced and corrected; FPGA test running.**
The MC10170 model retains a legacy B-output inversion for unreconciled
users. The three IM instances still selected it: ContA e18 and ContB j20
(read checkers), and ContB e01 (write-parity generator). Motorola's 1978
MECL data-book logic diagram gives the physical XOR function; the nominal
control-pin levels supply the inversion in its truth table. All three now
select `LEGACY_B_INVERT=0`. ProcH SignedCarry and other independent users
retain their existing settings.

With the old checkers, original Bootstrap at 7740B stops after one address
with parity enabled from startup, reproducing the FPGA symptom. Previously
`exec-parity` enabled the checks late and missed this. The new
`make -C verilog/verilator bootstrap-parity-test` passes three cases:
all 50 original Bootstrap instructions written through CP match every
field and both odd-parity bits; Bootstrap executes with checks enabled
from startup; a deliberately even-parity image is rejected. Restoring only
ContB e01's old inversion fails readback at IM[fc0], parity 1/0 instead of
0/1. The execution fixture is original microcode, not a C CPU snapshot;
native FPGA export still imports no CPU or memory saved state.

The harness preload now supplies odd parity by default; `+impareven` is a
negative-test fixture. `exec-parity` now also enables checks from startup.
Older datapath/operand probes deliberately repeat an instruction using the
Midas error freeze, so their fixtures now explicitly corrupt a parity bit.
The step test instead fills IM with a valid CPRegToLink instruction and
asserts no error occurs while running: disabling SetSS makes it fail, so
it tests stepping independently of the parity-error stop. SendViaMIR's
stored-bit counts include both complemented BLOCK and the correct odd
parity bit. The original Executive object/checkpoint remain preserved.
Evidence: `reports/2026-10-17-im-parity/`.

Native8/9's pre-fix fits were stopped. Native10, with the corrected IM
parity, actual display field and passive Stop/clock monitor, finished fitting:
34,984 ALMs, 44,188 registers, 178 RAM blocks and 35 DSPs. Its original
1080p analysis fails HDMI scaler setup by 0.188 ns (o_vpix_inner[1].r[6]
to o_poly_lum); core setup is +1.821 ns and hold +0.078 ns.

The same bitstream is qualified **only for fixed 720p60**, using MiSTer's
mode 0 and vsync_adjust=0. The post-fit analysis replaces the reconfigurable
HDMI output divider with six instead of three, preserving the actual PLL
VCO/source latency and analyzing 74.268 MHz, slightly above the actual
74.25 MHz output. All 29 reported checks are nonnegative, HDMI setup
+6.544 ns. No additional false paths or core timing changes are introduced;
the original failed report is preserved. Framework I/O remains incompletely
constrained. Raw reports, exact SDC and hashes:
`reports/2026-10-17-native10/`.

RBF SHA256 `ea95a9bc797e8c4b1c36e4550f1cf66742b8b9dd36e5e69f3eb676b6a2920476`
is loaded on MiSTer14 through `DoradoRTL_Cedar10_20261017/Cedar10.mgl`,
with a separate fresh Pilot pack (SHA256 `c0cd6784...`). Main PID 6859
uses the same immutable binary; its log is now
`games/DoradoRTL/Cedar10_20261017/main.log`. The server keeps PID 4406,
with log `System_20261017/server-cedar10.log`. Real C was held for Initial's BFN113 selection. Bootstrap now passes its
old entry stall. Native10 then stops reproducibly at CPU `30cd2c9e`:
CIA 6322B, TPC 6236B, Run'=0, Hold=0, task 0; Stop=1, dStop=0,
RunClk'a=1, while StopMIRClk and Freeze are both zero. Three physical
boot-button pushes make the firmware repeat this stop. No network packet
has been transmitted; Cedar has not booted.

**Decode the ROM payload, not standalone Initial symbols.** The ROM's
Boot0 at FE00 matches all 16 Bootstrap.mb hunks exactly. Its Boot1 at
C016 starts at IM C40 and contains 224 hunks; 204 differ from standalone
Initial.mb. ROM CD2 has the same opcode as the standalone BootEmulator
entry C92, whereas the standalone address CD2 belongs to a keyboard table.
The first focused tests used the wrong Initial version and cannot explain
this hardware stop. An independent extraction now produces the exact ROM
IM fields for focused diagnostics. Direct entry at F40 still requires the
original checksum protocol; zero-checksum/debug fixtures must be stated
explicitly and do not qualify the native firmware chain.

Native11 finishes fitting with HDMI setup +0.384 ns and core setup
+1.735 ns. Its two hold failures are msa2/group9 col[0] to saved_addr[0]
(-0.017 ns) and msa3Odd/group0 row[5] to saved_addr[12] (-0.002 ns).
These captures are disabled on source-changing machine edges: ready must
be high and algebraically excludes req_valid. Only those two instances'
own row/column-to-saved-address hold checks are excluded; setup and
request/enable checks remain active. A new simulation assertion enforces
that disabled capture. The group and complete-board equivalence gates pass,
including all 2,359,296 stored board bits and 7,872 writes. All 29 reviewed
timing checks are nonnegative, minimum +0.111 ns. Reports and proof:
`reports/2026-10-17-native11/`.

Native11 RBF SHA256 `6045b40988fc5439a05e735c6f59f0353e9fc4103c814454211f20725691a8a3`
is now loaded as the separate Cedar11.mgl, retaining fixed 720p60 for
comparison and another fresh Pilot pack. Main PID 8212 uses the immutable
binary; log is `games/DoradoRTL/Cedar11_20261017/main.log`. The server
keeps its `server-cedar10.log` file. Its cold boot is under observation;
no independent boot success is claimed.
Native12 is compiling the same functional RTL with ten additional passive
ContA control outputs in the unused status-word bits. It exposes rStop,
Error', StopAtT1, Jam, NoDispatch, dRun, SetRunRfsh, ClearMemStop,
SCorFreezea and preStartCyclea, in that order from status bit 15 to 6.
The existing six status bits and 32-bit CPU/BB words are unchanged.
The new export lints; its manifest enables the optional top-level binding,
so older native exports retain their interface. No storage performance
experiment is included in any of these candidates.

**The native Pilot medium requires a different format.**
`PilotDiskDefs.mc!1` and `PilotDisk.mc!1` specify drive 0's emulated geometry:
real cylinder = virtual cylinder MOD 815, real head = virtual cylinder / 815,
and real sector = (virtual sector + 8*(REAL cylinder MOD 4)) MOD 28. Drive
0 still supplies 117 physical pulses and has 29 complete slots; Pilot uses
28. The header retains the original virtual DiskAddress. This differs from
the simplified PDI path in the C reference.

`tools/pdi_to_trident.py` preserves all ten label words and 256 data words,
adds those address headers, and emits a raw/padded serial medium. Independent
inverse traversal verified all 65,535 work-volume pages byte-exact, every
header, all 4,075 spare slots and the 814-to-815 virtual-cylinder boundary.
`pdi_boot_links_to_chs.py` converts the disposable work copy's root links to
(3,20) and (4,27); no original artifact is changed. Its native pack SHA256 is
`c0cd6784ea9df43d669dab2c445b3a1a7241048ac87f10afa747927d931bad5c`.
The work volume's pages 104..138 already contain the exact matched
`Dorado.germ-6.1.6` (SHA256
`dbfa203f485dfbb53e8545ac995495758c11672aeeece81c2b3e5cbbc168a631`).
The HPS medium's default Alto 2/8/256 format retains its 11,024-bit serial
comparison/writeback gate. The optional native `--pilot-disk` selects
2/10/256. `make -C verilog/verilator pilot-pack-test` reads the germ's
physical (3,0,16) sector through the actual HPS track buffer and serial
medium: header (3,20), all ten label words and 256 data words match the
image, and all three Fire-code wire residues are zero. The same test now
changes the two extra label words and both ends of the data block, supplies
fresh independently calculated Fire codes on the differential write input,
and verifies the resulting HPS writeback against the whole expected image.
It catches an ignored write and checks all neighboring bytes. Fixture
changes stay in the test's in-memory disk copy. The real controller/germ
boot is still unqualified.

## Invaders starts on MiSTer14 (2026-10-17 UTC)

The canonical file-backed RTL simulation reproduces the FPGA's stall at
`>Invaders.RUN` (204 ink pixels), so the failure is not specific to the HPS
disk implementation. At 510M fabric samples, the Ethernet completion cell
at Alto address `0600B` contains `0776B`, rather than clean `0777B`.
`EtherDefs.mc` identifies the differing bit as `TxFifoPE`; the output task's
saved status is `0101B` (TxOn plus the parity error). The packet at the
output pointer is a raw Ethernet `FileSystem` name lookup. The C reference
also sends these lookups, receives no response, and nevertheless starts the
game; a separate file server is not required for this startup sequence.

Ether07's MC10170 packages k13/k17 check the FIFO's odd parity bytes. They
still had the legacy extra B-output inversion, causing every valid word to
report an error. The generator now selects the physical XOR truth table for
these two instances. `ethernet-fifo-parity-test` fails on the previous RTL
and passes after regeneration: all byte values are accepted, corrupt parity
is rejected, and the error latch only sets when a word is consumed.
`disk-fifo-parity-test` and the exhaustive `parity170-test` also pass.

Both the saved stalled run and a fresh continuation from the Executive
prompt now record clean transmitter status (`0100B`, TxOn only) and update
EOTime. The guest advances to its normal network-discovery timeout loop.
**Invaders reaches its first game screen at 190M fabric samples** from the
Executive checkpoint (132,932 ink pixels): alien formation, shields, tank,
title and high-score text. [RTL screenshot](images/rtl-invaders-first-screen-2026-10-17.png).
**The new FPGA core also starts Invaders.** It passes `foo` + Return at
Executive, then runs Invaders from the pack; a separate clean restart repeats
the game boot. Shields change and the tank count decreases while it runs.
[Hardware screenshot](images/rtl-mister14-invaders-2026-10-17.png).
[New-core Executive input](images/rtl-mister14-executive-parity-2026-10-17.png).
Gameplay controls are not yet qualified; mouse input remains disconnected.

The narrow playfield and lower-screen noise also occur on the FPGA and in
the C reference. Invaders requests low resolution (`DCB control=0xC112`),
while Xerox's `AEmuSources/DisplayMain.mc`, GetScan, explicitly says
"Ignore Alto resolution bit -- always use full resolution." This points
to a period-microcode compatibility limitation, not the transmit-parity
stall. No guest bitmap, completion flag or CPU state was patched to boot it.

Run `/media/fat/Scripts/DoradoInvadersParity.sh`, wait for the Executive
prompt, then type `invaders` + Return. The launcher returns before Main has
finished reloading/mounting; typing immediately can lose leading characters.
It uses a separate core and disposable working disk; the prior Executive
launcher remains available. Installed RBF:
`/media/fat/_Computer/DoradoRTL_Invaders_20261017/Dorado.rbf`, SHA256
`a851720c3754911fa886fba4f030b32e01a5963281dc7db9b8d7ba1517884f1d`.
The seed remains `c703e9df999a074d3c14abf941e706e9fa35a3520594075065a104bc48535332`.

The matching original Verilated model is preserved. The corrected checkpoint
simulation uses `fpga_invaders_parity` / `obj_invaders_parity`; it ran through
300M samples without media faults and saved `invaders-parity-last.rtl`.
The isolated Quartus project is
`misterubuntu:/home/alans/mister/Dorado_Invaders_20261017/core`.
Final fit: 23,713 ALMs, 21,960 registers, 445 RAM blocks, 35 DSPs. Core clock
33.333 MHz, Fmax 45.45 MHz; setup slack +7.996 ns, hold +0.095 ns. The first
fit's six -0.041 ns hold paths are MemC d06 / MemX e20 F10145A address latches
to their own RAM write-address ports. Capture and write enables are mutually
exclusive, just as in the already audited ContA a04. The added exceptions
are scoped to those instances and hold only; setup/read checks remain.
Original failing and final reports are in
`fpga/mister/reports/2026-10-17-invaders/`. Framework I/O remains incompletely
constrained. Neither the microinstruction sampling ratio nor speed changed.

## MiSTer14: interactive Executive (2026-10-16)

**The actual FPGA accepts `foo` + Return, prints `There is no subsystem
named foo.`, and returns to `>`.** MiSTer14 (`10.0.2.14`) is allocated for
this test. Keyboard events came through its existing remote virtual USB
keyboard, not a planted guest input queue. HPS serviced the real disk lookup
(the mounted file advanced to the cylinder-104 track window).

The hardware starts from the ready checkpoint of the verified assisted RTL
boot. Processor, memory, display controller, terminal and disk logic then
continue running on the FPGA. This is **a checkpoint start, not BaseBoard
cold boot**. The simulation evidence below remains the fresh-boot gate.

**Final hardware screenshot:**
[`rtl-mister14-executive-input-2026-10-16.png`](images/rtl-mister14-executive-input-2026-10-16.png).
The field-corrected RBF passes two clean restart/input tests. All 2,330
pixels of the herald/command-response text match the RTL bitmap reference
exactly. The final remote capture is
`/media/fat/screenshots/Dorado/20261017_001012-final-repeat-input.png`
(the filename uses UTC; the local bring-up date is October 16).

Two video integration issues were fixed. At 1024x768 the MiSTer scaler
keeps only one interlaced field; the per-core 1080p override retains both.
The initial `VGA_F1` value must be one for this saved continuation; zero
swapped adjacent rows. The uncorrected capture had 2,472 differing text
pixels; the corrected capture has zero. Native captures are 656x840,
including controller borders around the guest's 608x808 bitmap.

Final RBF: `build/rtl-executive/DoradoExecutive_20261016.rbf`, SHA256
`3b86d7136d576c7c7a2b9c0b83082e2ed1646aa30b4e95d69ebdc5632dcc32cf`,
verified identical on MiSTer14. The prior field-swapped RBF is retained as
`DoradoExecutive_20261016-field-swapped.rbf` in the same local directory.

### Run on the allocated MiSTer

Run `/media/fat/Scripts/DoradoExecutive.sh` from Scripts or SSH. It atomically
creates `executive-work.pack` from the unmounted seed, then loads
`/media/fat/_Computer/DoradoRTL_20261016/Executive.mgl` and its paired RBF.
Reload only with the matching fresh disk; the launcher handles that. OSD
reset/pause does not restore saved memory.

The disk directory is `/media/fat/games/DoradoRTL/20261016/`. Seed SHA256:
`c703e9df999a074d3c14abf941e706e9fa35a3520594075065a104bc48535332`.
The original INI is `/media/fat/MiSTer.ini.before-dorado-20261016`.
The appended `[DoradoExecutive_20261016]` section sets `video_mode=8` and
`vsync_adjust=0`; other cores keep their settings. The remote service is
`http://10.0.2.14:8182/`. Native captures use
`echo "screenshot name" > /dev/MiSTer_cmd` and appear in
`/media/fat/screenshots/Dorado/`.

### Build and validation

Build machine: `misterubuntu`, project
`/home/alans/mister/Dorado_RTL_20261016/executive-hps-core`, logs in sibling
`logs/`. Quartus 17.0.2 is under
`/home/alans/intelFPGA_lite/17.0/quartus/bin/`. The final HPS fit succeeds:
23,733 ALMs (57%), 21,936 registers, 445 RAM blocks (80%), 2,336,070 memory
bits (41%), 35 DSPs. Core Fmax is 48.06 MHz against a 33.333 MHz clock;
reported setup/hold slack is positive after the scoped hold-only exception
in `fpga/mister/checkpoint_timing.sdc`. The original fit reported -0.065 ns
hold from ContA a04 `a_lat[1]` to its RAM write-address port. In
`cell_F10145A`, address capture requires `!p13`, while a write requires
`p13 && !we_d && ce_lat`; that source cannot change on an enabled write edge.
The exception covers only this instance's address-to-write-port hold paths;
setup and all read paths remain checked. Remaining core hold slack is
+0.086 ns, setup +9.192 ns. Both original and final reports are in
`fpga/mister/reports/2026-10-16-executive/`. Framework I/O is not fully
constrained. Build logs are `executive-field-*.log`; final timing rerun is
`executive-final-sta.log`.

`make -C verilog/verilator executive-mister-export` exports the existing
matching model/checkpoint without rebuilding it. `tools/sil_fpga_checkpoint.py`
derives native offsets and current RTL XML, preserves actual per-instance
register/RAM values, and applies the saved disk overlay to `executive.pack`.
It uses no C-emulator state. The 64K-word Alto configuration rejects other
physical addresses; clocked state matches the original at checked samples
3 through 1,000. All 65,536 initial visible words match exactly.

The full HPS-backed continuation simulation accepts the same command and
its final bitmap is byte-identical to `fresh-input-bitmap-440000000.pgm`.
This 50M-sample run makes three HPS track reads without out-of-range storage
accesses. Its log is `build/rtl-executive/fpga-hps-input.log`.

`dorado_pack_hps.sv` implements track-window HPS reads and checked serial
writes. Its 16KB buffer includes neighboring bytes at unaligned boundaries;
writeback precedes a seek, and the machine pauses during host transfers.
`make -C verilog/verilator hps-pack-test` passes: 11,024 serial bits match
the file-backed reference, two reads and one write occur, and the full image
is unchanged after loopback.

FPGA-specific fixes: support `mycore.*` templates and embedded `emu` ports;
MLAB hints for asynchronous SRAM; an external first-cycle valid mux when a
saved RAM output latch of one would prevent Quartus from inferring RAM;
and a true dual-port track buffer with `no_rw_check` because neither consumer
uses read values while its port writes.

**Historical pre-fix result (superseded by the Invaders section above):**
An additional hardware test entered `invaders` at Executive. The OS found
and printed `Invaders.RUN`, cleared the screen, and accessed another disk
track (file position 10,376,192). No game screen appeared in the captures
separated by about 25 seconds. This was the transmit-parity stall fixed above.
Evidence: `build/rtl-executive/mister14-invaders.png` and remote captures
`20261016_235643-invaders.png` / `20261016_235709-invaders-later.png`.
The clean Executive was restored with the installed launcher afterward.

Next boot work must include a local ARM boot server and buffered FPGA/HPS
Ethernet transport before removing the assisted world load. Initial needs
that transport. Reuse the C emulator's server logic in a separate Linux
service on the MiSTer ARM, without its CPU emulator; Main should provide
packet transport/configuration where needed. External network access should
not be required. Gate unassisted boot with the network cable disconnected.
The completion sequence and RTL/Main split are in `fpga/mister/README.md`.

Other remaining work: BaseBoard integration, larger storage/SDRAM, mouse
input, and durable handling of the last dirty disk track without a subsequent
seek. The present launcher uses a disposable working disk. The core runs at
one quarter of Dorado speed, with additional pauses for HPS transfers.

The sections below preserve earlier simulation bring-up notes. Their
"currently running" process IDs and earlier pending-status statements are
historical; the hardware status above supersedes them.

## Verified: interactive Alto Executive in RTL (2026-10-16)

**Fresh boot, character echo, command execution and return to the prompt all
work in Verilator.** The OS printed its Executive/12 and OS20/16 herald,
echoed `foo`, accepted Return, printed `There is no subsystem named foo.`,
and displayed another `>` prompt. Screenshot:
[`rtl-alto-executive-input-2026-10-16.png`](images/rtl-alto-executive-input-2026-10-16.png).
The input travelled through PS/2 events, `dorado_keyboard`, the terminal
serial return channel, DispY, the period microcode, and the OS keyboard ISR.
No guest text, input queue, interrupt mask or completion flag was planted.

Run from the repository root:

```sh
make -C verilog/verilator run-executive-ready
```

The local ready state is `build/rtl-executive/alto-executive.rtl`, captured
at390M fabric samples from the corrected fresh boot, before any typing.
To recreate it, run `make -C verilog/verilator executive-checkpoint` (slow).
The default model uses four fabric samples per clock. Scripted and physical
key transitions are spaced4M samples apart so the OS can scan each state;
typing is correspondingly slow in Verilator. The shortcut was rebuilt and
tested, restoring a byte-identical bitmap.

The complete command proof is `alto-executive-input-proof.rtl` in the same
build directory. Its restored bitmap matches the440M capture byte for byte;
all keys are released and keyboard interrupts remain enabled. Hashes and
model fingerprint are recorded in `alto-executive.json`. Checkpoints contain
actual RTL state and serial disk-write overlays and require the exact model
build. The unmodified base pack remains required.

The SDL window is left running from the clean ready state, session54876,
log `build/rtl-executive/interactive.log`, with periodic saves to
`interactive-last.rtl` (PID17676). The scripted test window and obsolete
diagnostic processes were terminated after preserving their artifacts.
**Important limit:** the boot uses the existing
assisted Initial/AEmu handoff and the screen is a read-only rendering of the
actual guest DCB bitmap. Video pins and physical MiSTer boot are not qualified;
MiSTer still needs the storage, video-clock and reset work listed in its README.

The older diagnostic prefix's missing input was an artifact of continuing
execution that predates the address fix: it left `M[421B]=E000`. The fully
corrected fresh boot naturally produces `1100`, enters the keyboard ISR,
and accepts input. Do not patch that mask or use the older prefix as a gate.

## Fresh RTL boot reaches the Executive prompt (2026-10-16)

**Fresh boot is now proven through the Executive herald and `>` prompt.**
The current model boots from zero, through assisted Initial/AEmu and the
serial Trident medium. At354M fabric samples it has printed the herald;
by362M it shows `>// eventBooted` and `>`. Screenshot:
[`rtl-alto-executive-fresh-2026-10-16.png`](images/rtl-alto-executive-fresh-2026-10-16.png).
This is the guest's actual DCB bitmap, rendered read-only from RTL memory;
it is not video-pin validation or a physical MiSTer result.

`build/rtl-executive/cold-prompt-latest.rtl` is an exact-model fresh-boot
checkpoint at390M. **Use the fresh path for input tests.** The older
`wordaddr165679.rtl` diagnostic prefix boots to text too, but leaves
`M[421B]=E000` and never enters the keyboard ISR during the input test.
Fresh boot has `M[421B]=1100`, `M[453B]=1101`, and the saved keyboard ISR
registers show that it executes. No interrupt mask was patched. The older
prefix carried execution from before the odd-address fix and is not a
fresh-boot qualification.

An SDL input test is currently running from the fresh390M checkpoint:
`fresh-input.log`, session12850. It queues `foo` + Return at400M with4M
samples per transition, captures `fresh-input-bitmap-SAMPLE.pgm`, and
saves actual state every2M to `fresh-input-latest.rtl`. **OS character echo
is still being tested; do not claim it yet.** The earlier input tests
against the old prefix are paused, as is fresh headless PID901.

Host-only improvements: `--save-every N` atomically replaces a checkpoint
periodically and saves at exit; it cannot be combined with `--save-at`.
New captures use the real Alto polarity (one=black), with an `ink=` count.
The actual SDL window requires access to the Mac display server, which the
filesystem sandbox blocks; the current window was launched with escalation.

## Current Executive attempt: display installed, Executive.Run opening

The corrected continuation at **330,000,000 samples** has installed the
actual DCB chain at `0420B -> BFFC` (hex addresses). It is still blank, but
the saved call stack has advanced beyond `LookupEntries`: `8D30` is the
return from `FinishInitOs`, and `CAD4` returns from opening the literal
`Executive.Run.`. That file has 105 disk pages on this pack. There is **no
Executive text or OS character echo yet**.

Current continuation PID95274 / session34150 reads
`wordaddr-capture240.rtl`, saves `wordaddr-capture330.rtl`, and targets500M.
Its log is `build/rtl-executive/wordaddr-capture-next.log`; guest bitmap
captures use `wordaddr-bitmap-SAMPLE.pgm`. PID92710 is paused and superseded.
The actual330M checkpoint can be inspected without advancing it:

```sh
verilog/verilator/obj_executive/dorado-executive --headless \
  --restore build/rtl-executive/wordaddr-capture330.rtl --stop-at 330000000 \
  --out build/rtl-executive/inspect330.pgm
```

Fresh corrected boot PID88854 / session46687 is still active, saving at240M
to `wordaddr-cold240.rtl`. It follows a different path after the first
BootInit delay from the older diagnostic prefix: at160M it has read78,108
words, versus134,420 in that prefix. Do not claim that the continuation's
timing qualifies fresh boot. The old16-sample comparison PID80891 is paused.

The host renderer now follows the Alto convention, bit1=black and
bit0=white; new captures report `ink=`, while the still-running binaries
report the former inverted `white=` count. This host-only correction does
not change RTL state or checkpoint compatibility. The observer remains a
read-only DCB renderer, not a capture of video output pins.

## Live run/capture correction

Current continuation is PID92710, session30266, `wordaddr-capture.log`, from
actual166.5M state. It writes **guest DCB bitmap** P5 files with
`--capture-prefix build/rtl-executive/wordaddr-bitmap --capture-every 2000000`.
These are read-only renders of RTL memory, not pixel-pin captures. The
canonical frontend does not include the ignored raw `liveframes` probe, so
those older +args do not produce periodic frames there. The new host options
make capture explicit and print `BITMAP @sample white=... DCB=...`.
PID88847 (`wordaddr-boot.log`) was paused at~186M in favor of this observer.
The corrected **fresh** run PID88854, session46687, is still active from0.
Both corrected runs target340M and save actual exact-build state at240M.

The corrected short continuation at166.5M preserves new queueB552 and
previous completionFC90=0F00. Subsequent disk reads resume (141,848 words
at168M, 142,392 by170M). At170M onward the guest re-enters BootInit's30K
iteration display-off loop; follow it rather than interpreting the lack of
pixels during that loop as a stopped computer. No Executive/input result
yet. All new checks are now complete: unit regression, readback/memrun,
16-distinct-word cache fill at4/16, and cell/key-matrix/loop structural checks.

## 2026-10-16: odd-bank address setup fix preserves the actual backward jump

A concrete address-capture bug is fixed in the generator: MemD F10016
positions **b09/h07/k09** now select PRECAP=1. On the actual held IFU fetch
at165,679,831, the even bank already loads word-pair2 for FD25; the odd bank
samples the following dDad=0 as MemC releases the hold and advances its mux.
It therefore selects **FD21=FFF4** instead of **FD25=01FD**. The wrong opcode
is parity-correct and falls through the disk completion wait. In this window
cache RAM itself still contains the correct sixteen words.

`tb_cache_word_address_capture.sv` tests all three copies with changing
address at the capture edge and then counts a full line. Old model fails
0/0/0 expected2; new passes (compiled4 and16). Readback and memrun pass4/16.
The actual165,679,300 checkpoint maps by field identity into the corrected
canonical model, with new setup registers allowed to settle hundreds of
fabric samples before the failing handoff. All65,536 visible words match
before execution. New `wordaddr-ifupoll.log` shows D=007F4 (01FD plus parity),
FG opcode01, and CIA188 executing the correct backward JMP after NSK1C.
At166.5M the queue remains **B552** until the new command completes, instead
of being overwritten withzero; previousFC90 status is0F00. No queue or
completion state was planted. Still no Executive or character echo yet.

Current source: g22 PRECAP=0 (stale FastD pulse rejected) plus these three
odd-bank setup counters. Current exact-build state is
`wordaddr165679.rtl`; `wordaddr1665.rtl` also saved. Long continuation
`wordaddr-boot.log` (session79608) and fresh boot `wordaddr-cold.log`
(session46687) both target340M and save at240M. They have live raw video
frames every2M; final frontend bitmap is read-only from actual guest DCBs.
The old16-sample run `post16-cold.log` predates this address fix and is only
a comparison, not a current-source gate. New readback/memrun4/16 pass;
structural and actual16-distinct-word cache fill checks are being completed.

## 2026-10-16 refinement: queue store succeeds; completion poll exits early

The short replay disproves a lost queue-post store. M[0151] changes from
FC8F toB552 at165,697,978 with correct cache write enables. At165,733,338
task12 overwrites it withzero while completing the precedingFC8F command.
Actual FC90 status is stillzero at165,697,000, so the guest has advanced
before the previous command completed. This is a premature completion wait
exit, not an actual missing posting write. `queuewatch.log` records both
writes; `tasktrace.log` resolves their microinstructions.

The last software wait iterations read FC90=0, then at165,679,882 dispatch
`MOV#0,0,SNR` (FD24) with AC0 stillzero. The next dispatch165,679,962 enters
the AND atFD27 instead of looping throughFD25. `tb_polltrace` replays from
post140.bin, saves post165679.bin at165,679,300, and traces the failed skip
window through165,680,100. Need distinguish an IFU reschedule/fetch error
from an arithmetic/condition error; no fix is established yet.

The disassembler's left column is a logical .mb slot; the FOURTH word is
its real IM address (mask to12 bits). Example: STAX's logical0137 maps real
0053 (hex02B). CIA4F5 is ACmmdEnd's tail, not AWasntIdle. Map symbols before
interpreting an actual instruction trace.

## 2026-10-16: corrected fresh-prefix boot reaches a later queue-post stall

With g22 PRECAP=0, replay of the **fresh**104M state reaches the normal
BootInit delay at8C4B. Disk NotSelected/ReadError during that idle interval
is not a demonstrated blocking error: microcode deliberately deselects
between commands, and the next transfers run with error flags clear.
The canonical frontend's migrated104M state matches independent decoding
across all65,536 words and all24 checked shared CPU/disk progress records.

The corrected path then completes255 actual serial writes (C34/H4/S11
throughC44/H4/S11); no Sys.Errors trap is observed. It stalls later at
A3D0 polling command block **B552**, whose status remainszero. Actual live
RTL memory has M[0151]=0000 (empty disk queue), M[B552+1]=0000, command4840.
At165,697,650 the guest dispatches `STA0 @A416` (A416 contains0151), with
AC0=B552. At165,697,970 the store's final microinstruction has T=0151,
A=0151, B=B552. The pointer nevertheless never reaches the queue.
`tb_laststore` replays from actual post140.bin to165,699,000, saves at
165,697,000 and observes cache write enables/data and all four candidate
queue words across the failing instruction. The failing cell is not yet
identified; do not patch the queue or seed completion.

Useful logs/checkpoints: `post-boot.log`, `post-altoops.txt`,
`postlive.{bin,json}` (read-only debugger capture), `post140.bin` + `.media`,
`laststore.log`, `post165697.bin` + `.media`. The native PID69062 and
keyboard-enabled frontend PID71396 are SIGSTOP-paused at their stalled
states. Older PULSE_CAPTURE runs are also paused and superseded.

The finer-clock simulation is separately built into
`verilog/verilator/obj_executive16/dorado-executive`, preserving the4-sample
binary and fingerprints. Fresh run `post16-cold.log` uses16 samples per
clock half, target1.2B samples, save at700M. It is a diagnostic of clock
quantization, not yet a boot gate. Corrected stale-pulse regression passes
at both4 and16. Frontend now has `--help`, rejects unknown flags, and spaces
keys by2M samples by default to allow OS keyboard polling.

## 2026-10-16 correction: early FastD was a stale hit, not a store to preserve

The PULSE_CAPTURE interpretation below was wrong. Re-decoding the existing
77.5M snapshots shows **both** PRECAP=0 and PRECAP=1 successfully write all
132 intended disk-store words, including48FF/48EF/etc. PRECAP=1 additionally
corrupts unrelated resident18FF/18EF/etc with those new words. Comparing
against baseline alone mistakenly treated removal of this corruption as a
regression. Current generator is restored to **g22 PRECAP=0**; the pulse-OR
adapter is removed. The regression now rejects a stale old-hit pulse that
clears when the new cache address misses.

Actual cache-cell observations pin it down: at102,367,802, the F20F miss
writes48A8 into way0's still-resident **D20F**, previously0106. At102,368,002
the held-store retry correctly writes48A8 into newly allocated way2/F20F.
This is an extra wrong-column write, not a lost intended write. At104M,
PRECAP=0 differs from the C resident C000..F9FF image at only9 words,
versus253 for the baseline (241 at line endings). Remaining differences
include live mutable data; do not assume they are all bugs.

Fresh PRECAP=0 still encounters a separate disk error around105.8M;
`tb_post_optrace` replays from its **fresh104M** state to investigate that
without retaining the stale-hit corruption. Logs and checkpoints are in
`build/rtl-executive/post-optrace.log` and `post-altoops.txt`.

## 2026-10-16: current boot trap and cache-line boundary evidence

Executive and character echo are **still unverified**. The keyboard hardware
path works, but the fresh boot takes `SysErr(..., 1203)` (ecNotKs, "not a
disk stream") at fabric sample 105,743,xxx, calling the resident
`Sys.Errors` trap at F6D3. The instruction trace is
`build/rtl-executive/pulse-altoops.txt`; it comes from actual 77,466,644 RTL
state replay with the current PULSE_CAPTURE and keyboard model. This is a
guest fault, not simply a boot that needs a longer budget.

The guest's own OutLd wrote all 255 pages to the volatile disk overlay.
`recover_outload.py` recovers that data from **the fresh run's actual serial
writes**, reversing the Alto transfer word order. `fresh-pre-swat.bin` has
221 differences at cache-line word 15 in D000..DFFF against the working C
reference; 202 equal the C word at address +2000 (hex). E200..F9FF matches
exactly. The same discrepancies already exist in the actual 104M cache
snapshot, so they are not solely artifacts of later OutLd. This points to
cache replacement/store timing; the exact failing operation is still being
traced. Passing the earlier store pulse checks is insufficient evidence of
correct boot memory. The new `tb_linewatch` observes actual cache cells
without modifying the guest and saves state at 100M for short replays.

The timing estimate below must not be used as a deadline: C and assisted
RTL have different startup overheads. C's first Alto instruction is at
CPU=20,919,473, versus RTL sample ~65.67M / 8. Account for that offset before
comparing milestones. Native C's Executive at BB145M / CPU39.4M does **not**
imply the assisted RTL should need 300M+ samples.

Readback and memrun pass sequentially at ratios 4 and 16. Fresh long run
PID51306 (budget extended to440M) and standalone frontend PID60877 were
paused with SIGSTOP after actual checkpoints at185M and320M respectively;
resume explicitly if useful. Late replay completed420M with no Executive.
The canonical frontend's checkpoint migration copies only actual RTL state
by generated field name; the first shared CPU progress event matches the
fresh run, and all65,536 visible words match independent decoding.

## 2026-10-16 morning: preserve short and late write requests; live keyboard path

Active goal: fresh RTL boot to a working Alto Executive, with characters typed
through the real terminal interface. Still no Executive prompt as of this entry.

The word-15 discrepancy is **lost short disk-task stores**, not yet evidence
of a bad DRAM refill. A 33,356-sample replay from the actual 77,466,644 state
finds the first case at 77,471,367: FastD is high for one fabric sample and
clears as the modeled SpecialClk2 rises. Setup capture retains it; post-only
capture loses it. The previously captured held store arrives on the other
side of that edge and needs the later sample. The regression now exercises
late, steady, and early-pulse requests. The previous PRECAP=0 candidate fails
the early-pulse check with only two of three requested writes.

The current generator scopes `PULSE_CAPTURE=1` to MemD03 g22. The SE10231
model explicitly retains an asserted write request in either sample of the
edge-detection aperture, without moving WE into a new address/data phase.
This is a named oversampling adapter for pulse requests; general D flops
retain their previous setup semantics. Both clock ratios 4 and 16 pass the
expanded regression. The actual 77M -> 77.5M visible-memory comparison is
**byte-identical across all 65,536 words**, where PRECAP=0 differed. The
145M held-store replay also reads FC8F correctly and advances the queue.

`tb_exec.sv` now uses the existing MiSTer PS/2 keyboard decoder ahead of
`dorado_terminal`. Host input changes `terminal_ps2_key`, not guest RAM.
End-to-end tests on the executing machine deliver `e` as guest keyboard
word0=EFFF, then restore FFFF on release. Files:
`build/rtl-executive/keyboard-{down,up}.{log,rtl}` and the memory dumps.
The SDL frontend is `verilog/verilator/dorado_exec.cpp`, build target
`make -C verilog/verilator executive-sim`. It renders the guest's actual
DCB bitmap from RTL DRAM/cache using `tools/sil_rtl_memory_view.py`; the C++
view matches the independent Python decoder across all 65,536 words. It is
not yet the raw video-pin renderer. Keys use the real PS/2 -> serializer ->
DispY -> microcode -> guest-memory path. `tools/sil_verilator_state.py`
provides exact-build snapshots, excluding host pointers/handles/schedulers.
Snapshots include actual serial-written media overlay and reject a different
model fingerprint. The `--import-raw` option is diagnostic only.

Current runs (nohup, Mac):

- `interactive4-cold.log`, session 54515: fresh assisted boot, 260M limit,
  saves native state at 185M. Live frames every 2M. It has passed 105.8M
  without the old read failure. This is the decisive fresh-prefix test.
- `interactive4-long.log`, session 46923: real 145M checkpoint, 420M limit,
  native state save at 300M. The first twenty CPU-progress records match the
  non-keyboard PULSE_CAPTURE build exactly.
- `writepulse4-long.log`, session 28382: real 145M checkpoint, 220M limit,
  native save at195M. Past the old 190M Swat trap screen with no traps and
  continuing disk transfers. No text by 218M.

The C reference was measured rather than guessed: Executive text appears
around BB=145M, CPU=39.4M microinstructions, and settles at 2092px by BB=180M.
Those are **different counters**. The corresponding RTL budget must allow
roughly 300M+ fabric samples, not treat 190M as a boot deadline. Oracle log
`c-milestones.log`; it uses a disposable pack copy.

Short replay tools remain ignored in `build/rtl-executive`. `prepare_replay.py`
generates an exact-layout driver for a supplied object directory. Dynamic
keyboard inputs inhibit some optimizations, so old/native snapshots from
other builds must be mapped by field name; never assume raw layouts match.
Readback/memrun gates at 4 and 16 are being rerun sequentially because their
Make targets share object directories. Structural checks pass.

## 2026-10-16: actual RTL Swat text; cache-store timing still experimental

**A real pixel-stream capture now contains Alto text**, at 190M samples:
[`rtl-alto-swat-trap-2026-10-16.png`](images/rtl-alto-swat-trap-2026-10-16.png).
It identifies **Swat.30 - March 29, 1980**, with `Trap instruction 77400`
and register output. This is **not an Executive prompt**. The video image
has incomplete-field artifacts. A separate
[readable guest-bitmap rendering](images/rtl-alto-swat-bitmap-2026-10-16.png)
is decoded read-only from actual RTL DRAM/cache at 190M, then rasterized
using the guest's Alto DCB chain. That supplemental image is a software
rendering of RTL memory, not a capture of the video pins and not the C
emulator's framebuffer. `render_rtl190.py` records the decoding procedure.

The raw frame comes from assisted Verilator Initial -> AEmu boot, continued
from actual RTL state at 145M. Both clock candidates reproduce it
**byte-for-byte**. No text or queue completion was planted. No Quartus or
physical MiSTer was used. The canonical-candidate raw pixels and provenance
are `build/rtl-executive/writecapture145-live-190000000.pgm` and
`os-text-capture.json`; source log `writecapture145-long.log`.

The captured queue-swap failure is a **lost cache write-enable pulse**.
At sample 146,495,496, FastD reaches MemD03 g22 on the same fabric sample
as its unbuffered SpecialClk2 edge. The setup-capture model sees the old
input, so neither WriteD0 nor WriteD1 pulses; the next ordinary store does
write. The sheet explicitly labels the clock **SpecialClk2 NOT clk2'**:
it is j18b's buffered, non-inverted preClk2 output.

The first candidate added one fabric tick to SpecialClk2. It fixes the
late queue store, advances disk transfers to 212,182 words, and produces
the text capture, **but a fresh boot also exposes an earlier disk-read
failure at 105.8M**. Matching the new clock's level at checkpoint restore
does not remove that failure. The global clock-lag candidate is rejected;
its board survives only in ignored `MemD-specialclock-lag1.v` and binaries.

The narrower **experimental** candidate now in the source keeps the write
pulse in its original address/data phase: MemD g22 selects `PRECAP=0`, capturing at edge
detection to represent j18b's sub-fabric buffer ordering. SE10231's default
remains setup capture for every other socket. This candidate:

- Passes `memory-write-clock-test` at SYSPER 4 and 16. The old board fails
  the same test with zero write-enable pulses on both banks.
- Passes the actual 16-distinct-word cache RAM refill gate at both ratios.
- Passes the 104M -> 107M replay with the earlier disk read error-free.
- Writes FC8F/FC7D in the 145M replay, reads FC8F back, and advances beyond
  the former queue stall. The actual cache-cell trace is
  `writecapture-store-analysis.txt` (all 4,300 samples available).
- Passes readback/memrun at both ratios and cell/key-matrix/loop structural
  checks. Longer boot checks are still running; consult their logs below.

**Long-prefix qualification remains open.** The 77M -> 110M test also
changes behavior at 105.8M with PRECAP=0; the shorter 104M -> 107M replay
passes because the changed memory was created before that checkpoint.
At 104M, the actual visible-memory comparison has 1,008 changed words:
**1,001 are word 15 of a cache line**. Logs/decoded dumps are
`writecapture104-memory-diff.txt`, `baseline104-visible-memory.bin`, and
`writecapture104-visible-memory.bin`. This strongly narrows the remaining
refill-boundary problem; passing the isolated fill gate does not validate
all refill endings. Do not report the boot or cache timing as fully fixed.

Runs, all launched with `nohup`:

- `writecapture145-long.log`, session 84362: actual 145M checkpoint through
  192M, completed; actual 190M state and matching Swat pixel capture saved.
- `writecapture77-validation.log`, session 30208: actual 77,466,644 checkpoint
  through 110M, completed; confirms the earlier full-prefix divergence.
- `writecapture-cold.log`, session 1705: fresh assisted boot through 230M,
  live frames every 2M; saves actual state at 104M.

All retain normal ECC, the legacy a21 compatibility setting, the fitted
16K cache, storage-input delay 2, and real serial disk writes into a volatile
overlay. The source pack is opened read-only. The older lag1 runs
`writeclock-resume.log` and `writeclock-cold.log` were stopped after the
failure was captured. There is still no demonstrated physical MiSTer boot.

Checkpoint validation is complete for the old failing window: all 4,300
STORELOSS records from the unmodified 145M replay match the original long
run byte-for-byte. The snapshot includes a `.media` sidecar carrying actual
serial-written words. The 77M baseline replay also matches all 1,776 recorded
CPU/disk/frame events through 100M. Drivers map POD fields by generated
name, retain fresh host pointers/schedulers/strings, and restore the media
overlay. These are ignored, exact-build diagnostic tools, not a portable
snapshot format. Verilator 5.044 rejects `--savable` with `--timing`.

## Earlier October 16 diagnosis: the final queue swap loses a store at a memory hold

The Alto-dispatch trace (`altoops4.txt`) and two read-only snapshots narrow
the permanent stall to the resident buffer swap atFCB1–FCB5. At
146,495,482 the CPU has the correct distinct values, AC0=FC7D,
AC1=FC8F, AC2=FC7B, and is executing the two stores that should leave
M[FC7B]=FC8F and M[FC7C]=FC7D. A repeated microinstruction and a disk-task
access toF000 intervene. The following LDA3 at146,495,634 readsFC7D
back fromFC7B, the old value. A later append consequently links the
buffer to itself and clears the status that the guest then waits on.
This is evidence of a lost/misrouted store around the hold; the cell or
strobe responsible is **not yet identified**. Do not fix the software queue
or force its completion word. The short excerpt is
`build/rtl-executive/final-swap-opcodes.log`.

`queueboot4` finished150M. Its BOOTIO window began at146.5M, just after
the lost store, so it only confirms the stable wait. `altoops4` finished
150M and does include the swap. Its initial400,000-record trace limit was
extended to8M via LLDB by changing **only the diagnostic monitor limit**;
no emulated-machine state was changed. This leaves a trace gap between
the original cap and the extension, before the final-swap window.

The next run is `storeloss4.log` (session36586, limit-storeloss4.txt,
149M limit). A new ignored `store_loss_probe.sv` observes the actual
cache RAM cells atFC7B/FC7C in all four columns, Dbuf, cache-write data,
write enables, chip selects, addresses and the memory hold. Existing
BOOTIO/MEMWIN/CA/PIPE probes cover146,494,000–146,498,300. Its purpose
is to distinguish failure to issue/write from a later overwrite. It uses
the same machine configuration and source pack as the reproductions.

**Run recovery:** the debugger/session interaction interrupted that process
at105M. Do not poll its old unified-exec session after attaching LLDB.
The cycle-counter watchpoint was also removed: an8-byte counter write
triggers even a2-byte watch on its upper half, making conditional watches
on that counter prohibitively slow. A read-only snapshot at77,466,644
samples was retained (`checkpoint77-syms.bin`), before any serial writes.
The resumed run is now `storeloss4-resume.log`, session69119, launched
with `nohup` and the ignored `tb_storeloss4_checkpoint` native driver.
Across the overlap through100M, **all1,776 recorded CPU-progress/disk/
live-frame events are byte-identical** to the original full run.

This diagnostic driver restores scalar/register/RAM ranges from the exact
build's symbol dump, leaving host pointers, strings and schedulers fresh.
`checkpoint-layout.json`/`checkpoint_ranges.h` describe those ranges.
The original77M snapshot has no media overlay to restore. The driver now
also saves the associative serial-write overlay in a`.media` sidecar, and
will save `checkpoint145-native.bin` at145M without any debugger attach.
**Validate the full failing-window trace before trusting a short replay.**
These are local, build-specific diagnostic artifacts, not a portable
snapshot format. Native Verilator `--savable` cannot be combined with
`--timing` in the installed5.044 (checked with a minimal standalone model).

The native C reference still boots with either or both cold-Alto shims
removed. Its full180M-BB-cycle compact trace is
`build/rtl-executive/c-ifudisp180.compact`. The first~7,700 Alto dispatches
match the RTL apart from BLT rescheduling; subsequent disk-poll trip counts
differ, and should not be treated as opcode failures. RTL J has already
advanced by the clk0 observation point: compare the captured TNIA with
the C IFUM vector, not J directly. Collapse held repeats when normalizing.

## 2026-10-15 late follow-up: inspect the running guest, then trace the lost request

`writeboot4` completed its 320M-sample limit. It delivered140,192 disk
words and committed251 actual256-word serial data writes (all Fire-code
remainders zero), from cylinder24 through34, head4. Transfers stop after
the last write at146.8M; subsequent frames still have only15–16 lit pixels.
**There is still no RTL OS-text screenshot.** No Quartus or MiSTer used.

A read-only LLDB attachment captured the running model at222,399,056
samples, at entry to `Vtb_exec::eval_step`, then detached and resumed it.
The complete inline symbol object is `build/rtl-executive/live-syms.bin`;
`all-layout.json` describes offsets from that exact build. The ignored
`decode_live_storage.py` and `decode_live_cache.py` recover the actual DRAM
and cache contents, without planting memory or changing guest state.
Use storage decoder **mode1**: normalized column bits are the complemented
RP bits8,1,0, then word-address bits7,6,5,4 (LSB first). The serial stage
is `7 - ((wordAddress & 15)/2)`. MemC columns1/3 use complemented tag-RAM
row addresses; columns0/2 use the direct row. Omitting that inversion
produces false duplicate tags. With both corrections, **all16,160 clean
cache words match DRAM exactly**. The remaining224 cache words are dirty.

The guest executes the resident Alto disk-completion poll atFD22.
AC2=FC7B; M[FC7B]=M[FC7C]=FC7D, M[FC7D]=FC7D, M[FC7E]=0.
The command atFC7F is4858 (check header/label, write data); the data buffer
isFB00 and its disk address is00FC. KBLK's current chain pointer at0151
is0, status0152=2F00, last address0153=00FC. Thus the controller has no
queued work while the guest is waiting on a zero completion word. The
healthy C reference keeps the two resident buffer pointers distinct
(FC7D/FC8F). The cause of their merger is **not yet established**.

Native-C diagnostic copies with the cold-Alto accumulator clear removed,
the I/O-page initialization removed, or both removed still render the
2092-pixel Executive. Those shims do not explain this RTL divergence.
All diagnostic C runs use disposable copies of the pack.

Next runs: `queueboot4.log` (session9947) records BOOTIO from146.5M to
150M, covering the final write/completion. `altoops4.log` (session49296,
limit-altoops4.txt) adds an observational IFUJump/STK trace to
`altoops4.txt`, for comparison with `c-ifudisp84.log`. Both retain normal
ECC, the legacy a21 compatibility setting, delay2 and the16K cache fitting;
neither forces data or error status. The new probe is ignored build output.

## 2026-10-15 follow-up: disk writes are required before the OS screen

The MemC16K fitting removed the 103.5M continuous hold. The full
`cachecfg4` run was stopped at156M, after73,236 delivered disk words;
frames still have only15–16 lit pixels, no OS text. From106M onward it
repeats checked-write control00a4 on the same sector, then tries recovery.
The C reference trace (`c-disk-reference.log`) proves boot performs real
writes: control0244B checks header/label then writes the data block.
The Verilator `dorado_pack` was read-only, with no outgoing data connection.

Work in progress: actual serial write capture into a volatile word overlay
in `dorado_pack.sv`; the source pack stays opened `rb`. The parser checks
SYNC201B, all payload bits and both Fire-code words before committing a
block, and `load_track` reapplies modified words after seeking. `tb_exec`
connects actual DskEth DataP/M output wires and suppresses the drive's read
data pair while write mode is active; the timing track remains running.
A new `+writeback` mode in `tb_disk_stream` writes through the controller
FIFO, seeks away and back, then reads through the controller. This mode
is **not yet passing**. It originally waited for WriteFifoTW with TIOA
still selecting DiskControl; the period microcode selects DiskData first,
and the corrected bench now delivers all outgoing words without underflow.

The test also exposed the still-legacy FIFO parity checkers e09/e11.
`disk-fifo-parity-test` fails on the old board's first valid00ff/11 word,
and passes all256 byte values plus deliberately corrupted parity after
both positions select the physical MC10170B function. DskEth is regenerated.
IOB f04/g04 corrections stay enabled; the other MC10170 placements remain
as before. The remaining isolated-test parity flag was traced to startup
(sample2722, before any test transfer), not an established transfer timing
fault. Clearing startup status through the documented Muffler operation
after selection/timing settle makes the round trip pass. The test now
fails immediately on any new controller error during the write; no error
flag is forced or masked. **`disk-writeback-test` passes at SYSPER4 and16:**
three captured blocks, every physical Fire-code remainder zero, changes
survive a seek, all266 words read back through the actual controller.
All four read-stream cases pass at4, as does the original pack framing gate.
The added checked-write command00a4 also passes at4: compare header/label,
then write only the data block, seek and read it back. Two fixture details
come straight from AltoDiabloDisk.mc: clear CompareErr before the ECC tail,
and output SYNC plus the first data word before blocking for WriteFifoTW.
Waiting for that wakeup first deadlocks on the old OutReg's read/check type.
The normal all-write case still passes. **All four read cases and both
write forms also pass at SYSPER16** (`disk-read-write16-final.log`).
The source pack checksum remains unchanged across the serial-write tests;
its only writes are to in-memory overlay state.

The drive's Write and HeadSelect signals are control-tag cable levels:
the format PROM changes RAM[6] head-select to RAM[5] write+head-select
without necessarily dropping ControlTag. `write_gate` follows those levels.
A strict outgoing CRC/truncation failure remains fatal in the pack model.
The `+badwritecrc` link-corruption test flips one outgoing check bit and
fails with a nonzero Fire-code remainder, as required.

New full `writeboot4.log` is running (session21925, limit-writeboot4.txt,
320M sample limit), with delay2, fitted16K MemC, normal ECC, legacy a21,
and the FIFO checker corrections. No data forces; the only media changes
are genuine captured serial writes in the volatile overlay. Live frames
are `writeboot4-live-<sample>.pgm` when the raster exceeds100 lit pixels.
The OS-text screenshot is still outstanding. No Quartus or MiSTer used.

**Full boot milestone:** by120M, `writeboot4.log` has87,117 delivered
words and many complete256-word serial writes with Fire-code remainder0.
The firmware cleared startup FIFO status naturally; FifoParity, IOBParity,
underflow, ReadDataErr and ReadError are all0 in the120M progress sample.
No flag or data force was added. This passes the old checked-write retry
loop. Frames still have only16 lit pixels, so OS text is not established.


Reference-work caution: the native C emulator automatically flushes guest
writes to its pack on exit. The C comparison runs in this turn therefore
updated the derived `chm/diskpacks/games-trident.pack` (the compressed
source is untouched). Preserve this state rather than blindly restoring
an unknown earlier working pack. Future C comparisons must use the ignored
`build/rtl-executive/reference-pack.pack` copy. The serial overlay itself
never writes its source file; its checksum is unchanged across the tests.


## 2026-10-15 follow-up: fit MemC for the installed 16K cache

The storage-input adapter (`DORADO_STORAGE_INPUT_DELAY=2`) passes the
sixteen-distinct-word actual-cache-RAM check at SYSPER4 and SYSPER16.
Full runs `stable4`, `stable-noec4`, and `adapter-noec4` all read about
70,073 disk words, then hit the identical continuous hold at sample
103,496,374 (task C, CIA6c8, VA00000ae, DbufBusy=1, RefHold'=0,
EcHasA'=1). The latter uses the synthesizable adapter without any data
force. Disabling correction through the hardware diagnostic bit does not
remove this hold. No OS text appeared.

A separate a21 MC10170 physical-polarity experiment passes all 256
syndrome classifications (`memory-ecc-classify-test`), while the legacy
board fails even syndrome zero. However, full boots with that correction
fault much earlier, into task F CIA86e at VAfe1f, T6003. Zeroing both RAM
data seeds does **not** remove that fault (`zero4.log`). The a21 default
change has therefore been backed out pending reconciliation of its
upstream parity/check streams. The classifier test remains a diagnostic
that exposes this known compatibility debt; it is not a passing boot gate.
The established c14 byte-parity correction remains enabled.

MemC23.sil's loading table (PDF page 25) also specifies configuration
straps absent from the imported wire list. MemD uses F10470 cache RAMs
(16K words), but MemC had both the 4K and 16K address contributions
connected and reported CacheConfig3. The generator now applies the
**16K with parity** column: a03 socket straps 3-6 and 4-7; a07 outputs
2/4 disconnected; d03 outputs 2/15 disconnected; l11 input12 open.
The a03 straps explicitly bypass its logic, since HiB is a live register
output rather than a constant. Existing parity cuts and PageConfig3
remain in place.

`cache-config-test` fails on the old board with tag00/row00/HiB0 yielding
11. The configured board passes all tag/row combinations at both HiB
values, independent ProcVA row contributions, CacheConfig2, and
PageConfig3. The row check isolates the ProcVA contributions from the
address ALU that shares their output bus. `memory-address-bus-test` and
`memrun-test` and `readback-test` still pass, as do structural checks.
The actual-cache-RAM refill gate also passes with the new wiring. Its
initial 52d3-versus-5201 failure was a checker defect: it ORed the selected
and inactive column's raw arrays. The corrected ignored gate records each
chip's physical row and chip select at that bank's first write, then checks
the selected storage column. All sixteen input values and stored words
pass. No data path was altered for that repair.

Full `cachecfg4.log` is running with the new
configuration, delay2, normal ECC, legacy a21, and the existing diagnostic
data seed. The independent `cachecfg-ecc4.log` variant still faults into
task F CIA86e, VAfe1f, T6003 by 42M; the cache configuration does not resolve
the upstream ECC mismatch. That variant is stopped through its run-limit
file. The selected-column actual-RAM refill gate passes at SYSPER4 and16.
This remains an assisted Initial-to-AEmu simulation; no Quartus
or MiSTer hardware has been used. The required OS-text screenshot is
still outstanding.

**Full-run progress:** `cachecfg4.log` passes the former 103,496,374
continuous hold. At 104M it has read 70,614 words (past the former 70,073),
returns to task0 Alto execution, and continues reading later sectors. Live
frames have 16 white pixels, still no OS text. The 16K fitting therefore
removes that reproducible hold; a complete Executive boot remains unproved.
A transient FIFO parity flag appears at108M and clears by109M; this needs
follow-up if the loader stops making progress.

## 2026-10-15 follow-up: MemC's 256-word-page modification was missing

`rowtrace4.log` shows RP=0004 and RP13=1 stable well before RAS asserts
at 65,158,655. Nevertheless MemAd=ff, and the array captures row 7f instead
of the row containing that page bit. This is not late map data.

`MemC01.sil` Rev Be (PDF page 3 of
`DoradoDocs/doradodrawings/MemC-Rev-Be.press!1.pdf`) gives the missing
configuration beside i24: **"For 256 wd pages, 20-23 <- Hi"**; the 1K-page
alternative ties only 20-21 high. These are i24's B inputs (pins 4, 6, 11,
13), not the VA nets themselves, which the map/pipe still need. Left on
VA20..23, the inverting mux drives ones onto the row-address bus and
overrides MemX's real-page bits. With the specified high inputs, it
contributes zero while MemX supplies the row address.

`tools/sil_to_verilog.py` now applies this fitted configuration through
`FITTED_INPUT_JUMPERS`, and MemC is regenerated. The loading diagram on
PDF page 25 (`MemC23.sil`, PageConfig3) specifies cutting i24 pins
4/6/11/13 and wiring them to pin 9 (SB). The generated board implements
those exact connections: the B inputs are high whenever selected. The
running `pages4` binary used constant-high B inputs, which produce the
same mux outputs for both binary select values; the exact SB wiring also
passes the address-bus test. The new
`memory-address-bus-test` fails on the old board (VA=0, MemC contribution=f)
and passes with the modification: all 16 virtual-address patterns release
the row bus, while all 16 column patterns still drive the expected address.
`memrun-test`, `readback-test` and structural checks pass.

Full assisted boot `build/rtl-executive/pages4.log` completed at 77,003,849
samples. The intended repair is visible: RP0004 now addresses 1fc7/2038,
separate from boot page 0 at 3fc7/0038. At 65,670,981 the boot words are
still intact in DRAM (6205/5c49 at stage D). However the IFU still gets the
wrong word: at 65,671,154 the branch uses 0003 rather than 6205. The array
is no longer being erased, but its refill/readout ordering needs tracing.
Live frames at 66M through 76M remain empty; no OS text is established.

`readpipe4.log` completed at 68,003,848 with the exact SB wiring. The
external bound monitor confirms both storage buses and SinD deliver the
original 16 boot words in order, including 6205 at Sin at 65,671,049 and
5c49 four samples later. The cache refill instead writes the third word
at its first position, and D1 duplicates the following even word. Thus
there are separate control/data alignment and D1 setup errors.

`build/rtl-executive/readback_pattern_probe.sv` seeds a distinct 16-bit
value into each of the sixteen storage positions, at every physical
address, and observes the existing `tb_readback` sequencer. It is an
ignored diagnostic, not a passing gate: the old bench's original seed
assertions deliberately fail with this different seed. At both SYSPER4
and SYSPER16 the first refill starts with 5302 rather than 5100, while
D1 gets 5504 rather than the adjacent 5403. Sin and SinD themselves carry
all sixteen values in order. A full SYSPER16 boot (`pages16.log`) was
stopped at 17M after this short diagnostic established the same error at
both ratios; it had not reached disk boot.

MemD's D1 input registers b10/c10/d12 now preserve setup data
(`PRECAP=1`). Their clk1 fanout rises on the fabric sample where CD
changes; sampling afterward loses every odd word. The new
`cache-input-capture-test` checks all eighteen bits across that transition:
it fails with the old board and passes with the fix. The distinct-word
probe now gets adjacent 5302/5403, 5504/5605, etc. The initial two-word
skip remains. No full boot with this D1 change has run yet.

Clock-depth diagnostics generated into ignored files did not fix the
skip: MSA depth skips four words instead of two, MemD depth misaligns
addresses, and MemX depth does not complete a refill. Canonical clock
depth settings remain unchanged. Current investigation compares the
EC1 transport control's first enable with CD/preDin/D0in timing. No
OS-text screenshot has been established yet.

An experimental bound probe (`sin_retime_probe.sv` / `sin_retime_exec.sv`
in `build/rtl-executive`) delays the incoming data and check-bit stream.
With D1 setup corrected, `pattern-retime4.log` shows all sixteen distinct
words in order at the cache input writes, 5100/5201 through 5f0e/600f.
This is a diagnostic force, not a canonical RTL fix; force scheduling also
changes the setup relationship at a12. `retime4.log` is a full assisted
boot testing that causal adjustment with live frames completed at
89,003,849 samples. It passes the former failure: BLT resumes correctly
at 65,671,154 and completes through VA fdff at 66,686,010. But the next
instruction 5c49 becomes dc49 in the refill (IFU G=37124), and execution
falls into JMPZ 0 at 66,686,146. No OS text appeared. This may be an ECC
phase error introduced or exposed by the timing experiment; it is not yet
an independently established ECC defect.

`stable4.log` and `stable-noec4.log` repeat the experiment with the driven
stream changing between fabric sampling edges. The latter also holds the
documented hardware DisableEc bit active (HM Error Correction diagnostics).
They are a causal comparison, not canonical boot gates. Both have live
frame capture. The normal-ECC run additionally traces the 5c49/dc49 path.

The generator now emits an **off-by-default** synthesizable alternative
for that timing experiment: MemD parameter `STORAGE_INPUT_DELAY`, with
compile-time default `DORADO_STORAGE_INPUT_DELAY=0`. Zero is direct Sil
wiring; 2 inserts two word-clock stages on all 16 Sin and two EcIn inputs,
before the board's existing input latches. This is an FPGA timing adapter,
not a claim that the Sil board has extra registers. The ignored diagnostic
`cache_fill_gate.sv` seeds sixteen distinct words and checks both their
cache-write sequence AND the actual cache RAM contents. With delay 2 it
passes at SYSPER4 and SYSPER16 without any data force. Cache chips use
different complemented address fanouts, so its RAM check records each
chip's own physical row base; using a03's address for every chip gives a
false failure (4000 versus 5100). Default memory/capture gates still pass.

The archived compiled `chm/io-doradologic/DoradoProms.run!3` contains the
same complete EC-1 table at byte offset 4632 as `MemProms.bcpl`; the
two-word offset is not explained by choosing an old source table.

## 2026-10-15 follow-up: storage address jumpers were missing

The completed `dramedge4.log` still loses the opcode at 65,671,154, so the
DRAM cell repairs below did not resolve the boot. Its storage snapshots do
show the boot words in real DRAM, then show those same cells overwritten
as the BLT advances. Decoding the snapshots back through the SN74166
parallel inputs finds 6205 and 5c49 at 65,158,669 and zeros later.

The storage board's `msa03.sil+7/+8` nets were undriven. They are the two
differential inputs that translate MemAd.2 into TtlA0/TtlA0', so two DRAM
address bits (row and column) disappeared. The schematic, `msa03.sil`,
explicitly specifies **e26 5-to-12 and 6-to-11 for 16K chips**; the
generator left both alternatives open because its generic jumper inference
correctly cannot choose a fitted configuration. The 16K DRAM model now has
these two jumpers explicitly configured in `tools/sil_to_verilog.py`.
No other ambiguous jumper fields were changed.

`msa-test` now writes zero at neighbors differing in this extra row/column
bit and verifies the original ones survive. ShiftSout loads all eight
word positions and checks all 128 data bits, instead of testing only one
bit of the first position. The old board fails the isolation check; removing
either configured jumper independently also fails. With both fitted the
test passes, along with readback and cell/key-matrix/loop checks.

Full assisted boot `build/rtl-executive/jumpers4.log` completed at 71,003,849
samples with the same zero-code failure at 65,671,154. The jumper repair
therefore did not explain the remaining boot failure. Live-frame checks
at 66M/68M see six white pixels and 70M sees zero; no OS text appears.

The next completed probe, `maptrace4.log` (68,003,848 samples), dumps all
map RP planes at 42M: **1,024 distinct real page numbers**, with RP0 present
15,361 times and RP1..1023 once each, as expected for the initialized
module plus vacant VM. The map is not uniformly mapping everything to zero.
The earlier `MAPCENSUS` WP/Dirty labels were reversed (d13 feeds MapWP,
d11 feeds MapDirty); the probe labels are now corrected in source.

At 65,158,605 the new read's RP is 0044 and DRAM group0 address is 3dc7.
At 65,158,669 the victim write's RP is 0004 but group0 address is 3fc7,
the same location later read for boot page 0. The actual write commits at
65,158,676, with zeros in the boot-word positions. RP bit 13 (page bit 2)
should distinguish the DRAM row but does not. This narrows the fault to
row-address/data arrival timing, upstream of the array's contents.
The prepared `rowtrace4` probe records every sample around that RAS/CAS
interval, including RP, MemAd, mux select and the Map RP13 chip's data path.
No OS-text screenshot has been established yet.

## 2026-10-15 follow-up: DRAM writes also hit the previous column

The new `make -C verilog/verilator dram-write-test` writes different bits
to consecutive columns and reads all of them back. It fails on the old
`cell_MK4096P_6` and independently on the old `cell_MosRam`: writing zero
to column 16 clears the one previously stored at column 15. Both cells
write on the fabric sample that detects CAS' falling, while the column
latch updates only after that sample. The array therefore writes the old
column once before writing the intended column.

The initial repair required CAS' to have been low on the preceding sample.
Its full assisted boot is `build/rtl-executive/dram4.log`. The expanded test
then exposed a second defect: these level writes follow data changes after
the write strobe. Mostek's [1979 data book](https://www.bitsavers.org/components/mostek/_dataBooks/1979_Mostek_Memory_Data_Book_and_Designers_Guide.pdf),
DATA INPUT/OUTPUT, says the later falling edge of CAS' and WRITE' strobes
the data-in register. Both cells now latch data on that event and commit
one fabric clock later, after the column latch settles. The test checks
both early and delayed writes, including data changes after the hold time.
It fails the intermediate level-write model and passes the final model.

That change also exposed reversed strobe polarities in `tb_msa`: backplane
MemRAS/CAS/WE are active high and the board translates them to active-low
TTL. The old fixture drove the opposite polarity, effectively writing
during its read cycles; level writes concealed this. The fixture now
drives the correct levels and retains all its round-trip assertions.
Both msa and readback gates pass with the final model.

The final-model full run is `build/rtl-executive/dramedge4.log`, with a
bounded, external `storage_probe.sv` monitor covering all sixteen DRAM
address groups on both storage boards. Neither full run has established
a screen result yet. The isolated `dirty-roundtrip.im` experiment is an
incomplete cold-start fixture (held at its first Store), not a writeback
regression gate and not evidence for a new RTL defect.

## 2026-10-15: boot sector loads; Alto starts; later code reload still fails

**No Executive screen yet.** The latest completed RTL run loads the first
sector without timeout or FIFO underflow, correctly starts the Alto boot
program, then loses its instruction stream on a later reload. The same
pack boots the C emulator's Executive at 2,092 pixels. The completed RTL
baselines still produce an effectively black raster. Their assisted
`c92 -> 23e` Initial-to-AEmu handoff replaces world transport; these are
not unassisted BaseBoard cold boots.

The new `+bootio` probe distinguishes CPU Input reads from FIFO holding-register
refills and records the actual KWait arithmetic. It resolves the September
question: **the deadline is correct and the RTC result is corrupted.** In the
16-sample reference (`build/rtl-executive/boot-trace2.log`):

- Active starts at 258081889, pulse 4. CPU reads the expected header
  `0000 0000`, then `af80 047e 0000 0000`, and label beginning `007e`.
- First underflow is at 258115002, immediately after the final held header
  Input resumes (CPU Input #3 at 258114969).
- Task 0 issues its clock Fetch at CIA 417, then the disk task preempts it.
  At 258167016, CIA 43d compares **001f - 007e - 1 = ffa0**. The saved
  deadline is still 001f; the previous clock result was 0004. The emulator
  actually executes KBootTimeout at 258167080. This is not just an address
  transient or an insufficient simulation budget.
- A 4-sample run (`boot4.log`) reproduces the same sequence: first Input
  64528520, underflow 64528752, bad comparison 64541754, timeout 64541770.
  Its disk timer is scaled correctly. Both baselines predate the pixel-clock
  correction below; their final pictures are not canonical video timing.

### Md capture: cache-bank change crossed the latch edge

`boot4-md.log` captures the memory data path over the failure. The task-0 Md
slot changes from 0000 to 007e at 64541715; this is a real write to the slot,
not the processor selecting a different task's slot. **D contains 0004 from
64541705 through 64541710. At clk1's edge (64541711), the cache bank/address
advances and exposes its previous output, 007e.** MemD's post-edge MC10176
capture takes that next-bank word, and the task-0 completion writes it.

MemD **f06/f10/f14/f18** now use pre-edge capture, preserving D's setup value.
The per-board post-edge default remains for other packages. These four
registers also feed the Md parity bits; `md-capture-test` checks all 18 bits
at the generated board's actual Md RAM inputs. Restoring only these four
packages to post-edge capture makes the test return 007e instead of 0004.
`readback-test`, `memrun-test`, `reset-tags-test`, `memory-config-test` and
`disk-tioa-phase-test` pass with the change.

### FIFO underflow: the two consuming/checking flops disagreed about setup

DskEth c16 (MC10135, OutRegFull) sampled **post-edge** J/K while c15
(MC10231, underflow) sampled **pre-edge** D. A memory hold can release Input
on clk1: c16 consumed the word on that edge, while c15 did not recognize the
read until the following clock and then found OutReg empty. The payload
words still arrived in order, but the error latch was set.

MC10135 now supports a scoped PRECAP option, enabled only on **DskEth c16**.
`disk-input-hold-test` reproduces the failure without a long boot: the old
board consumes the word before the read's setup window; the fixed board
consumes exactly once with no underflow. All four `disk-stream-test` cases
pass at SYSPER=16 after the fix. Before this c16 change, all four cases also
passed at 8 and 16 after the timer correction; those stream tests alone do
not exercise the held-Input boundary.

The CPU's minimal `Input; Block` loop passes with three or four available
words. A separate two-task test preserves RM[1a] and RM[5a] across a write
combined with RBase change. Artificial hold tests must drive all MemC hold
copies and respect the manual's t1 boundary; arbitrary release-phase failures
are not evidence for another hardware change.

### Clock parameters and MiSTer status

`SYSPER` on the machine is samples per **30 ns clock**, whereas Trident and
K1115A oscillator helpers take samples per **60 ns instruction**. Fixes:

- Propagate the machine ratio through generated DskEth instances into the
  SN74123's 3 us index discriminator. The former fixed-16 timer made an
  8-sample full run start at physical pulse 3 with garbage framing. That run
  (`boot8.log`) is not evidence about the valid 16-sample read failure.
- Pass `2 * SYSPER` to the pixel oscillator, including in generated machine
  wrappers. tb_exec previously hard-coded 16, so its 16-sample run used a
  100 MHz pixel clock. `clock-domains-test` measures the 50 MHz edge count
  against the 30 ns processor clock, and checks disk timer and drive ratios.
  It passes at 4, 8 and 16; restoring the old oscillator setting fails with
  37,499 transitions instead of 18,750 over 100,000 samples.
- The MiSTer wrapper now passes `2 * MACHINE_SYSPER` to Trident. Its README
  records the actual August 28 fit (83% ALMs, 45% RAM blocks, 47.56 MHz Fmax)
  and current blockers. Mounted file contents are still not read, reset does
  not press the BaseBoard boot button, and pixel clock/CE integration still
  needs work. No Quartus build or hardware deployment was performed.

Structural cell, pin-matrix and combinational-loop checks pass. Wrapper
Verilator elaboration also passes using a local MiSTer framework and a PLL
stub (with that framework's existing PROCASSWIRE warning suppressed); this
is not a Quartus timing/fit result.

**Full-run confirmation:** `fix4.log` removes the bad RTC timeout but still
underflows, retries, then starts Alto execution after its second transfer.
`fix-all4.log` adds c16 setup capture: **the first transfer succeeds, with no
KBootTimeout and no FIFO underflow** (final count 0). It reaches
**AEMUIFUFGPARITY, real CIA 0c4, at 64594330**. CIA 0c4 is an IFU parity
error park, not Ethernet boot. Executive has not displayed yet.

Another defect was MemD c14's low-byte parity regeneration during
storage refill. With control 13 high, MC10170 B must complement A; its
legacy B inversion instead made both outputs equal. `memory-byte-parity-test`
fails on the old board for byte 30 (needs odd-parity bit 1, gets 0). A scoped
`LEGACY_B_INVERT=0` on c14 fixes the test; readback and memrun still pass.
`parity4.log` still enters CIA 0c4, so this correction alone does not explain
the IFU stop.

### IFU fetch buffer: clocks split a word, then capture the next word

MemD's F buffer has four slices: e06/e10 MC10176s clocked by Fclk'a/b,
and e14/e18 F10016 parallel-load registers clocked by clk1'Ca/b. All four
clocks come from b07's preClk1'A fanout. The generator delayed clocks named
clk1 but not their gated Fclk siblings, splitting byte/parity captures by
one fabric sample. The generator now delays those two Fclk nets equally.
`ifu-f-clock-test` drives the board's actual external clock and checks eight
non-vacuous transitions; removing just the Fclk delays fails the test.

With the clocks aligned, `fclk4.log` no longer latches SawFGParityErr, but
executes repeated Start/StartIFU. The new `+ifutrace` shows why: D contains
**00e5 with parity 10 at 64594053**, before the F clock rises. D then advances
to the following word, 0000/parity 11, and the post-edge F buffer captures
that following word. The C oracle confirms the first Alto instruction is
00e5 (jump to 00e5), followed by 8400. The first physical sector word 8530
is not the first instruction: the disk loop stores words in reverse order.

All four F slices now use setup capture. F10016 gains PRECAP=0 by default;
only MemD e14/e18 enable it, along with PRECAP on e06/e10. The new
`ifu-f-capture-test` checks all 18 bits across four cache-word changes. The
previous board fails immediately, capturing 0000/parity 11 instead of
00e5/parity 10; the corrected board passes. F-clock, Md-capture, readback,
memrun, cell, pin-matrix and combinational-loop checks also pass.

The older `ifu-test` wiring gate passes. `ifufetch-test` fails its assertion
that the pipe pointer never changes (two changes during startup). The same
failure, at the same simulation time, reproduces with both saved MemD boards
from before the Md-capture fixes and before the F-capture fixes; it is not
introduced by these MemD corrections. Logs: `fcapture-ifu-gates.log`,
`ifufetch-baseline.log` and `ifufetch-oldfc.log` under `build/rtl-executive/`.
The test has not been weakened or marked passing.

**Full-run result:** `fcap4.log` confirms F/G contain 00e5/parity 10, with
SawFGParityErr remaining clear. The IFU nevertheless dispatches to **ffc**
instead of JMPZ at 064. It does not boot the Executive; the run was ended
at 84,003,850 samples. The 16-sample run was ended during initialization at
81,015,254 because the next harness omission applies at every ratio.

### Assisted transport must restore opcode tables AFTER Initial

`InitialMain.mc:IFUMInitL` explicitly clears all four IFUM instruction sets.
The harness preloaded the world tables before starting Initial, then only
redirected its BootEmulator branch into AEmu. That does not replace the real
LoadRam transport: it leaves cleared opcode tables. Zero InstrAddrK' decodes
to 3ff, hence the observed microaddress ffc. AEmu's opcode-0 entry is 03e6,
which decodes to JMPZ at 064, as the C oracle expects.

The harness now saves the preload's IFUM/ALUFM values and restores/readbacks
them at the assisted world handoff, after Initial's initialization. This is
a test-harness correction, not an RTL or guest-microcode patch. All three
preload images have identical ALUFM tables. New runs: `tables4.log` and
`tables16.log`, runtime limits `limit-tables4.txt` and `limit-tables16.txt`.
`WORLD-TABLES` records the old IFUM[0] and verifies all restored entries.

**Confirmed:** `tables4.log` restores IFUM[0] from 0000/0000 to 03e6/35df,
and verifies all 256 IFUM and 16 ALUFM entries. Dispatch changes, but the
first fetch is now VA **005a**, not 0001. It executes the wrong program and
eventually trips the diagnostic hold limit at 64696766. The Executive is
still absent.

### PCF counter also needs setup data across the IFU bus handoff

VA 005a is BR31=1 plus word offset 59, the stale H byte b2 divided by two.
A short reproduction starts real AEmu Start/StartIFU with H held at b2.
`PCF<-0` has IfuBMux=0000 before clk1, but on that edge NewPc1 drops,
disabling the B input and exposing the IFU ALU's b2. The four post-edge
F10016 PCF counters capture that value instead. At sample 3944 the low
word-offset byte is 59; the same run without stale H has zero, hiding it.

IFU **a04/c04/d04/f03** now select F10016 PRECAP=1. Other counters retain
their defaults. `ifu-pcf-capture-test` executes the real StartIFU microcode,
seeds stale H, and checks the completed PCF load before its next increment.
It fails on the previous board with 59 and passes on the corrected board
with zero at SYSPER=4 and 16. The initial assertion sampled before the counter's
modeled update; delaying the check exposed that and made the mutation fail.

`pcf4.log` is the full follow-up with this correction. `start4.log` is the
pre-fix full run with a 256-sample `+startupfrom=64593970` probe. The latter
exists to compare the full failure with the short reproduction, not to test
the corrected source. `tables16.log` also predates the PCF fix and remains a
reference for the earlier Md/F/disk corrections and assisted table restore.

`start4.log` confirms the bus handoff in the full boot: IfuBMux=0000 at
64594037-38 before clk1, then 00b2 at 64594039 on the edge. PCF stays at
59; RMar becomes 0059 at 64594040 and the first VA is 005a at 64594047.
No hardware state was forced in this full trace. The run then reproduces
the same hold-limit failure as `tables4.log`.

Reproduce the latest source (the assisted world transport is still required):

```sh
make -C verilog/verilator ifu-pcf-capture-test V='verilator -j 4' SYSPER=4
make -C verilog/verilator boot-world-disk V='verilator -j 4' SYSPER=4 \
  WORLDARGS='+cycles=150000000 +pack=chm/diskpacks/games-trident.pack +pendulum +hasram +ioreset +fastwait +bbword=0000 +bbfeed +diskcmdtrace +memprogress +hold-limit=100000 +check-clock-domains'
```

For the PCF startup window add `+startupfrom=64593970 +startuplen=256`;
for the first byte stream add `+ifutrace +ifufrom=64593000`. At SYSPER=16
the first disk transfer is around 258M rather than 64.5M fabric samples, so
the run and diagnostic windows must be scaled. The PCF regression is bounded
and takes only about 4,000 samples at ratio 4 (not a full boot).

**PCF fix confirmed in the full machine:** `pcf4.log` fetches VA 0001,
dispatches JMPZ at 064 with T=00e5, then fetches opcode 84 at VA 00e5.
The ADC0, LDA, COM1 and BLT sequences agree with the C oracle. The initial
BLT copy loop also agrees: source words 0, 00e5, 00ec, 0103, 2222, 5410,
090d, f200 are delivered in order. CIA 0cc is AEmuReschedule, not MapFault;
an interrupt restarts the BLT, as in the C emulator. Later, around 65.67M,
code reloaded from storage becomes zeros and the guest loops through JMPZ 0.
The run ended at 108,003,848 samples. `pcf4-frame.png` is black, with zero
white pixels; it is not an Executive screen.

The completed 16-sample pre-PCF-fix run (`tables16.log`) confirms the earlier
fixes at the reference ratio: no KBootTimeout/FIFO underflow and no latched
FG parity failure, but the same bad first PCF offset (VA 005a at 258376241).
It reaches the diagnostic hold limit at 258487096.

### The odd storage board existed but its harness clock was unconnected

`tb_exec` connected CLK.ms0Even' but omitted CLK.ms0Odd'. The generated
machine includes both msa and msaOdd, but the latter therefore ran with a
constant-zero slot clock. Both boards' memory arrays were seeded, masking
the omission in the readback smoke tests. BaseBoard supplies both clocks
in the hardware backplane; this is another test-harness omission.

The exec, readback, storage, disk and display benches now connect both slot
clocks. The standalone single-board tb_msa correctly keeps only its own
clock. `clock-domains-test` now measures both storage boards' actual input
edges. Removing only the odd-clock connection fails at 100k samples with
**even=25000, odd=0** at SYSPER=4; the fixed harness passes at 4 and 16,
and readback passes at both ratios.

`storage-test` and `display-test` pass after the hookup. `disk-test +slowio`
fails its negative bIOin assertion with one asserted sample and zero actual
IOB output-enable samples. The exact same failure at the same simulation
time reproduces after removing only the odd-clock connection; the assertion
was not relaxed. Logs: `odd-clock-peripheral-gates.log` and
`disk-oldodd-slowio.log` (the latter is the comparable +slowio case).

**Completed follow-up:** `odd4.log`, including all prior fixes, ended at
90,003,849 samples. Both storage clocks run, the first transfer still succeeds
with zero underflow and no KBootTimeout, and the guest starts Alto execution.
It nevertheless reproduces the later zero-instruction stream at the same
point. The missing clock therefore did not explain that remaining failure.
`odd4-frame.png` records the final raster; no Executive text is visible.

Next probe: retain a known boot-code word (e.g. VA 00e8 = 6205) through a
dirty cache eviction and storage refill. The initial BLT fetches 6205 at
64600402 and 5c49 at 64600418; the first reschedule refetches them correctly
around 64782365, but a later restart around 65670922 fetches zeros. This
suggests the cache/storage round trip, but its exact cause is not proved.
The readback smoke test checks a seeded storage read, not preservation of
an arbitrary dirty line through writeback; its PASS does not rule this out.

The bounded C trace
is `c-first-page.log`, obtained with DORADO_UCODE_TRACE=1 and
DORADO_TRACE_GATE=76617600,76630000 on the same pack. C trace PCs and data
are octal; BOOTIO prints hexadecimal, and C's T is post-instruction while
BOOTIO's T is the setup operand. Compare A/B/ALU and successive T values.
The C reference still has zero display-list pixels at 100M and 130M BB
cycles and reaches 2,092 by the original 180M-cycle gate. Those are C-only
reference results, not evidence of RTL rendering.

The existing PHT diagnostic also armed itself indefinitely: its remaining
40-sample window was incorrectly used as the total pulse count. It now prints
only two 40-sample windows, as its comment intended; this only bounds logging.

Runs are on the Mac. Linux alias `misterubuntu` is available but the user
reports slower Verilator there. `build/rtl-executive/run-state.json` tracks
processes and runtime limit files. The three MiSTers are in use; request
allocation only when hardware testing is useful. No Quartus or hardware
access was needed for these fixes.


## 2026-09-09: cross-task slow-I/O address overlap isolated

**Full assisted boot attempt completed: still no OS boot.**
`/private/tmp/dorado-sep09-postclock-boot.log`, limit file
`/private/tmp/dorado-sep09-postclock-boot-limit.txt` (shortened from 350M to
280M after fallback), executable
`/private/tmp/dorado-sep09-history-obj/tb_history`. It includes the selected
Index, continuous media, corrected pack phase, and TIOA timing changes below.
The software transport is still replaced by the `c92 -> 23e` handoff.

The preceding tag-latch run completed at 173M. It identifies the actual
corruption BEFORE the previously measured reload loss:

```
172235921  disk task lowers its last DriveTag: 8800 -> 0800
172536465  entry to AltoLoop, CIA 58a: tag latch takes 90ff, TIOA=014
172537329  ordinary next selection: 90ff -> 0810
172537330  subsector reload register: 3 -> 0
```

AltoLoop's first instruction is `T_ clearSeekTagTW, Call[DoMuffOutput]`,
NOT an Output. The old task's still-active output crosses into the new
task's disk address. The stray `90ff` asserts DriveTag and load-subsector;
the next normal tag removes them while changing the data to zero, clocking
that zero into the subsector register. The LS174 itself is not the fix.
All preceding initialization and selection tags latched correctly.

The short clock trace (`/private/tmp/dorado-sep09-disk-clock.log`) showed
ProcH's TIOA changing one fabric sample BEFORE DskEth02 e02's FHCP latch
closes. ProcH30 k10b explicitly adds a buffer from Clock0'Bc to
**PostClock0'Bc**, and ProcH23 g10/h10 use that post tap for TIOA. The
generator modeled it with zero delay. `clock_lag_stages` now preserves that
extra stage. Those two F10000 registers also need pre-edge parallel data
capture, as the other register models already support: adding only the
post-clock delay samples the next bypass/mux value and FAILS InitRamDiablo
(`/private/tmp/dorado-sep09-postclock-short.log`). The scoped PRECAP option
on g10/h10 preserves their setup data; all other F10000 instances retain
their prior timing and serial shifting is unchanged.

`disk-tioa-phase-test` executes real disk initialization and asserts that
TIOA changes after the disk address latch closes. The fixed model checks
13 changes and passes the full sixteen-word format check. Rebuilding with
the old ProcH board fails at 278601, TIOA `000 -> 010` with the latch still
transparent (`/private/tmp/dorado-sep09-phase-baseline-test.log`). Compute
and cell checks pass. `disk-tioa-phase-test`, `memory-config-test`,
`reset-tags-test`, `exec-test`, `boot0-test`, `eth-ctl-test`, and
`memrun-test` also pass (logs `/private/tmp/dorado-sep09-postclock-startup-gates.log`,
`postclock-io-gates.log`, `postclock-memrun.log` under the same temp prefix).
The full-run result below confirms progress but is not a boot-success claim.

**Full-run confirmation through 177M:** initialization is unchanged and
passes. The stray tag write at 172536465 is GONE. At 172537329 the next
selection now correctly changes `0800 -> 0810`, not `90ff -> 0810`; the
subsector reload stays 3. This verifies the diagnosed cross-task failure is
fixed in the original long-run context. The run is continuing through
ABoot's legitimate real-time wait to its first disk command.

**Completed full-run result (280,015,252 samples):**

- DiskBoot enters at 249635913, as before. Read command `00fc` occurs at
  257781050. Active now starts at **258081889, physical pulse 4, angle 13,
  media phase 1**, rather than the old pulse 1/data-phase start.
- The stream is recognizable and correctly framed: header `0000 0000`,
  header checks `af80 047e`, label starts `007e 0000 0001 0001 0200`,
  boot code starts `8530 4126 8240`. This is the cylinder-3/head-4 boot
  sector used by the focused controller test, not an empty spare slot.
- Output-register events total 280: two startup events plus the full 278
  sector words (266 payload and 12 checksum/remainder words). These events
  are logged BEFORE register refill, so each printed value describes the
  previous holding-register value; do not treat them as CPU Input samples.
- ReadDataErr remains **3155**, exactly the startup baseline: no new ECC
  error. FIFO overflow is zero. **FIFO underflow is latched**, totaling
  261913 high samples. Those are durations, not independent error counts.
- **KBootTimeout is entered at 258167049 while the first transfer is still
  active**, and the guest falls back to Ethernet. The disk task finishes and
  clears its command at 258374682. Only one DiskBootRetry entry occurred.
  The final raster is still effectively black, not an Executive screen.

The next diagnosis is now at the CPU/memory/IO-consumption boundary, not
the disk's angular phase. `AEm0.mc` initializes ETemp0 to VM[0430] + 033B
and KWait branches to KBootTimeout if that deadline minus the current clock
minus one is negative. It promises roughly one second, yet this timeout is
only 8,530,400 fabric samples (~16 ms) after KWait first entered and ~160 us
after Active started. Capture the FIRST FIFO-underflow edge and the KWait
ALU/MD inputs and deadline before/through the read. Useful RM symbols:
ETemp0=001a, ETemp1=001b, ETemp2=001c, ETemp3=001d, R400=0010, KStatus=005a.
Do not guess that the disk is simply too slow, or repair this by enlarging
the guest timeout. No new hardware patch for this remaining issue was made.

## 2026-09-09 continuation: selected Index status

A full run with the continuous-media correction was started, then stopped
at about 49M samples before the world handoff after finding a second
selection defect. That partial run (`/private/tmp/dorado-sep09-media-boot.log`)
is not another disk-read failure.

Trident 76205-902 section 4.2 (printed pages 4-2 through 4-4) describes
Index among nine bussed status outputs gated with Select. In contrast,
Composite Sector/Index is explicitly NOT gated with Select. DiskSubrs.mc
UpdateSector calls out the resulting missing IndexTW when deselected.
`dorado_trident.TtlIndex_n` now obeys that gating. The independently
rotating SecIndx stream and subsector-counter reload are unchanged.
`trident-test` checks both the selected Index pulse width and its absence
over a full deselected revolution; the strict controller stream and actual
microcode format tests pass. The data-pair counter in the test is reset
before its separate measurement (it had shared the Index accumulator).
An otherwise identical temporary drive with the old ungated Index fails
the new deselected-Index assertion.

`disk-stream-test` now also exercises `+streamsector=1`: the command is
issued after Index and starts on the ordinary divided pulse 4. It reads
the corresponding file slot with all payload/ECC checks passing. A separate
`+streamsector=28` run likewise passes at pulse 112, the last complete slot.
These check the current pack angular mapping, not its still-to-be-confirmed
match to the boot microcode's command-sector convention.

The replacement full run is `/private/tmp/dorado-sep09-selected-index-boot.log`,
with `+diskcmdtrace` and all preceding fixes. It finished normally at
269M samples after shortening the limit. **Still no boot.** The first
command is correctly `00fc` at 257781146 (pulse 0, angle 3162), but Active
starts at 257853990 on **pulse 1**, divider count 0, with the media already
in its data phase. The first stream is still wrong. This is a concrete
difference from the isolated normal-sector test, which starts on pulse 4.
The count printed at Active is the running counter, NOT its reload latch;
do not conflate them or claim the reload latch became zero without a trace.

The next run, `/private/tmp/dorado-sep09-reload-watch.log`, finished normally
at 179M samples. It caught the actual reload latch changing **3 -> 0 at
172537330**, CIA `712`, IOB `0810`, physical pulse 48. That ordinary drive
selection does not request a subsector load. The unwanted clock into the
reload register, rather than the rotational counter itself, is now the
specific target. Its limit file was
`/private/tmp/dorado-sep09-reload-watch-limit.txt`.
`+diskcmdtrace` now additionally logs reload-latch changes (`DISK-RELOAD`)
and the first five pulse positions of each revolution (`DISK-PULSE`), with
the one-shot value and count. A short actual-microcode format probe with
a rotating pack (`/private/tmp/dorado-sep09-pulse-probe.log`) keeps reload
3 and correctly counts 3,2,1,0 across pulses 1..4 after Index. Therefore
the isolated AND short full-board probes do not yet reproduce the long
boot's phase difference.
An additional isolated `+pollsectors +fastio` controller probe spaces
selection/deselection polls at pulses 0,4,...48 and still retains reload 3,
then reads a complete sector with zero ECC remainders and no error latches
(`/private/tmp/dorado-sep09-timed-poll-test.log`). Thus neither pulse 48
alone nor repeated selection reproduces the full-machine corruption.
`/private/tmp/dorado-sep09-reload-history.log` completed at 173M samples.
The preceding 256-sample capture shows the tag register ALREADY holding
subsector data 3 with DriveTag asserted and load-subsector enabled; its
derived load clock is low. At 172537328 TagClock' opens for the ordinary
`0810` Output; at 172537329 the tag data becomes zero, DriveTag deasserts,
load-subsector disables, and the derived load clock rises. The LS174 then
captures zero. Do not paper over this by forcing the reload to 3: find when
the stale/incorrect tag register value was introduced.
`/private/tmp/dorado-sep09-tag-latch.log` is the next full reproduction,
logging every actual tag-register change (`DISK-TAG-LATCH`) with IOB,
TIOA, task, CIA and strobe state. The short actual-microcode initialization
probe `/private/tmp/dorado-sep09-tag-short.log` latches every tag correctly,
including the final `88f0 -> 08f0` transition; that probe does not reproduce
later full-machine activity.
The disk diagnostic flags are now read once at startup rather than scanning
argv on every fabric sample; this does not change any simulated signal.

The controller probe now has `+pollselect` and `+fastio` variations to
exercise repeated selection and 64-sample Output spacing. It uses timed
head/cylinder tags as SendTag does, rather than withdrawing them early.
Adding the proper wakeup-clear sequence exposed a sampling error in the
probe: it was inspecting OutReg AFTER Input's subsequent refill, not the
current word Input consumes. With FIFO/output-register state properly
cleared, this skipped the first header word. The consumer now samples the
current OutReg before consuming it, and permits a final full OutReg even
when the FIFO behind it is empty. Both sector 0/1 gates and the fast-I/O /
selection-poll variant pass, including error-latch assertions. This is a
probe correction, not a hardware fix or a full-processor I/O validation.

Remaining drive-model fidelity debt: Ready/OnLine and pack EndOfCylinder
still use the earlier presence/status convention, not the complete selected
bussed-status behavior. The Index change does not claim to resolve these.

### Pack phase correction, added after the reload watch launched

The C disk model is not authoritative for angular phase: `disk.c`
`disk_start_pending_transfer` deliberately starts reads immediately in the
CURRENT command sector, whereas HM 9.5 and DiskSubrs.mc UpdateSector say
the real controller executes the command in the NEXT sector. The converter
stores Alto logical sector 0 in file slot 0. Thus that slot must begin at
physical pulse 4, not at Index. Alto sectors 0..27 now occupy pulses
4..112. The spare file slot 28 occupies the otherwise unused complete
slot after Index, and pulse 116 remains the unusable fraction. This codec
is explicitly specific to the existing Alto pack, not a general Trident
format implementation.

The serial `pack-test` passes, and `disk-stream-test` now checks file slots
0, 1, and 28 at pulses 4, 8, and 0 respectively, plus the fast-I/O /
selection-poll variant. All four strict controller cases pass. A temporary
pack restoring only the old angular mapping fails the sector-0 controller
test immediately: header 1000 instead of 0000. Logs:
`/private/tmp/dorado-sep09-sector-phase-gates.log` and
`/private/tmp/dorado-sep09-old-sector-phase-test.log`.
The completed reload-watch, reload-history and tag-latch processes
have the OLD pack phase (the latter two use a temporary old-phase copy);
they diagnose the separate wrong-divider-phase issue, not validate
this last mapping correction or claiming an OS boot.

## 2026-09-09: ABoot wait works; disk framing and parity defects found

**Latest completed full run: still no OS boot.** Log
`/private/tmp/dorado-sep09-ecc-boot.log`, ended normally at 350M samples
using `/private/tmp/dorado-sep09-boot-limit.txt`. It includes the additional
counter/index/ECC fixes below, but NOT the subsequent continuous-media fix.
It reaches DiskBoot at 249635913, makes eight attempts, delivers 259 words,
and reaches KBootTimeout at 275665641. The first read is still wrong
(`0040,0000,0001,3f09,e185,...`). Final error sample counts: ReadDataErr
7795, FIFO overflow 53849, FIFO underflow 104562. These are accumulated
high-signal samples, not numbers of independent errors.

The focused controller gate now reads all
266 payload words correctly and returns six zero ECC remainder words.
This is not yet an OS boot claim. Mutation gates are confirmed: the old
50 us one-shot fails index reload (count 1 instead of 3), the old MC10136
sampling fails block boundaries (ECC slot receives label word 007e), and
payload-only ECC fails on remainder 3fe0. `memrun-test` still passes.

**Previous full run: still no OS boot.** The cable/head-fixes run finished normally
at 300M samples (the optional run-limit file shortened the original 350M
limit once repeated bad reads were established). Log
`/private/tmp/dorado-sep09-cable-boot.log`. Both memory-configuration and
disk-format checks PASS. DiskBoot still starts at 249635913; six attempts
deliver 114 words. Legal head 4 now gives HeadOvfl=0 and selected idle
ReadError=0, confirming the cable/head fixes independently of boot success.
But the first read at 257862266 still produces a short wrong stream
(`0000,1258,816e,fb3e,...`, not the file's zero header). Final counters:
ReadDataErr high 3155 samples, FifoUnderflow 36413, FIFO overflow zero.
The saved frame is mostly black, inspected as
`/private/tmp/dorado-sep09-cable-frame.png`. That run has finished; the
newer run named above follows the focused controller/cable fixes below.

### Last correction: deselection must not stop media position

The drive reset its bit-rate divider while deselected, so `data_adv`
stopped and the pack's stream state froze while sector/index rotation
continued. Selecting a drive partway through a sector resumed the old
preamble at the wrong angular position. The always-selected controller
probe did not exercise this difference; the real microcode repeatedly
selects/deselects the drive. The Trident specification's section 4 clock
description ties the clock to servo/data on the rotating pack, not a
selection-relative stream.

`dorado_trident` now advances media whenever attached, gating only the
external cable pairs with selection. `trident-test` checks exactly one
track's worth of internal bit advances over a deselected revolution while
the cable remains quiet. `pack-test` now waits for a complete sector
boundary after selection rather than demanding the whole preamble from a
sector already in progress. Both pass, as does the strict controller
`disk-stream-test` and real-microcode `disk-init-format-test`.
Restoring only the old selection-frozen divider in a temporary model makes
the new deselected-revolution assertion fail; no repository model was
reverted for that mutation. Final reruns of `sector-counter-test`,
`parity170-test`, `reset-tags-test`, `memory-config-test`, `memrun-test`,
and `baseboard-test` also pass.

**This last fix has NOT had a full boot rerun.** Do not claim it boots.
The next full run should add `+diskcmdtrace`: this logs disk-control writes
and the first forty Active rising edges with physical pulse, within-pulse
angle, divider count, and pack phase. Do not infer Active's start solely
from the later DSKDATA output-register trace. The short actual-microcode
format probe now also prints the programmed subsector value: **3**, as
expected for divide by four. The hypothesis that initialization loaded 0
is ruled out; the short probe has no attached rotating pack, so its live
counter value of 0 is not evidence of a rotating-divider fault.

### The controller-level reproduction found three more real defects

`make -C verilog/verilator disk-stream-test` uses the actual DskEth board,
slow-I/O ports, drive cable, format RAM, and FIFO. It independently checks
the file's 266 header/label/data words and the six hardware ECC remainder
words. The initial setup word is distinguished from disk data. It does NOT
patch any controller storage, replace microcode in the full boot, or force
errors clear. `+streamtrace` adds per-word/sequencer diagnostics.

The setup itself needed accurate I/O timing: a strobe changing while the
board's Clock1 window is open can cause two RAM writes, so the test aligns
each strobe to that window and verifies all sixteen stored words. Board
feedback signals (OKToSelect, OS0..3, TtlSector') must also be connected,
not silently tied low or replaced by a raw drive pulse.

The final gate also checks IOB parity, FIFO overflow/underflow, and
ReadDataErr latches, all clear. To make that check meaningful, the isolated
probe follows ClearDisk's two reset writes/tag reset and InitRamDiablo's
clearAll, waits for a nonempty FIFO, and spans only ONE Clock1'Ca edge
with each Input pulse. Using the format-write preclock for Input had
spanned two Ca edges and fabricated an underflow; checking the returned
word before the following output-register clock included stale data.
These were probe errors, not additional hardware fixes.

1. **MC10136 missed every counter load.** TriconD03 a19/c12 derive
   `LoadCnt'` from `WordClock'` itself. At the rising edge it has already
   deasserted; sampling its post-edge value left c19/c20/c21 at zero, so
   every data length and preamble delay was treated as zero. The cell now
   samples data/control/carry in the pre-edge setup window, like the other
   synchronous cells; its carry OUTPUT remains combinational. The probe
   now shows the real counts loading (1, 9, 255, etc.) and decrementing.

2. **The index discriminator was an invented 50 us.** TriconD06.sil,
   DskEth schematic PDF page 13 / printed page 12, explicitly draws a
   **3.0 us SN74123 pulse**, **1.2 us sector**, and **4.5 us index**. It
   also shows the actual 27K / 330pF network, contrary to the old model's
   claim that the values were absent. Ordinary pulses count; index outlasts
   the one-shot and reloads the LS169. The drive now generates the fixed
   widths (within the T-80 spec's 1.24+/-0.24 and 4+/-1 us), not a fraction
   of sector spacing. `sector-counter-test` checks the actual one-shot /
   LS169 feedback circuit, including index reload when NOT at terminal
   count. A separate integer-rounding bug generated a phantom pulse 117
   just before index; the division remainder now extends the last slot.

3. **Fire-code ECC must include SYNC.** With data now correct, the zero
   header still returned remainder `3fe0 0102`. The real divider is already
   enabled while hunting for sync; its input includes 201B. The pack had
   copied the C emulator's payload-only CRC, but `disk.c` explicitly calls
   that self-consistent rather than matched to physical hardware. Including
   sync gives header check words `af80 047e` and remainder `0000 0000`.
   Header, label, and all 256 boot-program words now pass through the real
   controller with zero remainders. The pack's independent serial checker
   likewise includes sync.

The requested 300M run proved that the previous 190M run was simply too
short. ABoot reads RTC values 0, 1, 2, 3, then leaves its wait and reaches
DiskBoot at **249635913**, DiskBootRetry at 249635945, and KWait at
249636649. Its comparison is `3 - RTC`, looping while nonnegative, so
RTC must reach 4. This is the actual `AEm0.mc` sequence, not a timing fix.
The run made six disk attempts, delivered only 56 words, and did not boot.
Log `/private/tmp/dorado-sep09-boot300.log`. The drive correctly sought
cylinder 3/head 4, but delivered words such as `bcf4` where the file has
zero header words. No KBootTimeout occurred.

### Disk media framing

`dorado_pack.sv` emitted ONE sync bit. HM 9.3 and
`AltoDiabloDisk.mc` ACmmdTable explicitly specify **201B**, a full sync
word; 001B is the different Alto Trident format. The hardware detector
requires ShiftReg.08 AND ShiftReg.15 unless the alternate format is
selected. The old single bit therefore missed zero headers and could
lock onto an incidental pattern inside later data.

The zero spacing was also shorter than the read-start sequence. It now
follows HM 9.6's WRITE sequence: 32 words before first sync
(head select 1 + settle 2 + write tag 1 + preamble 28), and 11 words
between blocks (postamble 2 + advance 1 + write tag 1 + preamble 7).
The sync word itself occupies another complete word. The old 12/4
spacing had been inferred from READ delays, which is not how a disk is
formatted.

T-80 capacity is **20,160 bytes = 10,080 words per track**, not the former
9,360 inferred from 80 words times 117 pulses. Source: Trident
`76205-902_Performance_Specification_Models_T25_T50_T80_T200_and_T300_198011.pdf`,
Table 2-1. `dorado_trident` now accepts `WORDS_PER_TRACK` separately from
the electronic-sector pulse count and uses a fractional divider, avoiding
per-half-bit truncation. The full bench and pack test select 10080 words.

`pack-test` now decodes the documented 201B pattern, checks preamble
lengths and completion within the four-pulse short-sector slot, and has
a real watchdog (the former timeout compared a wrapping revolution
counter to a value larger than one revolution). PASS: all 266 words,
three blocks, zero mismatches and cancelling Fire-code check words.
A temporary 001B-sync mutation fails. `trident-test` also checks the exact
number of bit cells per revolution and passes.

### Disk IOB parity: an old chip-model inversion is a real blocker here

A bounded microcode probe enters disk task initialization at real 3001B
(`601`) from Initial's BootMem (`c64`), after processor setup. It executes
`InitRamDiablo` and checks all sixteen format words at real 3007B (`607`).
This **passes**, unlike the old per-word jam `disk-format-test`. The three
old disk-input/command/format tests still fail their previously documented
stimulus-related assertions; they were not repaired or counted as passes.

The same probe showed every valid IOB word (`XOR(byte, parity)=1` on both
bytes) producing `dIOBParityErr=1`. The MC10170 datasheet's logic diagram
(ON MECL DL122 rev7 p.370 / PDF page 372) has **plain XOR**, not XNOR, at
pin 15. Its truth table's even-parity behavior assumes control 13 HIGH,
14 LOW; the old model inserted a second inversion. DskEth f04/g04 use
exactly those control levels, so every valid output latched an error once
ClearErrors was released.

**Compatibility debt is explicit, not a physical chip option:**
`cell_MC10170.LEGACY_B_INVERT` defaults to 1 to preserve the existing
unreconciled parity paths on other boards (see the older handoff's global
MC10170 experiment). The generator selects 0 only for DskEth f04/g04,
giving those checkers the datasheet function. This is NOT forcing errors
off: bad parity still asserts the checker. The new `parity170-test`
exhaustively checks all 2048 data/control combinations with legacy mode
off. The microcode format gate also asserts no IOB parity error was
latched. Both pass after the correction; the detailed probe shows valid
outputs giving `dErr=0` and `IOBParityErr=0` throughout.
The gate also checks the combinational checker on valid parity samples,
even while ClearErrors masks the latch. A temporary DskEth generated
without the two parameter overrides fails at sample 278681 on valid
word `0400`, proving this regression is detected.

New targets: `disk-init-format-test`, `parity170-test`. New optional
`+diskerrtrace` records bounded parity samples during the short probe.
`+memprogress` now includes ALU operands and disk error sources. An earlier
framing-only long run was deliberately stopped because it lacked this
verified parity correction; it is not another failed boot result.

### Drive cable polarity and head-range comparison

The framing/parity run passed the full microcode format check and, unlike
the baseline, showed `ReadError=0` when selected. It still read wrong data
and showed `HeadOvfl=1` even while the pack reported head 4. It was stopped
around 322M after these next two defects were isolated (log
`/private/tmp/dorado-sep09-fixed-boot.log`).

1. `(~tagbus_n[5:0]) >= HEADS` is NOT a six-bit complement: the integer
   comparison context extends its operand before complementing, making
   every head appear out of range. A six-bit `tag_head` wire now fixes the
   comparison and the immediate track-load predicate. `pack-test` checks
   legal 4, illegal 5, then legal 4 again. The pre-existing control-reset
   treatment of the overflow latch was not changed in this fix.
2. Data and clock have different differential polarity. TriconD07.sil's
   f01 compares ClockP against ClockM, but f02 compares **DataM against
   DataP**. The output path agrees: b02/g02 drive complemented WriteData
   on DataP and true data on DataM. The drive had copied the clock's
   convention onto data. The drive now emits the correct pair, and
   `trident-test` samples through an actual `cell_MC1650` wired as f02
   rather than copying the transmitter's assumption. The patterned-data
   round-trip and the pack framing test both pass. `pack-test` also now
   decodes through the actual MC1650 data AND clock cable receivers, with
   all 266 words and three checksums passing. Temporary mutations restoring
   the old head comparison or cable polarity each fail their focused test.

`+cycle-limit-file=PATH` optionally rereads a decimal run limit once per
million samples, allowing a progressing run to be extended (or finished)
without repeating initialization. It only changes the testbench duration,
not any machine state. Short test: `+cycles=350000` with a file containing
400000 reports `RUN-LIMIT -- now 400000 samples` and still passes the
microcode format/parity gate.

## 2026-09-08 follow-up: three configuration defects found

**Current verification: Initial AND AEmu memory initialization complete;
OS boot not yet demonstrated.** The 190M-sys_clk run with all three fixes,
without `+stperroff`, finished normally. Initial enters AEmu at 96896585;
GetMemConfig reports the correct 1024 pages/module; physical storage
clearing finishes around 166M. Task 0 then executes ABoot's clock wait
at `1de/1ed` (VM `430B` = `118` hex), while disk/display/junk tasks run.
The framebuffer is a mostly black raster, not an OS screen.

Do NOT call the clock wait a stall from this run. `AEm0.mc` explicitly
waits at least 100 ms for keyboard updates (`ETemp0 <- 3 + MD`, then poll
the RTC). With 32 fabric samples per 60 ns microinstruction, the 24M
samples after initialization allow only about 45 ms. A next run needs
at least 250M total samples; 300M gives useful margin. Inspect RTC/Md
values if the wait persists beyond that, rather than assuming the earlier
Md-collision diagnosis still applies.

The final report distinguishes controller setup from software boot:
693 distinct IM addresses, longest same-address run 663 at `800`,
`DiskBoot first @-1`, `KWait first @-1`, only 2 DSKDATA words. The pack
observed 25 blocks / 46502 bits during controller initialization, but
these are NOT evidence that the Alto boot command was issued. Junk-task
acknowledgments continue; no 100K-sample sustained-hold watchdog fired.
Log: `/private/tmp/dorado-codex-correct-size-boot.log`; inspected frame:
`/private/tmp/dorado-codex-correct-size-frame.png`. No simulation is left
running. Earlier long-run results below predate the storage-size correction.

Next full run (still an assisted world-transport handoff, NOT an unassisted
BaseBoard cold boot):

```sh
make -C verilog/verilator boot-world-disk WORLDARGS='+cycles=300000000 +pack=chm/diskpacks/games-trident.pack +pendulum +hasram +ioreset +fastwait +bbword=0000 +bbfeed +hold-limit=100000 +memprogress +check-memconfig +mdpc=1e2 +mdwinlen=2000 +memwinlen=2000'
```

The evening diagnosis below missed an earlier failure. During Initial.mb's
`ResetTags`, Store at real `6034B` (`c1c`) and Fetch at `6250B` (`ca8`)
both lost their Md completion writes. DisHold concealed this until the
later SetTestSyn store at `c6e` needed the tag to be valid.

**Confirmed cause, not a clock adjustment:** MemC generated an inconsistent
mixture of the two physical cache-tag configurations. MemC05.sil's b22 XOR
was connected, storing parity in `PrVA.4/Par`, but MemC11.sil's comparators
also compared that parity against VA[4]. On ResetTags' negative base address,
the eight high comparator bits were `ff` against tag `7f`: VA[4] alone
manufactured the miss. The apparent idle hit was the comparator enable
being off, not a successful cache reference.

The original schematic explicitly requires **five output pin cuts** for
parity mode: e20.3, e21.3, f20.3, f21.3 and g20.2. See
`DoradoDocs/doradodrawings/MemC-Rev-Be.press!1.pdf`, PDF page 13 / sheet
MemC11: "Cut 4 113 legs and 1 174 leg for parity rather than 16 bit CVA".
PDF page 7 / MemC05 describes the alternative VA[4] blue-wire conversion;
that is NOT the configuration implemented here. HM section 5.11 describes
the parity/VA[4] choice. The generator now applies the five documented cuts,
retaining the XOR/parity configuration. No generic cell function changed.

Checks so far:

- `make -C verilog/verilator reset-tags-test`: bounded 800,000-sys_clk
  Initial run, asserting both Md-tag completions; also passes without
  `+stperroff`. Completion writes occur at 751040 and 751264.
- Mutation test: generate a temporary MemC with `CUT_OUTPUT_PINS = {}`;
  the same test fails at the Fetch with "ResetTags reference never wrote
  its Md completion tag". Repository RTL was not reverted for this test.
- `make -C verilog/verilator memrun-test`: PASS.
- `make -C verilog cell-check`: PASS (connectivity and polarity).
- A 110M-sys_clk run with the existing `+stperroff` configuration passes
  the old c6e hold: TASKINITLOOP at 96875368, BootEmulator at 96896585,
  then AEmu's InitMap runs through the limit (last TNIA 6af, 250 distinct
  addresses, longest same-address run 9). MDhold and RefHold each total 0;
  no disk boot reached yet. `+enterworld=23e` still substitutes the boot
  transport, so this is NOT a clean cold OS boot.
- The unmasked run also passes c6e and reaches TASKINITLOOP and
  BootEmulator at EXACTLY the same cycles as the masked run. Initial does
  not need `+stperroff` with the cache parity cuts. It completes a 115M
  run in AEmu's map routines (255 distinct addresses, last TNIA 850,
  longest same-address run 44 at 734; MDhold 0, RefHold 828416). No disk
  boot yet.
- The extended 220M run (before the storage-size correction, with
  `+stperroff`) reached the page-zeroing loop at 143879145 and passed the
  old late-stage stall. It still had not reached DiskBoot at the limit:
  268 distinct addresses, longest same-address run 663 at 800, no sustained
  hold caught by the 100K watchdog. This was not evidence of a completed
  memory pass: GetMemConfig was reporting the wrong module size, below.

**The Makefile entry override was also broken.** Both boot-world targets
prepended `+enterworld=25b` to WORLDARGS. Verilator's `argPlusMatch` returns
the FIRST match, so the documented `WORLDARGS="... +enterworld=23e"`
still entered StartEmulator and skipped InitMap. Confirmed in the baseline
log's startup banner. Both targets now default to `23e` and omit that
default if WORLDARGS already supplies an entry. `mbdis -d AEmu.mb!2`
identifies INITMAP at real `1076B` = `23e`. The long follow-up runs above
invoked the binary directly with a single `+enterworld=23e`; they are not
affected by the former Makefile mistake.

### Storage size: a second missing assembly pin cut

The simulator's 14-address-bit storage chips implement 16K bits each:
one module is 256K words = 1024 pages. But actual AEmu `GetMemConfig`
reported **16384 pages/module**, corresponding to 256K-bit chips. A
bounded probe enters GetMemConfig at `200` from Initial's BootMem (`c64`),
after processor initialization; it needs only 300K fabric samples, not a
full boot. At real `4136B` (`85e`), T was `4000` hex rather than `0400`.

MemX23.sil specifies the distinction explicitly: if 256K chips are NOT
installed, break **g10.6 (MC10104)** before stuffing. See
`DoradoDocs/doradodrawings/MemX-Rev-Ch.press!1.pdf`, PDF page 25,
instruction 1; HM section 5.10 explains the shared 256K/16K size wire.
The generator had deliberately omitted this conditional cut because the
chip-size wires are external. That reasoning was insufficient: the SAME
external wire denotes 16K or 256K chips, and this physical cut tells them
apart. The storage cells now concretely model 16K chips, so the cut is
required. Without it the guest clears sixteen times the installed storage.

`BROKEN_PACKAGE_PINS` now includes MemX g10.6. A future 256K-chip storage
build must restore this pin AND enlarge the storage models. Map size is a
separate choice; GetMemConfig's VirtualBanks computation was 64 (16K map),
so it was the storage size, not the virtual-map-size probe, that was wrong.

`make -C verilog/verilator memory-config-test` asserts the actual microcode's
result is 1024 pages/module. It also verifies the Makefile honors both
`+enterat=c64` and `+enterworld=200` without prepending conflicting defaults.
The test also fails with a temporary MemX generated without the g10.6 cut
(16384 pages/module), confirming that it detects this specific regression.
After both board changes, `reset-tags-test`, `memory-config-test`,
`memrun-test`, and `make -C verilog cell-check` all pass.

**Another correction to the evening diagnosis:** `dSTPerr` is NOT a
"syndrome-nonzero flag". MemD04.sil (MemD-Rev-Ca PDF page 6, Hamming code
generator) shows k15 checking the parity bits EcSout.16/17 against the two
outgoing data bytes; h16b/k13g/d01f accumulate and register that transport
parity error. Storage-read syndrome checking is a different path. Idle
`dSTPerr=1` percentages do not establish corrupt storage or missing check
bits; examine valid transfer/EnableSTPerr timing. Both masked and unmasked
runs now complete Initial identically.

New optional `+cachewin=N +cachewinlen=M` prints bounded per-fabric-cycle
address, hit, pipe and comparator inputs. `+hold-limit=N` optionally stops
a boot after N consecutive held fabric samples, printing the hold sources;
it is a diagnostic watchdog, not a hardware timeout (default disabled).
The loaded Initial.mb and the available InitialSubrs.mc differ:
ResetTags' actual MCR is `0341B`, without
DisBR, despite the source listing including DisBR. The C emulator executes
the same `0341B` and negative BR; do not change RTL to match that stale source.

## Previous handoff (2026-09-08, evening) -- retained history

Branch `verilog-backplane`, head `dc2f03ca`, everything uncommitted. This
block is the state at the end of a full day of autonomous boot-driving; the
"later" block below it is the same morning's state and its blocker (the map
walk rewriting 64 entries) is fixed here.

### What was fixed today, each from a data sheet or the wire list

1. **`cell_MC10179` was a SKELETON** (MemC g05, the BR+Mar adder's look-ahead
   carry block). CO20'/CO12' undriven: no carry ever left VA.20-23, so a base
   register walked by 256-word pages wrapped after 64 pages and InitMem.mc's
   Map1to1Loop rewrote the same 64 entries for 290 M sys_clk. Modelled from
   MC10179.pdf; the data sheet's equations are on the PIN LEVELS (the part
   takes the F100181's active-low G'/P'/Cn' and returns active-low carries).
   EclDict numbers the groups MSB-first (its GI0 = the sheet's G3); the cell
   header has the pin table.
2. **`cell_MC1672` was a SKELETON** (triple XOR): MemC h17/h18 make "hit column
   differs from victim column" and `HitOrEc`; IFU d07.
3. **`cell_MC10163` was a SKELETON**: MemD a14/b14/j12/l12, the storage ECC
   check-bit generators (Motorola "IBM code", MC10163.pdf). Assumed odd-parity
   outputs (the MC10160 convention).
4. **`cell_MosRam` (the MAP) was 4K deep with seven address pins wired.** A
   4K x 1 DRAM has six pins; HM p.126 says "21 16k, 64k, or 256k x 1". The
   cell dropped two row bits and folded the 16K-entry map 4:1. Now 16K.
5. **`cell_MK4096P_addr` / `cell_MK4096P_6` (the STORAGE): the same defect**,
   HM p.10 "256k x 16-bit memory modules (using 16k-bit storage chips)". Both
   14-bit now; the generator's shared-latch wire is `[13:0]`; tb_readback,
   tb_exec's seeds and tb_msa's census loop to 16384.
6. **The bench's `RfshPeriod` was 8.3x too fast** (1.9 us; HM p.129 says
   "refresh references once every 16 us"): the map did a refresh between
   EVERY pair of InitMap's map writes. Default is 16 us now, `+rfshdiv=N`
   overrides. Initial reaches BootEmulator ~10 M sys_clk sooner.
7. **A BACKPLANE ALIAS HAD MERGED TWO MemX NETS, and the <-Md hold lost its
   input.** MemX has `EcDcomingForCt'` (f23.2o -> f22.11i, out on E39) AND a
   board-local `DcomingForCt'` (f22.15o -> f23.13i, g24.11i, e50). The
   2026-09-05 entry ('MemX', "EcDcomingForCt'") -> "DcomingForCt'" renamed the
   first onto the second inside MemX; a net cannot have two drivers, so the
   generator silently dropped f22.15o, f23.13i and g24.11i -- and g24 is the
   OR (CacheRef | SW | DcomingForCt') behind e19's DdataGood', the
   data-not-good input of MemC b23's Md-hold request. HOLDSRC read "MDhold 0"
   in every boot. The join is now on the MemC side (('MemC', "DcomingForCt'")
   -> "EcDcomingForCt'"; MemC has no local net of that name) and
   `sil_netlist.py` REFUSES any alias whose target already names a net on the
   board (mutation-tested against the old entry). Bench probes read
   `m.b_MemC.EcDcomingForCt_p_`.

8. **THE MACHINE HAD HALF A STORAGE MODULE.** The msa is 144 chips = 4 banks
   x 36 bits = two words + four check bits, HALF of a 72-bit quadword (HM
   p.129: 64 data + 8 check); HM p.10's "256k x 16-bit module (16k-bit
   chips)" is 288 chips = two boards. The BaseBoard fans out CLK.ms0Even'
   AND CLK.ms0Odd'; MemX drives LoadSinE/ShiftSinE/LoadSoutE'/ShiftSoutE
   AND their O twins; MemD exports EcOut.0-7 where the even board's .bp
   claims EcOut.0/1/4/5. Only the even board was instantiated: every odd
   word of every quadword had NO STORAGE and half the check bits went to
   unconnected top-level ports -- which is exactly what a syndrome-nonzero
   flag reading 1 on 99.7% of samples looks like. `SLOT_INSTANCES['msaOdd']`
   in sil_backplane.py instantiates the same module a second time with the
   nine slot-specific nets renamed (E->O strobes, CLK.ms0Odd', EcOut.0/1/4/5
   -> EcOut.2/3/6/7'); every configuration that carried `msa` now carries
   `msaOdd`, and tb_exec/tb_readback seed both boards. There is no .bp for
   the odd slot -- PARC states a board's pins once, for the slot it was drawn
   in -- so the rename table is the symmetry of the even one; the EcOut
   pairing (0/1/4/5 even, 2/3/6/7 odd) is the one assumption in it, and
   dSTPerr going quiet on the first boot with both boards is its test.

### What the missing Md hold did, measured (run21/22/24 in the scratchpad)

InitMap's page-zeroing loop (real 0o4000, `ITemp0_ (Store_ ITemp0)+1, DBuf_ T,
Branch[., Cnt#0&-1]`) wedged after ~5 pages with DbufBusy stuck at 1 and every
automaton idle: the page's LAST Store<- missed (144,132,955: Hit'b=1, dirty
victim), left A at 144,132,970, and NextMapEntry's `DummyRef_ T, T_ MD` ran
UNHELD at 144,133,033 (DdataGood'=0 "good", b23.3=1, b22.3=0 -- the tag half
did ask). The DummyRef displaced the store's flags; its line reached Ec2 at
144,133,450 with Store<-InEc2=0, so MakeD<-Dbuf (MemX j17 = VicSTPerr' &
Ec2State4 & Store<-InEc2) never fired and the DBuf was never handed over.
That is HM p.60 verbatim: "issuing Map<- without holding for a previous
Store<- to finish will result in infinite DBufBusy ... do not fail to issue
<-Md before or concurrent with Map<-" -- the microcode does issue it; the RTL
did not hold. Gate chain, for the next reader:

    DbufBusy     = MemC c17 FF b (clk0'): Store<- | (DbufBusy & ~NoRef & ~MakeD<-Dbuf & ~FastD<-Dbuf)
    FastD<-Dbuf  = MemC i22: Hit_a & SH_Bb & ~Dbusy & Store'InA & ~BLretry
    MakeD<-Dbuf  = MemX j17: VicSTPerr' & (Ec2State4 & Store<-InEc2)
    Dbusy        = ~AcanHaveD;  AcanHaveD = MemX g23 FF <- j23 f(MakeTransport0/1/2)
    MDhold'      = MemC e22 FF (PrClk1') <- dMDhold' = b23.3 | b22.3   (wired-OR, both low = hold)
    b22.3        = MDMtag' ^ ProcTag   (0 = the Md file's tag is stale; MemX f21 tag RAM vs e20 Ptag RAM)
    b23 (MC10121)= ~[DdataGood' & (At=Curt' | MemC16.sil+6 | <-MDI) & (<-MDI | DcomingForCt') & (<-MD | WantCR | <-MDI)]
    DdataGood'   = MemX e19 FF (Clk0'Cf) <- g24 = CacheRef | SW | DcomingForCt'
    HM glossary "tag": the extra bit in Md readout which complements for successive
    Fetch<-'es and Store<-'s by the same task; agreement with the current value = reference finished.
    MemX i19 writes the tag RAM on every MakeMDM<-D; MemD i12 blocks only the DATA write (DontWriteMDM).

### Where the boot stands

With the alias fixed the Md hold fires -- and the first thing it does is hold
Initial's TestSyndrome setup for ever: at real c6e (`Store<-T`, FF=300) the
store is held with MDhold'=0, STPerr=1, VicSTPerr'=0 (run25/26). STPerr is
the storage ECC error latch (MemX h08 <- i01 = dSTPerr & EnableSTPerr);
`dSTPerr` -- MemD's syndrome-nonzero flag -- has read 1 on 99.7% of samples
in EVERY run (run22: 28,214 of 28,310), but the latch only arms now that
STPerrNow' (j03 <- MemX05.sil+15) is live. With STPerr set, VicSTPerr' blocks
MakeD<-Dbuf AND MakeD<-CD, so a missed reference's data never enters the
cache, the reference never "finishes", the Md tag is never written, and the
hold is permanent. `+stperroff` (STPerr forced 0; in the recipe since
2026-09-01) is back in force for the boot runs; the ECC path itself is the
OPEN DEFECT: MemD sheets 3-5 (MC10163 byte parities -> MC10180 trees -> the
GenPh/ChkPh phases -> Syn0..7 -> dSTPerr), the msa's EcOut/EcIn check-bit
planes (shifted, `ShiftEcOut`/`LoadEcOut'`), and whether the corrector also
FLIPS data bits on a bad syndrome (it should not be able to, if the check
bits it stored are the ones it recomputes). Start with a probe that compares
GenP* on the write side with ChkP* on the read side of one zeroing store.

run29 (`+stperroff`, 2 G sys_clk, obj_bwdisk3) is the boot in flight at the
end of the session; its PROGRESS lines are in the scratchpad log. `+blfix`
is no longer used (run17: identical with and without it).

### The park moved EARLIER once the hold was alive, and both boards did not clear it

With the alias fixed the Md hold fires, and the boot now parks at Initial real
`0o6156` (CIA c6e, a `Store<-T`, in InitialSubrs before InitMap) rather than in
the zeroing loop -- the hold does its job, and the first store it holds never
releases. Measured (run25..run44):

- At c6e the Md hold is asserted (`MDhold'`=0) through `b22` = `MDMtag' ^
  ProcTag` = 0: the Md file's tag disagrees with the current tag, i.e. HM's
  "reference not finished." The reference does not finish because the store's
  data is never handed to the cache: `MakeD<-Dbuf` (miss path, MemX j17 =
  `VicSTPerr' & Ec2State4 & Store<-InEc2`) and `FastD<-Dbuf` (hit path) both
  stay 0 for it, so `DbufBusy` never clears and the tag RAM (MemX f21, written
  on `WriteMDM'` = `MakeMDM<-D`) is never updated.
- **Both storage boards do NOT clear it** (run30, no `+stperroff`): the park is
  identical. So it is not only the ECC syndrome. `+stperroff` (run29) does not
  clear it either.
- The tag chain is sound in isolation: PresetMap's kick-start (real d2c/d2e,
  `+tagbrief`/`+tagfrom`) shows references HIT, `ProcTag`/`Ptag`/`ProcTagInA`
  track, and the cache-address memory (MB7071H, `+cafrom`) stores and reads
  back VA correctly per column under FDMiss/UseMcrV. The one `WriteMDM'` in the
  whole 87 M-cycle boot is the power-up write at cycle 0.

So the open question is: **why does a store in the SETMCR region never
"finish" -- neither `MakeD<-Dbuf` (its miss reaches Ec2 without
`Store<-InEc2`) nor `FastD<-Dbuf` (a hit whose FastD path is gated off) fires,
so the Md tag never flips and the hold is permanent.** The corrector pipeline
(MemX l03: `Store<-InEc1'`/`CacheRefInEc1` <- MemC l03's pipe-flag F10016 <-
the pipe RAM MemC l04, written on `WrPipe02'c`) and the storage ECC (MemD
sheets 3-5, `dSTPerr` = syndrome nonzero, still 1 on 99.7% of samples even
with both boards) are the two suspects. `+tagfrom`/`+pipefrom`/`+cafrom`/
`+mdfrom` windows are in tb_exec for exactly this; the C emulator's
`DM_REF_STORE` (memory.c:1645) and its `last_ref_latency` (16/24 on a miss,
memory.c:1556) are the oracle for what "finished" should take.

### Oracle facts

C emulator (`dorado --eb worlds/aemu.eb --disk 0=../chm/diskpacks/games-trident.pack --boot-reason disk`,
run from `dorado/`): InitMap = Map1to1Loop 65,536 + CacheFlushLoop 65,536 +
MapInitLoop 16,384 (RealPages), 31.0 M -> 66.8 M BB cycles; display-list
pixels first appear between 130 M and 170 M BB (2104 at 170 M, 2092 at 450 M+).
170 M BB = 46 M microinstructions = 1.47 G sys_clk at 16x = ~2.5 h of
Verilator at 160 k sys_clk/s. The RTL (16K map, one module): Map1to1Loop
16,384 x ~1,030 sys_clk, CacheFlushLoop 16,384 x ~1,950, MapInitLoop 16,384
pages (ChipsAre256/16K=1 -> 256K chips -> 16,384 pages claimed; the msa
physically holds 4 banks x 16K x 36 bits -- how many pages that is remains
to be measured).

### How to run

    make -C verilog/verilator boot-world-disk WORLDARGS="+cycles=N +pack=chm/diskpacks/games-trident.pack +pendulum +hasram +ioreset +stperroff +fastwait +bbword=0000 +bbfeed +enterworld=23e"

or the binary directly (the Makefile captures stdout; the bench now prints
`PROGRESS @cycle CIA=xxx` every 1 M sys_clk and flushes). Windows:
`+seqfrom=CYC +seqlen=N` (map/storage automata + DbufBusy/hold/transport
signals on change), `+vafrom/+valen` (VA and map address per map op),
`+mdfrom=CYC +mdmax=N` (every <-Md with the hold terms), `+mdwin/+mdwinlen`
(HOLDWIN, per sys_clk). `scratchpad/seqdiff.py seq.txt lo hi [fields]`
prints a SEQ trace as field transitions; `prog.py log labels` maps PROGRESS
CIAs to InitMem labels. Verilator `--threads 4` was 2.3x SLOWER; `-O2` gave
nothing measurable; stay single-threaded.

### Gates

cell-check, loop-check, machine-test, memrun-test, readback-test,
refdecode-test, exec-test, exec-parity pass on the regenerated RTL.
**cell-check now PASSES and was RED at HEAD** (on MC10182): the complex-block
waiver (COMPLEX_PARTS in sil_check_cells.py -- MC10182, MC10179, MC10163,
MC1674, each output a data-sheet function of a SUBSET of the flat [G] input
union the dictionary states) clears the false positive while still catching a
cell that reads an input the union does not list. exec-tasking and exec-init
fail as they did at HEAD (red since commit d3e337a2, per CLAUDE.md -- they
wake tasks whose TPCs the bench never initialised). `make -C verilog
backplane` regenerates everything (now including the msaOdd storage board) and
exercises the net-merging alias guard.

## COLD RESTART (2026-09-08, later) -- history; superseded by the evening block above

### The Md-file blocker was never a timing defect: the RTL had NO ADDRESS ADDERS

Branch `verilog-backplane`, head `dc2f03ca` plus the uncommitted work listed in
the block below, plus this session's changes to `tools/sil_netlist.py`,
`tools/sil_to_verilog.py`, `tools/sil_gen_all.py`, `verilog/cells/cell_F100181.v`,
`verilog/cells/cell_F10470.v`, `verilog/verilator/tb_exec.sv` and the
regenerated `verilog/generated/*`.

**The measurement that reframed everything.** A per-sys_clk probe of the D bus
end to end (`MEMWIN5`, `+mdpc=1e2 +memwinlen=150`) at ABoot's clock wait showed
the cache data-array ADDRESS was `000` for BOTH the fetch of VM 430 and the
store to VM 420, and MemC's `dDad.02-13` was zero for the whole window. The
fetch and the store read and wrote the SAME array cell; the "store's word"
the Md file took was VM 420's old content read at the fetch's (wrong) address.
The Md file, its hold, `DontWriteMDM`, f06's capture and the clock-depth model
were all innocent: the machine had no virtual address.

**Root cause: a parser regex.** PARC's wire lists name some package positions
with a `#n` prefix -- `#1c42.5o`, `#5b42.4o`, `#-1c3.1o`, `#4i19.6o` -- and the
`.lc` part lists carry exactly those names. `sil_netlist.PIN_RE`
(`\b([a-z]+\d+)\.(\d+)([io])`) rejected every one of those pin references, so
the generator instantiated the packages (from the `.lc`) with NO PINS AT ALL:

| board | packages | what they are |
|---|---|---|
| MemC | `#1c42 #1c43 #1f42 #1i42 #5b42 #5b43 #5e42 #5h42` (8 x F100181) | the BR + Mar adders that make VA[4:31] -- and the carry-select pair for the cache row |
| MemD | `#-1c3 ... #-1l19` (36 x F10470) | the B halves of the cache data array (Dad.09 = 1) |
| BaseBd | `#4i19 #4i20 #4i21 #4j21` (4 x CA3140) | op-amps on the supply-margin DACs (analog) |

With the adders unwired, `dVA`/`dDad` never left zero: every reference in the
machine's life aliased to one cache line (which is why the hit compare read
97% and why "the Md file takes the store's word"), and half the cache did not
exist. Fixed by allowing the prefix in `PIN_RE` (and `PKG_RE`); `sil_netlist.py
--net 'MemC10.sil+4'` now lists `#1c42.5 out F100181`, and the regenerated
MemC wires all eight. `make -C verilog backplane` regenerates every board and
machine (24 generated files change).

**The guard.** `sil_to_verilog.py` now records every package emitted with no
pins, prints them in its report, and exits 3; `sil_gen_all.py` fails the
build on it. Mutation-tested: reverting the regex makes MemC report its eight
adders and the build fail. All sixteen boards report zero.

**The adder cell was wrong too, and had never been exercised.** With the
F100181s live, `cell_F100181` needed the data sheet: "A minus B plus Cn" is
A + ~B + Cn (the part inverts B internally; a two's-complement difference
needs Cn' LOW), not A - B + Cn -- one too many. MemC runs its slices in "B
minus A" with Mar' on A, so BR + ~(Mar') + 0 = BR + Mar, and the old cell gave
BR + Mar + 1. The group P'/G' outputs were not the data sheet's either, and
MemC chains slices through them: `#1c42`'s G' IS the carry into `#5b42`
(its own Cn is 0, so C4 = G), and the `#1c43`/`#5b43` pair is a CARRY-SELECT
adder for the cache row bits 20-23 -- `#1c43` with Cn' open (LOW, carry 1) and
`#5b43` with Cn' = Hib (carry 0), selected by `CO24&Add` through b07/c07's
NOR pairs. Cell rewritten from the data sheet's Function Table and lookahead
equations. `cell_F10470` gained pin 18 (VEE, wired on the bank-B positions).

### Where the boot stands now

With real addresses the machine no longer reaches ABoot: the disk task's first
`Store` (AWasntIdle, `Store_ KTemp3, DBuf_ T`, VM 522) map-faults, the fault
task takes `NotEmuFault` ("Fault in non-emulator task") and halts in its
`Branch[.], Breakpoint`. That is the microcode doing exactly what it says for
a map that was never initialised: the bench enters the world at
STARTEMULATOR (`+enterworld=25b`), which skips **InitMap** (`InitMapLoc`
0o1076 = IM 0x23e, `InitMem.mc`), the world's own map and storage
initialisation -- Map1to1, a full cache flush, GetMemConfig, the storage
zeroing, EmuBRHiReg. The bench's all-ones map seed stood in for it, and was
only ever exercised at entry 0 because every VA was 0. The C emulator enters
the world at 0o1076 (PCDIS: first executed at BB cycle 31,023,768) and its
world never runs the fault task at all.

### And with InitMap running, the second defect: the MCR's B side never loaded

Entering at 0x23e, InitMap runs `Call[TasksOff]`, `B_ FaultInfo'`,
`T_ mcr.noWake; LoadMCR[T, T]`, the TestSyndrome setup and its
`WaitForMapBuf`, then `TaskingOn` -- and the fault task immediately switches
in at Initial's leftover Block (IM 0xc06, "hist pc=c06" x34) and re-wakes for
ever, because its wakeup was never disabled: `WAKESRC ... WakeEnable
200000000 of 200000000`. HM Figure 10 / Table 4: Mcr[13:15] load from MapBuf,
bit 15 is NoWake; on MemX, f04 (F10016 on Clk1'Ab, PE' = Mcr<-' from MemC
l09) captures Mapbuf.15 as NoWakeups and g23 registers ~NoWakeups as
WakeEnable. `MCRWIN` (`+mcrpc=213`) showed the race at one sys_clk: MapBuf
(MemX g01/f01, MC10175 on MapbufLd') and f04's load edge are the SAME sys_clk,
and the MC10175 cell was a positive-edge flip-flop, so f04 read the OLD
MapBuf (0). memrun-test's SetMcr# jam holds the strobe for many clocks and
loads on a later edge, which is why the jam gates never saw it. The MECL data
book: "Any change on the data input will be reflected at the outputs while
the clock is low. The outputs are latched on the positive transition"; MemD01
says "175 follows input when clock is low". `cell_MC10175` is that latch now
(18 packages: MapBuf, DBuf, MemC's BR latches on SH', ProcH/ProcL i01/i02 on
Clock1'Ba, two on DskEth).

### And the third: the map arbitration gate that had been parked

With InitMap running (`+enterworld=23e`, latch fixed) the boot enters
Map1to1Loop and walks the map (6,679 entries by 102 M cycles, ~900 sys_clk
each). memrun-test, though, now FAILED where it used to pass: "preStartMem'
never asserted -- no storage cycle was started". A per-change trace of the
map and storage automata (`+seqwin`, printed by both the current tree and a
copy with the three RTL changes reverted) showed why the OLD machine passed:
with every VA zero, a Store HIT latched map function 01 and started a map
walk that happened to reach the storage-start state just as the flush's
dirty victim arrived. With real addresses the store latches 11 (no
function, correctly), the victim's own walk starts at MapState 000 and is
RESTARTED at 001 by the next reference's StartMap' -- because nothing denies
the address section the map while the map is busy. That is exactly gate c
of MemX f24 (`cell_MC1674`), `AcanhaveMap' = MapFree'`, parked at 0 on
2026-09-06 because "Initial deadlocks in its map initialisation" -- measured
on the machine with no adders. Live, memrun-test passes and its dirty-victim
write reaches DRAM for the first time (`MemRASa 12, MemCASa 6, MemWEa 2`,
`WriteInMem' asserted 256`). `+lenient` turns memrun's storage-cycle fatals
into reports so the diagnostic dump prints; `+seqwin` is the trace.

### Where the boot stands at the end of the session

With all three fixes the 400 M-cycle boot (`+enterworld=23e`) gets through
InitMap's preamble -- TasksOff, `B_ FaultInfo'`, LoadMCR (NoWake now takes:
`WakeEnable` high 106.05 M of 400 M, i.e. until that instruction),
TestSyndrome, `TaskingOn` (no spurious fault-task switch), GetMemConfig --
and into Map1to1Loop, where it writes 64 map entries (`MAPCENSUS` 64 zeros
in the WP, Dirty and RP00 planes) and then spins in WaitForMapBuf for the
rest of the run: `HoldMapBuf` 139 M samples, last TNIA 0x866, longest run
on one address 22. A refresh becomes due about every 45 k sys_clk and 64
iterations at ~900 sys_clk is 58 k, so the first refresh to arrive while a
`Map<-` is in flight is the suspect: with gate c live the map is now
genuinely arbitrated between the address section and refresh (MemX h20
`MapRfsh' = MapFree' | NeedRfsh'`, g24 adds the refresh-priority term to
`AcanhaveMap'`, j22 holds `MapFree'`), and one of them appears never to
release. `+seqfrom=CYC +seqlen=N` in tb_exec prints the map and storage
automata on every change (the run to look at opens at 106.33 M); run11 in
the scratchpad is the same boot WITHOUT `+stperroff +blfix`, to learn
whether those two no-adder-era workarounds still matter.

### Gate status (2026-09-08, evening)

Quick set, all PASS with the three fixes and gate c live: memrun-test,
compute-test, exec-test, exec-boot, boot0-test, sendmir, step-test,
display-test, taskrun-test, strap-test, readback-test, storage-test,
mem-test, exec-world, exec-parity, refdecode-test, task-test. Full suite:
see the line below this block for the final tally. Known pre-existing
failures, unchanged by this session: alu-test (cell_MC10181's data-sheet
check, 16 of 4880 vectors), exec-tasking, exec-init (fail at HEAD, see the
2026-09-03 block), the eight disk-* benches (bench-stimulus rot, commit
86b5be32), cell-check (MC10182 and MC1674 gate lists). storage-test fails
only from a stale obj dir -- `rm -rf verilog/verilator/obj_storage` first.

### How to run now

    make -C verilog/verilator boot-world-disk WORLDARGS="+cycles=N \
      +pack=chm/diskpacks/games-trident.pack +pendulum +hasram +ioreset \
      +stperroff +blfix +fastwait +bbword=0000 +bbfeed +enterworld=23e"

`+enterworld=23e` (InitMap) replaces the old `25b`. With gate c live Initial
itself takes 10 M cycles longer (its map loops now wait for the map, which
is what the note calling that a "deadlock" saw on the no-adder machine):
Initial reaches BootEmulator at 106.32 M, not 96.34 M, and the world enters
there. InitMap's Map1to1Loop then runs ~900 sys_clk per map entry. New probes
in tb_exec: `+mcrpc=HEX` (the MCR B-side chain at an address), `MEMWIN5`
under `+mdpc` (the D bus end to end). In tb_memrun: `+lenient` (storage
assertions report instead of stopping), `+seqwin` (map/storage automata on
every change). `+stperroff` and `+blfix` are bench workarounds diagnosed on
the machine with no adders; both are candidates for removal now, untested.

### Method notes worth keeping

- **A machine that "works" can have no address path.** Hit compare 97%,
  Initial booting, a raster painting, PARC's diagnostics passing -- none of
  it needed a correct VA, because every reference hit the one line. The
  first place memory CONTENT mattered for control flow (ABoot's clock wait)
  was where it showed. The guard in the generator is the lesson.
- **Re-measure every timing conclusion that predates the adders.** The Md
  hold, DontWriteMDM, f06's capture, PRECAP on the memory boards, the
  clock-depth pass, the parked MC1674 gate: all were tuned against a
  machine whose references aliased. Two of them (the gate, and the reason
  memrun "passed") were wrong in ways that looked like RTL timing.
- **The C emulator is still the oracle**, and cheap: `DORADO_PCDIS=lo,hi`
  (octal IM addresses) with `DORADO_PCDIS_LIMIT`, `DORADO_FAULT_TRACE=1`,
  `DORADO_MCR_TRACE=1` on `dorado --eb worlds/aemu.eb --disk 0=...
  --boot-reason disk --cycles N` (N in BaseBoard 6502 cycles, 3.71 per
  microinstruction). It settled that the world's entry is 0o1076, that its
  fault task never runs on a disk boot, and when ABoot/DiskBoot happen.
- **The reverted-tree A/B** (a lean copy under the scratchpad with the
  three files reverted, regenerated, memrun rebuilt by hand) took twenty
  minutes and answered a question no amount of reading would have.

## COLD RESTART (2026-09-08, morning) -- history; its blocker was a phantom (see above)

Branch `verilog-backplane`, head `dc2f03ca` plus UNCOMMITTED work in
`verilog/cells/{cell_MC10141,cell_MC10176,cell_MC10231,cell_SE10231,cell_F10145A,cell_MC1674}.v`,
`verilog/verilator/{tb_exec.sv,tb_memrun.sv,dorado_pack.sv,Makefile}`,
`tools/{sil_netlist.py,sil_to_verilog.py}`, and the regenerated
`verilog/generated/*`. Baseline (no clock depth) is what is checked in.

### State in one paragraph

The eleven-board RTL Dorado runs Initial, enters the AEmu world, drives a
Trident pack, paints its raster -- and does NOT boot. It reaches ABoot and
hangs there. ONE defect blocks it, and it has two faces that must be fixed
together: (a) at baseline, a `Store<-` between a `Fetch<-` and its `<-Md`
overwrites the fetching task's Md slot, so ABoot's clock wait reads the
store's word instead of the clock; (b) the derived-clock-depth pass, which is
the principled fix for (a)'s timing, makes the memory control register's load
strobe fire TWICE as often as the board's, and each spurious load sets the MCR
bit that forces every cache reference to miss -- so with depth on, the cache
never hits at all. Neither state boots. **ALL TEN GATES PASS at baseline**
(memrun-test included).

### The blocker, at the finest resolution reached

    baseline   the Md file takes the STORE's word (see "THE BLOCKER NOW" below)
    depth on   MCR load strobe: 1029 rising edges -> 2070 in the same 90 M boot
               ForceDirtyMiss (MCR bit, MemC k08 Q1, clocked by LdMcr')
                              0 -> stuck high 99.98% of samples
               cache hit compare        97% -> 0.02%
               => no dirty, no visible stores, every <-Md reads 0, ABoot hangs

The unanswered question, and THE PLACE TO START: are those extra edges
manufactured by the lag pipeline (which samples a combinational net at sys_clk
and can quantise a decode GLITCH into a full-width pulse), or do they already
exist upstream because that decode's inputs now move at inconsistent depths?
**Those need different fixes.** Answer it with `memrun-test` (a minute a run),
NOT a boot run (twenty). Two attempts to measure it failed on scaffolding, not
on the machine: excluding `LdMcr` from the depth tree removes the very
`__lead` net a probe needs, and an `ifdef` guard around that probe did not
take.

### What landed this session, all gated

- **The terminal.** `dorado_terminal` is wired into the bench (`+terminal`,
  default in the boot recipes). Without it the Alto keyboard words stay at
  Initial's zero, which is every key HELD, and ABoot took the BS key as down
  and chose the ETHERNET boot. This alone is why the disk was never tried.
- **The task-switch register file.** ProcH a21 (MC10141 on SHCP'C) sampled
  `Next` on the sys_clk it moved, so every T-file write-back at a switch was
  addressed to the INCOMING task's slot. The pre-edge capture `cell_MC10176`
  already had is now in `cell_MC10141`/`MC10231`/`SE10231`. The junk task went
  from dying after 7 wakeups to running continuously.
- **`cell_MC1674` was a SKELETON** -- one package, MemX f24, making
  `CacheRef`. Unmodelled it left `DdataGood'` permanently "not good", so the
  Md HOLD had never once fired in this project. Modelled from the datasheet
  (triple exclusive-NOR), it fires. Its gate c is parked at the skeleton value
  with its true expression beside it: enabling it deadlocks Initial's map init.
- **Smaller, each measured:** the pendulum is 16 us (IFU k08 halves it); the
  pack's head overflow is a latched status as `disk.c` has it; backplane E39
  (`DcomingForCt'`) joined by name; DskEth's SN74123 one-shots had no clock in
  the committed generated file.

### Mechanisms left in the generator (inert unless asked for)

- `DORADO_CLOCK_DEPTH_BOARDS=<list>` -- take the depth pass board by board.
- `DORADO_CLOCK_DEPTH_CAP=N` -- the BFS's stage cap (default 6).
- `PRECAP` parameters on `cell_MC10176`/`MC10141`/`MC10231`, so pre- vs
  post-edge capture can be chosen per package via `CELL_PARAMS`.
- `LdMcr` is in the depth skip-list beside `RunClk`/`CPStrb`/`TCPBus`, because
  a decoded strobe does not belong in a clock-distribution model. **It does
  not fix depth mode** -- its combinational sources are still lagged.

### Dead ends -- roughly twenty, do not repeat them

On the Md collision: delaying `FastD_u_Dbuf` (breaks the world before ABoot);
pre-edge on `MD_u_D`'s register, on the Md file's four data registers, and
removing the board capture rule entirely (all BYTE-IDENTICAL, so the bug is
independent of MC10176 capture mode). On depth: the cap at 1/2/3; every subset
of memory boards (MemD alone stalls the world at 25c; MemX alone breaks
stores; MemC+MemD leaves Md wrong); post-edge on MemC g17 (the hit FF) and
pre-edge on k08 (the MCR register) -- both verified as really emitted.

### Retractions from this session, all caught by measurement

1. **"The depth config FIXED ABoot's `<-Md`" -- FALSE.** With depth the cache
   never hits, so every `<-Md` returns 0, and ABoot's correct answer at the
   sampled instant IS 0 (VM 430 starts at zero). Right by coincidence.
2. **"The RTC never carries" -- FALSE.** `RTCCARRY` runs at exactly the rate
   `Junk.mc` predicts (1 carry per ~1237 wakeups). The claim came from
   grepping an `addresses executed:` line that was ABSENT from that log; zero
   matches on a missing line is not evidence of absence.
3. **"The per-task Md file address is stuck at 0" -- FALSE.** Probed during
   the junk task it tracks the task correctly (0/8/4/c for tasks 0/1/2/3, the
   familiar bit-reversed print).
4. **"The zero-delay cache path makes this structurally unfixable" -- too
   strong.** The cache address never moved in the window; the defect is a
   specific one-phase relationship, not an architectural impossibility.

### Gate status (2026-09-08, baseline)

**PASS (10/10):** memrun-test, compute-test, exec-test, exec-boot,
boot0-test, sendmir, step-test, display-test, taskrun-test, strap-test.
`memrun-test` passes again -- the 2026-09-05 block below saying it fails is
SUPERSEDED. Stale `obj_*` directories (holding the old repository path) still
masquerade as failures: `rm -rf` the directory before believing one.

### How to run

    make -C verilog/verilator boot-world-disk WORLDARGS="+cycles=N \
      +pack=chm/diskpacks/games-trident.pack +pendulum +hasram +ioreset \
      +stperroff +blfix +fastwait +bbword=0000 +bbfeed +frame"

For a long run invoke the binary directly with output to a file. World entry
is at ~96.3 M; ABoot's fetch (CIA 1e2) at ~96.44 M. Probes: `+pctask=N`,
`+pcpc=HEX`, `+mdpc=HEX` (opens MDWIN/MEMWIN/HOLDWIN at an address),
`+junklog`, `+ramwe`. `memrun-test` now carries DIRTYCHAIN / HITCHAIN /
MCRLOAD summaries -- use those, they are a minute a run.

## COLD RESTART (2026-09-05, evening) -- history

Branch `verilog-backplane`, head `dc2f03ca` plus UNCOMMITTED work in
`verilog/cells/{cell_MC10141,cell_MC10231,cell_SE10231,cell_F10145A}.v`,
`verilog/verilator/{tb_exec.sv,dorado_pack.sv,Makefile}`,
`tools/sil_netlist.py`, and the regenerated `verilog/generated/*` (MemX, DskEth,
the tops). Everything measured below is reproducible from those files.

### Where it stands in one paragraph

The morning block's diagnosis was wrong in its centre and right at its edges.
The disk boot never stalled in the disk controller: the world never ISSUED a
disk command, because AEm0.mc's ABoot chose the Ethernet boot -- and it did
that for two reasons that were fixed and measured today, and one that is
characterised to the gate and the manual's rule but not yet fixed. (1) The
bench had no terminal, so the Alto keyboard words stayed at Initial's zero,
which is every key HELD, and `ETemp0, Branch[EBoot, R even]` took the BS key
as down. (2) The junk task, which advances the real-time clock ABoot waits on,
died after seven wakeups because every T-file write-back at a task switch was
addressed to the INCOMING task's slot; a one-instruction slice left its T
clobbered and its `AckJunkTW<-T` carried a 0 into ShutUp. (3) Remaining: the
per-task Md file on MemD stores the word of a STORE that follows a fetch, so
ABoot's clock wait -- `Fetch; Store; <-Md; <-Md` -- reads 0 then 7401B and
exits on its first pass. The C emulator boots this pack to 2,104 pixels, so
the target is known-good.

### What was fixed, each with its measurement

- **The terminal.** `dorado_terminal` (the processor in the keyboard) now
  drives DispY's `KeyboardData` port in the bench (`+terminal`, passed by
  default by `boot-world` and `boot-world-disk`), one bit per horizontal
  blanking, all five words all-ones. KeyboardData EQUALS the terminal's
  OISData: BaseBd c06 straps OISData to the MC10125's true input and the
  MC10124 channel p11->p14 is non-inverting. Before: `DiskBoot first @-1`,
  16,272 EControl writes, EBOOT (0x406) in the address set and DISKBOOT
  (0x405) never; DispY's `OISRcvdData` never toggled. After: 1,898 line ticks
  delivered, `OISRcvdData` high 4.66 M of 104 M samples.
- **The task-switch chain (the junk-task death).** ProcH a21 (MC10141, D =
  `Next`, clock = `SHCP'C`) loaded the incoming task ONE CYCLE EARLY because
  `Next` (ContA h14, `Switcha ? BNT : CTask`) moves on the very sys_clk the
  SHCP'C edge is detected, and the cell sampled D one sys_clk after the edge.
  a20 (Last <- Curr, Clock1'Ca, 16 sys_clk later) then copied the NEW Curr, so
  during the write-back pulse (+23 of the following cycle, PreClock0) the
  T-file's CurrLast named the incoming task. Measured with the TFWIN window
  (task nets are MSB-first: `4` is task 2, `c` is task 3): task 0's final T
  written into task 2's slot at 96568096, task 2's `T_ RTCDeltaLo` (0xC2)
  written into task 3's slot at 96568128. Fix: the pre-edge capture
  `cell_MC10176` already had, in `cell_MC10141` (and, for uniformity,
  `cell_SE10231` and `cell_MC10231`). After: AckJunkTW strobes 7 -> 215 in the
  same 100 M, ShutUp never relapses, and the T ABoot computes reads 430B where
  it read 30B (task 0's own RM reads were being corrupted too).
- **The pendulum.** IFU k08 halves `Pendulum` (a toggle FF makes
  `TimeToWake`, and the wakeup FF clocks on THAT), so the bench's 32 us wave
  woke the junk task every 64 us (TWReq.02 rose every 34,144 sys_clk). It is
  16 us now, with `+pendulum_us=N` to reproduce older runs.
- **The pack's head overflow** is a LATCHED status: set by a head tag with
  head >= 5, cleared by a legal head tag or a Control tag carrying
  DeviceCheckReset (bit 3) or ReZero (bit 1), which is `disk.c`'s model and
  what `AltoDiabloDisk.mc` does after its head-5 probe ("Reset error
  status"). As a level it held ReadError up for every sample of every run.
  ReZero and HeadAdvance now move the pack's cylinder and head.
- **Backplane pin E39.** MemC receives `DcomingForCt'` there and MemX drives
  `EcDcomingForCt'` there; the generated machine left MemC's input as a cable
  port tied to 0. It is one of the seven inputs of MemC b23, the Md-hold
  gate. `BACKPLANE_BOARD_ALIASES` joins them (the ContA `SWb`/`SW` class;
  MemX's plain `DcomingForCt'` is board-local). It reads driven and
  deasserted now; it did not by itself produce the hold (below).
- **DskEth's subsector one-shots had no clock.** The committed
  `DskEth-Rev-Cf.v` predated `cell_SN74123` becoming clocked (2026-09-04), so
  a03 and a05 were instantiated without `.sys_clk` and Verilator tied it to
  0. Every boot run's DISK line read `SectorTW rises 1`. Regenerating with
  `make -C verilog backplane` added the two connections (FifoUnderflow 3154
  -> 0 in the next run). Regenerate before trusting any disk-side count.

### THE BLOCKER NOW: the per-task Md file after an intervening Store

Measured at ABoot's clock wait (run9/run11, `+mdpc=1e2` opens the windows;
cycle numbers are with all three cell captures in):

    96436297  1e2  ETemp0_ (Fetch_ T)-(10C)        fetch VM 430
    96436329  1e3  Store_ ETemp0, DBuf_ 0C         store VM 420
    96436361  1e9  ETemp0_ (3S)+MD                 MD_u_D=1 for +0..+15: the DIRECT
                                                   path delivers 0000 (correct)
    96436377       f06 (MemD MC10176 on clk1'Aa) re-registers the D bus: 0F01,
                   the store's word -- Dad (MemC e14, clk1'Da) moved at +16
    96436384       WriteMDM'a pulses (+23..+30, preClk0'BD, MDMad = task 0)
                   and stores 0F01 into task 0's slot (trailing-edge capture)
    96436393  1de  PD_ (ETemp0)-MD                 MD_u_D=0: from the file, 7401B
    96436425  1ed  Fetch_ T, Branch[.-1, ALU>=0]   3 - 7401B < 0: the loop exits

No other task runs between 1d7 and 1df (PCT window); the intruding word is
the STORE's. HM p.39: "Md remains valid until and during the next fetch by the
task. If a Store<- intervenes between the Fetch<- and its associated <-Md,
then <-Md will be held until the Store<- completes but will then deliver data
for the fetch exactly as though no Store<- had intervened." HM p.42: "An
immediate <-Md is held in the cycle after a fetch or store." The RTL holds
NEITHER: every run's HOLDSRC line has `MDhold 0`.

The hold request is `dMDhold' = b22 | b23` on MemC (both must be low). Measured
term by term at 1e9 (HOLDWIN): `<-MDI`=1 (the immediate decode reaches MemC
from ProcL), b22 = `MDMtag' XOR ProcTag` = 0, but b23 = 1 because
`DdataGood'` = 0 (asserted: "data good") -- b23 = ~[(DdataGood'|..) &
(At=Curt'|sil6|<-MDI) & (<-MDI|..|DcomingForCt') & (<-MD|WantCR|<-MDI)]. So
the hold hinges on `DdataGood'` staying asserted while the store occupies the
data section. `DdataGood'` is MemX e19 (MC10176, Clk0'Cf) <- g24 (MC10105:
CacheRef, SW, VictimInA', STfree'...); `MakeMDM_u_D'` (the file's write
enable, delivery-driven) is MemX g23 (MC10176, Clk0'Cb) <- i23 (MC10105:
AWordRefToD, EcWordRefToD, bHit', CacheRefInPair'...). Next probes, in
order: (a) a MEMWIN over MemX sheet 01 -- g24/i23's inputs, `Dbusy`,
`CacheRef`, `VictimInA'`, `STfree'`, `AWordRefToD` -- across 1e2..1de, to see
what the store does to `DdataGood'` and why the fetch's D window ends at t2
of the cycle after next instead of after the file write; (b) the store's data
phase: MemC e14's `Dad` moved at +16 of F+2, half a cycle after the fetch's
delivery began -- decide from the pipeline registers whether the store is
early or the file write late; (c) the F10145A captures its DATA at the
trailing edge of WE' (`+ramwe` now flags data moves too) -- a real 10145 needs
data held to the trailing edge, so this is only a fix if the board's own
timing says so. The C emulator is no oracle here (its Md is atomic,
per-task, latched at issue).

### Later the same night: the hold's root, and a skeleton on the map path

Three more things, in the order they were found, each measured.

- **The MC10176's pre-edge capture is right on the processor boards and wrong
  on the memory boards.** Reverting it globally makes memrun-test PASS
  (`preStartMem'` asserts; it had failed since d3e337a2) and stops Initial
  reaching BootEmulator (the ProcL d17 half-write that capture fixed). So
  `cell_MC10176` has a `PRECAP` parameter and the generator's new
  `BOARD_CELL_PARAMS` sets `PRECAP=0` for every MC10176 on MemC, MemD, MemX
  and msa (100 packages). With that: memrun-test green, Initial and the quick
  gates unchanged, `MakeMDM_u_D'` NOT moved -- so the Md file's write time is
  the netlist's own, and the store collision must be resolved the way HM
  p.39 says, by holding the <-Md.
- **The hold's root: `cell_MC1674` was a SKELETON, used in ONE package, MemX
  f24, whose gate a makes `CacheRef` from `CacheRef'`.** With it unmodelled
  `CacheRef` sat at 0 for ever; MemX g24 ORs it into e19's `DdataGood'`, so
  the memory never said "not good", MemC's `dMDhold'` never fired, and every
  run's HOLDSRC read `MDhold 0`. The part is a triple exclusive-NOR
  (DoradoDocs/datasheets/MC1674.pdf; EclDict `[G (3 5)>2, (6 13)>14,
  (7 11)>15]`, pin 5 unwired on f24 = an inverter). Modelled gate by gate
  against memrun-test: gates a and b pass; **gate c deadlocks Initial** --
  `AcanhaveMap'` = `~(MapFree' ^ TrueAC)` = `MapFree'` (TrueAC is g04.14's
  constant 1) makes the address section wait for a busy map, which is
  correct arbitration, and the map then never frees (145 addresses,
  TASKINITLOOP never reached, MiscHold 6.3 M -> 12.7 M; memrun's Store never
  dirties its line). MapFree' is j22 (F10016 as a register, PE' = MapWait)
  loaded from the i14 PROM: the map SEQUENCER not freeing under honoured
  arbitration is a latent defect the skeleton masked. Gate c is parked at the
  skeleton's 0 in the cell, with its true expression beside it, until that is
  run down; gates a and b are live.
- **`TrueAC` is fine** -- a first grep, cut short by `head -5`, said it had no
  driver; g04 pin 14 (MC10195, open inputs) makes it and the generated
  instance carries it. Retracted the same hour.

Also this half: pin E39 joined (`DcomingForCt'`) reads driven and deasserted
and by itself produced no hold; DskEth's a03/a05 one-shots have their clock
after regeneration (FifoUnderflow 3154 -> 0). `+mdpc=HEX` opens
MDWIN/MEMWIN/MEMWIN2/HOLDWIN at an address; memrun-test has a MAPFREE line.

**run16 (gates a+b live, gate c parked, memory boards PRECAP=0): THE HOLD
FIRES.** `DdataGood'` deasserts during the store, b23 goes low at the <-Md,
`dMDhold'` and `MDhold'` assert, `Hold` rises, and the <-Md at 1e9 repeats
one cycle -- HOLDSRC `MDhold 23707` where every run before read 0. The two
reads of the clock word now AGREE, and ABoot's wait loop ITERATES (71 passes
of 1de in the window; every earlier run made one). What both reads return
is still the STORE's line word (7401B), because the file was written at the
store's data phase: `DontWriteMDM` -- MemD h03, registered on clk0'B from
`bFastD_u_Dbuf | D_u_Dbufdly`, the "D comes from DBuf" flag that exists to
block exactly that write -- asserts one cycle AFTER the pulse. The wait
therefore runs from a wrong start value (7404B instead of 3).

**run17 (MEMWIN3): the flag is one clock edge late for this pulse.** MemC's
`FastD_u_Dbuf` (i22, an MC1660, combinational from the store's A-stage hit,
the `SH'Bb` phase and `MemC18.sil+4`) rises at 96436377 -- t2 of the store's
A stage, SEVEN sys_clk before the pulse at 384 -- but MemD h03 registers it
into `DontWriteMDM` on the clk0'B edge at 392, EIGHT sys_clk after the pulse
has gone. `D_u_Dbufdly` (c21, a clk2 delay of `D_u_Dbuf`) is later still.

**The shape this leaves, and the first probe for the next session.** The
RTL admits a reference into MemC's A stage at the END of its issue cycle
(`CacheRefInA'` falls at t0 of the next cycle; it is combinational at MemC
l20 from `MemC17.sil+11` and `EcHasAa`, so the admitting register is one
step back on sheet 17), where HM's own table has the CAM lookup at t1 and the
hit at t3 of the ISSUE cycle. The cache-data path (F10470s, then the
correction mux) is modelled with zero delay, so D still arrives at t2 of the
cycle after the fetch, as the manual has it -- but every control flag the A
stage produces (`MakeMDM_u_D'`, `MD_u_D`, `DontWriteMDM`, the store's Dad)
runs a cycle behind that data. That is exactly why the fetch's file write
lands after the store has taken D and why DontWriteMDM misses it. Probe
first: the sheet-17 register that makes `MemC17.sil+11`, its clock, and
whether it is a same-edge capture of the MIR-derived `WantProcRef'`
(the MC10231/MC10176 capture question, third instance). If the A stage moves
to the issue cycle, MakeMDM_u_D' asserts at t0 of the cycle after the fetch,
the file write lands at t3 of that cycle with D still the fetch's word, and
the store's FastD flag has a full edge to reach DontWriteMDM.

Second probe, if the first is not it: HM p.39's "will then deliver data for
the fetch exactly as though no Store<- had intervened" -- the held <-Md may
be REDELIVERED from the fetch's pipe entry rather than read from the file.
The MDMtag'/ProcTag compare (b22) and MemX's `AcanHaveD` are where that
would live.

### RETRACTED AND ROOT-CAUSED: THE DEPTH MODEL FORCES EVERY CACHE MISS (2026-09-07, evening)

**RETRACTION FIRST.** Earlier today this document said the three-board depth
config "FIXED" ABoot's `<-Md` (`alu=007404` -> `000003`). **That was an
ARTIFACT and the claim is withdrawn.** Under derived depth the cache never
hits, so every `<-Md` returns 0 -- and at the instant sampled, ABoot's
correct answer happens to BE 0 (VM 430 starts at zero, so `3 - 0 = 3`). A
read that is right by coincidence looked like a fix. The Md-file collision
described further down is still real and still unfixed.

**THE ROOT CAUSE, measured with a one-minute test instead of twenty-minute
boots.** Diffing `memrun-test` signal by signal between baseline and depth:

    signal          baseline   depth
    Store              707       707     (unchanged -- the store still runs)
    ForceDirtyMiss       0      3000     STUCK HIGH, every sample
    ForceMiss          832      3000
    hit compare       2168         0     the cache NEVER hits
    Hit'b              832      3000     (deasserted all run)
    Col0'/Col1'    480/480       0/0
    Dirty3             928         0
    HitColDirty        111         0

Every symptom is one signal. `ForceDirtyMiss` feeds `ForceMiss`, and
`ForceMiss` is an input to ALL FOUR column-hit terms (MemC f16/f17, two
MC1660s: each column's hit is
`NOR(ForceMiss, dMissN_16-19, dMissN_12-15, dMissN_04-11)`). Stuck high, no
column can ever report a hit -- so nothing is dirtied, no store is visible,
and every `<-Md` reads nothing.

**And `ForceDirtyMiss` is not a per-cycle signal at all -- it is a BIT OF THE
MEMORY CONTROL REGISTER.** It is Q1 of MemC k08, an MC10176 whose clock pin
is **`LdMcr'`**, the MCR load strobe, with D = `RMar_01`. So the machine is
running with a mis-loaded MCR, and the bit that says "force a dirty miss" is
set.

**Why depth does it.** `LdMcr'` becomes a LAGGED net under the depth pass, and
the generator initialises lagged clock nets to their idle level
(`reg LdMcr_p___lag1 = 1'b1;` -- "primed clock nets idle HIGH"). At baseline
`LdMcr'` is a plain combinational net that starts low and rises, so k08
captures once at startup and gets a benign value. Initialised high, that
startup capture moves or never happens, and the register keeps the wrong bit.

**It is NOT a bench artifact.** `memrun-test` never does a real `LoadMcr`, so
the first suspicion was that only the bench suffers. Measured in the REAL
boot under depth (`tb_exec`, 100 M cycles, where Initial genuinely loads the
MCR): `ForceDirtyMiss` high on **99,994,965 of 100,015,252** samples and the
hit compare essentially never asserting. The boot is broken the same way.

**Also negative, so nobody retries them:** post-edge capture on MemC g17 (the
hit flip-flop) and pre-edge on k08 itself both leave `ForceDirtyMiss` at
3000/3000 -- verified the parameters really were emitted. `cell_MC10231` and
`cell_MC10141` now carry `PRECAP` parameters from those attempts; both
default to today's behaviour and are harmless.

**THE MECHANISM, measured in the REAL BOOT (90 M cycles, both configs).**
`LdMcr'` rising edges -- k08 clocks on the trailing edge of the active-low
strobe -- and the data bit each load captures:

    baseline   1029 rising edges, RMar_01 set at 1025;  hit-compare 97%
    depth      2070 rising edges, RMar_01 set at 2070;  hit-compare 0.02%

**The strobe count DOUBLES.** The lag pipeline manufactures load edges the
board does not have, and every spurious load writes the force-miss bit. That
is a different and more serious fault than a phase being off: it is edges
being created.

**Excluding `LdMcr` from the depth tree is NOT sufficient, and that is
measured too.** It is now in the skip list beside `RunClk`/`CPStrb`/`TCPBus`
(a decoded strobe does not belong in a clock-distribution model), and
`memrun-test` STILL fails under depth -- because `LdMcr'` is produced by
combinational logic whose own inputs remain lagged, so its timing moves even
when the net itself does not. The next attempt must deal with the strobe's
SOURCE logic, not just the net.

**So the depth pass cannot be enabled until its lagged-net INITIALISATION is
right for strobes.** `LdMcr'` is a load strobe, not a free-running clock, and
idle-high init is wrong for it. That -- not the cap, not per-board subsets,
not any register's capture mode -- is the next thing to fix. All three were
swept and are dead.

### Superseded first reading of the same experiments
### PER-BOARD CLOCK DEPTH: THE FULL MATRIX, AND THE CONFLICT IT EXPOSES (2026-09-07)

`DORADO_CLOCK_DEPTH_BOARDS=<list>` was added to `sil_to_verilog.py` so the
depth pass can be taken board by board instead of wholesale. Every
combination was measured. **No configuration boots**, and the reason is a
genuine conflict, stated at the end.

| depth on | memrun | ABoot's <-Md | the world |
|---|---|---|---|
| none (baseline) | PASS | WRONG (alu=007404) | reaches ABoot, waits for ever |
| MemD | PASS | -- | STALLS at 25c, 425 addresses |
| MemX | FAIL | -- | -- |
| MemC,MemD | FAIL | still wrong; stores dead | reaches ABoot (582 addrs) |
| MemC,MemD,MemX | FAIL | **FIXED (alu=000003)** | reaches ABoot (583), stores dead |

**THE CONFLICT: MemX's derived phases are REQUIRED for the Md fix and BREAK
the store path.** MemC,MemD leaves the Md read wrong; adding MemX fixes it
(007404 -> 000003, the correct value, and MDhold drops to 0 because there is
no longer a collision to hold for). But every configuration including MemX --
or MemC -- fails `memrun-test` on
`HitColDirty never true -- the Store did not dirty the flushed line`.

**And that store breakage is what actually blocks the boot, which took a
while to see.** ABoot waits for VM 430 to advance by 3. The junk task DOES
run and DOES carry: `RTCCARRY` (real 7ee) executes at the correct rate (1531
junk-loop passes, 1 carry, i.e. one per ~1237 wakeups, exactly as Junk.mc's
RTCDeltaHi=65B predicts). But its `Store<-` to VM 430 never becomes visible:
sampling `Md` 4,000 times late in a 150 M run gives **0000 every time**, and
task 0's Md slot stays 0. The wait therefore computes 3-0 for ever. The same
store breakage memrun reports as one clean line is what silently stops the
boot.

So the two halves are, as implemented, mutually exclusive: MemX's depth is
needed to stop a Store corrupting a deferred fetch's Md, and it
simultaneously stops Stores dirtying their cache line. Both cannot be right;
MemX's depth values are partly correct and partly not. That is the
refinement the next session needs -- NOT merely enabling the pass, which is
what the older comment implied.

**FURTHER NEGATIVES, so nobody repeats them.** Two more knobs were swept
against the store breakage and BOTH are dead:

- **The depth CAP is not the problem.** `CLOCK_DEPTH_CAP` is now overridable
  (`DORADO_CLOCK_DEPTH_CAP=N`). At N=1 MemX gets 0 lagged nets and memrun
  fails differently (`preStartMem' never asserted`); at N=2 (51 nets) and N=3
  (222 nets) it fails on `HitColDirty` exactly as at the default 6. So the
  derived depths being "too large" does not explain it.
- **It is not the Dirty register's capture mode.** MemC i10, the MC10141
  holding the cache Dirty flags, is the register memrun names. `cell_MC10141`
  now takes a `PRECAP` parameter so the choice can be per package (the
  pre-edge default still fixes ProcH a21's task switch). Setting i10 to
  post-edge under depth does NOT fix it -- verified the parameter really was
  emitted (`cell_MC10141 #(.PRECAP(0)) u_i10`).

**So the store failure under derived depth is robust**: not the cap, not any
single register's capture mode, not which subset of memory boards. The
productive next step is a signal-by-signal diff of the store/dirty path
between baseline and depth on `memrun-test` ALONE -- it runs in about a
minute, where a boot run is twenty -- rather than any further configuration
sweep. Sixteen experiments across this blocker are recorded here; the space
of cheap knobs is exhausted.

**Two retractions from today's own work, both caught by measurement.**
"The RTC never carries" was WRONG: it came from grepping an
`addresses executed:` line that was ABSENT from that log (the run ended
before printing it), and a zero match on a missing line is not evidence of
absence -- the "a level is not an event" trap in a new dress. And "the
per-task Md file address is stuck at 0" was WRONG: probed during the junk
task it tracks the task correctly (0/8/4/c for tasks 0/1/2/3, the familiar
bit-reversed print), so Md addressing is sound.

**Reverted to the baseline (clk1-only); all gates pass.** The
`DORADO_CLOCK_DEPTH_BOARDS` hook is left in the generator: it costs nothing
unset and is how the next attempt should be driven.

### Superseded first reading of the same experiment


`DORADO_CLOCK_DEPTH_BOARDS=MemC,MemD,MemX` -- the depth pass applied to the
MEMORY boards only. ABoot's `<-Md` after an intervening Store now returns the
FETCH's word:

    at 1de   alu=007404 (the store's word)  ->  alu=000003, the correct value
    MDhold   23,707 samples                 ->  0 (no collision left to hold)
    AckJunkTW  160                          ->  23,872 (the RTC is advancing)
    IM addrs   541                          ->  571

and ABoot now sits in its real timing loop (1de/1ed) waiting for VM 430 to
advance by 3, i.e. ~119 ms of Dorado time, which is what the microcode says.

**Why per-board is the right granularity, and this is the whole trick.**
Enabling depth EVERYWHERE breaks the startup jam -- ContA goes from 0 lagged
nets to 77, its clk0/clk1/clk2 distribution moves under the phase ring that
drives the step chain, and `sendmir`/`step-test`/`boot0-test`/`compute-test`/
`exec-test` all fail ("CPRegToLink# did not put the address in Link", "the jam
step did not run one clk0 window"). But NO jam gate exercises the memory
boards' internal phases, and it is exactly those that the memory pipeline
needs right. Taking the boards separately gets both: ContA lagged 0, MemD
lagged 171, and **every one of those five gates passes** while the memory
timing is derived from PARC's own buffer chains instead of a flat zero.

This is the re-derivation the old comment called "a session of work", done
board by board rather than wholesale.

**Open, and now isolated: `memrun-test`.** It fails under memory-board depth
with `HitColDirty never true -- the Store did not dirty the flushed line`.
That is one specific assertion in a bench whose stimulus this document has
already flagged as rotted in places, and it is the only gate that regressed.
It must be run down before this is called finished -- either the bench's
store/flush stimulus needs re-deriving against the corrected phases, or the
dirty path has a real defect the flat phasing was masking.

### The DiskBoot attempt: two hypotheses eliminated, the chain traced (2026-09-06)

The Md-file-after-Store blocker was attacked directly. Both hypotheses are
DEAD, and the measurements narrow the next search sharply.

**The precise defect, at full resolution** (run17, `+memwinlen`, MEMWIN3, the
window 96436355-96436400 around the deferred fetch's Md write):

    361-376   MD_u_D=1   the FETCH's data is selected on the D bus
                          ...but WriteMDM'a=1 the whole time: NO write pulse
    377       MD_u_D=0   deselected; the store's bFastD_u_Dbuf rises the same
                          sys_clk
    384-391   WriteMDM'a=0   THE WRITE PULSE -- and it captures the STORE's
                          word, because MD_u_D fell seven sys_clk earlier
    392       clk0'B rises
    393       DontWriteMDM=1  the flag built to block that write, one clk0'B
                          edge too late; there is NO clk0'B edge between 377
                          and 384, so no capture mode on h03 can help

So the write strobe (preClk0'BD, t3) and the data selection (MD_u_D, falling
at t2) are half a clock phase apart, and the only write pulse in the
instruction lands in the store's data phase.

**Hypothesis 1, DEAD and HARMFUL: delay the store's D-usage.** Delaying
`FastD_u_Dbuf` one CLK phase (a 16-stage shift at the backplane assign in
both `dorado_full.v` and `dorado_mem.v`) keeps **memrun-test PASSING** but
stalls the world long before ABoot -- 386 distinct addresses against 541,
parked 5,091 cycles at TNIA=592, LOADHRAMLOOP never reached, the junk task
dead (TWReq.02 0, AckJunkTW 0). That net is load-bearing in the display/HRam
path. Reverted.

**Hypothesis 2, DEAD and INERT: pre-edge capture on MD_u_D's register.**
`MD_u_D` is MemD f24 (MC10176, clk2'Cf, Q5). Forcing `PRECAP=1` on that one
package through the generator's `CELL_PARAMS` hook produced a waveform
**byte-identical** to the run before it -- MD_u_D still falls at 377,
`alu=007404` unchanged at 1de. memrun stayed green and the boot was
unaffected. Reverted (an inert change whose comment claimed a fix would be
worse than none).

**Hypothesis 3, DEAD and INERT: pre-edge capture on the Md file's DATA
registers.** The file's data pins were traced pin by pin to exactly four
MC10176s -- MemD f06, f10, f14, f18, twenty bits. `PRECAP=1` on all four
(via `CELL_PARAMS`) left the boot window byte-identical again: the data pins
still turn over at 378, `alu=007404` unchanged, memrun still green. Reverted.

**Hypothesis 4, DEAD: the capture mode is irrelevant ENTIRELY.** Removing the
board rule so every memory-board MC10176 goes back to pre-edge (the state
before this session) leaves the boot window **byte-identical yet again** --
`alu=007404`, the same waveform, the same wrong Md -- while memrun fails as
it did this morning. So the Md-file corruption is INDEPENDENT of MC10176
capture mode. Two useful consequences: the whole capture-mode family of
hypotheses is dead, and `BOARD_CELL_PARAMS` (PRECAP=0 on the memory boards)
is SAFE -- it fixes memrun and provably does not touch the boot path.

**THE DEFECT, STATED PRECISELY.** From the same window, watching the Md
file's own data pins rather than any register:

    378-408   the file's data pins carry the STORE's word (din h15 = 1111)
    ~391-395  the write strobe COMMITS -- capturing that word
    410       the FETCH's word finally appears on those pins (din h15 = 0000)

**The Md-file write strobe fires roughly one clock phase BEFORE the fetch's
data arrives at the file.** Equivalently, the returning cache data is one
phase late relative to `preClk0'BD`. This is consistent with
`DdataGood'` being deasserted ("data NOT good") across the whole pulse and
only falling at 393. It is a single, quantified, one-phase relationship
between the write strobe and the cache-data arrival -- NOT a per-cell bug
(four experiments) and not the vaguer "zero-delay path" framing below.

**AND THE MEASUREMENT LANDS IT (MEMWIN4, run22).** Probing the cache array's
own address and write enable across the strobe:

    384-391   WriteMDM'a  asserted -- the Md-file write strobe
    385-391   WriteD0'a   asserted -- THE CACHE ARRAY'S OWN WRITE ENABLE,
                          i.e. the STORE writing the array
    355-401   Dad_a       UNCHANGED the whole time (the store's location)
    410       Dad_a       finally switches to the fetch's location

**The store's cache-array write and the fetch's Md-file write are
SIMULTANEOUS, at the SAME array address.** The Md file samples the F10470
outputs in the very window the store is writing them, so it captures the
store's word; the fetch's location is not presented until 410, long after
the strobe. Two operations the hardware must sequence are sharing one phase,
both gated off preClk0.

That is the whole bug, stated in one line, and it explains every earlier
null: no register capture mode matters when the array beneath them is being
overwritten at the sampling instant.

**THE TWO STROBES COME FROM DIFFERENT CLOCKS, which is the design's own
separation -- and the RTL loses it.** Traced to the source:

    WriteD0'a  (cache array write) = ~MemD03.sil+15, from e08 (SE10211),
               and MemD03.sil+15 is the Q of g22, an SE10231 flip-flop
               clocked by **SpecialClk2**
    WriteMDM'a (Md file write)     = ~(DontWriteMDM | preClk0'BD | MakeMDM')
               from i12 (SE10210) -- gated by **preClk0'BD**

Different clock nets, i.e. different phases on the real board. The
generator's `clock_lag` gives a relative delay to the **clk1 family ONLY**
(`_CLOCK_LAG_RE`); `SpecialClk2` and `preClk0'BD` both get zero, so their
separation collapses and the two windows overlap (385-391 against 384-391).

**And the deeper reading, which matters more than the overlap.** HM p.24 and
p.39 put these in DIFFERENT CYCLES, not merely different phases: a store's
"DBuf is passed to the cache data section during the next cycle" (store at
1e3 -> array write during 1e9, which is what the RTL does and is CORRECT),
while the fetch's Md is ready "at t3 of the instruction following the fetch"
(fetch at 1e2 -> Md delivery during 1e3). In the RTL `MakeMDM_u_D'` does not
assert until 361, the start of 1e9 -- **the fetch's Md-file delivery is one
CYCLE late**, which is what puts it in the same cycle as the store's
correctly-timed array write. `AWordRefToD` is already high from 355 (during
1e3), so the lateness enters at MemX g23, the MC10176 on Clk0'Cb that
registers i23's decode.

So the cache data section is servicing two operations in one cycle that it
must serialise, and the root is the late Md delivery, not the store.

**THE CHAIN, END TO END (measured from the fetch itself, run20 MEMWIN2).**

    1e2 297-328  the fetch.  AWordRefToD=0, CacheRefInA'=1 -- nothing in the
                 A stage yet
    1e3 329-360  AWordRefToD RISES AT 329 (one sys_clk after the clk0 edge at
                 328), CacheRefInA' falls the same sys_clk.  MakeMDM' still 1
    1e9 361-392  MakeMDM' asserts at 361 -- ONE FULL CYCLE after AWordRefToD,
                 because g23 (MemX, MC10176 on Clk0'Cb) missed the 328 edge
                 and caught the 360 one
       377-391   the store's bFastD_u_Dbuf: its cache-data-section slot
       384-391   the Md-file write strobe -- COLLIDING with it
       393       DontWriteMDM finally asserts, one cycle too late to block
       410       Dad switches to the FETCH's location; its data appears --
                 but DontWriteMDM (393-424) now BLOCKS the correct write

Note the two-register delay from AWordRefToD to MakeMDM' is CORRECT
flip-flop behaviour (two registers in series on one clock each cost a
cycle), so that is not the defect. Nor is the store's slot: HM p.40 says "on
a hit, DBuf is passed to the cache data section during the next cycle",
i.e. store at 1e3 -> D usage during 1e9, exactly what happens.

**THE DEFECT IS `DontWriteMDM`'s WINDOW, AND IT IS A HALF-CYCLE PHASE
ERROR.** `DontWriteMDM` is h03's Q4/Q5 (MemD, MC10176 on clk0'B) registering
`bFastD_u_Dbuf` -- the schematic labels it in words, "Don't write MDM". To
block the store from clobbering the fetch's Md slot it must be HIGH across
the store's D usage (377-391) and LOW again by the re-delivery (410). But
clk0'B's edges are at 360 and 392, so a clk0-registered flip-flop can only
produce the window 361-392 or 393-424 -- never 377-391. It currently
produces 393-424: too late to block the store, and then wrongly blocking the
fetch's re-delivery.

For the correct window (361-392) `bFastD_u_Dbuf` must be high AT the 360
edge. It rises at 377 instead, and it is
`NOR(Hit'a, SH'Bb, MemC18.sil+4, BLretry)` with Hit'a and BLretry both 0
here -- so its rise is set entirely by **`SH'Bb` falling at 377, the
MIDDLE of the cycle rather than its boundary.** `SH'Bb` descends from the
backplane's `MemSH`. A signal that gates a clk0-registered flag must settle
before the clk0 edge; this one moves half a cycle after it.

**So the fix is the phase of the SH family on MemC/MemD, not any cell's
logic.** That is a single, named, testable quantity: `MemSH`/`SH'Bb` should
resolve at the cycle boundary (360) rather than mid-cycle (377). It was NOT
hand-tuned here, deliberately: advancing a clock net by a constant is
exactly the hand-patching this generator's `clock_lag` comment warns
against, and a periodic net cannot be advanced by delaying it (every
candidate delay -- 14, 15, 16, 30 -- lands on 391, 392, 393 or 407, none of
them at or before 360). It needs the buffer-depth derivation applied to the
SH family, or the MemD/MemC sheets read for what sets MemSH's phase.

**THE DEPTH MODEL WAS TRIED, AND IT BREAKS EARLIER THAN THIS BUG.** The
generator already contains the principled fix -- a buffer-depth BFS behind
`DORADO_CLOCK_DEPTH=1`, whose own comment says the foundation is right.
Enabling it regenerates cleanly (MemC goes from 8 lagged nets to 225) and
then fails BEFORE the memory even matters:

    memrun-test   Fatal: send_via_mir: CPRegToLink# did not put the address
                  in Link          -- the control-processor bus load
    boot smoke    Fatal: Initial's entry never executed

i.e. it breaks the STARTUP JAM sequence, exactly as the comment predicted
("boot0/compute/exec-test fail ... landing it green is a session of
re-derivation"). Reverted. Anyone attempting that re-derivation should start
from those two assertions, which are the earliest failures and therefore the
cheapest to iterate on -- both reproduce in seconds, no boot run required.

**AND A CAUTION ABOUT THE PHASE ORDERING, which looks CORRECT as built.**
Measured: `prepreclk'd` edges at 377, `preClk0'BD` strobes at 384, `clk0'B`
edges at 392. That ordering -- prepre before pre before plain -- is exactly
what the names promise, so the phase ring is NOT obviously wrong, and the
one-cycle lateness cannot be blamed on it without more evidence. Given that
ordering, a flag registered on clk0 (392) can NEVER be high at the preClk0
strobe (384) of the same cycle; it is always a cycle behind. So for the
hardware to work, `bFastD_u_Dbuf` must be a PREDICTION -- high during the
store's OWN instruction (1e3), the memory knowing at store-issue time that
DBuf goes to the cache next cycle -- rather than the report of the current
cycle it currently is (rising at 377, inside 1e9). THAT is the sharpest
open question: should `FastD_u_Dbuf` assert in the store's own cycle? It is
`NOR(Hit'a, SH'Bb, MemC18.sil+4, BLretry)`, and the store's hit is known in
1e3, so the term holding it off is `SH'Bb`. Read MemC's sheet for whether
SH is meant to qualify the prediction or the report.

**Why no fix was applied.** The honest candidates all require inventing
logic the netlist does not have (gating `WriteMDM'` with `WriteD0'`, or with
`DdataGood'` -- neither term is in those gates) or changing the relative
phase of two clock nets, which is the depth model and must not be done by
one hand-tuned constant. The next session should decide between: (a) giving
`clock_lag` a real per-family depth so `SpecialClk2` and `preClk0'BD` sit
where the buffer chains put them, and (b) finding why `MakeMDM_u_D'` is a
cycle later than HM's t3-of-the-following-instruction. (b) is the smaller
question and is where I would start.

**Superseded candidate, NOT justified.** Gating `WriteMDM'` with `WriteD0'`
(never write the Md file while the cache array is being written) would do
it, but that gating is not in the netlist and this document's own rule is
that such a term must come from the sheets, not be invented. Find instead
why the two share a phase: both derive from preClk0 on MemD, and the sheets
must separate the cache-data-section write from the Md-file write. Read
MemD's sheets 01/07/08 (the Md file and its strobe) against the cache
write-enable generation before adding any gate.

**Superseded next probe:** what sets the arrival time of the
corrected cache data at MemD's f06/f10/f14/f18 inputs -- the ECC/correction
chain (`D_u_CD`, `preDin`, MemD b04's MC10158, the ChkPh ring) -- and
whether `preClk0'BD` or that chain carries the wrong phase. Compare the two
against `DdataGood'`, which is the memory's own statement of when the data
is valid and currently disagrees with the strobe by one phase.

**A superseded framing, kept because its structural observation is true.** Each failed
attempt has the same signature -- the value was already wrong BEFORE the
register's clock edge, so no capture mode matters. The reason is structural,
and needs no further measurement: the D bus is
`assign D_04 = D_04__a13_1 | D_04__d13_1;` -- a wired-OR taken straight off
two F10470 cache arrays, with NO register anywhere between the cache array
and the Md file's data registers. The moment the store's cache write
commits, D turns over with zero delay, and every downstream register
faithfully samples the new value. **The Md-file corruption is therefore not
a cell bug and cannot be fixed by any per-package capture mode or per-net
lag; it is the zero-delay cache-data path meeting clocked control.** The fix
is the depth/phase model this document has named as the open area from the
start, applied to the cache data path -- or an explicit hold on the Md
file's data across the store, which would have to be justified from the
sheets rather than invented.

**What hypothesis 2's null result PROVES, and it is the useful part.** If
pre-edge capture changes nothing, f24's D input was ALREADY 0 before the
clk2 edge -- so the deselect is NOT caused by the store's `bFastD_u_Dbuf`
rising at 377, even though the two coincide. f24's D5 is `MemD03_sil_pl_13`
from **MemD g23**, an MC10106 whose EclDict grouping is
`[G (4 5 6 7)>3, (9 10 11)>2, (12 13 14)>15]`, i.e. pin 3 =
NOR(`MakeMD_u_D'`, `D_u_Dbufdly`, `bFastD_u_Dbuf`, `FHD`). Three of those
four are quiet in the window (`D_u_Dbufdly` 0 throughout, `bFastD` rises only
at 377, `FHD` unmeasured), which leaves **`MakeMD_u_D'`** -- a MemX signal,
the twin of `MakeMDM_u_D'` -- as the thing that deselects the fetch's data
early. THAT is the next probe, not the store side.

Note `MakeMD_u_D'` (MD) and `MakeMDM_u_D'` (MDM, the file write enable) are
DIFFERENT nets from different MemX packages; earlier notes that conflate
them are wrong. Both come from MemX MC10176s now running PRECAP=0.

### Retractions, each by measurement

- "RTClock carries on every wakeup" (from an instruction count): wrong.
  RTClock climbs by 65B per tick and RTCCarry is never taken -- the
  81-instruction total was JunkTaskStart + the first wakeup's UpdateCounters.
- "The F10145A's address moves during WE' low": measured on all 405
  packages, never. The address was STABLE and WRONG (Last = incoming task).
- "l02 passes StartCycle through, shifting pClock1En'": the NBA ordering
  already gave the old value; the SE10231 capture alone changed nothing
  (run5 = run1 to the cycle). Kept for uniformity.
- The MC10231 capture (152 packages) moves BootEmulator 66,272 cycles later
  and adds one instruction to each task's TASKINIT (26 -> 27; the oracle runs
  34 in-window, its c02/c0a wait loops iterating 7 and 9 times where the RTL
  passes once). Not judged either way; all gates pass with it.

### How to run

    make -C verilog/verilator boot-world-disk WORLDARGS="+cycles=N +pack=chm/diskpacks/games-trident.pack +pendulum +hasram +ioreset +stperroff +blfix +fastwait +bbword=0000 +bbfeed +frame"

`+terminal` is in the recipe now. For any long run invoke the binary directly
with output to a file. World entry is at 96,336,265 (with the MC10231
capture; 96,269,993 without); ABoot's fetch (1e2) is at 96,436,297. The
probes: `+junklog` (every ShutUp/AckJunkTW/TWReq.02 event), `+pctask=N` (a
PCT line per MIR load of task N), `+pcpc=HEX` (open the PCT window at an
address), `+mdpc=HEX` (open MDWIN/MEMWIN/HOLDWIN at an address, lengths
`+mdwinlen`/`+memwinlen`), `+tfwin/+clkwin` (cycle-numbered), `+ramwe`
(F10145A address/data moves during WE', per package). Hierarchical per-sys_clk
probes cost about half the simulation rate (96 M in 8-9 min with them). Three
things the trace columns mean: PCT's `T` at instruction N is T after N-2 (an
instruction's T load and T-file write land at +17/+23 of the NEXT cycle);
task-number nets print MSB-first; an F10145A file's slot index is `~task`
bitwise (task 0 = mem[15], task 2 = mem[13], task 3 = mem[12]).

### Gate status (2026-09-05) -- SUPERSEDED, see the 2026-09-08 block at the top

PASS (20): taskrun-test, task-test, compute-test, exec-test, boot0-test,
step-test, exec-parity, exec-boot, mir-diff, operand-test, writeim-test,
sendmir, exec-world, exec-world9, refdecode-test, display-test, strap-test,
eth-ctl-test, crc-test, im-parity-check. FAIL (1): memrun-test
(`preStartMem' never asserted`, failing since before this session).
Stale `obj_*` directories (the old repository path) still masquerade as
failures -- `rm -rf` the directory first.

## COLD RESTART (2026-09-05, morning) -- history; its "blocker" was misread

Branch `verilog-backplane`, head `a9169ea9`. Everything below in the older
blocks is history; this section is the live state.

### The goal and where it stands in one paragraph

The generated RTL Dorado (eleven boards) runs PARC's Initial, hands over to
the Alto emulator world (AEmu.mb), and the world sets up its own display and
drives a Trident pack on the disk cable. It does NOT yet boot: the Alto's
disk boot STALLS in initialization -- the disk controller never goes Active
and never reaches its read command -- so the screen stays blank. The task
is to get the disk boot past init so the boot file is read and the Alto
screen paints. A C emulator boots this exact pack to a painted screen
(2,104 display pixels), so the target and the pack are known-good.

### How to run it (all commands from the repo root)

- Focused disk gates (seconds each): `make -C verilog/verilator pack-test`
  (drive+pack serve one sector, PASS), `trident-test` (drive turns, PASS),
  `disk-ram-test` (format-RAM counter walks 0->15, the RTL is sound).
- The world running with a pack, the real thing (LONG):
  `make -C verilog/verilator boot-world-disk WORLDARGS="+cycles=N +pack=chm/diskpacks/games-trident.pack +pendulum +hasram +ioreset +stperroff +blfix +fastwait +bbword=0000 +bbfeed +frame"`.
  It preloads AEmu+Initial+Bootstrap into IM, runs Initial, and at
  BootEmulator redirects into the world (`+enterworld`, replacing the boot
  transport). ~206,000 sys_clk/s; the world enters at ~96 M sys_clk.
- The C-emulator ORACLE (fast, ~1x): from `dorado/`,
  `DORADO_DISK_SEQ=1 ./build/dorado --eb worlds/aemu.eb --disk 0=../chm/diskpacks/games-trident.pack --boot-reason disk --cycles 400000000`.
  It reaches the first disk read at microinstruction 76.6 M and paints by
  108 M. Use its `[diskseq]` tag/sector trace as the target for what the
  RTL controller should do.

### THE BLOCKER, precisely (start here)

The disk boot stalls in init. From the 230 M-cycle run
(`$S/bwd5.log`-equivalent; regenerate by running boot-world-disk at
`+cycles=230000000 +addrs`), the DISK summary reads:

    Active high 0            <- the controller NEVER runs a transfer
    ReadError high 230015252 <- asserted EVERY sample (of 230015252)
    ReadDataErr 3155, FifoUnderflow 3154   <- rare, so NOT the cause
    DiskBoot first @-1       <- the DiskBoot command (AEmu 0x405) never reached
    PACK: drive at cyl 0 head 5   <- the drive is stuck at HEAD 5

The chain: AEmu's `InitRamDiablo` (AltoDiabloDisk.mc) selects head 5 to
tell a T-80 from a 19-head AMS-315 by reading `muffHeadOvfl`. `dorado_pack`
asserts head-overflow for head >= 5 (via `TtlEndOfCyl'` -> DskEth b24 ->
HeadOvfl), and HeadOvfl feeds `ReadError` (DskEth c23). The drive stays at
head 5 after the probe, so HeadOvfl -- and thus ReadError -- is stuck high,
the disk task loops in select forever (drive/head tags once per revolution,
seen in the DISKTAG log), and Active never asserts.

**The likely fix is one of:** (a) the world sends a head-4 tag for the boot
(partition 5 -> head 4) but `dorado_pack`'s head latch does not update, so
the pack stays at head 5 -- check the HeadTag decode in `dorado_pack.sv`
against a DISKTAG trace; or (b) the head-overflow indication on a real
T-80 is momentary (the drive reports it once, the world clears it and
proceeds), whereas `dorado_pack` drives it permanently-combinationally
from `head >= HEADS` -- it may need to be a cleared/latched status, not a
live level. Compare against the C emulator's disk.c head/overflow handling
(`DORADO_DISK_SEQ` trace shows the expected head sequence). This is fast to
iterate: the stall is reached by ~110 M sys_clk (~9 min), and the DISKTAG /
DISK probes in tb_exec.sv already report it.

### TWO PROCESS FIXES NEEDED BEFORE THE NEXT LONG RUN

1. **The Makefile captures stdout in `$(...)`, which overflowed shell memory
   on a 3.6 B-cycle run (gigabytes of debug) and LOST the whole summary.**
   For any long run, invoke the binary DIRECTLY with output to a file and
   the flooding flags OFF: no `+packdbg`, no `+addrs`; e.g.
   `./verilog/verilator/obj_bwdisk/tb_bwdisk +preload=... +cycles=N +pack=... +pendulum ... +frame > run.log 2>&1`.
   The bench already prints a 10%-interval progress line; that streams to
   the file so a killed run still shows how far it got.
2. **Do not launch multi-hour blind runs to find a stall that shows by
   110 M cycles.** Reach the stall in ~9 min, read the DISK/DISKTAG lines,
   fix, repeat. Only run to a screen (~2.5-3.5 B sys_clk, ~4 h) once the
   controller actually goes Active.

### WHAT IS SOLID (do not re-litigate)

- The RTL disk/memory READ logic is sound; the failing tb_disk-family and
  readback gates are BENCH-STIMULUS rot (jam-per-word, unset cache tags),
  proven by `disk-ram-test` (counter walks) and by reading the cells. Do
  not chase them as RTL bugs.
- `dorado_pack.sv` + `dorado_trident.v` serve games-trident.pack correctly
  (pack-test/trident-test PASS): sector framing, Fire-code check words, the
  index line, tag decode (DriveTag 0x8000 / CylinderTag 0x4000 / HeadTag
  0x2000 / ControlTag 0x1000, bus = low 12 bits).
- Real fixes already in on the disk path: `cell_SN74123` (subsector one-shot),
  `cell_SN74LS169` (setup-window sampling), `cell_p_8T98` (released output
  reads high), `cell_F9401` (CRC-16), the drive's `TtlIndex'` line, the
  head-overflow report, and `+pendulum` (the 32 us tick that no bench drove;
  without it the world's real-time clock never advances and ABoot waits
  forever). The run counter is 64-bit now (a boot is > 2^31 sys_clk).
- RETRACTED this session: the clk1-family lag is NOT the read-bench cause
  (tested with DORADO_NO_LAG); and my earlier "cache-fill mux takes the
  wrong source" was reading the bench's clobber write, not a fill defect.

### The run-length wall (real, secondary to the stall)

Even once the stall is fixed, a boot to a painted screen is ~2.5-3.5 B
sys_clk = ~4-5 h at 206 K/s. `--threads 8 -O2` is 5x SLOWER; `-O3` gains 9%.
No cheap lever. Budget overnight runs; fix the stall on short runs first.


## SESSION HANDOFF (2026-09-02) -- read this first; the 2026-09-01 block below is history

Branch `verilog-backplane`, picked up cold from the 2026-09-01 handoff, whose
"one remaining piece", the phase/depth model, turned out not to exist. Every
symptom that block attributed to phase collapse was measured again with new
per-sys_clk probes and each had an ordinary cause: one cell modelled two pins
of the ALU wrong, one backplane line was spelled two ways, and one BaseBoard
line the board-level bench never drove. The C emulator stayed the oracle
throughout (DORADO_PCDIS with a 6104,6141 window, and DORADO_TASK_TRACE).

### What the RTL does now

- **Initial's task-init loop terminates**, exactly as the oracle does: T
  counts 1..15, c46's ALU forms RM[1]+T = T-15, c47 branches on its sign,
  and at T=15 the loop exits to c44 (TaskingOn) and BLESSBASEBOARD.
- **A task switch fetches the new task's instruction.** Tasks 15 down to 3
  each run TASKINIT (c61 c23 c29 c05 c10 c0f c1b c00 c02 ...) for 832
  sys_clk and block, the oracle's sequence address for address.
- **The junk task is no longer woken from power-up**, and task 0 resumes at
  SETMCR (c00) after the tasks block.
- FULL-BOOT RESULT: **Initial runs to the end of its own initialisation.** All fifteen tasks
  run TASKINIT once and block (occupancy t1..t15 = 832 sys_clk each), task 0
  resumes at SETMCR, BOOTEMULATOR (c92) and INITHRAM (e20) are reached for
  the first time, the display HRam init (e00-e7c) runs, the 64K storage-
  init loop at c96 (0o6226, `T<-T+1, Store<-T`; the oracle runs it 65,536
  times too, so the bench's "stuck: 65537 on one address" is a false alarm
  for a counted loop) completes, and the Ethernet boot code (ec0-eff,
  BOOTTRANSFER f00) runs and lands at BOOTTRANSFERTIMEOUT (f1e, 0o7436) --
  correct, because the board-level `dorado_world` has no DskEth board. 379
  distinct IM addresses in 140M sys_clk (~24 min). NEXT: a world with
  DskEth and a bench that plays the C emulator's boot server
  (`dorado/src/ethernet.c`, EFTP) over the modelled cable, and a stuck
  heuristic that recognises a counted loop.

- **THE MACHINE PAINTS ITS OWN RASTER FROM ITS OWN HRAM** (later the same
  day, ten boards): `docs/images/rtl-initial-raster-2026-09-02.png` -- a
  656-pixel-wide, 496-line visible field, black because Initial installs no
  display list, on the bench's no-data grey. INITHRAM's table landed in the
  HRam EXACTLY (`+hramdump`: 34x6, 10x2, 145x0, 35x1, 148x0, 7x2, 1x6 = 380
  entries, the DisplayDefs.mc Alto waveform), ReleaseRam handed the RAM to
  the display's sync counter (DoradoHasHRam fell at 98,388,922 and stayed
  down), 3,356 HSync edges, 58 fields, and the display task ran its
  per-scan-line loop (task 3: 991,520 sys_clk, 6,733 task switches). That
  took a FOURTH defect of the spelled-two-ways class (below).

    make -C verilog/verilator exec-bootscreen CYCLES=112000000 \
      SCREENARGS="+ioreset +stperroff +blfix +addrs +nostuck +hramdump"

Deepest boot command (add `+ioreset` to last session's):

    make -C verilog/verilator exec-boot \
      EXECARGS="+fastwait +bbword=0000 +bbfeed +mb0 +ioreset +stperroff +blfix +addrs +cycles=140000000"

### Eleven boards, and the Ethernet output task wakes (later, 2026-09-02)

- **`dorado_full` = the ten-board screen machine + DskEth** (`make -C
  verilog backplane` emits it; `make -C verilog/verilator exec-bootfull`
  runs Initial on it with the Trident and Ethernet cables tied idle). It
  boots exactly as the ten-board machine and reaches Initial's Ethernet boot:
  four `Output<-` strobes to EControl (TurnOffRx/Tx, TurnOnRx/Tx), both
  Ethernet tasks take their init PCs and run 20 addresses each. **RETRACTED
  the same evening: the first two "eleven-board" runs had NO DskEth.** The
  Makefile defines SCREEN and FULL together and tb_exec's instantiation was
  an `ifdef SCREEN ... elsif FULL` chain, so the ten-board screen machine
  was built both times; "XmtData' one transition" was an undriven bench
  wire and "TWReq.06/07 zero" had no driver. Byte-identical task occupancies
  across a strap change were the tell. FULL now precedes SCREEN.
- **The cause was a STRAP, the same class as the TIOA and task straps in
  `strap-test`.** Found with a ONE-MINUTE bench instead of the 35-minute
  boot: `tb_disk +slowio +eth` (rule `eth-ctl-test`) aims the existing
  TIOA/Output loop at EControl 016B with EtherDefs.mc's `TurnOnTx` =
  `TxCmdEnbl(007777B) + STxOn(040000B)` = 4FFF. The write lands
  (`EthCtrl_IOB'` selects with 4FFF on IOB, j06 makes `TxCtrlClk'` from
  PreClock1' + that select + bIOB.00 = TxCmdEnbl') and j04 latches TxOn --
  but h05, the wakeup NOR (NoWakeups, TxOff, the Ether06 wired-OR of
  TxCntDwn/TxBusRegFull/Blocked-FF/TxEOP, Curr=EthTx, Next=EthTx?), stayed
  low on its task-identity terms: **the board decoded its Ethernet tasks as
  0 and 1**. a24 inverts Next.0-3, j23 XORs the top three against SIP j52's
  legs 6/7/8 (`TskAd.0-2`), h23 NORs that with Next.3, so Tx = `{~TskAd,0}`
  and Rx = `{~TskAd,1}` and the strap holds the COMPLEMENT of the task's top
  three bits. Uncut (111) that is tasks 0/1 -- the emulator task, so the
  wakeup was suppressed whenever task 0 was current or next, i.e. nearly
  always. `DskEth.pdf` p.35: "Cut SIP legs at j52 to set the Task numbers
  for the Ethernet. Standard tasks are 6 & 7. Task numbers differ only in
  the low order bit. Input is higher priority." 6/7 = 011x -> TskAd = 100 ->
  legs 7,8 cut (`sil_to_verilog.py`). Agrees with `ethernet.h` (EOT 06, EIT
  07) and the TWReq.06/07 jumpers; `strap-test` asserts it now.
- **With it, the chain runs end to end** (`eth-ctl-test +ioreset +ethlog`,
  sample numbers): TxOn latches at 136761 and the wakeup term rises the SAME
  sample; j24 (MC10176, Clock0') registers it onto TWReq.06 at 136777; the
  scheduler names task 6 as Next at 136809 (which drops the term, as
  designed); **CTask = 6 at 136841** -- a task switch driven by the DskEth
  board from a register write.
- **h04 is a "has blocked" flip-flop, SET by IOReset.** Its D is `Blocked`,
  its clock the transmit task's own Prev phase, and its Q' blocks the
  wakeup: a task that ran and has not blocked since cannot be woken again,
  and at power-up only IOReset arms it. The bench released IOReset on one of
  its five run paths; it releases it before every run now. A bench that
  never asserts IOReset sees the wakeup blocked forever -- the boot run
  (`+ioreset`) was never in that state, so this was not its blocker.
- **The CRC generator was a SKELETON.** DskEth g17 (transmit) and j05
  (receive) are Fairchild 9401s behind MC10125/MC10124 level translators,
  and `cell_F9401` said "TODO: model this part" -- so the transmitter would
  append sixteen zeros where the CRC belongs and the checker could never
  flag an error. Modelled from `DoradoDocs/datasheets/F9401.pdf` Figure 1:
  a 16-stage register clocked on CP's high-to-low edge, feedback = CWE AND
  (D XOR Q15) into stages 0, 2 and 15 for select 000 = CRC-16
  (x^16+x^15+x^2+1, both packages ground S0-S2), Q = stage 15, ER = any
  stage set, MR clears, P' presets. Gate `crc-test`: 0xFEE8 for "123456789"
  (the CRC-16/BUYPASS check value), ER low after message + check word, ER
  high with one bit flipped. A bench responder must produce this CRC, MSB
  first, zero initial register.
- **EtherClk is 23.5 MHz** (HM 11, one eighth of the 340 ns bit cell); the
  K1115A at DskEth j20 ran at the cell's 20 MHz default. `CELL_PARAMS`.
- `+xmtrec=PATH` in tb_exec records every `XmtData'` transition with its
  sys_clk index, for decoding the sent packet offline.

### The eleven-board machine, for real this time (2026-09-03)

Two bench bugs stood between "eleven boards" and a machine, and both were
SILENT. Neither was in the RTL.

- **The instantiation is an `ifdef` CHAIN with a SHARED TAIL.** `tb_exec`
  opens `dorado_full m (`, `dorado_screen m (` or `dorado_world m (` in the
  three arms of one `ifdef`, and the ports common to all three come after
  the `endif`. The Makefile defines SCREEN and FULL together, so with SCREEN
  first every "eleven-board" run built the TEN-board machine (retracted
  above); and once FULL was put first, the ports that live only in the
  SCREEN arm -- `CLK_display'`, `PixelClkVCO`/`RawPixelClk`, and the video
  the bench samples -- were no longer connected. **Verilator ties an
  unconnected input to 0 and only warns**, so DispY sat at its power-up
  levels and every display counter read as a plausible zero.
- **How it was found, and three wrong answers eliminated first.** The
  symptom looked like "adding DskEth breaks the display": on eleven boards
  `HRamCommand'` was asserted for the whole run and `ClkHRamAddr'` had zero
  edges, where ten boards gave 58 and 188,450. The processor was IDENTICAL
  -- both machines executed the SAME 120 IM addresses from Initial's entry
  -- so it had to be something DispY sees. A static diff of the two tops
  says DispY's inputs differ in exactly two places, `DMuxData` and
  `IOB.00-15`, and counting DskEth's contribution to each killed both:
  **IOB 0 on every one of the 18 bits, DMuxData 1 sample of 3,015,252**.
  IOAtten, the third candidate, is driven by ProcH (7,736,779 samples) and
  by DskEth on 0. Only then did counting DispY's own clock -- 47,350
  `clk1'Cc` edges on ten boards, **0** on eleven, with `CLK_display'`
  94,703 and 0 while `mclk` toggled at the bench -- name the port. With it
  connected the two configurations agree exactly, DISPY line for DISPY line.
  A `final` block now says so out loud when the display board is in the
  machine and its clock never toggles.
- **DskEth's MUFFLER strap was missing too**, found while chasing DMuxData.
  `DskEth.pdf` sheet 5: "Cut SIP legs at k52 to set the Muffler addresses
  for the board. Standard addresses are 2000-2177." k52 pins 5-8 are
  `MufAd.1-4`, k24 (an MC10166) compares them against `DMadr.01-04`, and
  the net k24 drives is NAMED **`MidasEn.01T.02F.03F.04F'`** -- the strap
  value written out, bit 1 true and bits 2-4 false. 2000B is the ninth of
  sixteen ranges, i.e. the top four bits of an 11-bit muffler address are
  1000, the same encoding as e41 (a cut leg is 0). Legs 6,7,8 cut. It was
  NOT the display bug -- DMuxData is quiet either way -- but it is a real
  strap and the sheet states its answer.
- **With a disk controller present, Initial tries the DISK first**, which is
  `InitialMain.mc` verbatim: "No key down. First attempt disk microcode
  boot." The eleven-board run therefore visits `BOOTTRANSFER`,
  `SETDRIVEANDSUBSECTOR`, `BOOTTRANSFERLP` and lands on
  `BOOTTRANSFERTIMEOUT`, which `DiskBootTransfer.mc` sends to
  `BootDiskError` -> `BootEOF` -> `BootTransferDone` with carry=1, i.e. the
  fall-through to `DoEtherMicrocodeBoot`. PARC's own way to skip the disk
  is the BOOT PARAMETER: STK[2] = `BootParameterSeal` (056623B) with
  STK[1]+STK[2]+STK[3] = 0 makes STK[1] the boot file number and branches
  straight to the Ethernet boot.
- **A boot server for the RTL exists now.** `tools/eb_replies.py` builds the
  MicrocodeBootReply stream for a `.eb` -- `append_reply`'s exact packet
  layout from `dorado/src/ethernet.c`, which `InitialEther.mc`'s EIEnd
  filters on -- phase-encodes it at the 340 ns bit cell and appends the
  9401's CRC-16; `tb_exec +rcvplay=PATH` replays it onto DskEth's `RcvData`
  once the machine's own transmitter has sent a packet and gone idle.
  38 packets for `aemu.eb`, 61 ms of wire, and every packet round-trips
  through `tools/decode_xmt.py` with a zero CRC residue.
  `+xmtrec=PATH` records the machine's own transmit line for decoding.
  For comparison, the C emulator's request is 13 words:
  `000042 001000 000026 000264 000001 000110 000000 000000 000004 000042
  000001 000001 177777` (`DORADO_ETH_TX_TRACE=1`).

### THE ELEVEN-BOARD MACHINE REACHES ITS ETHERNET BOOT AND WAKES THE OUTPUT TASK

The deepest the RTL has run: 453 distinct IM addresses in 120 M sys_clk,
Initial's own raster painting throughout (107 fields, HRam loaded, HSync
running), and **the Ethernet output task alive** -- `TWReq.06` asserted,
task 6 executing 28 addresses, and one `Output<-` reaching EData (015B).
That wakeup is the j52 strap fix arriving in a real boot: before it, task 6
ran its 15 init addresses and blocked forever.

**Where it stops, precisely -- and the trace names the sample.** With
ETHLOG2 (a change-only log of the wakeup chain, armed by the first EControl
write) the whole sequence is legible, and it is `InitialEther.mc` word for
word:

```
98405082  EControl <- f0ff  (task 0)   ResetEther: TurnOffRx  (RxCmdEnbl)
98405146  EControl <- 0fff  (task 0)               TurnOffTx  (TxCmdEnbl)
   ...    tasks 7 and 6 run their init PCs and block  (EITInitPC/EOTInitPC)
98406426  EControl <- f6ff  (task 0)   TurnOnRx  (RxCmdEnbl+SRxOn+SRxBOP')
98406490  EControl <- 4fff  (task 0)   TurnOnTx  (TxCmdEnbl+STxOn)
98406521  TxOn=1 RxOn=1 wakeD=1        the EOT wakeup term rises
98406537  TWReq.06=1                   j24 registers it onto the backplane
98406601  ctask=6                      the machine switches to the output task
98406874  EData <- 0119     (task 6)   the FIRST word of the boot request
98406906  l06=1                        the bus register takes it (TxBusRegFull)
98406921  ctask=0                      the emulator task resumes
98406937  TxOn=0 RxOn=0                *** both cleared, with NO control write
```

**Those are the only five `Output<-` strobes in the whole 120 M-cycle run**
(four EControl, one EData), so the last line is not the microcode turning
the transmitter off: something CLEARS the Ethernet control register one
microinstruction after the first data word. Both flip-flops take
`bIOReset` on their reset pins (j04 p4 for TxOn, l04 p13 for RxOn) and both
take their data from bIOB through the j06 control-clock latches
(`TxCtrlClk'`/`RxCtrlClk'`, gated by `EthCtrl_IOB'` = `bIOout'` AND
`TIOA=EthCtrl'`), so the next probe is exactly those five signals plus the
IOB value across sample 98406937: a reset pulse and a spurious clock are
distinguishable in one run. `bIOReset` reads 0 in the logged vector at that
sample, which argues for the clock, but the vector is change-triggered and
that is the one thing worth re-measuring directly.

**One candidate already eliminated, in one minute.** `eth-ctl-test
+ethdata` aims the same loop at EData (015B) with the word the boot sends:
960 strobes give `TIOA=EthCtrl'` low on **0** samples and `TxCtrlClk'`
falling **0** times, so the register decode is exclusive and an EData write
does not clock the control register. The reset path is out too --
DskEth j22 drives `bIOReset` through an MC10102 pin whose gate is the
**OR** (`p9 = p12 | p13`, both `IOReset`), so `bIOReset` equals `IOReset`
and reads 0 all run, leaving the MC10231 reset pins idle.

**Which leaves a stale-bus clock, and the log cannot see it.** After the
EData write `InitialEther.mc`'s EOT task does `TIOA[EControl]` with no
`Output<-`; if `bIOout'` is still asserted from the previous strobe when
TIOA changes, `EthCtrl_IOB'` asserts and the control register latches
whatever bIOB still holds -- and `0119` has both command-enable bits clear
with both ON bits clear, i.e. exactly "turn the transmitter and receiver
off". ETHLOG2 would MISS that write, because it triggers on the RISING
edge of IOBout and there is no new edge. **The measurement that settles
it: count `TxCtrlClk'` falling edges over the boot and compare with the
four EControl `Output<-` strobes.** In `eth-ctl-test` the two match
exactly (30 and 30); a fifth fall in the boot names the mechanism. This is
the same "the address must survive to the data" hazard the disk gates test
in the other direction.

**A second observation for whoever takes this on:** the word sent is
`0119`, where the C emulator's request begins `000042` = dest host 0,
source host 042. Ours is dest 01, source 031 octal, with `Host_0..7` tied
to 0 at the bench -- so either the host-address readback (`LHost_ Input`,
the left byte of EControl's status) or the packet buffer in VM is not what
Initial thinks it is.

**The old paragraph, kept because its measurements still hold.**

**Where it stops, precisely.** The transmitter never drives the wire:
`XmtData'` makes 2 transitions (its power-up pair) and exactly ONE data
word is written. That is self-consistent with the microcode --
`InitialEther.mc`'s EOT task writes a word and blocks, and HM 11 says the
next EOT wakeup comes "when the bus register is empty", which only happens
once the transmitter has shifted the word out. So the question is why the
transmit sequencer does not start. Measured on the one-minute bench
(`eth-ctl-test`): the Ethernet clocks RUN (`EtherClk340` 1,550 edges,
`EtherClk170` 3,101 over 263 us), `PDCarrier` is 0 (the line is properly
idle), `TxFifoEmpty` is high, and `TxGo` is 0 -- correct there, because that
bench never writes EData. The next measurement is the same chain in the
boot, where one word HAS been written: `TxGo`, `TxSRLoad`, `TxStart`,
`TxFifoEmpty` and the `TxState` counter, which `tb_exec`'s ETHLOG2 block
already logs.

**The reply side is ready and waiting.** `+rcvplay` now loads (266,254
transitions of `aemu.eb`); its first run silently loaded ZERO because the
reader was `$fgets` + `$sscanf` with a hand-rolled comment test. It is an
`$fscanf` loop now and `tools/eb_replies.py` writes pure data with its
packet index in a `.idx` sidecar. Gate: `make -C verilog/verilator
ebreplies-check`.

### Gate status, re-run in full on 2026-09-03

**PASS (28):** alu-diff, compute-test, exec-test, exec-parity, exec-boot,
exec-world, exec-world9, task-test, taskrun-test, refdecode-test,
im-parity-check, mir-diff, mirreg-diff, cpreg-diff, boot0-test,
datapath-test, machine-test, converge-test, loop-check, lint, prom-test,
osc-test, baseboard-test, firmware-probe, mem-test, operand-test,
run-test, sendmir, startseq, step-test, writeim-test, strap-test,
muffler-test, and the three new ones -- crc-test, eth-ctl-test,
ebreplies-check.

**FAIL, and also failing at 85fe8ed5 (this session's starting commit,
checked by checking that commit's file out and re-running):** exec-init
and exec-tasking (the fault task does not run its handler -- bisected in
the previous session to d3e337a2, the MC10176 setup capture), memrun-test
(`preStartMem'` never asserts), readback-test (the seeded cache word never
reaches dMD), and the tb_disk family.

**ELEVEN OBJECT DIRECTORIES WERE STALE**, each holding a generated
`.mk` that names the repository's OLD path
(`~/Documents/development/Dorado`), so eight gates -- baseboard-test,
mirreg-diff, firmware-probe, mem-test, operand-test, run-test, sendmir,
startseq, step-test, writeim-test -- failed with "No rule to make target
.../Vtb_*__pch.h" and NOTHING to do with the RTL. `rm -rf` the
`verilog/verilator/obj_*` directory and re-run before believing any
failure. Find them all with
`grep -rl Documents/development verilog/verilator/obj_*`.

### THE DISK/MEMORY READ RTL IS SOUND; THE BENCHES BIT-ROTTED (2026-09-04)

Ran the read path down and it is NOT broken. The tb_disk-family and
readback failures are BENCH STIMULUS artifacts, not RTL defects:

- **The format-RAM address counter WORKS.** `disk-ram-test` (the CYCLING
  loop) shows `RamCl'C` pulsing 30 times, the address walking to 1111 (15),
  and `LastRamAddr'` asserting on 150 samples. So b21 (F10016) counts
  correctly. `disk-format-test`'s "RamAddr stuck at 2" is purely its
  `+ram16` "park after one pass" jam producing ONE write across sixteen
  jams (`RamCl'C edges 1`); real microcode writes sixteen consecutive
  Output cycles in one run, as `disk-ram-test` does.
- **The cache write cell is correct** (`cell_F10470`), and `readback-test`'s
  failure is the muddled stimulus its own header documents (tags unset ->
  always miss -> fill overwrites the cache seed; the loop's Store clobbers).
- Each failing bench dies on a DIFFERENT unrelated assertion
  (`disk-ram-test` on `bIOin'`, `disk-format-test` on the RAM readback,
  `disk-input-test` on `ReadBlock`), i.e. accumulated stimulus rot across
  the `+ram16`/`+input`/`+tag` modes, not one RTL cause. All fail at
  85fe8ed5 too.

**So the RTL can very likely read the disk, and the real gate is the
multi-hour boot run.** The rung table's green claims were true for the RTL;
the benches rotted around them. The right next step is the actual boot, not
more bench archaeology.

### THE READ-PATH BLOCKER IS A CLOCK-LAG REGRESSION (2026-09-04, narrowed)

Going "ahead" on the read path, it is NOT a functional-logic bug and NOT
the cache. Narrowed to two symptoms and one likely cause:

- **The cache write cell is correct.** `cell_F10470` writes `mem[a] <= p17`
  when CE' and WE' are both low, and the array holds its filled values.
  `readback-test`'s red is the muddled STIMULUS its own header documents:
  the cache tags are never set, so every reference misses and fills from
  DRAM (overwriting the CPAT cache-data seed with the DRAM word), and the
  four-instruction loop's Store then clobbers the line before dMD is
  sampled. That bench needs a redesign (set a tag and read a hit, or
  sample dMD at the fill), not an RTL fix.
- **The format-RAM address counter does not advance.** `disk-format-test`:
  all sixteen words load with the right Q, but `RamAddr` reads 2 for every
  one of them, so all sixteen land in one slot and word 0 reads back 0.
  b21 (an F10016) counts on `RamCl'C` with count-enable tied true, so the
  increment is a clock-edge that is not arriving between the jams.
- **The clk1-family LAG is NOT the cause (tested, 2026-09-04).**
  Regenerating DskEth with the lag disabled (`DORADO_NO_LAG`) leaves
  `disk-format-test` failing identically -- `RamAddr` still reads 2 for all
  sixteen writes. So the format-RAM address counter (b21, an F10016 clocked
  by `RamCl'C`, count-enable tied true) is stuck for a reason independent of
  the clock lag: the increment edge is not arriving, or the bench's per-word
  `+ram16` jam re-zeroes the address each pass. This is a pre-existing
  disk-bench defect, unrelated to my session's changes and to the lag; the
  earlier note here blaming the lag is retracted.

So the read path is a focused TIMING reconciliation between the disk/memory
clock-family nets and the lag model, separable from the boot itself. The
memory frontier's own note already flags depth/phase timing as the open
area; this is the disk/memory-read corner of it.

**Net for the boot:** the world runs and drives the disk cable correctly;
the pack and drive are gated (`pack-test`, `trident-test`); the pendulum,
tags, one-shot, counter and CRC are fixed. What remains is (1) the
clk-lag read-timing regression above, and (2) the multi-hour run wall
(C-emulator oracle: 44.6 M microinstructions from world-load to the first
disk read, ~2 h of RTL sim; ~3.5 B cycles to a screen, ~5 h).

### WHAT IS BETWEEN THE RUNNING WORLD AND A BOOT (2026-09-04, measured)

The world runs on eleven boards with a pack on the disk cable, and the C
emulator boots that same pack to a painted screen, so the target is real.
Two things stand between them, one a wall and one a defect.

**The wall: a boot is billions of RTL cycles.** The C emulator
(`DORADO_DISK_SEQ=1`, `--disk 0=games-trident.pack --boot-reason disk`)
loads the world at microinstruction 32.0 M and issues its FIRST disk read
at 76.6 M -- 44.6 M microinstructions of world execution before a single
sector is read -- then reads the boot file (chs 35/4/0, 35/4/1, ...) and
paints 2,104 display-list pixels by 108 M. In RTL sys_clk that is
2.45 BILLION to the first read and ~3.5 billion to pixels. At the ten-board
rate of ~206,000 sys_clk/s (4 M in 19.4 s, measured; `--threads 8 -O2` is
FIVE TIMES SLOWER, `-O3` gains 9%, so no cheap lever) that is about two
hours to the first read and four to five to a screen -- per attempt.

**The defect: the memory READ PATH does not deliver a word, and it is
pre-existing.** `readback-test`, `memrun-test` and the whole `tb_disk`
family fail at 85fe8ed5 (this session's start) as they do now -- verified
by building that commit's cells and bench. This path is shared: a disk boot
reads the boot file into storage and executes it, and the display task
reads the framebuffer, both through it. So NOTHING paints from memory
content until it is fixed, disk boot or seeded image alike.

Localised, on MemD, as far as one mux:

- The DRAM read is GOOD. `readback-test`: the SN74166 loads the seeded
  word (`q at load = 10101100`, as seeded), and `Sin`/`SinD` carry it,
  64 samples high. So DRAM array -> shift register -> `Sin` works.
- The CACHE FILL takes the wrong source. `D0in` (the cache data input)
  is high on 85,678 samples while `SinD` is high on 64, and of 28 cache
  write-enable edges, ZERO have `D0in == SinD`. The cache latches the idle
  bus, not the returning read word, so `dMD`/`Md` read all-ones.
- The mux is MemD b04 (MC10158): `preDin = D_u_CD' ? Dbuf : CD`, where
  `CD` is the corrected cache data (from `SinD` via e20) and `Dbuf` the
  processor store buffer. During a fill `preDin` must take `CD`, and the
  select `D_u_CD'` comes from c18 (MC10107) off the ECC check-phase
  signals (`ChkP0A0`, `ChkP0B0`, `SinD_16`). So the break is in the
  fill/ECC check-phase path that drives `D_u_CD'` and `CD` -- a
  multi-cell state machine (the MC10180 syndrome parts, the ChkPh ring),
  not a one-liner. This is the concrete next bring-up, and it is fast to
  iterate: `readback-test` builds and runs in seconds.

Everything downstream is proven: once `CD` reaches the cache, `readback-test`
already checks `cache -> dMD -> Md`, and `memrun-test`/`disk-format-test`
are the next rungs up. The disk read benches fail for the same family of
reasons; re-greening `readback-test` is the root.

### A WORLD RUNS ON THE RTL, AND ITS DISPLAY TASKS DRIVE THE BOARD (2026-09-04)

`make -C verilog/verilator boot-world` is the deepest the machine has run.
One preload carries AEmu, Initial and Bootstrap -- their address ranges are
disjoint (0x000-0x879, 0xc00-0xfbf, 0xfc0-0xfff) so all three coexist.
Initial runs its whole initialisation and at `BootEmulator` the bench
redirects TNIA once into AEmu's `StartEmulator`, which replaces the
transport and nothing else.

| | Initial alone | Initial then the world |
|---|---|---|
| distinct IM addresses | 453 | **516** |
| task switches | 10,675 | **26,464** |
| display task (t3) sys_clk | 1,564,928 | **4,039,520** |
| visible field | rows 21..517 | **rows 18..895** |

The world reaches STARTEMULATOR, INITHRAM, LOADHRAMLOOP,
RESETDISPLAYCONFIG and both DWTINITPC and DHTINITPC: it loads its own HRam,
configures its own display, and starts its own display tasks, which then
run continuously and reprogram the raster to full height. Nothing is stuck
-- the longest run on one address is 148.

**Why the screen is still blank, and it is not the hardware.** `NEWDCB` and
`DWTSTART` are never reached: the display horizontal task walks a display
list that is not there, so the word task never starts and no bitmap is ever
fetched. Pixels need memory CONTENT, and there are exactly two ways to get
it. A transport (Ethernet or disk) so an operating system writes it, or the
bench writing it. **The bench cannot currently write a word into guest
memory**: the storage seed fills all 4,096 addresses of eight `MK4096P`
slices with one pattern, and the array is 144 chips with ECC, so a targeted
word needs the bit-to-chip map, or the write has to go through the
processor with a jammed `Store<-` after the world's map is up, which needs
the mid-run jam machinery the bench does not have yet.

**A cold world cannot be rescued by a knob, and two candidates were
eliminated by measurement rather than argument.** `+setmcr=HEX` now loads
the memory control register before the machine runs -- `T <- CPReg`, then a
jammed `LoadMcr[T,T]` (FA=1 FB=2 FC=6 = FF 0o126) built with the same
checked encoder tb_compute uses. It must go BEFORE the Link setup, because
loading T needs a CPReg write and doing that after `CPRegToLink#` left the
machine starting at address 0. With the MCR loaded the cold world still
wedges identically. `+rfshdiv=N` then made the refresh interval adjustable,
because `RfshPeriod` was a 9-bit free-running counter flipping every 512
sys_clk -- every 16 microinstructions, about 1 us -- which asks the memory
to refresh continuously. Sweeping it from 512 to 65,536 changes NOTHING:
`RefHold` 5,905,959 and `BLretry` 5,994,694 in both. The dominant hold is
the Being-Loaded retry, i.e. a cache line whose fill never completes, and
what makes it complete is Initial's map and storage initialisation. So the
authentic sequence is the only one: **a world needs Initial**, which is
what a real Dorado does, and `boot-world` is the configuration to iterate
on. `+setmcr` and `+rfshdiv` stay because both are honest instruments and
the refresh rate was genuinely wrong.

**And the display list really is absent, not mislocated.** Initial's
storage init is `T_ (Store_ T)+1, DBuf_ 0C` -- it writes ZERO to every
address, so after it the Alto's display-list head holds 0 and the display
horizontal task correctly finds nothing. (An earlier guess here, that the
loop writes each address's own value and would therefore leave a
self-referential DCB behind, is wrong; the source says `DBuf_ 0C`.)

**A TRIDENT PACK IS ON THE DISK CABLE (2026-09-04, evening).** The world's
own disk boot is the transport: `AEm0.mc`'s DiskBoot reads Alto disk
address 0 into location 1, waits a second for done status and retries
seventeen times, and the run that started the world already showed its
disk task alive, sending drive tags and waiting on sector pulses with
nothing on the cable. `dsk2trident` maps Alto address 0 to Dorado cylinder
3, head 4, sector 0 of a 29-short-sector track, and
`chm/diskpacks/games-trident.pack` is exactly that layout, 63,105,450
bytes = 815 x 5 x 29 x 534.

`verilog/verilator/dorado_pack.sv` is the pack. It decodes the
controller's tags off the cable -- a twelve-bit bus PARC names `.000 .00
.0 .1 .. .9`, MSB first, strobed by CylinderTag'/HeadTag'/DriveTag'/
ContTag' (HM p.100; `disk.c`: cylinder = all twelve bits, head = the low
six) -- loads the addressed track from the file ON THE SEEK, and streams
each short sector the way AEmu's `InitRamDiablo` format program reads it:
a zero preamble (the read delay is 11B WORD clocks), one sync bit, the
header/label/data words of 2/8/256 MSB first, each block followed by its
32-bit Fire-code check word (HM 9.10, `dorado_disk_ecc_compute`'s LFSR,
bit-serial). The drive (`dorado_trident`) gives **117 pulses per
revolution** and the controller's subsector counter divides them by four,
so short sector s starts at drive pulse 4s and the thirtieth slot is the
fraction -- `disk.c`'s constant, from TriconD's own SectorCounters test.
Two parameters worth knowing: `dorado_trident`'s SYSPER is per
MICROINSTRUCTION, so the bench passes 2x its own; and 80 words per pulse
puts 9,360 words on a track, the T-80's real capacity, for a bit period
of about 111 ns.

**And the index is a LINE, not a longer pulse.** DskEth c03 (an MC10124)
takes `TtlIndex'` and `TtlSector'` as two separate inputs and makes
`Index'` and `Sector` from them, so the controller does not separate index
from sector by pulse length as the drive model's comment supposed; the
bench had `TtlIndex'` tied deasserted, which would leave the controller's
sector counter never synchronised to the pack's sector zero -- a boot
would read the wrong sector and fail its header check. `dorado_trident`
now outputs `TtlIndex_n` in the sector-0 window and the bench connects it.

**The first pack boot found three defects, none in the pack.** The run was
150 M sys_clk and the world reached its disk boot in none of them -- ABoot
opens with a keyboard wait of 3 x 39 ms of Dorado time, about 62 M
sys_clk, and the world started at 102 M -- but the drive-side counters were
already decisive:

- **The controller raised ONE sector wakeup in seven revolutions.** The
  unit's sector pulse (cable `SecIndx0'`, buffered by LS08 d01) is one net
  feeding the SN74123 one-shot a05's A input, the LS169 subsector counter
  a01's clock AND both its enables; the one-shot's Q is the counter's LD'
  and the counter's terminal count RC' (`Sector0'`) is the one-shot's clear
  and the divided sector pulse the controller wakes on. Two cells broke
  it. `cell_SN74123` was a SKELETON -- Q undriven read 0, LD' held
  asserted, the counter never counted. It is a retriggerable monostable
  now. And `cell_SN74LS169` read its enables AT the clock edge, where on
  this board they are rising with it; the chip counts because it sampled
  them low a setup time earlier. One sys_clk of history on EP', ET' and
  LD' is that setup time (the MC10176 setup-capture idea again, and used
  on DskEth alone).
- **The world put the Alto disk on head 18.** `AltoDiabloDisk.mc` selects
  head 5 and reads `muffHeadOvfl` to tell a T-80 (MaxPartition 5, the
  Alto disk on head 4) from a 19-head AMS-315 (MaxPartition 23B, head 18),
  and `HeadOvfl` is DskEth b24's translation of the cable's
  **`TtlEndOfCyl'`** -- a line the drive never asserted, so the world took
  the bigger machine. `dorado_pack` asserts it for heads >= 5 now, and
  `+packdbg` prints every tag it receives with its decoded value.
- **The tag order was right after all.** The MC10173 latches pair B0->Q0
  = p6->p1, and under that pairing the register's IOB sources are
  monotonic: `.000` = IOB.04, the bus MSB, down to `.9` = IOB.15. The
  "cylinder 8, head 8" the first run reported was what the world sent
  while it thought it had nineteen heads.

**Two more, from the second and third pack boots, and neither is the
pack's either.**

- **The world's real-time clock has never ticked.** DiskBoot was not
  reached at 230 M cycles because ABoot waits 3 x 39 ms on VM 430, the
  Alto's real-time clock, which the junk task advances on the 32 us
  pendulum wakeup -- and `Pendulum` is an INPUT of the IFU board, from the
  BaseBoard's timer chain, that no bench ever drove. Every JUNK line this
  campaign has read "Pendulum edges 0". `+pendulum` supplies it, a 32 us
  square wave scaled by SYSPER, opt-in so the older gates do not move.
- **The tag strobes idled ASSERTED between tags.** They leave DskEth
  through 8T98 tri-state buffers (g22) whose enable is `TagStrobe` itself,
  so between tags the lines are released; the generated wired-OR made a
  released driver 0, which on an active-low line is asserted, and the drive
  saw the OTHER lines fall at the end of each tag window -- the head-5
  probe logged as a cylinder tag, the T-80 test missed. A real cable's
  pull-ups are at the drive and hold a released line high. All three 8T98
  packages in the machine drive this cable and nothing else drives those
  lines, so `cell_p_8T98` now reads high when released. The tag DECODE was
  right throughout: the microcode's constants (DiskDefs.mc: tagDrive
  100000B, tagCylinder 40000B, tagHead 20000B, tagControl 10000B), the
  MC10173 latch pairing, the MC10125 translators and the 8T98 buffers all
  map straight through to the cable's `DriveTag'`, `CylinderTag'`,
  `HeadTag'` and `ContTag'`.
- **Simulation rate, measured so nobody guesses**: the ten-board machine
  runs about 200,000 sys_clk per second (4 M in 19.4 s). `--threads 8
  -CFLAGS -O2` is FIVE TIMES SLOWER (103.6 s); `-CFLAGS -O3` alone gains
  9%. So a full Alto OS boot, one to two seconds of Dorado time, is a few
  hours of background running; there is no cheap lever in the build.

Gate `pack-test`: the drive turning, the pack serving c3/h4/s0, an
independent receiver decoding it -- all 266 words match the file and every
check word cancels its block. It found one bug: loading the track lazily
at the sector start served cylinder 0 for a whole sector after the tags
had named cylinder 3. `boot-world-disk` is `boot-world` on eleven boards
with `+pack=PATH`; unattached, every cable line idles deasserted exactly
as the old ties did.

**THE NEXT STEP, scoped.** Pixels need a display list in guest memory, and
with `boot-world` the machine is otherwise ready for one. The bench must
therefore be able to place a word, and the storage layout is now mapped far
enough to say what that takes:

- `msa` holds **144 `MK4096P` chips**, each 4096x1, whose `mem[]` index is
  the chip's own 12-bit {row, col}; the row and column come from
  `cell_MK4096P_addr`, of which there are **16**, one per control group.
- Their outputs feed **20 `SN74166` shift registers**, 8 parallel inputs
  each, which serialise onto `Sin` -- so a bit's position in a word is a
  (serialiser, input) pair, not a chip number.
- The bench's existing seed writes ONE pattern into the eight chips of a
  single serialiser group across all 4,096 addresses, which is why it can
  fill memory but not place a word.

So a targeted write needs two small derivations from the netlist: word
address to (`addr` group, `mem[]` index), and bit position to (serialiser,
parallel input). Both are static. The alternative is a mid-run jam of a
`Store<-` through the processor, which needs machinery the bench does not
have; the direct write needs none.

**Two things this settled on the way.** A world CANNOT be started cold: at
`START` (0x000) it parks in the IFU not-ready trap, which is correct for
that entry, and at `STARTEMULATOR` without Initial it wedges with RefHold
on 5,905,947 of 6,000,000 samples, its display task held on an instruction
that reads Md. Initial's `BlessBaseBoard` ends by loading the MCR with
`mcr.noWake` -- "now allow holds" -- and a cold world has never had an MCR
loaded at all. And `mbdis`'s columns are the ENTRY WORDS, not addresses:
the symbol table's `addr=` is the IMAGE index, and the real IM address is
the low twelve bits of the fourth word, which `im_image` prints directly as
an image-to-real map.

### Two traps, both of which cost a run each

- **The exec recipes filter their own output**: the rules end in
  `echo "$$out" | grep tb_exec`, so any `$display` whose text does not
  contain `tb_exec` is DISCARDED. Two probes appeared to produce nothing at
  all. Prefix every new probe with `tb_exec:`.
- **A THIRD instance of the filter trap**: `ETHLOG2`, the transmit-chain
  log, was written without the prefix and produced nothing in a 35-minute
  boot. Prefix EVERY probe with `tb_exec:`.
- **Moving a probe can consume an enclosing `endif`.** Relocating the DYCLK
  block took the wrong terminator with it; the junk-task block fell out of
  its `ifdef WORLD` and `exec-test` stopped elaborating. Check the guard
  stack after any edit that moves a conditional block.
- **Where you insert a probe decides which builds run it.** A block placed
  "just before the FULL section" landed INSIDE an enclosing `ifdef FULL`,
  so the control half of an A/B silently produced no line. Check the
  enclosing conditionals, not the neighbouring text.

### The three defects, each with its measurement

1. **`cell_MC10181`'s look-ahead pins were the TTL '181's G and P; this
   part's are the two carry-out conditions, with the names interchanged.**
   ProcH forms the carries into its two slices with discrete OR-AND gates
   from ProcL's GG/PG and ripple carry: d12 (MC10118) gives
   `(PG1 + C0) . GG1` and e12 (MC10121) gives
   `(PG1 + C0 + PG2) . GG2 . (PG2 + GG1)`. An exhaustive polarity search
   over those two gate networks finds exactly ONE reading under which they
   are a correct 16-bit adder: `GG` = carry out if Cn were 1
   (X+Y >= 15), `PG` = carry out if Cn were 0 (X+Y >= 16), carries
   active-high (8,112 vectors, 0 mismatches). The MC10179's own printed
   equations ("Cn+2 = (Cn+P0+P1)(G0+P1)G1") are the same form and correct
   under the same reading; the part's function table is the 74181
   family's active-low-data table, in which G and P exchange roles.
   Symptom: a carry GENERATED in the adjacent slice arrived, a carry
   PROPAGATED through it was lost -- fff1+000f read ff00, fff1+0010 read
   f001 -- so c46's T-15 never reached zero and the loop never exited.
   `alu-diff` never saw it (it chains the slices by ripple carry) and
   `compute-test` never saw it (a55a+1234 carries out of no nibble).
   compute-test now sweeps fff1/000f too and FAILS on the old cell
   (mutation-tested: ALUFM 14 gives ff00).
2. **ContA's task-switch select never reached ContB.** ContA drives it on
   backplane pin E166 as `SWb` (j19); ContB receives E166 as `SW` (k21, the
   fan-out to the e20/f20/g20/g21/h20 MC1662 muxes that put BNPC on the IM
   address). Same pin, different name: the backplane is wired by name, so
   ContB's SW sat at 0 and a switch loaded CIA and CTask with the new task
   while the IM went on reading the old task's next address -- CIA=c61
   with IM[c00]'s fields in the MIR, measured with the SWP probe. Fixed by
   `BACKPLANE_BOARD_ALIASES` in `tools/sil_netlist.py`, a BOARD-SCOPED
   alias (`('ContA','SWb') -> 'SW'`), because ContB also has a LOCAL net
   spelled SWb. (E167 is the same shape, ContA `SWm` / ContB `TNIA.03`,
   but on ContB that pin reaches only the muffler mux e23; left alone.)
   `make -C verilog backplane` now regenerates `dorado_world.v` and
   `dorado_screen.v` too -- they were not in the target.
3. **IOReset is a BaseBoard line the board-level bench never drove.** The
   firmware asserts it in `DoIOReset` (the first InitManifolds entry) and
   `PrepareProcessor` re-asserts it before every load. On the IFU it is the
   reset of a21 (ShutUp), which holds the junk-wakeup flip-flops j08/k08.
   Undriven, their power-up states leaked: TWReq.02 high from cycle 5129
   with ZERO pendulum ticks and ZERO acks (JUNK probe), so after task init
   the junk task re-executed Initial's BLOCK at c06 forever (679,732
   instructions on one address in the full boot). `+ioreset` holds the
   line through the startup jams and releases it when the machine starts.

4. **The display boards never received the slow-I/O strobes.** ProcL
   drives `IOout'`/`IOin'`; DskEth, IOTest and Music receive them under that
   spelling on E74/E71, but DispY and DispM spell the same pins `IOOut'`/
   `IOIn'`. Wired by name, both display boards saw their strobes at 0 --
   asserted, being active low. Measured with the DSW probe: HRamCommand'
   low across whole instructions, RIOB re-latching the idle bus every clock
   instead of holding the Output word (HM 10.7: it holds until the next
   command), the HRam address counter free-running (6 million edges), the
   table landing stretched (14x0 34x6 16x0 10x2 177x0 ...), and ReleaseRam's
   Keep' bit overwritten one clock later, so the sync counter stayed in
   reset. Board-scoped aliases for DispY/DispM in `BACKPLANE_BOARD_ALIASES`
   (not case aliases: ProcL's end is on E71/E70, the backplane is not
   straight-through). display-test/strap-test/muffler-test still pass.

**RETRACTED (same day): "cpu.c's ReadIM lane 0 reads one low".** The
oracle's 33, 9, 144, 34, 147 are TTemp0 read at 7054, AFTER DoHRamDisp's
`TTemp0-1`; before it they are 34, 10, 145, 35, 148, exactly the RTL's and
the source's. cpu.c is right; the commit message of fb7ecef3 is wrong on
this point. The RTL's ReadIM/B<-Link path is right (the HRam sum is
HRamMaxAddr+1 = 380).

### Retractions of the 2026-09-01 diagnosis, each by measurement

- **The WTPC 6-cycle residency works and always did** (RING probe: Phase0
  -> 1 -> 2 -> 3 -> 4 -> StartCycle, Link rewritten to CIA+1 at Phase4,
  next instruction c47), in the focused bench AND in the full boot.
  `RepeatCur` is the HOLD (f20: Hold & ~SwitchUp), not the residency; its
  being 0 was correct. `+wtpcfix` is not needed; do not use it.
- **"c61 fetched as a spurious instruction"** was TNIA showing Link during
  the residency, which Figure 7 specifies (TPCI <- TNIA(Link) at t1c).
- **"ResLtZero' sampled at the wrong phase"** was the wrong ALU result
  (defect 1). The branch-condition path (ProcH e13 RAM, e14 muxes, f17
  MC10173 latch on Clock1') samples correctly.
- **The bench's "visited c44" is an artifact**: `tnia_now` is read before
  the branch condition is ORed into bit 15, so a taken c47 shows c44 on
  TNIA while CIA loads c45. `TASKINITLOOP entries` undercounts for the
  same reason.
- **The phase/depth model is not needed for any of this.** Nothing in this
  session used `DORADO_CLOCK_DEPTH`.

### One clock, two names -- the SYSPER correction

HM 2.3: the atomic clock is 30 ns and a cycle is TWO clocks. The RTL's
CLK line is that 30 ns clock (one pulse per SYSPER sys_clk), and the ContA
phase ring (l07/k05/j01/k04) advances once per pulse: a normal
instruction is Phase0 then StartCycle, 32 sys_clk at SYSPER=16, measured.
The bench's "1,242 microinstructions in 20,000 cycles" counted both
edges of clk0' (which is gated by StartCycle, so it pulses once per
instruction): 621 instructions. Consequently `fpga/mister` at SYSPER=2 and
33.33 MHz runs a 60 ns CLOCK, a 120 ns microinstruction: **0.5x a real
Dorado, not 1.0x**. The README's speed claim is wrong by two.

### Probes and knobs added to tb_exec.sv

`+ringpc=HEX` / `+ringat=N` + `+ringlen=N` open a window printing three
lines per sys_clk: RING (the phase ring, next-address and Link/TPC path),
BRC (the branch-condition path and ALU operands/result), SWP (the
task-switch fetch: Switch, BNT, TPCO/BNPC, ContB's dRA and MIR fields,
the IM data bus). `+pcfrom=N +pclen=N` prints one PCT line per MIR load
(CIA, JCN, task, Link, T, R, ALU, conditions). `+tseed=N` seeds task 0's
T-file slot at the first TNIA=c45, so `+start=3141` reproduces the
task-init loop in ~3,000 sys_clk instead of 96M. `+ioreset` as above. The
JUNK summary (final block) counts the junk-wakeup flip-flops.

### Gate status (all re-run this session, self-consistently)

PASS: alu-diff, compute-test (now with fff1/000f), exec-test,
exec-parity, task-test, taskrun-test, refdecode-test, mir-diff,
boot0-test, im-parity-check, ifu-test, datapath-test, muffler-test,
machine-test, strap-test, exec-boot; `make -C verilog lint` clean (16
boards, 12 tops).
FAIL, unchanged from HEAD: exec-tasking, exec-init (both fail at HEAD
too; with the switch line joined the junk task 2 now wins the switch and
runs AEmu from IM[0] -- these jump-start benches wake tasks whose TPCs
were never initialised and their green states were artifacts),
memrun-test, readback-test (identical assertion at HEAD).
display-test: it regressed with defect 2's fix and is FIXED. The
`+slowio` bench starts with TaskingOn and requires the woken display
task (3, WakeDHT is permanently high) to take the machine; with the
switch really redirecting the fetch, task 3 ran the loop with ITS OWN T
(0), so `TIOA<-` addressed nobody. T is per-task; the bench now seeds
task 3's T-file slot with the TIOA value, as a real display task loads
its own TIOA (InitialDisplay.mc `T_ Statics ... TIOA_ T`), and it
passes more faithfully than before (TIOA held on 3,846 samples, not
128). tb_disk got the same seed for task 12, which moves disk-input-test
past "the address never survives to the data" (its HEAD failure) to a
later assertion; the whole tb_disk family (disk-test, -tag, -muff, -cmd,
-ram: "bIOin' asserted 1953 times with no Pd<-Input"; disk-format:
"format RAM[0] = 0000") FAILS AT HEAD too, self-consistently -- stale
green, not touched by this session. tb_disk also gained an opt-in
`+ioreset`; it changed no verdict there.

### Traps that cost time today

- The gate rules' `make_rc` after `| tail` is tail's status. Read the
  PASS/Fatal lines. Six gates "failed" because their obj_* directories
  still carried the old repo path; `rm -rf` them.
- `+ringpc=c44` opens at the FIRST c44, which is every c47's pre-branch
  TNIA; use `+ringat` for the loop exit.
- The datasheet's pin NAMES (GG/PG, "Cn is low") are a trap; the board's
  own gate network is the authority, and a brute-force polarity search
  over it takes a minute.

---

## SESSION HANDOFF (2026-09-01, end) -- read this first, then "START HERE" below

Branch `verilog-backplane`, HEAD `7d542c14`, **32 commits this session**
(from pre-session `fe0c1874`). Written to be picked up COLD by a fresh
or upgraded model.

### Where the boot stands, in one paragraph

PARC's Initial microcode now boots -- on the board-level `dorado_world`
(`make -C verilog/verilator exec-boot`) -- through the ReadBB handshake,
checksum, ALUFM/RM/STK/IFUM init, map init, the storage-module probe,
the stack, the fault path, and INTO Initial's TASK-INITIALIZATION loop.
Three successive "park" walls fell this session (each was a machine that
sat forever on one address). The RTL also PAINTS A BOOT SCREEN:
`docs/images/rtl-boot-raster-2026-09-01.png`, 891 scan lines of synced
raster from the ten-board `dorado_screen` while Initial runs
(`make -C verilog/verilator exec-bootscreen`). The one thing between here
and a world coming up is THE PHASE/DEPTH MODEL (next section).

### The exact command that boots deepest

    make -C verilog/verilator exec-boot \
      EXECARGS="+fastwait +bbword=0000 +bbfeed +mb0 +stperroff +blfix +wtpcfix +addrs +cycles=140000000"

Those knobs are BENCH STAND-INS for real hardware behavior the collapsed
SYSPER model does not reproduce, each documented at length below and in
the commit messages. They are not cheats -- every value fed is one the
bus demonstrably carried, or the state the microcode itself would leave:
`+fastwait` (clip LongWait's DRAM-settle countdown), `+bbword=0000
+bbfeed` (be the BaseBoard feeding ReadBB's checksum), `+mb0` (drive the
storage-module presence token the backplane ties at the PCMSA slot),
`+stperroff` (force the continuously-failing ST-store parity latch off
the CAS register's reset), `+blfix` (the fault path's Being-Loaded-flag
cleanup), `+wtpcfix` (force a Write-TPC's next-fetch to CIA+1, the C
emulator's own next=real_PC+1).

### THE ONE REMAINING PIECE: the phase/depth model

**Everything left converges on a single root: the SYSPER model collapses
the microcycle's phases**, so signals the real hardware samples at
specific phase edges are read at the wrong instant. Three things this
session proved it:

1. **Write-TPC (task-init loop, c46).** Its 6-cycle TPC/IM residency
   (HM §4.8) never happens (`RepeatCur=0`), so it fetches its Link value
   as a spurious instruction. `+wtpcfix` stands in (spin gone, longest
   run on one address 96,721 -> 7). The write itself is SOUND
   (taskrun-test writes TPC correctly from jammed instructions).
2. **The loop's continue/exit conditional (c47).** It is a BRANCH-ON-
   CONDITION `FFBrOnResLt0` (NOT a BDispatch -- cpu.c's label misled me;
   PARC's b15 makes FA=0/FB=6 the branch-on-condition group; PARC's real
   BDispatch is FA=0/FB=7). The RTL decodes it correctly, but `ResLtZero'`
   (the ALU sign) TOGGLES 1<->0 within c47's residency and the branch
   captures the wrong phase -> always exits (c44) where the oracle
   continues (c45). The condition net is combinational (a14, MC1662), so
   there is no clean targeted fix -- the datapath's other activity smears
   the ALU inputs across the collapsed cycle.
3. **The stale-green gates** exec-tasking, exec-init, memrun-test all
   fail on pre-session RTL (byte-identical inputs), bisected to
   **d3e337a2, the MC10176 pre-edge setup-time capture** -- itself a
   collapsed-timing workaround that helps the boot's checksum and breaks
   these. Same root.

**The fix is the depth/phase model** the 2026-08-29..31 depth campaign
began: per-net BUFFER-DEPTH lag from each board's CLK input (opt-in
`DORADO_CLOCK_DEPTH=1 make -C verilog generated-boards`; REGENERATE
DEFAULT AFTERWARDS). It was left with "OPEN: the jam's Link load" (see
the depth-mode block in the old START HERE). Bringing it up so the
task-init phases separate is what lands the boot. FAST-ITERATION PATH
(the first WTPC is at ~108.9M cycles, ~10 min/run): build a focused
bench that preloads IM and jumps straight to TASKINITLOOP (real 0o6105)
via exec-world9's `+start=`, seeding T as the loop counter and the
task-init PC in RM -- reproduces the loop in thousands of cycles.

### The method that carried the whole session

**The C emulator is the oracle.** `DORADO_PCDIS=<lo>,<hi>` (per-
instruction disassembly in an octal PC range) and `DORADO_TASK_TRACE`
(TPC writes) dump the exact microcode behavior of the daily-booting C
emulator; run them from the `dorado/` directory. Diff the RTL's
execution against that and it diverges at a NAMED instruction. This
found and settled every blocker.

### Traps that cost real time (do not re-pay them)

- **A bench force can destroy the very data under diagnosis:**
  `+fastwait`'s T-file clip zapped the SetMCR value and sent three
  sessions chasing a strobe-phase ghost. Measure the whole bus (all
  bits), not two.
- **A level is not an event; an idle level read through a 2-bit probe
  looks like armed data** (the all-ones bus). Count events; bucket by
  TIME before concluding a run has not settled.
- **cpu.c LABELS can mislead** ("BDispatch" for what PARC decodes as a
  branch-on-condition). PARC's raw `.wl` wire list is authoritative for
  what a gate decodes.
- **Stale binaries lie:** when the repo path moves, gate rules error
  `No rule to make target .../obj_*/Vtb_*__pch.h` and the OLD binary
  runs; and a build that fails on a bad net path leaves the prior
  binary. `rm -rf verilog/verilator/obj_<name>` and grep `%Error`
  before trusting any run.
- **im-parity-check is a `make -C verilog` target, not a verilator-dir
  one.**
- **The T counter reads BIT-REVERSED** in the T-file slot (8,4,12 =
  1,2,3) -- the same address reversal as RM/TPC/IM; don't mistake it for
  a stuck counter.
- **SYSPER=2 breaks the ReadBB/checksum stage** (RMINITL never reached);
  boot runs stay at 16x. Depth mode's phase spread will set the true
  minimum.

### Regression state (verified clean)

`make -C verilog lint` = 16 boards + all 12 machine tops elaborate, 0
failed (the skip-list was missing world/screen/disk/display -- fixed).
Passing gates: boot0, compute, task, taskrun, refdecode, exec,
exec-parity, mir-diff, ifu, im-parity-check. The only failures are the
pre-existing stale-green ones above (exec-tasking, exec-init, memrun).
The board regeneration from the JunkTW jumper (IFU JunkTW -> TWReq.02)
is confirmed clean.

### The probe kit left in tb_exec.sv (all WORLD-gated)

MCRWIN (the whole Mar bus on change, `+tcwin`/`+tclen`), MAPWEDGE
(freeze-keyed memory-state dump + 64-instruction approach ring),
DRAMWIN (a fixed window over the miss's DRAM cycle with MemState/Mod/RP),
MAPWRITE/MAPRAM/MAPCENSUS (the map write path), CFLAGS/VICWIN/W0STROBE
(cache-flag strobes), TRANSPORT/STARTCHAIN (storage transport), STKADJ/
STKWIN (the stack adder), TSW (the tasking latch, dispatch, T, and the
branch condition), TPCDUMP (the whole TPC file), FIRSTHIT (era-entry
cycles), and `+wtpcfix`/`+mb0`/`+blfix`/`+stperroff`/`+addrs`/`+trwin`.

---

## START HERE (2026-09-01) -- three parks fell in one sitting; the boot is deep in Initial

The 2026-08-31 state below is history now. What changed today, in order,
each with the measurement that settled it:

1. **THE LOADMCR SAGA IS CLOSED, AS A BENCH ARTIFACT.** The full 11-bit
   RMar probe (`+tcwin`/`+tclen`, change-only MCRWIN lines) showed the
   "in-residency data" of 198b28f8 was the bus's ALL-ONES IDLE (every bit
   1, two were read), and the true data phase (MarMuxAEn'=0) read all
   zeros because **`+fastwait`'s T-file clip had zapped the SetMCR value**:
   the clip fired while TNIA read c0a with T > 0x10, and the second
   LoadMCR executes exactly there with its value (0x67) in T, A<-T. Clip
   off (`+clip` restores it), every LoadMCR captures true data at its own
   edge -- NoRef sets AND clears under microcode control, the d27 park is
   gone, and **no timing fix was ever needed**. The l09/j09 strobe-phase
   hunt and the depth-mode motivating case are retracted (depth mode's
   PreClock1'/Clock1' physics still stand; its jam-Link open item is now
   uncoupled from the boot).
2. **NOSTORAGE: nothing originated the module-presence token.** Past the
   (bounded, ~35M sys_clk) RESETMAPL/WAITFORMAPBUF map init, Initial
   parked 758k microinstructions at real 6247 = NOSTORAGE. B<-Config'
   reads modules from M0..M3; MemX's four M pins are all INPUTS; the msa
   RELAYS Mb0 (C181) to its M0 output (h26, pure OR) on C184 -- MemX's M1
   PIN (slot-jumper pattern; pin != wire). The origin is the backplane
   ("awaits PCMSA"). The bench drives it under **`+mb0`** (opt-in: the
   AEmu jump-start gates were calibrated on the no-module machine), and
   `MACHINE_CONFIG_TIES` in sil_backplane.py ties it (plus
   ChipsAre256/16K) for the dorado_machine wrapper.
3. **THE STACK-POINTER ADDER WAS A SKELETON.** Next park: 3.57M
   microinstructions at real 6001 = DORETURN (BLOCK + RSTK=1000 = read
   STK[StkP], check on) -- StkP had sat at STKINITL's offset-0 through
   every push. The adjust value StkP[2:7] + sign-extend(RSTK[1:3]) is
   computed by ProcL j17/k17/l17, three **MC10182**s, and the cell was a
   TODO skeleton ("no datasheet" was stale -- MC10182.pdf is in
   DoradoDocs/datasheets). Modelled from EclDict + the datasheet (dict is
   MSB-first: D0=A1, H0=F1, F0=S1; S/M unconnected = LOW = A plus B plus
   Cn), StkP moves: 1,228 changes in 400k cycles, pushes +3/+1, pops -1.

4. **THE MACHINE PAINTS ITS BOOT SCREEN**
   (`docs/images/rtl-boot-raster-2026-09-01.png`): 891 scan lines of
   full-height synced raster -- sync, blanking band, dark visible field,
   what a real Dorado monitor shows before a world installs a display
   list -- from the ten-board machine WHILE Initial boots. What was in
   the way was TWO POWER-UP STATES, both in DispY h15 (one MC10231):
   FF a (DoradoHasHRam, clocked only during an HRamCommand' strobe)
   powers up with its Q' HIGH, and that net is l10's MASTER RESET --
   HSync can never move on an unconfigured board, correct hardware; and
   FF b (HRamWE) powers up with HRamWE' ASSERTED, so the walking HRam
   address BULLDOZES the +hram preload (measured: 35 one-sys_clk
   preHSync slivers where a 34-entry sync run should read high --
   38,795 l10 loads, preHSync=1 at ZERO of them). `+hasram` deposits
   both FFs to the state the microcode's HRam-load teardown leaves and
   reapplies the table. The older `+mrlow` pin force is INERT (module
   inlining aliases the forced port to the whole net). Ruled out by
   measurement: walk order (perfectly linear), the address counter
   (bit01's 25 transitions over 25,951 counts is a HIGH-ORDER bit doing
   its job -- the "nearly stuck" reading watched the wrong bit), and
   LdHRamAddr' (never spuriously loads).

**The boot command now:**

    make -C verilog/verilator exec-boot EXECARGS="+fastwait +bbword=0000 +bbfeed +mb0"

**The boot screen now:**

    make -C verilog/verilator exec-bootscreen CYCLES=3600000 SCREENARGS="+hram +hasram"

writes `dorado_frame.pgm` (1024x896, line-per-HSync, field-per-VSync,
degrades to a top-fill when vertical timing is absent -- as now) and
`dorado_raster.pgm` (the scan-line strip) in the repo root; `pnmtopng`
renders them. `+hram +hasram` stand in for INITHRAM, which the boot has
not reached.

**THE BOOT'S NEXT BLOCKER, measured (updated 2026-09-01 late):** the
park chain is traced to its bottom and has TWO layers. Layer 1: the
first cache miss's DRAM read runs RAS-ONLY -- the timing PROM's CAS
column (j13 pl12) commands CAS through states 0-5, but MakeMemCAS is
MemX i10, an F10016 whose MASTER RESET is **STPerr**, and STPerr (h08
registering dSTPerr & EnableSTPerr, dSTPerr computed on MemC) reads 1
on EVERY sample: MemC's ST-store parity check fails continuously --
the THIRD instance of the stored-parity-convention question (IM, IFUM
before it). `+stperroff` forces it low. Layer 2, found by that force:
CAS then fires EXACTLY ONCE in 140M cycles (CASa 4 transitions, RASa
257,114 -- the retry loop's RAS spam), the munch transport never
completes (CacheLoad 0 always), and the boot parks at the same
RESETFAULTINFO spot. Layer 2, then pinned finer with
counters under the force: the read cycle COMPLETES (CAS through states
1-6, both halves load -- LoadSinE and LoadSinO one pulse each -- and
MakeTransport0 pulses), but **Mod0SinEn' makes ONE transition in 200k
cycles**: the enable that lets module 0 drive its shifted data onto the
Sin bus never enables during the transport, so the read's data never
reaches MemD, the cache line is never written, CacheLoad stays 0, BL
never clears, and the retry cycles that follow are CAS-less because the
error state re-wedges. SUPERSEDED AGAIN, deeper (see
the dated block below this one for the final state). The decode is
traced to its end, WITH A RETRACTION: "Mod0SinEn' never enables" was an idle-level
misread -- the net is ACTIVE LOW and idles asserted (1 transition =
one deassert). The full-window probe shows the read running with
module 0's Sin enable asserted throughout, and **ModEn'a = 1110: the
read targets MODULE 3** -- correctly, because **RP.00-07 read all-ones
AT REFERENCE TIME** even after the bench seed was changed to map into
module 0 (RP planes = MosRam a04-a11; zeroing eight planes preserves
e11's parity; the change is in tb_exec and keeps its comment). So the
entry the miss translates through is all-ones at 82k -- Initial's own
RESETMAPL-era contents, i.e. a VACANT entry -- and **the map-fault
path never fires** (ValidMapFlt = 0 across every run): a reference to
a vacant page runs a real storage read into an absent module instead
of map-faulting, and the BL flag wedges. THE NEXT LINK: why MemX's
vacant/map-fault detect (ValidMapFlt) never asserts -- that is the
protection that turns this scenario into a handled fault on real
hardware. Then: (2) fit MemC's ST-parity write side against its
checker (the im-parity-check method) so +stperroff retires; (3) check
that a faulted/absent read still COMPLETES with error flags rather
than wedging (the manual reports and finishes).

**THE BOOT NOW REACHES TASK INIT, and the remaining defect is NAMED
(2026-09-01, later).** Three stand-in fixes cleared the RESETFAULTINFO
park -- the delayed map seed (entry reads VACANT -> the first miss
FAULTS; ReportFault counts 32, the first fault ever), +stperroff (the
ST-parity latch off the CAS reset), +blfix (the fault path's BL-flag
cleanup) -- and one real fix, the JUNK-TASK WAKEUP JUMPER (IFU JunkTW ->
TWReq.02, cpu.c's DORADO_JUNK_TASK=2; it was unwired, the slot-jumper
class). longest-run-on-one-address fell 971,971 -> 7 and the machine
runs Initial's TASKINITLOOP.

**But task init SPINS, and the oracle names the exact divergence.** The
C emulator (DORADO_PCDIS=6100,6160) runs the loop
c45(TASKINITLOOP) -> c60(GETTASKINITPC) -> c46 -> c47 -> c45, T counting
17,16,15..1 then exiting at T=0o20 having written TPC[1..15]=0o6141. Our
RTL runs c45 -> c60 -> c46 -> **c61** -> c44 -> ... : at c46 it goes to
0o6141 (=c61=TASKINIT, the VALUE being written) where the oracle goes to
c47 (=CIA+1). **c46 (0o6106) is a WRITE-TPC** (JCN=0o157: return-class,
JCN[2:4]=5). Write-TPC's next address is CIA+1; our RTL takes TNIA<-Link
like a RETURN (JCN[2:4]=0), and Link there holds 0o6141.

**THE MECHANISM, MEASURED AIRTIGHT (2026-09-01, probe TSW at c46).**
The RTL DOES decode WTPC: ContA d20 (MC10161) is the JCN[2:4]
sub-decoder, enabled by e21's `Return'` and JCN[2],[3],[4], producing
`WTPC'`/`RTPC'`/`RIM'`/`WIM'` -- and WTPC' asserts (low) exactly during
c46's decode. But **RepeatCur is 0 on EVERY sample**: the 6-cycle TPC/IM
RESIDENCY never happens. The phases toggle (Ph0'/Ph4' alternate
per-instruction) but the op completes in ONE cycle, so ContA's TNIA mux
(i24/i22/i21 for TNIA.10, and siblings) takes the **Return'c** source
-> TNIA = Link = 0o6141 = c61 (**CORRECTION**: an earlier commit called
f21 the branch-source mux; f21/j03 are the MUFFLER readback muxes
feeding DMuxData -- the real next-address path is i21(MC10159)/
i22(MC10101) for the CIA-relative address and i24(MC10121) which ORs in
`Link_10'` under `Return'c`), and c61 (TASKINIT, the VALUE being written) is FETCHED AND
EXECUTED as a spurious instruction. The full RTL loop is
c45 -> c60 -> c46 -> **c61** -> c47 -> c44; the oracle's is
c45 -> c60 -> c46 -> c47 -> c45. Two things are wrong and the first
causes the second: (1) WTPC fetches Link as an instruction because
nothing holds it for its 6 cycles; (2) the loop's BDispatch exit-control
(c47, FF=0o061, ORs Link[13:15] into the next TNIA) then lands c44 where
the oracle lands c45 -- the dispatch value is polluted by the spurious
c61. **THE FIX is the 6-cycle TPC/IM addressing flow, and it is ROOTED IN
THE PHASE RING** (HM §4.8, Figure 7). The residency mechanism is
LOCATED: `RWTPCorRWIM` (d21) feeds **j05 (MC10117)** together with
`dStartCycle`, `Phase0'`, `Phase4'`, `preStartCycle` -- i.e. TPC/IM ops
SUPPRESS the start of a new microcycle so the phase ring holds the
instruction for its 6 cycles, during which Link is written to TPC (data)
and, at completion, TNIA advances to CIA+1. RepeatCur=0 (measured)
because our SYSPER model COLLAPSES the phase ring: the StartCycle
suppression that should span 6 real cycles runs in one, so the op
completes immediately and i24's Return'c -> Link is fetched. **This is
the SAME phase-collapse root as the depth-mode campaign** (`dStartCycle`/
`preStartCycle` are the exact nets that campaign fought), not a decode
gap and not a jumper. **+wtpcfix WORKS AND KILLS THE SPIN; the loop's SECOND blocker is a
CONDITIONAL BRANCH, not a pipeline (2026-09-01, later still --
CORRECTS an earlier claim in this same file).** The CIA-based +wtpcfix (force
TNIA = CIA+1 while a WTPC is in the MIR -- CIA is ContA's current-
instruction-address register, the clean source; the first cut's
sys_clk history tracking glitched) fires exactly 15 times at CIA=c46
-> c47 and the RESULT IS DECISIVE: longest-run-on-one-address falls
**96,721 -> 7**, the stuck-fatal is gone, the machine sequences. But it
still does not reach BOOTEMULATOR: the loop still does not
complete: c47 always branches to c44 (exit), never c45 (continue), so
task init never iterates all 15 tasks. **c47 is NOT a BDispatch** --
that was a wrong turn (cpu.c LABELS FA=0/FB=6/FC=1 "BDispatch", but
PARC's decode is different): PARC's b15 (MC10162, raw wire list) makes
FA=0/FB=6 the BRANCH-ON-CONDITION group (FFBrOnRes=0, FFBrOnResLt0,
FFBrOnCnt=0, ...), and c47's FC=1 selects **FFBrOnResLt0** -- "branch
while the ALU result < 0", the loop-counter test. The RTL DECODES this
correctly (b15 is faithful; and FF_eq_BDispatch legitimately does NOT
fire, since PARC's BDispatch is FA=0/FB=7/FC=1, an entirely different
instruction). So the blocker is NOT a collapsed pipeline and NOT a
decode miss -- it is a CONDITION that evaluates to "exit" every time.
CONCLUSIVELY MEASURED (probe brres.log): T DOES advance -- the values
0008/0004/000c are C's T=1,2,3 with the low nibble BIT-REVERSED (the
same reversal the RM/TPC/T address lines use), so the counter is fine.
And the condition IS selected: at c47 (samples where pc=c44, MIR=c47)
FFBrOnResLt0=1 and CBrOnResLt0'=0 (asserted). THE ROOT IS
PHASE-SAMPLING: ResLtZero' TOGGLES 1<->0 WITHIN c47's residency
(result >=0 at one phase, <0 at another), and the branch captures the
>=0 phase -> TNIA15=0 -> exit (c44), where the oracle's SETTLED result
is <0 -> continue (c45). So the condition is computed correctly but
SAMPLED AT THE WRONG PHASE -- the SAME phase-collapse root as the WTPC
6-cycle residency and the whole depth-mode campaign. The task-init
loop is phase-dependent THROUGHOUT (WTPC residency + conditional-branch
condition-sampling), so landing the boot needs the PHASE MODEL, not
per-mechanism knobs. +wtpcfix stays as the demonstration that the WTPC
half is understood (write sound per taskrun-test); the
conditional-branch sampling is the next instance of the ONE root, not
a new bug. DEFINITIVE CONCLUSION of the task-init investigation: it is
the depth/phase model's territory, holistically -- the same core the
2026-08-29..31 depth campaign built the per-net buffer-depth lag for.

**And the WTPC WRITE MECHANISM IS PROVEN SOUND IN ISOLATION**, which
reframes the write failure: `taskrun-test` PASSES and writes TPC
correctly ("TPC[15] survives task 7", "the startup Link lands in slot
0") -- from JAMMED single instructions. So the h04/k06/k09 access path
works when the instruction is properly controlled; the free-execution
0x368 is a CONSEQUENCE of the collapsed-timing detour (the spurious c61
polluting the loop), not an independently broken write. That means the
phase-ring residency fix would land BOTH the next-address AND the write
-- it is the complete fix, and there is no separate write bug to chase.

A clean bench stand-in is nonetheless blocked by a specific collapse:
in the one-cycle model Link serves DOUBLE DUTY in the same cycle --
write-data (to TPC) and next-address (via Return'c) -- so it cannot be
overridden for the jump without corrupting the write. Separating them
is exactly what the 6-cycle phase ring does on hardware. And iterating
any fix is slow because the first WTPC is at ~108.9M cycles (task init
runs once, late). **FASTER ITERATION for the next session: a focused
bench that preloads IM and jumps straight to TASKINITLOOP (real 0o6105)
with T seeded as the loop counter and the task-init PC in RM -- the
exec-world9 +start= mechanism -- reproduces the WTPC-in-a-loop in
thousands of cycles instead of 108.9M.** (The older note below --
"the WTPC access itself is wrong ... a hold-only stand-in cannot land
it" -- was measured in the broken state and is superseded by the
taskrun-test proof that the access is sound.)

Superseded: the WTPC access itself is wrong, not just the next-address: the
TPCDUMP mid-spin shows every slot holding 0x0368, where the oracle
writes 0o6141 (=0xc61) to tasks 1-15. So a hold-only stand-in cannot
land the boot -- the 6-cycle access (RSTK-addressed TPC write of Link,
h04/k06/k09 gated by Phase2/Phase3) needs the phase model to run
correctly, not merely to be held. The tractable next step is the
depth/phase model
for the TPC/IM residency specifically -- make j05's StartCycle
suppression hold the phase ring for the op's real duration -- which is a
scoped instance of the general phase-ring problem rather than the whole
thing. A bench stand-in is possible
but entangled with the BDispatch, so it will not cleanly land the boot
alone. The probe kit (TSW with WTPC'/RTPC'/Return'a/GetTLink'/RepeatCur/
Phase) is in tb_exec.

**THE (superseded) one-line gate summary: ContA e21 (MC10210) decodes
`Return'` from JCN[0], JCN[1], JCN[5:7] ONLY -- never JCN[2:4]** -- so it CANNOT tell Write-TPC from
Return. On real hardware that distinction is the **6-cycle TPC/IM
addressing flow** (HM §4.8, Figure 7, "these take 6 cycles and use the
RSTK field as a sub-decode"), which overrides the next address; our
control section does not run that flow in free execution (taskrun-test
writes TPC correctly, but from JAMMED single instructions, not the
flowing 6-cycle sequence). So the fix is to bring up the 6-cycle
Read/Write TPC/IM choreography in ContA -- or, as a bench stand-in
matching every other knob this session, force the Write-TPC successor
to CIA+1. That is THE task-init blocker, isolated to one instruction
and one gate. Everything below is how the park that preceded it fell.

**FINAL STATE OF THE PARK DIAGNOSIS (2026-09-01, end of session).** Two
more measurements overturned the stored-complement claim (retracted:
the MAPCENSUS-vs-readback contradiction was the MosRam's dout HOLDING
between reads -- a probe trap, not a convention) and closed the chain:

- **The map READ path is CORRECT.** MAPRAM trace at the miss: RAS'a and
  CAS'a fall, addr=0000, dout_r takes mem[0], dout delivers it, the
  THi-strobed MC10124 puts it on RP (non-inverting 'o' output for
  RP.00). The identical RP=ff readback under opposite seeds happened
  because the read hits the ONE entry the machine itself had written
  (index 0 -- the MAPCENSUS's single flipped bit in each plane).
- **So the miss translates through INITIAL'S OWN Map<- write**, which
  stored WP=0, Dirty=1, RP=11111111. A map-reset should store VACANT
  (WP=1 AND Dirty=1, HM p.45). Stored-vs-intended differs in the
  pattern of the KNOWN B-bus inversion ("B carries the COMPLEMENT" --
  why PARC has SetCPReg and SetCPReg~): if the map write data rides the
  BMux complement and the write path stores it uncorrected, Initial's
  RP=0/WP=1 becomes RP=ff/WP=0 -- module 3, resident -- exactly what
  reads back. The Dirty bit reading 1 either rides a different
  translator sense (e10's 'o' vs 'OUT') or a different plane than
  assumed (census: d13 carries the single 0, d11 none -- so d13 is
  likelier the WP plane, d11 the Dirty).
- **NEXT, one focused fit** (the im-parity-check method, fourth
  instance): trace the Map<- WRITE data path (Mapbuf b01 loads from
  BMux) bit-for-bit against memory.c's map-entry layout, and settle
  which side of the B inversion each stored plane is on. When the write
  path stores what Initial means, entry 0 reads vacant, ValidMapFlt has
  something to detect, and the module field points at real storage.
- Also verified on the way: the map RAM is strobed once per translation
  (RTMapRAS'/CAS' a- and b-side); the vacant detect (g14/MapTrouble)
  correctly does NOT fire for the WP=0 entry -- no-fault is right given
  the stored content. The defect is upstream, in what got stored. The full probe kit is in tb_exec (MAPWEDGE, DRAMWIN,
TRANSPORT/TRANSPORT2, CFLAGS, VICWIN, W0STROBE, +stperroff).

**The original blocker text (still true as far as it goes):** after map init and the module
scan (155 addresses, 294k stack ops), the world run parks 1.28M
microinstructions at RESETFAULTINFO's `B<-FaultInfo'` (FF=0o160, next
address c01=DORETURN) under **BLretry 41M + HoldMapBuf 23M** -- a
storage reference retrying forever keeps the MapBuf busy, and
FaultInfo' shares the MapBuf (memory.h: "Map<- shares MapBuf with
LoadMcr, ProcSRN, and LoadTestSyndrome"). PrHoldReq is 32 samples all
run -- the stack story is CLOSED. Find which reference retries and why
its DRAM cycle never completes; `tb_memrun`/`tb_readback` own the
machinery for that question. VSync is a MICROCODE-driven register
(DispY d03, VSyncEn, DWT task) -- a full frame with real vertical
timing needs the display task's field program, downstream of
TASKINITLOOP.

**Known-stale gates (pre-existing, NOT this session), BISECTED:**
exec-tasking (and exec-init) fail at the pre-session commit fe0c1874.
Worktree runs, each self-consistent: **GREEN at 4f32aa1d** ("exec suite
green" was real), FAILING at f1fa2e34 -- and the ONLY commit in that
range touching cells/ or generated/ is **d3e337a2, the MC10176 pre-edge
setup-time capture**. The same change that made Initial pass its
checksum broke the AEmu jump-start choreography (failure shape at 4c:
held forever with tasking on; at fe0c1874: fault task never runs). The
capture is load-bearing for the boot, so the fix is re-diagnosing the
jump-start scenario under it, not a revert. **memrun-test is ALSO
stale-green** (`preStartMem' never asserted`): its inputs -- the memory
boards, ContA/ContB, ProcH/ProcL, tb_memrun -- are byte-identical to
pre-session (0 changed lines each), so it fails deterministically with
pre-session RTL and is not a regression of any recent session; likely
the same d3e337a2/MC10176 class since it uses that part throughout.
Separately: `SYSPER=2` breaks the ReadBB/checksum stage (RMINITL never
reached); boot runs stay at 16x.

**Regeneration is clean (2026-09-01 regression sweep).** After the
JunkTW jumper regenerated every board, `make -C verilog lint` is 16
boards + all 12 machine tops clean (the lint skip-list was missing
world/screen/disk/display -- fixed, they lint with boards), and
boot0/compute/task/refdecode/taskrun/exec/exec-parity/mir-diff/ifu/
im-parity-check all PASS. Only the pre-existing stale-green gates above
fail. NOTE: several gates leave a STALE binary when the repo path moves
(they errored `No rule to make target .../obj_*/Vtb_*__pch.h`); `rm -rf`
the obj dir and rebuild. And im-parity-check is a `make -C verilog`
target, not a verilator-dir one.

**Traps added today:** a bench force can destroy the very data under
diagnosis (the clip ate three sessions of strobe-phase work); an ALL-ONES
bus read through a 2-bit probe looks like armed data; `+mb0`,
`+addrs` (print visited past 64), `+trwin=N` (steady-state trace),
`+tclen` (widen tcwin), STKADJ/STKWIN probes.

---

## The 2026-08-31 state (history; superseded above)

Everything below the rule is the 2026-08-18 state and still true as
history. The CURRENT frontier is the BOOT CAMPAIGN of 2026-08-29..31,
whose complete live state -- established chain, next probe, traps -- is
maintained in TASK #48 (the task list) and in the last ~40 commit
messages on `verilog-backplane`, each of which is a self-contained
measurement log. Summary:

**DEFAULT MODE (the committed generation; every core gate green):**
PARC's Initial BOOTS deep into its own initialisation on `dorado_world`:

    make -C verilog/verilator exec-boot EXECARGS="+fastwait +bbword=0000 +bbfeed"

runs the ReadBB handshake (data byte first, TAG byte last -- the sync
bit is the MSB of the high byte), passes Bootstrap's checksum, executes
the self-modifying ALUFM init, RMINITL, STKINITL, IFUMINITL, loads
DisHold (no hold ever asserts), beats LongWait (`+fastwait` pulses the
ALU=0 condition at LWRETN c0a -- the T-file clip loses to the bypass),
and parks at Initial's FIRST STORAGE REFERENCE (a Store at image 253,
RefHold+BLretry) which should be SUPPRESSED by the MCR's noRef bit --
the bit LoadMCR #2 misses because its strobe fires one window after its
data (the LoadMCR saga; see the commits from d3e337a2 forward).
Key fixes that got here: the MC10176 pre-edge/setup-time capture
(d3e337a2), the clk1-family lag, the byte-order fix in the sender.
The DISPLAY is proven separately: `exec-screen` (dorado_screen = world
+ DispY) generates real HSync/video from PARC's own HRam table under
three bench forces; Initial's own INITHRAM (real 0xe20) replaces them
once the boot gets there. MILESTONE addresses (visited[] printed every
run): RMINITL c42, STKINITL c4a, IFUMINITL c65, TASKINITLOOP c45,
BOOTEMULATOR c92, INITHRAM e20.

**DEPTH MODE (opt-in: `DORADO_CLOCK_DEPTH=1 make -C verilog
generated-boards`; REGENERATE DEFAULT AFTERWARDS before committing
generated/):** the principled timing model -- every clock net lagged by
its BUFFER DEPTH from the board's CLK input (BaseBd j02 proves all
boards get ONE identical pulse per microinstruction; a board's phase
ordering lives entirely in buffer depths, which zero-delay erased).
Bring-up ledger: manifolds ✓ freeze mechanism ✓ (StopMIRClk is the
MIR-WRITE ENABLE and hold; armed by the entropy poke -- IMRH forced 1
across startup, since Verilator's zero IM has VALID parity)
choreography ✓ exclusions ✓ (RunClk/CPStrb/TCPBus are TTL-side, out of
the tree) idle-level init ✓ | OPEN: the jam's Link load. THE PREMISE
REFUTATION (be7c5d3f): a WORKING default jam shows NO SetRun, NO phase
pulses -- the jam was never stepped execution; Link loads via the
frozen MIR's FF decode + an always-live clock on the Link register,
and THAT clock is what depth broke. Next: find the Link regs' clock
(ContA a13 region), its lag depth, trace both modes
(`make boot0-test BOOT0ARGS=+jamtrace`, 1200-sample window).

**THE TRAP LIST** (each cost a session): mbdis/im_image image indices
print OCTAL; the TC/CKT pc column is the NEXT address; ff prints HEX
(0x56 = 0o126 = LoadMCR); Mcr_u_' is ACTIVE LOW; alua is the primed
bus (= ~pd); MC10159 INVERTS; `BSel=2/6` means 2 OR 6; j2 vs n_cyc2
time bases differ by the bench startup (~15k); broad clock-family lag
parks the machine at f40; a knob's announce line can print while its
logic is ifdef'd out; the prime is INTERIOR in raw net names
(endswith() lies); package-level BFS overclaims; CHECK THE PREMISE
AGAINST THE WORKING MODE FIRST.

---

2026-08-18. This file is written to be picked up cold: "Where it stands" is
the state of the machine, "START HERE" is the open work, and "How to read
the rest" maps the sections below. Read `docs/verilog-from-sil.md` for the
background and why each decision was made.

---

## Where it stands

**A four-board Dorado runs microinstruction cycles, executes microcode out of
IM, and COMPUTES.** The write path and the datapath are both proven end to end
against the C emulator. Parity is the one thing left in the boot chain. Everything is
generated from PARC's own Sil wire lists, and every claim below is a gate you
can run.

### The boot chain, rung by rung

| rung | state | gate |
|---|---|---|
| BaseBoard 6502 boots from its own EPROMs | **works** | `baseboard-test` |
| BaseBoard drives the control-processor bus | **works**, matches the C emulator | `cpreg-diff` |
| All 36 microinstruction bits decode | **works**, matches the C emulator | `mir-diff` |
| A microinstruction lands in the MIR | **works**, matches the C emulator | `mirreg-diff` |
| The Control section executes cycles | **works** | `run-test` |
| Four boards, microinstruction on the datapath | **works** | `datapath-test` |
| A jammed Write-IM deposits into IM | **works** (mechanism + half-select) | `writeim-test` |
| ...with the DATA from CPReg | **works** | `operand-test` |
| ...with the ADDRESS from CPReg | **works** -- Link -> TNIA -> the array's address lines | `operand-test` |
| The machine SINGLE-STEPS microinstructions | **works** | `step-test` |
| PARC's SendViaMIR loads words into IM | **works** -- Boot0's inner loop | `sendmir` |
| PARC's BLOCK LOADER walks REAL MICROCODE into IM | **works**, and it MATCHES THE C EMULATOR -- 16 hunks, 128 half-words | `boot0-test` |
| **THE MACHINE EXECUTES MICROCODE OUT OF IM** | **works** -- free-running, sequencing, 1,242 cycles | `exec-test` |
| **THE MACHINE COMPUTES** | **works** -- Q takes 25 octal from CPReg, holds it, ALUFM[0] stores it | `compute-test` |
| ...and T loads through the ALU | **works** -- 1234 gives 1234, a55a gives a55a, exact | `compute-test` |
| **TWO OPERANDS: the ALU as the BOARDS wire it** | **works** -- all 24 entries of HM Table 9 match the C emulator, carry chain and all | `compute-test` |
| **RM, the per-task register file** | **works** -- four addresses written and read, and each lands where the address pins say | `compute-test` |
| **the MEMORY section is in a machine and clocked** | **works** -- seven boards; each memory board's local clock follows its `MemClkEnable'` | `mem-test` |
| the REAL firmware runs the machine | boots itself past power-up and pacifies its watchdog; reaches `SetManifold`, then ends up executing filler ROM | `firmware-probe` |
| **A WHOLE WORLD PRELOADS INTO IM** | **works** -- 2,148 microinstructions of AEmu.mb, every one read back | `exec-world` |
| ...through a field map DERIVED from the wire list | **works** -- 65 addresses, all 8 fields, against a CP-bus load | `boot0-test`, `im-map-check` |
| **ALUFM**, the sixteen ALU operations the world uses | **works** -- derived from ProcL e13/e14, confirms HM Table 11d | `exec-world9` |
| **IFUM**, the opcode decode tables | **works** -- 256 entries, layout confirms cpu.c's Table 20 bit for bit | `exec-world9` |
| **NINE BOARDS DISPATCH AN ALTO OPCODE** | **works** -- START, the IFU traps, RESTARTIFU, then AND1 and SKPC | `exec-world9` |
| ...and then it STOPS FETCHING | **open** -- `IfuMemRef` makes 6 transitions and the opcode changes ONCE | `exec-world9` |
| **IM PARITY, the long-open question** | **ANSWERED** -- ODD over the 17-bit half, array stores the COMPLEMENT | `im-parity-check` |
| **THE MACHINE RUNS WITH IM PARITY ENABLED** | **works** -- Error propagated on 0 of 400,000, Stop never set | `exec-parity` |
| **THE FAULT TASK SERVICES THE STACK UNDERFLOW** | **works** -- RepeatCur 0, task 15 runs, 25 addresses, longest run 3 | `exec-tasking` |
| **THE FAULT TASK RUNS ITS OWN HANDLER** | **works** -- TPC[15] set, task 15 runs BEGINENUMERATEMAP / IWRITEMAPFLAGS | `exec-init` |
| **PARC'S BOOT CHAIN RUNS** | **works** -- Initial calls Bootstrap's READBB, which waits for the control processor | `exec-boot` |

### Every gate

```
make -C verilog boards          16 boards + 4 machine configurations -> generated/
make -C verilog proms           proms/*.mem and the per-package images
make -C verilog cells           cell skeletons for a new part (never overwrites)

make -C verilog lint            every board and all four machines elaborate
make -C verilog case-variants   BACKPLANE_CASE_ALIASES re-derived from the .bp
                                files: merge a case variant only where every
                                board using it agrees on the pin
make -C verilog cell-check      cells wire the inputs PARC's [G] lists say they
                                do, and a both-sense gate's two outputs are
                                complements of each other
make -C verilog loop-check      no combinational feedback beyond the known ones
make -C verilog prom-test       PROMs read back what the machine expects
make -C verilog alu-test        the ALU matches its datasheet
make -C verilog alu-diff        the ALU matches the C EMULATOR, 10,752 vectors
make -C verilog cpreg-diff      the BOOT INTERFACE matches the C emulator
make -C verilog mir-diff        all 36 microinstruction bits match
make -C verilog mirreg-diff     a jammed microinstruction reads back off the MIR
make -C verilog run-test        THE MACHINE RUNS -- it executes cycles
make -C verilog datapath-test   four boards, microinstruction on the datapath
make -C verilog writeim-test    a jammed Write-IM deposits into IM
make -C verilog operand-test    ...with its DATA and ADDRESS from CPReg
make -C verilog step-test       the machine SINGLE-STEPS microinstructions
make -C verilog sendmir         PARC's SendViaMIR loads words into IM
make -C verilog boot0-test      PARC's BLOCK LOADER walks real microcode into IM
make -C verilog exec-test       THE MACHINE EXECUTES MICROCODE OUT OF IM
make -C verilog compute-test    THE MACHINE COMPUTES -- PARC's ALU prologue:
                                25 octal from CPReg into Q, held, then stored
                                into ALUFM[0]
make -C verilog baseboard-test  the BaseBoard's 6502 BOOTS
make -C verilog muffler-test    exactly one address selects the BaseBoard
make -C verilog strap-test      each board's SIP straps give the I/O address
                                and task number its schematic states
make -C verilog osc-test        the three crystal positions run 1 : 2 : 5
make -C verilog task-test       the TASK PRIORITY ENCODER matches the C emulator
make -C verilog taskrun-test    BNT loads in a running machine, THE MACHINE
                                SWITCHES TASKS, and each task keeps its own
                                PC and Link
make -C verilog refdecode-test  the MEMORY SECTION's front door -- ASEL 0-3 is
                                a storage reference, against the C emulator
make -C verilog memrun-test     THE MEMORY RUNS: DRAM cycles from PARC's own
                                startup, the microcode makes a Flush, a Store
                                dirties a line, THE CACHE MISSES, <-Map writes
                                an entry and MapTrouble clears, a storage cycle
                                starts
make -C verilog mem-test        the memory section is in a machine and clocked
make -C verilog msa-test        A WORD GOES INTO THE STORAGE ARRAY: the msa
                                board standalone, where the bench owns the DRAM
                                strobes -- Sout registered, translated to TTL,
                                and written into the MK4096s
make -C verilog ifu-test        THE IFU is in a machine: IFUM is 27 x F10415A =
                                1024 x 27 bits (256 opcodes x 4 instruction
                                sets, HM Table 18), its slot clock reaches it,
                                and IfuData resolves
make -C verilog ifufetch-test   THE IFU COMES OUT OF HOLD when real microcode
                                runs -- released on 3000 of 3000 samples,
                                where the static bench holds it
make -C verilog storage-test    THE STORAGE ARRAY is in a machine: PARC's msa
                                board with its 144 MK4096 DRAMs, its address
                                path inverting through the SN74H04s, and the
                                memory size arriving over pin E55 from the
                                board rather than from the testbench
make -C verilog converge-test   the assembled machine SETTLES and its clock runs
make -C verilog machine-test    the same under the imgui harness

make -C verilog startseq        DIAGNOSTIC, not a gate: PARC's boot sequence
                                replayed, printing the Control section's state
make -C verilog parity-probe    DIAGNOSTIC, not a gate: do PARC's IRTable
                                entries satisfy our IM parity generators? (no)
```

Plus, outside the Makefile: `tools/dorado_proms.py --check` (26/26 PROMs
property-checked), `--placement` (which package holds which PROM), and
`tools/sil_backplane.py` / `--ports` (what the backplane is, measured).

### The machine configurations

`tools/sil_backplane.py` wires any subset of boards by name. `make boards`
emits six:

| module | boards | used by |
|---|---|---|
| `dorado_backplane` | the eleven of a working monochrome machine | `machine-test`, the imgui harness |
| `dorado_baseboard` | BaseBd alone | `baseboard-test` |
| `dorado_control` | ContA + ContB | `run-test`, `mirreg-diff` |
| `dorado_proc` | ContA, ContB, ProcH, ProcL | `datapath-test`, `writeim-test`, `operand-test` |
| `dorado_boot` | BaseBd + ContA/B + ProcH/L | `firmware-probe` |
| `dorado_mem` | ContA/B, ProcH/L, MemC/D/X | `mem-test`, `memrun-test`, `refdecode-test` |
| `dorado_storage` | `dorado_mem` **+ msa** | `lint` -- the storage array in a machine |

**THE MSA DID NOT NEED WRITING, IT NEEDED WIRING (2026-08-23).** The task
list said "write the behavioural MSA storage array"; that premise was wrong.
`chm/sil/msa-Rev-Bg.dm!1_/msa.wl` is PARC's own storage-array board and the
generator has been emitting `generated/msa.v` all along -- 291 packages, 768
nets, **144 MK4096 DRAMs** (4K x 1), and every part type it uses already had a
cell. What was missing is that no machine configuration instantiated it.

It hangs off the memory section exactly as it should: of its **66 backplane
nets**, 38 are shared with MemD, 23 with MemX, 4 with MemC, and **zero** with
ContA/ContB/ProcH/ProcL. The MemD interface is the storage data path itself --
`Sin.00-15` and `Sout.00-15`, the 16-bit word in and out, plus `EcIn`/`EcOut`
for ECC.

**And two things had to be fixed before it could work.**

**The coverage check compared the wrong name.** `known_cells` reads part names
off the FILENAMES, which are already sanitised (`cell_MK4096P_6.v`), while the
check used PARC's raw name (`MK4096P-6`). So any part whose name needs
sanitising was marked "NO MODEL" in the generated Verilog -- and counted as
missing -- while being correctly instantiated all along. **154 false comments
across 8 boards, 144 of them the MSA's DRAMs**, which have a complete model.
Fixed by comparing `vpart(ptype)`.

**`SN74H04` was a skeleton, and it is what drives the DRAM address lines.**
All 28 of its packages are on the MSA. The board takes its multiplexed
address off the backplane as `TtlA0'`..`TtlA6'` and inverts it here into
`A0a0`..`A6a0`, which fan out to the 144 MK4096s -- so unmodelled, **every
address line on the storage board read 0 and no location could be selected**.
The netlist proves the function without a data sheet: every package takes a
PRIMED net in and drives the UNPRIMED one out, in the standard 7404 pin pairs.

With both fixed, **the MSA is 265/265 logic packages modelled -- 100%**.

**The storage data path, mapped end to end (2026-08-23).** One bit followed
through the wire list, which is what `storage-test` and any future write test
have to sequence:

```
Sout.00-03  -> b01 D0-D3   (MC10176 hex D FF, clk msa01.sil+4)
            -> msa04/05.sil+*  -> the SN74166 shift chain
            -> f01 (MC10176, clk msa01.sil+8, ECC on EcOut.0/4)
            -> f02 (MC10125 ECL->TTL)  -> MK4096 pin 2, DIN

MK4096 pin 14, DOUT -> ... -> c01 D-inputs
            -> c01 Q (MC10176, clk msa01.sil+3) -> Sin.00-03, EcIn.0
```

**`Sout` is the write side and `Sin` the read side** -- the MSA drives `Sin`
while MemD drives `Sout`, which is worth stating because the names do not give
it away. And the word is **not presented in parallel**: it is registered, then
shifted.

**A WORD NOW GOES INTO THE ARRAY (2026-08-23).** `msa-test` drives the board
**standalone**, where the bench owns the DRAM strobes -- `storage-test` cannot,
because in a machine they come from MemX and need a real memory reference. The
sequence is: present `Sout`, pulse `LoadSoutE'` with `Mod0StrEn'` low to
register it, then a DRAM cycle -- row on `RAS'` falling, column on `CAS'`
falling, `WE'` low for an early write. Measured:

```
Sout -> b01 Q (msa04.sil+32/33) = 11 -> b02 (msa04.sil+8) = 1 -> b05 DIN = 1
writing 0000 -> DIN=0, 0 cells set        writing FFFF -> DIN=1, 1 cell set
```

Exactly one cell, at one location.

**AND IT COMES BACK.** The read path is the shifted one, and the '166 model
loads `q[7]` from pin 2 -- which is b05's DOUT -- so the bit is at the serial
output the moment the load happens, no shifts required:

```
b05 DOUT=1 -> SLa=0 (load) -> a13 Qh (msa04.sil+29)=1
           -> c02 (msa04.sil+31)=1 -> Sin.00=1

round trip: wrote 1, read Sin.00=1 ; wrote 0, read Sin.00=0
```

The zero case is there so the gate cannot pass on a stuck-high output.
Mutation-tested four ways -- breaking the MC10125 translator, disabling the
MK4096's write, disabling its read, and stopping the SN74166 loading -- all
caught.

**`LoadSinE` goes low then high.** The other way round leaves `SLa` high, the
'166 stays in **shift** mode and never takes the DRAM outputs, and the read
returns 0 while `b05`'s DOUT is plainly 1 -- which reads as a dead read path
and is a polarity mistake in the bench.

**Probe a DATA bit, not an ECC bit.** The first attempt watched `u_b04` and saw
a permanent 0 that looked like a dead write path. b04 is an **ECC** DRAM, fed
from f01/f02 off `EcOut`, which the bench was holding at 0. `u_b05` is the data
bit carrying `Sout.00`, and it was working all along. The two sit side by side
in the same package row.

**And the two sides are not symmetric -- the shift registers are on the READ
side only.** An SN74166 is parallel-in / serial-out (pin 15 SH/LD', pin 7
clock, pin 13 serial out), and a13 takes its eight parallel inputs **straight
from eight MK4096 DOUT pins** (`msa04.sil+1` is b06 pin 14), then shifts them
out through c02 (MC10124 TTL->ECL) into c01, which drives `Sin`. Twenty of
them at eight bits each is 160 lines for the 144 DRAMs plus ECC.

The **write** side has no shift register at all: b01 registers `Sout` and b02
(MC10125 ECL->TTL) drives the DRAM DIN pins directly. **So getting a word in
is a register-and-strobe; only reading it back needs the '166 load-then-shift
sequence.** That asymmetry is the useful fact for the next rung -- the hard
half is the read.

**Those register clocks are the board's own -- and the board needs its slot
clock.** b01 clocks off `c1`/`c2` and c01 off `SO`, and all three are internal
nets driven by MC10176/MC10210 packages on the MSA itself: an on-board
sequencer fed by MemX's `LoadSinE`/`ShiftSinE`/`LoadSoutE'`/`ShiftSoutE` --
**but also, and first, by `CLK.ms0Even'`, the MSA's own slot clock.**
Everything on that sequencer traces back to it.

**A first version of `tb_storage` did not drive it**, measured 0 edges on both
registers, and concluded the board was correctly quiet until a reference ran.
It was not: the storage board simply had **no clock**, and an unclocked board
makes every one of its signals look properly gated. That is the same trap
`dorado_mem` sets with `CLK.mc'`/`CLK.md'`/`CLK.mx'`, one board later. Driven,
the registers take **1250 and 2500 edges**, and the gate now asserts edges
rather than mere definedness. Mutation-tested: tying the slot clock low is
caught.

**And wiring it in found a seventh two-spelling backplane line.** MemX spells
it `ChipsAre64K`, the storage board `ChipsAre64k`, and both sit on pin **E55**
-- one wire, and the case rule applies. That is the very signal `tb_memrun`
has to drive by hand because "there is no MSA in this configuration".

Its neighbour is **recorded but deliberately not merged**: MemX's
`ChipsAre256/16K` and the MSA's `ChipsAre16k` are both on **E54** and are
plainly the same signal, but they differ by more than capitalisation, and this
table's discipline is that a NAME is the connection. PcMsa puts a *third* name
on the same pin (`ChipsAreA`), which is what a pin-based merge would have to
reconcile.

**The table now checks itself.** Its comment had promised
`sil_backplane.py --case-variants` long before that flag existed; it exists
now, is a make target, and re-derives the whole table from the `.bp` files --
10 case-variant groups, 7 agreeing on a pin (exactly the table), 3 not. It
must read the `.bp` files **raw**: `load_backplane()` returns names already
through `canon_net`, so a first attempt using it pronounced all seven aliases
stale, including `PrHold` -> `PRhold`, which the wire lists document with three
pins of evidence.

**And it corrected the comment it was written to confirm.** The old text said
`IOIn'` (E70) vs `IOin'` (E71) and `IOOut'` (E71) vs `IOout'` (E74). Measured
from the files:

```
IOIn'   DispM, DispY            E71
IOin'   DskEth, Music           E71   <- SAME pin as IOIn'
IOin'   ProcL                   E70   <- the odd one
IOOut'  DispM, DispY            E74
IOout'  DskEth, IOTest, Music   E74   <- SAME pin as IOOut'
IOout'  ProcL                   E71   <- the odd one
```

The spellings mostly **agree**; it is **ProcL** that sits on different pins
from everyone else. The group still must not be merged -- three pins are in
play -- but not for the reason given.

`make -C verilog backplane MACHINE=--boards=ProcH,ProcL` builds any other.

| piece | state |
|---|---|
| Netlist reader + Verilog generator | 16/16 boards, all lint clean |
| Cell library | **97** cells with behaviour, covering **97.7%** of the eleven-board machine's logic packages |
| 6502 | Andrew Holme's netlist-derived core (via jotego), in `cell_MCS6502` |
| 6532 RIOT | MiSTer Atari 7800's. **CC BY-NC** -- see `verilog/vendor/LICENSES.md` |
| PROMs | **26 of 26** generated, 29 packages wired in and reading back correctly |
| Board port lists | **from PARC's own `.bp`**, 1,920/1,922 exact, 0 spurious |
| Synthesisability | **no `inout`, no multiply-driven net, no gated clock** |
| Harness | Verilator + Dear ImGui, `--headless` CI mode |

Of the 64 packages still unmodelled, **42 are analog or mixed-signal** (op-amps,
comparators, a DAC, a transistor array, an ADC, analog muxes) and belong as
substitutions if at all. The 22 digital ones need datasheets this repository
does not hold: `F100181` (8, the MemC ALU), `MC10163`, `MC10182`, `MC10179`,
`F9401`.

## A WHOLE WORLD IN IM, and the machine dispatches an Alto opcode (2026-08-26)

`make -C verilog exec-world9 EXECARGS='+cycles=200000'` preloads all three
memories a microcode world needs and runs it on nine boards:

```
tb_exec: PRELOAD -- 2148 microinstructions written, 2148 read back, 0 wrong
tb_exec: PRELOAD -- 16 ALUFM entries written, 0 wrong
tb_exec: PRELOAD -- 256 IFUM entries written, 0 wrong
tb_exec: IFU -- IfuMemRef 6 transitions, IfuMemAck 6, opcode J changed 1 times
tb_exec: 14 distinct IM addresses executed; last TNIA=0fc; longest run on one address=6233
```

**Why preload at all.** The control-processor bus is the REAL loader and
`boot0-test` gates it; but sixteen hunks already cost a couple of million
fabric cycles, so 4,096 microinstructions of it will not simulate. Preloading
skips nothing unproven -- provided the map is right, which is the point of the
next paragraph.

**The map is DERIVED and GATED.** `tools/sil_im_map.py --verilog` emits
`verilog/generated/im_preload.vh` from ContB's, ProcL's and the IFU's own wire
lists. `boot0-test` then checks it against a control-processor-bus load of the
same microcode: 65 addresses, all eight fields, 0 wrong. Three mutations of the
generator are caught -- a swapped bank ordering (23 wrong), LSB-first field
numbering (50), and using the whole address as the index (49).

### The three memories

| memory | where | size | how the layout was fixed |
|---|---|---|---|
| IM | ContB, 144 F10415A | 4096 x 36 | bank = `{addr[11], addr[0]}`, EVALUATED from the CS gate polarities |
| ALUFM | ProcL e13/e14, 2 F10145A | 16 x 6 | data pins come off the B bus; confirms HM Table 11d |
| IFUM | IFU, 27 F10415A | 1024 x 27 | two write enables = the .MB's two words; confirms cpu.c's Table 20 |

**Knowing WHICH BITS pick a bank is not knowing which VALUE picks which bank.**
`CS0'..CS3'` are net NAMES; nothing says `CS0'` is the corner where both
address bits are 0. The generator evaluates the two MC10101s' gate polarities
-- d21 puts `RA.11a` through OR into CS0'/CS2' and NOR into CS1'/CS3', d22 puts
`RA.00a` through OR into CS0'/CS1' and NOR into CS2'/CS3' -- and gets exactly
one selecting value per bank.

**ALUFM confirms HM Table 11d from the netlist alone.** `ALUFdec.0..5` are
`B.08, B.11, B.12, B.13, B.14, B.15` in order: the entry's top bit is the ALU's
CARRY IN and is NOT part of a contiguous field. Three independent
confirmations, none derived from another -- `cpu.c` spells out the same
correspondence in LSB order; ALUFM[0] in AEmu.mb is 0x15 = **25 octal**, the
value PARC's own ALU prologue stores in `tb_compute`; and ALUFM[e] is 01 =
"NOT A", HM Table 9.

**IFUM's two halves are two write enables**, `DecHi'` and `DecLo'`, which is
why a .MB stores two words per entry. Comparing bit positions against `cpu.c`
settles which is which, and the agreement is exact and complete: `DecLo'`
carries all sixteen bits of `ifu_decode_lh`'s Table 20 layout (Sign 00, IPar
01-03, Length' 04-05, RBaseB' 06, MemB 07-09, TPause' 10, TJump' 11, N 12-15),
and `DecHi'` carries the eleven bits `cpu.c` masks as `ifum_lo & 0x07FF`
(TwoAlpha 05, InstrAddr 06-15). **The names cross over -- DecLo' holds what
the C emulator calls `ifum_hi` -- and the bits do not.**

### What the machine does, and what it does not

On FOUR boards the world spends all 124,994 microinstructions in the trap
vectors at the bottom of IM. That is correct for a machine with no memory
section and no IFU, not a bug.

On NINE it runs START, passes through `AEMUIFURAMPE` / `AEMUIFUFGPARITY` /
`AEMUIFUMAPFAULT`, reaches `RESTARTIFU`, and dispatches into **`AND1`** -- a
real Alto-emulator opcode implementation. Loading IFUM adds a second,
**`SKPC`**, which is how you know the dispatch through IFUM is real.

**IT IS NOT RUNNING A PROGRAM, AND THE PROBE SAYS SO PRECISELY.** Over 12,500
microinstructions `IfuMemRef` makes SIX transitions and the decoded opcode `J`
changes ONCE. The IFU makes a handful of references at startup and then stops
fetching, so the emulator dispatches on one stuck opcode and loops -- 6,233
microinstructions on a single address. Varying the seeded memory pattern
changes nothing now, where before IFUM was loaded it did.

**IT IS ALSO NOT A HOLD**, which is the obvious first theory and is wrong.
Sitting on one address looks exactly like the memory section freezing the
processor, so the whole chain is printed -- `Hold`, `PRhold`, `CBHold`,
`IfuHold`, `IOHold`, `MXHold`, `DisHold`, `CHoldReq`. All eight are 0 for the
entire run.

### ANSWERED: it was PARITY, twice -- and one of them is the first parity
### agreement between the two models

**The IFU was sitting in a permanent RAM parity error.** `RamPe` high on
200,000 of 200,000 samples, `SawRamParityErr` latched on 199,523.

**THE .MB DOES NOT CARRY IFUM PARITY.** The real machine computes the three
IPar bits in its LOAD microcode -- `cpu.c` cites `ifuRamSubrs.mc:ifuAddParity`
-- and 248 of AEmu's 256 entries FAIL `cpu.c`'s own `ifum_parity_ok` as stored,
which is why the C emulator only checks it behind `DORADO_IFUM_PARITY_TRAP`. A
preload that copies the stored bits hands the IFU 248 bad entries. Computing
them per HM Table 20's three disjoint even-parity groups takes `RamPe` and
`SawRamParityErr` to **ZERO**.

**That is the first parity agreement established between the two models.** The
RTL's parity generators come from PARC's wire lists; `cpu.c`'s grouping comes
from `ifuRamSubrs.mc` and the manual. Neither was derived from the other, and
feeding the RTL a word whose parity satisfies `cpu.c` makes the RTL's error
signal go clean. **IM parity is the same question** -- the machine still runs
only with the enables cleared -- and this is a tractable second instance of it.

**Then FG parity, over the instruction BYTES -- and this one is only PARTLY
settled.** The cache word is 16 data bits plus TWO parity bits, D.16 and D.17,
one per byte. Odd parity per byte -- D.17 over D.08-15, D.16 over D.00-07 --
minimises `FGParityErr` identically (176 residual) across six different
constant patterns: 52045, 49155, 0, 65535, 21845, 4660.

**IT IS NOT ESTABLISHED AS THE HARDWARE CONVENTION, and chasing it is what
showed why.** Three structural hypotheses were raised and all three REFUTED
from the netlist and from measurement:

- *The cache's two columns wire-OR different words.* MemD's a03 and d03 both
  drive D.00 but are separate 4096x1 arrays -- a03 on `Dad0.10-12` with enable
  `D0ACE'`, d03 on `Dad1.10-12` with `D1ACE'`. Refuted by counting: D0 only on
  199,603 samples, D1 only 0, BOTH 0, addresses never differ. They are
  perfectly exclusive.
- *The F/G mux takes the byte from one register and the parity from the
  other.* Refuted structurally: `i22` selects `FG.8` from
  `{G.16, F.16, G.17, F.17}` under the SAME `PcFG.15`/`GDv'` as the eight data
  muxes, so parity tracks both the byte choice and the F/G choice.
- *`G.16`/`G.17` are never loaded.* Refuted: `i21`, an F10016, loads them from
  `F.16`/`F.17` on the same `GLd'` that loads the data bits.

**What finally gave it away is that the winning assignment DEPENDS ON THE
DATA.** With `+cprog` giving each line its own word, the plain rule is clean
for a byte-swapped word (`+cprogfn=3`) and dirty for the plain one, and the
reverse for `+cprogfn=0`; `+cprogfn=1` is nearly clean under both. No real
parity convention can do that.

**So the counter is not measuring what it looks like.** `IfuMemRef` makes TWO
to SIX transitions over a whole run -- there are essentially no fetches -- and
a count of samples where a combinational signal sits high over 200,000 cycles
is an IDLE LEVEL, not a per-fetch check. Contrast the IFUM parity above:
`RamPe` went to EXACTLY ZERO, and IFUM entries are read continuously, so there
the level is meaningful. **A level is not an event. Before believing a sample
count, check that the thing it counts actually happened.**

Settling FG parity needs the error sampled AT a fetch, which needs a machine
that fetches -- so it is downstream of the same question, not beside it.

With both clear the machine reaches three addresses it had never touched --
octal 100, 435 and 464, the last of which `mbdis` names `CVEND` and whose ASEL
is `Fetch<-RM/STK`. It is executing microcode that issues storage references.

**HOW IT WAS FOUND, because the order mattered.** The chain was measured from
the far end inward. The byte path from the cache (MemD `e06` -> `f22` -> `f23`
-> `FG`) MOVES in a running world -- `Fclk'a` 10 edges, `GLd'` 4, `FG` changed
7, where `tb_ifufetch`'s synthetic loop got 0 for all three -- and it stops in
lockstep with `IfuMemRef`. So nothing downstream was broken and memory was not
failing to serve; the IFU had stopped ASKING. The run visits `AEMUIFURAMPE`
early, which named the suspect.

### ANSWERED: it is a HOLD, and the stack pointer is at zero

The chain is measured end to end, every link a counter over 400,000 samples:

| link | evidence |
|---|---|
| `StkP` = 0x00 | 399,435 samples (and 0x3f for 565 at the start) |
| ProcL asserts `PrHoldReq` | **399,435** -- exactly co-extensive with StkP=0 |
| MemC latches `MiscHold` | 399,419, sixteen samples later = one microinstruction |
| `Hold` on the backplane | 399,419, fanned out as PRhold/CBHold/IfuHold/IOHold/MXHold, all the same count |
| ContA f20 makes `RepeatCur` | 399,419 |
| the MIR holds, TNIA frozen at 0x040 | 12,481 microinstructions on one address |

**`StkP = 0` is stack UNDERFLOW.** ProcL j20 (an MC10117) drives `PrHoldReq`
from `StkP.6/7` and the RSTK field, which is the Dorado's stack
overflow/underflow hold: a stack operation that would run off either end holds
the processor instead of corrupting STK.

**And it sustains itself.** MemC c24 ORs `PrHoldReq, ExtHoldReq, CHoldReq,
WantCR'` and **`MXHold`** -- and `MXHold` comes from `Hold`. So once set the
hold feeds its own request. The reset, `DisHold`, is a MODE BIT in a control
register (k08, clocked by `LdMcr'` from the MAR), not a per-cycle clear, so
nothing releases it on its own.

**`MDhold` and `RefHold` are both 0**, so the memory section is not waiting for
data or refresh -- the processor is holding itself.

**THE EARLIER "NO HOLD ANYWHERE, EVER" WAS STALE AND THE METHOD WAS WRONG.**
That reading came from printing the eight hold signals at TEN INSTANTS and
seeing zeros -- true of the machine BEFORE IFUM was loaded and the parity
fixed, and long superseded by the time it was being quoted. Holds are counted
now, not sampled. **Sampling a level at ten points is not measuring it**, and a
conclusion drawn that way needs re-taking whenever the machine changes.

### RESOLVED: StkP = 0 is CORRECT, and the RTL is more faithful than the C emulator

`cpu.c`'s own transcription of HM Table 6 settles it:

> `RSTK[0]`: 0 = no underflow check; 1 = underflow if StkP originally 0 OR
> finally 0. Underflow / overflow set StkUnd / StkOvf flags ... and **would HOLD
> + wake fault task 15 on real hardware -- we just track the flags**.
>
> `StkP[0:1]` selects one of 4 stack regions (each 100 octal words).
> `StkP[2:7]` is the per-region offset, valid range 1..77 octal.
> **`StkP[2:7] = 0 denotes empty stack.`**

So offset 0 is the documented EMPTY-STACK sentinel, not a wrong value, and the
hold is the documented hardware response. The measurements line up exactly:

- StkP powers up at `0x3f` -- region 0, offset 63, the top of the region.
- At cycle 565 it reaches `0x00` -- region 0, offset 0, EMPTY.
- The instruction there carries **RSTK = 8**, i.e. `RSTK[0] = 1`: the underflow
  check ENABLED, which is precisely the condition Table 6 names.
- ProcL j20 gates `PrHoldReq` on `StkP.6/7` and the RSTK field, and asserts.

**Our RTL does what the manual says. The C emulator explicitly does not** --
"we just track the flags" -- which is a deliberate simplification there and a
divergence worth knowing about when the two are compared.

**And the hold was permanent for a reason the bench controlled.** The manual's
response is HOLD *and wake fault task 15*; task 15 is what services the
underflow and releases it. The bench started the machine with `Return#`, which
is `TaskingOff,Return` -- its FF is `0o142` = FA 1, FB 4, FC 2, which `cpu.c`
names TaskingOff -- so the fault task could never run.

### AND WITH TASKING ON, IT DOES

FC 3 is TaskingOn. Re-encoding `Return#` with FF = `0o143` through the encoder
that reproduces all eight of PARC's IRTable entries byte for byte gives
`70 13 E1 4A 43`, parity included, re-decoding to the same fields with only FF
changed. `make -C verilog exec-tasking`:

| | tasking OFF | tasking ON |
|---|---|---|
| `RepeatCur` | 399,419 of 400,000 | **0** |
| CTask occupancy | t0 = 400,000 | t0 = 181, **t15 = 399,819** |
| distinct IM addresses | 15 | **25** |
| longest run on one address | **12,481** | **3** |

The fault task runs, the hold goes away, and the machine sequences instead of
freezing. And what it executes are real Alto opcode handlers -- `mbdis` names
`LDAIZ`, `JMPI2`, `DOCRYS`, `SKP1C`, `TRAP17` and `JSRIIX`, the last of which
has ASEL = `Fetch<-T`, a genuine storage reference.

**This is the manual's mechanism working end to end in generated RTL**: a stack
operation on an empty stack holds the processor, wakes the fault task, and the
fault task clears it.

### But the fault task is requested from RESET, and never released

`TWReq.15` -- MemX's fault-task wakeup -- is high on **400,000 of 400,000**
samples. So with tasking on the machine switches to task 15 at cycle 181 (as
soon as TaskingOn takes effect; HM p.27 gives it a two-instruction delay) and
never comes back:

```
PER TASK -- t0:2 addresses  t15:23 addresses
```

**Task 0 executes TWO instructions.** And the 23 addresses task 15 executes are
ALTO EMULATOR handlers -- `LDAIZ`, `JMPI2`, `DOCRYS`, `SKP1C`, `TRAP17`,
`JSRIIX` -- not a fault handler, which means `TPC[15]` was never initialised and
the fault task is running whatever code it landed on, in a 23-address cycle.

**Two candidate causes tested and BOTH ELIMINATED:**

- *An empty Map entry failing its parity check.* `tb_readback` documents that at
  length as the cause of `MapTrouble`, so its 21-array MemX map seeding was
  lifted into this bench. `TWReq.15` is unchanged: 400,000 of 400,000 with the
  seed and without it (`+nomapseed` keeps the control).
- *The wrong entry point.* AEmu has named entries -- `START` 0, `STARTMB` 53,
  `AEMUNEXT` 51, `BOOT` 652, `STARTEMULATOR` 656 (octal) -- and `BOOT`/
  `STARTEMULATOR` looked far more plausible than 0. Sweeping all five with
  `+start=`, **0 is the best of them** (25 addresses, longest run 3); every
  other lands back in the 0x0c4 spin with 12,490+ on one address. START is the
  right entry.

### FOUND: `Faults` is a TERMINAL COUNT, and the counter sits at 0000

MemX h03 is an MC10104 (quad AND) wired

    TWReq.15 = (Faults & WakeEnable) | (WakeEnable & StkWake)

`StkWake` -- the stack wakeup HM Table 6 names -- reads **0** all run, so it is
the `Faults` term. And `Faults` is MemX **k09 pin 4**, which on an F10016 is the
**TERMINAL COUNT** output: `~(&q)`, LOW only at `1111`.

Its four parallel inputs are all `TrueBD`, a hardwired true. So the design is:

    LOAD 1111  = no fault          (PE' = `_FaultInfoDly'`)
    COUNT      = a fault reported  (CE' = `ReportFault'`), reaching 0000

**`Faults` is high because the counter is at 0000, not because a fault is
currently happening.** Measured: it reads `0001` by the time the run starts, so
ONE FAULT IS REPORTED DURING STARTUP -- and nothing ever reloads it, because on
real hardware the fault task reads the fault info and the load re-arms it, while
here the fault task runs emulator code (`TPC[15]` was never initialised).

**Presetting it proves the chain.** `+faultinit` sets k09 to `1111` *after*
startup -- an `initial` is too early, the counter counts away from it:

| | default | `+faultinit` |
|---|---|---|
| `TWReq.15` | 400,000 of 400,000 | **108,939** |
| first task switch | cycle 181 | cycle 661 |
| task 0 addresses | **2** | **20** |
| task 15 addresses | 23 | 10 |
| `RepeatCur` | 399,419 | 72,635 |

The emulator task actually runs. It is not a clean state yet -- the stack
underflow at cycle 565 still holds it intermittently and the fault task still
services it with the wrong PC -- so `+faultinit` is a diagnostic, not the fix.

### And StkP is LOADED with 0, not decremented into it

`+stkinit` presets the stack pointer to region 0, offset 32 -- ProcL l14 is an
MC10176 holding StkP.0-5 as `{p15,p14,p13,p4,p3,p2} = q`, so StkP.n is q[n];
l15 is an MC10231 with StkP.6 = qb and StkP.7 = qa.

The preset takes (the held value goes 0x3f -> 0x20) **and StkP still reaches 0
at the same cycle, 565**. That rules out a decrement: HM Table 6's RSTK[1:3]
adjusts StkP by -4..+3 only, so 32 could not reach 0 in one step, and the change
detector records TWO transitions in the whole run rather than the eight a
walk-down would give. **Something LOADS StkP with zero.**

Which is consistent with a world initialising an empty stack -- and then the
next stack operation with the check enabled underflows, because `cpu.c`'s Table
6 note says underflow fires "if StkP originally 0 OR finally 0". So the world
must raise StkP off zero before it uses the stack, and this bench never gets it
that far.

`+stkinit` alone therefore changes nothing measurable (task 0 still runs 2
addresses); it is `+faultinit` that moves the machine. Both are kept, because
the negative result is what pins the load.

### THE FAULT TASK RUNS ITS OWN HANDLER

When a task is woken it resumes at its own `TPC` -- there is no separate vector
-- so an uninitialised `TPC[15]` makes the fault task run whatever it lands on,
which is why it was executing EMULATOR handlers. AEmu's own handler is
`FAULTTASK`, octal 3747.

ContA i13/j13/k13/l13 are F10145A holding TPC in four-bit slices -- l13
TPC.00-03, i13 TPC.04-07, j13 TPC.08-11, k13 TPC.12-15, with
`q = {q3,q2,q1,q0} = {p14,p15,p1,p2}` -- and TPC.04-15 is the 12-bit IM address,
MSB first.

**AND THE ADDRESS PINS ARE PRIMED**, `TPCAd.0-3'`, so `a = ~task` and `TPC[15]`
lives at `mem[0]`. The same reversal CLAUDE.md records for RM's `~RSTK`. That is
not taken on faith: `+tpcslot=15` writes the un-reversed slot and has **no
effect at all** -- counts identical to not presetting -- while slot 0 changes
everything. The measurement settles it.

`make -C verilog exec-init` arms MemX's fault counter and presets TPC[15]:

| | `+taskingon` | `+faultinit` | `+faultinit +tpcinit` |
|---|---|---|---|
| task 0 addresses | 2 | 20 | 15 |
| task 15 addresses | 23 | 10 | 12 |
| `Hold` | 399,419 | 72,635 | **96** |
| `RepeatCur` | 399,419 | 72,635 | **64** |
| task 15's code | emulator handlers | emulator handlers | **octal 3700-3733** |

Those addresses are the fault-handler region, and `mbdis` names two of them:
**`BEGINENUMERATEMAP`** (3722) and **`IWRITEMAPFLAGS`** (3732), one carrying
`ASEL = Fetch<-RM/STK`. That is a fault task doing its actual job -- walking the
map and writing map flags -- in RTL generated from the wire lists.

**Two gate thresholds were widened, both with the reason measured rather than
assumed.** The microinstruction-clock floor assumed ~one instruction per SYSPER
fabric cycles with nothing stalling; a machine issuing real memory references
legitimately issues fewer, and it falls to 16,676 of a 25,000 ceiling while
`Stop` stays clear and `Hold` is on 96 of 400,000 samples -- work, not a stall.
And the clk0'/clk1' slack went from one to two, because once instructions can
REPEAT a second window boundary lands the same way (16,676 against 16,678). Two
is a property holding; a real divergence is thousands.

### PARC'S OWN BOOT CHAIN RUNS (2026-08-27)

Register-patching goes only so far; `exec-init` pokes three registers, which is
not how a Dorado starts. The real chain is BaseBoard -> Boot0 -> Initial ->
the world, and **the first two stages of it now run in generated RTL.**

`make -C verilog exec-boot`:

```
PRELOAD -- 976 microinstructions written, 976 read back, 0 wrong
addresses executed: f40 fc0 fe1 fe2 fe6 fe7
BOOTCHAIN -- INITIAL(0xf40)=1 READBB(0xfc0)=1, 4 addresses in Bootstrap's poll loop, longest run 0
PARC'S BOOT CHAIN RUNS -- Initial calls Bootstrap, which waits for the control processor.
```

**They are designed to coexist, and the addresses prove it.** `Initial.mb`
occupies real `0xc00-0xfbf` (926 microinstructions) and `Bootstrap.mb`
`0xfc0-0xfff` (50) -- adjacent and disjoint, the top of IM, so a world can be
loaded beneath them. Both load through the same derived field map, 976 written
and 976 read back.

**And the call is checkable arithmetic.** `INITIAL` is image 1 = real `0xf40`,
and `IM[0xf40]`'s JCN is `0xff` -- a GLOBAL CALL. The formula gives

    TNIA = CIA[2:3] || JCN[2:7] || 000000
         = 0b11 || 0b111111 || 0b000000
         = 0xfc0

which is Bootstrap image 54, **`READBB`** -- read BaseBoard. The machine then
cycles images 55/56/57/60 (real `0xfe7/0xfe2/0xfe1/0xfe6`) with **longest run on
one address = 0**: it never repeats, it polls.

**THAT IS THE CORRECT PLACE TO STOP.** This configuration has no BaseBoard --
`dorado_world` deliberately excludes it, because the bench drives the CP bus
itself -- so nothing is sending, and Bootstrap waits. It is not stuck; it is
doing its job.

Two things fall out. **Initial does not underflow the stack**: StkP holds `0x3f`
for the whole run, where AEmu reached 0 at cycle 565. And Initial's symbol table
names exactly the three things `exec-init` was patching by hand -- `RMINITL`
(35), **`STKINITL`** (56), `IFUMINITL` (61), **`TASKINITLOOP`** (106),
**`GETTASKINITPC`** (112), `TASKINIT` (113), `BOOTEMULATOR` (133). The machine
initialisation this project has been supplying from the testbench is a routine
sitting in the microcode, waiting to be run.

### And Bootstrap's poll loop IS data-dependent -- feeding it moves the chain on

`cpu.c` names the function Bootstrap's loop carries: `FB=7 FC=6` is
**`B <- RWCPReg`**, "HM page 31: `B<-RWCPReg = Link<-B, B<-CPReg'`" -- the
COMPLEMENT. `FF=176` in images 55 and 57 is exactly that, so the loop is reading
the control-processor register.

`+bootfeed=N` leaves N in CPReg before the machine starts, which is enough to
tell a poll from a dependency:

| CPReg | addresses executed |
|---|---|
| (none) | `f40 fc0 fe1 fe2 fe6 fe7` |
| `0000` | `f40 fc0 fe1 fe2 fe6 fe7` |
| `00ff` | `f40 fc0 fe1 fe2 fe6 fe7` |
| **`aaaa`** | `f40 f41 f42 f46 f4b fc0 fcd fe1 fe2 fe6 fe7` |

**With `aaaa` the machine LEAVES the poll loop and returns into Initial's body.**
`f41`, `f42`, `f46`, `f4b` are Initial images 2, 3, 5 and 4 -- `INITIAL1` and its
successors -- reached via `fcd`, a Bootstrap address the poll never touches. So
Bootstrap read the value, dispatched on it, and returned.

It then sticks at `f46` (12,485 microinstructions on one address), which is the
next thing to chase and is a different question from the one this answers.

**One static value is not the protocol** -- Bootstrap's symbols (`BOOTBYTEL`,
`WRITE000`..`WRITE111`, `BOOTSTAGE2`) say it reads a byte and dispatches on three
bits, so a real feed is a SEQUENCE the bench must supply as the 6502 does. But
the mechanism is now identified and demonstrated rather than inferred, which is
what the next step needs.

### Bootstrap's byte protocol, read off its own microcode

The eight dispatch arms `WRITE000`..`WRITE111` (images 33, 35, 37, 41, 43, 45,
47, 51 -- every second address, so each arm is two instructions) are
**IDENTICAL except for RSTK**: all carry `ALUF=00 BSEL=T LC=NoLoad
ASEL=A<-RM/STK FF=077 JCN=177(fast)`. Only the register-file address differs.

So a received byte is stashed into one of eight RM registers chosen by the
byte's own 3-bit code, and the mapping is a permutation:

| code | RSTK | | code | RSTK |
|---|---|---|---|---|
| 000 | 001 | | 100 | 101 |
| 001 | 011 | | 101 | 111 |
| 010 | 000 | | 110 | 100 |
| 011 | 010 | | 111 | 110 |

which is exactly **`RSTK = {n[2], n[0], ~n[1]}`**, all eight. That is not
arbitrary: CLAUDE.md already records that RM's low address bits are `~RSTK`, so
the permutation here is the microcode compensating for the same wiring, and
consecutive codes land in consecutive RM registers.

**So the protocol is: each byte carries data plus a 3-bit destination, and
Bootstrap assembles a microinstruction in RM before writing IM.** That is the
same shape as `SendViaMIR`'s eight half-words, reached by a different route --
this one is MICROCODE polling `B<-RWCPReg`, where `boot0-test`'s loader is the
BaseBoard's hardware jam path.

### THE HANDSHAKE IS BIT 15 OF THE CP REGISTER

Image 57 reads CPReg (`FF=176` = `B<-RWCPReg`) and branches `JCN=131`. Decoding
that against the table here: `JCN[0]=0` with `ccc = 1`, a **Conditional Jump**
on condition 1, which Table 13 gives as **`ALU < 0`** -- the sign bit of the last
ALU result.

So Bootstrap polls the CP register and proceeds when the value it reads is
NEGATIVE, i.e. when **bit 15 is set**. Measured over six values, three each way:

| `+bootfeed` | bit 15 | addresses executed |
|---|---|---|
| `0x8000` | 1 | **11** |
| `0xffff` | 1 | **11** |
| `0xaaaa` | 1 | **11** |
| `0x7fff` | 0 | 6 |
| `0x5555` | 0 | 6 |
| `0x0001` | 0 | 6 |

Perfectly separated, and it explains the first `+bootfeed` result -- `0xaaaa`
worked and `0x00ff` did not purely because of bit 15.

**So the BaseBoard's side of the protocol is: put the byte in the CP register
with bit 15 set to mean "ready", and clear it between bytes.** That, with the
3-bit destination code the `WRITE` arms decode, is the whole of what a feed has
to do.

**The next step is to feed Bootstrap properly.** `boot0-test` already walks real microcode
into IM over the control-processor bus using PARC's own hunk format and
`SendViaMIR`; pointing that at Bootstrap's `READBB` loop is what turns this from
"the boot chain starts" into "the boot chain completes".

**And the register-patching route's fix is still upstream: nothing has
initialised this machine.** A real boot
does not jump a cold world at `START` with the fault counter unarmed and every
task PC unset; Initial and the world's own `InitMem` do that first. That is what
`exec-world9` skips by preloading IM and jumping straight in.

### Traps found on the way

- **THE ADDRESSES ARE OCTAL.** `mbdis` prints IM addresses in octal -- its
  listing runs `...207, 210...` -- so a visited set collected in hex must be
  converted before it is looked up. Reading them as decimal named the wrong
  trap handler in a commit message.
- **`find_word` SEARCHED FOR THE BANK**, which is wrong whenever the wanted
  half is `0000`: a WIPED bank matches it too, so the search returned bank 0
  and read that bank's secondary bit. `IM[0x805]`'s right half in AEmu.mb IS
  `0000`. It computes the bank now; the search survives as the diagnostic that
  says where a missing word actually landed.
- **A VECTOR SET CONFINED TO LOW ADDRESSES** leaves `addr[11]` always 0 and
  exercises half the interleave, so `boot0-test` also loads four hunks from
  above 2048.
- **`$sscanf` RETURNS -1 ON A PARTIAL MATCH**, not the number of conversions it
  made -- while still correctly filling the arguments it DID convert. A
  three-field line read with a ten-field format comes back as -1 with every
  field right, so gating on the count silently skipped all of them.
- **`$fgets` RETURNS 0 AT END OF FILE** and leaves its buffer alone, so the
  `while (!$feof(fd))` idiom processes the LAST line TWICE -- 17 ALUFM entries
  out of a 16-entry file.
- **A GENERATED INCLUDE IS SHARED**, so anything in it that reaches into a
  board must be guarded: the IFUM tasks name `m.b_IFU`, and `tb_boot0` and the
  default `tb_exec` run on four boards that have no IFU.


## IM PARITY, ANSWERED (2026-08-27)

The machine has always had to run with the IM parity enables CLEARED, and this
file put the alternatives plainly: "either PARC's IRTable entries carry parity
that satisfies the generator, or our MC10170 parity generators on ContB j20/j21
compute something different". **It is the second, by exactly one inversion --
and that inversion is already known from elsewhere.**

    PARC's convention is ODD parity over the 17-bit half, and the IM ARRAY
    STORES ITS COMPLEMENT -- exactly as `dBlock'` does.

Three steps, none of them assumed, and each independent of the others.

**1. WHICH BITS EACH HALF COVERS, fitted rather than guessed.** `im_image` now
emits, for every present address, both the decoded fields AND the two 17-bit
half-words parity is computed over. Fitting one against the other across 2,148
AEmu addresses gives a UNIQUE match for every one of the 34 bits, with no
ambiguity and no inversions:

    left  half = RSTK.1-3, ALUF, BSEL, LC, ASEL   + RSTK.0 as the secondary
    right half = FF, JCN                          + BLOCK  as the secondary

**2. THE SENSE THE ARRAY WANTS, measured on the running machine.** Preloading
IM parity and sweeping sense against secondary-inclusion, over 200,000 samples:

| rule | IMLHPE |
|---|---|
| **even, secondary included** | **125** |
| odd, secondary included | 200000 |
| even, secondary excluded | 199680 |
| odd, secondary excluded | 445 |

**3. THE SENSE PARC USES, read off PARC's own hand-coding.** The IRTable's
five-byte format carries explicit `P015` and `P1631` bits -- and they VARY
(1/1 for `Nop#`, 1/0 for `Return#`, 0/1 for `CPRegToLink#`), so they are real
computed parity, not padding. All eight entries carry ODD parity over the same
17 bits: **8 of 8, 16 of 16 bits.**

Steps 2 and 3 are complements, which is the answer.

### What the check CANNOT tell you

Mutating `im-parity-check` four ways, two do NOT fail -- and both for
principled reasons rather than fixable ones, so the PASS must not be read as
covering them. The tool prints both limits beside its own result.

- **Whether the SECONDARY bit is included.** Every one of PARC's eight entries
  has `RSTK.0 = 0` and `BLOCK = 0`, so the dataset cannot separate 16-bit from
  17-bit parity. Step 2's machine measurement is what settles it, and the two
  steps are therefore not redundant.
- **The bit ORDER within a field.** Reversing it is a PERMUTATION of the bits
  parity is taken over, and parity is invariant under permutation. No parity
  check can ever detect it.

### The encoder now reproduces PARC's microinstructions EXACTLY

`mi()` in six benches used to set `P015` and `P1631` to 1 unconditionally, on
the reasoning that "a jammed instruction fails IM parity anyway -- that is the
jam mechanism". It computes them now, and the halves turn out to be simply the
fields, since parity does not care about order:

    P015  = odd parity over  RSTK, ALUF, BSEL, LC, ASEL   (4+4+3+3+3 = 17)
    P1631 = odd parity over  BLOCK, JCN, FF               (1+8+8     = 17)

With that, the encoder reproduces **all eight of PARC's hand-coded IRTable
entries byte for byte, parity INCLUDED** -- where its comment previously had to
say "parity bits aside". `im-parity-check` gates the round trip, which is a
stronger check than comparing parity bits alone: regenerating all five bytes
pins the BYTE LAYOUT too, so a wrong field position is caught where parity,
being permutation-invariant, would not catch it. 46/46 gates stay green.

### AND THE MACHINE RUNS WITH THE ENABLES ON

`make -C verilog exec-parity`:

```
tb_exec: LATE parity enables IMLH=1 IMRH=1, Stop=0
tb_exec: WHEN -- first parity error at cycle 0, last at 148; Stop first set at -1
tb_exec: IM PARITY ENABLED -- Error propagated on 0 samples, Stop=0, 24991 clk0' edges
tb_exec: THE MACHINE RUNS WITH IM PARITY ON.
```

**What was in the way was not the parity VALUES.** Bucketing the errors by TIME
rather than totalling them settles it: they run from cycle 0 to **148** and then
**clear by themselves, never returning across 400,000 cycles**. The preloaded
array's parity is correct; what fails is the machine's own power-up transient.

With the enables on from the start, `Stop` latches at cycle **125** -- inside
that window -- and **`Stop` gates the clock that would clear it** (`bCLKEnable'
= Stop | Run'` gates `clk2'`, the stop latch's own clock), so a momentary error
is permanent. Switching the enables on at cycle 400 instead gives zero
propagated errors and a machine that free-runs.

**A TOTAL SAID "parity is broken". A DISTRIBUTION SAID "the enables are on 148
cycles too early".** That is the third time in this project that reading a total
instead of a distribution produced a wrong conclusion.

### The jam's parity routing WORKS -- a claim of mine, retracted

**Retracted:** I wrote that "the jam never delivers a parity bit at all",
because capturing `IMLH` inside `parc_micro` reads 0 after all four data
strobes even for `Nop#`, whose `P015` is 1. Sweeping the actual routing refutes
it.

**Byte 0 is FOUR BITS, and they ride as the NINTH CP-BUS BIT of the four data
strobes** -- which is why its low four bits are always zero:

    b0[7] RSTK.0 -> fn 4        b0[6] P015  -> fn 5
    b0[5] JCN.7  -> fn 6        b0[4] P1631 -> fn 7

`make -C verilog/verilator exec-world9 EXECARGS='+jamsweep'` drives the ninth
bit alone on each function code and reads the MIR back:

```
JAMSWEEP fn=5  ninth=0 -> IMLH=0 IMRH=0   ninth=1 -> IMLH=1 IMRH=0
JAMSWEEP fn=7  ninth=0 -> IMLH=0 IMRH=0   ninth=1 -> IMLH=0 IMRH=1
```

Exactly what the byte layout implies, and it holds under every variant tried:
one strobe or two (`+jamsweep1`), preceded by fn 4 or not, and with any data on
the eight data bits (`+swdata=`).

**So the open question is narrower than I said**: the path delivers the bit,
and something in `parc_micro`'s full sequence clears it between the strobe and
the capture. That is a testbench-timing question about where to sample, not a
missing connection -- and it is not what makes `exec-parity` work, since that
switches the enables on after the startup transient and needs no jam parity at
all.

Both checkers work out to the same rule, derived from the netlist and the cell:

    ContB j20/j21 (left)   IMLHPE' = ~( XOR(17 field bits) ^ IMLH )
    ContA e18/e19 (right)  IMRHPE' = ~( XOR(17 field bits) ^ IMRH )

-- and they reach it by different routes, the left taking `IMLH'` and three
`LC'` (four inversions, which cancel), the right taking `IMRH` true. So NO ERROR
requires the stored bit to be the EVEN one, which is exactly what the preload
measurement found.

With a MIR parity bit of 0, an entry passes only if its field-XOR is zero --
true of `Nop#` alone among PARC's eight, and NOT of `Return#`, which is what the
startup jams (XOR(rh) = 1). Forcing `IMRH=1` across the jam (`+jampar`) drops
the error count 133 -> 85, which confirms the arithmetic even though the
startup transient is what actually gates the enables.

### The remaining inversion

`+imparityon` still stops the machine after two `clk0'` edges -- but with only
133 parity samples out of 400,000, so what fails is the jammed `Return#` in the
MIR at startup and nothing else.

The discrepancy is sharp: **the ARRAY wants EVEN (preloading even parity clears
IMLHPE; that is measured) while the JAM supplies PARC's ODD (Return# carries
P015=1, P1631=0, which is odd parity over its halves; that is read off PARC's
source).** In the real machine both paths satisfy one generator, so our RTL has
an inversion in one of them that the hardware does not.

`cell_MC10170` is the obvious suspect and was NOT changed, deliberately: it
matches its data sheet (A is odd parity over the nine data bits, B is even with
the controls folded in) and its header records that it was already tuned
against PARC's IRTable, fixing eleven of thirteen entries. Changing a
datasheet-correct cell on a hunch is how the last round of parity confusion
started. The place to look is where the two paths differ -- the CP-bus write
complements its data (`BMux` is the complement of `CPReg`, which is why
`SendViaMIR` uses `SetCPReg~` for IM data and the plain form for T), and
whether the jam path does the same is the question.

## START HERE: two open questions

The machine loads IM from real `.MB` hunks, matches the C emulator word for
word, **executes** microcode out of it, and **computes** -- Q, ALUFM and T all
verified against PARC's own boot sequence (`boot0-test`, `exec-test`,
`compute-test`). ONE thing in the boot chain is open. Section 2 below is kept
because what it cost is worth not paying twice, but it is a gate now, not a
question.

**And the MEMORY SECTION is traced end to end (2026-08-22/23)**, from a
microinstruction's ASEL field to one gate short of the DRAM array. It has its
own long write-up further down -- search for **"the memory reference-kind
chain"** -- and the short version is:

- every hop is gated: `WantProcRef'`, `FFok'`, `WantCR`, `Flush'`, the `Store←`
  that dirties a line, `HitColDirty` → `FlushStore` → `ForceMiss`, **the cache
  missing**, `←Map` writing an entry so `MapTrouble` clears, and `StartMem'`;
- one real RTL bug came out of it -- **VBB was modelled as 0**, which strobed
  off the whole map read path through five MC10124s. Fixed per-pin in
  `tools/sil_to_verilog.py`: *VBB loses to a real signal and beats an open
  pin*. 41 packages;
- what is still open is `MemWEa`, and it is **not an RTL fault**. `MemWEa`
  needs MemState 4; the bench's hand-built loop delivers 3, because a
  four-microinstruction loop beats against the map's own accept cadence
  (MapFnc=00 every 256 samples, MapState=3 every ~1024, coinciding once). A
  one-shot reference already gives 4x the window. The fix is a **short window
  of authentic reference cadence** walked into IM -- see the task list, and
  note PARC's memory diagnostics are millions of steps, far too long to run
  whole under Verilator.

### 1. Parity: the RULE is now known, and the fix is one line with a catch

`make -C verilog parity-probe` jams each of PARC's thirteen IRTable
microinstructions and reads the parity generators with the jam STILL IN THE
MIR: **all thirteen fail, both halves.**

**PARC'S RULE IS ODD PARITY OVER EACH 17-BIT HALF.** This is settled by pure
computation against PARC's own table -- no RTL involved. `doradoboot.masm`
states both the FIELDS and the BYTES for all thirteen entries, and P015/P1631
live in byte 0 (bits 6 and 4), so the rule can simply be fitted:

```
left  17 bits = RSTK(4) ALUF(4) BSEL(3) LC(3) ASEL(3)  -> P015
right 17 bits = BLOCK(1) FF(8) JCN(8)                  -> P1631
parity bit = ~(XOR of those 17)   i.e. ODD parity including the bit itself
```

That reproduces PARC's stated **P015 for 13/13 and P1631 for 13/13**; even
parity matches 0/13. The split is exactly the hunk format's two 17-bit halves,
and the checkers' own input lists confirm the domains -- ContA e19+e18 take
JCN, FF, CABlock and **IMRH**; ContB j21+j20 take BSEL, LC, ASEL, RSTK, ALUF
and **IMLH'**. Those two ARE the parity bits, so each checker sees 17 data + 1.
(All thirteen entries have RSTK.0 = 0 and BLOCK = 0, so the data cannot say
whether those two bits are inside the domain or outside it.)

**THE ONE-LINE FIX, AND WHY IT IS NOT COMMITTED.** `cell_MC10170`'s B output is
`~(par9 ^ p13 ^ p14)`. The data sheet (1978 MECL book p.123) draws B as a PLAIN
three-input XOR of the two controls with the odd-parity tree -- no bubble, the
same symbol as the four gates building the tree. The inversion its truth table
shows comes from the NOMINAL control levels the same diagram labels, "Control
(1) 13 High, Inputs (2) 14 Low": at pin 13 HIGH and pin 14 LOW,
`B = XOR(1,0,A) = ~A`, the table exactly. Baking that inversion into the gate
is right only at those levels and the Dorado never uses them -- ContA e18
leaves pin 13 unconnected and takes the cascade on pin 14. There is also an
argument needing no data sheet: PARC ran with IM parity errors ENABLED, so a
correct microinstruction MUST read `PE' = 1`. With `p15 = par9 ^ p13 ^ p14`
**all thirteen do**; with the `~`, none do.

Changing it, however, **regresses five jam-based gates** -- `datapath-test`,
`operand-test`, `step-test`, `sendmir` and `compute-test` -- measured with that
cell as the ONLY change, while `parity-probe` itself goes green. Of the 41
MC10170 packages, only FOUR use pin 15 at all:

| package | pin 15 drives | what it is |
|---|---|---|
| ContA e18 | `IMRHPE'` | right-half IM parity check |
| ContB j20 | `IMLHPE'` | left-half IM parity check |
| ContB e01 | `ContB03.sil+1` -> d05 -> **`RBMuxP`** | B-BUS parity; its controls are `BMux.16/17`, the parity bits riding that bus |
| ProcH d13 | `SignedCarry` | **not a parity signal at all** |

**THE CHEAP EXPLANATION WAS TESTED AND IS WRONG.** The obvious guess is that
those five jam SYNTHETIC microinstructions whose P015/P1631 were arbitrary, so
a correct checker rightly errors on them. Giving `mi()` the odd-parity rule
changes nothing -- all five still fail -- and `tb_datapath`'s hand-built
instruction already carries the correct bits (1 and 1). Do not spend time
there again.

**THE COUPLING IS THROUGH B-BUS PARITY, and the failure signature says so.**
With the fix, `compute-test` reports `T took 1200, not 1234`: ProcH's half of T
is right and **ProcL's half is zeroed**. ProcL uses no MC10170 pin 15 at all,
so this is not a local effect -- the bridge is ContB e01, whose two CONTROL
inputs are `BMux.16`/`BMux.17` and whose output reaches ContB d05, an MC10102
driving `RBMuxP`, the B-bus parity bit itself. Flip the cell and the processor
boards see a bad-parity B bus.

**So the next step is the B-bus parity chain, not the IM checkers.** Make
`RBMuxP` and the `*.Perr'` generators on ProcH/ProcL (`R.Perr'`, `T.Perr'`,
`Md.Perr'`, `IOB.Perr'`, all MC10170 pin-2 outputs, which this change does NOT
touch) consistent with the corrected B output, then re-run. `SignedCarry` at
ProcH d13 needs its own check against the C emulator's overflow logic (HM
section 3.7, QW7). And `mi()`'s parity computation should land TOGETHER with
the cell fix, never before it -- with the present cell a CORRECT parity bit is
exactly what the checker rejects.

**A trap worth recording, because it cost a wrong diagnosis here.** Register 0
of the manifold is not "the parity enables" -- `12'h030` is
`DisableDoradoErrors`, and writing `12'h000` to the same register does not
"turn parity off", it re-ENABLES every Dorado error class. Patching the
jam-based testbenches that way broke four of them, which then looked exactly
like fallout from the cell change and was not: the same four still failed with
the cell reverted. Change ONE thing at a time when a fix touches 41 packages.

### The real firmware runs the machine

Every other gate here has the TESTBENCH play BaseBoard, strobing the
control-processor bus by hand from sequences transcribed out of
`doradoboot.masm`. `make -C verilog firmware-probe` does not: it brings up
**BaseBd + ContA + ContB + ProcH + ProcL** as one machine (`dorado_boot`,
generated like the other configurations) and lets the 6502 run its OWN firmware
out of its OWN EPROMs. A diagnostic, not a gate. Two switches:
`+define+LONG_RUN` (260 M sys_clk, enough to cross a watchdog interval) and
`+define+G22_DISARMED` (an experiment that disarms the watchdog at time zero,
to separate it from everything else).

| | 4 M armed | 4 M disarmed | 260 M armed |
|---|---|---|---|
| `MCReset'` assertions | 19 | 1 | 397 |
| `CPStrb'` edges | 37 | 450 | 27,674 | (Control and MIR3 only — see below) |
| `PACIFYWATCHDOG` F692 | 0 | 5 | 240 |
| `DMuxClk` edges | 0 | 0 | 0 |

**THE BASEBOARD BOOTS ITSELF PAST POWER-UP.** In the 260 M run, resets bucketed
by watchdog window (each Q21 edge starts a bucket):

| window | from | g22 FF1 `Q'` | resets |
|---|---|---|---|
| 0 | 0 | 1 — **ARMED** | **397** |
| 1 | 83,886,119 | 1 — ARMED | **0** |
| 2 | 167,772,199 | 0 — disarmed | 0 |
| 3 | 251,658,279 | 0 — disarmed | 0 |

Every reset is in window 0. After the first watchdog interval there are none,
across 176 M sys_clk — **including window 1, which is still ARMED**. The
firmware gets far enough to pacify, the XOR stays 0 from then on, and the
watchdog is satisfied for the rest of the run.

#### What made it reach the Dorado

One fix, and it belongs in the **generator**, not a cell: `WEAK_PORT_DRIVERS`
in `tools/sil_to_verilog.py`, symmetric to the existing `OVERRIDE_DRIVERS`.

A 6532 port pin is high-Z with an internal pull-up when its DDR makes it an
input, and the core states that directly — `PA_out = out_a | ~dir_a`, with the
comment that the output "must be fed back to input ... for the chip to read
properly". That is a **wire-AND** convention; these nets resolve as
**wired-OR**, so the pull-up won instead of losing and pinned every such net
high.

**It has to be per-net.** Masking inside the cell with the DDR holds the 6502
in reset, because `WatchdogOut`'s only driver *is* the RIOT and its pull-up is
real. The rule: the pull-up loses where something else drives, stands where
nothing does — information the generator already has.

It is sound because of what those nets are. Across the machine **33 nets** have
a 6532 port pin sharing with another driver, and in every one the other driver
is a real totem-pole part ('174, '259, '01, '157, '175, '74, MC10125) or a
strap, with the 6532 pin as the READER: **`RCPReg.00-15`** — how the BaseBoard
reads the Dorado's CP register back — plus `MCManif.0-3`, `TCPI.0-3`, the
temperature senses and `WatchdogIn`. All were stuck high.

#### The watchdog

Every part of the chain is in the RTL:

| package | part | role |
|---|---|---|
| g21 | **MC14521B** 24-stage divider | **the timer**: Reset tied low, `MCPreClk` into In2, **Q21** (pin 13) out. Interval = MCPreClk / 2^21, of order a second. Was an unmodelled skeleton; the cell now comes from the data sheet |
| g22 | SN74LS74 pair, both TOGGLES | FF1 is clocked by that Q21, so it alternates ARMED (`Q'`=1) and disarmed every interval |
| g23 | SN7486 | XOR of `WatchdogIn` against `WatchdogOut` — the pacify test |
| j17 | SN74LS01 (open collector) | NANDs those two to drive `BootMC'`, wire-ORed with a strap at h07 |
| j08 | SN74LS74 | D = `BootMC'`, clock = `MCClk`, async `PwrGood`, output `MCReset'` |
| f63 | MCS6532 RIOT | PA7 out as `WatchdogIn`, PA6 in as `WatchdogOut`; `PacifyWatchdog` echoes one onto the other |

**What trips it is a one-cycle glitch, not the timer.** `WatchdogOut` is driven
low for exactly one MCClk cycle (215,518 → 215,598) while the firmware sets up
the RIOT — the DDR is written before the output register, so the pin drives
whatever `ORA` holds. That transient spikes the g23 XOR, pulls `BootMC'` low,
and j08 latches a reset 521 sys_clk later. It only gets through because g22's
FF1 `Q'` is 1; in the disarmed half j17 masks the XOR entirely. Hence the 397
resets confined to window 0 and none afterwards.

`SkipWait'` reads 1, which *would* make `PacifyWatchdogIfJumper` skip — but
that entry is never used; the firmware calls `PacifyWatchdog` directly.

#### The manifold path — and what the CP-bus traffic actually is

Traced by **routine**, in the disarmed build:

| routine | addr | visits |
|---|---|---|
| `SETMANIFOLD` | F95A | 4 |
| `SETMUFFLERADDRESS` | F9D0 | 4 |
| `TRYGETTINGMUFMANCONTROL` | FA0E | 52 |
| `WAITFORCPCONTROL` | FA1F | 4 |
| `DATUMTOMANIFOLD` | F977 | 0 (different entry, unused here) |

`SetManifold` calls `SetMufflerAddress`, which calls
`TryGettingMufManControl` and gives up on carry set. That routine reads
`MCPBusL & SetRunIn` (bit 2, the net `TSetRun`) and fails when the bit is SET —
it reads **0**, so the gate passes.

The shift does **not** use the `DMux*` nets. `SetMufflerAddress`'s inner loop
at F9F6 sets `MCPBusL = $10` — **function code 1, `Clock`** — then writes
`MCPBusH` and pulses `MCPStrb` twelve times. `i62` is that RIOT: PA drives
`MCPBus.00-07`, PB7 `MCPBus.08`, PB6-4 `MCPABus.0-2`, PB0 `MCPStrb`. So a
manifold word goes out as ordinary CP-bus transactions, and the BaseBoard's own
k22/k17 decode them into `CPDMuxData`/`CPDMuxClk`, through l19 and the l24
TTL-to-ECL translator, onto the backplane as `DMuxData`/`DMuxClk`.

**Measure the PC with SYNC, not the address bus.** The 6502's SYNC output marks
an opcode fetch. Ungated, the address bus shows data reads too — which is how
`FF00` (1024) and `FF80` (669) came to look like a PC parked in filler, when
`FEF0..FF10` disassembles as **data**. Gated on SYNC, the most-executed
addresses are all F84A..F865, the ADCONVERT loop.

**And one generator fix made the manifold chain run: a 6532 port pin reads its
own pin.** `read_excluding` drops a package's own contribution — right for a
gate, which does not read its own output; wrong for a port pin, where the chip
reads the **pin**, and an output pin is whatever the chip itself drives. The
core says so outright: *"NOTE that port output must be fed back to input ... in
order for the chip to read properly."* `READBACK_OWN_PIN` in
`tools/sil_to_verilog.py` now wires `pa_in`/`pb_in` from the **resolved net**.

What it cost while wrong: every read-modify-write on a port read **zero** for
its own output bits. BaseBd i62 is MCPBusL, and `SetMufflerAddress` pulses the
CP-bus strobe with `INC $0582` / `DEC $0582` — reading 0x00 instead of 0x10
turned those into 0x01 and 0xFF, so the three `MCPABus` function bits read 0
then 7 instead of a constant 1, and k22/k17 never decoded a DMux pulse.

**With it, the muffler address shifts out:**

```
strobe 2 (fetch f9fd) dir_b=f1 out_b=13: fn=1 data=001000000
strobe 3 (fetch fa08) dir_b=f1 out_b=13: fn=1 data=000000000
... 24 strobes, all fn 1 (Clock) = twelve bits, two strobes each

CPDMuxClk edges 24    the BaseBoard's own decode fires
TDMuxClk  edges 24    through l19
DMuxClk   edges 24    onto the BACKPLANE, to every board
READMUFFLER F986 1 visit, SETMUFFLERADDRESS F9D0 1
```

`out_b = 0x13` is `$10` plus the input bits reading back — exactly what a
read-modify-write on a 6532 should give. `MCPBus.00` is the data bit and
`MCPBus.01` the clock; `CPDMuxData` stays still here only because the address
bits shifted so far are zeros.

**So the BaseBoard now clocks the muffler chain on the backplane.**

**The far end is measured, and the measurement is inconclusive** — worth
stating rather than reading either way. ContB's twelve-bit shift register and
its latch:

```
DMuxClk edges 24            the clock reaches the backplane
ContB DMD changes 0         its shift register never moves
ContB ManClk.0' pulses 1    at time 1 only, i.e. power-up, DMD=000
```

A dead register looks **exactly** like a register being fed zeros. Every data
bit shifted in this window was 0 — `MCPBus.00` is the data bit and it is never
set across the 24 strobes, so the muffler address being sent is all zeros — and
shifting zeros into a zero register changes nothing. This says nothing yet
about whether ContA/ContB act on the chain.

**To settle it,** catch a shift whose address is *not* zero: run
`+define+G22_DISARMED +define+LONG_RUN` so the firmware walks more muffler
channels, and watch `CPDMuxData` (the BaseBoard's own data bit) alongside
ContB's `DMD`. If `CPDMuxData` toggles and `DMD` still does not, the far end is
genuinely not shifting; if neither toggles, the addresses are still zero and
the run was too short.

#### Also worth knowing

The firmware's hot loop (F84A..F864) is a **successive-approximation A/D
conversion**: write `DAC = 400+PA`, settle, read `Comparators = 480+PA`, shift.
Its analog chain is unmodelled — CA3140 op-amps (g18, i19, i20, i21, j21),
CD4051 analog muxes (i2125, j24, k24), AUGATCG16 resistor platforms. That is a
fact about the RTL; it is a **cycle count and not a proven blocker**, since the
firmware demonstrably proceeds past it.

**Three instrument traps this probe walked into**, all the same shape — a proxy
measured instead of the thing, each producing a confident wrong conclusion that
survived several rounds of reasoning:

* counting `0xFFFC` on the address bus as a **reset**. It is not one; the
  firmware reads ROM there. Count `MCReset'` falling edges.
* reading a **total** as a **distribution**. "397 resets over 260 M" looked
  like a machine that never settles; bucketed by window, all 397 are in the
  first one and it settles permanently after.
* inferring the manifold path from a plausible-looking net (`DMuxClk`) instead
  of reading what the firmware **writes**. The path is the CP bus.

### The memory section, and a backplane line that was not connected

`make -C verilog mem-test` is the first rung for MemC/MemD/MemX, and
deliberately the same one the processor started on: a machine that elaborates,
with the boards clocked. `dorado_mem` is **ContA + ContB + ProcH + ProcL +
MemC + MemD + MemX** -- the Control and Processor boards are there because a
memory reference does not exist without them.

**The memory clocks are GATED**, which is the first thing to know. Each board
fans its backplane clock (`CLK.mc'`, `CLK.md'`, `CLK.mx'`) out through a first
stage that ANDs it with `MemClkEnable'`, driven by a pair of MC10231 flip-flops
on ContA. So a dead local clock out of reset is CORRECT, and "the clock runs"
is the wrong thing to assert. The gate asserts the consistency instead -- a
board's local clock runs **iff** its enable is asserted:

```
MemC  MemClkEnable'a=1  local clock 0 edges     correctly gated off
MemD  MemClkEnable'a=1  local clock 0 edges     correctly gated off
MemX  MemClkEnable'c=0  local clock 1250        running
```

**MemX is a finding:** its enables are undriven. ContA drives `MemClkEnable'a`
and `'b`; MemX asks for `'c`, which no board drives, so its clock runs only
because an unconnected net floats to the enabled state. The pins do not settle
it either -- all three memory boards take theirs on C12 while ContA drives on
C17 and C20, exactly the non-straight-through backplane this machine models by
wiring on NAME. Whether `'c` is a third fan-out copy spelled differently or a
separate line is not established.

**What turns the memory clocks on, and THE FIX FOR `machine-test`.** `MemClkEnable'a` is a wired-OR of two MC10231s on
ContA: one latches **`dMemRun`** (set by `RunRefresh`, which comes from
`WantRunRfsh`, which comes from **`SetRunRfsh` -- a backplane line the
BASEBOARD drives**), the other latches **`dStop`**. So the memory clocks come
on when the machine RUNS and refresh is asked for, which is why they are off in
every jam-mode test.

`+define+MEM_RUN` runs run-test's own start sequence and then asserts
`SetRunRfsh` as the BaseBoard would. That first produced
`Active region did not converge` -- the same failure `machine-test` had on the
eleven-board machine, reproduced on seven, which is what made it findable.

**IT WAS `cell_F10016`'s TERMINAL COUNT, and the data sheet settles it.**
Verilator's `UNOPTFLAT` warning (which the gates suppress) named the loop:
MemD's `d14` is an F10016 counter whose `TC` output (pin 4) runs through `d20`,
an MC10195 whose select pin is unconnected -- so an inverter -- straight back
to that same counter's own `CE` (pin 6). The cell had

```
assign p4 = ~(&q & ~p6);      // TC gated by CE
```

which at terminal count reduces to `TC = ~TC`. An oscillator. The Fairchild
data sheet (`DoradoDocs/datasheets/F10016.pdf`) names the pins outright --
**`CE` Count Enable (LOW to Count)**, **`TC` Terminal Count (10016 LOW at
HHHH)** -- so TC is a function of the Q state ALONE. The part carries its own
expansion logic ("INTERNAL COUNT ENABLE - FOR HIGHEST SPEED EXPANSION"), and
the eight `TC -> CE` cascades in the machine (IFU has a three-stage chain, plus
ProcH, MemX, DispM) work correctly that way: TC goes low at HHHH, CE is low to
count, so the upper stage counts exactly when the lower one is at terminal.
MemD's own arrangement is then a stable count-and-stop.

With `assign p4 = ~(&q)`: the memory boards clock (`MemClkEnable'a=0`, 1250
edges on MemC, MemD and MemX), the seven-board machine converges, and
**`machine-test` passes** -- 24/24 clock nets toggling. 226 F10016 packages
across the machine, one line.

#### SIX BACKPLANE LINES WERE SPELLED TWO WAYS, and one was the memory hold

Found while chasing the above, and it is the first real defect in the memory
section. PARC's draughtsmen were not consistent about capitalisation, and this
backplane is wired by NAME, so a spelling difference silently leaves a line
unconnected:

**`PrHold` on MemC and `PRhold` on ProcH/ProcL are ONE WIRE** -- `#07-E.42`,
`#s05-E.42`, `#s04-E.42`: same connector, same pin, three slots. Before this,
MemC drove `PrHold` into nothing and the processor read `PRhold` from nothing,
so **the memory section could not hold the processor at all.**

| one line, two spellings | genuinely separate (different pins) |
|---|---|
| `PrHold` / `PRhold` — MemC → ProcH, ProcL | `CLKEnable'a` C16 vs `ClkEnable'a` C8 |
| `MxHold` / `MXHold` — MemX ↔ MemC | `IOin'` E71 vs `IOIn'` E70 |
| `HoldMapbuf` / `HoldMapBuf` — MemX ↔ MemC | `IOout'` E74 vs `IOOut'` E71 |
| `Subtask.0`, `Subtask.1`, `FoutSubtask.0` — MemX ↔ ProcL, DispY | |

**Case-insensitive matching would be WRONG**, which is why this is a six-entry
table (`BACKPLANE_CASE_ALIASES` in `tools/sil_netlist.py`) and not a rule.
Among backplane nets there are nine case-variant groups and **three sit on
different pins** -- separate lines that merely look alike. Outside the backplane
it would be far worse: **63** net names differ only by case, mostly per-board
LOCAL clock fan-out such as `Clk0'Aa` on MemX against `clk0'Aa` on IFU, and
merging those would tie every board's clock distribution together. The rule is
narrow and checkable: merge a case variant only where every board using it
agrees on the pin. No board carries both spellings, so no rename collides.

### RM, and a general microinstruction encoder

`compute-test` writes RM (`LC[6]` = "RM/STK <- Pd") and reads it back
(`ASEL[4]` = A<-RM/STK with ALUFM[0] = 37 octal = A), at four addresses with
four values.

**It also checks the PHYSICAL address**, because a write-then-read alone proves
little: RM's address pins take `RbAdr.0-.3` unprimed but `RbAdr.4'-.7'`
PRIMED, and a mis-modelled polarity there is a consistent PERMUTATION -- it
round-trips perfectly and is still wrong. That is the trap that hid the IM
address reversal until Boot0 compared IM against something external, and
**nothing external ever sees RM**.

**The low four address bits are `~RSTK`.** `RbAdr.4'` comes from an MC1662 NOR
at ProcH k08 -- a 2:1 read/write address mux whose other input is `RbWadr` --
so the RSTK half reaches the RAM complemented, as its primed name says. Three
independent things agree: `mir-diff` proves RSTK is right in the MIR, the wire
list shows the NOR, and the gate measures `~RSTK` end to end. Harmless to the
machine (a permutation within each RBase bank) but **it must be applied if RM
is ever diffed against the C emulator**, whose `RM[n]` is index n. The high
four bits are RBase, checked only for constancy; they come from an MC10231's
true output on ProcL g10 across the backplane and read 15 here, and nothing in
the IRTable loads RBase, so which value that represents is not settled.

**A GENERAL MICROINSTRUCTION ENCODER**, which is what made this reachable.
`tb_compute.sv`'s `mi()` builds the five bytes from
`(rstk, aluf, bsel, lc, asel, ff, jcn, block)` per the layout `doradoboot.masm`
states, and it **reproduces all thirteen IRTable entries byte for byte** --
parity bits aside, which PARC states per instruction. So it is checked against
PARC's own hand-coding rather than trusted, and any future microinstruction can
be built with it instead of hand-assembled.

(That validation also, incidentally, extracts `P015`/`P1631` for all thirteen
entries alongside their field values -- which is the dataset the open parity
question needs.)

### The ALU, in the machine

`compute-test` ends by computing: **A from T, B from CPReg, the function from
ALUFM, result back into T** -- and sweeping every entry of HM Table 9, all 16
logical and all 8 arithmetic, against the C emulator's own `alu_op()`.

This is the first thing to exercise the ALU AS THE BOARDS WIRE IT. `alu-diff`
matches the C emulator on 10,752 vectors but builds four MC10181 slices in a
TESTBENCH; here they are ProcH's and ProcL's own with the carry chain running
`f61 -> e61 ->` across the backplane, and `A+B = b78e` only comes out right if
carries ripple through all four.

Getting to the A side took changing **one field** of PARC's `TFromCPReg#`:
`ASEL[4]` is A<-RM/STK -- which is why `alua` read 0000 in every earlier probe
-- and `ASEL[6]` is A<-T, so byte 4 goes `C0` -> `E0`. The check that the
encoding is right is that `doradoboot.masm`'s stated byte layout reproduces
PARC's own `C0` for `ASEL[4]`.

**THE ALUFM ENTRY IS NOT A CONTIGUOUS FIELD OF B**, and this cost an hour.
HM Table 11d: **"ALUFMEM <- B.8, B[11:15]"**. The entry's MSB -- the ALU's
CARRY IN -- comes from `B.08`, the other five from `B[11:15]`. Every LOGICAL
entry is <= 037 octal and lands entirely in those five bits, so the logical
half of the sweep passed while `A+1` did not: writing the carry at bit 5 of B
puts it nowhere. The wire list says so directly (ALUFM's data pins carry
`alub.08/11/12/13/14/15`, not six contiguous bits) and `cpu.c` documents the
same mapping from the same table. Reversing the sense of the MC10107 that
makes `aluCin` is caught by the arithmetic half and NOT by the logical half --
nor by `alu-test` or `alu-diff`, which never see that gate.

**Two more conventions closed on the way.** `cell_MC10173` (81 packages) had
carried a `VERIFY` note saying its select sense and its transparent-clock level
were read from pin names rather than a truth table. Both are now read off the
data book (p.3-79) -- SB high selects the datasheet's D_0 pins, and the latch
is transparent while the clock is LOW -- and both are GATED: reversing either
makes `compute-test` fail, because T is loaded through that part.

#### What getting here cost, and the two cell bugs it found

Both were invisible to all eighteen other gates; every one of them passed with
both bugs in place.

* **`cell_MC10141` loaded its PARALLEL ENTRY rotated one place** -- wired
  `Q0<-D3, Q1<-D0, Q2<-D1, Q3<-D2` where the truth table says `Q0<-D0 .. Q3<-D3`
  flat. That is the whole of the old "Q = 008a where 25 octal is 0015": run 0015
  through a per-nibble rotate and you get 008a exactly. **60 packages.**
  The cell had carried a `VERIFY` note saying its mode encoding came from the
  pin names rather than a truth table, and it was right to.

* **`cell_MC10119` modelled a SHARED pin as a standalone AND term.** Pin 10 of
  the four-wide OR-AND belongs to TWO of its OR groups -- the part is a
  4-3-3-3, thirteen input slots across twelve pins, and the thirteenth is pin
  10 counted twice. ANDing it separately instead pulls the output low whenever
  pin 10 is low; on ProcH b17/c17 that pin is `FA=0'`, so **every**
  microinstruction with FA=0 -- PARC's own `Nop#` among them -- asserted
  `QshiftL'`/`QshiftR'`, and both asserted is PARALLEL ENTRY on the MC10141. Q
  was reloaded from a dead B bus every cycle. **10 packages.** This is the same
  class as the MC10121 shared pin, and `cell-check` cannot see it: the cell
  reads all the right PINS, it just groups them wrongly.

**PARC's DICTIONARY DISAGREES WITH MOTOROLA ON THE MC10141's PIN NAMES, and
neither is wrong.** EclDict calls pin 5 `DL` and pin 13 `DR` (Motorola: `DR`
and `DL`), and pin 10 `SL'`, pin 7 `SR'` (Motorola: `S1`, `S2`). The two swaps
are consistent with each other -- the mode PARC calls shift-left is selected by
its `SL'` and consumes its `DL` -- so it is only which end of a 16-bit register
you call "left". The board reads correctly under PARC's names; the CELL must
implement Motorola's function per PIN NUMBER. Do not reconcile them.

**THE NOPS IN PARC'S SEQUENCES ARE NOT PADDING.** The Qshift controls come out
of a register (ProcH b15) clocked by `Clock1'`, while the MC10141's own clock
`QClock'` is gated off `PreClock1'`, which is EARLIER. So the controls in force
at any load edge are the ones the PREVIOUS instruction latched: decode in cycle
N, act in cycle N+1. **Q is not loaded by `QFromCPReg#`; it is loaded by the Nop
after it.** PARC's comment on `DoIRTableInstAndNop` -- "the Nop holds CPReg
constant through T3 of the previous instruction" -- is load-bearing, and a
probe that samples right after an instruction is reading one cycle early.

**A KNOWN HOLE.** Mutating the MC10141's two SHIFT modes against each other
still passes `compute-test`, because this prologue only ever uses parallel
entry and nothing in the suite drives a Q shift. The corrected cell agrees with
three independent sources (both Motorola books and EclDict's own `[FF]` arcs,
which pair DR/pin 5 with Q3/pin 3 and DL/pin 13 with Q0/pin 14), but that is
ARGUED, not gated. Closing it is contained: `sil_check_cells.py` already parses
the `[FF]` lines for their CLK and RS pins and **throws the per-output
data-input arcs away** -- and those arcs are exactly the per-output dependency
list it already checks combinational parts against.

**Read PARC's IRTable field comments before decoding bytes by hand.** They
state the fields outright -- `TFromCPReg#` is
`RSTK[0],ALUF[0],BSEL[0],LC[1],ASEL[4],FF[176],JCN[201]` -- and an arithmetic
slip reading byte 3 sent this chase off after the wrong FF decode for a while.

### Then

Three cells landed from datasheets Alan supplied (`F10016.pdf`, `F100181.pdf`,
`ADC-MC8BC.pdf`) plus one identified from a bitsavers schematic note. The
F10016 fixed `machine-test`. **`cell_F100181`** (8 packages, all MemC -- the
memory board's address arithmetic) is modelled but **nothing exercises it
yet**, and two of its conventions are assumed rather than confirmed: the
carry-in polarity and the output-latch sense, both flagged in the cell.
**`cell_K1115A`** is the 20 MHz crystal oscillator on DispY a05 and DskEth j20
(driving `EClk0`), a SUBSTITUTION like the VCO with the divisor a documented
choice, not a measurement.

Still missing as datasheets: nothing critical. `PLAT1816` is an Augat resistor
platform, not a chip. Eleven parts remain unmodelled but their datasheets are
already in `DoradoDocs/datasheets/` -- CA3140 x9, AM2615 x7, MPQ6002 x5,
MC1650 x4, MC10163 x4, CD4051 x3, 8T98 x3, MC1672 x3, MC10182 x3 -- so those
are work, not a blocker.

With the datapath gated, the natural next rungs are **RM and STK** (the MB7071H
register file at ProcH h06/i06, which `datapath-test` reaches but nothing
exercises) and **a microinstruction that computes on TWO operands** -- every
gate so far moves ONE value, so an A operand has never been driven.

Also open: `machine-test` is the one red gate (the assembled machine does not
converge under the C++ `eval()` model though it does under the event
scheduler); `memory.c` against MemC/MemD/MemX is the next C cross-check; the
cell library is at 97.7% with datasheets for almost everything missing now in
`DoradoDocs/datasheets/`; and `sil_check_cells.py` still throws away the
per-output data-input arcs in the dictionary's `[FF]` lines, which is the check
that would have caught both of this session's cell bugs mechanically.

**Nine things that will otherwise cost you a day:**

* **MEASURE PARITY WITH THE JAM STILL IN THE MIR.** The generators are
  combinational off it, so the machine must be STOPPED. Running first reloads
  the MIR from IM and measures the parity of whatever was there -- that read
  the left half passing and the right failing, and both were an artifact.
* **`Error'` = 0 IS an error**, settled by function not by the prime in the
  name: it feeds `dStop` and stops the machine.
* **ONCE `Stop` SETS IT GATES THE CLOCK THAT WOULD CLEAR IT.**
  `bCLKEnable' = Stop | Run'` gates `clk2'`, the stop latch's own clock, so
  `dStop`=0, `Run'`=0, `Stop`=1 is a real state and only ClrStop escapes.
* **CONTROL STROBES MUST BE SPACED** -- the BaseBoard is a 1 MHz 6502 and
  `SetRun` must survive three `RunClk'` edges. Without a gap nothing loads.
* **`SetCPReg~` writes the COMPLEMENT**, and `SetCPReg` does not: the ALU
  function code goes in uncomplemented, the address and data complemented.
* **A hunk is 17 bytes and that is exact**; **IM is INTERLEAVED**, the low
  address bit picking the bank; **the right half's secondary bit is stored
  COMPLEMENTED**, BLOCK being the one field bit the MIR wires inverted.
* **The first microinstruction out of a cold stop runs only its `clk0` half.**
* **Check more than the obvious, and pick asymmetric data.** Four mutations
  passed against these gates before they were finished.

## How to read the rest

The sections below are in the order they were written, which is roughly the
order the machine came up. Grouped:

* **Reference** -- the FPGA shape and why it is not the physical one; where the
  clock comes from; how the backplane was derived; OR/NOR polarity; why
  `machine-test` is not a toggle count.
* **The bring-up, in order** -- the BaseBoard's 6502 boots; the boot interface;
  all 36 microinstruction bits; the MIR; the reply path; the machine runs; four
  boards; IM mapped; PARC's hand-coded microinstructions; the Write-IM.
* **Classes of bug worth knowing** -- Tim's COMMON pin; the passive packages;
  the machine not settling; `MCD_0..7` double-driven.
* **Tasks and traps** -- filling in the cell library; testing against the C
  emulator; things that will bite; the one open licence question.

## Historical: what landed on 2026-08-15/16

- **The last two PROMs.** `EtherRcvr` and `EtherXmtr` are ported, so all 26
  now generate. They are the only PROMs that are genuine state machines, and
  each has a property check asserting the behaviour the source's comments
  describe -- reachability, no undefined next state, the transitions
  themselves. The checks were then **mutation-tested**: fourteen deliberate
  transcription errors were injected (dropped inversions, swapped low-true
  polarities, transposed `srCtrl` encodings, a lost BCPL switchon
  fall-through, a field shifted one bit) and all fourteen are caught. The
  first draft of the checks caught only eleven; the three misses are why the
  checks now pin the srCtrl encodings and the two low-true inputs
  behaviourally rather than just rejecting the illegal value.
- **The backplane, found rather than inferred.** Every board directory states
  its interface THREE times and all three agree: `<Board>.bp` (`ALUCarry:
  E179`), bare `E179` tokens in the `.wl` itself, and slot-qualified
  `#s05-C.5` in `-C.nl`/`-E.nl` -- 2,052 of 2,054 pins identical, the five
  exceptions being ground nets the `.wl` numbers individually (`GND-26`) and
  the `.bp` collapses (`GND`). The Reference section below has what this means
  for wiring boards together.
- **The port lists now come from `.bp`.** They were inferred before, by
  treating a net whose only consumers were `Term100` pins as leaving the
  board. That was wrong at the root: **`Term100` is a 100-ohm terminating
  resistor network, not a connector** -- ECL terminates every line, and ProcH
  has 139 of them against 175 backplane nets. The inference missed 703
  backplane nets (emitted as internal wires, so those signals could never have
  reached another board) and invented 833 ports that are not on the backplane.
  All sixteen boards now emit exactly what PARC states, and
  `tools/sil_backplane.py --ports` is the gate: 1,920 of 1,922, 0 spurious.
  A **third port direction** falls out: 512 nets are `inout`, because the
  board both drives and senses them. `wire`/`output` there would let a board
  read back only its own contribution to a wired-OR bus instead of the bus.

- **The backplane top module is generated.** `make -C verilog backplane`
  writes `verilog/generated/dorado_backplane.v`: eleven boards, **501
  internal nets** (83 of them `wor`), **407 top-level ports**. Every port
  carries a comment saying whether it goes to a connector on the backplane
  (274 -- disk tag bus, ethernet transceiver, monitor, keyboard) or is
  waiting for a board this configuration does not have (133). `--boards`
  takes any subset, so the machine can be brought up a board at a time, and
  every subset tried lints clean.
- **DispM plugs INTO DispY; they are not alternatives.** 42 nets are shared
  by the two display boards and by no other board -- `AItem.0-7`, `BItem.*`,
  `AOff`, `BByPass` -- so a colour machine has BOTH, and adding DispM turns
  those 42 dangling ports into internal nets. This agrees with the Hardware
  Manual (doc p.110: "on a Dorado with only a 7-wire terminal and no color
  monitor, only the DispY board is present") and with the C emulator, where
  the colour display is a second screen rather than a colourisation of the
  first.

- **The PROM contents are in the RTL, and read back correctly.** The handoff
  used to say "give `cell_MB7071H` a `$readmemh` array". That was the wrong
  part: `MB7071H` has `WE'`, block selects and separate IN/OUT pins -- it is
  a **RAM**, and its 70 packages sit on DispM (32, the colour tables), MemC
  (21) and ProcH/ProcL (4 each). The PROMs are three other parts, and the
  counts settle it exactly: **`SG10139`** (32x8 ECL) has 15 packages =
  DskEth 2 + IFU 1 + MemX 8 + Proc 2+2, precisely the 32x8 entries;
  **`MCM10149`** (256x4 ECL) has 15 = DskEth 11 (disk and ethernet) +
  DispY 2 + DispM 2; **`SN74S288`** has the one 32x8 TTL part, the disk
  drive-select. All three now model the part and take an `INIT_FILE`, and
  the generator passes each of the **29 placed packages** its own image.
  Gate: `make -C verilog prom-test`, 1,360 checks, mutation-tested with five
  injected wiring errors (address order, output order, chip-enable sense).
- **Bit order is one rule on both axes, and it is now tested.** PARC's
  structures are MSB-first, so **A0 is the most significant ADDRESS bit and
  Q0 the most significant OUTPUT bit**. Both are stated in the sources --
  `EtherProms` marks `pdCarrier bit // A0 pin 4` for a field that is address
  bit 7, and `DiskProms` has `Pin1 = #200`, bit 7 of a byte, where Pin1 is
  Q0. A neat confirmation fell out: `SN74S288` wires only six outputs
  (Q0..Q5), and `MakeDriveSelect` uses only bits 7..2 -- exactly those six.

---

## Reference: the FPGA shape, and why it is not the physical one

The target is an FPGA, which rules out two things the Dorado does natively.
Both are handled in the GENERATOR, so the RTL stays derived rather than
hand-adjusted.

**Wired-OR buses become OR trees.** MECL 10K outputs are open emitters and
the machine ties them together: 115 backplane nets are driven by more than
one board, the B bus `BMux.00-15` among them (ContA, IFU, MemC, MemD, MemX
and ProcH/ProcL all drive it). An earlier pass modelled that with `inout`
ports and `wor` nets, which simulates correctly -- and synthesises nowhere,
because an FPGA has no wired-OR outside its I/O ring. Now each board exports
its CONTRIBUTION and reads the resolved bus back:

```verilog
    output wire BMux_00__drv,      // in the board
    input  wire BMux_00
    ...
    assign BMux_00 = BMux_00__ContA | BMux_00__IFU | ... ;   // in the machine
```

which is exactly what the open emitters compute, in one LUT level. The gate
is `verilator --lint-only` with `-Wno-MULTIDRIVEN` REMOVED: it reports zero.

**Distributed clocks become clock enables.** The Dorado fans a clock out to
every board and every flip-flop is clocked from that ECL net -- 1,201
packages across ten part types. Synthesised literally that is 1,201 gated
clocks off combinational logic, which no fabric can route. Every clocked cell
now runs on `sys_clk`, the fabric clock, and uses the modelled clock net as
an ENABLE:

```verilog
  reg  ck_d;
  always @(posedge sys_clk) ck_d <= p9;
  wire ck_en = p9 & ~ck_d;
  always @(posedge sys_clk) if (ck_en) q <= {p12, p11, p10, p7, p6, p5};
```

Standard oversampling, faithful as long as `sys_clk` is faster than the clock
net -- `dorado_machine` divides by four to guarantee it. Asynchronous inputs
(MR, S/R, CL') are level-tested on the same edge, which keeps them out of the
fabric's reset network. The two DRAM cells got the same treatment and their
writes are synchronous now, so they INFER BLOCK RAM instead of 309 packages'
worth of latches.

`sys_clk` is threaded by the generator: a cell that declares the port gets it
connected, and boards and the top module pass it down. It is not a Dorado
signal and does not appear in any `.bp`.

## Reference: where the clock comes from

Worth knowing, because the obvious answer is wrong. **The master clock is
GENERATED ON THE BASEBOARD, not fed to it**, and the chain is:

```
h06 MPQ3303   an analog VCO -- transistor quad, control voltages from a cable
  -> h05 MC1660    shapes VCOPhase0/1
  -> g05 MC1660    one input, both outputs: two ANTI-PHASE clocks
  -> g04+h04 MC1690  cross-coupled, a Johnson counter dividing by four
     g03+h03 MC1690  the same, delayed on the other phase
  -> StartClockPulse' (g04.Q') and EndClockPulse (h03.Q)
  -> j02 MC10210 ORs them -> l01/k01/j01/i01/h01 -> CLK.ph', CLK.mc', ...
```

`CLK.InBase` is how the BaseBoard receives that distributed clock BACK on its
own C9 -- the same pin every other board takes it on -- so the wrapper loops
it from `CLK.OutBase'`, which is what the backplane does. Driving C9 from
outside reaches ten nets and stops.

**Only the VCO is substituted**, and it is the one substitution in the design:
an analog oscillator has no digital behaviour to transcribe (resolved as
logic, its loop simply latches), and an FPGA has no VCO either -- a clock
comes from a pin or a PLL. `cell_MPQ3303` is therefore a divider off
`sys_clk`, and everything downstream of it is the board's own logic.

An earlier version injected a clock at `dStartClockPulse` (a real backplane
input into j02) because MC1690 had no model. That is gone. If it is ever
needed again, note the trap it produced: once MC1690 WAS modelled,
`StartClockPulse'` idled high, the OR at j02 stuck high, and the injected
clock stopped reaching anything -- the toggle count went from 27 to 0.

## Reference: how the backplane was derived, and why by name

`tools/sil_backplane.py` builds the top module; this is the evidence behind
it, kept so nobody has to re-derive it.

**The backplane is NOT straight-through.** The clock distribution proves it in
one line -- the BaseBoard drives each board's clock from a different pin while
every receiver takes it on pin **C9**:

```
CLK.ph'   BaseBd=C16 -> ProcH=C9      CLK.mc'   BaseBd=C20 -> MemC=C9
CLK.pl'   BaseBd=C13 -> ProcL=C9      CLK.mx'   BaseBd=C21 -> MemX=C9
```

Of 639 net names on more than one board only 446 sit at the same pin, and 182
pin positions carry different nets on different boards (`E90` is `MemBase.2`
on the processor and memory boards, `TNIA.06` on Control). **A pin number
cannot identify a signal. The name is the connection** -- which is what PARC's
flow assumed: boards were drawn against agreed names, the router assigned pins
per board, and the backplane was wired to suit.

Name matching then produces a machine that hangs together, which is the real
check on it. ProcH, ProcL, ContB, MemC and MemD have **2-5 unpaired nets each**
out of ~175, and the boards with many unpaired ones are unpaired for a reason
you can name: DskEth's 72 are the disk tag bus, the differential data pairs
and the ethernet transceiver; DispY/DispM's are monitor DACs and sync; the
BaseBoard's 80 are its own ACP bus, serial lines and grounds. Those go to
connectors ON the backplane, not to another board. The BaseBoard's paired
nets are the whole of how it reaches the machine, and they read exactly as
the Hardware Manual describes it: the clock star, `CPAddr`/`CPIn`/`CPOut`/
`CPStrb` to Control only (that is CPReg), and `DMuxClk`/`DMuxData` at E186/
E187 on all ten boards.

**The caution the earlier plan did not have: 115 shared nets are driven by
more than one board.** That is not a conflict to resolve, it is ECL
open-emitter wired-OR working as designed -- `BMux.00-15`, the B bus, is
driven by ContA, IFU, MemC, MemD, MemX and ProcH/ProcL together. In Verilog
these want `wor` nets. Verilator 5.044 accepts `wor` but raises MULTIDRIVEN,
which is fatal by default, so the emitted file brackets the declarations:

```verilog
/* verilator lint_off MULTIDRIVEN */
wor BMux_00;
/* verilator lint_on MULTIDRIVEN */
```

That works -- with the bracket in place and MULTIDRIVEN *not* waived on the
command line, the assembled machine reports it on none of the 83 wired-OR
nets. It reports it on eight others; see the known defect below.

Mind which boards can be present together when reading that count.
**msa/PCMSA are two generations of storage-module board** and appear to
co-drive 19 nets only because the listing reads all sixteen board files; a
machine has one or the other. **DispM and DispY are NOT such a pair** -- see
above, the colour board plugs into the monochrome one and a colour machine
has both.

`tools/sil_backplane.py --emit` does exactly this, and `--json OUT` dumps the
same netlist as data -- every net with the boards and pins it touches, its
drivers, and a `wired_or` flag.

**The mechanism is verified, not assumed.** Verilator 5.044 was checked to
simulate this shape correctly -- a cell instance output tied straight to a
module `inout`, two such boards on one `wor`, both reading the resolved value:

```
a=0 b=0 -> bus=0 sa=0 sb=0      a=1 b=0 -> bus=1 sa=1 sb=1
a=0 b=1 -> bus=1 sa=1 sb=1      a=1 b=1 -> bus=1 sa=1 sb=1
```

Two things NOT to use:

- **The slot numbers.** `Slot` and the `.nl` headers give one, but BaseBd,
  ContB and IFU all claim `s01`: it is whatever the last build was run with,
  not the machine's layout. It affects only the `#sNN-` prefix in the `.nl`
  node names, which the name-matching does not need.
- **`DoradoDocs/schematics/Backplane.pdf` as a data source.** It IS the real
  backplane wiring chart (8 pages, `BPRight01-04.sil` / `BPLeft01-04.sil`,
  Mike Overton, 7/24/80, pin rows x board columns) and its `.sil` source is
  NOT in the archive -- `[IO]<DoradoBuildDocs>` has only `bpwirelist01/02.sil`
  and cable drawings. The PDF is a scan run through Acrobat OCR, and the OCR
  mangles exactly what matters (`Mel.08` for `Md.08`, `106.14` for `IOB.14`,
  `BO` for `80`). Use it the way it was used here: as an INDEPENDENT SPOT
  CHECK. Its pin 24 = `RSTK.0` and pin 5 = `StartCycle'`, and `ProcH.bp` says
  `RSTK.0: C24` and `StartCycle'a: C5`. That agreement is what confirms the
  `.bp` reading is right.

## Fixed: BaseBd's `MCD_0..7` were double-driven

Found while linting the assembled machine, and pre-existing (the committed
board file had the same eight). BaseBd's internal 6502 data bus `MCD_0..7`
each had two continuous drivers: `emit()` built its on-board wired-OR stubs
from the pins the WIRE LIST marks `o` and redirected only those, so a pin the
wire list marks `i` whose CELL declares an output still connected straight to
the net -- the `assign MCD_7 = stub | stub | ...` racing that cell output.

Same shape as the ASSIGNIN bug below, same fix: `Generator.drivers_in_rtl()`
is now THE definition of "this pin drives", used by `classify()` for port
direction and by `emit()` for wired-OR resolution, so the two cannot disagree.
MULTIDRIVEN with the waiver removed went 8 -> 0.

It moved five nets from `inout` to `input`, and that is the right answer:
`OISData`/`OISData'` (BaseBd), `Collision`/`RcvData` (DskEth) and `Syn+5V`
(Music) are marked `o` in the wire list at pins belonging to **`AUGATCG16`,
an Augat connector block, and `SIPpackage`, a resistor network**. Neither is
a logic part, so nothing in RTL drives them -- `Collision` and `RcvData`
arrive from the ethernet transceiver over a cable. They are board inputs, and
the top module exposes them.

## Tim found a whole class of bug: the COMMON pin

He read `cell_MC10100` and saw that pin 9 -- the strobe MC10100 feeds into all
four NOR gates -- had been wired into the first gate only, and that
`cell_MC10101` had the same fault with its common pin 12. Both cells were
written from EclDict's PIN BLOCK, which names a common input ONCE, on the
first gate, and leaves you to know it is shared.

**The dictionary says it a second way, unambiguously.** Beside the pin blocks
is a gate summary per part, with every gate's complete input list:

```
MC100
[G (4 5 9)>(2), (6 7 9)>(3), (10 11 9)>(14), (12 13 9)>(15) : 1 3.3 ]
```

`tools/sil_check_cells.py` (`make -C verilog cell-check`) compares every
combinational cell against those lists, inlining intermediate wires so a
factored cell is checked like a flat one. It found **seven** bugs across
**270 packages** -- Tim's two and five more:

| part | pkgs | what was wrong |
|---|---|---|
| `MC10101` | 54 | common pin 12 in one gate of four (Tim) |
| `MC10100` | 25 | common strobe pin 9 in one gate of four (Tim) |
| `MC10125` | 66 | each channel read one of its two DIFFERENTIAL inputs, and the fourth channel was not modelled at all -- its output tied to zero |
| `MC10195` | 47 | common pin 9 in one gate of six, AND the wrong function: it is an XNOR, not a NOR |
| `MC10124` | 41 | common strobe pin 6 missing from all four gates, AND the two output senses swapped |
| `MC1660` | 28 | my own: gate a drove pins 3 and 15, gate b drove 14 and 2 -- the two halves crossed |
| `MC10118` | 9 | two independent gates modelled as one term feeding both outputs |

The data book settled the ones where the FUNCTION mattered, not just the
wiring -- `DoradoDocs/datasheets/ON_MECL_DataBook_DL122_rev7.pdf`, 474 pages,
which is worth having for everything else too:

- **MC10124**: "When the common strobe input is at the low logic level, it
  forces all true outputs to a MECL low logic state" -- so each channel is
  AND(data, strobe), both senses brought out.
- **MC10125**: the pin assignment shows differential pairs, `AIN`/`A-bar-IN`
  on pins 3 and 2 and so on, with one TTL output each.
- **MC10195**: its truth table is an XNOR, and EclDict names pin 9 twice,
  `Invert'` and `Buffer` -- the common pin CHOOSES which the part is. That is
  the "Inverter/Buffer" in its name.

**What the MC1660 fix then exposed.** With that cell correct, the machine
stopped clocking -- and the earlier "self-clocking" result turned out to have
depended on the bug. The BaseBoard's VCO is a transistor quad (`MPQ3303`)
cross-coupled with an MC1660, and its job on `VCOPhase0`/`VCOPhase1` is to
PULL THEM DOWN. A wired-OR resolves as an OR, which can only pull up, so the
loop latches high and the clock never starts. Since the VCO is already a
documented substitution -- an analog oscillator has no digital model -- the
substitute now drives those nets outright: `Generator.OVERRIDE_DRIVERS`, one
part, one net pair, stated in the generator rather than hidden in a cell.

**Then the same audit on the SEQUENTIAL cells**, which the first version
skipped. The dictionary has a parallel summary for them, and it names the
clock and asynchronous set/reset outright:

```
MC231
[FF 10 {1.1 .8}>(14 15) : CLK (9 11) (1.5 3.7) RS (12 13) ...]
[FF  7 {1.1 .8}>(2 3)   : CLK (9 6)  (1 3.7)   RS (5 4)   ...]
```

**Pin 9 is in BOTH clocks** -- Tim's class again, in the biggest flip-flop in
the machine. `cell_MC10231` (152 packages) and `cell_SE10231` (8) used only
each half's own clock pin and explicitly dismissed pin 9 as unused. The data
book says how they combine (DL122 rev 7, MC10231): "Each flip-flop may be
clocked separately by holding the common clock in the low state and using the
enable inputs for the clocking function. If the common clock is to be used to
clock the flip-flop, the Clock Enable inputs must be in the low state." Either
can clock it while the other is held low, which is an OR.

Fixing it moved the machine: `machine-test` went from 27 signals to **34**,
and the new ones are a coherent group -- `LoadSoutE'`/`LoadSoutO'`,
`ShiftSinE`/`ShiftSinO`/`ShiftSoutE`/`ShiftSoutO`, the memory system's
even/odd shift path.

The check is bounded by each cell's DECLARED pins, because a cell only
declares what the boards wire: MC1690 has a second clock input that no Dorado
board uses, and reporting that would be noise.

**What the checker cannot do**, so that its silence is not over-read: it
checks CONNECTIVITY, not function -- a gate that ORs where it should AND uses
the same pins. And 19 cells legitimately read MORE than `[G]` lists, because
that summary omits selects, enables and carries (MC10158's pin 9 is SELECT,
MC10174's pin 14 is ENABLE, MC10180's carry-in); those are reported
separately, not as errors.

## Reference: OR/NOR output polarity -- for the OR/NOR family only

> **BOUNDED 2026-08-18.** This rule is right for the OR/NOR parts it was
> derived from and **does not generalise** -- MC10104's `OUT` pins carry the
> AND and MC10121's `OUT` pin the plain OR-AND. Nor can PARC's net naming
> decide it, unanimity notwithstanding. See "Correction: role `OUT` is the
> INVERTING output does NOT generalise" above for what does: the data book's
> logic diagram, read by coordinates.

A third of the packages in this machine are OR/NOR gates, and getting the
sense backwards produces a machine that almost works. The rule below is
confirmed against Motorola's own sheets for that family, so those cells can be
written mechanically.

**For an OR/NOR part, EclDict role `OUT` is the INVERTING (NOR) output and
role `o` the non-inverting (OR) output.**

| part | EclDict | datasheet pin labels |
|---|---|---|
| MC10101 | `a,OUT,2 > a,o,5` (and b,c,d) | pin 2 = **A-bar-OUT**, pin 5 = AOUT |
| MC10212 | `a,OUT,3,4 > a,o,2` | pins 3,4 = **A-bar-OUT**, pin 2 = AOUT |

Eight gates across two parts, unanimous. (MC10101 from
`futurlec.com/Datasheet/Motorola/MC10101.pdf`, MC10212 from the same
directory; both are the Motorola technical data sheets with the DIP pin
assignment drawn out.)

**The one exception, and it is not really one.** A part with only a single
output sense uses `OUT` for all of them, so the letter cannot discriminate:
MC10110 (3-output OR) and MC10111 (3-output NOR) have IDENTICAL dictionary
entries. There the PART NAME decides, which is what `cell_MC10210` and
`cell_SE10211` do.

**What this fixed.** `cell_MC10105` had its two senses the wrong way round --
34 packages, 31 of them in the machine -- and `cell_MC10103` gave one gate's
two outputs the same expression, which cannot be right for a gate the
datasheet says "has both OR and NOR outputs". Both corrected.

**What did not settle it, so nobody repeats the work:** PARC's net naming
looks decisive and is not (on MC10212, eight gates put the primed net on the
`nout` pin and five on `out` -- a gate fed already-inverted inputs is named
for its function, not its pin sense), and the MECL Pocket Book scan that
`cells/PARTS.md` cites carries functions and schematics but no pinout tables.
Go to the per-part Motorola technical data sheets instead.

## The BaseBoard's 6502 boots (2026-08-17)

`make -C verilog baseboard-test` is the gate. It asserts four things at once,
and they were derived separately, from four different sources:

* the 6502 addresses `0xFFFC`/`0xFFFD`;
* `0xA7` and `0xF3` come back on `MCD`, so the vector reads `0xF3A7`;
* it then FETCHES from `0xF3A7`; and
* the bytes that follow are the ROM's own reset routine --
  `CLD`, `LDX #$00`, `LDA #$00`, `STA $00,X`, `DEX`, `BNE $F3AC`, byte for byte
  what `chm/disassembly/bb_F000-FFFF.s` disassembles at that address.

The fourth check is the one that matters. A 6502 that fetches its vector but
latches nothing still walks a plausible-looking address bus -- during bring-up
it walked `0x0000` upward for thousands of cycles, with a healthy 5,941
transitions -- so the gate reads the OPCODES, not the addresses.

Six things had to be true together, and each was mutation-tested by breaking
it deliberately and confirming the gate fails, each with its own message:

**1. Resistor packs are not a cell, and the machine's constant 1 comes from
one.** `SIPpackage` pins DRIVE on some boards and are the tie point on others
-- seven of the eight are used both ways -- so no fixed set of port directions
fits, and the cell that existed declared all eight as inputs and drove
nothing. So `TTLTrue.A`..`E`, which BaseBd g47 supplies and every TTL counter,
flip-flop and enable on the board counts from, sat at zero. The generator
resolves these now (`sil_to_verilog.py sip_pull`): a SIP is resistors from a
COMMON pin to the rest, the common is found by what it is CONNECTED to -- a
power net, or a reference the board makes for the purpose (`True`,
`ECLTrueA`) -- and every other pin is held there. Two exclusions matter. A net
held by a pull-UP pack and a pull-DOWN pack at once is a resistive DIVIDER, a
bias network at an analog input, so neither contributes and the net stays
open: that is DskEth's `RcvData`, the Ethernet receiver's own input, which it
would otherwise force high. And the common may never be a net the pack itself
drives -- a pack that MAKES a reference calls it something like `TTLTrueA`,
and taking that as its own common emits `assign X = X;`, a wire that inputs
its own output, which Verilator accepts and which then fails to settle tens of
thousands of cycles later on a different board.

**2. The supply rails have to be stated.** A wire nobody assigns reads zero,
and 1,010 nets across the sixteen boards are a gate's enable, preset or count
input tied straight to VCC. `VCC`/`VDD` are 1; `GND`, `VEE` (-5.2 V), `VTT`
(the -2 V ECL terminator) and `VBB` (-1.3 V) are 0.

**3. The ROM decode is a wire-wrap strap, and the netlist states it
geometrically.** `Rom0'`..`Rom7'` come from the '138 at g11, whose selects
`RSA.0/1/2` are driven by nothing at all: they arrive on c07, an Augat header.
What a header does is decided by which wires a technician wrapped onto it, and
that is not in the netlist -- but the GEOMETRY is. Every pin in the wire list
carries an `{x,y}`, and a jumper position is a COLUMN of pins at one x. At
c07, `MCA.11`/`MCA.12`/`MCA.13` sit directly across from
`RSA.0`/`RSA.1`/`RSA.2`. `tools/firmware_eproms.py` had derived exactly that
strapping by asking what tiles the address space with 2K parts; the header
says it outright.

The generator takes a strap only where the netlist forces it (`jumper_straps`):
a column of exactly two pins where one net has no other source on the board.
A column of THREE is a choice and is left alone -- MemX's b14 offers
`RTMapAd.1a` or `VCC-47`, msa's e26 picks `ChipsAre4k` against `ChipsAre16k` --
because guessing there would be inventing a machine configuration.

**4. The EPROM bytes are stored BIT-REVERSED, and the 1987 chip dumps prove
it.** The wire list puts the 2716's pin 9 on `MCD.7` and its pin 17 on
`MCD.0`, while the 6502's pin 26 (DB7) is on `MCD.7` and pin 33 (DB0) on
`MCD.0`. The data sheet calls 2716 pin 9 `O0` and pin 17 `O7`, so the ROM's
least significant output feeds the processor's most significant data bit: the
byte arrives reversed, and the chips were blown reversed. The address lines
are NOT crossed -- pin 19 is A10 on both the board and the data sheet -- so
this is a deliberate data-bus reversal, not a numbering artifact.

`firmware/B-08.BIN`, read bit-reversed, is byte-for-byte identical to
`doradobaserom.mb!13`'s `0xF000` block, all 2048 of them; `B-10.BIN` reversed
gives NMI=`0xF000`, RESET=`0xF3A7`, IRQ=`0xF2A2`, that image's own vectors. Two
artifacts sharing no lineage -- a chip read in 1987 and a `.MB` from the
archive -- agreeing once the bits are put back. **That retires a note in
`tools/firmware_eproms.py` that those dumps were "a different set or a
different layout" because none had a plausible 6502 vector triple.** They had
one all along, and they land on exactly the four populated sockets:
B-08 -> c61 (0xF000), B-10 -> b61 (0xF800), C-08 -> f60 (0xC000),
C-10 -> e60 (0xC800). Only B-08 matches byte for byte; the other three are a
later build than the archived `.MB`, which the 1987 date on the chips would
predict.

**5. Tri-state parts must contribute NOTHING when they are not driving.** The
shared nets here resolve as an OR of their drivers, which is right for MECL
open emitters. A TTL bus is different: one part drives and the rest are in
high impedance, so a part that is not driving has to contribute zero and leave
the active driver's value intact. The 2716 already did this with its CS'/PD';
the 6502 and the 6532 did not, so every part on the bus contributed at once,
`MCD` read `0xFF` at every address, and the ROM might as well not have been
fitted.

**6. A ROM is a registered block RAM, and here that is also the part's
timing.** The BaseBoard enables its EPROMs from `MCClk'`, so a 2716 drives
only while phase 2 is high -- and the processor latches the byte as that phase
FALLS. On the real board the '138 and the 2716 take tens of nanoseconds to let
go, which is exactly the data hold the 6502 needs. In zero-delay RTL a
combinational output vanishes in the same instant the CPU latches: the reset
vector was read correctly off the bus, `0xA7` then `0xF3`, and the processor
still started at `0x0000` every time. One fabric clock of registered output
supplies the hold, and is what an FPGA block RAM gives you anyway.

**And a seventh, which is the power-on reset.** `PwrGood` is the comparator
saying the supplies have reached their thresholds, and it is the clear input
of the flip-flop at j08 that produces `MCReset'`. `cell_MPQ3303` asserted it
true from the first cycle -- "the supplies are up, which for a simulated
machine they are" -- which released the netlist 6502 before it had run a reset
sequence at all: it came up executing `BRK` off an all-zero bus and never
fetched a vector. Supplies ramp; it comes up low and rises.

Also fixed on the way: the netlist 6502 core needs the FAST fabric clock on
`clk` and the board's `MCPreClk` on `phi`. Both were wired to `MCPreClk`,
which gives the 1,725-node relaxation one settling step per phase, and it
never converges.

### The parts that had to be modelled

The whole BaseBoard clock and reset chain, from PARC's `TtlDict.Analyze` pin
lists: `SN74LS04`, `SN74LS74`, `SN74LS163`, `SN74LS175`, `SN74LS259`,
`MC12061` (the crystal oscillator -- a documented SUBSTITUTION, like the VCO,
driving a divider off the fabric clock), plus the plain TTL family the board
uses around them: `SN74LS00`, `SN74LS01`, `SN74LS08`, `SN74LS32`, `SN7486`,
`SN7438`, `SN74LS139`, `SN74LS151`, `SN74LS251`, `SN74LS157`, `SN74LS253`,
`SN74LS85`.

**`SN74LS01` and `SN7438` are OPEN COLLECTOR, and the dictionary does not say
so.** TtlDict groups parts by PINOUT, so the '01 sits with the totem-pole '02
and '28; only the part NUMBER says the output is a pull-down with no pull-up.
It matters because a shared net here resolves as an OR -- right for emitter
followers, which pull UP -- and these pull DOWN onto a net a resistor holds
high, which is an AND. There are 15 such nets across two boards and each has
effectively one driver, so a single open-collector output plus its pull-up
computes the gate's own function and the cells drive that; the wired-AND only
bites if a second driver is ever added. `BootMC'` is one of them, and it is
the D input of the 6502's reset flip-flop: with the '01 unmodelled that net
sat low and the processor was held in reset forever.

`make -C verilog cell-check` now reads **both** dictionaries. It was reading
only `EclDict.Analyze`, which left all eighteen new 74-series cells unchecked;
`TtlDict.Analyze` states its gates in the same `[G ...]` form. 50 cells
checked, 0 ignoring an input.

### A part that reads a bus it also drives

`cell_MCS6502` and `cell_MCS6532` present the two directions separately --
`dbo` out, `dbi` back -- because the RTL has no `inout` anywhere by design.
Nothing in the wire list can say which companion port goes with which pins;
that is a fact about the PART, so it is named in the generator's `READBACK`
table and connected to the resolved nets.

It must be connected to what the OTHER drivers put on the bus, not to the
resolved net. Inside the netlist 6502 `dbo` is combinational on `dbi` -- both
come out of the same relaxation -- so handing back the whole net is a wire
that inputs its own output. It settles perfectly while the part is off the bus
and stops settling the instant it drives one: the machine ran thousands of
cycles and then failed to converge at the 6502's first `STA`, reported against
a different board entirely. Excluding the part's own contribution is also what
the hardware means -- a driver does not read its own drive to learn what it is
driving -- and the wired-OR already gives every driver a private stub, so it
costs nothing and no delay. (Registering the readback instead works too and is
wrong: it delays the read path enough to miss the ROM's hold window.)

## The BOOT INTERFACE cross-checks against the C emulator (2026-08-17)

`make -C verilog cpreg-diff`: **176 strobes from the C emulator, 44 of them
loading CPReg, 0 mismatches.**

This is how a Dorado starts, and it is the second thing in the machine to be
checked against the software model rather than only against the netlist it came
from. The BaseBoard's 6502 has no access to IM or to the processor's registers.
It drives nine data bits (`CPOut.0-8`), a three-bit function code
(`CPAddr.0'-2'`) and a strobe (`CPStrb'`) across the backplane, and CPReg, the
microinstruction register, run and single-step are all built out of that.

Two derivations meet, neither taken from the other:

* the RTL, generated from PARC's wire list -- an MC10161 at ContA a01 decodes
  the function code into `CPStrb0'`..`CPStrb3'`, and MC10176 hex flip-flops at
  a03, f02, g02 and h02 latch `CPReg.00`-`.15` from `CPOut.0-7`;
* `apply_mcp_strobe()` in `dorado/src/baseboard.c`, written from PARC's
  `doradoio.mdefs` and `doradocpint.masm`.

`dorado/tests/cpreg_vectors.c` includes `baseboard.c` directly, so the vectors
come from the emulator's real decoder, and `tb_cpreg.sv` feeds the same strobes
through the gates. Four mutations were tried and each fails: leaving the
function code uncomplemented, reversing its bit order, sending the data byte
LSB-first, and latching on the leading edge of the strobe.

**Three things the netlist settles**, all of which had to be right for a single
vector to pass:

* **The function code arrives COMPLEMENTED and the strobes are active low.**
  ContA's decoder takes `CPAddr.0'` on its most significant select, so function
  0 presents 7 and comes out on Q7' -- which the board calls `CPStrb0'`. The
  naming is self-consistent once you see why.
* **MSB-first, here as everywhere.** `CPOut.0` is the data byte's bit 7: it
  traces back through `TCPBus.00` to `MCPBus.00`, which is a RIOT's PA7. And
  `CPReg.00` is the high byte's most significant bit -- which is what makes
  ContA's `CPStrb2'` loading `CPReg.00`-`.07` the same statement as the C
  emulator's "ABMux0 latches the HIGH byte".
* **`SetRun` comes from data bit 0.** The BaseBoard latches `TSetRun` from
  `TCPBus.07`, and MSB-first over `.00`-`.07` makes `.07` the least significant
  bit. The C emulator tests `data & 0x01`. Same bit, from opposite directions.

**And two things the netlist adds, which are gaps in the C model rather than
disagreements:**

* **Function 1 is not a no-op.** The C emulator calls it "Clock" and ignores
  it. ContA's `CPStrb1'` latches three real control bits at i02:
  `CP=UseCPReg`, `ClrReady` and `GetTLink`, from `CPOut.2`, `.3`, `.4` and
  `.7`.
* **A REMOTE host can drive this bus instead of the local 6502.** Three
  SN74LS157 multiplexers on the BaseBoard (d06, e06, f06) select between
  `MCPBus`/`MCPABus`/`MCPStrb` and `RCPBus`/`RCPABus`/`SelStrb`, which arrive
  on AM2615 line receivers from the `ACP*` cable, under a software-set
  `AHasCP` latch -- "A has the Control Processor". The C emulator models only
  the local path, which is enough to boot but is not the whole interface.

One correction to an internal comment, worth having: `baseboard.c` says each
MIR byte's extra bit "rides on the SetSS line during the strobe -- this is a
hardware multiplex". It is the same physical RIOT pin, PB7, but it is not a
multiplex. The netlist fans it out: continuously to `MCPBus.08` -> `CPOut.8`,
the ninth bit of the nine-bit slot, and separately into an SN74LS175 at g07
which latches it into `SetSS'` on the Control strobe. Same pin, two
destinations, one of them registered.

## ALL 36 BITS of the microinstruction load path agree (2026-08-17)

`make -C verilog mir-diff`: **88 microinstruction strobes, 0 field mismatches.**

Functions 4-7 are the four microinstruction bytes, and ContA's a01 leaves
Q0'-Q3' unconnected because they are decoded a different way. Six **MC10172**
dual decoders -- ContA g01/h01/i01 and ContB h04/i03/i04 -- are wired as 1-to-4
**demultiplexers**: the data bit enters on each half's ENABLE pin and the two
select lines route it to one of four outputs. Nine `CPOut` lines times four
function codes fan out to the thirty-six field lines `sRSTK.0-3`, `sALUF.0-3`,
`sBSEL.0-2`, `sLC.0-2`, `sASEL.0-2`, `sBLOCK`, `sFF.0-7`, `sJCN.0-7`, `sIMLH`
and `sIMRH`.

Against that stands `dorado_decode_mir()` in `dorado/src/disasm.c`, written from
the Hardware Manual's microinstruction format, byte by byte, from a document
rather than a netlist. Every bit lands in the same place:

| strobe | `CPOut.0` .. `CPOut.7` | `CPOut.8` |
|---|---|---|
| MIR0 (fn 4) | RSTK.1 RSTK.2 RSTK.3 ALUF.0 BLOCK FF.0 FF.1 FF.2 | RSTK.0 |
| MIR1 (fn 5) | ALUF.1 ALUF.2 ALUF.3 BSEL.0 FF.3 FF.4 FF.5 FF.6 | IMLH |
| MIR2 (fn 6) | BSEL.1 BSEL.2 LC.0 LC.1 FF.7 JCN.0 JCN.1 JCN.2 | JCN.7 |
| MIR3 (fn 7) | LC.2 ASEL.0 ASEL.1 ASEL.2 JCN.3 JCN.4 JCN.5 JCN.6 | IMRH |

which is `dorado_decode_mir`'s byte table exactly. Two of its comments are
confirmed outright -- "MIR0's extra bit is RSTK[0]" and "MIR2's extra bit is
JCN[7]" -- and the other two are named: what the C emulator calls the parity
bits P015 and P1631 are the board's `sIMLH` and `sIMRH`, the IM left and right
half.

`cpreg_vectors.c` strobes ONE byte at a time from a cleared MIR, so the fields
the C decoder reports are exactly the bits that strobe sets. Five mutations
were tried and each fails: assembling FF LSB-first, reversing RSTK, sampling
after the strobe releases instead of during it, leaving the function code
uncomplemented, and swapping BSEL for LC.

Two things worth keeping. **The field lines are SET inputs, not register
outputs** -- a decoder output is a level that stands while the strobe is
asserted, so they are read with the strobe low and released afterwards. And
**PARC's `.0` is the most significant bit of a field**, here as everywhere:
`sRSTK.0` is the RSTK bit the Hardware Manual puts in iw2 rather than iw0.

## A microinstruction is JAMMED INTO THE MIR AND READ BACK (2026-08-17)

`make -C verilog mirreg-diff`: **144 microinstructions jammed and read back, 0
field mismatches.**

`mir-diff` proved the decoders -- which `CPOut` bit reaches which field line.
This proves the REGISTER: that the bits stick, that four strobes accumulate,
and that what comes out is the microinstruction the C emulator says it is. It is
what the BaseBoard does 475 times during a cold boot, since it cannot write IM
directly.

**The MIR is a bank of set/reset flip-flops**, which the software model does not
show. Every field bit is half an MC10231:

```
  S = s<FIELD>    the BaseBoard's jam path, from the MC10172 decoders
  D = d<FIELD>    the execute path, from IM
  R = rMIRa       the reset
  C = clk0'..     the microinstruction clock
  Q = <FIELD>     the signal the datapath runs on
```

So a jam is: reset the lot, then SET the ones the four bytes call for. That is
exactly why the Control function carries a `ClrMIR` bit and why the four strobes
accumulate rather than each loading a register. `ClrMIR` is `CPOut.5`, the data
byte's bit 2 -- `data & 0x04`, precisely what the C emulator tests. All five
Control bits check out the same way: `rStop` from `CPOut.1` (0x40, ClrStop),
`StopAtT1` from `CPOut.2` (0x20), `Jam` from `CPOut.3` (0x10), `rCT` from
`CPOut.6` (0x02). The sixth is a naming difference worth knowing: the bit the C
emulator calls **Freeze** (0x08) the board calls **`NoDispatch`**.

**Three things this test got wrong first, all of them instructive:**

* **`rMIRa` crosses the BACKPLANE.** ContA makes it and ContB receives it, and
  both boards drive it, so it resolves as a wired-OR. Instantiating the two
  board modules side by side leaves ContA's own reset unconnected to its own
  flip-flops, and nothing clears. The fix is to use
  `sil_backplane.py --boards=ContA,ContB`, which resolves it -- so the test runs
  a real two-board machine, and `make -C verilog lint` now elaborates that
  configuration too.
* **BLOCK is wired INVERTED, and it is the only field bit that is.** For every
  other bit `s<FIELD>` goes to S and `<FIELD>` comes off Q. For BLOCK, `sBLOCK`
  goes to **R** (on both e23 and g16), `rMIRa` goes to **S**, and `Block` comes
  off **Q'**. The two inversions cancel, so it means the same thing -- but until
  the backplane reset reaches that S input, `Block` sits at 1 no matter what,
  which is what the first run showed on every vector.
* **The claim that this test holds the microinstruction clock stopped was
  false**, and is now stated correctly. Driving `CLK.ca'`/`CLK.cb'` with a
  square wave toggles them 999 times and moves `clk0'Bc` ONCE: the Control
  board's clock generator is gated by the run/stop state, which is idle here. So
  the machine being STOPPED is why a jam sticks, and this testbench does not
  exercise the Freeze path at all. Measured, not assumed -- the mutation that
  should have failed passed, which is how it came to light.

Five mutations were tried; four fail as they should (no ClrMIR between vectors,
ClrMIR never released, BLOCK polarity flipped, one of the four strobes dropped)
and the fifth is the clock one above.

## The REPLY path, specified -- and the C emulator does not model it (2026-08-17)

The BaseBoard writes the machine through `CPOut`/`CPAddr`/`CPStrb'` and reads it
through **`CPIn.0-3`**, four bits at a time. That side is not in the C emulator
at all, so there is no oracle to diff against and no test here -- writing one
from the netlist would only check the netlist against itself. What follows is
the specification, which is the useful output: it is what the read side needs
if it is ever implemented, and it is complete.

Four **MC10164** eight-input multiplexers on ContB (d04, d03, c03, b03) drive
`CPIn.0`, `.1`, `.2`, `.3`. The select is `CPOut.0/1/2` -- the TOP THREE BITS of
the byte the BaseBoard is presenting, MSB-first as ever, so `data >> 5`. No
strobe is involved: the path is combinational, and the BaseBoard reads it by
setting `CPOut` and looking.

That gives a 32-bit status window read as eight nibbles:

```
select | CPIn.3        CPIn.2         CPIn.1        CPIn.0
-------+---------------------------------------------------------
   0   | RBMux.03      RBMux.02       RBMux.01      RBMux.00
   1   | RBMux.07      RBMux.06       RBMux.05      RBMux.04
   2   | RBMux.11      RBMux.10       RBMux.09      RBMux.08
   3   | RBMux.15      RBMux.14       RBMux.13      RBMux.12
   4   | RamPEenable   MDPEDly        IMLHPEDly     IMRHPEDly
   5   | MemPEenable   MemPEDly       RAMPEDly      IOPEDly
   6   | MDPEenable    DoradoStopped  bIMLHPE       bIMRHPE
   7   | StopMIRClkEn  IOPEenable     IMLHPEenable  IMRHPEenable
```

Selects 0-3 are the 16-bit **`RBMux`** read-back bus a nibble at a time -- how
Midas reads the machine's registers -- and 4-7 are the parity-error enables and
delays plus **`DoradoStopped`**, which is the counterpart of the C emulator's
`dorado_running`. It is read on `CPIn.2` with select 6, and that was checked:
the multiplexer routes it.

**Two things measured while establishing this**, both worth keeping:

* **The Control latch bits are where the C emulator says.** Strobing function 0
  with one-hot data and watching ContA's j02: `rStop` follows 0x40, `Jam`
  follows 0x10 and `NoDispatch` follows 0x08 -- the emulator's ClrStop, Jam and
  Freeze masks exactly, with `NoDispatch` being the board's name for the third.
* **`DoradoStopped` stays asserted throughout**, which is correct and not a
  fault: the machine has no clock and no microcode, so it IS stopped. The run
  logic develops only once `clk0'` runs, which needs a machine that is going.

Nothing here contradicts the C emulator. Function 1 -- `CP=UseCPReg`,
`ClrReady`, `GetTLink` -- is what selects between these sources and CPReg for
some reads, and remains the one function the C model treats as a no-op.

## THE MACHINE RUNS (2026-08-17)

`make -C verilog run-test`. Over 40,000 fabric cycles after PARC's own boot
sequence: **`clk0'` 2,493 edges, `clk2'` 4,987 -- exactly twice -- `Phase0`
2,494, `StartCycle'a` 2,494, and `DoradoStopped` clear.** The Control section is
executing microinstruction cycles.

Everything before this proved the machine could be WRITTEN. This is the first
time it does something by itself, and it reports so on the readout bit PARC's
`doradoio.mdefs` calls `Stopped`.

**One thing unlocked it, and it is the only mutation that fails the test:
ClrStop and SetRun must go in the SAME Control byte, 0x41.** `rStop` is a LEVEL
out of the latch at ContA j02 and it lasts only until the next Control strobe.
PARC's `DoDoradoMicroInst` issues ClrStop and SetRun as separate strobes, so by
the time SetRun lands ClrStop has been withdrawn -- and `Stop` re-latches
`dStop` before the machine gets going. Issued together, the stop latch is held
clear across the moment the run latch sets.

That the real machine does not need this is presumably a timing matter, or
something a board outside ContA/ContB supplies; it is the obvious loose end.

### Three earlier conclusions this corrects

The previous section here said the machine could not start because "the clock
enable is clocked by the clock it enables" and "`preRunClk'Bb` is an OR that
`bCLKEnable'a` gates", so nothing could ever tick. **That was wrong, and
measurement is what showed it.** `preRunClk'Bb` toggles 187 times per 3,000
fabric cycles and `RunClk'a` with it; `Run'` DOES clear when `SetRun` arrives.
The stuck term was never `Run'` -- it was `Stop`, re-latching `dStop`.

Two hypotheses recorded there are still correctly eliminated (`SetRunRfsh`, and
a complementary power-up state), and one recorded next step turned out to be the
right one: **work forward from the free-running clock rather than back from the
stall.** ContA's l01 takes only `CLK.ca'`, so `prepreclk'a/c/d` run
unconditionally, and following them forward is what showed the run latch working
all along.

### Two things that look load-bearing and are not

Both were measured, because both were assumed first:

* **The microinstruction PARITY BITS.** The extra bits of MIR1 and MIR3 are the
  left- and right-half parity -- `sIMLH`/`sIMRH` on the board, "P015"/"P1631" in
  the C emulator -- and they really do reach two MC10170 parity generators on
  ContB: `IMLHPE'` tracks the MIR1 extra bit exactly, and `Error'` responds. But
  setting either wrong does NOT stop the machine here; it runs 40,000 cycles
  either way. The path needs `IMLHPEenable`, which comes off the Midas
  diagnostic-mux chain and is 0 in this configuration.

  > **This one turned out to be load-bearing after all, and the last sentence
  > is why.** Enable it -- `DisableDoradoErrors`, one of the two manifold
  > words PARC's boot ROM writes at power-up -- and the parity path becomes
  > the thing that FREEZES THE MIR, which is what lets a jammed
  > microinstruction execute at all. See "What holds the MIR" above. It is
  > still true that it does not stop the machine; it stops the MIR clock.
* **The `Clock` function.** `DoDoradoMicroInst` opens with
  `DoClock(InhibitCAHolds+ClrReady)`; removing it changes nothing.

### What the run test asserts

Not merely that something wiggles. `clk2'` must run at exactly twice `clk0'`,
which is the two-phase relationship the whole machine is built on, and `Phase0`
and `StartCycle'a` must each step once per `clk0'`. Those ratios came out
2,493 / 4,987 / 2,494 / 2,494 -- within an eighth of exact on every one.

## FOUR BOARDS, and the microinstruction is on the datapath (2026-08-17)

`make -C verilog datapath-test`. `dorado_proc` is ContA, ContB, ProcH and ProcL
wired by the backplane; the boot sequence is tb_run's, but the microinstruction
jammed is a distinctive one rather than all zeros. Two questions, both answered:

**The clock reaches the datapath, exactly.** ProcH's `Clock0'Bc` and ProcL's
`Clock0'Bd` each moved 2,493 times against the Control boards' 2,493 -- asserted
as EXACT equality, not a tolerance. The processor boards step in lockstep with
the Control section.

**And so do the fields.** Jamming `MIR0=0xE0+extra`, `MIR1=0xE0+extra`,
`MIR2=0xC0`, `MIR3=0x70+extra` puts, at the processor:

```
  RSTK = 1111    ALUF = 0111    BSEL = 011    ASEL = 111
```

which is exactly what the `mir-diff` table predicts. That one read exercises the
whole path end to end: the BaseBoard's nine-bit bus, the function decode, the
MC10172 demultiplexers, the MC10231 register, and the backplane.

**Some fields cross COMPLEMENTED and some do not.** `BSEL'` and `ASEL'` are
primed on the backplane; `RSTK`, `ALUF` and `LC` are not. That is the boards'
own choice rather than a convention, and it is read off the generated port names
rather than assumed -- reading BSEL uncomplemented is one of the three mutations
that fails this test, along with dropping the MIR2 strobe and splitting ClrStop
from SetRun.

## IM, mapped -- and it confirms the C emulator's Write-IM model exactly

**IM is on ContB, and it is 144 `F10415A` packages.** Each is a 1024 x 1 ECL
RAM, so 144 x 1024 = 147,456 bits = **4096 words x 36 bits**, which is the
34-bit microinstruction plus its two parity bits. That is the same 36 the `.MB`
format implies -- `MB_MEMORY` declares a storage width of 64 for a logical 36 --
now confirmed from the hardware.

The array's 144 packages resolve to **36 distinct data outputs, four packages
each** (four banks of 1024), and those 36 outputs are the microinstruction, one
signal per bit:

```
dRSTK.0-3  dALUF.0-3  dBSEL.0-2  dLC.0-2  dASEL.0-2   (17)
dBlock'  dFF.0-7  dJCN.0-7                            (17)
dIMLH  dIMRH                                          (2 parity)
```

**Those outputs are the `d<FIELD>` lines that feed the MIR's D inputs.** So the
whole core is now one picture: IM -> `d<FIELD>` -> the MC10231 microinstruction
register -> `<FIELD>` -> the backplane -> the datapath. And `dBlock'` comes out
of IM COMPLEMENTED, which is exactly consistent with the MIR flip-flop for BLOCK
being wired inverted (`sBLOCK` to R, output from Q') -- two independent oddities
that agree.

### The half-write is the C emulator's, bit for bit

The write enables split into `WEL'` and `WER'`, and each covers **exactly 18
bits**:

```
WEL'  dRSTK.0-3 dALUF.0-3 dBSEL.0-2 dLC.0-2 dASEL.0-2 dIMLH   = iw0 + RSTK.0 + parity
WER'  dBlock' dFF.0-7 dJCN.0-7 dIMRH                          = iw1 + JCN.7  + parity
```

which is `cpu.c`'s Write-IM comment verbatim:

> `RSTK[3]` -- half-select. 1 = LH (writes iw0 + RSTK[0] of the destination),
> 0 = RH (writes iw1 + JCN[7] of the destination). `RSTK[2]` -- value of the
> secondary bit. `RSTK[1]` -- parity bit.

The data inputs confirm the rest of it. `RBMux.00`-`.15` each feed EIGHT
packages (four banks x two halves) -- the 16 bits from B. `RBMuxP` feeds eight
more: the parity bit, `RSTK[1]`. And two further nets feed four packages each,
one per half: `MidasOrRSTK.2` and `ContB04.sil+1` -- the SECONDARY bit, which is
`RSTK[0]` for the left half and `JCN[7]` for the right, exactly as the comment
says.

So the same `RBMux` bus the BaseBoard READS through `CPIn` is what IM is WRITTEN
from. That is the whole jam mechanism in one sentence.

## PARC's hand-coded microinstructions, decoded (2026-08-17)

`doradoboot.masm` carries an **IRTable**: microinstructions hand-written in the
five-byte format `DoDoradoMicroInst` jams, "to be jammed into IM and executed
for their side effects". Its own comment states the byte layout --

```
0:  RSTK.0,P015,JCN.7,P1631,0,0,0,0
1:  RSTK.1,RSTK.2,RSTK.3,ALUF.0,BLOCK,FF.0,FF.1,FF.2
2:  ALUF.1,ALUF.2,ALUF.3,BSEL.0,FF.3,FF.4,FF.5,FF.6
3:  BSEL.1,BSEL.2,LC.0,LC.1,FF.7,JCN.0,JCN.1,JCN.2
4:  LC.2,ASEL.0,ASEL.1,ASEL.2,JCN.3,JCN.4,JCN.5,JCN.6
```

-- which is the `mir-diff` table exactly, arrived at from the other side, and it
names the two parity bits `P015` and `P1631` just as `cpu.c` does.

Decoded (the source is `.RDX 2`, and `X^n.` is X shifted left n):

```
Nop#            70 01 0F 4C 40      IFUReset#        60 02 0F 44 40
CPRegToLink#    30 13 EF 04 40      Return#          60 13 E1 42 43
IMLHRSTK.0Is0#  60 33 EF 03 4F      IMLHRSTK.0Is1#   20 73 EF 03 4F
IMRHBLOCKIs0#   20 13 EF 03 4F      IMRHBLOCKIs1#    60 53 EF 03 4F
QFromCPReg#     30 13 EF C4 40      TFromCPReg#      70 03 0F 04 C0
ALUFM[0]FromQ#  30 05 09 C4 40      SetMcr#          30 02 0B 84 60
SetHoldTaskSim# 70 13 EF 84 40
```

**All ten with field comments decode to exactly what PARC says they are** --
`CPRegToLink#` really is `RSTK[0],ALUF[17],BSEL[0],LC[0],ASEL[4],FF[176],
JCN[201]`, and so on. That validates the whole five-byte format independently,
and gives a reusable vocabulary of real microinstructions to jam.

Two of them are the Write-IM pair, and their `JCN=177` octal is precisely the
encoding `cpu.c` derives from the Hardware Manual (JCN[1]=1, JCN[2:4]=7,
JCN[5:7]=111). Their RSTK values confirm the half-select and secondary bit:
`IMLH...Is0#` has RSTK=1 (RSTK[3]=1, left half), `IMLH...Is1#` RSTK=3 (left
half, secondary 1), `IMRH...Is0#` RSTK=0 (right half), `IMRH...Is1#` RSTK=2.

## A jammed Write-IM deposits into IM, and RSTK[3] selects the half

`make -C verilog writeim-test`. From a wiped array, the right-half Write-IM sets
17 right-half cells and **0** left-half; the left-half one sets 16 left-half and
**0** right. Checked both ways across all 147,456 cells. `RSTK[3]` selects the
half, precisely as `cpu.c` says.

**Getting the BASELINE right was the whole difficulty, and the first version of
this test was worthless because of it.** IM comes up with 17 cells set: from the
all-zero initial state the ContB write logic asserts its active-low enables for
a few cycles until the combinational logic settles, and the array takes a write
of whatever `RBMux` then held. The first test counted non-zero cells from time
zero, found 17, and called it a successful write. Four mutations passed, and a
no-stimulus control reproduced it exactly with the machine stopped.

**The enables are not stuck**, which was the next wrong theory and is worth
recording as such. MEASURED: after 2,000 idle cycles `WER'Aa`, `WEL'Aa` and
`preWE'a` all read 1, de-asserted, and across the next 20,000 cycles the array
takes nothing at all. It is a settling transient, not a stuck line -- and not a
fault in the hardware either, since Boot0 exists to load IM and the machine
never assumes IM holds anything at power-up. The fix was to the TEST: settle,
wipe, then measure.

(A third wrong theory, tried and discarded on the way: that PARC's
`PrepareProcessor` -- which really does exist, and whose comment is "do a lot of
microinstructions to clear out the cobwebs", forty of them -- was what settled
the enables. It is not. Pure idle settles them just as well.)

### What it does not prove -- and why: A JAM MUST BE SINGLE-STEPPED

The DATA does not come from CPReg (zeroing CPReg leaves the written pattern
unchanged) and nor does the ADDRESS (every write lands at IM[0]). Tracing that
produced the most useful correction of this whole sequence.

**The CPReg-to-B path, fully identified.** `CPReg.00/01/08/09` feed an MC10159
quad multiplexer at ContA b02, which drives `BMux.00/01/08/09`. Its SELECT is
`UseCPReg` and its ACTIVE-LOW ENABLE is `B_Link'`, and both come from a clocked
register at ContA c17 fed by the FF-field decoders. The decode itself is
`FF=Link_CPReg` at a17, an MC10100 over **`FA=1'`, `FB=7'`, `FC=6'`** -- exactly
the FA=1 FB=7 FC=6 that `cpu.c` documents for `B <- RWCPReg`. The hardware
decodes FF into FA/FB/FC sub-decodes and recombines them, just as
`ff_override_b()` does.

**Why it never asserted: free-running destroys the jam.** Measured, with the jam
in MIR and the machine STOPPED, `FF` reads `01111110` -- 176 octal, exactly
PARC's `CPRegToIM#` -- and `FF=Link_CPReg` asserts. Start the machine with
ClrStop+SetRun and one microinstruction clock later `FF` reads `11111111`: **the
MIR has reloaded from IM**, because `d<FIELD>` is the execute path and IM is
empty. The jam survives only as long as the machine is stopped.

**So a jam must be SINGLE-STEPPED, and that is what `SetSS` is for.** PARC's
`DoDoradoMicroInst` ends with `Control(SetRun, SS=ShouldSingleStep)` -- SetRun
WITHOUT ClrStop, plus SetSS -- which executes exactly one microinstruction from
the jam. Measured: single-stepping leaves `FF` at `01111110` and
`FF=Link_CPReg` asserted, where free-running does not.

**This corrects `tb_run`'s conclusion**, or rather bounds it. That test found
ClrStop and SetRun had to go in one Control byte, and that is right FOR
FREE-RUNNING -- it is how you get the machine to sequence continuously. It is
the wrong mode for a jam. Two different operations, and conflating them is what
cost the operand.

### The operand: it was never the FF decode, it was the MIR CLOCK

`make -C verilog operand-test`. **The jammed microinstruction's DATA comes from
CPReg**: `CPReg=a53c` puts `5ac3` on BMux and RBMux, `CPReg=1234` puts `edcb`,
and IM takes a different pattern for each.

The section this replaces said the open task was "work out which of b16's gates
drives `ContA04.sil+2`". **Three things about that were wrong**, and all three
came from following the wrong net.

* **`B<-Link'` does not come from b16 at all.** It is ContA **a13** pin 2, an
  MC10102 gate a = `NOR(FF=ReadLink, FF=Link_CPReg)`, driving `ContA04.sil+4`
  into c17 pin 7 -- and MC10176's `[FF 5>2, 6>3, 7>4, ...]` makes pin 7 the D
  input whose Q is pin 4, which is `B<-Link'`. b16 feeds c17's OTHER inputs:
  pins 5 and 6 (both `ContA04.sil+2`, giving `Link<-BMuxa` and `Link<-BMuxb`)
  and pin 10 (`ContA04.sil+5`, giving `UseCPReg`).
* **b16's decode needed no work.** The dictionary states it outright --
  `MC103: a,IN,4,5>a,o,2 ; b,IN,6,7>b,o,3 ; c,IN,12,13>c,OUT,9>c,o,15 ;
  d,IN,10,11>d,o,14`. Gates a AND b both drive `ContA04.sil+2`, an on-board
  wired-OR making one 4-input OR of `FF=WriteLink`, `FF=Link_CPReg`,
  `FF=BigBDispatch` and `FF=BDispatch`; gate c gives
  `ContA04.sil+5 = OR(CP=UseCPReg, FF=Link_CPReg)`.
* **The combinational half was already right.** `ContA04.sil+4` goes 1 -> 0 at
  the exact moment `FF=Link_CPReg` asserts. Measured.

**What was actually wrong is that c17 never clocked.** `clk1'Ca` gets ZERO
edges while a jam is in the MIR -- single-stepped and free-running alike. The
clock tree is fine: free-running it runs 1,240 edges, in step with `clk0'Ca`.

**Because within one microinstruction the MIR loads FIRST.** The order is
`h*clk0'` rises and the MIR takes `d<FIELD>` from IM, then `clk1'` rises and
c17 latches what the FF field decodes to. Coming out of a stop, clk0 is always
first. So IM overwrites the BaseBoard's jam half a microinstruction before the
register that would act on it ever clocks. Measured: free-running, `FF` goes
`01111110` -> `11111111` at fabric cycle 206 and `clk1'` does not pulse until
214.

### What holds the MIR: PARC's boot ROM sets it at power-up, through the manifold

`h*clk0'Ca` is ContA **c19**, and it is an OR of three things:

```
h*clk0'Ca = RepeatCurC | StopMIRClk | preclk0'Cb
```

`RepeatCurC` is the microcode's own hold (ContA f20, off `CAHold'` and
`SwitchUp`) and the BaseBoard cannot reach it. `StopMIRClk` is ContB **k02**:

```
StopMIRClk = NOR(ContB09.sil+3, StopMIRClkEn')
```

where `ContB09.sil+3` (ContB l03, an MC10121) is the **IM parity-error** term,
built from `IMLHPE'`/`IMRHPE'` -- the MC10170 generators at j20/j21 -- and their
enables; and `StopMIRClkEn` is ContB **g03**, an MC10176 whose D is `DMD.06`
and whose clock is `ManClk.7'`. Both come off the **manifold**, and PARC's boot
ROM sets both, once, in `doradomufman.masm`'s `InitManifolds` -- the group
"invoked as a group just after the power supplies are up and stable":

```
DisableDoradoErrors  .ADR (ParityEnables+030)^4   ; all except IM parity errors disabled
SetMidasStopMIRClk   .ADR (MidasStopMIRClk+20)^4  ; turn on MIR debug feature
```

**So the "MIR debug feature" -- freeze the MIR on an IM parity error, so the
failing microinstruction is still there for Midas to read -- is ALSO what makes
a jam executable.** A microinstruction the BaseBoard put into the MIR did not
come from IM and does not satisfy IM's parity, so the MIR clock stops and the
jam survives its own first clock. Measured: with the jam in the MIR,
`IMLHPE'` and `IMRHPE'` both read 0.

### The manifold, decoded -- and it agrees with PARC's table three ways

`tb_operand` drives it the way the BaseBoard does, so the decode is a gate
rather than a reading. `SetMufflerAddress` shifts **twelve bits, MSB first**,
each on a Clock strobe carrying `ShiftDMD` plus the bit in `DAddrBit`; the
BaseBoard turns those into `DMuxData`/`DMuxClk`; ContB's l22 and l20 (two
MC10176s) are the shift register, clocked by `bDMuxClk'` = `~DMuxClk`, so the
FIRST bit in ends at `DMD.00` and the last at `DMD.11`. Then `DoClock(UseDMD)`
/ `DoClock(0)` pulses `ManClk`.

| DMD | what it is | checked by |
|---|---|---|
| `.00 .01 .02` | board select; ContB answers to 000, which is `UseDMDEnable' = DMD.00\|.01\|.02` | matches `ControlBManifold = 0` |
| `.03 .04 .05` | register number, decoded by k01 into `ManClk.0'`..`.7'` | `ParityEnables` is 0 and `MidasStopMIRClk` is `7^6`; both land |
| `.06` .. `.11` | the six data bits, MSB first | `030` sets EXACTLY `IMLHPEenable` and `IMRHPEenable` and none of the other four -- PARC's comment, bit for bit |

`ManClk.0'` clocks ContB **c04**, which IS the ParityEnables register
(`IMRHPEenable`, `IMLHPEenable`, `IOPEenable`, `RamPEenable`, `MemPEenable`,
`MDPEenable` off its six Qs); `ManClk.7'` clocks g03, which is `StopMIRClkEn`.
Five mutations fail the gate: dropping either manifold word, shifting the
twelve bits LSB-first, reading BMux uncomplemented, and dropping a MIR strobe.

**BMux carries the COMPLEMENT of CPReg**, and that is the part, not a bug:
EclDict gives ContA b02's MC10159 `Q0'..Q3'`, four inverting outputs. Reading
it uncomplemented is one of the mutations.

`DMuxData`, `DMuxClk` and `UseDMD` are FORCED in the testbench because
**BaseBd l24 drives all three** and `dorado_proc` has no BaseBoard. They are
backplane signals a BaseBoard would supply, not internal state. (ContA drives
them too, from the microcode's `FF=UseDMD` and the Midas connector, which is a
different path and gated by `SetRun`.)

### Two cell bugs found on the way, both on this path

* **`cell_MC10121`, 44 packages** -- and one of them is ContB l03, which gates
  the MIR clock. Pin 10 was ANDed in as a term of its own; it is **shared
  between two OR groups**. The data book's DIP table labels it `A2IN, A3IN`,
  exactly as MC10117's pin 9 is `A2IN, B2IN` (which `cell_MC10117` already
  handles), and EclDict says it the way it always says a common input -- once,
  under role `c`. **Tim's common-pin class again**, and `cell-check` could not
  see it: the gate does read all eleven pins, only the grouping was wrong.
* **`cell_MC10104` gate d** gave BOTH its outputs the same expression -- the
  fault the handoff already records for `cell_MC10103`. p15 is the AND, p9 the
  NAND.

`make -C verilog cell-check` now also runs **`tools/sil_check_polarity.py`**,
which asserts the two properties that ARE universal, both by evaluating the
cells' own expressions over every input assignment:

* where the dictionary gives one gate both an `OUT` pin and an `o` pin, the two
  must be exact complements; and
* two parts that share a pinout and a `[G]` summary but are NOT second sources
  of each other must not have the same model -- which is what `cell_MC1662`
  copied from `cell_MC1664` was.

Second sources are recognised by part number with the family and speed letters
stripped (`MC10164`/`MU10164`, `SN74LS01`/`SN74S01`), not by the shared pin
block -- TtlDict groups by PINOUT, which is why `H01, H02, LS01, LS02, LS28`
sit on one line. Two genuinely-different pairs that legitimately share a model
are named with their reasons rather than silenced. 56 cells, 28 both-sense
gates, 47 distinct pinouts, 0 wrong; mutation-tested by putting the MC10104,
MC10102 and MC1662 faults back.

It also checks ADDRESS BIT ORDER on the memories -- see "The memories had their
address bits backwards" above.

It reports rather than asserts the OUTPUT DIRECTION, and two false positives of
its own are worth knowing: an expression it cannot evaluate (a ternary multiplexer,
a concatenation) means the pair cannot be COMPARED -- calling that "the same"
made MC10158/MC10159 look like copies when they are correctly two models.

### The IM ADDRESS, traced -- and `cell_MC1662` was the OR part's model

Every jammed Write-IM landed at IM[0], and the cause was one cell.

The path, from the array back:

```
IM (F10415A)  addressed by RA.01a..RA.10a + four chip selects
   RA.nn      ContB a12/b12/../l12, MC10211 NOR buffers of
   dRA.00'-11'  ContB e20/f20/f21/g20/g21/h20, SIX MC1662 packages:
                dRA.nn' = gate(TNIA.mm, SWa) wired-OR gate(SW'a, BNPC.mm)
   TNIA.04-15   from ContA -- twelve bits, 4096 words
   BNPC         the per-task saved PC; `SW` selects between them
```

**`cell_MC1662` computed OR.** MC1662 and MC1664 are a complementary pair with
the SAME pinout and the same `[G]` summary, and the dictionary separates them
by role letter alone -- `OUT` on the '62, `o` on the '64 -- which is exactly
what that letter is for. `cell_MC1664`'s own comment already said it ("the
MECL III pairing where MC1662 is the NOR part"); this file had simply been
written as a copy of it. The data book is no help: the MC1662 sheet was DELETED
from DL122 rev 7.

The circuit settles it independently. Those six packages are a 2:1 multiplexer,
which they are only as NORs. As ORs the second leg is `SW'a | BNPC`, and `SW'a`
is 1 whenever the machine is not task-switching, so every `dRA'` sticks high and
IM is addressed at 0 whatever TNIA holds. **33 packages**, across ContA, ContB,
IFU, MemC, MemX, ProcH and ProcL -- ProcH/ProcL's are `alub.00-15`, the ALU's B
input, and `RbAdr.4-7'`, the RM address.

With it fixed, `dRA` tracks `TNIA` exactly and `CPRegToLink#` works end to end:
`CPReg=002A` -> `BMux=ffd5` -> `Link[4:15]=fd5`.

### The memories had their address bits backwards

Three memory cells assembled their address LSB-first when PARC wires them
MSB-first. **`cell_F10415A` is IM** -- 144 packages -- and it is the one with
proof that owes nothing to convention:

* the board wires pin 2 to `RA.01a` and pin 12 to `RA.10a`;
* `RA.01a` comes from `dRA.01'`, which ContB's multiplexer takes from
  `TNIA.05`, the SECOND MOST significant address bit, while `RA.10a` traces to
  `TNIA.14`, the second least;
* so pin 2 carries the more significant bit -- and the cell had it as the
  least.

Measured: a Write-IM whose address lines read **195** deposited at **780**,
which is 195 with its ten bits reversed. Fixed, it deposits at 195.

`cell_F10145A` (**405 packages**, the biggest cell in the machine -- the 16x4
ECL register file behind TLink) and `cell_F10470` (the DRAM) had the same
inversion and are fixed with it. **The other seven addressed memories were
audited and are right**: `MB7071H` (RM and STK -- ProcH h06 takes `RbAdr.0` on
the dictionary's A0 pin), `F10414` (STK, which says `// A0..A7, MSB first` in
its own comment), `i2716`, and the three PROM parts, whose bit order
`prom-test` already pins. `cell_i2125` is the one genuine exception and is
named as such: its address is the BaseBoard's own `RA0..RA9` off a 6502 bus,
where 0 is the LEAST significant.

`sil_check_polarity.py` checks this now, so it cannot come back: for every
cell whose dictionary entry names `A0..An`, the address expression's first
element must be the pin the dictionary calls `A0`, with `i2125` exempted by
name and reason. Mutation-tested by reversing five memories' addresses in turn;
each is caught.

**The convention is stated by the repository's own cells.** `cell_F10414` --
the same family, the same board, the STK register file -- carries the comment
`// A0..A7, MSB first`, and `cell_i2716` was corrected for exactly this before
("PARC names those MSB-first: the dictionary's A0 is pin 19, the sheet's A10
... a reversed address would have scrambled the image silently").

**Why nothing caught it.** A consistently reversed address is a PERMUTATION:
writes and reads use the same mapping, so a machine that only reads back what
it wrote behaves identically, and `writeim-test` counts bits rather than
addresses while `operand-test` asserts `dRA == TNIA` rather than where the bits
land. It bites the moment IM is compared against something external -- a `.MB`
file, or the C emulator -- which is exactly what Boot0 is for.

### The machine single-steps, and `rStop` is why it took three tries

`make -C verilog step-test`. Steps two through five are each **`clk0'` 2,
`clk1'` 2, `clk2'` 4, `Stop` 1** -- one microinstruction, stopping again -- and
`Link[4:15]` reads `fd5` throughout, which is `CPReg=002A` complemented: the
jammed `CPRegToLink#` executed.

Three things had to be understood, and each first presented as the machine
being broken:

* **`SetRun` alone runs half a microinstruction.** PARC's
  `DoDoradoMicroInst` ends with SetRun and SS and no ClrStop, and that gives
  `clk0'` twice and `clk1'` not at all. It is not a fault: the phase generator
  comes out of reset with `sPhase0` held and `StartCycle` cleared, so the first
  window is the clk0 half. Every step after it is whole -- and PARC's
  `DoIRTableInstAndNop` says as much, jamming a Nop after every IRTable entry
  because "the Nop holds CPReg constant through T3 of the PREVIOUS
  instruction".
* **`ClrStop+SetRun` on its own FREE-RUNS.** `rStop` is a LEVEL out of ContA
  j02 latched from `CPOut.1`, and it lasts until the next Control strobe.
  Issued once and left, it holds the stop latch reset for ever: 26 `clk0'`
  edges in a window where a step gives 2, with `Stop` still 0. A following
  Control strobe without ClrStop is what lets the machine stop again. This is
  the other half of `run-test`'s finding -- sharing the byte is how you START,
  letting ClrStop go is how you STOP.
* **The machine runs during the strobes.** The first version counted clocks in
  a window opened after the last strobe and measured zero of everything, which
  looks exactly like a machine that never started. Count continuously.

And one correction made by a mutation that PASSED: the comment first claimed
all three of PARC's strobes were load-bearing. Dropping the middle one still
steps correctly, because `Control(0)` withdraws ClrStop just as
`Control(SetRun)` does -- it is the FOLLOWING STROBE that matters, not its
data. The gate now mutates by removing every following strobe, which does fail.

### `machine-test` was testing a week-old binary, and the machine does not settle

Two separate things, and the first hid the second.

**The gate ran a prebuilt `obj_dir/Vemu` and never rebuilt it.** On 2026-08-18
that binary was dated a week earlier -- it had been reporting on RTL from
before several cell fixes, and passing. `machine-test` depends on `all` now.

**Rebuilt, the assembled eleven-board machine does not converge.** It is
PRE-EXISTING and not the cell work above: stashing all four of this session's
cell changes and rebuilding gives the identical failure. Recorded as measured,
with the three things that narrow it:

* `--converge-limit 2000` does not help, so it is an OSCILLATION, not slow
  settling.
* **The same RTL settles fine under Verilator's event scheduler.**
  `make -C verilog converge-test` builds `dorado_machine` with the same eleven
  boards and runs 20,000 cycles with `CLK.ph'` toggling 2,501 times, both with
  an `always #1` clock and with an explicit `sys_clk = 0; #1; sys_clk = 1; #1;`
  loop. So the boards are not obviously at fault -- the difference is
  `--binary` (event scheduler) against `--cc` plus C++ `eval()`.
* Sweeping `probe_sel` the way the harness does is not it either; that was
  tried.
* `loop-check` still passes, so the candidates are the four structural loops it
  knows about and accepts: the processor's multiplexer chain feeding back, and
  a counter's carry returning to its own count enable on DispM, DispY and MemD.

`converge-test` builds in about five seconds against the harness's several
minutes of imgui C++, so it is the one to iterate on. The pair of gates is the
finding: one green, one red, same RTL.

### And `cell_MC10159` drove its outputs HIGH when disabled

With the address moving, `TNIA` still was not `Link`. Probing each of TNIA's
wired-OR contributions separately -- the generator gives every driver its own
stub, `TNIA_04__g24_3` beside `TNIA_04__g22_14` -- answered it in one run:

```
bit  Link  TNIA  contributions
 .04    1     1    g24_3=1 g22_14=1
 .05    0     1    h24_3=0 g22_15=1        <- Link says 0, the bus says 1
 .06    1     1    g23_3=1 g22_2=1
 .07    0     1    h23_3=0 g22_1=1
 .08    1     1    h22_3=1 i22_2=0 i21_14=1
 ...
```

**All twelve MC10121 selectors carried Link EXACTLY RIGHT.** Every wrong bit
came from two packages, ContA g22 and i21, both MC10159 multiplexers, both
reading 1 where they should have been silent.

`DoradoDocs/datasheets/MC10159.pdf` has the truth table, and its last row is
the one that was missing:

```
Enable  Select  D0  D1   Q
   L       L     X   L   H
   L       L     X   H   L
   L       H     L   X   H
   L       H     H   X   L
   H       X     X   X   L      <- DISABLED: the outputs go LOW
```

A disabled MECL part drives its outputs LOW, which is the entire point of an
enable on a part whose open-emitter outputs are tied into a wired-OR: a
disabled driver must contribute NOTHING. The cell had `~(en & mux)`, which
reads 1 when disabled and so held every bus it sat on to all ones. **67
packages**, 25 of them on ContA. With it fixed, `TNIA` is `Link[4:15]` bit for
bit.

The same sheet also confirms the select polarity that `cell_MC10158` and
`cell_MC10159` both carried a "VERIFY" comment about: SELECT high routes
D00/D10/D20/D30 (pins 6, 4, 13, 11), which is what both cells already did.

**The lesson is about the probe, not the part.** Four sessions of "TNIA is
wrong" would have said nothing; one run printing each driver's own stub said
which package and left eleven others exonerated. When a wired-OR reads wrong,
split it.

### Two gates were passing for the wrong reason, and this exposed both

`datapath-test` and `writeim-test` both jam and then FREE-RUN, which the
handoff says destroys a jam. They passed only because the MIR kept reloading
IM[0] -- the same cell bug. With a working address the machine walks IM and
they failed.

Both now do what a BaseBoard does before it ever jams: write the two manifold
words that hold the MIR. `datapath-test` also had to move its clock probe from
ContB's `clk0'Bc` to ContA's `clk0'Ca`, because the former IS the MIR clock
(ContB j05 takes `StopMIRClkBD`) and the hold stops it on purpose. And
`writeim-test` needed its `setcpreg(16'hFFFF)` changed to `16'h0000`: BMux
inverts, so `FFFF` reached the array as `0000` and a left-half write of zeros
counted as no write. It now reports 34 right-half cells for the right-half jam
and 16 left-half for the left, with `RBMux=ffff` at a real address.

### The gates were not gating: `$fatal` left every rule green

Every test rule ended in `| grep tb_x`, and a pipeline's exit status is the
LAST command's. A testbench that hit `$fatal` printed its assertion and the
rule still succeeded -- which is how the two tests above reported PASS in a
sweep and FAIL when run by hand, in the same minute.

**The obvious fix does nothing here.** `SHELL := /bin/bash` with
`.SHELLFLAGS := -o pipefail -c` is the standard answer, and macOS ships **GNU
Make 3.81** -- `.SHELLFLAGS` arrived in 3.82, so make ignores it silently. The
same failure mode twice in one afternoon. All twelve rules capture the status
explicitly instead:

```make
{ out=$$(./verilog/verilator/obj_x/tb_x 2>&1); rc=$$?; \
  echo "$$out" | grep tb_x; exit $$rc; }
```

which works in any make and any sh. Mutation-tested by turning three tests'
`$finish` into `$fatal` and watching the rules go red.

### Correction: "role `OUT` is the INVERTING output" does NOT generalise

The Reference section below states that rule. It holds for the OR/NOR family it
was derived from and **fails on two parts checked here**: on MC10104 the `OUT`
pins carry the AND and `o` the NAND, and on MC10121 pin 2 (`OUT`) is the plain
OR-AND while pin 3 (`o`) is the OR-AND-INVERT.

**PARC's net naming cannot settle it either, and this is worth knowing because
it looks as though it can.** Of the eighteen MC10121 packages that use both
outputs, nine name them as a complementary pair, and all nine put the primed
name on pin 2 -- unanimous, four different boards, and WRONG. Nine packages
likewise prime pin 9 of an MC10102, whose pin 15 is certainly the NOR. A net is
named for the sense its designer treats as primary, not for which pin inverts.

**What does settle it is the data book's LOGIC DIAGRAM, read by coordinates.**
The pin labels' overbars are in a font the PDF does not embed -- they render as
empty boxes and extract without the bar -- but the inverting BUBBLE rasterises
fine and `pdftotext -bbox` gives every pin label's y in points. Match the two:

```
pdftotext -f <page> -l <page> -bbox DL122_rev7.pdf -    # label y, in points
pdftoppm  -r 400 -f <page> -l <page> -x .. -y .. -png DL122_rev7.pdf out
```

For MC10121 (book page 284, PDF page 286) the labels sit at y=326.5 and
y=336.1 and the arrows at y=324.9 (plain) and y=335.2 (bubbled); for MC10104
(PDF page 251) gate d's bubble is on pin 9, whose label is above pin 15's.

## The passive packages, and what each one turned out to be (2026-08-17)

Three of the `.lc` part types are not logic and cannot be cells, because what
they do is decided by what a BOARD put across them, not by the part. All three
are resolved in the generator instead, and in every case the wire list's
per-pin `{x,y}` coordinates are what settle the geometry.

| type | what it is | how it is resolved |
|---|---|---|
| `SIPpackage` | resistor pack, common pin to the rest | hold every other pin at the common's level; a net held by a pull-up pack AND a pull-down pack is a bias divider and gets neither |
| `AUGATCG16` | wire-wrap header, a field of jumper positions | a column of exactly two pins where one net has no other source; three-pin columns and contested targets are a CHOICE and are refused |
| `PLAT1816` | resistor platform, eight series resistors | pin N to pin 17-N, where the far net has no other source and the near one is driven by a digital part |

**`PLAT1816` was 24 dead address lines.** The pairing is not inferred: pin N
and pin 17-N share an x coordinate, unanimously, 122 pairs across the machine
with no exception. On MemX three platforms carry the map DRAM's address and
strobes through series damping resistors -- `TMapAd.0a` in on pin 1 and
`RTMapAd.0a` out on pin 16, and so on for 24 signals -- and with the part
unmodelled every one of those lines sat at zero.

The display boards' platforms are why the conditions are there. Theirs sit in
the video DAC's supply filtering, across `GNDBlue`, `RegVCCB`, `FilterVEEB`, a
DAC output and two references; passing a level through those would be inventing
a signal. What separates them is the DICTIONARY, not our modelling progress: a
digital part carries a `[G ...]`, `[FF ...]` or `[M ...]` summary and an analog
one carries none, and DispM's `DACBlue` comes off an MC10318, which has none.
24 series passes taken, 98 left alone.

That needed a fix in `sil_ecldict.py`, which had walked straight past the `#`
that starts the dictionary's third section: every behaviour summary in the file
was being attributed to whichever pin block came last, so only 3 parts had one
instead of 168. It was also parsing those bracket lines as pin clauses, which
invented gates named after pin numbers.

**A jumper target offered more than one source is a choice.** Each column is
two pins, so the two-pin rule fires on each -- but MemX's b13 offers
`RamA1orVCCa` a ground while b14 offers it VCC-47 and the map address line
`RTMapAd.1a`. The names say what that is: "RamA1 OR VCC", the size of RAM
fitted. Taking several at once asserted a contradictory configuration, that net
strapped to GND and VCC together. Contested targets are refused now and
counted, along with the three-pin columns.

## Fixed: the machine did not settle, and it was two latches that are not

`make -C verilog machine-test` failed the moment the BaseBoard genuinely ran:
Verilator's settle loop no longer terminated, at 500 iterations as readily as
at 100. Verilator named 40 circular combinational paths across ProcH, ProcL,
MemC, MemD, MemX, DskEth, DispY and the IFU -- and named the same 40 before and
after the machine came alive, so it could not say which mattered. The first
reading of that was that these were the design's own gate-delay tricks, which
would have meant substituting delays. That reading was wrong.

`tools/sil_loops.py` builds the graph directly, from the cell files the RTL is
emitted from: nets are nodes, and an edge exists where a package has a
combinational path from one pin to another. Two modelling mistakes accounted
for all of it, and the archive settles both.

**Six cells were transparent latches.** `F10145A`, `F10415A`, `F10470` and
`i2125` are memories whose write is LEVEL-sensitive -- while the write and chip
enables are asserted, the stored bit follows the data input -- and `MC10173`
and `SN74LS259` are latches. Written as `always @*` they pass a level straight
through, so every read-modify-write path in the machine was a combinational
loop, which is most of a datapath. F10145A alone is 405 packages.

Rewriting them on `sys_clk` with the part's own level as an ENABLE is not a
new idea here: it is the convention this design already uses for every clocked
element, and the same change its two DRAM cells already carried. `sys_clk`
heavily oversamples every signal on the board, so the behaviour while
transparent is the same to a fabric clock's precision -- and unlike a latch it
synthesises. That took the graph from 1,333 back edges to 40.

**A second theory was tried and is wrong**, and is recorded because it is a
plausible misreading of the dictionary. `F10016`'s carry out is a gate,
`~(&q & ~CE')`, which puts a path from a package's own count enable to its own
carry -- and the dictionary lists pin 4 only in an `[FF ...]` output list,
beside the four Q pins, with no `[G]` entry for the part at all. That looks
like a statement that the carry is clocked, and registering it removed three
loops.

It is not one. **An `[FF]` entry is a TIMING ARC from the clock, and a gate
after the register is folded into it** rather than given its own [G]. `S169`
shows this outright: RC' gets a SECOND [FF] block with a clock-to-output delay
of 30.8 ns against 16.5 ns for the Q outputs, and the extra 14 ns is the carry
gate. A synchronous counter's carry has to be combinational anyway, or a
cascaded stage counts a clock late. The change was reverted, and the machine
settles without it -- the latch fixes were the whole of it.

**Two false loops were in the analysis, not the design,** and are worth knowing
because they would mislead the next person the same way. Every MC10181 slice
reported a two-net cycle between its own result bits -- the pin numbers on the
left of `assign p6 = ...` were being read as pins the `always @*` block
consumes; a part's own outputs are not its inputs. And the BaseBoard's
`VCOPhase0`/`VCOPhase1` pair is the analog VCO's relaxation loop, which the
MPQ3303 substitution already replaces, so nothing else on those nets is a path.

**Four structural loops remain, and all of them settle.** The processor's own
multiplexer chain feeding back:

```
DMuxData -> DMData    (h17, MC10158)
DMData   -> Pdata.00  (f03, MC10164)
Pdata.00 -> MuxData2  (d11, MU10164)
MuxData2 -> DMuxData  (l24, MU10164)
```

and the same shape on DispM, DispY and MemD, where a counter's carry returns to
its own count enable through an MC10195:

```
StopWakeCount -> KillDWTWakeup   (e24 F10016, CE' to CO')
KillDWTWakeup -> StopWakeCount   (d24 MC10195)
```

None is a loop in operation. `StopWakeCount` is a wired-OR that d24 also drives
from two other gates, so either of those holds it; the multiplexer chain's
selects never route all four legs at once; and the carry gate only closes at
terminal count. They are left alone rather than cut, because a delay inserted
there would be an invention, and the machine converges with all four present,
over 200,000 cycles as readily as 20,000.

**`make -C verilog loop-check` is the gate**, and it costs a fifth of a second.
It reports every board's back edges against a list of the loops that are
understood, so a cell modelled as a latch is caught where it is written rather
than as a non-convergence on some other board tens of thousands of cycles
later. Reverting `cell_MC10173` to `always @*` makes it fail with ProcH and
ProcL named.

`cell-check` briefly gained a property from the second theory -- a pin listed
only under `[FF]`/`[L]` may not be computed from an input pin -- and it was
removed with the theory. `sil_check_cells.py` records why, so it does not get
re-derived.

## Reference: why `machine-test` is not a toggle count

The harness reports how many probed signals move, and that reads like a
progress bar for the cell library. **It is not, and treating it as one would
punish correct work.** Modelling four more gate types and fixing MC10105's
polarity against the datasheet took the count from 31 DOWN to 27: an undriven
net reads 0 and flaps with whatever else moves, while correct logic holds it
steady. Isolating it showed the polarity fix alone accounted for 2 of the
drop.

So the gate asserts the CLOCK instead -- all 24 nets the BaseBoard fans out
to the slots must toggle, by name, against the probe map the generator emits.
That cannot improve by luck, and if it fails nothing downstream can compute.
`tools/rtl_machine_check.py`, mutation-tested by killing the oscillator.

The toggle total is still printed, as information rather than a threshold.

## Task A -- fill in the cell library

The machine is assembled, self-clocking and gated; what stops it computing is
that 63 of 125 cell types are still skeletons with correct ports and no body.
`make -C verilog machine-test` asserts the clock still reaches every slot;
see the note above on why it does NOT gate on how many signals move.

The clock generator is done (MC1660, MC1690, and the VCO substitution) and so
is `MB7071H`, the 256x4 RAM that is the machine's REGISTER FILE -- ProcH h06
is `RbAdr`/`SelectRm'` (that is RM), i06 is `StkAdr`/`SelectStk'` (STK), four
packages per board for the 16-bit width, and the other 62 are MemC's cache
tags and DispM's colour tables. It writes synchronously and reads
combinationally, which is a distributed/LUT RAM and is what the part does.

Seven more landed: `SE10211` (11), `MC10161` (21, Binary to 1-8 Decoder Low),
`MC1668` (12, dual clocked flip-flop), then with the polarity settled
`MC10109` (21), `MC10212`+`SE10212` (40) and `MC1664` (19). Coverage
86.5% -> 89.8%. `MC1664`'s FUNCTION came from the rule rather than a
datasheet: all four of its gates have a single `o` output, and `o` is the
non-inverting sense, so every output is an OR -- which matches the MECL III
pairing where MC1662 is the NOR part.

`MC10117` (28) and `MC10180` (13) then landed from their data sheets, which
are filed in `DoradoDocs/datasheets/` -- see the README there. MC10117 also
settled EclDict's `u`/`v` role letters, which the OR/NOR rule does not reach:
`u` is the inverting output, `v` the non-inverting, exactly as `OUT`/`o` are.
MC10180's sheet gives the mode table, and it is worth knowing what the part
does -- a select input INVERTS its operand, so one adder does add, subtract,
reverse-subtract and negate, with the carry-in supplying the +1.

**The ALU is modelled and verified** (`MC10181`, 8 packages: two slices on
ProcH, two on ProcL for the 16-bit datapath, four more on the IFU). Written
as a 16-way mux straight from the datasheet's function table -- one row per
line, so it reads against the sheet -- which is also one LUT level on an
FPGA. `make -C verilog alu-test` checks it EXHAUSTIVELY against independent
expressions (`A + B`, `A - B`, `A & B` written directly, not the table it was
built from): 4,880 checks, and mutation-tested with six injected errors
(operand order, select order, carry sense, a swapped table row, mode ignored,
propagate using AND for OR).

Two things worth keeping from writing it:

- **The pin indices look reversed and are not.** EclDict names `D0` for pin
  10, which the datasheet calls `A3`. Xerox numbers bit 0 as the MOST
  significant throughout this machine, the same convention the PROMs use.
- **A confirmation of the architecture docs.** `CLAUDE.md` says ALUF is a
  4-bit pointer into ALUFM, "16 x 6 bits". Six bits per entry is exactly what
  this part takes: S0-S3, M and Cn. The netlist and the microcode
  documentation agree without either having been derived from the other.

One mutation was NOT caught, and that was right: gating the adder's carry-in
by M is dead code, because M already selects the logic result and forces the
carry out. The test found redundancy rather than a bug, and the cell is
simpler for it.

**Six more cells landed after the audit**: `MC10162` (10, the decoder's
active-high sibling), `SN74LS174`/`SN74S174` (16+28, hex D flip-flop -- PARC's
TtlDict gives the whole part in one line, `[FF 3>2, 4>5, ... : CLK 9 RS 1]`),
`MC10136` (9, the hexadecimal counter, whose four modes decode straight out of
EclDict's `SU'`/`SD'` naming), `SN74LS138` (7, the 3-to-8 decoder) and
`i2716` (8, the EPROM).

**The BaseBoard's EPROMs are LOADED, and the socket map is established.**

`make -C verilog proms` now fills them. The contents are NOT `firmware/` --
those five 1987 chip dumps look like the obvious source and are not usable, as
no image has a plausible 6502 vector triple in its tail (B-10 gives
NMI=0x0F00, IRQ=0x4F45) and nothing in this repository reads them. The C
emulator boots `chm/dorado/doradobaserom.mb!13`, so a small program
(`dorado/tests/baseboard_rom_dump.c`) dumps it through the emulator's OWN
loader and `tools/firmware_eproms.py` slices it into sockets. Whatever the
RTL's 6502 fetches is then byte-for-byte what the C one fetches.

The map was derived, not guessed, and it validates itself:

- The eight sockets are chip-selected by `Rom0'`..`Rom7'` from the SN74LS138
  at g11, whose select is `RSA.0/1/2`.
- Each socket's address pins take `MCA.10` down to `MCA.00` -- a clean 2K
  window. (PARC names those MSB-first: the dictionary's `A0` is pin 19, the
  sheet's A10. `cell_i2716` had that backwards until this was traced; a
  reversed address would have scrambled the image silently.)
- `RSA.0/1/2` are driven by no gate. They arrive on `c07`, an Augat STRAP
  BLOCK which also brings `MCA.11/12/13/14` out. With 2K parts the only
  strapping that tiles the space is RSA = MCA.11/12/13.
- So Rom<n> sits at 0xC000 + n*0x800, and `baseboard.h` puts the ROM area at
  exactly those 16K.

Then the ROM agrees: its vectors are NMI=0xF000, RESET=0xF3A7, IRQ=0xF2A2,
all inside the ROM area, and its 6,955 non-zero bytes fall in exactly four 2K
blocks -- C000, C800, F000, F800, which is the 1981 Hardware Manual's "4
EPROMs at F000, F800, C000, C800". Four chips in eight sockets.

**The 6502 does not run yet, and the reason is specific.**
`verilog/verilator/tb_baseboard.sv` instantiates the board alone and watches
the address bus: it never changes. The core itself is real (Holme's
netlist-derived 6502) and so are the RIOTs -- what is missing is the CLOCK AND
RESET CHAIN around them, every part of it still a skeleton:

| part | pkgs | role |
|---|---|---|
| `SN74LS74` | 3 | g08 makes `MCPreClk`, the 6502's clock; j08 drives `MCReset'` |
| `SN74LS04` | 5 | hex inverter, incl. `MCClk` -> `MCClk'` for the ROM decoder |
| `SN74LS163` | ? | e07 drives `Max`, which clocks g08 -- the chain runs back to here |
| `MC14521B` | 1 | 24-stage divider off `MCPreClk` |
| `SN74LS175` | 2 | latches on the reset path |
| `SN74LS259` | 1 | addressable latch on the reset path |

**That is the next task, and it is worth doing**: they are small standard TTL
parts, and with them the BaseBoard would execute its own boot firmware in RTL
-- the same code the C emulator runs, which makes it diffable the way the ALU
now is. `tb_baseboard` is already written to report it: it looks for the 6502
addressing 0xFFFC/D, which would mean the core, the decoder, the socket
mapping and the image are all right at once.

**What is left in the machine** is a short list, and none of it is a gate:

| part | pkgs | what it is |
|---|---|---|
| `CA3140` | 9 | **op-amp** -- analog, like the VCO; expect a substitution, not a model |
| `F100181` | 8 | the Fairchild 100K ALU on MemC -- active-low carries and an output enable, so it needs its own sheet |
| `MC10172` | 7 | |
| `AM2615` | 7 | |
| `SN74LS251` | 6 | |

## The cross-check works: the netlist and the C emulator agree

**This is what generating RTL from PARC's netlists was for**, and it now
returns a number. `make -C verilog alu-diff`:

```
tb_alu_vs_c: 10752 vectors from the C emulator, 0 mismatches
```

Four `cell_MC10181` slices chained into the 16-bit ALU -- the way ProcH and
ProcL build it -- fed vectors that `dorado/tests/alu_vectors.c` dumps from
`cpu.c`'s own `alu_op()`. All 21 operations the emulator implements, every
pair of 16 edge-case operands, both carry-in values. **Neither side was
derived from the other**: the cell is a transcription of Motorola's data
sheet, `alu_op()` was written from the Hardware Manual's Table 9, forty-five
years apart.

The encoding agrees too, which is a result in itself. The 6-bit ALUFM entry
decodes as **`{Cn, S3, S2, S1, S0, M}`** -- carry-in, the part's four select
lines, then the mode bit -- and under that mapping all 21 of `alu_op()`'s
cases match the data sheet's function table exactly, sixteen logical and five
arithmetic. `CLAUDE.md` describes ALUFM as "16 x 6 bits" without saying what
the six bits ARE; they are this part's control inputs.

Two things about how it is built:

- `alu_vectors.c` **includes `src/cpu.c`** rather than linking it, because
  `alu_op` is static. That is deliberate -- the test has to run the
  emulator's real code, not a copy that could drift. The Makefile rule links
  `LIB_OBJS` minus `cpu.o` to avoid duplicate symbols.
- It is the only test that exercises the **carry ripple between slices**.
  Four correct 4-bit adders still give a wrong 16-bit answer if the chain is
  wired backwards, and mutation-testing confirms it: reversing the chain
  gives 1,409 mismatches, reversing a slice's bit order 5,776, and corrupting
  one row of the function table 512.

**The obvious next targets** are the other places the two models overlap.
`memory.c` against MemC/MemD/MemX (the cache's four operations are already
generated as the `EC-1`/`EC-2` PROMs), and the shifter, whose masks the
LMASK/RMASK PROMs already cross-check.

The biggest remaining parts overall -- `DS3649` (32), `SN74S174` (28),
`SN74H04` (28) -- have ZERO packages in the eleven-board machine. They are on
IOTest, msa/PCMSA and Music. Skip them unless those configurations matter.

For the rest, order by package count -- `python3 tools/sil_netlist.py --all
chm/sil` ranks them -- and prefer the parts the processor boards use, since
that is where the C emulator can check the answer.

The incremental path is supported directly, if a smaller machine is easier to
reason about:

```
make -C verilog backplane MACHINE="--boards=ProcH,ProcL --module=dorado_proc"
```

That gives 76 internal nets and 164 ports, every one labelled with the board
it is waiting for.

The PROMs are done: `make -C verilog proms` writes both the per-PROM images
and the per-package slices, and the generator wires all 29 placed packages.
Six PROM images have nowhere to go yet, and each for a stated reason
(`--placement` prints them): the three display-timing PROMs sit on a small
board the Sil tree does not include, and `Mouse-Motion`/`Keyboard-Map` are
blown at IFU-i03/k05/l05 where **IFU Rev Ch has ordinary logic** -- by the
1981 manual the terminal microcomputer decodes the keyboard and mouse and
serialises them to 177034B (Table 24), so those belong to an earlier IFU.

## Task B -- test against the C emulator

This is the reason the whole approach is worth it: **we have a working
software Dorado to diff against**, and the two were derived from different
sources 45 years apart.

Cheapest first:

1. **PROM contents vs emulator behaviour.** Already demonstrated: LMASK/RMASK
   generated from PARC's BCPL match the shifter masks the C emulator computes
   from the Hardware Manual. The same trick is available for the memory
   system (`EC-1`/`EC-2`'s four cache operations against `memory.c`), the
   disk (a sector is four blocks), the keyboard (`Keyboard-Map` against
   `display.c`'s 61-key matrix, which has been checked three other ways), and
   now the ethernet: `EtherRcvr`/`EtherXmtr` are the receiver and transmitter
   state machines whose behaviour `dorado/src/ethernet.c` models at packet
   level. **Note the keyboard one is a layer question**: the PROM is what the
   IFU applies, `display.c` is what the terminal delivers, so a mismatch would
   be interesting rather than automatically a bug.
2. **Clock a board and compare a signal.** Once ProcH is instantiated, drive
   a known microinstruction and compare an ALU output against the C
   emulator's at the same point.
3. **Boot something.** A long way off, and the honest ordering is 1, 2, then
   worry about 3.

---

## Things that will bite, learned the hard way

- **`make -C verilog lint` after every change.** It is seconds, and it caught
  every structural error here -- including two of mine in one batch.
- **Verilator compiles inside `obj_dir`**, so relative `-CFLAGS` include
  paths need `../`.
- **A comment line beginning with the tool's own name is parsed as a pragma**
  (`BADVLTPRAGMA`). Do not start a comment with it.
- **Hand-written cells must declare every pin the BOARDS wire**, not just the
  ones the dictionary names -- p1/p8/p16 are power on a 16-pin MECL package,
  and omitting them is `PINNOTFOUND`.
- **Net names are not unique after naive sanitising.** `CTask.0` and
  `CTask=0` are different nets on ContA and both mapped to `CTask_0`; the
  mapping is injective now and verified collision-free, but the same trap
  waits in any new name mangling.
- **Cell port directions are global; net classification is per-board.** A pin
  that drives on MemC and is only sensed on IFU is `output` in the shared
  cell.
- **Check a transcription structurally, not by eye.** Every PROM generator
  has a property test for this reason, and they have now paid for themselves
  three times.
- **Then check the CHECK.** The Ethernet property checks passed on their
  first run and still missed three of fourteen injected errors. A gate that
  has never failed has not been shown to work; break the thing it guards and
  watch it go red.
- **Xerox files are CR-terminated.** `wc -l` reports 0 lines for every `.bcpl`
  and `.bp` in the archive, and a naive read gets one enormous line.
- **A sampled probe sees one clock phase.** The harness reads probes after
  `tick()`, which leaves the clock HIGH, so anything that follows the clock
  combinationally reads as a constant and looks dead. That is why the machine
  clock is divided from `sys_clk` rather than being `sys_clk`: the cells need
  to see an edge, and the observer needs to sample between them.
- **zsh does not word-split unquoted parameters.** `$FLAGS` reaches the
  program as ONE argument, and Verilator reports the whole string as an
  unknown warning. Use an array, or write the flags out.
- **Two passes over the same netlist must agree about what a driver is.**
  `classify()` decided port direction from the CELL's port directions while
  `emit()` decided on-board wired-OR from the WIRE LIST's `o` pins. On five
  nets across three boards the two disagreed, and the generator declared a
  module input and then assigned to it: `%Error-ASSIGNIN`. If two functions
  answer the same question, have one of them ask the other.
- **A checker can be the thing that is broken.** The port gate reported 514
  missing ports right after the fix landed; its regex matched `input|output`
  and the new direction is `inout`. Check a red gate against a hand count
  before believing it.
- **The comma goes before the comment.** Emitting
  `input wire X  // why,` puts the separator INSIDE the comment, where it
  vanishes. Generated Verilog with per-port comments needs the punctuation
  placed deliberately.
- **Verilator resolves a module by FILENAME.** `-I<dir>` finds `cell_X.v` for
  module `cell_X`, but board module `ProcH_m_Rev_m_Ce` lives in
  `ProcH-Rev-Ce.v`, so the top module cannot be linted with `-I` alone -- the
  board files must be passed on the command line. That is why the lint rule
  treats the top module separately.
- **zsh does not word-split unquoted parameters.** `$FLAGS` reaches the
  program as ONE argument, and Verilator reports the whole string as an
  unknown warning. Use an array, or write the flags out.

---

## The one open licence question

`verilog/vendor/riot/M6532.sv` is **CC BY-NC 4.0** -- more restrictive than
anything else here. DECISION: keep it, the project is not shipping
commercially, and the note exists so the constraint is known rather than
discovered. If it ever binds, the preferred fix is porting our own:
`dorado/src/baseboard.c` already models the 6532 well enough to run the real
BaseBoard EPROMs, so it is a validated spec written in this repository with
no third-party licence at all.

## Configuration straps: where each board's identity lives

A board's slow-I/O address and task number are not in its wire list. They are
set by cutting legs off a resistor SIP, and the netlist still lists the pin --
it just has no resistor behind it. The fact lives on the board's configuration
sheet, and `SIP_BROKEN_LEGS` in `tools/sil_to_verilog.py` carries the tables.

Gated by `make -C verilog strap-test` and `make -C verilog muffler-test`:

| board | pack | cut | field | reads | source |
|---|---|---|---|---|---|
| DispM | g41 | 6 | `DDMTIOA` | 36B -> 0360-0367 | "making DDMTIOA = 360B" |
| DispM | b52 | 3,4 | `AltoWTask` | 1001 = 9 | "for Task 9D = 11B" |
| DskEth | e41 | 4,5,6,7 | `TIOA-Ad` | 1 -> 010-017 | "* Standard addresses are 10-17" |
| DispY | g41 | 4,5 | `WakeupWait` | 1100 | DispY31.sil Rev Cl |
| DispY | g42 | none | `DDCTIOA` | 37B -> 0370-0377 | DispY31.sil Rev Cl |
| DispY | k51 | 4,5 | `DDCDMD` | 1100 | DispY31.sil Rev Cl |
| DispY | k52 | 3 | `DWTTask` | 1011 = 11 | DispY31.sil Rev Cl |
| BaseBd | l49 | 2,4,5,7 | `Midas` | 1001 = 9 | "all pins cut except 1, 3, 6, & 8" |

Three of them state the ANSWER on the sheet, so they check themselves. The
encoding comes from the DskEth table, which lists all 32 ranges: **a cut leg
is 0, an intact one is 1, MSB first**, and the five-bit field is the top of an
eight-bit address whose low three bits select the register within the device.

**The C emulator agrees and was not consulted.** `include/dispm.h` defines
`DORADO_DISPM_TIOA_BOARD 0360`; `include/disk.h` defines
`DORADO_DISK_TIOA_DISKCONTROL 010` with the comment "Disk uses task 14 octal
exclusively, on TIOA 10-14 octal". Two models, no shared code, same addresses.

### Three traps

1. **Take the sheet the wire list names.** Every `.wl` header lists its
   constituent `.sil` files with Rev and Date. The scanned
   `DoradoDocs/schematics/DispY.pdf` is Rev Ci 11/02/79 while the wire list is
   Rev Cl 3/25/82, and g41's cut list CHANGED between them (3,4,5 -> 4,5). The
   per-revision scans in `DoradoDocs/doradodrawings/` have the built one.
2. **A resistor leg has no direction.** Sil marks pin 4 of an 8-pin SIP `in`
   as a matter of drawing convention. Requiring `out` left 18 nets with no
   driver at all, including the IFU's `TTLHigh` and six active-low DskEth
   drive-status lines that then read ASSERTED -- a disk that is not there.
3. **A pull-up against a pull-down is not always a divider.** For DskEth's
   `RcvData` it is (an analog bias at the receiver input). For the BaseBoard's
   `Midas` straps it is not: the pull-down pack sets the default 0 and the
   pull-up pack has legs only where a 1 is wanted.

### Pins broken off logic chips

Not only resistor packs. MemX's "Stuffing and Configuration Instructions"
(`Memx23.sil`, 10/29/79) gives two, and only one is ours to apply:

* **Applied.** "2. Break h20.10 before stuffing." h20 is an MC10105 and pin 10
  is `MapPerr`, so `MemPE` is `STPerr | HitPerr`. MapPerr keeps its other two
  consumers (g49.2, k08.5); only this term goes. An open MECL 10K input has an
  internal pulldown to VEE and reads 0. `BROKEN_PACKAGE_PINS` in the generator.
* **NOT applied, on purpose.** "1. If 256 chips are NOT installed in the MSA,
  then break g10.6." That is conditional on how much memory the machine was
  built with, and `ChipsAre256/16K` / `ChipsAre64K` are backplane INPUTS to
  MemX rather than straps on it -- the configuration comes from outside the
  board. The sheet's blue wire (k4.6 / k4.5 / k4.4 to k4.16), the b14 and c12
  jumper sockets, and the PLAT capacitor values are all the same 16K / 64K /
  256K choice. Decide the machine's memory configuration first, then apply one
  column of that table as a set.

### The crystal oscillators run at their own frequencies

Four K1115A positions across three boards, and the configuration sheets give
each a different value. Until `CELL_PARAMS` existed the generator instantiated
every cell without parameters, so all four ran at one rate.

| position | value | source |
|---|---|---|
| DispY a05 | 50 MHz | DispY31.sil Rev Cl note 5 (the Rev Ci scan says 20) |
| DispM c05 | 10 MHz, the VCO | DispM30.sil Rev Ch |
| DispM d13 | 20 MHz | same sheet, "20 MHz for Alto / 50 MHz for LF" |
| DskEth j20 | not stated | Ether12.sil draws it on EClk0 with no value |

d13 is taken as the Alto-style 20: DispM's two monitor modes are 640x480 and
1024x768, a 2.5x pixel-rate ratio which is exactly 50/20, and the C emulator's
head reports the 640x480 "standard" one.

`cell_K1115A` is a PHASE ACCUMULATOR, not an integer divider. sys_clk is
266.667 MHz (16 to a 60 ns microinstruction), so the divisors for these would
be 13.33, 6.67 and 2.67; rounded, they give 1 : 1.86 : 4.33 where the parts
are 1 : 2 : 5. The accumulator carries the remainder, and `osc-test` measures
9.99 MHz at c05 with the ratios exact.

### Not yet read

Only two sheets are headed "Configuration Information" -- the BaseBoard's is
"Stuffing Information" and DskEth's IOA table is on a plain reference sheet --
so search by content. Outstanding: MemC's "cut the 4 107 legs marked X" (an
OPTIONAL conversion of parity into VA.4; the built board is the un-converted
one, so do NOT apply it), MemX's configuration PLATs, ProcL's parallel
discrete resistors, and per-position oscillator frequencies (DispY a05 50 MHz,
DispM c05 10 MHz, DispM d13 20/50 MHz -- all four currently share one divisor).

## The test jumper that is correctly absent

ProcH's Task Simulator sheet says "To enable this test circuit be sure there
is a jumper to connect TestTW (109) to ContA pin (140)". `TestTW` is a ProcH
backplane OUTPUT and ContA has nothing of that name, so the jumper is NOT in
the generated machine -- which is right, it is fitted by hand when you want
the test circuit. Worth knowing before someone reads a board output that goes
nowhere as a missing connection.

## The memory reference-kind chain, derived end to end (2026-08-22)

Three gates on MemC decide what kind of memory reference a microinstruction
makes, and all three are now read off PARC's own wire list rather than
guessed:

| gate | part | function |
|---|---|---|
| `b24` c | MC10103 | `WantProcRef' = IgnoreProc \| ASEL.0` -- is a reference wanted at all |
| `d22` a | MC10117 | `WantCR = ~[(FF.0mem' \| WantProcRef') & (ASEL.1' \| WantProcRef')]` -- cache reference, or alternate |
| `b24` | MC10103 | `WantAltRef' = WantProcRef' \| WantCR` -- the enable on the alternate decoder |
| `j24` | MC10161 | `{EmuOrFT', ASEL.2, FF.1mem}` -> `Flush_'` (Q3) / `Map_'` (Q1) |
| `a24` | MC10162 | `{ASEL.1, ASEL.2, FF.1mem}` -> `IFetch_` / `Lfetch_` |

So an ALTERNATE reference -- Map or Flush -- requires `ASEL.1 = 0` and
`FF.0 = 0` (which makes `WantCR` low) on top of a reference being wanted.

**Both MC10103 and MC10117 name their outputs backwards from the roles here.**
On `b24` pin 9 carries the name `WantProcRef` and is the NOR; pin 15 carries
`WantProcRef'` and is the OR. On `d22` pin 3 is named `WantCR` and is the
inverting output while pin 2, named `WantCR'`, is the non-inverting one. Net
naming settles nothing about polarity -- read the roles and the datasheet.

**And the C emulator agrees, independently.** `EmuOrFT'` is the
emulator-or-fault-task condition, and `cpu.c` comments `DM_REF_MAP` /
`DM_REF_FLUSH` as "emulator/fault" against the IO kinds as "io task". Neither
model was derived from the other, and the reference kind is task-dependent in
both.

`make -C verilog memrun-test` measures the chain, and over 3000 samples the
alternate path decodes correctly 149 times for 149 -- exactly as the `d22`
algebra predicts, on every sample where the flush-shaped FF field is in force.

**The front door is proven open.** With the ASEL=1 reference actually
executing -- 2851 of 3000 samples -- `WantProcRef'` is low on **every one of
them**, from `IgnoreProc = 0` and `ASEL.0 = 0`, which is `b24` gate c's
algebra exactly. Gated in `memrun-test` and mutation-tested.

Getting there meant identifying the running instruction by the right field.
An earlier pass conditioned on `FF.0mem'`, which selected the 149 **startup**
cycles instead of the 2851 running ones and made the whole chain look dead.
**IM is four interleaved banks** -- the low two address bits pick the bank --
so reading IM[0..3] back settles what is actually there: IM[0] and IM[1] both
hold the bench's reference (`L=0101 R=0081`), the machine loops over those
two, `TNIA` 000 is the startup state and 001 the running one, and the
instruction is identified by its ASEL.

Two things remain, both small and both stated by measurement:

1. **RETRACTED -- all four of `build_hunk`'s copies do land.** An earlier
   note said IM[2] and IM[3] read back as zero. That came from reading
   `rd_L2(0)`/`rd_L3(0)` as "IM[2]" and "IM[3]", which they are not:
   `find_word` *searches* all four banks precisely because the bank-to-address
   mapping is not obvious. Scanning every bank at several idx gives the real
   mapping:

   | address | bank | idx |
   |---|---|---|
   | 0 | 0 | 0 |
   | 1 | 1 | 0 |
   | 2 | 0 | 1 |
   | 3 | 1 | 1 |

   i.e. `bank = addr & 1`, `idx = addr >> 1`. Banks 2 and 3 are a different
   part of the store and are correctly empty here. All four microinstructions
   are present, and `build_hunk4` now packs four *distinct* ones.

2. **RESOLVED -- and the microcode now makes a Flush.** `FF.0mem'` and
   `FF.1mem` are inputs only on MemC; ProcH and ProcL drive them from
   `d24`/`d23`, a pair of MC10101s whose pin 12 is the **common** input:

   ```
   FF.0mem' = ~(FFok'a | FF.0)      FF.0mem = FFok'a | FF.0
   FF.1mem  =   FFok'a | FF.1
   ```

   With `FFok'a` high both bits are **forced high**, so the memory section
   sees `ff01 = 3` whatever the microinstruction holds. That is why packing
   `FF = 0o100` changed the IM word (right half `4081` instead of `0081`, so
   the write path was never at fault) and changed nothing at MemC.

   **`FFok'` comes from ContA `f24`**, an MC10211 (NOR) whose two gates are
   **wired-OR** onto each of `FFok'a/b/c`:

   ```
   FFok' = ~BSEL.0'  |  ~(JCN.0 | JCN.1 | JCN.2or3)
   ```

   so it goes low only when **BSEL.0 = 0 and one of those JCN bits is set**.
   The bench used `BSEL = 4` -- BSEL.0 = 1 -- which held `FFok'` high and
   masked the FF field off. **The FF field is only a function field when BSEL
   and JCN are not claiming those bits**, exactly the kind of interdependence
   HM Table 11 encodes.

   With `BSEL = 0` and `JCN = 0o201`, measured over the 2851 cycles the
   reference runs: `FFok'a` high on **0**, `WantCR` low on **2851**,
   `WantAltRef'` low on **2851**, and **`Flush'` asserted on 2851**. All four
   gated; reverting BSEL to 4 is caught.

   **The tension this exposes:** BSEL >= 4 is what puts the A leg on MAR (per
   `compute-test`), and BSEL >= 4 is exactly what masks the FF function off.
   A reference carrying an FF function cannot take its address that way --
   where the address comes from for BSEL < 4 is the next question.

3. **RESOLVED -- the cache misses, and a Store is what makes it.** The rest
   of the chain below `Flush'`:

   | gate | part | function |
   |---|---|---|
   | `l19` d | MC10100 (pin 9 common) | `FlushStore = ~(FSinPair' \| EcHasAb)` |
   | `k21` | MC10176 hex D FF, clocked by `LdPair'` | `FSinPair'` is Q5, fed by D5 on pin 12 |
   | `j23` | MC10117 second gate (pins 10-13, 9 common) | that D input is `FlushInA & HitColDirty` |

   `HitColDirty` is the point: **flushing a clean line needs no write-back**,
   so only a dirty line has anything to store. A single flush against a clean
   cache could never reach the storage path.

   So the bench runs **two instructions**, alternating -- `build_hunk2` packs
   them (copies 0,2 are A and 1,3 are B, so IM[0] = A and IM[1] = B; byte 0
   carries RSTK.0 and BLOCK for all four, so those must match). IM[0] stores
   to dirty a line, IM[1] flushes it, the Local Jumps bouncing 0 -> 1 -> 0.

   **The Store encoding comes out of the gates, not a table.** `j22`
   (MC10105) gate a gives `Store← = ~(CacheRef' | Store←IfCR')`, and `b24`
   gate b gives `Store←IfCR' = Q0 | ASEL.2`, where Q0 is the `a24` decoder's
   zero output addressed `{ASEL.2, FF.0mem', FF.1mem}`. So ASEL.2 = 0, and
   `CacheRef'` low wants `WantCR = 1` which wants FF.0 = 1, and Q0 = 0 with
   FF.0 = 1 then wants FF.1 = 1 -- giving **ASEL = 000 with FF = 0o300**.

   And the whole path runs. Over one 3000-sample run:

   | signal | cycles |
   |---|---|
   | `Store←` asserted | 1440 |
   | `HitColDirty` | 160 |
   | `FSinPair'` fell | 320 |
   | `FlushStore` | 320 |
   | `ForceMiss` | 1088 |
   | MISS(a) / MISS(b) | 1043 / 1043 |

   All gated in `memrun-test`; collapsing the Store back into a second Flush
   is caught.

4. **And the miss starts a storage cycle.** MemX `i14` is an SG10139 PROM
   making `preStartMem'` from `{MapFnc.0', MapFnc.1', MapState.0..2}` -- the
   storage cycle is a **PROM state machine**. Measured over the run: MapState
   takes **7 of its 8 values**, MapFnc 2 of 4, `preStartMem'` is low on 2712
   samples and `StartMem'` on 2680. All gated.

5. **The write is scheduled, and an empty Map entry is what stops it.**
   `MemWEa` is 0 for the whole run, and it is **not a gate**. MemX `c02` is an
   MC10176 clocked by `Clk1'Aa` whose Q0 is `MemWEa`, fed from MemX `i10`, an
   F10016:

   ```
   C = Clk0'Ba    CE' = TrueBD    PE' = MemIdle    MR = STPerr
   H0 -> MemWEa   H2 -> MakeMemCAS
   ```

   **`TrueBD` is a hardwired TRUE** -- `g04` is an MC10195 with pin 12 open,
   the same trick as ProcH's `TrueA` -- so CE' is permanently high and **this
   F10016 never counts**: a parallel-load *register*. Measured: in load on
   2936 samples, allowed to count on 0. (Do not reason about its terminal
   count; that is the bug that once oscillated the whole machine, and it is
   not in play here.)

   So `MemWEa` is D0 registered, and D0 comes from `i11`, an MC10105 gate b --
   a three-input NOR:

   ```
   D0 = ~(WriteInMem' | MemX07.sil+10 | MapTroubleInMem)
   ```

   with each term low on: `WriteInMem'` **1280** (a write *is* in the
   pipeline), `MemX07.sil+10` 96, `MapTroubleInMem` **0** -- asserted the
   entire run.

   **And `MapTrouble` is asserted because the Map entry fails its parity
   check.** `j11` (an MC10176) registers it at `StartMemClk0'` from
   `MapTrouble`, which is `g14`, an MC10121 4-wide OR-AND whose **common**
   input is `ReadOrWriteInMap'`:

   ```
   y = (CheckWP' | MapWP'    | ROWIM')
     & (MapWP'   | MapDirty' | ROWIM')
     & (TrueBD   | ROWIM')                  <- pinned to 1
     & (MapEven' | preRfshInMem | ROWIM')
   ```

   **and `MapTrouble` is the INVERSE of that.** MC10121 puts the plain OR-AND
   on pin 2 and the OR-AND-INVERT on pin 3 -- the *opposite* of the "role
   `OUT` is the inverting output" rule, settled from the MECL data book's own
   logic diagram (PARC's naming here agrees with the roles and is wrong).
   `MapTrouble` is pin 3, so **it clears only when all four groups are 1**.

   Measured over the run, low on: `ReadOrWriteInMap'` 3000, `MapWP'` 0,
   `MapDirty'` 0, `MapEven'` 3000, `CheckWP'` 2691, `preRfshInMem` 3000 --
   with `MapTrouble` high on 3000 and `MapTrouble'` on 0.

   **So the blocker is the fourth group**, and an earlier version of this note
   blamed the wrong one: `MapWP'` and `MapDirty'` being *high* makes groups 1
   and 2 equal 1, which is exactly what is wanted. Group 4 is
   `(MapEven' | preRfshInMem | ROWIM')` and all three are low.

   **`MapEven'` is the map parity check.** MemX `e11` is an MC10170, the
   9-bit parity part, taking `RP.00`..`RP.08` -- the map entry's real-page
   bits -- with `MemX13.sil+1` on the carry-in, and pin 15 is its EVEN output.
   An uninitialised, all-zero map entry fails that check, and a failed map
   parity is exactly what `MapTrouble` should mean. (The entry lives in
   `MosRam` packages d11/d13, reached through MC10124 TTL-to-ECL translators
   at e17/e10, so `MapWP` and `MapDirty` are literally the stored bits.)

6. **FIXED -- the map read path was strobed off by one net, and VBB was
   why.** Writing a map entry would not have helped; the entry never reached
   the logic.

   The map array *is* strobed (RAS' 31 edges, CAS' 21, read condition on
   579/3000) and its output *is* live -- but preloading all 21 bit planes
   changed nothing downstream, so the break was after the array.

   It was the MC10124 TTL-to-ECL translators. The cell is right
   (`p12 = ~(p10 & p6)`) and **pin 6 is the common strobe**: *"when the common
   strobe input is at the low logic level, it forces all true outputs to a
   MECL low logic state and all inverting outputs to a MECL high logic
   state."* Exactly what we saw.

   That strobe is `THi`, feeding pin 6 of five MC10124s -- the whole map read
   path -- and it comes from e15 (MC10125) as `THi = p15 & ~p14`, pin 15 being
   `VBBe15` and pin 14 **open**.

   **`VBB` is the ECL switching threshold, not a logic signal**, and all 39
   VBB nets in the machine have zero drivers. A differential pair with VBB on
   one side follows the *other* side, so **VBB must lose to a real signal and
   beat an open pin** -- and the EclDict roles already distinguish them:
   `common` (EclDict's `c`) is the complement input, where VBB reads 0 so the
   true input decides; `in` is the true input, where it reads 1 against an
   open complement. A single constant per *net* cannot do this, because one
   VBB net reaches both sides: on e15, channel a has VBB on `common` with a
   real signal opposite, while channel d has VBB on `in` with an open
   complement and **manufactures the constant TTL high**.

   The rule is in `tools/sil_to_verilog.py` now, beside `OVERRIDE_DRIVERS` and
   `WEAK_PORT_DRIVERS`, and gated: `THi` must be high on every cycle and the
   map outputs must vary. Reverting the rule is caught. With it, `MapWP'` and
   `MapDirty'` go low on 2434 samples where they were stuck. **All 29 gates
   stay green** across the machine-wide change.

7. **The microcode writes its own Map entry, and the fault clears.** With
   the read path open, the entry's *content* decides `MapTrouble`: e17 takes
   `THi` as data and plane d13 as its common strobe, so `MapWP' = ~d13`, and
   likewise `MapDirty' = ~d11`; group 4 additionally wants `MapEven' = 1`.

   **Do not plant the entry from the bench.** The preload *lands*
   (`d13.mem[0]` reads `000` right after the loop) and is *gone* by the
   measurement window, with no write strobe inside it -- **the machine writes
   the map itself during startup**, which is the write path working.

   So the entry is written the way the hardware does it. The loop is **four
   instructions** now (`build_hunk4`), running 0 → 1 → 2 → 3 → 0:

   | addr | reference | encoding |
   |---|---|---|
   | IM[0] | `←Map` | ASEL = 000, FF.0 = 0, FF.1 = 1 (j24 Q1) |
   | IM[1] | `Store` | ASEL = 000, FF.0 = 1, FF.1 = 1 |
   | IM[2] | `Flush` | ASEL = 001, FF.0 = 0, FF.1 = 1 (j24 Q3) |
   | IM[3] | none | ASEL = 100, `WantProcRef'` stays high |

   all with BSEL = 0 so `FFok'` stays low, every JCN keeping a JCN.0 bit set.

   **And it works.** The map planes now read 0 where they read 1 -- the `←Map`
   wrote them -- `MapEven'` is **high on 2354** samples where it was low on
   all 3000, and **`MapTrouble` clears on 2354 of 3000 cycles** where it was
   asserted the entire run. `MapTroubleInMem` is low on 2339, and
   `WriteInMem'` and a clear map **coincide on 352**. Gated; turning the
   `←Map` into a second Store is caught.

8. **The memory size is a backplane input, and it picks the DRAM timing
   PROM.** `MemX07.sil+10` -- the third term of `MemWEa`'s D0 -- is Q0 of
   j13/j14, SG10139s addressed by `{RfshInMem, MemState.0..3}` with pin 15 as
   CE'. Those enables are `Use256/16KProm'` (j13) and `Use64KProm'` (j14),
   driven from **`ChipsAre256/16K` / `ChipsAre64K` -- backplane inputs from
   the MSA**, which is not in this configuration. Undriven, **neither 16K PROM
   is enabled** and the memory state machine has no timing table at all --
   which looks exactly like a sequencer bug and is not one. Gated; leaving
   them undriven is caught.

   Swept: only `ChipsAre256/16K` selects (`ChipsAre64K` moves neither enable).
   With it 0, j14 is enabled and `x10` is low on 96 cycles; with it 1 -- the
   historically correct build -- j13 is enabled and `x10` is never low.

   **A note on the two sockets.** PARC's BCPL names them `MX16k-j13` and
   `MX4k-j14`, and `MemProms.bcpl` records the switch: *"change to
   memx-16k-j13 from -j14. comment-out the memx-4k option. September 26,
   1979."* But **this board is Rev Ch**, later, and calls j14's enable
   `Use64KProm'` -- the socket was re-purposed for a 64K table that does not
   exist in the PROM source we hold. So on a late board j13 is the live socket
   and **j14 should be empty**; our 4K image sitting in it is harmless only
   because the enable keeps it off.

9. **What stops MemState is `StartMem'` itself, and my earlier reading of it
   was backwards.** MemX `j16` is an F10016 with

   ```
   C = Clk0'Dd    CE' = MemIdle    PE' = StartMem'
   H0-H3 -> MemState.0-3,  AND ALL FOUR D INPUTS OPEN
   ```

   Parallel load overrides count, and an open MECL input sits at VEE. So
   **every cycle `StartMem'` is low, the state machine is loaded with zero.**
   Measured: `StartMem'` is low on **2680 of 3000** samples -- 89% -- so the
   counter is free on only **288**, and it reaches all 3 of its observed
   states inside that window (non-zero while held: 32, the transition
   residue).

   An earlier gate read "preStartMem' low on 2712, StartMem' on 2680" as *"the
   storage cycle is being started"*. It is the opposite: **a start held as a
   level is a hold.** This loop restarts the storage cycle faster than it can
   finish one, because it issues a reference every four microinstructions
   forever. Real microcode issues a reference and then does other work while
   the memory completes.

   **Spacing the references was the wrong theory, and measuring it said so.**
   A second hunk of four non-references at IM[4..7], jumps running 0→…→7→0,
   made it **worse**: `StartMem'` free on 192 samples instead of 288, MemState
   still 3 of 16, and the flush stopped working entirely (`FlushStore`,
   `ForceMiss` and both MISS counts all 0, the dirty line no longer surviving
   to its Flush). `WriteInMem' & !MapTrouble` did rise from 352 to 1091, so
   spacing helps *that* pair -- but it does not touch `StartMem'`. Reverted.

   **So `StartMem'` is not driven by our reference rate**, and MemX `j22` says
   why. It is another F10016 with `TrueBD` on CE' -- so it **never counts** and
   is a parallel-load register:

   ```
   C = Clk0'Dc   CE' = TrueBD (constant 1)   PE' = MapWait
   D1 = preStartMem'  ->  H1 = StartMem'
   D0 = MemX06.sil+3  ->  H0 = preRfshInMem
   ```

   i.e. **`StartMem'` is `preStartMem'` latched on `MapWait`**, and
   `preStartMem'` is Q2 of `i14`, the MapState PROM. **The map sequencer
   drives the memory sequencer.** MapState does walk (7 of its 8 values), so
   the question is which of its states assert `preStartMem'` and for how long
   -- and that is **readable, not guessable**: the PROM images come from
   PARC's own BCPL and are gated by `prom-test`, so decode `i14`'s table
   against the MapState values actually visited.

   **`i14`'s table is decoded, the machine is read against it, and the whole
   chain now adds up.** The PROM is addressed `{MapState.2, MapState.1,
   MapState.0, MapFnc.1', MapFnc.0'}` with CE' open, and `preStartMem'` is its
   Q2. Q2 is high at exactly **three** addresses -- 3, 11, 19 -- all with both
   MapFnc bits set (no map function pending) at MapState 0, 2 or 4. So
   `StartMem'` low is the *default*, and being j16's PE' it is a **reset**.

   **Logged against the machine, all three are visited:** address 3 on 672
   cycles, 11 on 64, 19 on 64 -- ~800 where `preStartMem'` is high.
   `StartMem'` is high on only **288** of those, because j22 latches it on
   `MapWait`. And that 288 is **one contiguous window**, not fragments.

   **Inside that window the counter gets about three counts, which is exactly
   what it shows.** `Clk0'Dd` has **18 edges** there (nine rising), and CE'
   (`MemIdle`) is low on only **64 of 288**. Nine clock edges with the enable
   down four fifths of the time is ~3 counts, and MemState reaches 3 values.
   **Nothing is broken in this path** -- the window is simply too short for a
   16-state cycle.

   An earlier prediction from this same table -- that our `←Map`, re-issued
   every fourth microinstruction, was holding the sequencer -- was **wrong**.
   Running the Map once (IM[3] → IM[1]) leaves the window at exactly 288 and
   MemState at 3, while losing the map entry. Reverted.

   **And the sequencer is a closed loop, now mapped end to end.** Two things
   limit MemState, both out of MemX `g13`, an MC10104 whose `OUT` pins carry
   the AND:

   ```
   MapWait = StartMap' & MapFree      (gate b, pins 6,7 -> 3)
   MemIdle = StartMem' & MemFree      (gate d, pins 12,13 -> 15)
   ```

   `MemIdle` is the CE' on j16, the MemState counter, so **inside** the free
   window counting needs `MemFree` low -- and it is low on only **64 of 288**.
   That is the inner limiter, under the window length.

   `MemFree` comes from `j12`, a **fourth** F10016 with `TrueBD` on CE' --
   another parallel-load register -- clocked by `Clk0'Ba`, loading when
   PE' = `MemIdle` is low, with `D1 = MemX07.sil+1 -> H1 = MemFree`. And
   `MemX07.sil+1` is an **output of j13, the DRAM timing PROM**, which is
   itself addressed by MemState:

   ```
   MemState -> j13 (16K timing PROM) -> MemX07.sil+1
            -> j12 (latched on MemIdle) -> MemFree
            -> g13 gate d -> MemIdle -> j16's CE' -> MemState
   ```

   **A proper microsequencer**: the PROM output feeds back to pace its own
   counter, and `MemIdle` is both j16's count enable and j12's load enable. It
   has to bootstrap, which is why a short window with a mostly-high `MemFree`
   leaves it turning over three states.

   **Walking the tables answers it -- but only after reading the cell.**
   `cell_SG10139` is **MSB-first**: "A0 is the most significant *address* bit
   and Q0 the most significant *data* bit", grounded in DiskProms'
   `Pin1 = #200`. A first pass used LSB-first on **both** the address and the
   data, for **both** PROMs, and produced two wrong answers -- a bootstrap
   paradox on j13 that does not exist, and "preStartMem' is high at 3, 11 and
   19" on i14, also wrong. **Read the cell before reading the table.** Third
   time in this file a PARC convention has inverted a conclusion.

   **j13** (16K DRAM timing), address `{RfshInMem, MemState.0..3}`, Rfsh = 0:

   | output | behaviour |
   |---|---|
   | Q5 → `MemFree` | 0 at every **even** MemState, 1 at every odd |
   | Q0 → `x10` | 0 at MemState **4 and 8**, nowhere else |

   `MemIdle = StartMem' & MemFree`, so counting needs an even state, and since
   j12 *latches* `MemFree`, **each memory state takes two clocks**. Nine
   rising `Clk0'Dd` edges in the 288-cycle window ≈ 4 states; 3 observed. The
   model is self-consistent. **`MemWEa` needs MemState 4; the window delivers
   3.**

   **i14** (map PROM), address `{MapFnc.0', MapFnc.1', MapState.0,
   MapState.1, MapState.2}`: Q2 → `preStartMem'` is high at **exactly one
   address, 3** -- both MapFnc bits **low** (a map function *is* pending, the
   opposite of the first reading) with **MapState = 3**. A memory cycle starts
   in one precise state of the map sequencer.

   **The decode and the machine now agree exactly:** address 3 is visited
   **192** times and `preStartMem'` is high on **192**. That equality is the
   check that the convention is finally right.

   **And there is only ever one window, at sample 54** -- `StartMem'` goes
   high once, for 288 cycles, never again. So it is not "make the window
   longer" but **"why does the loop never start another memory cycle?"**

   The address log says where to look. `preStartMem'` is high on 192 cycles,
   **all before sample 213**, while `MapWait` is low on 2776 -- the latch is
   wide open and it is the **PROM input** that stops. The machine then sits at
   address 24 (672 visits) = MapState 0 with no map function pending, i.e.
   idle, plus 28 (483), 8 (448), 0 (384).

   **And the two halves stop coinciding -- neither one stops.** Address 3
   needs **both** MapFnc bits low **and** MapState = 3. Measured separately
   over 3000 samples:

   | condition | cycles | last sample |
   |---|---|---|
   | MapFnc = 00 (function pending) | 597 | 2133 |
   | MapState = 3 | 320 | 2581 |
   | **both** (address 3) | **192** | **213** |

   So the map function keeps being issued and the sequencer keeps reaching
   state 3, but after sample 213 **they never line up again**. That is a
   **phase** relationship, not a missing signal -- the classic shape of two
   things free-running at rates that no longer agree: this loop presents a
   reference every four microinstructions while the map sequencer walks at its
   own pace.

   (Full distributions -- MapFnc `{0',1'}`: 00=597, 01=832, 10=32, 11=1539.
   MapState visits all eight: 0=1536, 1=128, 2=128, 3=320, 4=547, 5=149,
   6=128, 7=64.)

   **And it is a beat frequency, measured.** Logging the onset of every run:

   ```
   MapFnc=00 onsets : 1, 406, 662, 918, 1558, 1814, 2070
                      intervals 405, 256, 256, 640, 256, 256
   MapState=3 onsets: 22, 1494, 2518
                      intervals 1472, 1024
   ```

   MapFnc=00 recurs every **256** samples, MapState=3 every ~1024-1472. The
   first MapState=3 run, at 22, falls inside the MapFnc window opened at 1 --
   **that single overlap is the only memory start in the whole run.**
   Afterwards MapState=3 lands in the gaps: 1494 between the window at 918
   (ending ~1003) and the next at 1558; 2518 past 2070's.

   **And 256 samples is four loop iterations** (4 microinstructions × 16
   sys_clk = 64 each), so the map function is accepted only on every *fourth*
   `←Map`. The map has its own accept cadence and this bench asks faster than
   it answers.

   **The loop is phase-fragile in both directions.** Adding **one**
   non-reference at IM[4] -- after the Flush, leaving Store and Flush adjacent
   -- breaks `HitColDirty` just as the earlier four-instruction insertion did.
   So the dirty line's survival is not about adjacency either; it depends on
   the phase this particular loop sits in. At four instructions the
   Store/Flush pair works and the map window is missed; at five the map phase
   moves and the pair breaks.

   **This is an artefact of an artificial loop, not an RTL fault.** Nothing is
   broken: MapState visits all eight values, the reference machinery is gated
   end to end, and the PROM tables agree with the running machine to the cycle
   (192 = 192).

   **And two experiments confirm it, with numbers -- including real Xerox
   microcode.** First, point IM[3]'s JCN at *itself* so Map/Store/Flush run
   once and the machine parks. Second -- **`+realucode`**, wired up in the
   bench -- skip the overwrite entirely and execute the four hunks of
   `AEmu.mb!2` already loaded at IM[0..15]. Xerox's own microcode at its own
   cadence, no hand-built loop:

   | | looping | one-shot | **real AEmu** |
   |---|---|---|---|
   | `StartMem'` high | 288 | 1187 | **2584** |
   | longest window | 288 | 899 | **1589** |
   | in-window `Clk0'Dd` edges | 18 | 74 | **162** |
   | `preStartMem'` high | 192 | 832 | **1205** |

   **Nine times the window and nine times the clock edges.** The beat was
   entirely an artefact of the four-microinstruction loop.

   **And it separates the two limiters for good.** Even with 162 clock edges,
   MemState *still* reaches only 3 -- because CE' (`MemIdle`) is low on just
   **85 of those 1589** samples. `MemFree` is high almost all the time **no
   matter what the cadence is**. So:

   - the **window length** was a bench artefact, now understood and gone;
   - the **inner limiter** -- `MemFree` holding CE' high -- is not, and it is
     what actually stops MemState reaching 4.

   (The AEmu snippet makes references but no write-back: `WriteInMem'` and the
   Store/Flush path read 0 throughout, and no `←Map` runs. It exercises the
   *cadence*, which is what it was used for.)

   **So the next question was `MemFree`, not the loop -- and probing it shows
   the feedback settling, which is by design.** Across a window, `MemFree` and
   `MemIdle` each change **exactly once** (one edge apiece in 288 samples,
   MemFree high on 224). They do not alternate.

   The structure says they cannot, and it is **not** a modelling error -- g13's
   cell is right and PARC's naming agrees, `MemIdle'` on the NAND pin 9 and
   `MemIdle` on the AND pin 15:

   ```
   MemFree = 0                 -> MemIdle = 0 -> j12 LOADS (MemFree follows Q5)
   MemFree = 1, StartMem' = 1  -> MemIdle = 1 -> j12 HOLDS, MemFree stuck high
   MemFree = 1, StartMem' = 0  -> MemIdle = 0 -> j12 loads: ESCAPE
   ```

   **The escape is `StartMem'` going low** -- which is also what reloads
   MemState with zero. So a storage cycle is **self-limiting**: it runs a few
   states, parks when `MemFree` latches high, and is **re-triggered** by the
   next reference. It is not meant to free-run through all sixteen states, and
   MemState reaching 3 in a window is the machine behaving, not stalling.

   **Which re-frames the `MemWEa` question one last time.** `x10` is 0 at
   MemState 4 and 8, so the write phase is **one state past** where these
   windows park. The question is no longer "why does the sequencer stop?" --
   it stops on purpose -- but **"what does a real write-back reference do
   differently that carries it one state further?"** The AEmu snippet makes
   references but never a write-back (`WriteInMem'` reads 0 throughout), so it
   cannot answer that; a microcode window that actually stores to memory can.
   The same conclusion the cadence work reached, now for a second and
   independent reason.

   **Worth generalising, and the count is now four:** i10, j22 and j12 all
   have `TrueBD` -- a hardwired constant 1 -- on CE', so all three are
   parallel-load **registers** that never count. **j16 is the only real
   counter** of the four, and its CE' is `MemIdle`. On MemX this part is a
   load register more often than a counter, so read PE' and the D inputs
   **first**, and do not reason about counting or terminal count until CE' is
   shown to go low.

**Three sampling traps in one file.** The first read an instant instead of
counting edges; the second read the end of a run instead of the interesting
moment; the third conditioned on a field that picked the wrong instruction
entirely. Count edges, and identify the running instruction by a field you
have read back out of IM.


## Where the boot stands (2026-08-22)

The five-stage chain, what is done and what blocks the rest, is the ROADMAP TO
BOOT at the top of `docs/verilog-from-sil.md`. Short version: stages 0 and 1
are done and cross-checked against the C emulator; stage 2 runs but only with
IM parity disabled; stage 3 is started -- the memory section is clocked, in a
running machine, and being asked for storage -- and stage 4 needs I/O device
backends that do not exist. 29 gates.

**The single thing blocking stage 3, stated exactly.** The microcode ASKS for
storage and nothing COMPLETES. `WantProcRef'` asserts, MAR carries a real
register value, and the reference is ACCEPTED -- `RefHold'` reads 1, so it is
not stuck at the front door. But the Pipe pointer does not move over a whole
run, and `MDhold'`/`MiscHold'` read 0: the processor is held waiting for
memory DATA. `DdataGood'` is properly connected (MemX e19 drives it) and says
the data is not good, because nothing has ever completed a storage access.

So the next thing to build is a behavioural storage array behind MemD's
interface. That is forced, not a shortcut: main storage lives on separate
**MSA** boards (144 `MK4096P-6` DRAMs each) for which the archive has only
`.lc`/`.bp`/`.nl` and no wire list, so it cannot be generated from Sil even in
principle.

**Two capacity questions, one settled.** Memory is not a risk on MiSTer:
`tools/sil_ram_groups.py` measures 246 M10K worst case against the DE10-Nano's
557, so the machine fits without merging the bit-sliced RAMs at all (merging is
a ~40% saving, not a gate). Logic is the open one, and the estimate is
arithmetic rather than a fit -- synthesise to find out.

**A full OS boot will not happen in gate-level simulation.** Simulation proves
the chain stage by stage, each gate cross-checked against the C emulator; an
FPGA is where anything boots. Do not spend effort trying to make a simulation
boot an OS.



## SYSPER: the oversampling ratio is now testable, and 8x breaks three gates

**Why this matters.** The RTL recovers each distributed ECL clock net by
sampling it SYSPER times per microinstruction, so real time on an FPGA needs
`sys_clk = SYSPER x 16.67 MHz`. Measured Fmax for the eleven-board machine is
**48.99 MHz** (`fpga/quartus/reports`), which makes SYSPER=2 the real-time
target and the default 16 a 0.18x machine.

**The benches could not test it.** Six carried a local
`localparam integer SYSPER = 16` for scaling their own waits, and the machine
carried its own `parameter integer SYSPER = 16`. **The two were never
connected** -- they simply both happened to say 16 -- so no bench had ever run
the machine at a different ratio. They pass it down now, overridable:

    make -C verilog/verilator taskrun-test SYSPER=8

**At 16 all six pass. At 8, three fail:**

| gate | at 8x |
|---|---|
| display-test | pass |
| disk-test | pass |
| readback-test | pass |
| **taskrun-test** | **fail** -- the startup Link<- did not land in task 0's slot alone, and BNT stops following the priority encoder |
| **memrun-test** | **fail** -- FSinPair' never fell, so the flush was not latched into the pair |
| **ifufetch-test** | **fail** -- the Pipe pointer advanced twice, where ProcSRN is a FIXED slot and non-I/O references must all overwrite it |

**The first is a KNOWN CLASS, and its recurrence is the point.** The handoff
already records it against `cell_F10145A`: taking "write while WE' is LOW"
literally writes on every sys_clk the pulse spans, and if the address moves
inside the pulse it writes SEVERAL CELLS -- "that is exactly what happened at 8
sys_clk per microinstruction: the startup `Link<-` landed in every task's slot
instead of task 0's ... **it survived at 16x only because the pulse happened to
sit inside one stable address there.**"

The cell was fixed to write once, on the rising edge of WE'. It fails again at
8x, so the remaining question is whether the ADDRESS is still stable at that
edge when there are half as many samples -- a question about when the address
is captured, not about how many writes occur.

**These are real bugs in the cell models, not bench artifacts.** They are
exactly what a lower ratio exists to expose, and they must be fixed before
SYSPER=2, which is what a real-time FPGA Dorado needs.
