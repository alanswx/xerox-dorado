# Complete modeled ContA state access and restore

The exact frozen Native49 ContA control-board module now passes isolated
state access, continuation/restore, classification and routed timing. This
is an experiment: production sources and the live MiSTer are unchanged.
Full-core integration, global service barriers and physical snapshots remain
required. The qualified source hash is1eb77199 (full value in the manifest).

The adapter retains all1,353 non-diagnostic modeled bits:681 FF/setup/history,
576 register-file RAM,90 pending-write-window and6 original board phase-history
bits. This is a modeled state count, NOT a fitted baseline register count;
state access can retain model history that synthesis otherwise prunes.
75 FF packages and nine F10145 files are addressed through229 valid words.
The12-bit local index uses package bits11..5 and row bits4..0. Packages0..74
have only row0,75..83 have RAM rows0..15/window16, and84 row0 has the six
board phase bits. All other indices reject without mutation. Upper padding
bits are ignored on write and zero on read. Entry/running/release requests
are ignored. Back-to-back held requests have one-cycle responses.

Original pin wiring, parameters, combinational feedback and reviewed
nonblocking process semantics are preserved. The actual host advance is the
same pre-edge enable that creates the guest clock edge. All FF and board
phase history freezes with that enable. The remaining FF-family generator
guards exact process tokens and complete register declarations; new processes
or missing state require review. F10145 clones use the separately qualified
original write-window process. No C state or host-completed instruction/IOCB.

Full-board comparison uses an untouched original, independent resolved
feedback for each copy, deterministic fixture-only RAM seeding and a separate
reference clock.59,787 host edges,19,878 held edges (including state scanning),
13,334 late-enable changes and9,758 state requests pass. All1,353 bits are
walked individually; every invalid12-bit address is tested. Candidate-only
execution changes state while the original remains stopped. Destructive
state writes, restore, immediate output/state equality, host-only release,
recapture before execution and the FIRST real edge plus20,000 more randomized
edges all match. All138 output bits are checked. Natural file writes are
742/742/742/742/495/804/804/804/804 across the nine actual packages.
Seven damaged RTL variants fail: ungated FF/phase, running access, lost
counter setup, lost pending file window, lost phase restore and invalid alias.

Fresh original-board XML atSYSPER4 accounts for all2,271 candidates:424
sequential,1,639 combinational and208 initial-only. All1,353 modeled hardware
bits are SAVE;954 simulation-only diagnostic bits (nine original file trace
blocks, including flags) are explicitly excluded. Other nodes are continuous
expressions to recompute.17 altered layout/inventory/source controls reject,
including missing history/phase, changed width, aliases, swapped window bits,
and an added process/register in every newly adapted FF family. These are
board-boundary claims; external clock phase/inputs and machine/ARM state are
not included. Original XML/source and decisions are compressed byte-for-byte.

Matched isolated fits on misterubuntu completed at
/home/alans/mister/SnapshotContA_20261019; handle61268 terminal. MiSTer device,
50MHz, opposite-edge launch/capture shell,2ns maximum/0ns minimum virtual I/O,
same DORADO_F10145_LOGIC setting. Baseline is the original continuously clocked
board; independent RTL tests establish equivalence during backend holds.
All12 setup/hold/pulse-width checks per variant/four conditions pass, snapshot
minimum+0.163ns. No cuts/unconstrained clocks or I/O; no asynchronous recovery/
removal paths.769->1974 ALMs,1343->1951 registers, zero block RAM in both.
Counts include shell/pruning and live isolated state access, not projected
whole-core costs. Four damaged source/SDC/QSF/hold reports reject. QSF checks
allow only blank lines and the exact Quartus17.0.2 version assignment.

Reproduce with the tracked test_conta_full.py/classify_conta_full.py tools and
`--source` pointing to the decompressed ContA_original.v.gz. Fit reports,
inputs and logs are in fit-evidence.tar.gz; extract into a new directory and
run audit_conta_full.py DIRECTORY --output RESULT --negative-controls from
the matching checkout. Simulation commands/generated sources/logs and tool
hashes are preserved. Compiled objects are excluded.

Next integrate ContA alongside the already converted ContB in the full core,
audit their cross-board timing and qualify original firmware/Cedar behavior.
Then extend remaining boards/wrapper/devices/input/raw SDRAM and ARM services,
coherent pause/drain, disk/server generations and atomic checked bundles.
Actual FPGA Cedar restore, original60ns timing and final repeated/stress gates
remain unfinished. No physical save/load or speed gain is claimed here.
