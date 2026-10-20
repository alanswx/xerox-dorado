# Actual MSA, actual arbiter and physical SDRAM pause circuit

Connect the complete 1538-bit MSA and late-freeze coordinator to the real
16-group interleaved/prefetch arbiter and 50MHz SDRAM backend. Both actual idle
outputs drive the coordinator. Independent XSDS chip models supply physical
memory; no host code completes raw requests, guest instructions or IOCBs.
The original frozen MSA observes the same peer for full physical/pin/traffic
comparison during ordinary operation and drainage.

PASS: 2,011,195 checks, 605,280 host edges, 4,477 guest advances, 5,456 reads,
992 writes, four pauses starting with two accepted reads/two accepted writes,
all 86 held state words read without changes, and 3,290 physical refresh
commands. The pin models have zero errors/warnings. FF state holds while raw
work drains; frontend hold stays latched through serial rotation; guest hold
persists through thaw. Normal traffic resumes against the original reference.

Four compiled broken variants reject: early frontend freeze, stopped refresh
(independent chip tRAS(max) failure), nonsticky frontend hold and early guest
release. Earlier header-parser/expected-pin-diagnostic harness attempts remain
preserved, alongside the pre-controls positive run. All 159 members read back
losslessly; exact executed source hashes and a rebuilt extracted complete
circuit pass without working HDL. Compiled objects/executables are excluded.

One MSA/16 raw groups only. All-eight-MSA/128-group native wiring, full native
state/ARM/disk-generation barriers, raw SDRAM bytes/controller save/restore,
physical fit/timing, complete FPGA snapshot and measured speed are still open.
