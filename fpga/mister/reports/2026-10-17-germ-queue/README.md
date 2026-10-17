# Germ post-entry map-fault reproducer

The diagnostic four-original-storage-module run reproduces the FPGA's
RequeueSearchSQ loop around168M samples. It restores exact-model RTL POD
state at85M and selects Native21 j01 delay1; this is NOT a fresh cold boot.
Processor/memory state matched the cold fixed100M run at every17 sampled
progress points. Full cold native DDR-adapter run remains in progress.

A fingerprint-checked read-only snapshot at188514528 samples and the
normal driver snapshot at199999900 both show the queue context below.
A cache-aware physical cursor viewer reads812 / germInLoad. The saved
FaultParam0/1 identify VA0x3e056e; FltEmuPC013f maps to SDBM1 in matched
Cedar.mb!6 (byte store). FaultVal=0003, FaultMapVal=0003, FaultInfo=4080.
WDC=1, CurrentPSB=Process=0400, IndexMask=0ffc, RTemp0/1/2=0.
The fault address is inside the germ, not beyond the16K-entry map.
BasicCedarDorado.boot!22 has1047 data pages, maximum virtual page06b8;
its full1060-page header/trailer layout parses exactly. Do not infer that
this boot file requires a larger map from the final fault loop.

The final C++ host stop precedes the SV reporting coroutine completing.
check_cedar_rtl_germ.py therefore reports missing final IFU/Initial summaries,
not zero actual instruction execution. Do NOT treat this diagnostic as a
full cold qualification gate. The original cold100M Native21 gate remains
PASS. File descriptors/coroutines/non-POD BaseBoard internals are not in
these POD snapshots. No C-emulator state is imported.

First-queue watcher now replays85M, retains two rolling checkpoints from
160M, traces from166M and stops after10000 task0 samples within the four
queue PCs. It observes only; no guest repair or forced execution is used.
Read the first bad map operation/store, rather than repairing the queue.

Cold DDR-adapter run subsequently completed at200003764 samples. The standard
checker passes all source-copy, Initial and Cedar image gates, with1812418
IFU fetches and1813208 acknowledgements. Same post-entry queue loop around168M.
See cold-ddr-gate.json. First-germ-map-fault.txt records the software map writes
vacating VP3e02..3e05 before FaultInfo changes. Correct lower/upper bounds
were read from GF09d0 globals; comparison/jump diagnosis remains open.
