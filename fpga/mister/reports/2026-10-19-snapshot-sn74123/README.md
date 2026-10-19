# Complete modeled SN74123 timer state PASS

Both signed32bit countdowns and all six trigger histories (70bits per cell)
are exported in four18bit words, with16valid bits in the final word and invalid
indices4..7 rejected. Reset/retrigger/expiry priority, signed comparisons and
original tick arithmetic/parameters are preserved. No production cell changes.

Original RTL and independent signed-counter oracle agree for110,122host edges,
2,880held/2,872requests/4,800cell-word writes in six parameter modes: zero/one
count boundary cases and SYSPER2/4/8/16 with TW_NS3000, including actual native
SYSPER4/TW400. Tests cover all70bits/transport padding/all64pin values, signed
negative/maximum/zero/one countdowns, destructive mutation/individual restore,
quiet release/recapture/first edge and20,000more samples. Nine broken RTL and
two changed-original-source guards reject. Fresh XML validates actual bindings,
all sequential fields and signed32bit integer types;17damaged schemas per mode
reject. Original source byte-matches frozen Native57/58.

Matched50MHz isolated fits pass four operating conditions/min+0.140ns, no
illegal/unconstrained clocks or I/O, RAM or extra exceptions. Resources522->602
ALMs/520->904registers include the staged shell/admin/pruning, not whole-board
snapshot cost. Five damaged source/SDC/macro/hold/warning controls reject.
Compiled inputs are reverified after collection. Raw sources/reports/logs are
gzip-compressed unchanged; executable build databases excluded.

This preserves modeled pulse state; it does not calibrate timers to original
wall time, provide full DskEth/global save/load or change live Native58 speed.
Complete board/monitor/phase state qualification and integration remain open.
