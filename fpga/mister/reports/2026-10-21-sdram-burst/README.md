# Raw SDRAM BL8 component evidence

Component qualification only. No new image has been assembled or loaded, and
no native instruction-speed improvement or Cedar boot is claimed here.

`manifest.json.gz` indexes 765 losslessly preserved members, including frozen
test inputs, terminal logs/process return codes, runtime bad controls, raw
request traces and the exact 499-file native stage. Unchanged blobs may refer
to the earlier immutable front-observation receipt. Verify every member with:

```
python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-sdram-burst
```

The passing V3 pin suite contains seven positive cases and six compiled,
runtime-rejected controls: stale write data, orphan refill after reset, early
capture, wrong burst mode, short tRCD and the exact original model hold glitch.
Both slower supported clock rates and a 6ns physical-clock delay are covered.
Every positive checks 1,197 operations (636 reads and 561 writes). Existing BL1
frequency/delay positives and its three timing-error controls also pass.

The full native MSA fixture matches 12,012 logical board cycles, 1,111 serial/
ECC output transitions and all 9,437,184 storage bits against the original
144-chip board. Dropping host timestamps, the 12,380 raw request/final-memory
records are identical for BL1 and BL8. Reads fall 3,008 -> 750; physical writes
remain 7,872. Host cycles fall 122,779 -> 113,841 (7.28% fewer). Unrelated cold
reads cost a complete burst; native workload benefit remains unmeasured.

Rejected V1 exposed an independent chip-model defect, diagnosed with the
preserved passive pin transcript. V1 and V2 controller bytes are identical;
the corrected natural-burst retirement preserves the previous beat's hold
time. The hold checker rejects an exact restoration of the original model.
V2 then exposed insufficient refresh headroom at 20MHz. V3 reserves 6us and
passes. The model correction is simulation-only and cannot explain FPGA812.
The earlier V38/V39 native boot processes retain their frozen model bytes.

The native stage derives from qualified MC1668. After project relocation,
only its raw-backend wrapper selection, one QSF source append and the new
helper differ. Actual synthesis reports both helper and wrapper used in the
netlist. The archived live observation records the same builder3523028 and
all499 source hashes checked on misterubuntu. That observation is a prefix,
not a terminal timing result. Full-device timing, physical memory, firmware,
input and hardware issue/hold measurements remain required before selection.

The collector rechecks input hashes, the paired raw trace and positive log
claims. Its source is itself archived. An incomplete first collection (missing
optional launch metadata) was kept privately; it did not change any simulation
or fit. The completed manifest above is the authoritative receipt.
