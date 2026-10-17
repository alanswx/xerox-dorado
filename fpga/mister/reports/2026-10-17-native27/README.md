# Native27 passive disk-fault observation

Native25h stops with pack_fault after writing Basic.Loadees and RPCRuntime
byte-exact; UserProfile remains incomplete after1024 bytes. Native26 is an
independent8MB/64K hardware control. Native27 keeps its functional RTL and
adds first-fault observation. No errors are ignored and no completion is
invented. Build path: misterubuntu:/home/alans/mister/Dorado_Native27_20261017.
Fit/timing review remains required before deployment.

When pack_fault is set, existing UIO machine/processor debug words switch
to fault context/extra. When Run' is set (processor stopped), machine-debug shows BaseBoard
observation; otherwise it shows receive counters. Processor-debug remains
CPU observation until pack_fault selects its captured extra word.
Context: reason[31:28], cylinder[27:16], head[15:10], block[9:8],
write-state[7:6], bit-count[5:0]. Extra is failed CRC residue, requested LBA,
or captured word-count/shift state (reason-dependent).

| Reason | Existing fault condition | Extra |
| --- | --- | --- |
| 1 | write gate ended inside record | word count/shift |
| 2 | invalid differential pair inside record | word count/shift |
| 3 | invalid geometry or read-only write | read-only bit |
| 4 | nonzero Fire-code residue | residue |
| 5 | detach/remount with dirty/in-flight transfer | requested LBA |
| 6 | read-only dirty flush | cached LBA |
| 7 | seek interrupts dirty serial transfer | cached LBA |
| 8 | host transfer timeout | requested LBA |

`make -C verilog pack-fault-test` passes normal changed-data writeback,
intentional bad-CRC first-fault capture and intentional timeout capture.
Full native64 wrapper lint passes. This is diagnostic qualification, not
a fix for the hardware fault.

Initial fit rejected prior-image post-fit SDC cardinality assertions before
placement. Refit uses native26-before-review.sdc. A subsequent in-progress
fit was stopped before any RBF to add stopped-CPU BaseBoard visibility;
map/fit restarted from the final diagnostic source. The active FPGA26 was
untouched by these build retries.
