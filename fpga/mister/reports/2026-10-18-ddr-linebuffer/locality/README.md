# Backing-memory locality observation

This is a synthetic MSA fixture, not a Cedar workload or an FPGA speed test.
No functional RTL, physical DDR mapping or deployed core changed.

The existing `tb_storage_board` ran with `ADDR_BITS=16`,
`LINE_CACHE_BITS=7`, the same generated 64K MSA/reference boards and variable
latency DDR peer. A private testbench copy added only this observer:

```verilog
integer trace_fd;
initial trace_fd=$fopen("build/rtl-executive/storage-locality/raw.csv","w");
always @(posedge clk) if(rv && rr && !reset)
    $fdisplay(trace_fd,"%0t,%0d,%0d,%0d",$time,steps,rw,ra);
```

CSV fields are simulated time, machine step, write flag and raw word address.
Input hashes are in `inputs.json`. Regenerate the two MSA files with
`tools/sil_storage_ddram.py --chip-address-bits 16` before reproducing.
The normal fixture passed 12012 chip cycles, 1111 output changes and every
one of 9,437,184 stored bits. Its measured 2992 DDR reads match offline
replay of the 10880 accepted raw requests exactly.

Run `python3 replay.py` to check the saved results. Cache rules match the
128-line adapter: full tags, current hash, read allocation, write invalidation.
Three analysis-only alternatives use the same request order and capacities:

| Layout/policy | Read fills | DDR writes |
| --- | ---: | ---: |
| Current | 2992 | 7872 |
| Four groups per 64-bit word | 1500 | 7872 |
| Transposed row/column bits | 3008 | 7872 |
| Current layout, retain a matching write-hit line | 2992 | 7872 |

Writes dominate this fixture and remain unchanged. The replay does not model
changed service latency, write batching, DDR row efficiency, timing closure
or Cedar locality. Measure the real workload before implementing a new layout.
