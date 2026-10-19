# Native53 full-core processor integration

Frozen52 plus ProcH/ProcL exact-advance adapters; eight existing adapter files
reused byte-identically. All snapshot admin inputs inactive. The outer wrapper,
50MHz/SYSPER4 clocks, memory policies and four SDC files remain unchanged.
No full machine snapshot, enabled snapshot resource budget or speed gain claim.

31 summary checks and100 detailed setup/hold/recovery/removal/pulse/SDRAM/four
board crossing categories pass across all four conditions, min+0.115ns. No new
cuts or illegal/unconstrained clocks; unchanged framework I/O remains3 input
ports/13 paths and50 output ports/154 paths. Physical processor RAM retains
12 MLAB instances/9216bits/288cells; eight instruction1024x18 OLD_DATA banks.
30232ALMs/52845registers/330M10Ks/35DSP. Only extra synthesis warning10036
unused object; fit warning counts unchanged. Six damaged-evidence controls fail.

Qualification initially omitted explicit inherited memory-policy fields; final
read-only audit adds checked52 policy metadata. All compiled inputs and reports
are identical. The initial JSON/log and final executed audit are retained.
Hardware deployment/original boot is not proved by this timing report.

Raw reports/logs/sources are gzip-compressed without content changes.
