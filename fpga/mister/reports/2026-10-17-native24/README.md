# Native24: passive receive-status monitor

Deployed on MiSTer14, SHA256
`1cdc35a2dca2c8bb5902e194d64d819cf2a8c84b0c188daf86cdf690ea2d0d17`.
Same functional RTL, clock ratio4 and2MB/16K geometry as Native23.
29 timing checks pass, minimum +0.122ns; no illegal/unconstrained clocks.
Exact same-cell MemX c21 hold-only exception is recorded in reviewed.sdc;
setup/read/data/enables remain checked. External framework I/O coverage
is a separate limitation.

**CPU monitor bytes are invalid on this deployed image.** input_eop was
wired to EthData_17 (parity). Correct physical EOP is EthData_18 from l07.
Ignore machine bits23:16 and7:0. Bits31:24 count FIFO EOP writes (mod256)
and bits15:8 retain the hardware status. Routing replies show status00;
initial BOL during startup showed20. This does not prove CPU receipt.
Generator corrected; separate Native24b fit pending. No functional guest
state is driven by either monitor.

Original firmware requested BFN113, C released, disk-loaded Cedar proceeds
to routing retries; screenshot shows maintenance cursor only, no login.
Quiet diagnostic server1d5e05e9... suppresses idle broadcasts after BFN113
so directed-reply status remains visible. Requested replies are unchanged.
ARM routing,31,376-byte BFN113,66,296-byte STP, quiet/reconnect tests pass.
Delay500 and nil-checksum diagnostics did not resolve Native23 retries.

Four64K-chip modules /8MB RAM /64K map software reference reaches Viewers
(166,685px), using the C IOCB bridge; not FPGA evidence. An advertised16K
map desktop is invalid proof because C still indexes64K and references381
high pages. Scratch larger-memory RTL has cell/full-MemX geometry gates
and complete MSA/DDR comparison; cold boot is still running.
