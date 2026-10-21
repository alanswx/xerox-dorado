# Native SDRAM diagnostic: build passes, physical memory fails

The 501-source build passes all 31 device timing summaries, 196 original timing categories, 56 SDRAM-buffer categories, 8 first-error categories and 24 self-test categories over four conditions (minimum +0.116 ns). All 170 RAM entries and 165 parameter tables preserve their contracts; 332 M10Ks, 35 DSPs. Actual fit: 30,992 ALMs, 53,584 registers. Thirteen damaged-evidence controls reject. Guarded assembly and normal Main deployment pass.

The actual fitter inventory preserves 79 logical self-test registers (ready is named observation[0]) plus 12 named duplicates; the observer preserves all 82 logical registers plus a valid duplicate. The unchanged backend retains all 12 response bits plus one copy of bit 5. All original and new timing paths remain checked; these placement aliases do not change functional RTL. The supplementary inventory is read-only and terminal.

On MiSTer14 the first forward read of raw address 0 expects 0xA59 and returns 0xA58. Both metadata samples show phase 1, failed=true, ready=false. The diagnostic correctly prevents guest release; no firmware boot was attempted with this image. The neighboring address 1 carries pattern 0xA58, suggesting burst ordering/capture timing, but that single mismatch does not distinguish read alignment, write corruption or a data-bit fault. The full independent pin simulation passed; its difference from physical hardware remains unresolved.

Normal loader barriers preserved the closed BootFault2 pack unchanged, with 4,715 trace records, two barriers and no trace gaps. Current Main 11889 and daemon 11888 retain their previous binary hashes and are detached. Native V38/V39 simulations remain separate running prefixes; their input-phase scheduling counter writes are documented, and no CPU/IM/RAM/clock/cycle/IOCB state was written. A prepared logger-bound correction remains unloaded after automatic review rejected interrupting its debugger.

This archive contains terminal fit/source/report evidence, rejection controls, normal-load proof, actual memory failure and simulation/input prefixes. It excludes bitstreams, active packs and imported machine state.

Verify: `python3 tools/archive_r_front_ready_followup.py --verify fpga/mister/reports/2026-10-21-storage-selftest/native-fit`
