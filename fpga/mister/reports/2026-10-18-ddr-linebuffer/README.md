# Optional raw DDR line buffer

Cedar loader on Native31(50MHz):20s coherent fabric-edge sample shows
16.9255% enabled /83.0745% globally paused, host50.0034MHz. This counts
machine edges, not retired instructions. Global backing-storage pauses are the immediate measured bottleneck.
Counters do not split DDR and HPS disk duration; merely raising the PLL
cannot remove these stalls.

The adapter previously discarded three of each read's four16-bit words.
Optional LINE_CACHE_BITS=7 retains128 complete64-bit lines (1KiB payload).
Hash mixes raw chip-group owner with word-line bits, using full address tags
so collisions only cause misses. Synchronous arrays keep lookup off the
request-accept path. Writes ALWAYS reach DDR with original byte enables;
acceptance invalidates the corresponding buffer slot. No write completion
is fabricated. Reset invalidates all slots; outstanding reads still drain,
and orphan responses cannot fill the new buffer. The original Dorado cache,
ECC, DRAM controls, serial bus and read-before-write model stay upstream.

Default is OFF: LINE_CACHE_BITS=0, generator --storage-line-cache-bits0.
Existing uncached DDR regression passes unchanged. The generator records
its explicit option in manifest.json. Cached Native32 differs from frozen
Native31 only in this adapter and one generated instance's parameters;
the freshly generated wrapper comparison asserts that before fitting.

Reproduce:

```
make -C verilog/verilator storage-ddram-test
make -C verilog storage-linecache-test storage-linecache-array-test storage-linecache-board-test
verilator --lint-only -Wall -Wno-PROCASSINIT -GLINE_CACHE_BITS=7 --top-module dorado_storage_ddram fpga/mister/dorado_storage_ddram.sv
```

PROCASSINIT is suppressed only for the repository's synthesizable FPGA
register power-up initialization convention; remaining strict lint passes
with the buffer both enabled and disabled.

Independent word scoreboard:458 transactions, variable busy/read latency,
zero-latency returns, blocked-DDR hits, all write lanes held under busy,
neighbor preservation, collisions/evictions, response backpressure, reset
invalidation and orphan drain. Eight DDR reads satisfy32 interleaved words
from eight groups. Disabling the cache fails32 vs8reads; removing write
invalidation fails with stale1234 instead of9000. Both controls are saved.

| Fixture | Chip cycles | DDR reads | Writes | Comparison |
| --- | --- | --- | --- | --- |
|3 groups, cached|1505|509|463|Every output and stored bit; reset drain|
|Same fixture, uncached Native31 gate|1505|4449|463|Reference in arbiter-tree report|
|Complete MSA14-bit|12012|2944|7872|1111 output changes,2,359,296 stored bits identical|
|Complete MSA16-bit|12012|2992|7872|1111 output changes,9,437,184 stored bits identical|

These are fixture bus-traffic reductions, not measured FPGA speedups.
Frozen Native32 candidate on misterubuntu fitted at50MHz/SYSPER4.
Quartus17 synthesis succeeds and retains128x64 data +128x21 tag RAMs.
Full29-check STA plus global fast/slow setup/hold/recovery/removal/pulse
checks pass (overall+0.084ns); no new cuts or unconstrained clocks. Loaded
on14 at04:19UTC, original firmware is loading. First enabled-edge sample
is25.0156%, against26.4617% uncached firmware: no gain there. Cedar/input
qualification and real loader/desktop benefit remain pending. See native32/.
