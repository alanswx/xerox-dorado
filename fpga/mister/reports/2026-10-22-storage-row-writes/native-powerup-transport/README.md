# Full-width native power-up transport: data agree, firmware still pending

Both original5c65 and the queued/reuse/write candidate complete the actual
23-bit raw-memory selftest against independent SDRAM pins. Each native wrapper
has128 groups/16 local address bits/23 raw address bits. The six passes distinguish
all8,388,608 raw locations and leave zeros before guest release. This is model
qualification, not an exhaustive physical-module or fitted IO result.

The first8,191 COMPLETED original-native requests have identical global
address/operation/data order and correct reads, across all128 owners. The
8,192nd request is retained without its reply because the original bounded
observer stopped printing there; it is not counted as completed. All captured
requests are reads at CIAfff before any instruction issues or Cedar113 request.
This prefix is actual native power-up traffic, not a Cedar workload.

| Metric, same completed prefix |5c65 control|queued/reuse/write candidate|
|---|---:|---:|
|Elapsed host ticks|109,062|102,908|
|Consumed raw-service ticks|26,995|27,039|
|Unowned raw ticks|82,066|75,868|
|Same-edge raw handoffs|5,999|7,050|
|Positive inter-request gaps|2,191|1,140|

Candidate uses6,154 /5.6427% fewer elapsed ticks. Raw-service latency is
essentially unchanged; the improvement is in gaps between owned operations.
Unowned time includes guest/fabric execution and demand gaps, so it is not
exclusively arbitration overhead. This is not evidence of5.64% faster Cedar.
The write feature is not exercised in this read-only prefix.

The reader rechecks every frozen input hash, parses only complete log records,
checks ordered ownership/payload/latency, retains the cap's unpaired request
and refuses to call an empty comparison a pass. Five malformed-trace controls
reject; changed read results and write payloads disagree; irrelevant write
response data is ignored. A small independent example checks the service/gap
budget. Earlier control results and the final executable reader source are
frozen here.

Both existing simulation PIDs were verified live at collection. The full
compiled-input receipt is linked by its exact earlier manifest. These are
running log prefixes, not terminal boot results. Observe85205/control and
85896/candidate plus drivers84869/85545; expired tool handles do not imply
stopped jobs. Do not restart either on an observation timeout.

Important remaining capture gap: the8,192-request cap filled BEFORE the CPU
ran. A later original-firmware/113 capture window and coherent wait categories
are still required before selecting a native fit for a claimed Cedar gain.
The current runs continue toward original-ROM boot qualification, with no
CPU/cache/Map/IOCB state import. No new hardware fit/load or speed is qualified.
Original60ns phases/timers and final persistence/three-boot/stress gates remain
open. Goal active; complete FPGA snapshots deferred.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-row-writes/native-powerup-transport`.
