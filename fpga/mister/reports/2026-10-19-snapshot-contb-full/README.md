# Complete modeled ContB continue/restore: RTL, coverage and isolated fit PASS

This extends the qualified IM-only experiment to all modeled state on the
actual generated ContB:147456 RAM bits,144 raw read bits and209 flip-flop/
pre-capture/clock-history bits, totaling147809 bits. Production generated
sources, cells, project assignments and the live MiSTer are unchanged.

`generate_contb_full.py` retains the audited eight18-bit half-banks and clones
the eleven MC10176 and eleven MC10231 packages. All reviewed original
nonblocking processes are retained under the exact guest advance on the host
clock. Restore temporarily replaces them while globally held. Original
pre-capture, common-clock OR and S/R priorities remain unchanged. Extraction
fails if the original process text changes or another process is introduced.
The original production cell sources are neither edited nor imported as state.

The local15-bit word index uses groups0..7 for IM (words0..1023 RAM,1024 raw
pipeline) and group8/words0..21 for sequential packages, in manifest order.
Each package exports a complete13-bit or6-bit word with zero padding to18bits;
writes ignore that padding. All other addresses error without mutation.
Requests require a completed freeze/held handshake and are ignored on entry.
No state-port request may remain outstanding when the caller resumes guest
clocks. A valid write's response value is unspecified; read it back separately.
This local access protocol is not the final file format or machine barrier.

The untouched original board has an independently stopped reference clock
and separately resolved backplane self-contributions. Only the fixture seeds
both RAM copies. An independent array oracle checks normal old-before-write
behavior. All71 outputs, all144 read bits and all209 sequential bits match
during ordinary operation, backend holds and deliberately late enable changes.
Every exported FF bit is individually inverted/read/restored; untouched
packages are checked. Every RAM/read bit is inverted/restored as well. Every
invalid15-bit address is exercised, including unused groups and rows.

After save, the candidate alone runs10000 cycles under REAL board logic.
The reference stays stopped; none of its state is patched. Restoring RAM,
read pipelines and all sequential words with the captured external inputs
reproduces the reference immediately, survives host-only clocks/recapture,
and matches from the FIRST actual guest edge through20000 more execution
cycles.132764 host edges,92737 held edges,13334 late-enable changes and82735
state accesses pass. Real-strobe writes cover every half-bank. Six broken
controls fail for their intended reasons: lost clock history, lost setup
pre-capture, unqualified FF write (detected on freeze entry), ungated FF clock,
FF-to-RAM aliasing and wrong FF response selection.

`classify_contb_full.py` freshly elaborates the ORIGINAL board and accounts
for every inventory candidate. All147809 sequential bits map to explicit
ports/words with checked geometry. Continuous-wire constants and combinational
expressions are recomputed; unreviewed procedural modules/initial variables
fail closed. Seven controls reject missing history, wrong width, missing
parity, duplicate/new state, overlapping fields and aliased words. External
backplane input capture and clock phase remain the caller's responsibility.
Added administrative request/result/freeze state is reconstructed by the
completed access protocol; raw physical scan outputs are hidden until the
first normal read, while exported guest-visible raw pipelines are preserved.

Matched Quartus17.0.2 fits on5CSEBA6U23I7,50MHz, retain147456bits/16M10Ks.
The candidate has eight1024x18 simple-dual-port OLD_DATA memories. The same
registered shell keeps2ns max/0ns min virtual I/O and10ns internal shell
budgets. The original baseline uses the falling-edge clock gate; the complete
candidate uses its matching advance on the host clock. All12 setup/hold/pulse
checks per variant/four operating conditions pass, candidate minimum+0.165ns.
R/R have no asynchronous paths. Illegal/unconstrained clocks and I/O are zero.
No timing exceptions are added. Nine damaged adapter/project/clock/source/
constraint/packing/mode/timing/coverage controls reject.

Matched shell counts:288->774ALMs and320->693registers (+486/+373), no RAM
growth. These include admin/shell pruning differences. They are not whole-
core resource estimates or timing qualification, particularly for paths to
other boards that still use the original gated clock.

Commands on Mac:

```
python3 fpga/mister/snapshot/test_contb_full.py --mutations --output NEW_SIM_DIRECTORY
python3 fpga/mister/snapshot/classify_contb_full.py --negative-controls --output NEW_COVERAGE_DIRECTORY
python3 fpga/mister/snapshot/fit_contb_full.py --prepare NEW_FIT_DIRECTORY
```

Copy the prepared fit to misterubuntu and execute its `run-fits.py`. Collect
results, then audit on Mac:

```
python3 fpga/mister/snapshot/audit_contb_full.py COLLECTED_DIRECTORY --negative-controls
```

Remote `/home/alans/mister/Dorado_SnapshotContBFull_20261019` and Mac handle
68165 are COMPLETED. The Quartus archive retains frozen inputs, assignments,
logs and all-corner reports. Simulation sources/logs and raw XML/classification
are gzip-compressed byte-for-byte. The auditor checks current source/tool
provenance and exact generated adapters/board/top/project/SDC. Source hashes
and classification decisions are archived alongside raw results.

Next: integrate the board access path into a complete-machine experiment,
extend/classify the other boards and actual wrapper/device/input queues,
and implement one coherent guest/SDRAM/disk/server barrier. Serialize matching
disk generation and daemon sessions/timers with atomic checksummed bundles,
qualify whole-core timing and pending/error/reset behavior, then measure
actual FPGA Cedar desktop restore. No physical snapshot, full-machine restore,
fast-boot speed or cross-build compatibility is claimed here. The required
60ns clock migration and final repeated/stability gates remain unfinished.
