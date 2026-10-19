# Complete modeled processor-board state: restore/classification/timing PASS

Separate exact frozen49 ProcH and ProcL adapters preserve5712 and5581 mutable
modeled bits. These include all FF q/pre-capture/clock histories, every
register/stack/parity RAM word, small-file pending-write windows, and ten
board phase-history bits each.512bits of immutable PROM per board are pinned
separately by package/path/content hash. Production/live FPGA sources unchanged.

New MC10175 and MC1668 clones preserve the sampled transparent latch/reset
rule and S/R priority with both clock histories. Previously qualified FF,
F10145 and asynchronous256-word RAM adapters are reused. The original board
body, pin bindings/parameters/continuous logic remain unchanged. A16bit state
address selects7bit package and9bit row; FF/phase row0, small files0..15 plus
pending window16, async RAM0..255. Every other address errors without writes.
Caller must hold every consumer/frontend and external phase/input state:
async scans CAN change combinational outputs. No global barrier is supplied.

Full-board original/continuation/restore fixtures pass, all75ProcH/96ProcL
output bits and all5712/5581 modeled bits. ProcH144860host edges/104840held/
13334late enable changes/94831requests/1737valid words. ProcL144264/104290/
13334/94235/1708. Every bit walked, every invalid16bit address rejected;
all original RAM writers exercised (14H,12L). Zero/all-one and random stimulus
avoids leaving two low-board writers unexercised. Candidate-only destructive
execution, complete restore, immediate release/first real guest edge and20000
more cycles match the stopped untouched original. Eight faulty RTL variants
per board fail, including lost history/latch/RAM/window and invalid alias.
Some corrupt-state controls fail on the write/readback check; others fail on
the immediate original-state comparison while restoring. Both detect the
intended corruption; early diagnostic-order refinements are retained.

Fresh original XML classifies every candidate: mutable saved fields,
848H/636L simulator-only file diagnostics excluded under translate_off,
512immutable PROM bits each, and recomputed combinational/constant logic.
The ALU's complete case/default table and read-only PROM source are pinned;
no general procedural-state waiver.16 damaged schema/source controls per
board reject, including missing/changed/mutable PROMs. Original cells are
byte-identical to frozen49 except MC10170 full-line documentation comments;
a narrow complete-code-line comparison proves that one difference and keeps
both raw sources. No inline/block-comment/code change is waived.

Initial opposite-edge shell fits are REJECTED/PRESERVED: the untouched
boards also fail the artificial10ns interface budget. Slow-corner original/
adapter setup slack H-7.262/-5.665ns, L-1.220/-0.621ns; paths are shell->FF
or FF->shell, with10ns clock relationships. They are not timing-qualified.
The real processor neighbors sample on positive host edges. A separate
matched positive-edge launch/capture experiment therefore uses20ns embedded
paths at the SAME50MHz, SAME2ns max/0ns min virtual I/O and no timing cuts.
Board/adapter RTL and immutable PROMs are identical across the experiments.
No production constraint or FPGA clock is changed to pass this experiment.

Qualified projects SnapshotProcessor_positive_ProcH_20261019 and
SnapshotProcessor_positive_ProcL_20261019; parent job88958 TERMINAL. All map/
fit/STA/four-condition jobs ran sequentially. All12applicable setup/hold/pulse
checks per variant/board pass; recovery/removal have no paths. Minimum
snapshot slack H+0.136ns/L+0.038ns. No illegal/unconstrained clocks or I/O.
Actual synthesis AND fitter tables retain six MLAB memories/4608bits/144MLAB cells
per board: four256x4 plus two256x1, no M10Ks. Small files remain logic.
H1109->2385ALMs/1201->1679regs; L1110->2259/1124->1613. These include shell,
admin and otherwise-pruned history retention, not projected full-core cost.
Non-unused/stuck-output warning categories never increase; fitter counts
match. Seven corrupted source/QSF/constraint/inference/physical-placement/
hold/warning controls per board reject. Raw reports/sources/logs preserve
exact bytes through gzip, including rejected half-cycle evidence.

Reproduce test_processor_full.py and classify_processor_full.py with --kind,
--source, --output and --mutations/--negative-controls. fit_processor_full.py
--kind KIND --source FROZEN --prepare NEW then --run NEW on misterubuntu;
audit_processor_full.py NEW --output qualification.json --negative-controls.
No whole-core clock distribution, FPGA boot, complete physical snapshot,
restore-time benefit, original60ns cadence or speed gain is proved here.
Next full-core integration/cross-board timing/original firmware qualification,
then remaining IFU/cache/map/device/wrapper/raw SDRAM/Main/server state,
global barriers and complete atomic bundles.
