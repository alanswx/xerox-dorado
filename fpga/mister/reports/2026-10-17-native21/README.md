# Native21 module-selection fix

MemX j01 formerly sampled the module mask before same-phase j03 exposed
LoadSinO. With four storage modules, the previous module supplied the first
two source words; 45 sampled germ words differed and Mesa never fetched an
instruction. One local fabric stage at j01 clock pin13 lets that load become
visible. Shared Clk0, the f03/f04 clocks, MSA boards and guest images are unchanged.
This is a discrete-time clock adapter correction, not a changed chip truth table.

The captured module3-to0 regression drives original MemX/MSA serializers,
seeding only DRAM read pins with a germ line and old-module DEAD/BEEF sentinels.
It passes all16 words with the fix; the old RTL fails word0 (DEED vs06EC).
The canonical test also asserts the captured LoadSinO phase. Map-buffer hold
regression passes. Full cold100M with four modules and native BaseBoard clocks
passes the original-image and sampled-germ gates; see validation.json.

Quartus17.0.2 fits at84% ALMs. Complete pre-review hold reports and summary
are preserved. The three -0.023ns paths are ContA e04 address capture to its
own inferred RAM write-address bit1 registers. The cell captures only !p13
and commits only p13&&!we_d&&ce_lat, so same-edge transfer is impossible.
reviewed.sdc asserts exactly1 source and3 sinks and excludes only HOLD.
Full repeated STA/clock audit passes29 checks, minimum+0.097ns, zero illegal
or unconstrained internal clocks. Framework I/O delays remain separate.

RBF/source hashes and isolated build path are in validation.json.
Actual FPGA Cedar login, typed command and screenshot are still required.
