# Native48 matched memory-arbitration experiment

TIMING PASS and deployed. Original-firmware login and preliminary measured
idle gain: ../2026-10-19-native48-field/. Full software qualification pending.

`build.py` freezes Native47's physical DDR network, SDRAM/no raw cache,
50MHz/SYSPER4, architectural boards and timing constraints. It copies the
tested Native43 response-boundary raw arbiter, enables PREFETCH_NEXT in the
generated wrapper and records that policy in the manifest. The script asserts
exactly those three source changes and unchanged clocks/constraints before
building `misterubuntu:/home/alans/mister/Dorado_Native48_20261019`.

PREFETCH_NEXT selects the next pending group at a response boundary; it does
not speculatively complete guest reads or add an outstanding raw request.
Native43's component/whole-memory equivalence evidence stays separate.
This build combines it with the current DDR network for a comparison using
identical Main/server/packet transport to clean Native47b.

Build76623/fitter498946 COMPLETED.31 summary/36 detailed checks across four
conditions pass, minimum+0.102ns, zero illegal/unconstrained clocks, no new
cuts.30,533 ALMs73%,52,795 registers,330M10Ks. Frozen sources/fit/timing
reports and RBF hash40750c11 are included. Do not restart this build.

Next measure clean47b idle login before Guest, then48 on the same fresh seed,
Main21e67a3f/serverc5996249, direct DDR, guest time and input sequence. Repeat
independent boots before promoting a measured gain. Original firmware/all34
loader/desktop/input/persistence and final sustained gates remain required.
