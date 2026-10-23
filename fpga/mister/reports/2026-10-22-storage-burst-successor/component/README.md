# Adjacent-beat successor: protocol, physical pins and128-group transport

The exact default-off candidate from the preceding original-MSA replay now
passes a dedicated protocol suite in both Icarus and Verilator. Six positive
runs cover off/on57.1MHz and on66.7MHz, with identical summary counters between
engines. All requested beats, ranks/banks, held responses, same/cross-line,
wrong-row/rank/beat offers, write exclusion/coherence, reset at every beat and
refresh admission are exercised. Accepted requests equal completions plus
explicit reset aborts. Eight actual broken variants reject in both simulators:
corrupt forwarding, ignored response hold, early write, wrong line, wrong beat,
ignored refresh, retained reset response and duplicate tail completion.

That protocol bench's small command-pin memory is a functional test double.
The separate independent XSDS physical model passes control/candidate at both
57.1/66.7MHz:64 cold chains per case span every rank/bank and first requested
beat, with response stalls. Candidate forwarded129/128 requests respectively,
covering beats1..7. Existing physical coherence/reset/refresh/hold tests remain.
An actual corrupted forwarded word fails the independent data/order check.
The earlier original144-chip MSA comparison and all9,437,184 backing bits are
retained in the separately hash-bound exploratory-msa receipt; its narrow0.7023%
fixture result is not replaced by these synthetic fanout figures.

The new fanout test binds the exact qualified5c65 arbiter, native128-group /
16-bit local /23-bit raw geometry and original native lookahead/interleaving
parameters. It drives24 pinned coherent/scattered read/write rounds through
independent SDRAM pins:1,920 accepted/completed requests,640 physical writes,
and direct final checks of every programmed physical location. Requests and
completions retain exact per-channel order, owner, address and valid read data;
invalid write-ACK data is intentionally not compared. A wrong-owner RTL mutation
fails the held-response check. No CPU, original boot or Cedar executes here.

Control9,386 active host ticks versus candidate8,492:894 ticks /9.52482% fewer
in THIS synthetic workload. Burst-drain admission blocks fall2,017->1,122;
raw-response wait remains4,920->4,930; admissions1,920 each. Candidate forwards848
requests. All exclusive categories sum to the measured intervals. Positive
raw traces reproduce byte-identically in two terminal runs. The detailed mode
and read/write service-latency breakdown is frozen in analysis.json. This is
not evidence of a9.5% Cedar/hardware speedup or complete native CPU wait
attribution. Actual native firmware traffic still needs capture/measurement.

All failed test development is retained: protocolv1's test-memory launch was
one clock late; v2/v3 rejected real bad variants but expected a later diagnostic
than the first failing check. Finalv4 adds exact accepted/reset accounting and
an explicit refresh-admission assertion; all22 cases finish. Fanoutv1 accidentally
aliased its supposedly cold scattered address with a prior programmed cell;
v2 separates those addresses and passes both positives. Fanoutv3 rejects the
wrong owner through response stability before the expected owner check; finalv4
accepts either causally valid diagnostic and passes. No DUT repair or source
change was needed. Terminal source/logs and corrected final drivers are frozen.

No new native fit/load or clock/Main/daemon change occurred. Live5c65 remains
the last qualified4.02547345M prelogin StartCycle issue baseline at140ns nominal.
The next implementation is broader per-beat fill validity, completed-line
service during drain and bounded owner-tagged admission. Preserve the original
machine-edge barrier and write/old-word semantics. Original60ns phases/timers,
actual native wait attribution, explicit5c65 Rollback/cold persistence and final
three-seed/two-hour qualification remain open; the goal stays active.

Verify with `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-burst-successor/component`.
