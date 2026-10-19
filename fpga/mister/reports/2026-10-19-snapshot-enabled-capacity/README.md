# All-enabled core: terminal capacity rejection and remaining-state inventory

Enabled60 passed synthesis and its independent source-integration audit, then
failed fitting. Error170012 requires 5,505 LABs versus 4,191 available. The
failed-fit summary reports 54,403 ALMs versus 41,910 available: an excess of
12,493 ALMs (29.81%). The builder stopped on fitter exit3; no STA, assembler,
deployment or physical snapshot followed. All eleven board interfaces remained
enabled throughout; no coverage was removed to reach a smaller design.

`capacity-review.json` checks the terminal failure and records the eleven-board
resource attribution against Native59. These are diagnostic estimates from a
failed fit, not a legal placed design or a sum predicting another fit. The
largest increase is BaseBoard: 3,291.9 -> 9,765.9 attributed ALMs. Its five
RIOTs account for 7,341.5 attributed ALMs. This motivated the separately
qualified shared-RAM-port experiment in ../2026-10-19-snapshot-riot-shared-port.
The failed summary's zero RAM-block count is not a placement result; there
is no valid routed/timing qualification for Enabled60.

The fresh support inventory uses the exact local staged source hierarchy,
215 HDL source files plus QSF, fifteen project macros and the actual native
wrapper parameters. Elaboration includes 3,925 instances. After separating
the eleven already qualified board scopes, it retains 12,666 candidate rows:
154,037 sequential candidate bits, 1,802 initial-only candidate bits, 335
subroutine-review bits and 28,249 combinational-review bits. All remain review
candidates; none is declared unnecessary or already snapshot-capable.

Of the sequential candidates, the disk media bridge accounts for 131,816 bits
(including its 16KiB buffer), the eight raw-memory-array wrappers for 21,520,
and the remaining wrapper/arbiter/SDRAM/keyboard/terminal/drive/pixel-clock/
diagnostic fabric scopes for 701. External 16MiB raw data/ECC backing, MiSTer
top-level mouse/key queue/wire/DDR/pack/video state and ARM disk/server state
are outside this native-machine XML scope. This inventory is not a complete
machine classification or save/restore implementation.

Inventory v1 stopped on a QSF comment apostrophe before elaboration; the
parser now selects assignment lines before shell-style tokenization. V2 and
v3 elaborate byte-identical XML; v3 groups records by owning instance rather
than variable name. Only v3's duplicate XML/full inventory is stored, alongside
both versions' distinct provenance/summary/logs. Large JSON, XML, raw reports
and logs use lossless gzip. All 22 raw archived files pass hash readback.

Remaining work: reduce enabled administrative logic without losing any state,
obtain an actual legal full-core fit and all-device timing, finish support
state classification/access, and implement a coherent global barrier and
atomic disk-paired bundle. Current DDR lifecycle RESUME reconfigures the Pup
server and discards its old protocol session; it must not be reused as a
snapshot pause/restore. A separate preservation path must freeze/rebase server
timers and retain connections and queued frames. Live Native59 is unchanged.
