# Physical F10145 register-file state access

The next primitive needed for complete snapshots now passes isolated
save/continue/restore and routed timing. Production cells and the live
Native51 core are unchanged. This is not a board or machine snapshot.

`generate_f10145.py` clones the unchanged original16x4 cell, preserving the
exact write-window process under the actual host-clock advance. It exports
64 RAM bits and10 pending-write bits (we_d, captured address/data and CE).
Words0..15 access4-bit RAM; word16 accesses the10-bit window. Invalid5-bit
indices cannot mutate state. Entry/running/release requests are ignored;
back-to-back held requests receive one-cycle responses. The caller must
provide a machine-wide hold before claiming guest_held. Admin bookkeeping
is reconstructed through the completed local protocol, not guest state.

The original synthesis translate_off diagnostics are explicitly excluded:
106 bits including the plusarg flag. Fresh Verilator XML classifies every
original candidate:74 SAVE bits,106 simulator-only bits and continuous
expressions to recompute. These simulator-only candidates explain part of
the larger earlier native-machine review inventory; do not quote its raw
candidate total as physical snapshot storage. The generator rejects changed
physical declarations/processes and diagnostic references outside their
excluded region.11 damaged inventory/manifest/source controls reject,
including missing write history, swapped bits, RAM geometry and extra state.
The original cell is byte-identical to frozen Native49, SHA in the report.

`test_f10145_snapshot.py --mutations` passes35,407 host edges,7,987 backend
holds and326 state requests against the unchanged original cell AND an
independent write-window oracle. All74 physical bits are walked, RAM padding
is ignored, all invalid indices are rejected, candidate-only continuation
destroys state while the original remains stopped, and restore immediately
matches all physical state. Host-only release edges preserve the pending
write; the FIRST actual guest edge commits the correct captured address,
data and CE.20,000 additional randomized edges follow. Eight broken RTL
variants fail for intended reasons (ungated hold, lost address/CE, current
instead of captured write address, running/entry access, invalid alias and
lost window restore). Raw commands, generated sources and test logs are
preserved under simulation; compiled objects are excluded.

Matched isolated Quartus fits on misterubuntu completed at
/home/alans/mister/SnapshotF10145_20261019, handle97189 terminal. Both use the
MiSTer device,50MHz, opposite-edge launch/capture shell,2ns maximum/0ns minimum
virtual I/O and the existing DORADO_F10145_LOGIC setting. Baseline uses the
original continuously clocked cell; the independent RTL fixture establishes
equivalence to its stopped reference. All12 setup/hold/pulse-width checks per
variant/four conditions pass. No cuts or unconstrained I/O/clocks; no async
recovery/removal paths. Snapshot minimum slack+0.146ns.57->117 ALMs and
89->142 registers; zero block RAM in both. Counts include shell/pruning and
are NOT a projected whole-core snapshot cost. Four damaged source/SDC/QSF/
hold-report controls reject. QSF comparison permits only blank lines and
the exact Quartus17.0.2 version assignment; all other settings remain pinned.

The original fit reports/sources/logs are preserved byte-for-byte in
fit-evidence.tar.gz. To reproduce the audit, extract it into a new directory
and run `python3 fpga/mister/snapshot/audit_f10145.py DIRECTORY --output
/tmp/f10145-audit.json --negative-controls` from the matching checkout.
Simulation and classification commands are in the tracked tools and their
archived command.json files. No FPGA was reloaded for this experiment.

Next integrate/classify the remaining ContA cells and its six clock-history
registers, then other boards/devices/wrapper, physical SDRAM and ARM services.
Global freeze/drain, matching disk/server generation, atomic checksummed
bundles, full-core timing and actual Cedar save/load remain required. The
original60ns clock migration and final repeated/stress gates are unfinished.
