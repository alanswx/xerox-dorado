# Complete FPGA snapshots: board state access experiments

Complete machine save/restore is required milestone4a in
[the delivery plan](../../../docs/verilog-clock-plan.md). It is not implemented
in the running MiSTer core. This directory now qualifies bounded RAM access
and complete modeled ContB state restore in isolated RTL/routed experiments.
Machine-wide state access, coherent barriers and physical restore remain open.

The complete actual BaseBoard now passes 40,366-bit save/restore, untouched
original/independent-model comparison, fresh complete XML classification and
matched four-condition timing (+0.159ns). It retains 11 M10Ks and exact firmware
bytes. This remains an isolated board experiment. Next is complete-core
integration and enabled capacity/timing, then global barriers and physical
machine restore. Evidence: reports/2026-10-19-snapshot-baseboard-full.

The remaining BaseBoard clock/reset primitives now pass original/oracle/
restore, fresh XML and matched timing(+0.145ns). Nine new families cover87
actual bits;192bits reuse Native59 adapters unchanged. Fresh actual-board
classification maps all279bits/30packages. These are isolated helpers;
complete BaseBoard integration/save/restore/timing is next, followed by
whole-core enabled budget and coherent global barriers. Evidence:
reports/2026-10-19-snapshot-baseboard-ff.

All eight BaseBoard EPROM pipelines now pass complete72bit four-state
original/oracle/restore and actual-board classification. Empty sockets stay
uninitialized. Corrected matched fits retain four original images/eightM10Ks,
pass four conditions(+0.165ns), and verify synthesized image bytes exactly.
The first logic-inference fit is rejected and preserved. These are isolated
helpers; remaining279clock/reset/FFbits and complete BaseBoard/global
qualification are next. Evidence: reports/2026-10-19-snapshot-i2716.

The actual BaseBoard's four 2125 byte banks now pass complete 32,800-bit
original/oracle/restore tests, wire-list grouping, fresh XML classification
and matched four-condition timing (+0.211ns). They retain four M10Ks and
reuse the qualified shared RAM transport unchanged. These helpers are isolated;
Native59's production BaseBoard is unchanged. Next are EPROM registered reads,
remaining clock/reset state, then full BaseBoard and global qualification.
Evidence: reports/2026-10-19-snapshot-i2125.

Latest wrapper qualification: the actual `cell_MCS6532` phase_d plus all
RIOT state now pass complete1,098-bit original/oracle/restore tests and four-
condition matched timing, minimum+0.162ns. These remain isolated helpers;
Native59's original BaseBoard is unchanged. Native59 now passes fresh full
Viewers/mouse/exact typing/warm file readback. Next remaining BaseBoard RAM/
EPROM/clock-reset state, whole-board qualification and an enabled global
resource/barrier/restore check. See the current handoff for live artifacts.

Latest RIOT qualification: `generate_riot.py` / `test_riot.py` /
`classify_riot.py` / `fit_riot.py` / `audit_riot.py` now qualify all1,097bits
of the actual MOS RIOT, including RAM/read output/timers and block-local PA7
history. Four-condition matched timing passes minimum+0.176ns. The enclosing
MCS6532 phase-history bit remains required; full BaseBoard/global save/load
are not implemented. Native59 has completed the base-loader final ACK and
is installing fonts; see the current handoff before historical notes.

Latest physical update: Native59 is now normally loaded on MiSTer14 after
pushed full-core qualification; original firmware boot is running. The actual
netlist6502 adapter now also passes matched50MHz routed qualification with
all1725nodes retained and minimum+0.151ns across four conditions. Full BaseBoard
and global save/load remain open; see handoff for the live watcher/job handles.

Latest qualification (2026-10-19): Native59 now passes complete full-core
source/memory and four-condition timing checks with ten board adapters
administratively inactive. Native58 remains the interactive hardware core;
Native59 hardware boot is next. The actual BaseBoard inventory and all-node
6502 simulation/classification are archived. A corrected matched6502 fit is
running; no full BaseBoard/global machine save/load is installed. See the
current [handoff](../../../docs/verilog-handoff.md) before historical notes.

Update2026-10-19: the qualified ContB host-clock/exact-advance adapters are
qualified in the full Native51 core on MiSTer14. Native49c is now the live
matched control historically; live Native52 adds ContA adapters and now passes
original firmware/Pilot/Cedar login, with loader/desktop qualification pending. Four-condition whole-core/crossing
timing, original firmware, Cedar login and all34 base transfers pass. ALL
snapshot admin inputs remain inactive; the running core cannot save/load
state.51 full Viewers/mouse/exact keyboard and warm marker now pass. The
slower-loader/replay investigation remains open;49 is the performance baseline. Evidence:
reports/2026-10-19-native51-contb and native51-field. Historical isolated
experiments below did not themselves change production RTL.

`f10415_bank.sv` models an IM-like1024x34 bank from34 original F10415A slices.
It exports1024 RAM words plus a separate34-bit read-pipeline word. The RAM
read always clocks. During scanning, a saved register supplies the guest's
previous read output. On resume, the first ordinary edge performs the normal
read/write; no guest warm-up edge is inserted. The caller must hold every
other guest clock/state at the same boundary. That system barrier is not
provided by this prototype. Do not wire this into production as a complete
pause implementation.

The requester asserts `freeze` after holding guest state, waits for `frozen`,
then issues requests. Each accepted request produces `done` and its read data
after one clock; write-response data is unspecified. Word0..1023 accesses RAM,
word1024 accesses the saved pipeline, and every larger11-bit index returns
`error` without writing. Requests during entry or resume are not accepted.
Transfers use complete34-bit words, keeping write enables shared so Quartus
can pack the slices. A future byte/DDR serializer must assemble a complete
word before committing it. This is a local bank interface, not a snapshot
file format or the final host protocol.

Run `python3 fpga/mister/snapshot/test_f10415.py --mutations` on the Mac.
The test uses34 unchanged original cells with a separately controlled reference
clock, plus an independent array oracle. It checks every exported/restored
bit, a saved read output deliberately different from its current RAM word,
every invalid index, destructive continuation and first-edge resume, and
30000 random running steps. Four broken variants must fail: lost pipeline,
scan output leakage, running access and invalid-address aliasing.

`fit_f10415.py --output NEW_DIRECTORY` runs isolated matched projects on
misterubuntu with Quartus17.0.2, the MiSTer5CSEBA6U23I7 device,50MHz and
virtual data pins with2ns input/output budgets. It does not program a board.
`audit_f10415.py DIRECTORY` checks source hashes, memory geometry/packing,
OLD_DATA mixed-port collision configuration, all four operating conditions
and unconstrained-path reports. Recovery/removal have no applicable paths
in this synchronous bank.

The qualified whole-word fit retains34816 RAM bits/fourM10Ks in both variants.
Baseline101 ALMs becomes163, with43 additional registers. All12 setup/hold/
pulse-width checks per variant pass across four conditions; snapshot minimum
slack is+0.096ns. These are isolated virtual-I/O measurements, not a full-board
resource or timing budget. Two rejected experiments show why inference must
be checked: separate conditional write addresses expanded the bank into
logic, and independent per-bit restore enables used34 M10Ks. The production
original cell is unchanged; the prototype additionally requests defined
old-data collision behavior instead of its `no_rw_check` attribute.

Next: prove grouping/access on an actual board with its real strobes and
clock-history registers, then extend the inventory and barrier to external
SDRAM, all devices, input and ARM disk/local network-server state. A complete
bundle must pair a matching disk generation, validate schema/build/hashes
before changing live state, and pass actual Cedar desktop restore tests.
No physical snapshot, fast-boot timing or cross-build compatibility is claimed.

Actual ContB grouping is now checked by `audit_contb_im.py` and
`test_contb_im.py`. The physical IM is4096x36:34 instruction bits and2 stored
parity bits,144 original RAM packages and144 registered read bits. Following
actual buffered address/WE/CE expressions proves eight18-bit groups, two
independently written halves per bank. This is the grouping for the next
board-access experiment; the qualified34-bit bank above remains an isolated
prototype. Six damaged-board controls reject; a common inversion of all
address copies is explicitly outside a grouping proof's coverage. Evidence:
reports/2026-10-19-snapshot-contb. Neither production RAM nor clocks change.

The next18-bit prototype is `f10415_enable_bank.sv`. It uses a single host
clock with the actual guest advance as a read/write enable. This preserves
ordinary backend holds while permitting frozen scans; the saved read output
survives release until the first real guest edge. Original-cell/array tests
and five broken variants pass. Matched registered-shell fits retain twoM10Ks
and OLD_DATA, with all four-condition checks passing, minimum+0.255ns.
Fixture counts are53->102ALMs/48->138registers and include launch/capture
registers; do not extrapolate that delta to a complete core. The two-clock
prototype also in this directory is REJECTED by Quartus inference. Earlier
direct-virtual-input timing failures are preserved. Evidence and full scope:
reports/2026-10-19-snapshot-enable. No physical save/load or complete snapshot
is implemented by either prototype.

`generate_contb.py` now creates a separate actual ContB experiment with the
original144 RAM slices replaced by the eight enabled half-banks; all other
board logic stays unchanged. `test_contb_snapshot.py --mutations` compares
all71 outputs against the original board, actual write/CE/address/parity cones
and raw read pipelines, plus an independent array.97,418 host cycles and
57,392 state operations pass, including late clock-enable changes, every
RAM/pipeline bit, invalid rows, cross-bank responses and first-edge resume.
Four damaged RTL variants reject. The matching isolated board fit retains
16M10Ks/147456bits; all four-condition checks pass min+0.168ns, no cuts or
unconstrained paths. Shell counts287->600ALMs/325->584regs include admin
pruning differences; do not project to the core. Eight audit controls reject.
Evidence: reports/2026-10-19-snapshot-contb-board. Production is unchanged.

The subsequent `generate_contb_full.py` experiment adds all209 remaining
MC10176/MC10231 q, pre-capture and clock-history bits. The complete modeled
board now passes save/candidate-only continuation/restore/first-edge execution
against an untouched original board. All71 outputs and147809 modeled state
bits are covered; all209 FF bits are individually walked, all invalid15-bit
addresses are rejected and six broken RTL controls fail. Fresh original-board
XML classification accounts for every candidate; seven missing/overlapping/
aliased-state controls reject. External inputs and clock phase are outside
this board boundary and must be captured by the caller.

Matched complete-board isolated fits retain16M10Ks/147456bits/OLD_DATA and
pass all12 checks per variant/four conditions, minimum+0.165ns. No cuts or
unconstrained paths. Shell counts288->774ALMs/320->693regs (+486/+373) include
admin pruning differences, not whole-core overhead. Nine damaged-evidence
controls reject. Sources: generate_contb_full.py, test_contb_full.py/.cpp,
classify_contb_full.py, fit_contb_full.py and audit_contb_full.py. Evidence:
reports/2026-10-19-snapshot-contb-full. Production/live FPGA remain unchanged.
Next integrate access in the complete-machine experiment, expand/classify the
other boards and wrapper/device/input scope, establish the global services
barrier and qualify whole-core timing before physical Cedar restore.

The remaining delivery order is:

1. Classify the generated inventory and prove bounded access on an actual
   machine. Complete modeled ContB access/continue/restore and isolated timing
   now pass; other boards, wrapper/devices/inputs and whole-core timing remain
   required. Inventory/classify every remaining state path before release.
2. Implement one machine-wide pause boundary with ARM disk/network services.
   Preserve or drain pending operations explicitly; keep SDRAM refresh running.
3. Save all classified RTL, RAM, device, clock and input state together with
   matching disk contents/generation and local Pup/BSP/STP server state.
   Validate the complete versioned, checksummed bundle before restoring it.
4. Qualify same-build save/continue/restore, including pending I/O and rejected
   corrupt/mismatched bundles. Restore a real FPGA Cedar desktop, verify input,
   transfers and subsequent disk writes, and measure time against a cold boot.

The intended user operation is Save/Load machine state. It remains a required
part of release acceptance alongside fresh original-firmware cold boots.

The next primitive, `generate_f10145.py`, now preserves each16x4 file's64 RAM
bits and all10 pending-write-window bits. `test_f10145_snapshot.py --mutations`
checks the unchanged original and an independent oracle, including freeze
inside WE low, candidate-only continuation and exact first-edge commit.
All74 bits/invalid indices/backend holds pass; eight damaged RTL controls fail.
`classify_f10145.py` accounts for original XML candidates, explicitly excluding
106 synthesis-translate-off diagnostic bits with source guards and11 failure
controls. Raw native inventory counts remain review candidates, not physical
snapshot size. Matched isolated fits pass four conditions/min+0.146ns, no cuts
or unconstrained paths, zero block RAM.57->117ALMs/89->142registers include
registered shells/pruning; no core budget or hardware save/load claim.
Evidence: reports/2026-10-19-snapshot-f10145. Next integrate/classify remaining
ContA FF/latch families and six clock-history bits, then global machine state.

Full frozen49 ContA now passes modeled board restore in
`generate_conta_full.py`/`test_conta_full.py`: all1,353 non-diagnostic modeled
bits and138 outputs, including75 FF packages, nine register files with write
windows and six phase-history bits.59,787 host edges,13,334 late-enable changes,
all bits/invalid addresses and first-edge resume pass; seven damaged RTL
variants fail. Fresh XML classification excludes954 original simulation-only
diagnostic bits and rejects17 altered inventory/layout/source cases. The
modeled bit count is not a fitted baseline register count. Matched isolated
fits pass four conditions/min+0.163ns, no cuts/unconstrained paths, zero block
RAM.769->1974ALMs/1343->1951regs include shell/pruning. Evidence:
reports/2026-10-19-snapshot-conta-full. No production/live FPGA change yet.
Next whole-core ContA+ContB clock-enable integration and original boot, then
remaining board/device/ARM access and complete safe machine restore.


Both asynchronous256-word RAM families now have isolated bounded access via
`generate_async256.py`. MB7071H256x4/F10414256x1 preserve1280bits, original
select/level-write/asynchronous-read behavior and exact host advance. Scans
borrow the read address and CAN change combinational guest outputs: the
caller must hold all consumers. They do not provide a global pause. Reads
return saved RAM words; writes return zero, indices256..511 reject. Original/
array/complete restore/first-edge tests,14 faulty RTL variants, XML coverage
and12 damaged schemas pass. Matched routed pair retains1280MLABbits/40cells,
zeroM10Ks; all four-condition timing passes min+0.354ns.212->227ALMs/47->64regs
include the shell. The first two-write-address implementation expanded to
registers and is rejected/preserved. Evidence2026-10-19-snapshot-async256.
Next complete processor board state/access/restore/timing; physical full-core
snapshots, global service barriers and original60ns clocks remain required.


Complete modeled ProcH/ProcL adapters now pass5712/5581mutable bits, all
75/96outputs, bit/invalid-address walks, destructive continuation/restore/
first guest edge and20000more cycles against untouched original boards.
Every original RAM writer exercised. Fresh XML pins512immutable PROM bits
per board and the ALU combinational table;16 damaged schemas/source controls
per board reject. Eight broken RTL variants per board fail. Isolated matched
positive-edge50MHz fits pass all four conditions, min+0.136/+0.038ns, retaining
4608MLABbits/144cells each, zeroM10Ks. Earlier10ns opposite-edge shells also
failed the originals and are rejected/preserved. Positive-edge shell models
the real processor-neighbor edges,20ns budgets, identical SDC clock/I/O and
no cuts. Source generate/test/classify/fit/audit_processor_full.py and
processor FF helper; report2026-10-19-snapshot-processor-full. Full-core
integration/crossings/original firmware and complete physical snapshots remain
required. The running52 includes ContA/B adapters only, all admin inactive.

Native53 now integrates both qualified processor adapters into frozen52 with
all admin inactive, the same actual falling-edge-latched advance and unchanged
outer wrapper/clocks/SDCs. `build_native_processor.py` pins all source/PROM
dependencies, reuses eight existing adapter files byte-identically and adds
six RTL files. Routed timing passes31summary/100detailed checks/four conditions,
min+0.115ns, no new cuts or illegal/unconstrained clocks; unchanged framework I/O.
All12processor MLAB RAMs/9216bits/288cells and eight OLD_DATA instruction banks
are retained.30232ALMs/52845regs/330M10Ks/35DSP with admin inactive. Evidence
reports/2026-10-19-native53-processors. Normally loaded53 now passes original
firmware/new BFN113/release/Pilot/Cedar login and all34base transfers/final ACK.
Idle1.947682Missues/s matches52; zero protocol replays in this loader interval,
311.447..311.698s. Full desktop/mouse/exact keyboard/warm marker readback now
pass; final performance qualification remains pending. See native53-field/handoff;
no speed promotion from this single boot.
`audit_native_processor.py` checks100 detailed timing categories across four
conditions, including both directions for all four adapted boards. No complete
snapshot, enabled snapshot resource budget or speed gain is claimed.

The next IFU primitive, `generate_jk.py`, preserves MC10135's seven modeled
bits: qa/qb, four setup samples and shared clock history. `test_jk.py` compares
all four PRECAP combinations with untouched original cells and an independent
oracle over92195 host edges, all128 state words and first-edge restore plus
20000 subsequent cycles. Seven damaged RTL variants fail; three altered-source
and eight XML/schema controls reject. `classify_jk.py` accounts for all original
state and nine derived wires. Evidence: reports/2026-10-19-snapshot-jk. This is
simulation/coverage plus matched isolated timing, not IFU integration. Negative-
edge launch/positive-edge capture50MHz shell passes four conditions,min+0.164ns,
68->73ALMs/38->74regs/zeroRAM; all five corrupted-fit controls reject. Same
cell RTL/SDC,10ns into cell/20ns output paths and2ns max0ns min I/O. Earlier
positive-edge shell failed virtual-input hold at-0.465ns; multi-corner option
alone did not repair it. Both failed fits are rejected/preserved in the report.

IFUM grouping review `audit_ifu_im.py` now pins all27 RAM packages against the
original wire list. Although programming words contain11/16bits (24data/3parity
total), separately registered address replicas remain independent. Preserve
eleven groups widths1/5/1/1/4/1/5/2/1/4/2 and all27 read-pipeline bits. All27 CE
ports are omitted in the original generated board/wire list; no explicit zero
tie-off is introduced or qualified. Seven damaged boards reject. Evidence:
reports/2026-10-19-snapshot-ifum-grouping. This is logical grouping review only;
width-correct old-data RAM/state access and full IFU coverage/restore/isolated
timing now pass as recorded below. Physical machine snapshots/global barriers
remain open.

`f10415_word_bank.sv` qualifies widths1/2/4/5/18, raw read pipelines, clipped
transport padding, invalid addresses and first-edge restore against untouched
originals/oracle. Eight faulty RTL/11schema/seven fit controls reject. Matched
five-bank50MHz fits retain30,720bits/sixM10Ks,min+0.171ns/all four conditions.
`generate_ifu_full.py` then preserves28,653mutable bits/256immutable PROM bits,
all eleven independently addressed RAM groups and original omitted CE. Complete
original/oracle/bit/invalid/destructive restore/first-edge tests pass498,875host
edges/448,849requests/20,000post-restore cycles; seven brokenRTL/14schema
controls reject. Positive-edge50MHz matched fits pass min+0.157ns/four conditions,
27,648RAMbits/four->elevenM10Ks,466->1474ALMs/867->1832regs including shell/admin.
Nine bad fits reject; all11physical banks/macros OLD_DATA. Reports
snapshot-f10415-word/snapshot-ifu-full preserve exact sources and earlier failed
fixtures/auditors. Two-state qualification; no new CE tie or four-state proof.
`build_native_ifu.py` prepares Native54 against frozen53, reusing five shared
FFs and adding three RTL files. Full-core timing/firmware qualification is next;
all admin remains inactive, so this provides no physical machine save/load.

Native54 full-core IFU timing now passes31summary/116detailed checks/four
conditions,min+0.114ns, no new cuts/clock or framework-I/O coverage changes.
All11IFUM groups retain OLD_DATA; processor MLABs and eight IM banks are
unchanged.30342ALMs/53052regs/330M10Ks/35DSP with administration inactive.
Normal fresh-seed deployment is under original-firmware qualification; see
native54-ifu/handoff. No speed gain or enabled-snapshot resource claim.

Next `generate_f100181.py` preserves the four-bit sampled ALU latch without
changing arithmetic/lookahead or adding BCD correction. Original/ripple-oracle/
all16384pin combinations/all16states/holds/restore/20,000continuation tests,
freshXML classification and matched isolated timing pass,min+0.150ns. Five
badRTL/three changed-source/nine schema/five fit controls reject. Initial
positive-edge virtual-input hold failures are rejected/preserved; accepted
matched negative-edge launch/positive-edge capture gives10ns into cell/20ns
output paths with unchanged RTL/SDC. Evidence snapshot-f100181.

Complete frozen MemC model generation/test/classification now passes23,718
mutable bits:507FF, five phase,21,504asyncRAM,1472smallRAM/230write-window bits.
Its44original writers all fire through normal board pins, including directed
clock/decoder/bank sequences.682,448host/177,507requests, complete bit/invalid
walks, destructive restore/first edge/recapture/20,000continuation, ten badRTL
and16schema controls pass.44RAM responses use a six-bit selection index;
five-phase transport is padded to18bits. Verilator's encoded double-underscore
package names are accounted for in XML paths. Isolated timing subsequently passes as recorded below; full-core integration,
device/ARM coherence and whole snapshots remain.

MemC isolated matched timing now passes all four conditions,min+0.152ns,
21,504MLABbits/672cells/zeroM10Ks retained,2294->5390ALMs/2340->3357regs.
Both variants add a negative-edge external input stage before positive-edge
launch/capture, retaining20ns embedded paths and unchanged cellRTL/SDC.
Earlier original-only virtual-input hold(-1.960ns) rejected/preserved; eight
bad-fit controls reject. Report snapshot-memc-full. Native55 full-core build
adds only MemC board/F100181clone, reuses13existing adapters and keeps admin
inactive. Full-core timing now passes as recorded below; original hardware
qualification remains required, with no speed claim.

Native55 full-core MemC fit/source audit passes31summary/132detailed/four
conditions,min+0.040ns, no new cuts/clocks or changed framework I/O coverage.
All21MemC MLABs/21,504bits/672cells and inherited processor/IFUM/IM RAM
properties retained;30588ALMs/52757regs/330M10Ks/35DSP with admin inactive.
Candidate NOT DEPLOYED; current54desktop/input is under qualification.
Evidence native55-memc. Physical snapshots/60ns clocks remain unfinished.

Next `audit_memd_im.py` pins72cache bit-cells to the originalwirelist and
proves19literal/14canonical groups of widths3/6/9 without equating stateful
replicas; eight damaged boards reject. Indexed storage-delay assignments
remain opaque. New `f10470_word_bank.sv` qualifies4096words plus raw pipeline
at widths1/3/5/6/9/18:284,039host/245,818requests/172,074state-bit checks,
destructive restore/first resume/recapture/20,000further cycles. Eight badRTL/
11schema/eight fit controls reject. Matched50MHz fits retain172,032RAMbits/
23M10Ks, all six real macros/physical rows OLD_DATA, all four conditions
min+0.168ns,332->500ALMs/169->478regs including shell/admin/pruning.
Reports snapshot-memd-grouping/snapshot-f10470-word. Full MemD board state/
restore/timing and global machine/device/ARM snapshots remain separate work.

Native54 now passes full Viewers/Clock/Watch/Edit, actual mouse/Cmd then
separate initialized focus, exact short/alphabet/digit echo and warm file
create/close/readback. Normal guest845/Main barriers preserve its volume.
Native55 subsequently loaded normally and passes original firmware/BFN113/
released C/Cedar login and1,947,777.041idle issues/s, matching54. Loader/full55
desktop qualification is pending under a bounded observer. All six board
adapters remain admin inactive; there is no full physical save/load or60ns
clock migration yet. Evidence native54-field/native55-field.


## Zero-delay MemD qualification (2026-10-19; native-setting correction below)

`generate_memd_full.py`, `test_memd_full.py/.cpp`, `classify_memd_full.py`,
`fit_memd_full.py` and `audit_memd_full.py` cover296,554mutable native MemD
bits:968FF/history,ten phase,294,912cache RAM,72rawreadpipeline,512smallRAM,
80small-file window. This experiment covered STORAGE_INPUT_DELAY=0 only; other settings explicitly
fatal. Full-core integration subsequently proved actual native delay is TWO;
that guard rejected the first integration before an RBF. Its earlier native
label was incorrect; raw zero-delay evidence is preserved with an erratum.14reviewed canonical cache
banks retain original CE/WE/pin order; all22memory responses use5selectorbits.

Original/oracle/fixture-only physical probes exercise all14cache/eight
small-file writers, allstatebits/all1,048,576indices, destructive whole-image
restore/recapture/firstactualadvance/20,000subsequent cycles. Ten brokenRTL
controls and nonzero delay reject; fresh XML14damaged schemas reject. Exact
50MHz matched staged-input/positive-edge fits pass fourconditions,min+0.164ns,
no unconstrained/cut paths.294,912cacheRAMbits/38->40M10Ks,604->2267ALM and
1353->2340regs include shell/admin/pruning, not whole-core enabled costs.
All14macros/physicalbanks OLD_DATA; nine damaged fits reject. Initial frozen
original Verilog parser failure retained; accepted project uses SystemVerilog
for the same original's optional-loop declaration, with no RTL/SDC relaxation.
Evidence reports/2026-10-19-snapshot-memd-full. Full-core MemD integration,
remaining boards/device/ARM state/global barriers and actual FPGA restore are
still required; this does not change production clocks or provide snapshots.


## Actual native2 MemD and generated-scope inventory correction (2026-10-19)

Native55 QSF binds DORADO_STORAGE_INPUT_DELAY=2. Both input words and edge
history are now exported:296,591mutable bits. Other delays explicitly reject.
The XML inventory now includes generated lexical scopes; a real nested-scope
regression covers arrays, histories and same-name variables without aliasing.
Fresh classification16damagedschemacontrols reject, with64source-pinned loop
index bits explicitly recomputed (each overwritten before use).

Native2 original/oracle/physical-probe simulation passes allbits/all20bitindices,
all14cache/eight small-file writers and14,265actual storage captures, complete
destructive restore/recapture/firstedge/20,000cycles. Twelve broken RTLs plus
unsupported delay reject. Matched isolated50MHz/four-condition timing passes,
min+0.107ns, all14actual cache banks OLD_DATA/294,912bits/38->40M10Ks.
620->2358ALM/1389->2411regs include shell/admin/pruning, not enabled fullcore.
Nine badfitcontrols reject. Evidence snapshot-memd-native2 includes rejected
first56 integration/authoritative native parameter; zero-delay evidence remains
historical. Fresh56b fullcore integration is building; actual global save/load,
remaining boards/device/ARM state and original60ns migration remain open.


## Native56b full-core timing (2026-10-19)

`build_native_memd.py`/`audit_native_memd.py`: actual native two-stage MemD
adapter, eight inherited helpers reused byte-identically, only board/cache
helper new; all admin inactive. Exact frozen55clocks/SDCs/wrapper/policies.
31summary/148detailed/seven-board crossings/fourconditions pass,min+0.116ns.
All14MemD physical RAMs OLD_DATA, inherited IFUM/IM/MemC/processor geometries
retained.30,303ALM/52,996regs/332M10Ks/35DSP, not enabled snapshot budget.
Eleven bad source/QSF/hold/RAM controls reject. Read-only audit dependency/
scoped-control fixes preserved without RTL/SDC change/refit. RBF0bbe092f,
not deployed; original-firmware hardware qualification remains next. Evidence
reports/2026-10-19-native56b-memd. Global snapshots and60ns remain open.


## Independent MosRam socket state (2026-10-19)

`generate_mosram.py`, `test_mosram.py/.cpp`, `classify_mosram.py` and
`fit_mosram.py` qualify14/16address-bit original sockets independently. All
RAM plus21/23history bits (row/column, RAS/CAS/WE, pending write/data, raw
read pipeline and held output) are exported; no per-socket history merging.
81,964modeled bits/all invalid indices/padding/destructive RAM+history restore/
quiet recapture/firstpendingwrite and20,000morecycles pass against original+
independent oracle+physical probes. Eleven brokenRTLs/unsupported density reject;
freshXML12bad-schema controls per density reject. Matched50MHz/fourconditions
min+0.166ns,no cuts/unconstrained I/O,81,920bits/tenM10Ks retained. AUTO style
and table Type remain original; actual2/8M10Klocations and zero MLABs/OLD_DATA
prove placement.59->137ALM/57->256regs include shell/admin/pruning; eight corrupt
fits reject. Earlier label/diagnostic-only audit failures retained; no RTL/SDC
refit. Evidence reports/2026-10-19-snapshot-mosram. Full MemX classification/
restore/timing/integration/global device/ARM snapshot state still required.

## DskEth inventory and CRC9401 primitive (2026-10-19)

The actual frozen DskEth rxmonitor board is now inventoried with its explicit
SYSPER4 and required34bit receive-monitor state. First missing-dependency failure
and corrected evidence are retained in snapshot-dsketh-inventory. Its tentative
2,656mutable bits need complete independent classification; no board adapter yet.

The F9401 primitive preserves all19bits in both actual PRECAP modes, including
clock/data/CWE histories. Original RTL and independent CRC oracle, all bits and
indices, destructive restore/first edge/20Kmore, fresh XML and matched50MHz
isolated timing pass (fourconditions/min+0.170ns). Eight broken RTL variants,
ten damaged schemas per mode and five bad-fit controls reject. Original CRC
behavior/source unchanged. Sources: generate_crc9401/test_crc9401/classify_crc9401/
fit_crc9401. Evidence snapshot-crc9401. Whole DskEth/global/ARM snapshots and
original60ns cadence remain unfinished.

Four more DskEth local state-word families now pass original/independent-oracle
restore simulation, fresh XML and matched50MHz isolated timing(+0.164ns):
MC1650(2bits),MC10136(12),SN169(8),SN174(13,PRECAP0/1).54bad schemas,8broken RTL
and5damaged fit controls reject; no production cell/shared helper changes.
Sources generate/test/classify/fit_dsketh_ff; report snapshot-dsketh-ff.
Oscillator simulation now additionally covers actualDskEth23530/SYSPER8, with
original/generator unchanged (report snapshot-oscillator-dsketh); the added pair
requires full-board fit and later timer calibration. SN74123 timers, full
DskEth board/monitor/phase restore and global/ARM state remain open.

SN74123 timers now pass complete70bit export/restore, original/independent
signed oracle, six actual/boundary parameter modes, freshXML signed types and
matched50MHz isolated fits(+0.140ns/fourconditions).9brokenRTL,17bad schemas per
mode and5badfitcontrols reject. Sources generate/test/classify/fit_sn74123;
report snapshot-sn74123. No timer-rate/production change. Full DskEth/monitor/
phase state integration and global/ARM snapshot boundaries remain open.

Complete modeled actual DskEth, including34bit passive monitor, now passes
all2,656mutable/12,032immutablePROMbits, all18normal RAM writers, destructive
per-word shadow restore/quiet/firstedge/20Kmore, freshXML and four-condition
isolated50MHz timing(+0.147ns).9brokenRTL/16bad schemas/11badfitcontrols reject.
Baseline5M10Ks hold immutable ROMs; candidate PROMs are LUTs, with all14assets
unchanged; all141statehelper synthesis register counts are checked. Full native59
build/hardware/global snapshots remain unqualified. Sources generate/test/
classify/fit/audit_dsketh_full and generate_receive_monitor; report
snapshot-dsketh-full. build_native_dsketh prepares frozen58+qualified adapter
using11byte-identical helpers/8newfiles, all administrative inputs inactive.
No clock/speed gain, timer calibration or complete-machine save/load claim.

## Enabled eleven-board diagnostic transport (2026-10-19)

`board_state_fabric.sv` routes one outstanding 18-bit operation to all eleven
qualified adapters. It requires freeze, the stopped guest-enable boundary and
all eleven frozen acknowledgments. It checks board ID and the complete 24-bit
index before narrowing to the selected board. A registered owner selects the
reply; writes and errors return zero. Missing replies time out after sixteen
waiting edges. A timeout or lost boundary quarantines requests until thaw.

`board_state_uio.sv` uses EXT_BUS commands76..79, beside unchanged Ethernet
commands70..75. Commands mutate state only on deselection after an exact
packet, including its command word. Words are16bits:

| Command | Request words after command |
| --- | --- |
| 77: hold/release boards | B04D, mode0/1, ABI1 |
| 78: read/write state | B04D, operation0/1, boardID, indexLow16, indexHigh8, dataLow16, dataHigh2 |
| 76 or79: coherent status | Clock eleven further words after the initial reply |

Nonzero padding, wrong key/ABI/operation, short/overlong packets, running
requests and requests while busy are rejected. Busy release is rejected too.
Status is captured on the command word: D0B0, ABI1, flags, boardCount11,
frozenMask, sequenceLow/High, resultLow/High, error, controlErrorsLow/High.
Flag bits0..6 are freeze, guestHeld, allFrozen, busy, complete, replyError,
quarantined. Bit8 is permanently1 to identify board-only scope. Errors1..5
mean bad board, index high bits, child error, timeout, lost boundary.

The board order and index widths are ContA12, ContB15, ProcH16, ProcL16,
IFU18, MemC16, MemD20, MemX24, DskEth13, DispY19, BaseBd17. The actual child
adapter additionally checks its selector/row validity. This interface is
diagnostic access, not a complete machine pause: raw memory, device wrappers,
input, ARM work/server connections and disk generation are not captured.

Four-state simulation v6 passes9,974ticks/433packets/201checks with all eleven
stub boards, delayed/error replies, timeout/re-entry, lost boundary, counter
rollover, a completion during status and the actual unchanged Native59 Ethernet
UIO. All13broken RTL controls reject. Initial index0/board0 exposed an Icarus
initial-sensitivity issue; `always_comb` supplies the required initial decode.

`build_enabled_core.py` prepares the complete BaseBoard plus22asset files,
reuses4byte-identical shared helpers and adds18sources. It enables every board
interface (1,944,744modeled bits) in a frozen Native59-derived experiment.
Original clocks, SDRAM/ECC and timing constraints stay unchanged. The builder
does not assemble/deploy an RBF. `audit_enabled_core.py` independently checks
all413baseline receipt files,2,137original board pins,85other core files,
state routes, boundary/bus wiring and source/constraint policies;15broken
integrations reject. The audit also passes against the actual remote build.
Quartus17's optional complete set of relative source aliases is checked to
resolve exactly to the18new files. Evidence: reports/2026-10-19-snapshot-board-state.

The first full-core synthesis passes, but fitting is terminal/rejected at
54,403 ALMs versus 41,910 available (5,505 LABs versus 4,191). No timing or
hardware operation is qualified. Evidence: reports/2026-10-19-snapshot-enabled-capacity.

## First enabled-area reduction: shared RIOT RAM port (2026-10-19)

`generate_riot_shared_port.py` folds mutually exclusive normal/administrative
RAM accesses into one address/data/write-enable port. It preserves all 128x8
RAM bits, exact whole-array reset, read-before-write, latency and the existing
133-word ABI. There is no new state or validity/reconstruction scheme.
`test_riot_shared_port.py` reuses the unchanged original/independent oracle:
all 1,097 bits, addresses, timers, reset/hold and restore/first-edge/20K further
cycles pass; six broken variants reject. Fresh XML and synthesis retain all
1,097 physical + 21 interface bits. Matched enabled fixtures use identical
50MHz constraints; four conditions pass, minimum +0.167ns, 1,645 -> 782 ALMs.
Seven damaged fit/retention controls reject. These are isolated shell counts.

`generate_baseboard_shared_riot.py` changes only the RIOT child and its manifest.
`test_baseboard_shared_riot.py` selects it in the unchanged full-board fixture;
all 40,366 physical bits/131,072 indices, original outputs, independent models,
destructive restore and 20K further cycles pass. This run repeats the positive
board test; earlier SRAM writer and broken-board coverage remains separate.
Only M6532_snapshot.sv differs among the 22 prepared HDL assets.

The opt-in `build_enabled_core.py --shared-riot` full build is terminal at
50,414 ALMs versus 41,910 available: 3,989 fewer than the first experiment,
still over capacity. Every board remains enabled. Evidence:
reports/2026-10-19-snapshot-shared-riot-capacity. No new RBF or speed change.

`inventory_machine_support.py` also elaborates exact staged source/macros to
inventory the native wrapper outside the eleven boards. It identifies 154,037
sequential candidate bits, mostly disk buffering/raw-memory frontend state.
This is REVIEW ONLY. Top-level input/network/video, external RAM and ARM state
remain outside that XML and need their own classification and global barrier.

## Shared register-file writer and rejected MLAB placement (2026-10-19)

`generate_f10145_shared_write.py` folds exclusive normal/restore RAM writes;
both read ports, asynchronous output and the complete pending-write window
stay unchanged. The original/independent oracle passes all 74 physical bits,
restore/first-edge/20K further cycles; eight broken variants reject. Fresh
XML and both synthesized cell hierarchies retain all 74 physical + 13 interface
bits. Matched enabled fixtures pass four conditions (minimum +0.172ns),
117 -> 96 ALMs, zero block RAM; seven damaged fit controls reject.

`test_boards_shared_f10145.py` runs the unchanged positive full-board fixtures
against the folded helper. All eight affected boards pass, covering 1,727,916
modeled state bits; generated board wiring is identical to Native59. Accepted
v2 covers the first six; v3 uses actual Native58/57 parents for DskEth/DispY.
The newer MC10170 comments are checked token-identical; actual frozen cell
bytes are compiled. Prior broken-board suites are not claimed rerun.

`generate_f10145_mlab.py`/`fit_f10145_mlab.py` retain the rejected strict-memory
experiment. Functional RTL differs only in the hint. Timing passes, but
Quartus cannot infer either read-during-write RAM, uses zero MLAB bits and
increases area from 96 to 107 ALMs. This is not a selectable core variant.

The opt-in `build_enabled_core.py --shared-riot --shared-files` experiment
passes source audits and synthesis but is terminal/rejected at48,353ALMs versus
41,910available:6,050fewer than the first all-enabled experiment,6,443still
over capacity. Exactly the two old register-file QSF aliases are replaced.
The source auditor rejects seventeen damaged integrations and retains its
fifteen legacy controls. No timing or hardware qualification follows this
failed fit. Evidence: reports/2026-10-19-snapshot-shared-files-capacity,
reports/2026-10-19-snapshot-f10145-shared-write and
reports/2026-10-19-snapshot-f10145-mlab-rejected.

## Zero private write replies and asynchronous RAM (2026-10-19)

`generate_f10145_zero_write.py` returns zero on private administrative writes;
the public fabric already does so. Administrative RAM reads explicitly exclude
writes. RAM/state reads, ordinary asynchronous output and pending-write capture
remain unchanged. `test_f10145_zero_write.py` derives a copy of the unchanged
original/oracle harness with an additional zero-write-response check. All74bits,
35,407host edges,7,987holds,pending-write restore and20K further cycles pass;
ten damaged variants reject. `classify_f10145_zero_write.py` selects the
candidate in the qualified fresh-XML classifier;74physical+13interface bits,
no sequential exclusions and seven damaged inventories pass. All eight affected
positive full-board restore fixtures also pass. Earlier broken-board suites
are separate evidence, not claimed rerun.

The first zero-reply strict MLAB trial also infers no RAM (two Info276009
messages), uses0MLABbits and expands96 ->107ALMs despite passing timing;
its executed sources and rejected result remain in the follow-up report.

`fit_f10145_mlab.py --zero-write-reply --original-async-mlab` tests the explicit
read guard with the original asynchronous `MLAB, no_rw_check` style. Functional
preprocessed variants differ only in the memory hint. Two physical16x4MLAB
read replicas are inferred (128bits), both with unregistered read inputs and
outputs. The cell retains23owned control/interface registers and8inferred
RAM-input registers,31total; the logic reference retains87. The matched shell
uses97 ->69ALMs and147 ->87registers. All four timing conditions pass, minimum
+0.346ns, with unchanged50MHz/opposite-edge constraints and no added cuts.
`audit_f10145_zero_async.py` verifies exact sources, retention, actual memory
geometry/latency/locations and timing; six damaged evidence controls reject.
819raw archived files pass SHA256 readback; independent extracted-fit audit
passes. Report: reports/2026-10-19-snapshot-f10145-zero-async.

The opt-in core flags are `--shared-riot --shared-files --zero-write-reply
--files-async-mlab`. The full-core build
`misterubuntu:/home/alans/mister/Dorado_Enabled60ZeroAsync_20261019` is terminal:
synthesis passes, fitting rejects45,228ALMs /41,910available (3,318excess).
It saves3,125ALMs versus the shared-writer core,9,175versus the first enabled
core. No STA/assembly/deployment followed. Local and actual remote source audits
pass413baseline receipt files/17damaged integrations. The independent
`audit_enabled_f10145_mapping.py` derives all85files from the board manifests;
all170replicas are inferred. Every administrative copy retains64bits; twelve
ordinary copies are narrower only where the compiled board leaves output pins
absent. Each width is checked against those pins. Total replicated bits10,480,
logical RAM bits5,440. Seven damaged mappings reject;29archived raw files and
extracted mapping audit pass. Report: reports/2026-10-19-snapshot-zero-async-capacity.
Whole-core timing, firmware boot and physical snapshots remain unproven. Global support-state and ARM/server/input/matching-disk barriers
are still required; the diagnostic board hold is not a machine snapshot.


`generate_riot_row_decode.py` exposes explicit row writes while retaining RAM,
reset timing/priority and the 133-word ABI. Original/independent simulation and
fresh XML preserve all 1,097 physical + 21 interface bits; six damaged RTLs and
seven damaged inventories reject. The matched job is terminal: 782 ALMs and
1,323 shell registers in both variants, all four conditions passing, minimum
+0.167 ns. Seven damaged fit/retention controls reject. It is rejected for no
area benefit; no BaseBoard rerun/core integration follows. All 172 raw files
pass readback. Report: reports/2026-10-19-snapshot-riot-row-decode-rejected.

## Serial FF scan controller prototype (2026-10-19)

`ff_scan_controller.sv` tests an area-reduction architecture for FF access:
rotate the existing bank through a one-bit chain, capture/replace only the
requested compact 18-bit word, and complete only when the entire bank is back
in its original orientation. No shadow bank is stored. Every consumer must
remain held. The hold_required output stays asserted while busy or faulted;
boundary loss quarantines requests/shifting until SYSTEM reset. A transport-
only reset cannot recover partially shifted physical state.

`test_ff_scan_controller.py` uses an independent direct word/bit oracle and a
mock bank. V2 passes 438 transactions across 1/17/18/19/56/1725 bits, including
partial words, busy-owner stability, invalid/high indices, freeze entry,
partial-rotation hold loss and system-reset recovery. Four damaged controllers
reject. All 111 raw files pass readback. Report:
reports/2026-10-19-snapshot-ff-scan-controller.

This does not qualify a real board, resource reduction, timing or a global
snapshot. Next: chain actual heterogeneous FF helpers, generate an explicit
compact schema/ABI, prove ordinary/complete-state restore behavior and measure
matched area/timing. Global hold/reset ownership must prevent guest execution
during an incomplete rotation; existing board-only diagnostics are insufficient.


## Complete ContA serial FF access (2026-10-19)

`generate_conta_ff_scan.py` connects the unchanged75FF helpers into a681-bit
chain. Compact FF words0..37 replace the old per-package access map; RAM/window
packages75..83 and phase84 keep their addresses. Every operation rotates the
whole chain before replying. A lost boundary locks all local FF/RAM/window/
phase state until SYSTEM reset; reset-time accesses are rejected and re-entry
requires a fresh freeze acknowledgment. Global consumers still need a separate
barrier. The ff[] metadata retains legacy package information; use scan.layout
for the new compact mapping and version the ABI before integration.

`test_conta_ff_scan.py --mutations --async-files` derives the qualified full
board fixture/oracle. It passes1,657,868host edges/all1,353physical bits/all138
outputs/all-bit access/destructive restore/first-edge/20Kmore, with all nine RAM
writers active. Six broken variants reject. `classify_conta_ff_scan.py` checks
fresh XML against the original physical schema:1,353physical+206private bits,
no exclusions/eight damaged inventories reject. Six-size primitive controller
regression repeats438transactions/four controls after explicit offset sizing.

`fit_conta_ff_scan.py` compares enabled parallel/serial boards with identical
asynchronous F10145 helpers and50MHz/opposite-edge constraints. Corrected
serial area is1,472 ->1,261ALMs; shell registers1,373 ->1,494. All681FF bits,
18placed MLAB replicas/576logical RAM bits/1,136replica bits and asynchronous
read latency are checked. Four conditions pass, minimum+0.131ns; no new timing
cuts or unconstrained paths. `audit_conta_ff_scan.py` rejects six damaged fit/
retention/RAM-latency controls. All1,324raw files pass readback; independent fit
and classification audits pass again after extraction. Report:
reports/2026-10-19-snapshot-conta-ff-scan.

This is an isolated211ALM saving; the actual full core remains3,318over capacity.
No core builder or live FPGA change. Next: more board adapters and a combined
fit, schema/formatter updates, longer transport timeout (current16edges versus
681for a scan), and global hold/fault/reset ownership. The earlier207ALM trial
and exact harness/launch failure history remain in the report. Complete machine
snapshots, original60ns clocks and final release qualification remain open.


## Complete processor serial FF access (2026-10-19)

`generate_processor_ff_scan.py` extends serial access to ProcH/ProcL:502bits in
64FF helpers and519bits in69helpers. RAM/window/phase addresses remain intact.
New compact FF words0..27/0..28 have explicit scan.word_map entries and
state_access_schema=compact-ff-v1. Old addresses become legacy_parallel_word;
ff.word is absent, preventing an implicit old-layout access alias. No shadow
bank is introduced. Local busy/fault hold, reset rejection and re-entry use
the qualified controller/ContA behavior; global hold ownership remains separate.

`test_processor_ff_scan.py` preserves each full processor original/independent
oracle and raw probes. All5,712/5,581bits, all outputs, all65,536indices,
destructive restore/first-edge/20Kmore pass; all14/12RAM writers are active
(two ProcL writers only once). Six damaged variants reject each. Fresh XML
classification accounts for5,712physical+233private and5,581physical+209private
bits, no exclusions; nine damaged inventories/layouts reject per board.

`fit_processor_ff_scan.py` uses matched enabled boards, identical asynchronous
F10145 helpers and the qualified50MHz/positive-edge/20ns shell/SDC. ProcH saves
215ALMs (1,950 ->1,735), minimum+0.137ns; ProcL saves196 (1,946 ->1,750),
minimum+0.096ns. All four conditions pass. Exact FF retention, all22/18physical
MLAB rows/read latency and immutable PROM bytes are checked. No new warning
category, cut or unconstrained paths. Six damaged fit controls reject each.
All966raw files pass readback; extracted fit and classification audits pass.
Report: reports/2026-10-19-snapshot-processor-ff-scan.

No core builder/FPGA or speed change. The full-core result remains3,318ALMs
over capacity; these are isolated measurements. Continue remaining boards and
combined capacity/timing qualification, schema/timeout changes and global
hold/fault/SYSTEM-reset integration before selecting this in a live core.


## Complete display serial FF access (2026-10-19)

`generate_dispy_ff_scan.py` chains all140unchanged FF helpers/1,439bits, with
compact words0..79 (last17bits). RAM/window, raw pipeline, oscillator and phase
access remain intact; defaults/CE omission/PROMs are unchanged. The explicit
compact-ff-v1 layout replaces ff.word aliases with legacy_parallel_word.
No shadow bank is added. Busy/fault holds every local helper until SYSTEM reset;
reset-time access is rejected and re-entry requires a new freeze acknowledgment.
Foreign consumers still require a global barrier.

`test_dispy_ff_scan.py --mutations` preserves the original full display and
independent RAM oracle. It passes7,874,288edges/all13,993bits/all524,288indices/
destructive restore/raw pipelines/first-edge/20Kmore, all15normal writers active.
Seven damaged variants reject. A surviving word-bank hold mutation in v1 exposed
coincidentally unchanged data; direct freeze observations for all16helpers
close that test gap. V3 completes the strengthened suite. Fresh XML accounts for
13,993physical+232private bits/no exclusions and2,048PROM bits; twelve damaged
inventories/layouts reject. Word saved_q is the canonical original pipeline at
the acknowledged frozen/pending boundary; raw private read stages remain outside
that architectural alias, with first-edge equivalence tested.

Matched enabled display fits use identical asynchronous RAM helpers, staged
inputs, positive-edge50MHz/20ns budgets and SDC. Area2,814 ->2,313ALMs saves501;
all four conditions pass, minimum+0.124ns. All1,439FF bits,18placed memory rows,
12,480logical/12,672replica RAM bits, three independent M10Ks and OLD_DATA are
checked. No new cuts, unconstrained paths or warning category. Eight damaged
fit/source/retention/hold/collision/placement controls reject. All940raw files
pass readback; extracted fit/classification/source regeneration pass again.
Report: reports/2026-10-19-snapshot-dispy-ff-scan.

No production builder/live FPGA/speed change. Complete-core capacity remains
3,318ALMs over budget; these are isolated counts. Continue remaining boards,
combined fit/timing and versioned schema/timeout/global-hold integration. The
current16-edge transport cannot serve1,439-edge scans unchanged. Full machine
snapshots, original60ns clocks and final hardware release gates remain open.


## Complete disk/Ethernet serial FF access (2026-10-19)

`generate_dsketh_ff_scan.py` chains all 117 original FF helpers / 1,070 bits,
without a shadow bank. Compact words 0..59 end with eight valid bits. RAM and
write windows, CRC PRECAP modes, signed timers/trigger histories, oscillator,
receive monitor and phase access keep their original qualified map and rates.
`compact-ff-v1` supplies explicit bit offsets; `legacy_parallel_word` is review
metadata, not an address alias. System-reset access is rejected. Loss of the
external freeze/held boundary during rotation holds every local helper and
requires SYSTEM reset; foreign consumers still need a global barrier.

`test_dsketh_ff_scan.py --mutations` derives only administrative mapping/waits
and reset/quarantine checks from the original complete-board test. The unchanged
original board and independent RAM oracle pass 3,947,818 host edges, all 2,656
bits/all 8,192 indices, destructive restore, quiet recapture, first resumed edge
and 20K more cycles. All 18 normal RAM writers are exercised. Nine damaged
variants reject, including corrupted CRC/timer high bits and monitor quarantine.
All 24 helper frozen flags are observed. Fresh XML accounts for 2,656 physical
and 452 private bits, no sequential exclusions, and 12,032 immutable PROM bits;
14 damaged inventories/layouts reject.

Matched enabled fits retain identical async MLAB helpers, staged-input/
positive-edge 50 MHz/20 ns shell and SDC: 2,447 -> 2,092 ALMs saves 355;
2,560 -> 2,684 registers. All four device conditions pass, minimum +0.115 ns.
Exact FF/write-window/CRC/timer/oscillator/monitor retention and all 36 physical
MLAB replicas are checked: 1,152 logical / 2,256 implemented RAM bits, with
normal-output pruning grounded in actual disconnected pins. Both variants keep
identical PROM assets and no block RAM. Ten damaged fit/source/retention/hold/
placement controls reject; no new timing cuts, unconstrained paths or warning
categories. All 847 raw files pass hash readback, followed by extracted fit,
fresh XML classification and source regeneration/control suites.
Report: `reports/2026-10-19-snapshot-dsketh-ff-scan`.

No core builder, FPGA or speed change. The last actual whole-core result stays
3,318 ALMs over capacity. Continue the remaining boards and combined fit/timing;
integration also requires versioned formatters, longer bounded transport and
global hold/fault/SYSTEM-reset ownership. Complete machine/ARM/disk-paired
snapshots, original 60 ns clocks and final hardware release gates remain open.


## Complete IFU serial FF access (2026-10-19)

`generate_ifu_ff_scan.py` chains all 94 unchanged FF helpers / 974 physical bits,
without a shadow bank. Compact words 0..54 end in two valid bits. The eleven
independently addressed IFUM groups, 27 raw read pipelines and four phase bits
keep their existing maps. CE remains omitted at every original cell/bank;
there is no RAM regrouping, tie-off, rate change or immutable PROM alteration.
`compact-ff-v1` gives explicit offsets; legacy_parallel_word is review metadata.
Loss of the external frozen boundary holds every local bank until SYSTEM reset;
access during reset and reset re-entry is rejected. Foreign consumers still
need a global barrier.

`test_ifu_ff_scan.py --mutations` retains the complete original IFU, raw probes
and independent RAM oracle; only admin mapping/waits and reset/quarantine tests
are derived. It passes 3,559,110 host edges, all 28,653 physical bits/all 262,144
address values, destructive restore, quiet recapture, first resumed edge and
20K more. All eleven normal RAM writers are exercised. Eight damaged variants
reject, including lost pipeline, busy ownership and lost local/bank hold, with
every bank's freeze acknowledgment observed. Fresh XML accounts for 28,653
physical + 171 private bits, no exclusions and 256 immutable PROM bits. Fourteen
damaged inventories/layouts reject. Canonical serialized comparison preserves
all original pin-cone expression fields across tuple/JSON-array representation.

Matched enabled fits retain the original positive-edge 50 MHz/20 ns shell and
SDC and identical eleven-bank RAM helper: 1,482 -> 1,100 ALMs saves 382;
1,802 -> 1,762 registers. All four device conditions pass, minimum +0.112 ns.
All 974 physical FFs, 27 canonical pipeline registers, eleven independent M10K
sites / 27,648 RAM bits, port latency and OLD_DATA behavior are checked. Three
shared private bank flags retain their total; synthesis ownership is checked.
Nine damaged fit/source/retention/hold/collision/placement controls reject.
All 495 raw files pass hash readback, followed by extracted fit, fresh XML,
source and archived original wire-list/location regeneration/control suites.
Report: `reports/2026-10-19-snapshot-ifu-ff-scan`.

No production builder, RBF or live FPGA/speed change. Combined capacity remains
3,318 ALMs over budget; isolated savings are not a new whole-core measurement.
Remaining control/memory/BaseBoard access, combined fit/timing, versioned
formatters/longer bounded transport/global hold, full ARM/disk-paired machine
snapshots, original 60 ns clocks and final hardware release gates remain open.


## Complete native MemD serial FF access (2026-10-19)

`generate_memd_ff_scan.py` chains all 92 original FF helpers / 968 physical
bits, without a shadow bank. Compact words 0..53 end in fourteen valid bits.
Fourteen independent cache groups, their 72 raw read pipelines, eight small
files/write windows, ten phase bits and both native input words/clock history
(37 bits) retain their qualified map and behavior. STORAGE_INPUT_DELAY=2 is
preserved; unsupported settings reject. There is no cache/map/ECC, timer or
clock-rate change. The compact-ff-v1 schema supplies explicit offsets and
legacy_parallel_word metadata. All local helpers remain held on a lost scan
boundary until SYSTEM reset; foreign consumers still require a global barrier.

`test_memd_ff_scan.py --mutations` retains the original full board, independent
cache/input-pipeline oracles and physical RAM probes. It passes 5,556,005 host
edges, all 296,591 bits/all 1,048,576 address values, unused padding, destructive
restore/quiet recapture/first resumed edge/20K more. All fourteen cache and eight
small-file writers are active; 14,314 input-pipeline advances occur. Eleven
damaged variants reject, with all 22 helper freeze flags observed in quarantine.
V1 stopped at an earlier-than-expected broader diagnostic for lost input clock
history; V2 completes with the original diagnostic alternatives accepted. RTL
and derived C++ are unchanged; both histories are archived. Fresh XML accounts
for 296,591 physical + 336 private transport bits, no sequential exclusions;
sixteen damaged inventories/layouts reject.

Matched enabled fits retain identical async MLAB files, cache RAM helper,
staged-input/positive-edge 50 MHz/20 ns shell and SDC: 1,863 -> 1,453 ALMs saves
410; 1,917 -> 2,072 registers. All four device conditions pass, minimum +0.112 ns.
Physical checks cover every FF, write window, cache pipeline and board/input
field; fourteen cache groups retain forty distinct M10Ks / 294,912 RAM bits
and OLD_DATA. Sixteen MLAB replicas retain 992 bits; normal-output pruning is
justified against actual disconnected pins, with all four admin bits retained.
Three shared private flags retain their total with exact synthesis ownership
checked. Eleven damaged fit/source/retention/hold/collision/placement controls
reject; no new cuts, unconstrained paths or warning category. All 776 raw files
pass hash readback and extracted fit/XML/source/original wire-list/location
regeneration/control suites. Report: `reports/2026-10-19-snapshot-memd-ff-scan`.

No production builder or FPGA/speed change. The last combined core remains
3,318 ALMs over capacity; no isolated savings are subtracted. Continue MemX,
MemC/control/BaseBoard and combined fit/timing. Versioned formatters, longer
bounded transport/global hold, ARM/disk-paired full snapshots, original 60 ns
clocks and final hardware release gates remain open.


## Complete MemX serial FF access (2026-10-19)

`generate_memx_ff_scan.py` chains all 74 original FF helpers / 819 physical bits,
without a shadow bank. Compact words 0..45 end in nine valid bits. Every map
socket retains its own 64K plane and 23-bit row/column, edge, pending-write,
read-pipeline and output history. Ten small files/write windows, fourteen
phase/delay bits and eight immutable PROMs keep their qualified maps and
semantics. There is no density, grouping, clock or guest memory behavior change.
The compact-ff-v1 schema provides explicit offsets; legacy_parallel_word is
review metadata. All local helpers remain held on a lost scan boundary until
SYSTEM reset; foreign consumers still need a coherent global barrier.

`test_memx_ff_scan.py --mutations` preserves the original full board, independent
per-socket map oracle and physical RAM probes. It passes 28,943,989 host edges,
all 1,378,312 modeled bits/all 16,777,216 address values, unused padding,
destructive pending-write restore, quiet recapture, first resumed edge and
20K more. All 21 map and ten small-file writers are active. Ten damaged variants
reject, including lost map history and separate lost holds for map/small-file
helpers; all 31 frozen flags and physical map data are observed in quarantine.
Fresh XML accounts for 1,378,312 physical + 724 private bits, no exclusions and
2,048 immutable PROM bits; nineteen damaged inventories/layouts reject.
saved_dout_r is each original read pipeline at the frozen boundary; raw
borrowed-read/response state is private, with first-edge resume verified.

Matched enabled fits retain identical RAM helpers, staged-input/positive-edge
50 MHz/20 ns shell and SDC: 2,355 -> 2,143 ALMs saves 212; 2,750 -> 2,835 registers.
All four device conditions pass, minimum +0.125 ns. Physical checks retain
all FF/window/phase/socket-history bits, 168 distinct M10Ks / 1,376,256 map RAM
bits, original OLD_DATA and port latency. Twenty MLAB replicas retain 1,168
bits; normal-output pruning is grounded in actual disconnected pins and all
four admin bits remain. Two common frozen/pending flags keep their total;
exact synthesis ownership is checked. Eleven damaged fit/source/retention/
hold/collision/placement controls reject; no new cuts, unconstrained paths or
warning categories. All 661 raw files pass hash readback and extracted
fit/XML/source regeneration/control suites.
Report: `reports/2026-10-19-snapshot-memx-ff-scan`.

No production builder or FPGA/speed change. The last combined core remains
3,318 ALMs over capacity; isolated savings are not subtracted. Continue MemC,
control/BaseBoard and combined fit/timing, with versioned formatters, longer
bounded transport/global hold, ARM/disk-paired complete snapshots, original
60 ns clocks and final hardware release gates still open.


## Complete MemC serial FF access (2026-10-19)

`generate_memc_ff_scan.py` chains all 63 unchanged FF helpers / 507 physical
bits, including eight sampled ALU latches. Compact words 0..28 end in three
valid bits. Twenty-three small files/write windows, twenty-one async RAMs and
five phase bits keep their original maps and behavior. Frozen RAM bytes must
match the exact qualified clone sources. No shadow bank, arithmetic, memory,
clock or timer change. The compact-ff-v1 schema gives explicit bit offsets;
legacy_parallel_word is review metadata. Loss of the external frozen boundary
holds every local helper until SYSTEM reset; foreign consumers still need a
global barrier.

`test_memc_ff_scan.py --mutations` preserves the original full-board stimulus,
reference and raw physical state probes. It passes 1,571,025 host edges,
all 23,718 bits / 5,797 state words / 65,536 address values, unused padding,
destructive restore/quiet recapture/first resumed edge/20K more; all 87 outputs
are compared outside held async-address scans. All 44 normal RAM writers are
active. Ten damaged variants reject; all 44 helper frozen flags and complete
raw state are observed in quarantine. V1/V2 retain test build path/constant
fixes. V3 caught the lost ALU latch at an earlier broad state comparison; V4
completes with either correct diagnostic accepted. V3/V4 derived C++ and
emitted RTL/schema are identical; a source-only frozen RAM guard is stronger.
Fresh XML accounts for 23,718 physical + 535 private bits, no exclusions;
fourteen damaged inventories/layouts reject. Negative-coordinate socket names
are explicitly escaped, including in the damaged-field controls.

Matched enabled fits retain identical async helpers, staged-input/positive-edge
50 MHz/20 ns shell and SDC: 4,002 -> 3,803 ALMs saves 199; 1,909 -> 1,995 registers.
All four device conditions pass, minimum +0.155 ns. Exact FF/ALU/window/phase
retention, twenty-one 256x4 RAMs and 46 small-file replicas / 67 MLAB rows /
24,432 physical MLAB bits are checked, including original port latency. Normal
output pruning is grounded in actual disconnected pins; all four admin bits
remain. Private shared frozen/input-register ownership is accounted for.
Eleven damaged source/retention/hold/geometry/placement controls reject; no new
cuts, unconstrained paths or warning categories. All 1,079 raw files pass hash
readback and extracted fit/XML/source regeneration/control suites.
Report: `reports/2026-10-19-snapshot-memc-ff-scan`.

All three memory-board serial adapters are qualified in isolation. No core
builder, RBF or live FPGA/speed change; the measured whole-core result remains
3,318 ALMs over capacity. Continue ContB/BaseBoard and combined fit/timing;
versioned formatters, longer bounded transport/global hold, ARM/disk-paired
full snapshots, original 60 ns clocks and final hardware gates remain open.


## Complete ContB serial FF access (2026-10-19)

**Complete ContB serial state access is qualified, saving 58 ALMs in an isolated matched fit. BaseBoard is the remaining board before the combined serial-access capacity build. The last combined core remains 3,318 ALMs over capacity.**

`generate_contb_ff_scan.py` chains all 22 original FF helpers / 209 physical
bits without a shadow bank. Twelve compact words occupy 16384..16395, ending
in eleven valid bits; the eight IM groups and their raw read pipelines retain
their original access maps. The exact frozen Native59 board, wire list,
locations and cell bytes are bound explicitly; the MC10170 difference from
the current tree is comments only, with identical tokens. `compact-ff-v1`
records offsets and legacy_parallel_word review metadata without aliases.

The unchanged original full-board stimulus and independent IM oracle pass
281,347 host edges, 241,290 holds, 13,334 late-enable changes and 82,675
requests. All 147,809 modeled physical bits / 32,768 address values, padding,
destructive restore, quiet recapture and the first resumed edge plus 20,000
more cycles pass. All 71 output bits are compared, and every normal IM writer
is exercised. Nine damaged variants reject. A new reset-entry acknowledgment
prevents access through stale child frozen flags immediately after SYSTEM
reset. Quarantine checks every RAM helper's frozen flag, all shifted FF bits,
canonical read pipelines and direct RAM probes while guest changes continue.
Fresh XML accounts for 147,809 physical + 250 private transport bits with no
sequential exclusions; fourteen damaged inventories/layouts reject.

Matched enabled fits retain the original gated-clock shell and identical
50 MHz/20 ns SDC and IM helpers: 777 -> 719 ALMs; 703 -> 747 registers.
All 209 FF bits, 144 canonical read bits, 147,456 RAM bits and sixteen distinct
M10Ks are retained. Each 1024x18 group keeps its registered input/unregistered
output and mixed-port OLD_DATA behavior. Shared private flag ownership is
checked explicitly. All four device conditions pass, minimum +0.124 ns;
no new cuts, unconstrained paths or unreviewed warning categories. Ten damaged
fit/source/retention/constraint/hold/geometry controls reject. All 414 raw files
pass hash readback; extracted fit/XML/source regeneration and damaged-control
suites pass. Evidence:
`fpga/mister/reports/2026-10-19-snapshot-contb-ff-scan/`.
Quartus session 6747 and simulation 89053 are terminal.

Next: BaseBoard serial access, then a combined full-core capacity/timing audit.
Earlier isolated savings remain qualified; none are subtracted from the last
measured whole-core result of 45,228 / 41,910 ALMs. No production builder,
RBF, live FPGA or speed change is claimed. Before integration, version schemas/
formatters, extend the 16-edge transport budget and provide global hold/fault/
SYSTEM-reset ownership across boards, foreign consumers, backends, input and
ARM services. Local quarantine is insufficient.

Classify/expose the 154,037 native-wrapper candidate bits (review-only),
top-level input/network/video and 16 MiB external raw RAM. Complete coherent
ARM/disk/input/server barriers that preserve connections/rebase timers, and
atomic bundles paired with the matching disk generation. Actual FPGA Cedar
save/continue/restore, original 60 ns clocks/timers, pinned comparisons, three
fresh boots/cold persistence and two-hour stress remain open. The last
qualified Native59 idle rate stays 1,947,654.12 issues/s. Goal ACTIVE. All
experiments in this entry are terminal; older running-job entries are historical.


## Complete BaseBoard serial FF/CPU access (2026-10-19)

**Complete BaseBoard serial state access is qualified, saving 839 ALMs in an isolated matched fit. All eleven board serial adapters are now qualified individually. The last combined core remains 3,318 ALMs over capacity; the combined serial build has not been measured.**

`generate_baseboard_ff_scan.py` chains all 279 clock/reset/FF bits and all
1,725 original 6502 nodes into one 2,004-bit bank without a shadow bank.
Compact words 0..111 end in six valid bits; SRAM/EPROM/RIOT packages 30..46
retain their original access maps. Legacy per-package FF addressing and the
old CPU selector are retired.
`compact-ff-cpu-v1` records all offsets and legacy_parallel_word metadata.
SYSPER4, CLOCKGEN N4, the vendor CPU netlist/relaxation/input/output wiring,
five complete RIOTs, four byte SRAMs, and all eight EPROM pipelines remain.
Both matched fits use the already-qualified shared RIOT RAM writer.

The unchanged original full-board stimulus, physical probes and independent
SRAM/RIOT oracles pass 13,920,731 host edges, 13,822,573 holds, 33,334 late-enable
changes and 402,802 requests. All 40,366 modeled physical bits / 4,890 state
words / 131,072 addresses, padding, destructive restore, quiet recapture and
the first resumed edge plus 20,000 more cycles pass. All 100 output bits are
compared. Normal extra-SRAM writer counts remain zero, as in the earlier full
board gate; their unchanged primitive/bank tests provide normal-write coverage.
All twelve damaged RTL variants reject, including real chain/CPU/FF loss,
completion/ownership, reset access/reentry, SRAM/EPROM/local hold, RIOT phase
and SRAM pipeline corruption. Quarantine checks all seventeen memory/device
frozen flags and the full physical image while guest advance/pins/new writes
are attempted. A fresh reset acknowledgment prevents stale-boundary access.

V1 caught a real generator error at edge2: the old CPU-admin replacement also
removed its original combinational input/output suffix. V2 restores that suffix
and guards its exact bytes. Its positive passes, but a proposed mutation
changed an unused chain wire; V3 targets an actual consumed serial-input pin.
V3 then catches reset reentry at the broader physical-state comparison before
the expected later diagnostic. V4 accepts both valid diagnostics. V2/V3/V4
positive RTL, fixture and derived C++ are byte-identical. V4 reuses exact compiled
fixtures where available only after checking every original/helper/candidate/
fixture/C++/header/oracle byte, and actually runs every binary again, recording
its hash and exit status. All histories are retained.

Fresh XML accounts for 40,366 physical + 582 private transport bits and 65,536
immutable firmware bits, without sequential exclusions; 23 damaged inventories/
layouts reject. The EPROM's qualified raw/restored-byte selection is unchanged,
including the four uninitialized empty sockets. Matched enabled fits preserve
the original staged-input/positive-edge 50 MHz/20 ns shell and SDC:
6,403 -> 5,564 ALMs; 9,136 -> 9,163 registers. All FFs/CPU nodes/RIOT state,
SRAM pipelines and canonical EPROM pipelines are retained. Four independent
mutable SRAM M10Ks and the immutable ROMs' seven shared physical sites retain
98,304 RAM/ROM bits / eleven M10Ks, original latency and RAM OLD_DATA behavior.
All four synthesized firmware images match original assets in each variant;
raw MIF evidence is kept separately from the excluded Quartus databases.
All four device conditions pass, minimum +0.124 ns; no new cuts, unconstrained
paths or unreviewed warning categories. Fourteen damaged fit/source/state/
clock/constraint/memory/firmware controls reject. All 1,403 raw files pass hash
readback; extracted fit/XML/source regeneration and damaged controls pass.
Evidence: `fpga/mister/reports/2026-10-19-snapshot-baseboard-ff-scan/`.
Quartus session 31054 and final simulation 24065 are terminal.

Next: prepare the combined build with explicit schema/formatter versions,
longer bounded transport, global hold/fault/SYSTEM-reset ownership, and helper
namespaces. The compiled-helper review finds four conflicting definitions:
three intentional BaseBoard serial interfaces and ContA's older equivalent
first_bit width cast. Resolve those explicitly, then qualify actual combined
capacity/timing. Earlier isolated savings are not subtracted from the last
measured whole-core result of 45,228 / 41,910 ALMs. No production builder,
RBF, live FPGA or speed change is claimed. Local quarantine is insufficient
to freeze foreign consumers, backends, input and ARM services.

Classify/expose the 154,037 native-wrapper candidate bits (review-only),
top-level input/network/video and 16 MiB external raw RAM. Complete coherent
ARM/disk/input/server barriers that preserve connections/rebase timers, and
atomic bundles paired with the matching disk generation. Actual FPGA Cedar
save/continue/restore, original 60 ns clocks/timers, pinned comparisons, three
fresh boots/cold persistence and two-hour stress remain open. The last
qualified Native59 idle rate stays 1,947,654.12 issues/s. Goal ACTIVE. All
experiments in this entry are terminal; older running-job entries are historical.


## Serial transport ownership and compact format (2026-10-19)

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
