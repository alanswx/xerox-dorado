# Complete ProcH/ProcL serial FF state access

Both complete processor boards pass original/independent restore simulation,
fresh state classification and matched isolated area/timing. No full-core area
saving, physical snapshot, firmware boot or speed change is claimed.

| Board | Physical state bits | FF bits/packages | Compact FF words | Parallel/serial ALMs | Minimum serial slack |
| --- | ---: | ---: | ---: | ---: | ---: |
| ProcH | 5,712 | 502 / 64 | 28 (last16bits) | 1,950 / 1,735 | +0.137 ns |
| ProcL | 5,581 | 519 / 69 | 29 (last15bits) | 1,946 / 1,750 | +0.096 ns |

The existing FF helpers form a one-bit chain; no shadow bank is added. Reads
return only after the full rotation restores the bank's orientation. Writes
replace only their selected compact word. All small-file RAM/window, 256-word
RAM and phase addresses remain unchanged. Their qualified helpers and ordinary
logic remain intact. Both fitted sides use the same asynchronous F10145 helper.
The inherited per-package FF word is renamed legacy_parallel_word; no ff.word
access alias is advertised. state_access_schema=compact-ff-v1 and scan.word_map
provide the new exact addresses, widths and physical offsets. These layouts
are incompatible with earlier checkpoints and need versioned transport/loaders.

The full processor oracle is preserved, including raw hierarchical probes,
all-bit writes, all65,536indices, padding/invalid access, destructive continuation
and restore, first resumed edge and20,000further cycles. Only administrative
word mapping, bounded completion and the separately qualified ContA fault/reset
test sequences are derived. Both original C++ programs and all driver inputs
are retained. Changing requests/payloads during a scan cannot steal ownership
or write RAM/window state. Reset-time access is rejected; a fresh freeze entry
is required after reset. Boundary loss during a partial rotation locks every
local FF, RAM/window helper and phase register until SYSTEM reset, even with
attempted guest advances and further requests. Foreign consumers still require
the global barrier. A transport-only reset cannot recover partially shifted
state.

ProcH simulation passes1,013,101host edges/973,213holds/94,579requests,
1,701state words and75outputs. ProcL passes1,072,508edges/1,032,580holds/
93,955requests,1,668words and96outputs. Both have13,334late-enable changes.
All14/12ordinary RAM writers are exercised. Two ProcL writers fire only once
in this fixture; wider hardware workload qualification remains separate.
Each board rejects six damaged variants with specific expected diagnostics:
lost chain bit, missing FF shift, early completion, stolen owner, lost local
quarantine and reset-time access.

Fresh candidate XML is checked against each qualified original physical
schema. ProcH accounts for5,712physical+233private transport bits; ProcL for
5,581physical+209private bits. No sequential exclusions are introduced. Exact
compact offsets, every setup/edge-history bit, RAM/window, phase and unchanged
immutable-PROM metadata are covered. Nine damaged inventories/layouts reject
per board, including reintroduced legacy FF address aliases. Original cells
compile from frozen Native59; the MC10170 comment delta is checked token-
identical. Both immutable PROM assets per board remain byte-exact.

The matched fits keep the qualified50MHz/positive-edge launch/capture shell,
20ns embedded budget, virtual IO delays, SDC and all four operating conditions.
All502/519physical FF bits remain in their64/69synthesized helpers. ProcH has
22placed MLAB rows:512logical small-file RAM bits plus4,608large-RAM bits,
5,536replicated physical bits total. ProcL has18rows:384small-file+4,608large-
RAM bits,5,296replicated bits. Every administrative small-file copy retains all
64bits; ordinary widths match exact connected board pins. Depths, modes,
read-port latency, legal locations and immutable PROM bytes are checked.
Read inputs/outputs remain unregistered. No new warning category, timing cut
or unconstrained clock/IO path is accepted. Shell registers increase from
1,192to1,302 on ProcH and1,255to1,312 on ProcL.

Independent source/policy/physical-retention/timing audits pass. Each rejects
six damaged source, constraint, macro, FF-retention, hold and RAM-latency cases
at the intended check. Extracting the archive and rerunning both fit and state
classification audits passes again. Jobs SnapshotProcHFFScan_20261019 and
SnapshotProcLFFScan_20261019 on misterubuntu are terminal; local sessions61476
and38118 are closed. No production builder or live FPGA changed.

All966raw files pass lossless SHA256 readback. HDL/C++/headers/PROMs/reports/
logs and JSON over100KiB are gzip-compressed without byte changes; large
manifests are gzip too. Generated objects/databases are excluded. Additional
extracted-audit JSON files are verification metadata.

The actual full-core result remains45,228ALMs /41,910available,3,318over
capacity. Do not subtract or multiply these isolated savings into a claimed
whole-core result. Next: additional board adapters and a combined capacity/
timing fit; versioned compact schemas; an adequate bounded timeout (the current
fabric allows16edges, versus502/519for these scans); global hold/fault/SYSTEM-
reset ownership; complete backend/input/ARM/server/disk-paired snapshot state.
Original60ns clocks, fresh hardware Cedar restore and final release gates
remain open.
