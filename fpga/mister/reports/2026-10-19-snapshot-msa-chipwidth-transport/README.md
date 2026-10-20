# Eight chip-width MSA adapters through SPI

Board-only schema **af0b647cb43a3a62**, 1,957,048 physical board bits. Original
first eleven maps stay. Slot11 has eight 1538-bit members, 86 words each, last 8 bits;
unit stride 256 and dense payload 1538 bytes for all eight. Both owner/router
reject 86..255 before dispatch. Prior main-only, original MSA and compact-MSA
schemas reject before state changes. Deadline 4096 and five service domains stay.

Four-state SPI test passes 75,935 ticks /326 packets /129 checks, including 34 actual
Python-formatted packets through all eight real chip-width adapters. It checks
unique member data, first/last words, new holes/high bits, schema rejection,
competing requests through completion, unknown/missing boundaries, timeout,
active parser reset, sticky quarantine and inherited Ethernet behavior. Ten bad
actual RTL cases and ten rehashed catalog mutations reject. The dense integer
oracle checks all 688 words/12304 bits, with no per-unit padding. Status/capability
padding and immutable packet-map checks pass. Original guest behavior and full
restore/resume are qualified separately by the complete-MSA receipt.

Fresh XML covers 12304 physical MSA +648 private MSA +417 private owner/UIO/router
bits, no sequential exclusions. Twelve geometry/schema/deadline/exact-alias
controls reject. Six inverse source controls check exact qualified owner/UIO/
router bytes except explicit word-bound/module edits, plus the chip-width board
source proof. Foreign services and the other eleven endpoints remain stubs;
guest advances are stopped. No native machine pause or complete FPGA snapshot.

All 380 raw members pass readback. Extracted catalog/fixture/vector/RTL generation
is byte-identical; format/source/XML controls and rebuilt actual eight-board SPI
replay pass. The dependency closure/original cells and immutable original,
compact and chip-width component receipts are recorded; no working HDL is used.
Objects/databases/executables are excluded; sources/reports are lossless gzip,
large JSON gzip. No full-core fit, hardware reload or measured speed claim.
