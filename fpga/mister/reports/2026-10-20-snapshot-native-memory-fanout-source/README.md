# Serial70 source: real memory pause with fanout256

Serial69's routed network_step drives 1,213 loads. Serial66's aggressive
limit64 and Serial67's global assignment worsened timing. This experiment uses
limit256 with the exact terminal Serial69 RTL, original seed, clocks, phase,
IO constraints and board-only schema. Only two targeted QSF assignments differ;
there is no new logical state or exception. This is source qualification only.

All 660 parent inputs are checked, including their rebased constraints. The
inverse source review retains Serial69's memory pause and upstream 2,761 pin
bindings. Six rehashed bad policy/source cases reject; the legacy Serial66
source reader still passes. Fresh actual-QSF XML retains all 2,094,301 required
bits and rejects fifteen geometry/task/clock/alias cases. Other service ACKs
remain virtual. New network-pause RTL in the working tree is NOT in this run.

The receipt records builder2602043 and fitter2604107 live as of its observation,
with all 660 inputs unchanged after synthesis. Build path:
/home/alans/mister/Dorado_Serial70MemoryFanout_20261020.
Packet SHA256:44f2fab19f72c114d57b3891abaf45ca44af98cd1e67a1d18f8cce038a115dd7.
Observe these same handles through terminal; timeout is not a restart reason.
Assembly is disabled. Actual assignment application, legal capacity and all
four timing conditions/304 crossings still require terminal review.

All 1,154 raw members (94,806,823 bytes; 9,408,728 stored) read back losslessly.
Extracted tools replay source review and fresh XML, and restaging reproduces
all 660 exact prepared hashes using immutable parent receipts and frozen HDL
dependencies. No working HDL, FPGA deployment or speed claim is used.

The same run is now terminal and rejected for setup timing. Its complete
review is in 2026-10-20-snapshot-native-memory-fanout-rejected; this directory
retains the earlier source-only receipt and its live-as-observed process data.
