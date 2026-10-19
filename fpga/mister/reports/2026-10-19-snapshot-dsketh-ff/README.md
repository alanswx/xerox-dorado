# Four DskEth small-state families PASS

Pinned original MC1650 comparator latch(2bits), MC10136 counter(12bits),
SN74LS169 counter(8bits) and SN74LS174 FF(13bits, PRECAP0/1) now have local
18bit state words. All original pin/parameter/initialization/continuous logic
is preserved; exact reviewed NBA processes run only on host advance or frozen
state load. Every counter, edge/setup history and comparator latch is saved.
Original production cells and the running Native58 are unchanged.

Original RTL and independent per-family state/output oracles agree for111,271
host edges/2,157held edges/5,230cell-word writes. All4096input combinations,
all logical bits plus transport padding, destructive whole-state mutation,
individual restore/quiet release/first resumed edge and20,000further samples
pass in five modes. Eight broken RTL variants and four extra-state original
source guards reject. Fresh XML checks every sequential field and constant/
derived candidate, including actual SN174 PRECAP bindings;54damaged schemas
reject. Exact first XML review and accepted classification are preserved.

Matched50MHz staged-shell baseline/candidate fits pass all four operating
conditions/min+0.164ns. No illegal/unconstrained clocks or I/O, no RAM or extra
exceptions. Resources93->113ALMs/76->168registers include shell/admin/pruning,
not a whole-board/machine estimate. Five source/SDC/macro/hold/warning corruptions
reject. Raw sources/reports/logs gzip-compressed unchanged, build DBs excluded.

These are local primitives: the caller must freeze all board consumers and
gate writes correctly. No board request/invalid-address handshake or global
machine snapshot is provided. Remaining DskEth work includes SN74123 timers,
actual oscillator binding coverage, receive-monitor/board phase classification,
complete restore and timing before core integration. No clock/speed change.
