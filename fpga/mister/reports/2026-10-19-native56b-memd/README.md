# Native56b full-core MemD qualification

Frozen Native55 plus the complete actual native2 MemD state adapter. Eight
shared cell adapters are reused byte-identically; only the MemD board and
4096-word cache RAM helper are new. Two actual storage-input delay stages
and their edge history are preserved. Original first Native56 zero-delay
integration rejected before producing an RBF; its evidence is in the native2
component report. Same50MHz/SYSPER4/wrapper/SDCs/SDRAM/no raw cache/direct DDR,
arbitration+interleaving on, deferred reads off; all snapshot admin INACTIVE.

Build34403 and final read-only audit29951 TERMINAL.31summary/148detailed
checks across four operating conditions PASS, minimum slack+0.116ns.
No new timing exceptions, illegal/unconstrained clocks or framework I/O gaps.
All14MemD cache macros/physicalbanks preserve OLD_DATA;11IFUM groups/eight IM
banks preserve OLD_DATA;21MemC MLABs/21504bits and12processor MLABs/9216bits
retain their exact geometry.30,303ALMs/52,996registers/332M10Ks/35DSP; these are
normal-operation inactive-admin costs, not an enabled snapshot resource budget.

The first audit failed to import its unstaged helper dependencies. The next
passed positive checks but its physical MemD negative control changed the
first OLD_DATA row in the retained table, which belonged to the MiSTer scaler.
The final control changes a specifically named MemD bank row and rejects it.
All11source/QSF/hold/processor/IFU/MemC/MemD controls reject. No RTL/SDC refit
or report rewrite was used for either audit-driver correction. Exact earlier
logs and the earlier auditor source are preserved.

RBF SHA256: 0bbe092f1e10476dc3cc465418836f94a3e06c04ce5f08530c055fbfc52dd7cf.
Candidate downloaded to build/rtl-executive/native56b-candidate; NOT DEPLOYED.
Original-firmware/Cedar/input and measured-speed qualification remain next.
No whole-machine snapshot, enabled snapshot budget,60ns migration, speed gain,
three final boots or sustained combined stress is claimed.

Raw compiled sources, reports and logs are gzip-compressed unchanged. The
qualification and compiled-input hashes bind the exact fitted artifacts.
