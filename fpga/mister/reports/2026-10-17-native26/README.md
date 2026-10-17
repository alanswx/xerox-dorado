# Native26 experimental memory geometry

Four64K-chip modules provide8MB physical RAM with64K Map entries.
Extra row/column address bits are wired from MSA MemAd.1 and MemX RTMapAd.1
through the explicit density straps; all21 Map planes select64K arrays.
Raw DDR grows to16MB. Native25 receiver setup correction is retained.

Parameter defaults preserve16K cells and byte-identical generated default
MemX/MSA boards. Independent cell, full MemX geometry and full MSA-vs-DDR
comparisons pass, including all9,437,184 stored bits. Default2,359,296-bit
MSA comparison passes too. Full generated wrapper elaborates.

Cold500M prototype remains active. Partial source-copy observation has8880
words with no mismatch and germ entry, below the existing8900 minimum; it
is not qualified. Review missing coverage, final IFU/image/error summaries,
routed timing and actual firmware startup before claiming FPGA behavior.
No Native26 image has been deployed and no desktop claim is made.

Fit/review now complete:29 timing checks pass, minimum+0.074ns, zero
illegal/unconstrained clocks. Exact own-address hold-only reviews and full
fast/slow violating paths are included. No FPGA deployment yet.
