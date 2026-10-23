# Original Control-A MIR capture component

The original wire list and all 503 qualified 5c65 source identities bind eleven
MC10231 packages: a23/a24/b23/b24/c23/c24/d23/d24/e23/e24/g16. Their 22 physical
Qs include 21 wired halves and one unobserved g16.A half. The active native
snapshot wrappers have the same physical pin connections as the original
monitor; state administration stays inactive. Three actual bad pin bindings
reject after deliberately bypassing only the initial whole-file hash check.
This inventory is not a native replacement or an arrival/timing proof.

Six capture nets remain distinct: h*clk0'Ca through Cf. In particular e23's
Block' and g16's PrBlock' use different clocks and have their original opposite
maintenance set/reset roles relative to the ordinary FF/JCN registers. Both
physical d23 halves driving the wired-OR JCN.2or3 are retained; normal-data
agreement with d24 is not an arbitrary initial-state invariant.

`dorado_mir_capture_step.sv` expresses those registers as an explicit event
transfer, with 45 raw data/maintenance/clock boundary inputs and 22 existing Qs.
There is no new reset, initial value, copied RAM, host state transport or
architectural pipeline stage. The unwired half retains its state. The original
pin-level reference is extracted from .wl independently of the candidate's
manual bit equations, using the prior ideal event-driven MC10231 reference.

Both Icarus and Verilator agree: 12,396 events, 6,112 capture transfers, 4,117
asynchronous-control exercises, 4,102 late-data/held-clock checks and 2,290
observations of differing Block/PrBlock values. All six clocks are exercised;
both values of every physical Q occur. Four actual damaged RTL variants reject
in BOTH engines: merged Block/PrBlock clocks, a lost JCN physical capture,
wrong Block maintenance and late-data pass-through. Exact executed sources,
reference graph, command lines and all raw compile/test logs are retained.

This is a stable-setup digital component test. It excludes simultaneous
asynchronous set/reset, and does not characterize analog setup/recovery,
independent fanout skew or actual IM/maintenance arrival timing. The next work
is coupling actual MIR/CP sources and clock fanouts to the Ready/arbiter/phase
slice, preserving the pre-event data epoch. No new native compile/fit/load,
66.7MHz selection or60ns CPU qualification is claimed.

The appendix freezes the initial running read-only watcher 7144 (tool90999)
for existing native v2 tests 2284/2331. It observes every 30 s and preserves both
actual113 windows when available, including any request/data disagreement.
It never changes limits, controls, stimulus or processes. Both simulations are
live and initialized; no CPU issues/113 at this prefix. A four-hour watcher
bound only stops observation, not either simulation. Observe existing jobs.

Hardware remains5c65/Main13244/daemon13243, with cold native files/input PASS
and external HDMI OPEN. Hardware speed is unchanged. The complete clock and
memory goal remains active; original firmware, disk sources and protected
Executive artifacts are preserved.

Verify bytes: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-23-mir-capture/component`.
