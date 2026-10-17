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

Hardware test18:28UTC: authentic BFN113 request and download, no CRC/RX
errors. The FPGA still enters RequeueSearchSQ with a white raster/tiny
cursor. Read-only DDR now decodes all8960 germ source words correctly,
allowing the two original PilotBoot writes; a partial relocated copy is
visible at0xfdc00. This resolves the demonstrated source-transfer defect,
but does not qualify the relocated copy (dirty cache is invisible) or prove
an OS boot. Next compare the native DDR-adapter run and extend the original
storage run beyond the first Mesa instructions. No guest image patches or
host IOCB-completion bridge are used.
