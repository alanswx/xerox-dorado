# Native57 complete MemX normal-operation integration

Frozen Native56b plus the qualified complete modeled MemX adapter and one
new MosRam helper. Ten existing shared helpers are reused byte-identically.
The outer clock/wrapper, SDCs, memory/network policies and actual MemD input
delay of two are unchanged. All eight board adapters' administrative inputs
remain inactive. This is normal-operation qualification, not FPGA save/load.

The terminal build and independent read-only audit pass 31 summary checks,
164 detailed path categories, four operating conditions, and 32 board/corner
crossing-coverage checks. Minimum slack is +0.116 ns. No added exceptions,
illegal clocks or unconstrained clocks; existing MiSTer framework I/O coverage
is unchanged. SDRAM and all eight boards' crossing categories are timed.

Resources: 30,734 ALMs, 52,945 registers, 332 M10Ks and 35 DSPs. These counts
have administrative inputs inactive and do not estimate enabled snapshot cost.

Both baseline and candidate retain all 21 independent 65,536x1 MemX RAM planes:
1,376,256 bits in 168 unique M10Ks, eight per plane, zero MLABs, OLD_DATA mixed
port behavior and exact physical implementation geometry. All eight actual
baseline/candidate MemX PROM files hash-match the qualified manifest. Inherited
MemD cache, IFUM/IM and processor/MemC memory geometry/behavior also pass.
Eighteen corrupt-project/timing/memory/placement/PROM controls reject.

The exact executing v1 builder is preserved. Its report-name substitution
left the parent's Native56 labels on the corner script; crossing reports
are labelled Native57. The final audit requires the exact unchanged corner
script and all expected categories under those names. Future builder code
corrects report labels and adds PROM prebuild guards. The actual build's
PROMs are explicitly checked by the final audit. Neither correction changes
this fit's RTL/SDC bytes or reruns timing against a live fitted database.

RBF SHA256:
064e37848a123cbc0affeeb07ff309d242aff8b8deca9c0992c78e8ee56bc268

Raw reports, logs and sources are gzip-compressed unchanged. Compiled source
hashes and executed driver/auditor are retained. Hardware deployment and
original-firmware/Cedar/input qualification follow separately. No speed gain,
complete machine snapshot or original 60 ns clock qualification is claimed.
