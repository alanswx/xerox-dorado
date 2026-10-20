# Eight compact MSA adapters through SPI: qualified component transport

New board-only schema **56e997e5c1fb54d0**, 1,957,944 physical board bits.
The first eleven maps remain unchanged. Slot11 uses eight members, 1,650 bits /
92 words each, last word 12 bits; index[10:8] selects the unit and index[7:0]
selects the word. Both owner and router reject words 92..255 before dispatch.
Stride 256 does not add padding to the dense eight-member payload: 13,200 bits /
1,650 bytes. The integer packing oracle checks every mapped word independently.
Old main-only and old MSA schema tags are rejected before state changes.

The SPI test passes 78,859 ticks /293 packets /113 checks, including 34 Python-built
packets driven through all eight actual complete compact-MSA adapters. It covers
unique values in every unit, first/last words, new holes, high bits, old schemas,
boundary loss/unknowns, competing valid indices through completion, timeout,
active parser reset, sticky quarantine and existing Ethernet. Ten bad actual RTL
variants reject. Ten damaged catalogs, rehashed before review, reject; status
capability/padding and immutable packet-map tests pass. Foreign service contracts
and the other eleven endpoints are stubs, and the guest is stopped here. Original
normal-state/pins/raw traffic and restore/resume have separate complete-MSA proof.
No native pause or complete FPGA snapshot is claimed.

Fresh XML retains all 13,200 physical MSA bits, 648 private MSA bits and 417 private
transport bits. No sequential exclusions. Twelve geometry/parameter/exact-alias
controls reject. The 4,096-cycle response bound and five service domains remain.
Independent source review inverts only the compact word limit/module edits back
to qualified owner/UIO/router bytes, then checks the compact-board source proof.
Six damaged source controls reject. Reset, unknown-child and ownership behavior
are not weakened. The current PLL-unlock reset proxy still needs genuine machine
reseed ownership before production use.

History: simulation-v1 left the outer owner's word limit at150 while the member
router used 92. Hole92 consequently returned child error 3 instead of invalid index 2.
Simulation-v2 fixes the generated owner limit; the old_bounds mutant reproduces
that failure. The first XML runner missed an imported utility before producing
XML; classification-v2 is the authoritative fresh inventory. Raw histories and
executed inputs are retained. The shortened-timeout control uses 1,000 cycles,
below an actual 1,650-edge rotation; 2,048 is sufficient for this smaller component
and would no longer be a valid negative control. The accepted budget stays 4,096.

All 414raw members pass lossless readback. Extracted catalog/fixture/vectors and
board/helper/router/owner generation is byte-identical. Dense-format/source/XML
controls and rebuilt eight-board SPI test pass. The tools dependency closure and
original cell bytes are included; replay additionally reads committed immutable
main-board/MSA receipts and compact component archive through its recorded
reports symlink. No working HDL is consumed. Generated objects/databases and
executables are excluded; sources/reports are lossless gzip, large JSON gzip.

Next is actual nineteen-board native integration and a new complete-core fit.
The component saving does not prove capacity. No new Quartus fit, RBF, deployment,
complete snapshot or measured speed gain is claimed by this transport milestone.
