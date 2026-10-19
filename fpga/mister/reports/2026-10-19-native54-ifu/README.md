# Native54 full-core IFU integration qualification

Frozen Native53 plus the complete IFU exact-advance adapter, its MC10135
clone and width-correct F10415 banks. Five existing FF adapter files are
reused byte-identically. Clocks, outer wrapper, constraints, memory policies
and all other compiled source bytes are unchanged. All snapshot administration
inputs remain inactive; this is normal-operation integration.

All 31 timing-summary checks and 116 detailed timing categories pass across
four operating conditions, minimum +0.114 ns. Detailed reports include both
directions for ContA, ContB, ProcH, ProcL and IFU, plus device and SDRAM paths.
No new timing cuts or illegal/unconstrained clocks. MiSTer framework I/O
coverage remains unchanged: three input ports/13 paths and 50 output ports/
154 paths. Source/QSF, timing, processor RAM and IFU OLD_DATA fault controls
reject corrupted evidence.

All eleven IFUM groups retain their qualified widths, depth and OLD_DATA
macro/physical read-during-write behavior. The twelve processor MLABs retain
9,216 bits/288 cells; eight instruction banks retain OLD_DATA. Full-core
resources are 30,342 ALMs, 53,052 registers, 330 M10Ks and 35 DSPs. Administration
is disabled and pruned, so these are not enabled-snapshot resource estimates.

RBF SHA256: 80d67e1212b944cdb235040f26b5d794a7c4451d8302b24888c342caf16bdb77.
Build and all reporting jobs ended before the read-only final audit. Exact
compiled inputs, project/constraints, reports, logs and executed scripts are
retained; raw text is gzip-compressed without normalization. Physical boot
qualification is separate. No speed gain, full-machine snapshot or original
60 ns clock migration is claimed.
