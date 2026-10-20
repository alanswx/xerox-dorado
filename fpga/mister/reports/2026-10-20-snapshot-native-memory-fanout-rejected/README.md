# Serial70 fanout256: applied, fits, rejected timing

Exact Serial69 RTL with two targeted network_step fanout assignments fits at
41,449 / 41,910 ALMs (461 free), 52,826 registers, 329 RAM blocks, 2,498,471
block-memory bits, 35 DSPs and 3 PLLs. The mapper reports limit256 and four
inserted replication cells; routed timing names network_step~SynDup_3 with
256 loads. The policy was applied, not silently ignored.

Setup fails at -0.561 ns (slow100C) and -0.328 ns (slow-40C). Hold passes all
four conditions, minimum +0.111 ns. SDRAM IO passes. Actual clocks and inherited
framework exceptions match Native59: zero illegal/unconstrained clocks, same
three input/fifty output exceptions. This is not full framework IO coverage.
All 304 crossings are retained, with sixteen setup-failing groups. The worst
paths now launch from a replicated step to DispY/ContA and the virtual memory
drained output. The fanout policy is rejected; do not try further undirected
limits/seeds or relax the original periods.

All 660 prepared inputs remain unchanged at terminal. Builder2602043 and every
project child are absent. No Quartus job remains live. Assembly is disabled;
no RBF/SOF or hardware deployment. The newly qualified network/parser pause is
NOT in this frozen candidate; foreign service permits remain virtual.
Terminal packet SHA256:
9dffd8993b2f049cdb3bf21112585bd2e60c86f08b35197892ded6fcecd1f56e.

Independent source/inverse review retains all 2,761 pins and unchanged clocks/
constraints. Fresh XML accounts for 2,094,301 required bits and includes the
120 task-written SDRAM bits, with no sequential exclusions. Source/state/
capacity/timing/crossing controls reject 6/15/11/6/3 damaged cases, including
wrong actual mapper settings and missing actual replication. The previous
Serial69 capacity reader still passes.

All 2,207 raw members (219,023,511 bytes;16,026,685 stored) read back losslessly.
Extracted tools replay source/fresh XML/capacity/timing rejection/crossing checks
and all controls. Restaging reproduces all 660 prepared input hashes at the
original destination without working HDL. Immutable parent receipts and frozen
HDL dependencies remain explicit. No full snapshot, restore or speed claim.
