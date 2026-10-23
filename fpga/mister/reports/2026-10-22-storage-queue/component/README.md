# Bounded owner-tagged storage queue and reset drain

The new standalone queue is not selected by a native wrapper. Its four-slot
ownership window includes queued descriptors, the one issued raw operation
and a held raw response. It retains owner/address/data/write fields until use,
allows one outstanding request per original group and completes in admission
order. Queue admission is not physical write completion. Reset suppresses new
frontend traffic/replies while draining EVERY accepted descriptor, including
unissued writes. Old replies are discarded, then the empty backend receives a
reset clock before a new epoch. enable gates new admission only.

Protocolv3 passes ten positives in Icarus/Verilator with identical summaries:
depth1/3/4, non-power-of-two group count and native128 groups with16 local address
bits/23 raw bits. Every owner and both values of every local-address bit occur;
the full8,388,608-word reference store is compared after drain, not exhaustively
written or physically tested. Native128:262 admissions/issues/completions,
148 writes,13 discarded old-epoch replies,249 frontend completions,221 same-edge
raw handoffs and five reset epochs. The independent FIFO/oracle checks full
payload, per-group exclusion, total credit, ordered issue/response, held data,
fairness and accepted writes after reset. Eight real broken implementations
reject in BOTH engines: early backend reset, dropped queue, wrong owner, live
payload, excess credit, duplicate group, stuck reservation and blocked drain.

Independent physical SDRAM pins also pass depths4 and3 using the same reset/
payload oracle:139/137 admitted/issued/completed descriptors,85/72 physical
writes, all reference locations unchanged after drain except expected writes,
held replies and five reset epochs. The physical backend is reset only after
accepted work completes; an actual early-reset mutation fails that property.
These small pin fixtures use unwritten-zero model data, not physical RAM init.

Original144-chip MSA agrees over12,012 architectural cycles/750 backing reads/
7,872 writes/1,111 output transitions and all9,437,184 stored bits. Per-group
ordered operation histories and final raw oracle are checked; the actual global
raw operation order ALSO remains identical. Control101,546 -> queue101,328 host
ticks, only218 /0.21468% fewer. A wrong owner fails the queue's unowned-issue
check. The shared board driver's legacy mode reproduces103,659/101,546 traces
byte-identically and retains the original bad-data rejection.

The exact native128 synthetic transport completes1,920 requests/640 writes with
identical admission/completion channel order, valid read data and final physical
locations. Control8,492 -> queue8,491 ticks: one tick less response backpressure;
raw-response wait4,930 and burst-drain block1,122 are UNCHANGED. This is no
substantive speedup and does not justify a native fit alone. A wrong owner fails
the internal ownership check. All failed development is retained: the first
drop-queue mutation had an invalid extra semicolon, and the first fanout bad
owner failed unowned issue before its expected later owner/completion checks.
Both corrected final suites are terminal; the DUT required no repair.

Native integration is OPEN: connect reset globally, gate new admission with
storage_initialized, give queue reset drain priority over the raw initialization
mux, hold selftest initialization reset until that drain finishes and drive the
backend from backend_reset. Do not wire the old arbiter's reset-or-not-initialized
directly onto this queue or disconnect accepted raw work. Global machine-edge
barrier, original chip-group old-word/commit, all native clocks and Main/daemon
remain unchanged in the running5c65. No native fit/load or CPU/Cedar speed claim.

The appendix retains a separate default-off same-row WRITE stream preparation.
Alliance AS4C32M16SB Rev1.4 section6/Figures10-13 and section8/Table11 support
consecutive writes with proper precharge/read-turnaround recovery. It has ONLY
passed syntax/elaboration in both simulators; width diagnostics are retained.
Behavioral, physical-pin, actual ACK-after-capture, row/rank/refresh/reset/
backpressure/coherence/old-word and native timing checks remain mandatory. It
is not part of the qualified queue/controller and has no native selection.

Next qualify that write-command/ACK path and measure real native traffic before
native candidate selection. Original60ns CPU phases/all-device rates, explicit
5c65 Rollback/cold persistence, final three fresh seeds and two-hour stress stay
open. Goal active; full FPGA snapshots deferred.

Verify with `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-queue/component`.
