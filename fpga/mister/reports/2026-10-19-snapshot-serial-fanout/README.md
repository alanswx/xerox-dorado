# Serial66Fanout source and synthesis qualification; full fit pending

Every Serial65 RTL byte, clock period, IO delay, timing exception and snapshot
schema stays. The only policy additions are MAX_FANOUT=64 and an explicit
IGNORE_MAX_FANOUT_ASSIGNMENTS OFF on `emu|network_step`. The installed Quartus
descriptor is retained. The change addresses the 273-load source of Serial65's
remaining 10 ns MemC setup path, not the nominal clock period.

Independent review checks all 659 parent inputs and 2,761 original pins/parameters;
six rehashed wrong-target/limit/ignore/cut/initialization/service cases reject.
Extracted tools reproduce every source input and all three integration records.
`post-map-verification.json` confirms all 659 actual inputs remain unchanged and
Quartus inserted four logic cells for exactly `emu:emu|network_step`. No RTL
state, clock or port is added.

The full-core job is ACTIVE at `/home/alans/mister/Dorado_Serial66Fanout_20261019`,
builder 2489421 / fitter 2491688. Observe the same handles rather than restarting on
timeout. Packet SHA256:
`290c13575512f1e0a9800d285b971117b918c1bebf22216660e6091129e77070`.
No assembly or hardware deployment. Actual capacity, all-condition timing and
all 304 adapter crossings must be reviewed after terminal completion. The disk
phase prototype is not included. Real service barriers/full snapshots remain
pending; no speed gain is claimed.

All 669 manifest members pass lossless readback and isolated source/restaging
replay. Prior immutable Serial65/readiness receipts are required; working HDL
is not used.
