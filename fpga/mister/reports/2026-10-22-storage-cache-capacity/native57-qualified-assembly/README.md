# Native16-line backing buffer: reviewed and assembled

Exact503-source regeneration from reviewed9e changes one native wrapper: permitted4->16 backing-buffer lines and initialized-only passive hit event. CPU, original cache/Map/ECC, physical SDRAM algorithm, clocks/constraints, disk adapter, Main and daemon stay unchanged.57.142857MHz/SYSPER4 still gives140ns nominal ordinary instructions before waits; the original60ns goal remains open.

All31 summaries and356 detailed categories/four conditions pass, minimum detailed setup+0.211ns/hold+0.114ns. All170 RAM rows/modes match, original architectural cache remains independent40 and BaseBoard ROMs retain the seven-block pool.332 M10Ks,35 DSPs,57107 registers. Expanded buffer has1536 data bits (raw12-bit cell payload, padding optimized),320 tag bits and16 canonical validity bits. The physical32-bit hit counter and24 counter/event/initialized paths pass; the old deployed zero event was not a hit-rate measurement.

Warnings differ only by the unchanged bounded FIFO-wrap expression truncating its promoted32-bit zero/increment to a4-bit index instead of2. Every other warning remains matched. No timing or logical-memory waiver.15 real source/timing/memory/counter/clock/evidence corruptions and9 assembly-guard corruptions reject; positives and original evidence unchanged.

Counter queryv1 used unsupported Quartus17 -regexp; v2 escaped bus-name filter matched nothing; exact-name-checked v3 passes. Both failures remain in the supplemental archive. First reviewer-control invocation exposed nondeterministic supplemental path-list order; sorted-corner output is reproducible. Both qualifications carry identical timing data after sorting; first failure retained. None required a fit restart or design change.

One guarded assembler3600312/3600339 completed0. RBF SHA25693ad59c3613dc60fc07d56872fff81e5d35e1d3208ea29f554c4b68373991265,4100208 bytes. It is not deployed or firmware/performance qualified. Full physical startup/original boot/input/files and pinned speed comparison remain required. Main337 comparison, accurate60ns phases, final three fresh seeds and two-hour stress remain open.

The embedded terminal tar is SHA25687e8d0b1ccfdc2983e9d632e020882da82cd96883b44d1783977de7f2782f433; all956 members were expanded and checked before review. The supplemental tar SHA256f4a353fe59800dff732a8824b6a2c0d73050d552dfe9e9426da6cc1d4ecac424 retains all three read-only counter attempts. Both archives contain reports/source only. Qualifications, guards, logs, exact tools/dependencies and freeze script are separately preserved.

Verify: tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-cache-capacity/native57-qualified-assembly. Reproduce review with tools/review_storage_cache_native.py and frozen recipe/counter paths, then the reviewer/assembler rejection drivers. No disk image, bitstream or ELF is committed.
