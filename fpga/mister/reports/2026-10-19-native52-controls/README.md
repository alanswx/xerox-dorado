# Native52: both control-board adapters, full-core timing PASS

NOT DEPLOYED. RBF SHA256:
`be80e03f26e5b9e5e7984a0d9b5df15b84d264a1f526e95149e085d25a08768c`.
No hardware boot, speed improvement or complete physical snapshot is claimed.

Builder `build_native_conta.py` from5e1da91a freezes Native51 and integrates
qualified ContA host-clock/exact-advance adapters alongside its ContB adapters.
Seven new RTL sources and one active backplane source change; the two shared
FF clones remain byte-identical. Original ContA SYSPER is declaration-only;
that fact and the native instance binding are checked. Core wrapper, storage,
other boards, clock distribution, Main/server and timing constraints remain
unchanged. ALL snapshot administration inputs remain INACTIVE. Quartus can
prune this access logic, so these counts do not budget enabled snapshots.

Actual project: `/home/alans/mister/Dorado_Native52_20261019`.
Inputs: `/home/alans/mister/Dorado_Native52_inputs_20261019`.
Build19107 completed. Map/fit/asm/STA/device audit/both-control crossing audit
ran sequentially. The full collector ran only after terminal build completion.
31 summary checks and68 detailed reports pass across four operating
conditions:36 device/SDRAM plus16 ContA and16 ContB crossing reports. Minimum
slack+0.112ns. No additional timing exceptions; illegal/unconstrained clocks0.
Existing framework I/O coverage remains3 input ports/13paths and50 output
ports/154paths. SDRAM and control crossings are timed; this is not a claim of
zero unconstrained I/O. All eight actual IM macros retain1024x18 OLD_DATA.

Resources:30,553ALMs (73%),52,682registers,330M10Ks,35DSP. Relative to51,
-87ALMs/-239registers; admin pruning prevents interpreting that as the cost
of enabled snapshot access. Original50MHz/SYSPER4, SDRAM/no raw cache,
interleaving/arbitration ON, deferred reads OFF, direct-DDR networking.

The first collector correctly rejected a new10230 truncation warning.
The exact expression selects package75..83 minus75, otherwise0: selected
range0..8 fits its4-bit destination. The unsized zero widens the expression
to32bits. v2 pins the exact guard/statement/register width/single writer and
entire10230 warning delta, proves all128 package values, and rejects unsafe
range, false-arm and unrelated-warning controls. Three additional10036
unused-object warnings are reviewed; fitter warning counts are unchanged.
No RTL, constraint, fitted database or raw report changed during this review.
The initial rejection and final audit are archived. Additional QSF macro,
SDC and wrong-source controls reject. Only exact approved physical source
aliases, blank lines and the Quartus version marker are normalized.

`qualification.json`, `compiled-inputs.json`, integration/assets manifests,
QSF, raw timing/logs and generated sources retain evidence/provenance. Raw
logs and source files are gzip-compressed without changing their bytes.
The local candidate is `build/rtl-executive/native52-candidate`.

Next resolve51's observed slower loader using a fresh selected49 control,
then normally load52 and qualify original-ROM startup/Cedar/input. Remaining
boards, active state access, global barriers, SDRAM/Main/server matching
state, atomic snapshot bundles, original60ns cadence and final repeated/
persistence/two-hour stress gates remain required.
