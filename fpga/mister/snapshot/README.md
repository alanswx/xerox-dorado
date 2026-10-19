# Complete FPGA snapshots: board state access experiments

Complete machine save/restore is required milestone4a in
[the delivery plan](../../../docs/verilog-clock-plan.md). It is not implemented
in the running MiSTer core. This directory now qualifies bounded RAM access
and complete modeled ContB state restore in isolated RTL/routed experiments.
Machine-wide state access, coherent barriers and physical restore remain open.

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
