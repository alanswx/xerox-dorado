# Explicit frontend D/enable factoring: rejected for area

Matched fully enabled native WIDTH8/WIDTH12 frontends use **278 ALMs** with the
qualified serial implementation and **282 ALMs** with explicit next-value and
update-enable factoring. The candidate is not adopted. Both have 519 registers
after synthesis; placement reports 597/592 after duplication. Identical device,
registered 50 MHz shell, 221 virtual pins, 2 ns/0 ns IO budgets and fitter policy
are retained. Four device conditions pass with minimum slack +0.160 ns and no
unconstrained clocks/IO. Recovery/removal have no paths, as expected for this
synchronous fixture. Four damaged fit/constraint reports reject.

The transformation preserves all original registers, initial values, scan order,
normal RHS expressions and procedural assignment priority. A combinational
process supplies enable/next-value pairs, and clocked registers use those pairs.
No new sequential fields, memory policy, geometry or interface behavior is
intended. This was a component experiment, not a change to the running core.

The unchanged original RTL and DRAM-cell oracle pass 110,808 host edges /
2,237,064 checks, with 5,010 guest advances, 10,948 raw requests, 924 writes,
63,324 scan edges and nine snapshots across pending-read, pending-write and
write-done boundaries. All eleven actual damaged RTL variants reject.

Icarus additionally compares all physical fields and all interface outputs
against the qualified serial frontend over 12,143 ticks with X/Z inputs and
deliberately unreachable restored states. Both copies omit only the original
legal-advance simulation assertions for that algebra comparison; valid guest
behavior is checked separately by the original DRAM oracle. Three damaged RTL
variants reject, including different X-hold semantics and write-done priority.

History is retained: the first generated process lacked whitespace between
adjacent end/begin tokens and did not compile; the next run's last damaged RTL
correctly failed but the runner expected a later diagnostic. Spacing and the
expected diagnostic were corrected without changing normal logic. Four-state
v1 omitted serial_out from its comparison bus; v2 corrects the bus to 55 bits
and repeats all checks. See qualification-history.json.

All 650 raw archive members pass lossless readback. Extracted pinned original
bytes regenerate identical baseline/candidate RTL. The extracted original DRAM
oracle rebuild, four-state tests/controls and matched-fit review/controls pass.
Generated objects/databases/executables are excluded; raw sources and reports
are compressed without whitespace edits.

The installed Quartus17 assignment metadata is retained in fit/query-area-options
files. It documents supported aggressive-area and register-chain packing modes.
Serial62Area is a separate whole-core experiment using the unchanged nineteen
qualified board adapters and four QSF optimization changes. This rejected
factored frontend is absent from that build. No hardware or speed change.
