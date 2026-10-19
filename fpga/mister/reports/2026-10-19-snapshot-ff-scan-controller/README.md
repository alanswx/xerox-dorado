# Isolated serial FF scan controller

Simulation PASS for an experimental controller and mock physical FF bank only.
No real board integration, resource reduction, routed timing, firmware boot or
whole-machine snapshot is qualified. No live FPGA or clock change occurs.

The controller rotates the existing physical bank through a one-bit chain;
it stores no shadow bank. A read captures the requested compact 18-bit word
and completes only after exactly STATE_BITS shifts have returned the bank to
its original orientation. A write replaces only the requested bits during
that rotation. Other bits return unchanged; final partial-word padding reads
as zero and writes are discarded. Requests/payloads arriving while busy cannot
replace the owner. This is a new compact scan word layout, not the old per-chip
word layout; real integration needs an explicit generated schema and ABI.

Every consumer must remain held throughout rotation. The controller asserts
hold_required while busy or quarantined. Losing freeze or guest_held produces
an error and sticky fault, blocks further shifting/requests and retains hold
until SYSTEM reset. A transport-only reset cannot recover partially shifted
physical registers. Real integration must include this hold in the global
machine barrier and implement reset/lifecycle ownership; the present board-only
diagnostic freeze is insufficient for such a scan.

Simulation v2 passes 438 transactions across six bank sizes: 1, 17, 18, 19, 56
and 1,725 bits. The independent oracle applies direct word/bit operations to a
bank copy; it does not simulate serial rotation. Tests cover every word, all
18 payload lanes, partial final words, simultaneous changing requests while
busy, freeze entry, invalid/high indices, running requests, exact bounded
completion, and reset recovery. Hold-boundary loss is tested during a partial
rotation for sizes greater than three, and before the first shift for size one.
Four damaged implementations reject with the expected specific diagnostics:
lost feedback, early completion, writes outside the selected word and escaped
quarantine. V1 is retained; V2 strengthens partial-rotation fault coverage and
makes the counter width internal to the single STATE_BITS parameter.

The archive preserves all source/test/command/log/result bytes, excluding the
compiled Icarus simulation executables. Hash readback passes. The next gate is
to integrate a chain through actual heterogeneous board FF helpers, prove all
physical bits and normal-operation/restore behavior, then compare matched area
and all-device timing before selecting this architecture in a complete core.
