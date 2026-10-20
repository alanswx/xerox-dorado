# Complete nineteen-board chip-width core: capacity passes, timing rejects

**41,381 / 41,910 ALMs**,529 spare. Fit succeeds and routes all nineteen enabled
boards, saving 1,068 ALMs from Serial63 and 5,321 from Serial62Area. Actual fit:
52,561 registers, 329/553 RAM blocks, 2,498,471 block-memory bits, 35 DSPs, 3 PLLs and
seven virtual capacity pins. Assembly is disabled; no RBF/SOF/hardware change.
This is a legal capacity result, not a usable or timing-qualified release.

Four-condition review finds slow 100C setup -0.239 ns and hold -0.088 ns; slow -40C
setup -0.150 ns and hold -0.155 ns. Fast conditions pass. Setup failures reach falling
network_step from memory readiness and scan fault/busy flags. Real hold is
u_media.cyl[6] -> cached_cyl[6]; virtual service/reset inputs fail at slow -40C.
All crossing reports are retained. SDRAM input/output timing passes. The exact
Native59 clock definitions and framework IO exception names/comments/counts,
including BOTH setup and hold port tables, match. No illegal/unconstrained
clocks. The inherited 3 input/50 output asynchronous/framework IO exceptions are
explicitly listed; not every physical IO is timed. No clock periods are relaxed.

Source/pin review checks all 659 staged parent inputs, 413 Native59 receipts,
2,761 original pins/parameters, qualified chip-width/transport assets and unchanged
clock/IO/area policy/constraints. Twenty-three damaged source/policy/alias cases
reject. Quartus leaves all 659 inputs byte-identical. Actual QSF XML selects 319 HDL,
15 macros, 3,945 instances: 1,957,048 physical board +4,578 private board +69 private
owner/router +132,484 wrapper ACCESS PENDING =2,094,179 sequential bits. No
sequential exclusions. Fourteen state/geometry/parameter/exact-alias cases reject.
Top emu/UIO, raw RAM and ARM state remain outside this inventory. Four damaged
capacity/process reports reject. Timing reader mutations are tested on the
KNOWN PASSING Native59 baseline; actual negative slacks are retained as rejection
evidence. Six reader cases reject, including a changed second port table.

The source tree still uses virtual service contracts and a PLL-unlock reset
proxy; full native barriers, genuine reseed and full snapshots are unfinished.
Schema af0b647cb43a3a62 is BOARD ONLY. Raw 16-bit data/ECC interface, 8 MiB guest /
16 MiB padded backing, all original fields and clock cadence remain. Only unused
implementation cache padding changes. Component and eight-board SPI qualification
are separate committed receipts; no C-state import or host instruction completion.

The run is terminal: builder 2433951 and fitter 2436350 are absent. Project
/home/alans/mister/Dorado_Serial64Chipwidth_20261019 is retained. Packet SHA256:
a1e8e12bc900ebc46381331e13d148ec416a7c8b9353fab5aa36f392b675d8cc.
Do not restart it. Driver completes all timing/crossing reports without assembly;
Quartus returning success does not mean timing passes.

All 2,761 raw members pass lossless readback. Extracted tools reproduce all 659
staged bytes after path rebase and replay source/pin/alias and complete native
inventory controls. Capacity review/controls and timing-rejection review plus
passing Native59 baseline controls replay. Actual Native59 baseline compressed
bytes are included as raw receipts (outer gzip is only archive storage). The
Python dependency closure/original cells and immutable parent/component archives
are recorded through the reports symlink; no working HDL. All raw bytes except
small JSON use lossless gzip; objects/databases/executables are excluded.

Next investigate removing redundant snapshot-hold propagation from the CPU
memory-ready predicate, retaining storage initialization and global hold. Prove
actual MSA/owner/quarantine equivalence before adoption; frontend readiness is
not complete arbiter/SDRAM drain. Prove disk cylinder/cache capture exclusivity
before any hold exception. Replace virtual service/reset ports with real
acknowledged synchronization/barriers. No new exception is applied here.
Complete snapshots, original 60 ns clocks, three boots/persistence/pinned speed
and two-hour hardware stress remain required. No speed improvement is claimed.
