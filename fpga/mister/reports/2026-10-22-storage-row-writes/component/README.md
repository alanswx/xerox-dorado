# Same-row writes and native reset/init coordination

The default-off `PIPELINED_ROW_WRITES` controller issues consecutive single
writes to an unchanged rank/bank/row. An ACK is consumed only after the chip's
intervening capture edge. Recovery remains required before reads, row changes
and refresh. Every matching completed raw line is invalidated. There are no
new state bits. Original board capture, old-word/commit, cache/Map/ECC and the
machine-edge barrier remain unchanged. No new FPGA image has been loaded.

Protocol v5 passes six positives in Icarus and Verilator with identical
coverage; eight actual broken variants reject in both engines. Each positive
checks 1,483 write ACKs against actual command capture and stored data, including
held replies, row/rank/bank changes, mixed reads/writes, neighboring cached
lines, refresh and reset at three write positions. At57.1MHz,1,314 successors
stream and the longest uninterrupted command streak is340. The1,024-write
stream takes1,079 host ticks versus3,204 without the feature. At66.7MHz the
enabled case streams1,319 successors. These are component workloads, not CPU
throughput or a fitted IO timing result.

Independent XSDS pin v4 passes off/on at57.1/66.7MHz with conservative-7
timings. Every consumed write ACK checks the physical chip's stored value and
physical write count. The finite fixture's initial patterns are seeded before
controller startup; every subsequent write uses real command/DQ pins. Early
ACK and wrong DQ mutations fail. Queued physical-pin testing also passes at
depths4/3, preserving all accepted writes across five resets; early backend
reset rejects. Streamed depth4 admits/issues/retires205 descriptors,114 writes,
192 frontend completions and13 intentionally discarded old replies.

Original144-chip MSA outputs and all9,437,184 backing bits agree over12,012
architectural cycles,750 reads,7,872 writes and1,111 output transitions.
The actual global raw operation order also agrees. Queued reuse control takes
101,328 host ticks; queued writes take92,813,8.4034% fewer. Wrong physical DQ
fails the complete backing-array oracle. Native128 synthetic transport takes
8,491 ->7,370 ticks,13.2022% fewer, with1,920 requests/640 writes and identical
ordered admission/completion data and final physical values. Raw-response
wait falls4,930 ->3,763 ticks. Wrong owner rejects. Separate runs of the SAME
binaries reproduce all four control/candidate trace hashes byte-identically.
Neither workload is actual Cedar firmware traffic or a hardware speed claim.

`prepare_storage_queue_native.py` transforms the exact qualified5c65 wrapper:
global queue reset, initialization gating of NEW admissions, queue-drain mux
priority, backend reset after accepted work, and selftest held reset during
drain. Its real storage fragment, actual selftest/queue/backend and independent
SDRAM pins pass with all128 owners and bounded3-bit local/10-bit raw addresses.
Three full selftest epochs leave all1,024 words zero;648 admitted/issued/retired
descriptors produce640 frontend completions. Eight old replies are discarded
after reset while the accepted writes reach the chips. Queue occupancy reaches
four; reset covers held responses, reads in flight and selftest in flight.
Disconnected drain, premature backend reset and unheld selftest mutations all
reject. Full16-bit local/23-bit raw initialization, complete native elaboration,
fitted timing and firmware remain separate gates.

Failed test development is retained. Refresh moved the future-wait reset probe;
it now waits for a natural refresh-safe window. A duplicate-ACK mutation exposed
missing detection of an orphan VALID while READY was low. The first write reset
probe asserted reset before cold-row activation; it now targets the actual
command-emitting edge and catches the same-edge reset-guard mutation. Native
fixture v1 called a nonexistent model task; v2 checked the intended reset wire
instead of the physical backend input. The final fixture checks that actual
input. None of these corrections required a DUT repair.

Actual native firmware attribution is still open. The separate
`native-cedar-5c65-traffic-v1-20261022` diagnostic uses the exact qualified160
compiled HDL sources and original EPROM bootstrap, with passive wait/fanout/
latency counters. Its simulation is not included as a terminal result here.
Do not restart it on an observation timeout. No Quartus candidate, assembly,
hardware speedup or goal completion is established by this receipt.

Next collect real firmware traffic, compile the full candidate, review all
four timing corners and measure on MiSTer14. The60ns CPU phases/timers,
explicit5c65 Rollback/cold persistence, three fresh boots and two-hour stress
remain open. Preserve5c65/Native59 and existing disk volumes; goal stays active.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-row-writes/component`.
