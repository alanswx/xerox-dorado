# Native58 complete display state: full-core normal-operation qualification

Frozen Native57 plus the qualified DispY_field adapter and K1115A helper.
Ten shared helpers are byte-identical. The outer wrapper, clock constraints,
SDCs, memory/network policies and actual MemD input delay2 are unchanged.
All nine adapters have administrative inputs inactive. This is normal-operation
qualification; it does not enable whole-machine save/load or change speed.

The terminal build and independent read-only audit pass31summary checks,
180detailed path categories, four operating conditions and36board/corner
crossing checks. Minimum slack+0.116ns. No new exceptions, illegal clocks or
unconstrained clocks; framework I/O coverage is unchanged. All SDRAM and nine
adapter crossing categories are timed.27deliberately corrupted controls reject.

Resources:30,688ALMs/52,887registers/330M10Ks/35DSPs, with admin inactive.
These counts are not an enabled whole-machine snapshot resource estimate.

All inherited IM/IFUM/processor/MemC/MemD/MemX geometry, physical types and
required OLD_DATA behavior pass. All ten immutable MemX/DispY PROMs hash-match
both baseline and candidate. DispY's inactive admin fit prunes unused u_f04,
retains eight logical256x4FIFO rows and three1024x1word planes. Equivalent FIFO
replicas may share storage; word planes pack into one M10K. This differs from
the separately qualified enabled display adapter's nine full FIFO banks and
three independently writable word M10Ks. No enabled state is claimed here.

The first auditor stopped on a changed cache row block count. Inspection of
all14actual banks shows Quartus packs banks3/5 and8/10 into shared groups of
three M10Ks each. Per-row block counts repeat the shared locations: summing
those counts gives44, but the cache uses38unique M10Ks, versus40in Native57.
Every logical bank still has4096words, its original width and OLD_DATA. The
native-only checker verifies implementation geometry, physical type, exact
reviewed sharing, disjoint group locations and group capacity, preserving all
294,912bits. The historical isolated-MemD checker is unchanged. Five additional
cache corruptions (packing, implementation width, invalid/duplicate locations,
unreviewed sharing) reject. This is a packing change, not added cache capacity.

The second audit attempt passed the positive checks but its new fault injector
matched an earlier non-table instance line. The third restricts injection to
the actual physical RAM table; all positive and27negative checks pass. Both
earlier source versions and exact failures are preserved. No report, RTL,
constraint, fit or timing run was modified to accommodate those corrections.

RBF SHA256:
eebc0e1fa669ba48ae35ebfe7ba93161afa48eca40247ff79cee9e9853293ae2

Raw reports/logs/sources are gzip-compressed unchanged; exact executed builder,
audit dependencies and compiled-input hashes are retained. Native58 is not yet
loaded. Original-firmware/Cedar/input qualification follows separately. Complete
snapshots, original60ns cadence and final repeated-boot/stress gates remain open.
