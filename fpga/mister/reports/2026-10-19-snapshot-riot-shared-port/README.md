# Shared normal/administrative RIOT RAM port

Complete physical-state simulation, fresh field classification, matched routed
timing and full BaseBoard restore simulation PASS. This changes only the
experimental RIOT adapter; the live FPGA is unchanged.

Normal and administrative accesses are mutually exclusive at state_hold.
The generator folds their RAM address/data/write-enable paths into one port,
retaining all 128x8 RAM bits, the exact original whole-array reset, read-before-
write behavior, normal read latency, timer/PA7/control ordering and the same
133-word snapshot ABI. It adds no state, valid mask or reconstructed storage.
The original vendor RTL and qualified source generators remain unchanged.

The unchanged original/independent model oracle passes 54,701 host edges,
4,448 held edges, 4,109 accepted operations, 2,593 writes, 27 timer underflows,
2,113 PA7 events, all 1,097 bits, all 256 indices, all RAM words and four
prescalers, destructive restore, first resumed edge and 20,000 further cycles.
Six deliberately broken variants reject: administrative alias, normal write
enable, reset enable, reset while held, running access and lost PA7 history.

Fresh XML of both qualified and shared-port adapters contains exactly the
same 1,097 physical bits plus 21 private transport bits, with no sequential
exclusions. Seven damaged inventories reject. Synthesis independently retains
all 1,118 registers in both cell hierarchies. Matched identical enabled-state
fixtures use 50MHz, the same launch/capture shell and unchanged SDC. All four
conditions pass, minimum shared-port margin +0.167ns, with no illegal clocks,
unconstrained ports/paths, added cuts or new warning category. Shell totals:
1,645 -> 782 ALMs, 1,350 -> 1,323 registers, zero block RAM in both. The private
cell retains 1,118 registers in both; shell register packing differs. Five bad
fit controls and two lost-synthesis-bit controls reject. These isolated
figures must not be multiplied into a claimed whole-core saving.

The complete original-versus-candidate BaseBoard fixture also passes:
526,012 host edges, 427,972 held, 402,862 requests, 33,334 late enable changes,
all 40,366 bits, all 131,072 indices, 4,905 words, 5,940 RIOT rises and 20,000
post-restore cycles. Its independent SRAM/read-pipeline and RIOT models remain
unchanged. No CPU/RIOT/clock state is planted. The deterministic SRAM image is
fixture-only; normal extra-SRAM writer counts remain zero, so ordinary SRAM
writes still rely on their earlier unchanged primitive/bank oracle evidence.
This run repeats the positive full-board gate; it does not claim another run
of the earlier eleven full-board broken variants.

The frozen original board text and 21 other prepared HDL assets are unchanged;
only M6532_snapshot.sv differs. The opt-in --shared-riot builder variant records
the new child manifest/hash while retaining every board state bit and clock,
memory/ECC and timing policy. Local full-core source audit passes all 413
baseline receipt files, 2,137 original board pins and fifteen damaged controls.
The new full-core experiment is running at:
`/home/alans/mister/Dorado_Enabled60SharedRIOT_20261019` on misterubuntu.
Whole-core capacity and timing remain unqualified; no RBF is assembled/deployed.

Raw simulation/classification/fit/full-board/prepared-source evidence is
losslessly archived. All 283 raw files pass SHA256 readback; generated objects
and Quartus databases are excluded. Initial remote fit invocation stopped on
missing Python imports before any Quartus stage; supplying the two imports
started the same untouched prepared project. Both matched fits are terminal. The independent source/state/timing audit also
passes after extraction from the archive.
No speed, timer migration, global pause or complete machine snapshot is claimed.
