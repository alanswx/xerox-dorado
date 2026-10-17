# Native24: passive receive-status monitor

Same functional RTL, clock ratio4 and2MB/16K geometry as Native23.
An explicit DskEth/backplane observation port replaces the machine-debug
word with four bytes: hardware EOP count, CPU-consumed EOP count, hardware
status, CPU-consumed status (MSB first). Counts wrap at256. CPU task/PC and
machine-status words remain. No controller/guest state is driven.

Generated DskEth differs from frozen canonical RTL only in module name,
observation port and passive monitor instance, verified by stripping those
additions and comparing bytes. Backplane diff contains only those explicit
port connections. First hierarchical taps were rejected by Quartus and were
replaced by explicit ports. Full generated native wrapper passes Verilator
elaboration; Quartus map with explicit ports passes and fitting is active.
Timing/clock review and actual hardware observation remain pending.

`DORADO_RX_MONITOR` changes the meaning of Main's `machine=` trace field.
Enable it through the native generator's `--receive-monitor`; build.sh reads
the manifest and sets the macro. Default generated boards remain unchanged.
