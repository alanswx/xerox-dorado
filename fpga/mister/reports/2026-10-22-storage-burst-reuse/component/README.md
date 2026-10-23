# Per-beat and completed-line reuse during SDRAM burst drain

The default-off BURST_READ_REUSE extension adds12 logical state bits: an8-bit
received-word mask, one waiting-for-data bit and a3-bit reply selector. The
unchanged physical address is the stable fill tag. A completed line, received
word or currently arriving word can reply immediately; an accepted future word
waits for its own arrival. Command ownership and logical reply ownership remain
separate. No early write, unreceived word, changed physical request, duplicate
tail response or overwritten held reply is allowed. Physical reset/drain,
refresh and original chip-group write/old-word/commit rules are retained.

Protocolv3 passes six positives in BOTH Icarus and Verilator: off/on57.1MHz,
on66.7MHz. Summaries/counters match exactly between engines. Directed57MHz
coverage observes65 received-word,137 current-word,20 future-word and8 other
completed-line admissions,69 admissions without an old reply pending, and one
explicit future-wait reset.66MHz likewise exercises every category. Accepted
requests equal completions plus reset aborts. A128-clock bound checks every
accepted request's response assertion while permitting arbitrarily held replies.
Nine actual wrong implementations reject in both engines: stale valid mask,
wrong fill tag, wrong received data, wrong future selector, admission before
the original reply, early write, ignored response hold, wrong completed-line
data and overwritten physical owner. This bench uses a small functional
command-pin memory test double, not the independent timing model.

Independent physical pinsv3 passes control/candidate at57.1/66.7MHz, retaining
all prior rank/bank/first-beat cold chains, coherence, reset and held-refresh
checks. Additional probes use same-fill received/future words and a completed
line in the other physical rank while the original burst drains, plus reset
while waiting for a future word. Candidate211/218 in-burst admissions cover
all eight offsets. A real corrupted received word fails the physical data
oracle before reaching the later new probe. The old successor mode regenerates
its prior pin fixture BYTE-IDENTICALLY and reproduces all four positive results.

Original144-chip MSA comparison passes12,012 cycles/750 backing reads/7,872
writes/1,111 output transitions and all9,437,184 stored bits. Default-off raw
trace is byte-identical to the qualified103,659-tick control. Reuse takes101,546
ticks (2,113 /2.0384% fewer), versus the prior narrow successor102,931. Canonical
operation/data order and final memory are unchanged. Corrupting the new reply
path fails the original MSA output check at cycle3251.

The exact native128-group arbiter synthetic fixture remains9,386 control versus
8,492 candidate ticks. Both raw traces are byte-identical to the preceding
adjacent-successor experiment: NO ADDITIONAL GAIN from broader reuse on this
traffic. All1,920 requests/640 writes, channel order/owner/data and final physical
locations agree; a wrong-owner mutation rejects. The shared fanout driver's
legacy mode also reproduces both old traces exactly. This is not a native CPU,
Cedar or FPGA speed result. Actual firmware wait attribution remains open.

Failed attempts are retained. Protocolv1's wrong-tag negative could not compile
because Icarus forbids that later wire declaration; v2 corrects the negative,
then the wrong future selector produces a missing response caught only by the
overall deadline. Finalv3 adds the explicit accepted-request bound and all24
cases pass/reject as intended. Physical pinsv1 exposes the inherited sender's
stale combinational-ready assumption. For reuse only, the fixture now holds
valid until an actual posedge handshake. Pinsv2's corrupt DUT correctly fails
the earlier physical read checker, before the expected later probe; finalv3
recognizes that precise earlier data rejection. No DUT repair was required.

No native fit/load, host clock, Main or daemon change was made. The live5c65
baseline remains the preceding4.02547345M prelogin StartCycle measurement at
140ns nominal, not60ns. Next: bounded owner-tagged request admission, measured
raw response/write-command timing and actual native firmware traffic, then
qualified native selection. Preserve the global machine-edge barrier. Full
FPGA snapshots remain deferred; original60ns phases/all-device rates, explicit
5c65 Rollback/cold persistence, three fresh seeds and two-hour stress stay open.

Verify with `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-22-storage-burst-reuse/component`.
