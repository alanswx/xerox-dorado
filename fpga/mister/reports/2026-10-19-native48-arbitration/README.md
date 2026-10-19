# Native48 matched memory-arbitration experiment

BUILDING; NOT DEPLOYED. Collect and review full four-condition timing before
programming MiSTer14. A build is not an FPGA speed measurement.

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

Current invocation: Mac exec76623, remote fitter498946 verified live at
00:54UTC19Oct, routing after successful placement. Revalidate the same handle
or actual process before restarting. `collect.py` requires31 summary and36
detailed checks across four conditions, nonnegative slack and no illegal/
unconstrained clocks, then records the RBF and compiled-source hashes.
There is no qualification artifact until those checks pass.

Next measure clean47b idle login before Guest, then48 on the same fresh seed,
Main21e67a3f/serverc5996249, direct DDR, guest time and input sequence. Repeat
independent boots before promoting a measured gain. Original firmware/all34
loader/desktop/input/persistence and final sustained gates remain required.
