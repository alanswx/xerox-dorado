# MC10135 state adapter — simulation and coverage

The original dual J-K cell, independent oracle, and state adapter agree across
92,195 host edges and all four independent PRECAP modes. All128 saved state
words and first-edge restore pass, with20,000 subsequent randomized cycles.
Seven intentionally broken RTL variants fail. Three altered original-source
controls and eight altered XML/schema controls reject. Fresh XML accounts
for all seven mutable bits: qa/qb, four J/K setup bits, shared clock history.

This is primitive simulation/classification qualification only. No isolated
routed timing, IFU integration, global barrier, physical snapshot or speed
claim. The original production cell and running FPGA remain unchanged.

Final executed sources and raw logs/XML are byte-preserved as gzip files.
Source hashes in simulation/results.json match the current sources.
