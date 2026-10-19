# Dorado RTL clock implementation plan

Started 2026-10-18 UTC. Native49 is the selected50MHz Cedar desktop/input
baseline (fresh49b repeats it); Native34 is the earlier50MHz reference and
Native26 preserves the earlier33.333MHz reference. The user has released MiSTer14 for reloads/faster builds; keeping
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
Native34 is the earlier desktop/input reference; Native35 provides
the earlier diagnostic baseline. SDRAM41c now passes original-firmware
Viewers/input and41d cold marker readback. Direct-DDR network47 passes real
startup/login, loader traffic and packet progress during delayed Main worker
work; full47/48 Viewers/input and active STOP/reload now pass. External IFS/ContrAlto tunnelling is
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

**Complete MemX serial state access is qualified, saving 212 ALMs in an isolated matched fit. The last combined core remains 3,318 ALMs over capacity.**

`generate_memx_ff_scan.py` chains all 74 unchanged FF helpers / 819 physical
bits, without a shadow bank. Compact FF words 0..45 end in nine valid bits.
All 21 independent 64K map planes retain their own 23-bit row/column, edge,
pending-write, read-pipeline and output histories (483 bits total). Ten small
files/write windows, fourteen phase/delay bits and eight immutable PROMs keep
their qualified maps and semantics. Density, clocks and guest memory behavior
are unchanged. `compact-ff-v1` provides explicit offsets and
legacy_parallel_word review metadata, without parallel-address aliases.

The unchanged original board, independent per-socket map oracle and physical
RAM probes pass 28,943,989 host edges, 28,742,687 holds and 26,418,989 requests.
All 1,378,312 physical modeled bits/all 16,777,216 address values, unused padding,
destructive restore with a pending map write, quiet recapture, first resumed
edge and 20,000 more cycles pass. All 21 map and ten small-file writers are
exercised. Ten damaged variants reject, including lost history, busy ownership,
reset access and lost local/file/map hold. All 31 helper frozen flags are
observed in quarantine, with auxiliary state and physical map data held.
Fresh XML accounts for 1,378,312 physical + 724 private transport bits, no
sequential exclusions and 2,048 immutable PROM bits; nineteen damaged
inventories/layouts reject. At the acknowledged frozen boundary saved_dout_r
is each socket's canonical original read pipeline; the borrowed RAM read
register and response state remain private, with first-edge equivalence tested.

Matched enabled fits retain identical async MLAB files, map helpers and the
original staged-input/positive-edge 50 MHz/20 ns shell and SDC. Area falls from
2,355 to 2,143 ALMs; registers rise from 2,750 to 2,835. All 819 FF bits,
483 socket histories, ten write windows and fourteen phase/delay bits are
checked. Each map plane retains eight distinct M10Ks: 168 total / 1,376,256
bits, with original OLD_DATA and port latency. Twenty MLAB replicas retain
1,168 bits; actual disconnected normal output pins justify pruning, while
admin replicas retain all four bits. Two common frozen/pending flags retain
their total, with exact synthesis ownership checked. All four device conditions
pass, minimum +0.125 ns; no new cuts, unconstrained paths or warning categories.
Eleven damaged source/retention/constraint/hold/collision/placement controls
reject. All 661 raw files pass hash readback; extracted fit/XML/source
regeneration and damaged controls pass. Evidence:
`fpga/mister/reports/2026-10-19-snapshot-memx-ff-scan/`.
Quartus session 17581 and simulation 52011 are terminal.

Next: MemC, remaining control/BaseBoard access and a combined full-core
capacity/timing audit. Earlier isolated savings remain qualified; none are
subtracted from the last measured whole-core result of 45,228 / 41,910 ALMs.
No production builder, RBF, live FPGA or speed change is claimed. Before
integration, version schemas/formatters, extend the 16-edge transport budget,
and provide global hold/fault/SYSTEM-reset ownership across boards, foreign
consumers, backends, input and ARM services. Local quarantine is insufficient.

Classify/expose the 154,037 native-wrapper candidate bits (review-only),
top-level input/network/video and 16 MiB external raw RAM. Complete coherent
ARM/disk/input/server barriers that preserve connections/rebase timers, and
atomic bundles paired with the matching disk generation. Actual FPGA Cedar
save/continue/restore, original 60 ns clocks/timers, pinned comparisons, three
fresh boots/cold persistence and two-hour stress remain open. The last
qualified Native59 idle rate stays 1,947,654.12 issues/s. Goal ACTIVE. All
experiments in this entry are terminal; older running-job entries are historical.

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
